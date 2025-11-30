/*
AVX Helper Functions
*/

#include "avx_helpers.h"

#if IDA_SDK_VERSION >= 750

#include "../common/warn_off.h"
#include <name.hpp>
#include "../common/warn_on.h"

// Operand analysis
bool is_mem_op(const op_t &op) { return op.type == o_mem || op.type == o_displ || op.type == o_phrase; }
bool is_reg_op(const op_t &op) { return op.type == o_reg; }
bool is_xmm_reg(const op_t &op) { return op.type == o_reg && op.dtype == dt_byte16; }
bool is_ymm_reg(const op_t &op) { return op.type == o_reg && op.dtype == dt_byte32; }
bool is_zmm_reg(const op_t &op) { return op.type == o_reg && op.dtype == dt_byte64; }
// AVX register (XMM or YMM) - for backwards compatibility
bool is_avx_reg(const op_t &op) { return op.type == o_reg && (op.dtype == dt_byte16 || op.dtype == dt_byte32); }

// Any vector register (XMM, YMM, or ZMM)
bool is_vector_reg(const op_t &op) { return op.type == o_reg && (op.dtype == dt_byte16 || op.dtype == dt_byte32 || op.dtype == dt_byte64); }

// Check if operand is an AVX-512 register (including ZMM)
bool is_avx512_reg(const op_t &op) {
    return op.type == o_reg && (op.dtype == dt_byte16 || op.dtype == dt_byte32 || op.dtype == dt_byte64);
}

// Check if operand is an opmask register (k0-k7)
bool is_mask_reg(const op_t &op) {
    return op.type == o_kreg;
}

// Check if instruction uses AVX-512 features
bool is_avx_512(const insn_t &insn) {
    // Check if any operand is a ZMM register (512-bit)
    if (is_zmm_reg(insn.Op1) || is_zmm_reg(insn.Op2) || is_zmm_reg(insn.Op3) || is_zmm_reg(insn.Op4))
        return true;

    // Check if any operand is an opmask register (AVX-512 masking)
    if (is_mask_reg(insn.Op1) || is_mask_reg(insn.Op2) || is_mask_reg(insn.Op3) ||
        is_mask_reg(insn.Op4) || is_mask_reg(insn.Op5) || is_mask_reg(insn.Op6))
        return true;

    return false;
}

// Map XMM mreg -> matching YMM mreg (same number, wider class)
mreg_t get_ymm_mreg(mreg_t xmm_mreg) {
    int xmm_reg = mreg2reg(xmm_mreg, XMM_SIZE);
    if (xmm_reg == -1) return mr_none;

    qstring xmm_name;
    if (get_reg_name(&xmm_name, xmm_reg, XMM_SIZE) == -1) return mr_none;

    // Convert "xmmN" to "ymmN"
    if (xmm_name.length() < 3) return mr_none;
    qstring ymm_name = "ymm";
    ymm_name += xmm_name.substr(3);

    int ymm_reg = str2reg(ymm_name.c_str());
    if (ymm_reg == -1) return mr_none;

    return reg2mreg(ymm_reg);
}

// Clear upper lanes of an XMM destination through the matching YMM
// In AVX, VEX-encoded instructions that write to XMM automatically zero the upper 128 bits.
// We model this by not emitting any explicit zeroing - the semantics are implicit.
minsn_t *clear_upper(codegen_t &cdg, mreg_t xmm_mreg, int op_size) {
    (void)cdg;
    (void)xmm_mreg;
    (void)op_size;
    // No explicit operation needed - AVX semantics handle this automatically
    return nullptr;
}

#if IDA_SDK_VERSION < 760
// IDA < 7.6: synthesize a store by rewriting the last emitted ldx.
bool store_operand_hack(codegen_t &cdg, int n, const mop_t &mop, int flags, minsn_t **outins) {
    mreg_t memX = cdg.load_operand(n);
    QASSERT(0xA0100, memX != mr_none);

    minsn_t *ins = cdg.mb->tail;
    if (!ins) return false;

    if (ins->opcode != m_ldx) {
        if (ins->prev && ins->prev->opcode == m_ldx) {
            minsn_t *prev = ins->prev;
            cdg.mb->make_nop(ins);
            ins = prev;
        } else {
            // Cannot apply hack if ldx is not found
            return false;
        }
    }

    // Verify size matches
    if (ins->d.size != mop.size) {
        DEBUG_LOG("%a: store_operand_hack size mismatch (ins=%d, mop=%d)", cdg.insn.ea, ins->d.size, mop.size);
    }

    ins->opcode = m_stx;
    ins->d = ins->r; // mem offset
    ins->r = ins->l; // mem segment
    ins->l = mop; // value to store

    if (outins) *outins = ins;
    return true;
}
#else
bool store_operand_hack(codegen_t &cdg, int n, const mop_t &mop, int flags, minsn_t **outins) {
    return cdg.store_operand(n, mop, flags, outins);
}
#endif

// AVX-512 masking support

// Check if instruction has opmask in Op6 (EVEX encoding stores mask in Op6)
bool has_opmask(const insn_t &insn) {
    // Op6 holds the opmask register in EVEX-encoded instructions
    // A valid opmask is k1-k7 (k0 means no masking)
    if (insn.Op6.type == o_kreg && insn.Op6.reg >= R_k1 && insn.Op6.reg <= R_k7)
        return true;
    return false;
}

// Check if instruction uses zero-masking (EVEX.z bit)
bool is_zero_masking(const insn_t &insn) {
    // EVEX.z bit is stored in evex_flags (Op6.specflag2)
    return (insn.evex_flags & EVEX_z) != 0;
}

// Get the opmask register number (0-7 for k0-k7)
int get_opmask_reg(const insn_t &insn) {
    if (insn.Op6.type == o_kreg) {
        return insn.Op6.reg - R_k0;
    }
    return 0; // k0 = no masking
}

// Get mreg for opmask register
mreg_t get_opmask_mreg(const insn_t &insn, codegen_t &cdg) {
    if (!has_opmask(insn))
        return mr_none;

    // Load the opmask register value
    // Opmask registers are 64-bit (can mask up to 64 elements for byte operations)
    // For ZMM with 32-bit elements, only 16 bits are used
    // For ZMM with 64-bit elements, only 8 bits are used
    mreg_t kreg = reg2mreg(insn.Op6.reg);
    return kreg;
}

#endif // IDA_SDK_VERSION >= 750