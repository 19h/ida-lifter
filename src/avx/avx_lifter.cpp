#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <intel.hpp>
#include <idp.hpp>
#include <bytes.hpp>
#include <funcs.hpp>
#include <name.hpp>
#include <typeinf.hpp>
#include <ua.hpp>
#include <fpro.h>
#include "../common/warn_on.h"
#include <map>
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

static bool insn_uses_zmm(const insn_t &insn) {
    return is_zmm_reg(insn.Op1) || is_zmm_reg(insn.Op2) || is_zmm_reg(insn.Op3) ||
           is_zmm_reg(insn.Op4) || is_zmm_reg(insn.Op5) || is_zmm_reg(insn.Op6);
}

static ea_t get_direct_call_target(const insn_t &insn) {
    if (insn.itype != NN_call) return BADADDR;
    const op_t &op = insn.Op1;
    if (op.type == o_near || op.type == o_far) return op.addr;
    return BADADDR;
}

static bool function_uses_zmm(ea_t ea) {
    func_t *pfn = get_func(ea);
    if (pfn == nullptr) return false;

    for (ea_t item = pfn->start_ea; item < pfn->end_ea; item = next_head(item, pfn->end_ea)) {
        insn_t insn;
        if (decode_insn(&insn, item) <= 0) continue;
        if (insn_uses_zmm(insn)) return true;
    }
    return false;
}

static bool is_zmm_direct_call(const insn_t &insn) {
    ea_t target = get_direct_call_target(insn);
    return target != BADADDR && function_uses_zmm(target);
}

static void force_voidarg_type(ea_t target) {
    func_type_data_t ftd;
    ftd.rettype = tinfo_t(BT_VOID);
    ftd.set_cc(CM_CC_VOIDARG);

    tinfo_t tif;
    if (tif.create_func(ftd)) {
        apply_tinfo(target, tif, TINFO_DEFINITE);
    }
}

static merror_t handle_zmm_direct_call(codegen_t &cdg) {
    ea_t target = get_direct_call_target(cdg.insn);
    if (target == BADADDR) return MERR_INSN;

    force_voidarg_type(target);

    qstring name;
    if (!get_func_name(&name, target) || name.empty()) {
        name.sprnt("sub_%a", target);
    }

    AVXIntrinsic icall(&cdg, name.c_str());
    icall.emit_void();
    return MERR_OK;
}

//-----------------------------------------------------------------------------
// Debug: dump full microcode on demand (gated by AVX_DUMP_MC env var).
// We stash the mba during codegen so we can dump it from the hxe_interr hook,
// i.e. at the exact moment the verifier rejects our generated microcode.
//-----------------------------------------------------------------------------
static mba_t *g_cur_mba = nullptr;
static bool g_dump_mc = false;

static void dump_mba_full(mba_t *mba, const char *tag) {
    if (mba == nullptr) {
        msg("[MCDUMP:%s] mba=null\n", tag);
        return;
    }
    msg("\n========== MCDUMP [%s] ea=%a maturity=%d nblocks=%d ==========\n",
        tag, mba->entry_ea, (int) mba->maturity, mba->qty);
    for (int i = 0; i < mba->qty; i++) {
        mblock_t *blk = mba->get_mblock(i);
        if (blk == nullptr) continue;
        qstring succ;
        for (int s = 0; s < blk->nsucc(); s++) {
            char tmp[16];
            qsnprintf(tmp, sizeof(tmp), "%d ", blk->succ(s));
            succ.append(tmp);
        }
        msg("--- BLK %d serial=%d start=%a end=%a type=%d flags=%X succ=[ %s] ---\n",
            i, blk->serial, blk->start, blk->end, (int) blk->type, blk->flags, succ.c_str());
        if (blk->lists_ready()) {
            msg("    [use ] mustbuse=%s maybuse=%s\n", blk->mustbuse.dstr(), blk->maybuse.dstr());
            msg("    [def ] mustbdef=%s maybdef=%s\n", blk->mustbdef.dstr(), blk->maybdef.dstr());
            msg("    [dead] dead_at_start=%s dnu=%s\n", blk->dead_at_start.dstr(), blk->dnu.dstr());
        }
        for (minsn_t *m = blk->head; m != nullptr; m = m->next) {
            qstring s;
            m->print(&s, SHINS_VALNUM);
            tag_remove(&s, s);
            msg("  %a| %s\n", m->ea, s.c_str());
        }
    }
    msg("========== END MCDUMP [%s] ==========\n\n", tag);
}

//-----------------------------------------------------------------------------
// Supported-itype manifest (authoritative; shared with match()).
//
// avx_match_itype_core() is the *itype-only* portion of match()'s decision (the
// big classification OR). match() calls it and then applies operand-specific
// gates. Pulling it out lets the coverage-closure tooling enumerate exactly the
// set of itypes the lifter intends to handle, with no risk of drifting from the
// real dispatch logic.
//-----------------------------------------------------------------------------
static bool avx_match_itype_core(uint16 it) {
    return is_compare_insn(it) || is_extract_insn(it) || is_conversion_insn(it) ||
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
           is_permutex_insn(it) || is_permutex2_insn(it) || is_shuf_lane_insn(it) || is_ternary_logic_insn(it) ||
           it == NN_vblendmps || it == NN_vblendmpd || it == NN_vpblendmb ||
           it == NN_vpblendmw || it == NN_vpblendmd || it == NN_vpblendmq ||
           it == NN_vpbroadcastmb2q || it == NN_vpbroadcastmw2d ||
           it == NN_vbroadcastf32x2 || it == NN_vbroadcasti32x2 ||
           it == NN_vexp2ps || it == NN_vexp2pd ||
           it == NN_vrcp28ps || it == NN_vrcp28pd || it == NN_vrcp28ss || it == NN_vrcp28sd ||
           it == NN_vrsqrt28ps || it == NN_vrsqrt28pd || it == NN_vrsqrt28ss || it == NN_vrsqrt28sd ||
           it == NN_v4fmaddps || it == NN_v4fnmaddps || it == NN_v4fmaddss || it == NN_v4fnmaddss ||
           it == NN_vp4dpwssd || it == NN_vp4dpwssds ||
           it == NN_vcomish || it == NN_vucomish ||
           it == NN_vgatherpf0dps || it == NN_vgatherpf0qps || it == NN_vgatherpf0dpd || it == NN_vgatherpf0qpd ||
           it == NN_vscatterpf0dps || it == NN_vscatterpf0qps || it == NN_vscatterpf0dpd || it == NN_vscatterpf0qpd ||
           is_compress_insn(it) || is_expand_insn(it) || is_scatter_insn(it) ||
           is_rotate_insn(it) || is_var_rotate_insn(it) ||
           is_fp16_packed_math_insn(it) || is_fp16_scalar_math_insn(it) ||
           is_fp16_sqrt_insn(it) || is_fp16_fma_insn(it) ||
           is_fp16_fmaddsub_insn(it) || is_fp16_complex_insn(it) ||
           is_fp16_scalar_sqrt_insn(it) || is_fp16_scalar_misc_insn(it) ||
           is_shift_double_insn(it) || is_multishift_insn(it) ||
           is_getexp_insn(it) || is_getmant_insn(it) || is_fixupimm_insn(it) ||
           is_scalef_insn(it) || is_range_insn(it) || is_reduce_insn(it) ||
           is_mask_to_vec_insn(it) || is_conflict_insn(it) ||
           is_ifma_insn(it) || is_vnni_insn(it) || is_bf16_insn(it) ||
           is_popcnt_insn(it) || is_lzcnt_insn(it) || is_gfni_insn(it) ||
           is_pclmul_insn(it) || is_aes_insn(it) || is_sha_insn(it) || is_cache_ctrl_insn(it) ||
           is_fp16_move_insn(it) ||
           is_phsub_insn(it) || is_pack_insn(it) || is_sad_insn(it) ||
           is_ptest_insn(it) || is_pmaskmov_int_insn(it) ||
           is_fmaddsub_insn(it) || is_movmsk_insn(it) || is_movnt_insn(it) ||
           is_vpbroadcast_b_w(it) || is_pinsert_insn(it) ||
           is_pmovsx_insn(it) || is_pmovzx_insn(it) || is_pmovwb_insn(it) ||
           is_pmov_down_insn(it) || is_byte_shift_insn(it) || is_punpck_insn(it) || is_extractps_insn(it) ||
           is_insertps_insn(it) || it == NN_vsqrtsd;
}

// Instructions that write an opmask (k-register) destination and are routed to a
// real handler in apply(); match() admits these via the is_mask_reg(Op1) gate.
static bool is_cmp_to_mask_itype(uint16 it) {
    return it == NN_vcmpps || it == NN_vcmppd || it == NN_vcmpss || it == NN_vcmpsd ||
           it == NN_vcmpph || it == NN_vcmpsh ||
           it == NN_vpcmpb || it == NN_vpcmpw || it == NN_vpcmpd || it == NN_vpcmpq ||
           it == NN_vpcmpub || it == NN_vpcmpuw || it == NN_vpcmpud || it == NN_vpcmpuq ||
           it == NN_vptestmb || it == NN_vptestmw || it == NN_vptestmd || it == NN_vptestmq ||
           it == NN_vptestnmb || it == NN_vptestnmw || it == NN_vptestnmd || it == NN_vptestnmq ||
           it == NN_vpshufbitqmb ||
           it == NN_vfpclassps || it == NN_vfpclasspd || it == NN_vfpclassss ||
           it == NN_vfpclasssd || it == NN_vfpclassph || it == NN_vfpclasssh ||
           it == NN_vpmovb2m || it == NN_vpmovw2m || it == NN_vpmovd2m || it == NN_vpmovq2m ||
           it == NN_vp2intersectd || it == NN_vp2intersectq;
}

// Full set of itypes the lifter dispatches to a handler (any operand form).
static bool avx_supported_itype(uint16 it) {
    return avx_match_itype_core(it)
        || (it >= NN_kmovw && it <= NN_kunpckdq)  // k-register move/ALU/unpack
        || is_cmp_to_mask_itype(it);
}

//-----------------------------------------------------------------------------
// Coverage-closure instrumentation (gated by env vars; zero overhead otherwise).
//   AVX_COV=<file>          append observed per-itype coverage at plugin term
//   AVX_COV_MANIFEST=<file> dump the supported-itype manifest once at init
// A supported itype that the corpus never feeds a memory *source* (and that is
// not in the harness allowlist of register/imm-only forms) is a coverage hole.
//-----------------------------------------------------------------------------
static bool g_cov = false;
static qstring g_cov_path;
static std::map<uint16, uint8> g_cov_seen;  // itype -> bit0:seen  bit1:seen w/ src-mem

static bool insn_has_src_mem(const insn_t &insn) {
    return is_mem_op(insn.Op2) || is_mem_op(insn.Op3) || is_mem_op(insn.Op4);
}

static void cov_record(const insn_t &insn) {
    uint8 b = 1;
    if (insn_has_src_mem(insn))
        b |= 2;
    g_cov_seen[insn.itype] |= b;
}

static void cov_flush() {
    if (!g_cov || g_cov_seen.empty())
        return;
    FILE *fp = qfopen(g_cov_path.c_str(), "a");
    if (fp == nullptr)
        return;
    for (const auto &kv : g_cov_seen) {
        const char *nm = PH.get_canon_mnem(kv.first);
        qfprintf(fp, "%u\t%s\t%u\n", (unsigned) kv.first, nm ? nm : "?", (unsigned) kv.second);
    }
    qfclose(fp);
    g_cov_seen.clear();
}

static void cov_dump_manifest() {
    qstring path;
    if (!qgetenv("AVX_COV_MANIFEST", &path) || path.empty())
        return;
    FILE *fp = qfopen(path.c_str(), "w");
    if (fp == nullptr)
        return;
    for (int it = PH.instruc_start; it < PH.instruc_end; it++) {
        if (!avx_supported_itype((uint16) it))
            continue;
        const char *nm = PH.get_canon_mnem((uint16) it);
        qfprintf(fp, "%d\t%s\n", it, nm ? nm : "?");
    }
    qfclose(fp);
}

//-----------------------------------------------------------------------------
// The microcode filter
//-----------------------------------------------------------------------------
struct ida_local AVXLifter : microcode_filter_t {
    //----- match
    bool match(codegen_t &cdg) override {
        ea_t ea = cdg.insn.ea;
        uint16 it = cdg.insn.itype;

        // Segment-overridden (fs/gs) vector memory operands are not safely
        // modelable by our operand-load path: emitting a ZMM/UDT-sized ldx
        // against a segment base crashes microcode generation (INTERR 50757).
        // Decline so IDA renders the instruction as __asm instead of aborting
        // the whole function.
        if (cdg.insn.segpref == R_fs || cdg.insn.segpref == R_gs) {
            return false;
        }

        if (is_zmm_direct_call(cdg.insn)) {
            return true;
        }

        // Skip k-register manipulation instructions (kmov/kunpck) - emit NOP
        // These instructions IDA can't handle natively
        if (it >= NN_kmovw && it <= NN_kunpckdq) {
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
                           is_ifma_insn(it) || is_vnni_insn(it) || is_bf16_insn(it) ||
                            is_fp16_packed_math_insn(it) || is_fp16_scalar_math_insn(it) ||
                            is_fp16_sqrt_insn(it) || is_fp16_fma_insn(it) ||
                            is_fp16_fmaddsub_insn(it) || is_fp16_complex_insn(it) ||
                            is_fp16_scalar_sqrt_insn(it) || is_fp16_scalar_misc_insn(it) ||
                            is_bitwise_insn(it) || is_shift_insn(it) || is_var_shift_insn(it) ||
                            is_shift_double_insn(it) || is_multishift_insn(it) ||
                            is_rotate_insn(it) || is_var_rotate_insn(it) ||
                            is_shuffle_insn(it) || is_shuf_lane_insn(it) || is_perm_insn(it) || is_permutex_insn(it) ||
                            is_permutex2_insn(it) || is_align_insn(it) || is_blend_insn(it) ||
                            it == NN_vbroadcastss || it == NN_vbroadcastsd ||
                            it == NN_vblendmps || it == NN_vblendmpd || it == NN_vpblendmb ||
                            it == NN_vpblendmw || it == NN_vpblendmd || it == NN_vpblendmq ||
                            is_packed_compare_insn(it) || is_packed_int_compare_insn(it) ||
                            is_scalar_minmax(it) || is_scalar_move(it) ||
                            is_move_insn(it) || is_compress_insn(it) || is_expand_insn(it) ||
                            is_gather_insn(it) || is_scatter_insn(it) || is_addsub_insn(it) ||
                            is_approx_insn(it) || is_round_insn(it) ||
                            is_scalar_approx_insn(it) || is_scalar_round_insn(it) ||
                            is_getexp_insn(it) || is_getmant_insn(it) || is_fixupimm_insn(it) ||
                            is_scalef_insn(it) || is_range_insn(it) || is_reduce_insn(it) ||
                            is_mask_to_vec_insn(it) || is_popcnt_insn(it) || is_lzcnt_insn(it) ||
                            is_gfni_insn(it) || is_fp16_move_insn(it) ||
                            is_ternary_logic_insn(it) || is_conflict_insn(it) ||
                            it == NN_vcvtss2sd || it == NN_vcvtsd2ss;
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

        bool m = avx_match_itype_core(it);

        if (m) {
            DEBUG_LOG("%a: MATCH itype=%u", ea, it);
        }
        return m;
    }

    //----- apply
    merror_t apply(codegen_t &cdg) override {
        ea_t ea = cdg.insn.ea;
        uint16 it = cdg.insn.itype;

        if (g_dump_mc) g_cur_mba = cdg.mba;  // stash for the AVX_DUMP_MC interr dumper
        if (g_cov) cov_record(cdg.insn);     // coverage-closure accounting

        TRACE_ENTER("apply");

        if (is_zmm_direct_call(cdg.insn)) {
            return handle_zmm_direct_call(cdg);
        }

        // Compare / test / classify into an opmask (k-register destination).
        if (it == NN_vcmpps || it == NN_vcmppd || it == NN_vcmpss || it == NN_vcmpsd ||
            it == NN_vcmpph || it == NN_vcmpsh ||
            it == NN_vpcmpb || it == NN_vpcmpw || it == NN_vpcmpd || it == NN_vpcmpq ||
            it == NN_vpcmpub || it == NN_vpcmpuw || it == NN_vpcmpud || it == NN_vpcmpuq)
            return handle_v_cmp_to_mask(cdg);
        if (it == NN_vptestmb || it == NN_vptestmw || it == NN_vptestmd || it == NN_vptestmq ||
            it == NN_vptestnmb || it == NN_vptestnmw || it == NN_vptestnmd || it == NN_vptestnmq ||
            it == NN_vpshufbitqmb)
            return handle_v_2src_to_mask(cdg);
        if (it == NN_vfpclassps || it == NN_vfpclasspd || it == NN_vfpclassss ||
            it == NN_vfpclasssd || it == NN_vfpclassph || it == NN_vfpclasssh)
            return handle_v_fpclass_to_mask(cdg);
        if (it == NN_vpmovb2m || it == NN_vpmovw2m || it == NN_vpmovd2m || it == NN_vpmovq2m)
            return handle_v_movx2m(cdg);
        if (it == NN_vp2intersectd || it == NN_vp2intersectq)
            return handle_v_p2intersect(cdg);

        // k-register move / ALU / unpack -> modeled via __readmask/__writemask.
        if (it >= NN_kmovw && it <= NN_kmovd)
            return handle_kmov(cdg);
        if (it >= NN_kaddw && it <= NN_kxord &&
            it != NN_kortestw && it != NN_kortestb && it != NN_kortestq && it != NN_kortestd &&
            it != NN_ktestw && it != NN_ktestb && it != NN_ktestq && it != NN_ktestd)
            return handle_k_alu(cdg);

        // kortest/ktest set EFLAGS only — emit NOP (flag effect not modeled).
        if (it >= NN_kmovw && it <= NN_kunpckdq) {
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
        if (it == NN_vcvtps2udq) return handle_vcvt_ps2udq(cdg, false);
        if (it == NN_vcvttps2udq) return handle_vcvt_ps2udq(cdg, true);
        if (it == NN_vcvtpd2udq) return handle_vcvt_pd2udq(cdg, false);
        if (it == NN_vcvttpd2udq) return handle_vcvt_pd2udq(cdg, true);
        if (it == NN_vcvtudq2ps) return handle_vcvt_udq2ps(cdg);
        if (it == NN_vcvtudq2pd) return handle_vcvt_udq2pd(cdg);
        if (it == NN_vcvtpd2qq) return handle_vcvt_pd2qq(cdg, false, false);
        if (it == NN_vcvtpd2uqq) return handle_vcvt_pd2qq(cdg, false, true);
        if (it == NN_vcvttpd2qq) return handle_vcvt_pd2qq(cdg, true, false);
        if (it == NN_vcvttpd2uqq) return handle_vcvt_pd2qq(cdg, true, true);
        if (it == NN_vcvtps2qq) return handle_vcvt_ps2qq(cdg, false, false);
        if (it == NN_vcvtps2uqq) return handle_vcvt_ps2qq(cdg, false, true);
        if (it == NN_vcvttps2qq) return handle_vcvt_ps2qq(cdg, true, false);
        if (it == NN_vcvttps2uqq) return handle_vcvt_ps2qq(cdg, true, true);
        if (it == NN_vcvtqq2pd) return handle_vcvt_qq2fp(cdg, true, false);
        if (it == NN_vcvtqq2ps) return handle_vcvt_qq2fp(cdg, false, false);
        if (it == NN_vcvtuqq2pd) return handle_vcvt_qq2fp(cdg, true, true);
        if (it == NN_vcvtuqq2ps) return handle_vcvt_qq2fp(cdg, false, true);
        if (it == NN_vcvtpd2ph || it == NN_vcvtph2pd || it == NN_vcvtph2psx || it == NN_vcvtps2phx ||
            it == NN_vcvtph2w || it == NN_vcvttph2w || it == NN_vcvtph2uw || it == NN_vcvttph2uw ||
            it == NN_vcvtw2ph || it == NN_vcvtuw2ph ||
            it == NN_vcvtdq2ph || it == NN_vcvtudq2ph || it == NN_vcvtqq2ph || it == NN_vcvtuqq2ph ||
            it == NN_vcvtph2dq || it == NN_vcvttph2dq || it == NN_vcvtph2udq || it == NN_vcvttph2udq ||
            it == NN_vcvtph2qq || it == NN_vcvttph2qq || it == NN_vcvtph2uqq || it == NN_vcvttph2uqq ||
            it == NN_vcvtph2ps || it == NN_vcvtps2ph)
            return handle_vcvt_fp16(cdg);
        if (it == NN_vcvtsh2si || it == NN_vcvttsh2si || it == NN_vcvtsh2usi || it == NN_vcvttsh2usi ||
            it == NN_vcvtsd2usi || it == NN_vcvttsd2usi || it == NN_vcvtss2usi || it == NN_vcvttss2usi ||
            it == NN_vcvtusi2sd || it == NN_vcvtusi2ss || it == NN_vcvtsi2sh || it == NN_vcvtusi2sh ||
            it == NN_vcvtsd2sh || it == NN_vcvtsh2sd || it == NN_vcvtss2sh || it == NN_vcvtsh2ss)
            return handle_vcvt_scalar_ext(cdg);
        if (it == NN_vldmxcsr || it == NN_vstmxcsr) return handle_vmxcsr(cdg);

        // SAD (sum of absolute differences)
        if (is_sad_insn(it)) return handle_vsad(cdg);

        // moves
        if (it == NN_vmovd) return handle_vmov(cdg, DWORD_SIZE);
        if (it == NN_vmovq) return handle_vmov(cdg, QWORD_SIZE);
        if (it == NN_vmovss) return handle_vmov_ss_sd(cdg, FLOAT_SIZE);
        if (it == NN_vmovsd) return handle_vmov_ss_sd(cdg, DOUBLE_SIZE);
        if (it == NN_vmovsh) return handle_vmov_sh(cdg);
        if (it == NN_vmovw) return handle_vmovw(cdg);
        if (it == NN_vmovaps || it == NN_vmovups || it == NN_vmovdqa || it == NN_vmovdqu ||
            it == NN_vmovapd || it == NN_vmovupd ||
            it == NN_vmovdqa32 || it == NN_vmovdqa64 ||
            it == NN_vmovdqu8 || it == NN_vmovdqu16 || it == NN_vmovdqu32 || it == NN_vmovdqu64)
            return handle_v_mov_ps_dq(cdg);

        // compress/expand (masked load/store)
        if (is_compress_insn(it)) return handle_v_compress(cdg);
        if (is_expand_insn(it)) return handle_v_expand(cdg);

        // bitwise (now full 128/256-bit via intrinsics)
        if (is_bitwise_insn(it)) return handle_v_bitwise(cdg);

        // popcount/lzcnt
        if (is_popcnt_insn(it)) return handle_v_popcnt(cdg);
        if (is_lzcnt_insn(it)) return handle_v_lzcnt(cdg);

        // GFNI
        if (is_gfni_insn(it)) return handle_v_gfni(cdg);

        // carryless multiply
        if (is_pclmul_insn(it)) return handle_v_pclmul(cdg);

        // AES
        if (is_aes_insn(it)) return handle_v_aes(cdg);

        // SHA
        if (is_sha_insn(it)) return handle_v_sha(cdg);

        // Cache control
        if (is_cache_ctrl_insn(it)) return handle_cache_ctrl(cdg);

        // scalar math (add/sub/mul/div)
        if (it == NN_vaddss || it == NN_vsubss || it == NN_vmulss || it == NN_vdivss)
            return handle_v_math_ss_sd(
                cdg, FLOAT_SIZE);
        if (it == NN_vaddsd || it == NN_vsubsd || it == NN_vmulsd || it == NN_vdivsd)
            return handle_v_math_ss_sd(
                cdg, DOUBLE_SIZE);
        if (is_fp16_scalar_math_insn(it)) return handle_v_math_sh(cdg);

        // scalar min/max
        if (is_scalar_minmax(it)) return handle_v_minmax_ss_sd(cdg);

        // packed math (+ min/max + integer add/sub + integer mul)
        if (is_packed_math_insn(it)) return handle_v_math_p(cdg);
        if (is_fp16_packed_math_insn(it)) return handle_v_math_ph(cdg);

        // abs
        if (is_abs_insn(it)) return handle_v_abs(cdg);

        // sign
        if (is_sign_insn(it)) return handle_v_sign(cdg);

        // fma
        if (is_fma_insn(it)) return handle_v_fma(cdg);
        if (is_fp16_fma_insn(it)) return handle_v_fma_ph(cdg);

        // fp16 complex ops
        if (is_fp16_complex_insn(it)) return handle_v_complex_ph(cdg);

        // IFMA / VNNI / BF16
        if (is_ifma_insn(it)) return handle_v_ifma(cdg);
        if (is_vnni_insn(it)) return handle_v_vnni(cdg);
        if (is_bf16_insn(it)) return handle_v_bf16(cdg);

        // shifts
        if (is_shift_insn(it)) return handle_v_shift(cdg);
        if (is_var_shift_insn(it)) return handle_v_var_shift(cdg);
        if (is_rotate_insn(it)) return handle_v_rotate(cdg);
        if (is_var_rotate_insn(it)) return handle_v_var_rotate(cdg);
        if (is_shift_double_insn(it)) return handle_v_shift_double(cdg);
        if (is_multishift_insn(it)) return handle_v_multishift(cdg);

        // shuffles, perms, align
        if (is_shuffle_insn(it)) return handle_v_shuffle_int(cdg);
        if (is_shuf_lane_insn(it)) return handle_v_shuf_lane(cdg);
        if (it == NN_vblendmps || it == NN_vblendmpd || it == NN_vpblendmb ||
            it == NN_vpblendmw || it == NN_vpblendmd || it == NN_vpblendmq)
            return handle_v_blendm(cdg);
        if (it == NN_vpbroadcastmb2q || it == NN_vpbroadcastmw2d)
            return handle_v_broadcastm(cdg);
        if (it == NN_vbroadcastf32x2 || it == NN_vbroadcasti32x2)
            return handle_v_broadcast_x2(cdg);
        if (it == NN_vexp2ps || it == NN_vexp2pd ||
            it == NN_vrcp28ps || it == NN_vrcp28pd || it == NN_vrcp28ss || it == NN_vrcp28sd ||
            it == NN_vrsqrt28ps || it == NN_vrsqrt28pd || it == NN_vrsqrt28ss || it == NN_vrsqrt28sd)
            return handle_v_er(cdg);
        if (it == NN_v4fmaddps || it == NN_v4fnmaddps || it == NN_v4fmaddss ||
            it == NN_v4fnmaddss || it == NN_vp4dpwssd || it == NN_vp4dpwssds)
            return handle_v_4fma(cdg);
        if (it == NN_vcomish || it == NN_vucomish)
            return handle_v_comish(cdg);
        if (it == NN_vgatherpf0dps || it == NN_vgatherpf0qps || it == NN_vgatherpf0dpd ||
            it == NN_vgatherpf0qpd || it == NN_vscatterpf0dps || it == NN_vscatterpf0qps ||
            it == NN_vscatterpf0dpd || it == NN_vscatterpf0qpd)
            return handle_v_prefetch_gs(cdg);
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

        // fp16 sqrt
        if (is_fp16_sqrt_insn(it)) return handle_v_sqrt_ph(cdg);
        if (is_fp16_scalar_misc_insn(it)) return handle_v_fp16_scalar_misc(cdg);

        // getexp/getmant/fixupimm/scalef/range/reduce
        if (is_getexp_insn(it)) return handle_v_getexp(cdg);
        if (is_getmant_insn(it)) return handle_v_getmant(cdg);
        if (is_fixupimm_insn(it)) return handle_v_fixupimm(cdg);
        if (is_scalef_insn(it)) return handle_v_scalef(cdg);
        if (is_range_insn(it)) return handle_v_range(cdg);
        if (is_reduce_insn(it)) return handle_v_reduce(cdg);

        // broadcasts
        if (it == NN_vbroadcastss || it == NN_vbroadcastsd) return handle_vbroadcast_ss_sd(cdg);
        if (it == NN_vbroadcastf128) return handle_vbroadcastf128_fp(cdg);
        if (it == NN_vbroadcasti128) return handle_vbroadcasti128_int(cdg);
        if (it == NN_vbroadcastf32x4 || it == NN_vbroadcastf64x4 ||
            it == NN_vbroadcasti32x4 || it == NN_vbroadcasti64x4)
            return handle_vbroadcast_x4(cdg);

        // packed compares
        if (is_packed_compare_insn(it)) return handle_vcmp_ps_pd(cdg);
        if (is_packed_int_compare_insn(it)) return handle_vpcmp_int(cdg);

        // blend
        if (it == NN_vblendvps || it == NN_vblendvpd) return handle_vblendv_ps_pd(cdg);
        if (it == NN_vblendps || it == NN_vblendpd) return handle_vblend_imm_ps_pd(cdg);
        if (it == NN_vpblendd || it == NN_vpblendw || it == NN_vpblendvb) return handle_vblend_int(cdg);

        // maskmov
        if (is_maskmov_insn(it)) return handle_vmaskmov_ps_pd(cdg);
        if (is_pmaskmov_int_insn(it)) return handle_vpmaskmov_int(cdg);

        // misc
        if (it == NN_vsqrtss) return handle_vsqrtss(cdg);
        if (it == NN_vsqrtsh) return handle_vsqrt_sh(cdg);
        if (it == NN_vsqrtps || it == NN_vsqrtpd) return handle_vsqrt_ps_pd(cdg);
        if (it == NN_vshufps) return handle_vshufps(cdg);
        if (it == NN_vshufpd) return handle_vshufpd(cdg);
        if (it == NN_vpermpd) return handle_vpermpd(cdg);
        if (it == NN_vmovlhps) return handle_vmovlhps(cdg);
        if (it == NN_vmovhlps) return handle_vmovhlps(cdg);
        if (it == NN_vmovhps || it == NN_vmovlps || it == NN_vmovhpd || it == NN_vmovlpd)
            return handle_vmovl_h_ps_pd(cdg);
        if (it == NN_vzeroupper) return handle_vzeroupper_nop(cdg);
        if (it == NN_vzeroall) return handle_vzeroall(cdg);
        if (it == NN_vphminposuw) return handle_vphminposuw(cdg);
        if (is_vtest_insn(it)) return handle_vtest_ps_pd(cdg);

        // extract/insert
        if (it == NN_vextractf128 || it == NN_vextracti128 ||
            it == NN_vextracti32x4 || it == NN_vextracti32x8 || it == NN_vextracti64x4 ||
            it == NN_vextractf32x4 || it == NN_vextractf32x8 ||
            it == NN_vextractf64x2 || it == NN_vextractf64x4 || it == NN_vextracti64x2)
            return handle_vextractf128(cdg);
        if (it == NN_vinsertf128 || it == NN_vinserti128 ||
            it == NN_vinserti32x4 || it == NN_vinserti32x8 || it == NN_vinserti64x4 ||
            it == NN_vinsertf32x4 || it == NN_vinsertf64x4 ||
            it == NN_vinsertf32x8 || it == NN_vinsertf64x2 || it == NN_vinserti64x2)
            return handle_vinsertf128(cdg);

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

        // mask to vector
        if (is_mask_to_vec_insn(it)) return handle_v_mask_to_vec(cdg);

        // broadcast byte/word
        if (is_vpbroadcast_b_w(it)) return handle_vpbroadcast_b_w(cdg);

        // insert into vector
        if (is_pinsert_insn(it)) return handle_vpinsert(cdg);

        // sign extend
        if (is_pmovsx_insn(it)) return handle_vpmovsx(cdg);

        // zero extend
        if (is_pmovzx_insn(it)) return handle_vpmovzx(cdg);

        // narrow to bytes
        if (is_pmovwb_insn(it)) return handle_vpmovwb(cdg);

        // down-convert packed integers
        if (is_pmov_down_insn(it)) return handle_vpmov_down(cdg);

        // byte shift
        if (is_byte_shift_insn(it)) return handle_vpslldq_vpsrldq(cdg);

        // integer unpack
        if (is_punpck_insn(it)) return handle_vpunpck(cdg);

        // extract float to GPR/mem
        if (is_extractps_insn(it)) return handle_vextractps(cdg);

        // insert single float
        if (is_insertps_insn(it)) return handle_vinsertps(cdg);

        // flag-setting vector tests
        if (is_ptest_insn(it)) return handle_vptest(cdg);

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
        case hxe_microcode: {
            // Microcode has just been generated (MMAT_GENERATED). Dump it.
            mba_t *mba = va_arg(va, mba_t *);
            g_cur_mba = mba;
            if (g_dump_mc)
                dump_mba_full(mba, "GENERATED");
            break;
        }
        case hxe_interr: {
            // The verifier (or anything else) raised an internal error. Dump the
            // microcode we were last building so we can see the bad construct.
            int errcode = va_arg(va, int);
            if (g_dump_mc) {
                msg("[MCDUMP] hxe_interr errcode=%d -- dumping last mba\n", errcode);
                dump_mba_full(g_cur_mba, "AT-INTERR");
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

    // Opt-in microcode dumper for debugging verifier INTERRs. When AVX_DUMP_MC
    // is set we install a callback that dumps the full microcode at generation
    // time and at the moment of any internal error (e.g. INTERR 50920).
    g_dump_mc = qgetenv("AVX_DUMP_MC", nullptr);
    if (g_dump_mc) {
        g_callback_active = true;
        install_hexrays_callback(hexrays_debug_callback, nullptr);
        msg("[AVXLifter] AVX_DUMP_MC set: microcode dumper installed\n");
    }

    // Coverage-closure: record observed per-itype memory coverage / dump manifest.
    g_cov = qgetenv("AVX_COV", &g_cov_path) && !g_cov_path.empty();
    if (g_cov)
        msg("[AVXLifter] AVX_COV set: recording itype coverage -> %s\n", g_cov_path.c_str());
    cov_dump_manifest();

    g_avx = new AVXLifter();
    install_microcode_filter(g_avx, true);
}

static void MicroAvx_done() {
    if (!g_avx) return;

    cov_flush();  // append this run's observed coverage before teardown

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
