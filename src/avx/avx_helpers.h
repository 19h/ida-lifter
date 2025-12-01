/*
 AVX Helper Functions
*/

#pragma once

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <intel.hpp>
#include "../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

#include "avx_types.h"

// Operand analysis
bool is_mem_op(const op_t &op);

bool is_reg_op(const op_t &op);

bool is_xmm_reg(const op_t &op);

bool is_ymm_reg(const op_t &op);

bool is_zmm_reg(const op_t &op);

bool is_avx_reg(const op_t &op);

bool is_vector_reg(const op_t &op);

bool is_avx512_reg(const op_t &op);

bool is_mask_reg(const op_t &op);

bool is_avx_512(const insn_t &insn);

// Register mapping
mreg_t get_ymm_mreg(mreg_t xmm_mreg);

minsn_t *clear_upper(codegen_t &cdg, mreg_t xmm_mreg, int op_size = XMM_SIZE);

// Store operand hack for IDA < 7.6
bool store_operand_hack(codegen_t &cdg, int n, const mop_t &mop, int flags = 0, minsn_t **outins = nullptr);

// Load operand with UDT flag support for large operands (> 8 bytes)
// This wraps cdg.load_operand() and post-processes the emitted ldx instruction
// to set the OPROP_UDT flag on its destination, which is required for AVX-512
// 64-byte operands to pass the microcode verifier.
mreg_t load_operand_udt(codegen_t &cdg, int opnum, int size);

// Emit a ZMM (64-byte) load from memory using load_effective_address() + manual m_ldx.
// This bypasses cdg.load_operand() which fails verification for 64-byte destinations.
// Returns the destination register containing the loaded value, or mr_none on failure.
mreg_t emit_zmm_load(codegen_t &cdg, int opidx, int zmm_size = ZMM_SIZE);

// Emit a vector store to memory using load_effective_address() + manual m_stx.
// This bypasses the ldx->stx conversion approach which fails for global addresses.
// Works for XMM (16-byte), YMM (32-byte), and ZMM (64-byte) sizes.
// Returns true on success, false on failure.
bool emit_vector_store(codegen_t &cdg, int opidx, mreg_t src_mreg, int vec_size);

// Legacy name for backwards compatibility
bool emit_zmm_store(codegen_t &cdg, int opidx, mreg_t src_mreg, int zmm_size = ZMM_SIZE);

// AVX-512 masking support
// Check if instruction has opmask in Op6 (EVEX encoding stores mask in Op6)
bool has_opmask(const insn_t &insn);

// Check if instruction uses zero-masking (EVEX.z bit)
bool is_zero_masking(const insn_t &insn);

// Get the opmask register number (0-7 for k0-k7)
int get_opmask_reg(const insn_t &insn);

// Get mreg for opmask register
mreg_t get_opmask_mreg(const insn_t &insn, codegen_t &cdg);

#endif // IDA_SDK_VERSION >= 750
