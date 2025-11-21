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

bool is_avx_512(const insn_t &insn);

// Register mapping
mreg_t get_ymm_mreg(mreg_t xmm_mreg);

minsn_t *clear_upper(codegen_t &cdg, mreg_t xmm_mreg, int op_size = XMM_SIZE);

// Store operand hack for IDA < 7.6
bool store_operand_hack(codegen_t &cdg, int n, const mop_t &mop, int flags = 0, minsn_t **outins = nullptr);

#endif // IDA_SDK_VERSION >= 750
