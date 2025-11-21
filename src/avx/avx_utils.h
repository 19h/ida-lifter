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

// Operand helpers
mreg_t load_op_reg_or_mem(codegen_t &cdg, int op_idx, const op_t &op);

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

bool is_move_insn(uint16 it);

bool is_bitwise_insn(uint16 it);

bool is_scalar_minmax(uint16 it);

bool is_packed_minmax_fp(uint16 it);

bool is_packed_minmax_int(uint16 it);

bool is_int_mul(uint16 it);

bool is_avg_insn(uint16 it);

bool is_abs_insn(uint16 it);

bool is_sign_insn(uint16 it);

bool is_shift_insn(uint16 it);

bool is_var_shift_insn(uint16 it);

bool is_math_insn(uint16 it);

bool is_broadcast_insn(uint16 it);

bool is_misc_insn(uint16 it);

bool is_blend_insn(uint16 it);

bool is_maskmov_insn(uint16 it);

bool is_packed_compare_insn(uint16 it);

bool is_horizontal_math(uint16 it);

bool is_dot_product(uint16 it);

bool is_approx_insn(uint16 it);

bool is_round_insn(uint16 it);

// Predicate extraction
uint8 get_cmp_predicate(uint16 it);

#endif // IDA_SDK_VERSION >= 750
