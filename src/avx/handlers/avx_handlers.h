/*
AVX Instruction Handlers
*/

#pragma once

#include "../../common/warn_off.h"
#include <hexrays.hpp>
#include "../../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

// Conversions
merror_t handle_vcvtdq2ps(codegen_t &cdg);

merror_t handle_vcvtsi2fp(codegen_t &cdg);

merror_t handle_vcvtps2pd(codegen_t &cdg);

merror_t handle_vcvtfp2fp(codegen_t &cdg);

merror_t handle_vcvtpd2ps(codegen_t &cdg);

merror_t handle_vcvt_ps2dq(codegen_t &cdg, bool trunc);

merror_t handle_vcvt_pd2dq(codegen_t &cdg, bool trunc);

// Moves
merror_t handle_vmov_ss_sd(codegen_t &cdg, int data_size);

merror_t handle_vmov(codegen_t &cdg, int data_size);

merror_t handle_v_mov_ps_dq(codegen_t &cdg);

// Math
merror_t handle_v_math_ss_sd(codegen_t &cdg, int elem_size);

merror_t handle_v_minmax_ss_sd(codegen_t &cdg);

merror_t handle_v_math_p(codegen_t &cdg);

merror_t handle_v_abs(codegen_t &cdg);

merror_t handle_v_sign(codegen_t &cdg);

merror_t handle_vsqrtss(codegen_t &cdg);

merror_t handle_vsqrt_ps_pd(codegen_t &cdg);

merror_t handle_v_hmath(codegen_t &cdg);

merror_t handle_v_dot(codegen_t &cdg);

merror_t handle_vrcp_rsqrt(codegen_t &cdg);

merror_t handle_vround(codegen_t &cdg);

// Logic & Misc
merror_t handle_v_bitwise(codegen_t &cdg);

merror_t handle_v_shift(codegen_t &cdg);

merror_t handle_v_var_shift(codegen_t &cdg);

merror_t handle_vshufps(codegen_t &cdg);

merror_t handle_vshufpd(codegen_t &cdg);

merror_t handle_vpermpd(codegen_t &cdg);

merror_t handle_vzeroupper_nop(codegen_t &cdg);

merror_t handle_vbroadcast_ss_sd(codegen_t &cdg);

merror_t handle_vbroadcastf128_fp(codegen_t &cdg);

merror_t handle_vbroadcasti128_int(codegen_t &cdg);

merror_t handle_vcmp_ps_pd(codegen_t &cdg);

merror_t handle_vblendv_ps_pd(codegen_t &cdg);

merror_t handle_vblend_imm_ps_pd(codegen_t &cdg);

merror_t handle_vmaskmov_ps_pd(codegen_t &cdg);

#endif // IDA_SDK_VERSION >= 750
