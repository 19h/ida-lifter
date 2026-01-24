/*
AVX Helper Functions
*/

#include "avx_helpers.h"
#include "avx_intrinsic.h"
#include "avx_types.h"

#if IDA_SDK_VERSION >= 750

#include "../common/warn_off.h"
#include <name.hpp>
#include "../common/warn_on.h"

// Operand analysis
bool is_mem_op(const op_t &op) { return op.type == o_mem || op.type == o_displ || op.type == o_phrase; }
bool is_reg_op(const op_t &op) { return op.type == o_reg; }
bool is_xmm_reg(const op_t &op) {
    if (op.type == o_xmmreg) return true;
    return op.type == o_reg && op.dtype == dt_byte16;
}
bool is_ymm_reg(const op_t &op) {
    if (op.type == o_ymmreg) return true;
    return op.type == o_reg && op.dtype == dt_byte32;
}
// ZMM check: dtype can be dt_byte64 OR the register can be in the ZMM range
// Some instructions (like vpmovsxbd zmm, mem) may not have dt_byte64 dtype set
bool is_zmm_reg(const op_t &op) {
    if (op.type == o_zmmreg) return true;
    if (op.type != o_reg) return false;
    if (op.dtype == dt_byte64) return true;
    // Also check register number for ZMM0-ZMM31
    return (op.reg >= R_zmm0 && op.reg <= R_zmm31);
}
// AVX register (XMM or YMM) - for backwards compatibility
bool is_avx_reg(const op_t &op) {
    if (op.type == o_xmmreg || op.type == o_ymmreg) return true;
    return op.type == o_reg && (op.dtype == dt_byte16 || op.dtype == dt_byte32);
}

// Any vector register (XMM, YMM, or ZMM)
bool is_vector_reg(const op_t &op) {
    if (op.type == o_xmmreg || op.type == o_ymmreg || op.type == o_zmmreg) return true;
    return op.type == o_reg &&
           (op.dtype == dt_byte16 || op.dtype == dt_byte32 || op.dtype == dt_byte64 ||
            (op.reg >= R_zmm0 && op.reg <= R_zmm31));
}

// Check if operand is an AVX-512 register (including ZMM)
bool is_avx512_reg(const op_t &op) {
    if (op.type == o_xmmreg || op.type == o_ymmreg || op.type == o_zmmreg) return true;
    return op.type == o_reg && (op.dtype == dt_byte16 || op.dtype == dt_byte32 || op.dtype == dt_byte64);
}

// Check if operand is an opmask register (k0-k7)
// Note: IDA sometimes encodes k-registers as o_reg with register numbers R_k0-R_k7
// instead of using o_kreg operand type
bool is_mask_reg(const op_t &op) {
    if (op.type == o_kreg)
        return true;
    // Also check for k-registers encoded as o_reg
    if (op.type == o_reg && op.reg >= R_k0 && op.reg <= R_k7)
        return true;
    return false;
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

// Load operand with UDT flag support for large operands (> 8 bytes)
// For standard sizes (<= 32 bytes), use cdg.load_operand().
// For 64-byte operands (ZMM), we use emit_zmm_load() which bypasses load_operand().
mreg_t load_operand_udt(codegen_t &cdg, int opnum, int size) {
    // For sizes > 32 bytes (ZMM), use the manual emit approach
    if (size > YMM_SIZE) {
        return emit_zmm_load(cdg, opnum, size);
    }

    mreg_t reg = cdg.load_operand(opnum);
    if (reg == mr_none)
        return mr_none;

    // For sizes > 8 bytes (XMM/YMM), set the UDT flag on the ldx destination
    if (size > 8) {
        minsn_t *ins = cdg.mb->tail;
        if (ins && ins->opcode == m_ldx) {
            ins->d.set_udt();
        } else if (ins && ins->prev && ins->prev->opcode == m_ldx) {
            // Sometimes there's an extra instruction after the ldx
            ins->prev->d.set_udt();
        }
    }
    return reg;
}

// Emit a ZMM (64-byte) load from memory using load_effective_address() + manual m_ldx.
// This bypasses cdg.load_operand() which fails verification for 64-byte destinations
// because it internally verifies before we can set the UDT flag.
//
// The approach:
// 1. Use load_effective_address() to compute pointer (pointer-sized, so verifier-safe)
// 2. Manually emit m_ldx with UDT-flagged 64-byte destination
//
// m_ldx format: ldx {l=seg, r=off}, d
// - seg: segment register (size 2, typically DS)
// - off: memory offset/address (size = address size, 8 for 64-bit)
// - d: destination register (size = data size, 64 for ZMM)
mreg_t emit_zmm_load(codegen_t &cdg, int opidx, int zmm_size) {
    // 1. Compute effective address into a temp register
    // load_effective_address() only works with pointer-sized data, so verifier-safe
    mreg_t ea_reg = cdg.load_effective_address(opidx);
    if (ea_reg == mr_none) {
        // Not a memory operand or failed
        return mr_none;
    }

    // 2. Determine address size (8 for 64-bit mode)
    int addr_size = inf_is_64bit() ? 8 : 4;

    // 3. Build segment operand (DS, size 2)
    // In 64-bit mode, segment overrides are mostly ignored except for FS/GS
    // For simplicity, we use DS (R_ds = 3 in intel.hpp)
    mop_t seg;
    seg.make_reg(reg2mreg(R_ds), 2);

    // 4. Build offset operand (the effective address we computed)
    mop_t off;
    off.make_reg(ea_reg, addr_size);

    // 5. Allocate destination register and build UDT-flagged destination operand
    // Note: alloc_kreg with check_size=false allows non-standard sizes
    mreg_t dst_mreg = cdg.mba->alloc_kreg(zmm_size, false);
    if (dst_mreg == mr_none) {
        return mr_none;
    }

    mop_t dst;
    dst.make_reg(dst_mreg, zmm_size);
    dst.set_udt();  // Critical: mark as UDT before any verification

    // 6. Emit m_ldx with our mops
    // emit() does NOT internally verify - verification happens later at mba->verify()
    minsn_t *ldx = cdg.emit(m_ldx, &seg, &off, &dst);
    if (ldx == nullptr) {
        cdg.mba->free_kreg(dst_mreg, zmm_size);
        return mr_none;
    }

    return dst_mreg;
}

// Emit a vector store to memory using store intrinsics.
// This bypasses m_stx emission which causes INTERR 50708 for large operands.
//
// All vector stores (XMM/YMM/ZMM) are emitted as intrinsic calls:
// - _mm_storeu_ps / _mm256_storeu_ps / _mm512_storeu_ps
//
// This approach works for all memory operand types (global, stack, heap).
bool emit_vector_store(codegen_t &cdg, int opidx, mreg_t src_mreg, int vec_size) {
    const op_t &op = opidx == 0 ? cdg.insn.Op1 : (opidx == 1 ? cdg.insn.Op2 : cdg.insn.Op3);
    int addr_size = inf_is_64bit() ? 8 : 4;

    // Get the effective address into a register
    mreg_t addr_reg;

    if (op.type == o_mem) {
        // Direct memory reference (global address) - load immediate address
        addr_reg = cdg.mba->alloc_kreg(addr_size);
        mop_t addr_imm;
        addr_imm.make_number(op.addr, addr_size);
        mop_t addr_dst;
        addr_dst.make_reg(addr_reg, addr_size);
        mop_t empty;
        cdg.emit(m_mov, &addr_imm, &empty, &addr_dst);
    } else {
        // o_displ or o_phrase - compute effective address
        addr_reg = cdg.load_effective_address(opidx);
        if (addr_reg == mr_none) {
            return false;
        }
    }

    // Determine intrinsic name based on size
    const char *iname;
    if (vec_size == ZMM_SIZE) {
        iname = "_mm512_storeu_ps";
    } else if (vec_size == YMM_SIZE) {
        iname = "_mm256_storeu_ps";
    } else {
        iname = "_mm_storeu_ps";
    }

    // Create void* type for the address argument
    tinfo_t ptr_type;
    ptr_type.create_ptr(tinfo_t(BT_VOID));

    // Create vector type for the value argument
    tinfo_t vec_type = get_vector_type(vec_size, false, false);

    // Build the intrinsic call (void return)
    AVXIntrinsic icall(&cdg, iname);
    icall.add_argument_reg(addr_reg, ptr_type);
    icall.add_argument_reg(src_mreg, vec_type);
    icall.emit_void();

    // Don't free addr_reg - it's used in the emitted instruction

    return true;
}

// Legacy name for backwards compatibility
bool emit_zmm_store(codegen_t &cdg, int opidx, mreg_t src_mreg, int zmm_size) {
    return emit_vector_store(cdg, opidx, src_mreg, zmm_size);
}

// Store operand - handles all sizes including ZMM (64-byte)
bool store_operand_hack(codegen_t &cdg, int n, const mop_t &mop, int flags, minsn_t **outins) {
    // For large operands (> 8 bytes, i.e., XMM/YMM/ZMM), use the manual emit approach
    // The ldx->stx conversion approach fails for global addresses (o_mem) with INTERR 50708
    if (mop.size > 8) {
        // mop contains the source register
        if (mop.t != mop_r) {
            return false;  // Only register sources supported
        }
        return emit_vector_store(cdg, n, mop.r, mop.size);
    }

#if IDA_SDK_VERSION < 760
    // IDA < 7.6: synthesize a store by rewriting the last emitted ldx.
    mreg_t memX = cdg.load_operand(n);
    if (memX == mr_none) return false;

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
#else
    // For standard sizes (<= 8 bytes), use store_operand
    bool result = cdg.store_operand(n, mop, flags, outins);
    return result;
#endif
}

// AVX-512 masking support

// Check if instruction has opmask in Op6 (EVEX encoding stores mask in Op6)
bool has_opmask(const insn_t &insn) {
    // Op6 holds the opmask register in EVEX-encoded instructions
    // A valid opmask is k1-k7 (k0 means no masking)
    if ((insn.Op6.type == o_kreg || insn.Op6.type == o_reg) &&
        insn.Op6.reg >= R_k1 && insn.Op6.reg <= R_k7)
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
    if (insn.Op6.type == o_kreg || insn.Op6.type == o_reg) {
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
