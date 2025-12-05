/*
AVX Intrinsic Call Builder
*/

#pragma once

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include "../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

#include "avx_types.h"

struct ida_local AVXIntrinsic {
    codegen_t *cdg;
    mcallinfo_t *call_info; // Heap-allocated (IDA allocator)
    minsn_t *call_insn; // Heap-allocated (IDA allocator)
    minsn_t *mov_insn; // Heap-allocated (IDA allocator)
    bool emitted; // Track ownership transfer
    int stk_off; // Stack offset for arguments to satisfy verification

    explicit AVXIntrinsic(codegen_t *cdg_, const char *name);

    ~AVXIntrinsic();

    void set_return_reg(mreg_t mreg, const tinfo_t &ret_ti);

    void set_return_reg(mreg_t mreg, const char *type_name);

    void set_return_reg_basic(mreg_t mreg, type_t basic_type);

    void add_argument_reg(mreg_t mreg, const tinfo_t &arg_ti);

    void add_argument_reg(mreg_t mreg, const char *type_name);

    void add_argument_reg(mreg_t mreg, type_t bt);

    // Add argument with explicit size (for pointer arguments where type size may not match target)
    void add_argument_reg_with_size(mreg_t mreg, int size);

    void add_argument_imm(uint64 value, type_t bt);

    // Add mask argument (__mmask8, __mmask16, __mmask32, __mmask64)
    void add_argument_mask(mreg_t mreg, int num_elements);

    minsn_t *emit();

    // Emit a void-returning intrinsic (like store intrinsics)
    minsn_t *emit_void();
};

#endif // IDA_SDK_VERSION >= 750
