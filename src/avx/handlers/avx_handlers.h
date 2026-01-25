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

merror_t handle_vcvtdq2pd(codegen_t &cdg);

// SAD (Sum of Absolute Differences)
merror_t handle_vsad(codegen_t &cdg);

// Moves
merror_t handle_vmov_ss_sd(codegen_t &cdg, int data_size);

merror_t handle_vmov(codegen_t &cdg, int data_size);

merror_t handle_v_mov_ps_dq(codegen_t &cdg);

merror_t handle_v_gather(codegen_t &cdg);

merror_t handle_v_scatter(codegen_t &cdg);

merror_t handle_v_compress(codegen_t &cdg);

merror_t handle_v_expand(codegen_t &cdg);

// Math
merror_t handle_v_math_ss_sd(codegen_t &cdg, int elem_size);

merror_t handle_v_minmax_ss_sd(codegen_t &cdg);

merror_t handle_v_math_p(codegen_t &cdg);

merror_t handle_v_abs(codegen_t &cdg);

merror_t handle_v_sign(codegen_t &cdg);

merror_t handle_v_fma(codegen_t &cdg);

merror_t handle_v_ternary_logic(codegen_t &cdg);

merror_t handle_v_conflict(codegen_t &cdg);

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

merror_t handle_v_shuffle_int(codegen_t &cdg);

merror_t handle_v_perm_int(codegen_t &cdg);

merror_t handle_v_permutex(codegen_t &cdg);

merror_t handle_v_permutex2(codegen_t &cdg);

merror_t handle_v_align(codegen_t &cdg);

merror_t handle_vshufps(codegen_t &cdg);

merror_t handle_vshufpd(codegen_t &cdg);

merror_t handle_vpermpd(codegen_t &cdg);

merror_t handle_vzeroupper_nop(codegen_t &cdg);

merror_t handle_vbroadcast_ss_sd(codegen_t &cdg);

merror_t handle_vbroadcastf128_fp(codegen_t &cdg);

merror_t handle_vbroadcasti128_int(codegen_t &cdg);

merror_t handle_vcmp_ps_pd(codegen_t &cdg);

merror_t handle_vpcmp_int(codegen_t &cdg);

merror_t handle_vblendv_ps_pd(codegen_t &cdg);

merror_t handle_vblend_imm_ps_pd(codegen_t &cdg);

merror_t handle_vblend_int(codegen_t &cdg);

merror_t handle_vmaskmov_ps_pd(codegen_t &cdg);

// Extract/Insert
merror_t handle_vextractf128(codegen_t &cdg);

merror_t handle_vinsertf128(codegen_t &cdg);

// Move duplicates
merror_t handle_vmovshdup(codegen_t &cdg);

merror_t handle_vmovsldup(codegen_t &cdg);

merror_t handle_vmovddup(codegen_t &cdg);

// Unpack
merror_t handle_vunpck(codegen_t &cdg);

// Scalar approximations
merror_t handle_vrcp_rsqrt_ss(codegen_t &cdg);

// Scalar rounding
merror_t handle_vround_ss_sd(codegen_t &cdg);

// Scalar sqrt double
merror_t handle_vsqrtsd(codegen_t &cdg);

// Addsub
merror_t handle_vaddsubps_pd(codegen_t &cdg);

// Broadcast d/q
merror_t handle_vpbroadcast_d_q(codegen_t &cdg);

// Permute 128-bit lanes
merror_t handle_vperm2f128_i128(codegen_t &cdg);

// Horizontal subtract
merror_t handle_vphsub_sw(codegen_t &cdg);

// Pack
merror_t handle_vpack(codegen_t &cdg);

// Test
merror_t handle_vptest(codegen_t &cdg);

// FMA add/sub alternating
merror_t handle_vfmaddsub(codegen_t &cdg);

// Move mask
merror_t handle_vmovmsk(codegen_t &cdg);

// Non-temporal store
merror_t handle_vmovnt(codegen_t &cdg);

// Broadcast byte/word
merror_t handle_vpbroadcast_b_w(codegen_t &cdg);

// Insert
merror_t handle_vpinsert(codegen_t &cdg);

// Sign/zero extend
merror_t handle_vpmovsx(codegen_t &cdg);
merror_t handle_vpmovzx(codegen_t &cdg);

// Byte shift
merror_t handle_vpslldq_vpsrldq(codegen_t &cdg);

// Integer unpack
merror_t handle_vpunpck(codegen_t &cdg);

// Extract float to GPR/mem
merror_t handle_vextractps(codegen_t &cdg);

// Insert single float
merror_t handle_vinsertps(codegen_t &cdg);

#endif // IDA_SDK_VERSION >= 750
