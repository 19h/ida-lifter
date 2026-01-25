#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <intel.hpp>
#include <bytes.hpp>
#include <name.hpp>
#include <typeinf.hpp>
#include "../common/warn_on.h"
#include "../plugin/component_registry.h"
#include "avx_types.h"
#include "avx_intrinsic.h"
#include "avx_helpers.h"
#include "avx_utils.h"
#include "avx_debug.h"
#include "handlers/avx_handlers.h"

#if IDA_SDK_VERSION >= 750

// Note: ZMM memory operands are now supported via emit_zmm_load/emit_zmm_store
// which bypass cdg.load_operand() and manually emit m_ldx/m_stx with UDT flags.
// The has_zmm_memory_operand check has been removed.

//-----------------------------------------------------------------------------
// The microcode filter
//-----------------------------------------------------------------------------
struct ida_local AVXLifter : microcode_filter_t {
    //----- match
    bool match(codegen_t &cdg) override {
        ea_t ea = cdg.insn.ea;
        uint16 it = cdg.insn.itype;

        // Skip k-register manipulation instructions (kmov, kunpck, etc.) - emit NOP
        // These instructions IDA can't handle natively
        if (it >= NN_kmovb && it <= NN_kunpckdq) {
            return true;
        }

        // Skip instructions with k-register as destination (compare-to-mask)
        // e.g., vcmpeqps k1, ymm0, ymm1
        if (is_mask_reg(cdg.insn.Op1)) {
            return true;
        }

        // Instructions with k-register masking (EVEX opmask in Op6)
        // e.g., vaddps zmm0{k1}, zmm1, zmm2
        // Allow masking only for handlers that implement masked intrinsics.
        if (has_opmask(cdg.insn)) {
            bool mask_ok = is_packed_math_insn(it) || is_fma_insn(it) || is_fmaddsub_insn(it) ||
                           is_bitwise_insn(it) || is_shift_insn(it) || is_var_shift_insn(it) ||
                           is_shuffle_insn(it) || is_perm_insn(it) || is_permutex_insn(it) ||
                           is_permutex2_insn(it) || is_align_insn(it) || is_blend_insn(it) ||
                           is_packed_compare_insn(it) || is_packed_int_compare_insn(it) ||
                           is_move_insn(it) || is_compress_insn(it) || is_expand_insn(it) ||
                           is_scatter_insn(it) || is_addsub_insn(it) ||
                           is_ternary_logic_insn(it) || is_conflict_insn(it);
            if (!mask_ok) {
                return false;
            }
        }

        // Skip YMM (256-bit) operations in 32-bit mode.
        //
        // While 32-bit x86 with AVX only has YMM0-YMM7 (no REX prefix), the issue is NOT
        // the register numbers - it's that IDA's Hex-Rays microcode verifier causes
        // INTERR 50920 ("Temporary registers cannot cross block boundaries") when we
        // emit 256-bit kregs and intrinsic calls in 32-bit mode.
        //
        // This appears to be a fundamental limitation in Hex-Rays' 32-bit microcode
        // representation - the verifier doesn't properly handle 256-bit temporaries.
        //
        // By returning false, we let IDA show these as __asm blocks, which preserves
        // the instruction visibility. XMM (128-bit) operations work fine in 32-bit.
        if (!inf_is_64bit()) {
            bool has_ymm = (cdg.insn.Op1.type == o_reg && cdg.insn.Op1.dtype == dt_byte32) ||
                           (cdg.insn.Op2.type == o_reg && cdg.insn.Op2.dtype == dt_byte32) ||
                           (cdg.insn.Op3.type == o_reg && cdg.insn.Op3.dtype == dt_byte32) ||
                           (cdg.insn.Op4.type == o_reg && cdg.insn.Op4.dtype == dt_byte32);
            if (has_ymm) {
                return false;
            }
        }

        bool m = is_compare_insn(it) || is_extract_insn(it) || is_conversion_insn(it) ||
                 is_move_insn(it) || is_scalar_move(it) || is_bitwise_insn(it) ||
                 is_math_insn(it) || is_scalar_math(it) || is_scalar_minmax(it) ||
                 is_broadcast_insn(it) || is_blend_insn(it) || is_packed_compare_insn(it) ||
                 is_packed_int_compare_insn(it) ||
                 is_maskmov_insn(it) || is_misc_insn(it) ||
                 is_horizontal_math(it) || is_dot_product(it) ||
                 is_approx_insn(it) || is_scalar_approx_insn(it) ||
                 is_round_insn(it) || is_scalar_round_insn(it) ||
                 is_gather_insn(it) || is_fma_insn(it) || is_vzeroupper(it) ||
                 is_extract_insert_insn(it) || is_movdup_insn(it) || is_unpack_insn(it) ||
                 is_addsub_insn(it) || is_vpbroadcast_d_q(it) || is_vperm2_insn(it) ||
                 is_permutex_insn(it) || is_permutex2_insn(it) || is_ternary_logic_insn(it) ||
                 is_compress_insn(it) || is_expand_insn(it) || is_scatter_insn(it) ||
                 is_conflict_insn(it) ||
                 is_phsub_insn(it) || is_pack_insn(it) || is_sad_insn(it) ||
                 is_fmaddsub_insn(it) || is_movmsk_insn(it) || is_movnt_insn(it) ||
                 is_vpbroadcast_b_w(it) || is_pinsert_insn(it) ||
                 is_pmovsx_insn(it) || is_pmovzx_insn(it) ||
                 is_byte_shift_insn(it) || is_punpck_insn(it) || is_extractps_insn(it) ||
                 is_insertps_insn(it) || it == NN_vsqrtsd;

        if (m) {
            DEBUG_LOG("%a: MATCH itype=%u", ea, it);
        }
        return m;
    }

    //----- apply
    merror_t apply(codegen_t &cdg) override {
        ea_t ea = cdg.insn.ea;
        uint16 it = cdg.insn.itype;

        TRACE_ENTER("apply");

        // Handle k-register manipulation instructions (kmov, kunpck, etc.) by emitting NOP
        if (it >= NN_kmovb && it <= NN_kunpckdq) {
            cdg.emit(m_nop, 0, 0, 0, 0, 0);
            return MERR_OK;
        }

        // Handle compare-to-mask instructions (k-register destination) by emitting NOP
        if (is_mask_reg(cdg.insn.Op1)) {
            cdg.emit(m_nop, 0, 0, 0, 0, 0);
            return MERR_OK;
        }

        if (try_convert_to_sse(cdg)) return MERR_INSN;

        // conversions
        if (it == NN_vcvtdq2ps) return handle_vcvtdq2ps(cdg);
        if (it == NN_vcvtsi2ss || it == NN_vcvtsi2sd) return handle_vcvtsi2fp(cdg);
        if (it == NN_vcvtps2pd) return handle_vcvtps2pd(cdg);
        if (it == NN_vcvtss2sd || it == NN_vcvtsd2ss) return handle_vcvtfp2fp(cdg);
        if (it == NN_vcvtpd2ps) return handle_vcvtpd2ps(cdg);
        if (it == NN_vcvttps2dq) return handle_vcvt_ps2dq(cdg, true);
        if (it == NN_vcvtps2dq) return handle_vcvt_ps2dq(cdg, false);
        if (it == NN_vcvttpd2dq) return handle_vcvt_pd2dq(cdg, true);
        if (it == NN_vcvtpd2dq) return handle_vcvt_pd2dq(cdg, false);
        if (it == NN_vcvtdq2pd) return handle_vcvtdq2pd(cdg);

        // SAD (sum of absolute differences)
        if (is_sad_insn(it)) return handle_vsad(cdg);

        // moves
        if (it == NN_vmovd) return handle_vmov(cdg, DWORD_SIZE);
        if (it == NN_vmovq) return handle_vmov(cdg, QWORD_SIZE);
        if (it == NN_vmovss) return handle_vmov_ss_sd(cdg, FLOAT_SIZE);
        if (it == NN_vmovsd) return handle_vmov_ss_sd(cdg, DOUBLE_SIZE);
        if (it == NN_vmovaps || it == NN_vmovups || it == NN_vmovdqa || it == NN_vmovdqu ||
            it == NN_vmovapd || it == NN_vmovupd)
            return handle_v_mov_ps_dq(cdg);

        // compress/expand (masked load/store)
        if (is_compress_insn(it)) return handle_v_compress(cdg);
        if (is_expand_insn(it)) return handle_v_expand(cdg);

        // bitwise (now full 128/256-bit via intrinsics)
        if (is_bitwise_insn(it)) return handle_v_bitwise(cdg);

        // scalar math (add/sub/mul/div)
        if (it == NN_vaddss || it == NN_vsubss || it == NN_vmulss || it == NN_vdivss)
            return handle_v_math_ss_sd(
                cdg, FLOAT_SIZE);
        if (it == NN_vaddsd || it == NN_vsubsd || it == NN_vmulsd || it == NN_vdivsd)
            return handle_v_math_ss_sd(
                cdg, DOUBLE_SIZE);

        // scalar min/max
        if (is_scalar_minmax(it)) return handle_v_minmax_ss_sd(cdg);

        // packed math (+ min/max + integer add/sub + integer mul)
        if (is_packed_math_insn(it)) return handle_v_math_p(cdg);

        // abs
        if (is_abs_insn(it)) return handle_v_abs(cdg);

        // sign
        if (is_sign_insn(it)) return handle_v_sign(cdg);

        // fma
        if (is_fma_insn(it)) return handle_v_fma(cdg);

        // shifts
        if (is_shift_insn(it)) return handle_v_shift(cdg);
        if (is_var_shift_insn(it)) return handle_v_var_shift(cdg);

        // shuffles, perms, align
        if (is_shuffle_insn(it)) return handle_v_shuffle_int(cdg);
        if (is_perm_insn(it)) return handle_v_perm_int(cdg);
        if (is_align_insn(it)) return handle_v_align(cdg);

        // gather
        if (is_gather_insn(it)) return handle_v_gather(cdg);

        // scatter
        if (is_scatter_insn(it)) return handle_v_scatter(cdg);

        // horizontal math
        if (is_horizontal_math(it)) return handle_v_hmath(cdg);

        // dot product
        if (is_dot_product(it)) return handle_v_dot(cdg);

        // approximations (rcp, rsqrt)
        if (is_approx_insn(it)) return handle_vrcp_rsqrt(cdg);

        // rounding
        if (is_round_insn(it)) return handle_vround(cdg);

        // broadcasts
        if (it == NN_vbroadcastss || it == NN_vbroadcastsd) return handle_vbroadcast_ss_sd(cdg);
        if (it == NN_vbroadcastf128) return handle_vbroadcastf128_fp(cdg);
        if (it == NN_vbroadcasti128) return handle_vbroadcasti128_int(cdg);

        // packed compares
        if (is_packed_compare_insn(it)) return handle_vcmp_ps_pd(cdg);
        if (is_packed_int_compare_insn(it)) return handle_vpcmp_int(cdg);

        // blend
        if (it == NN_vblendvps || it == NN_vblendvpd) return handle_vblendv_ps_pd(cdg);
        if (it == NN_vblendps || it == NN_vblendpd) return handle_vblend_imm_ps_pd(cdg);
        if (it == NN_vpblendd || it == NN_vpblendw || it == NN_vpblendvb) return handle_vblend_int(cdg);

        // maskmov
        if (is_maskmov_insn(it)) return handle_vmaskmov_ps_pd(cdg);

        // misc
        if (it == NN_vsqrtss) return handle_vsqrtss(cdg);
        if (it == NN_vsqrtps || it == NN_vsqrtpd) return handle_vsqrt_ps_pd(cdg);
        if (it == NN_vshufps) return handle_vshufps(cdg);
        if (it == NN_vshufpd) return handle_vshufpd(cdg);
        if (it == NN_vpermpd) return handle_vpermpd(cdg);
        if (it == NN_vzeroupper) return handle_vzeroupper_nop(cdg);

        // extract/insert
        if (it == NN_vextractf128 || it == NN_vextracti128) return handle_vextractf128(cdg);
        if (it == NN_vinsertf128 || it == NN_vinserti128) return handle_vinsertf128(cdg);

        // movdup
        if (it == NN_vmovshdup) return handle_vmovshdup(cdg);
        if (it == NN_vmovsldup) return handle_vmovsldup(cdg);
        if (it == NN_vmovddup) return handle_vmovddup(cdg);

        // unpack
        if (is_unpack_insn(it)) return handle_vunpck(cdg);

        // scalar approximations (rcp, rsqrt)
        if (is_scalar_approx_insn(it)) return handle_vrcp_rsqrt_ss(cdg);

        // scalar rounding
        if (is_scalar_round_insn(it)) return handle_vround_ss_sd(cdg);

        // scalar sqrt double
        if (it == NN_vsqrtsd) return handle_vsqrtsd(cdg);

        // addsub
        if (is_addsub_insn(it)) return handle_vaddsubps_pd(cdg);

        // broadcast d/q
        if (is_vpbroadcast_d_q(it)) return handle_vpbroadcast_d_q(cdg);

        // permute 128-bit lanes
        if (is_vperm2_insn(it)) return handle_vperm2f128_i128(cdg);

        // permute bytes/words (VBMI)
        if (is_permutex_insn(it)) return handle_v_permutex(cdg);

        // permute from two tables
        if (is_permutex2_insn(it)) return handle_v_permutex2(cdg);

        // ternary logic
        if (is_ternary_logic_insn(it)) return handle_v_ternary_logic(cdg);

        // conflict detection
        if (is_conflict_insn(it)) return handle_v_conflict(cdg);

        // horizontal subtract (including saturated)
        if (is_phsub_insn(it)) return handle_vphsub_sw(cdg);

        // pack
        if (is_pack_insn(it)) return handle_vpack(cdg);

        // fmaddsub/fmsubadd
        if (is_fmaddsub_insn(it)) return handle_vfmaddsub(cdg);

        // move mask to GPR
        if (is_movmsk_insn(it)) return handle_vmovmsk(cdg);

        // non-temporal store
        if (is_movnt_insn(it)) return handle_vmovnt(cdg);

        // broadcast byte/word
        if (is_vpbroadcast_b_w(it)) return handle_vpbroadcast_b_w(cdg);

        // insert into vector
        if (is_pinsert_insn(it)) return handle_vpinsert(cdg);

        // sign extend
        if (is_pmovsx_insn(it)) return handle_vpmovsx(cdg);

        // zero extend
        if (is_pmovzx_insn(it)) return handle_vpmovzx(cdg);

        // byte shift
        if (is_byte_shift_insn(it)) return handle_vpslldq_vpsrldq(cdg);

        // integer unpack
        if (is_punpck_insn(it)) return handle_vpunpck(cdg);

        // extract float to GPR/mem
        if (is_extractps_insn(it)) return handle_vextractps(cdg);

        // insert single float
        if (is_insertps_insn(it)) return handle_vinsertps(cdg);

        // Note: vptest is NOT lifted - it sets flags without a vector destination
        // Let IDA handle it natively

        return MERR_INSN;
    }
};

//-----------------------------------------------------------------------------
// Debug callback for printing disassembly and microcode
//-----------------------------------------------------------------------------
static bool g_callback_active = false;

static ssize_t idaapi hexrays_debug_callback(void *, hexrays_event_t event, va_list va) {
    // Safety check: don't process if we're shutting down
    if (!g_callback_active)
        return 0;

    switch (event) {
        case hxe_maturity: {
            // hxe_maturity arguments: (cfunc_t *cfunc, mba_maturity_t new_maturity)
            cfunc_t *cfunc = va_arg(va, cfunc_t *);
            mba_maturity_t new_maturity = va_argi(va, mba_maturity_t);

            // Safety check: ensure cfunc and mba are valid
            if (!cfunc || !cfunc->mba)
                break;

            mba_t *mba = cfunc->mba;

            DEBUG_LOG("hxe_maturity event: ea=%a maturity=%d", mba->entry_ea, new_maturity);

            // Print disassembly once when we first generate microcode
            if (new_maturity == MMAT_GENERATED) {
                DEBUG_LOG("Calling print_function_disassembly for %a", mba->entry_ea);
                print_function_disassembly(mba->entry_ea);
            }

            // Print microcode before our lifter processes it
            if (new_maturity == MMAT_PREOPTIMIZED) {
                DEBUG_LOG("Calling print_function_microcode BEFORE for %a", mba->entry_ea);
                print_function_microcode(mba, "BEFORE LIFTER");
            }

            // Print microcode after our lifter has processed it
            if (new_maturity == MMAT_LOCOPT) {
                DEBUG_LOG("Calling print_function_microcode AFTER for %a", mba->entry_ea);
                print_function_microcode(mba, "AFTER LIFTER");
            }
            break;
        }
        default:
            break;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Component glue
//-----------------------------------------------------------------------------
static AVXLifter *g_avx = nullptr;

static bool isMicroAvx_avail() {
    // Support both 32-bit (IA-32) and 64-bit (x86-64) binaries
    // In 32-bit mode, only YMM0-YMM7 are available (VEX.R/X/B ignored)
    // IDA's decoder handles this constraint - we just use the decoded register numbers
    if (PH.id != PLFM_386)
        return false;
    return true;
}

static bool isMicroAvx_active() { return g_avx != nullptr; }

extern "C" void set_debug_logging(bool enabled) {
    debug_logging_enabled = enabled;
    msg("[AVXLifter] Debug logging set to %s\n", enabled ? "TRUE" : "FALSE");

    // Also enable/disable debug printing
    ::set_debug_printing(enabled);
}

static void MicroAvx_init() {
    if (g_avx) return;

    // Enable debug logging temporarily for debugging
    debug_logging_enabled = true;
    ::set_debug_printing(true);

    msg("[AVXLifter] Initializing AVXLifter component\n");

    // Skip debug callback installation - it might cause issues
    // g_callback_active = true;
    // install_hexrays_callback(hexrays_debug_callback, nullptr);

    g_avx = new AVXLifter();
    install_microcode_filter(g_avx, true);
}

static void MicroAvx_done() {
    if (!g_avx) return;

    msg("[AVXLifter] Terminating AVXLifter component\n");

    // Disable callback first to prevent any callbacks during cleanup
    g_callback_active = false;

    // Remove microcode filter before removing callback
    install_microcode_filter(g_avx, false);

    // Remove debug callback
    remove_hexrays_callback(hexrays_debug_callback, nullptr);

    // Clean up lifter instance
    delete g_avx;
    g_avx = nullptr;
}

static const char avx_short_name[] = "avx";
REGISTER_COMPONENT(isMicroAvx_avail, isMicroAvx_active, MicroAvx_init, MicroAvx_done, nullptr, "AVXLifter",
                   avx_short_name, "AVXLifter")

#endif // IDA_SDK_VERSION >= 750
