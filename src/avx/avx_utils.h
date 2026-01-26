/*
AVX Utility Functions and Classification
*/

#pragma once

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <intel.hpp>
#include "../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

#include "avx_types.h"
#include "avx_helpers.h"

// Helper to load operand into a register
// Note: We do NOT free kregs because the emitted microcode references them.
// IDA's microcode engine manages the lifetime of values in emitted instructions.
struct AvxOpLoader {
    codegen_t &cdg;
    mreg_t reg;
    bool is_kreg;
    int size;

    AvxOpLoader(codegen_t &c, int op_idx, const op_t &op) : cdg(c) {
        if (is_mem_op(op)) {
            size = get_dtype_size(op.dtype);
            // Use load_operand_udt for large operands (> 8 bytes) to set UDT flag
            // This is required for AVX-512 64-byte operands to pass the verifier
            reg = load_operand_udt(cdg, op_idx, size);
            is_kreg = true;
        } else {
            reg = reg2mreg(op.reg);
            is_kreg = false;
            size = 0;
        }
    }

    // Do not free kregs - they are used in emitted microcode
    ~AvxOpLoader() = default;

    // Disable copy
    AvxOpLoader(const AvxOpLoader &) = delete;

    AvxOpLoader &operator=(const AvxOpLoader &) = delete;

    operator mreg_t() const { return reg; }
};

int get_op_size(const insn_t &insn);

// Naming and Type helpers
qstring make_intrinsic_name(const char *fmt, int op_size);

tinfo_t get_type_robust(int op_size, bool is_int = false, bool is_double = false);

// SSE Conversion
bool try_convert_to_sse(codegen_t &cdg);

// Instruction Classification
bool is_compare_insn(uint16 it);

bool is_extract_insn(uint16 it);

bool is_conversion_insn(uint16 it);

bool is_sad_insn(uint16 it);

bool is_move_insn(uint16 it);

bool is_bitwise_insn(uint16 it);

bool is_scalar_minmax(uint16 it);

bool is_scalar_math(uint16 it);

bool is_scalar_move(uint16 it);

bool is_vzeroupper(uint16 it);

bool is_packed_minmax_fp(uint16 it);

bool is_packed_minmax_int(uint16 it);

bool is_int_mul(uint16 it);

bool is_avg_insn(uint16 it);

bool is_abs_insn(uint16 it);

bool is_sign_insn(uint16 it);

bool is_shift_insn(uint16 it);

bool is_var_shift_insn(uint16 it);

bool is_rotate_insn(uint16 it);

bool is_var_rotate_insn(uint16 it);

bool is_shift_double_insn(uint16 it);

bool is_multishift_insn(uint16 it);

bool is_fp16_packed_math_insn(uint16 it);

bool is_fp16_scalar_math_insn(uint16 it);

bool is_fp16_sqrt_insn(uint16 it);

bool is_fp16_fma_insn(uint16 it);

bool is_fp16_fmaddsub_insn(uint16 it);

bool is_fp16_complex_insn(uint16 it);

bool is_fp16_scalar_sqrt_insn(uint16 it);

bool is_shuffle_insn(uint16 it);

bool is_perm_insn(uint16 it);

bool is_permutex_insn(uint16 it);

bool is_permutex2_insn(uint16 it);

bool is_align_insn(uint16 it);

bool is_gather_insn(uint16 it);

bool is_scatter_insn(uint16 it);

bool is_compress_insn(uint16 it);

bool is_expand_insn(uint16 it);

bool is_fma_insn(uint16 it);

bool is_ifma_insn(uint16 it);

bool is_vnni_insn(uint16 it);

bool is_bf16_insn(uint16 it);

bool is_fp16_move_insn(uint16 it);

bool is_pmovwb_insn(uint16 it);

bool is_pmov_down_insn(uint16 it);

bool is_ternary_logic_insn(uint16 it);

bool is_conflict_insn(uint16 it);

bool is_popcnt_insn(uint16 it);

bool is_lzcnt_insn(uint16 it);

bool is_gfni_insn(uint16 it);

bool is_pclmul_insn(uint16 it);

bool is_aes_insn(uint16 it);

bool is_math_insn(uint16 it);

bool is_packed_math_insn(uint16 it);

bool is_broadcast_insn(uint16 it);

bool is_misc_insn(uint16 it);

bool is_blend_insn(uint16 it);

bool is_maskmov_insn(uint16 it);

bool is_packed_compare_insn(uint16 it);

bool is_packed_int_compare_insn(uint16 it);

bool is_horizontal_math(uint16 it);

bool is_dot_product(uint16 it);

bool is_approx_insn(uint16 it);

bool is_scalar_approx_insn(uint16 it);

bool is_round_insn(uint16 it);

bool is_scalar_round_insn(uint16 it);

bool is_getexp_insn(uint16 it);

bool is_getmant_insn(uint16 it);

bool is_fixupimm_insn(uint16 it);

bool is_scalef_insn(uint16 it);

bool is_range_insn(uint16 it);

bool is_reduce_insn(uint16 it);

bool is_addsub_insn(uint16 it);

bool is_vpbroadcast_d_q(uint16 it);

bool is_vperm2_insn(uint16 it);

bool is_phsub_insn(uint16 it);

bool is_pack_insn(uint16 it);

bool is_ptest_insn(uint16 it);

bool is_extract_insert_insn(uint16 it);

bool is_movdup_insn(uint16 it);

bool is_unpack_insn(uint16 it);

bool is_fmaddsub_insn(uint16 it);

bool is_movmsk_insn(uint16 it);

bool is_movnt_insn(uint16 it);

bool is_mask_to_vec_insn(uint16 it);

bool is_vpbroadcast_b_w(uint16 it);

bool is_pinsert_insn(uint16 it);

bool is_pmovsx_insn(uint16 it);

bool is_pmovzx_insn(uint16 it);

bool is_byte_shift_insn(uint16 it);

bool is_punpck_insn(uint16 it);

bool is_extractps_insn(uint16 it);

bool is_insertps_insn(uint16 it);

// Predicate extraction
uint8 get_cmp_predicate(uint16 it);

// AVX-512 Masking Info
// This structure captures all mask-related information for an instruction
struct MaskInfo {
    bool has_mask;          // true if instruction uses opmask (k1-k7)
    bool is_zeroing;        // true if zero-masking, false if merge-masking
    mreg_t mask_reg;        // mreg for the mask register (loaded via codegen)
    int num_elements;       // number of elements being masked (for mask type selection)

    MaskInfo() : has_mask(false), is_zeroing(false), mask_reg(mr_none), num_elements(0) {}

    // Initialize from instruction context
    // NOTE: This only captures mask metadata. The mask_reg must be loaded separately
    // using load_mask_operand() because reg2mreg doesn't work for k-registers.
    static MaskInfo from_insn(const insn_t &insn, int elem_size) {
        MaskInfo info;
        info.has_mask = has_opmask(insn);
        info.is_zeroing = is_zero_masking(insn);
        info.mask_reg = mr_none; // Must be loaded via load_mask_operand()

        if (info.has_mask) {
            // Calculate number of elements based on vector size and element size
            int vec_size = ZMM_SIZE; // Default to ZMM for AVX-512 masked operations
            if (is_ymm_reg(insn.Op1)) vec_size = YMM_SIZE;
            else if (is_xmm_reg(insn.Op1)) vec_size = XMM_SIZE;

            info.num_elements = vec_size / elem_size;
        }
        return info;
    }
};

// Load the opmask register value into a microcode register
// Returns mr_none if no mask or load fails
mreg_t load_mask_operand(codegen_t &cdg, MaskInfo &mask);

// Generate masked intrinsic name
// base_name: e.g., "_mm512_add_ps"
// mask_info: masking information
// Returns: e.g., "_mm512_mask_add_ps" or "_mm512_maskz_add_ps"
qstring make_masked_intrinsic_name(const char *base_name, const MaskInfo &mask_info);

#endif // IDA_SDK_VERSION >= 750
