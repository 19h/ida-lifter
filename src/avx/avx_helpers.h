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
