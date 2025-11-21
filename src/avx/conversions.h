/*
 AVX Conversion Instruction Handlers
*/

#pragma once

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include "../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

// Conversion instruction handlers
merror_t handle_vcvtdq2ps(codegen_t &cdg);

merror_t handle_vcvtsi2fp(codegen_t &cdg);

merror_t handle_vcvtps2pd(codegen_t &cdg);

merror_t handle_vcvtfp2fp(codegen_t &cdg);

merror_t handle_vcvtpd2ps(codegen_t &cdg);

merror_t handle_vcvt_ps2dq(codegen_t &cdg, bool trunc);

merror_t handle_vcvt_pd2dq(codegen_t &cdg, bool trunc);

#endif // IDA_SDK_VERSION >= 750
