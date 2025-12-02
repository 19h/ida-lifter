#ifndef COMMON_H
#define COMMON_H

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

// Force noinline to ensure the lifter sees a distinct function for each instruction
#if defined(_MSC_VER)
  #define NOINLINE __declspec(noinline)
#else
  #define NOINLINE __attribute__((noinline))
#endif

// --- AVX (Float) Prototypes ---

// Arithmetic
__m256 test_vaddps(__m256 a, __m256 b);
__m256 test_vsubps(__m256 a, __m256 b);
__m256 test_vmulps(__m256 a, __m256 b);
__m256 test_vdivps(__m256 a, __m256 b);
__m256 test_vsqrtps(__m256 a);
__m256 test_vrsqrtps(__m256 a);
__m256 test_vrcpps(__m256 a);
__m256 test_vmaxps(__m256 a, __m256 b);
__m256 test_vminps(__m256 a, __m256 b);
__m256 test_vroundps_floor(__m256 a);
__m256 test_vroundps_ceil(__m256 a);
__m256 test_vhaddps(__m256 a, __m256 b);
__m256 test_vhsubps(__m256 a, __m256 b);
__m256 test_vfmadd231ps(__m256 a, __m256 b, __m256 c);

// Logic
__m256 test_vandps(__m256 a, __m256 b);
__m256 test_vorps(__m256 a, __m256 b);
__m256 test_vxorps(__m256 a, __m256 b);
__m256 test_vandnps(__m256 a, __m256 b);

// Compare (Immediate variants)
__m256 test_vcmpps_eq(__m256 a, __m256 b);
__m256 test_vcmpps_lt(__m256 a, __m256 b);
__m256 test_vcmpps_le(__m256 a, __m256 b);
__m256 test_vcmpps_neq(__m256 a, __m256 b);

// Permute/Shuffle (Immediate variants)
__m256 test_vpermute_ps_0x1B(__m256 a);
__m256 test_vpermute2f128_ps_0x01(__m256 a, __m256 b);
__m256 test_vshuffle_ps_0x1B(__m256 a, __m256 b);
__m256 test_vblend_ps_0xAA(__m256 a, __m256 b);

// Conversion
__m256i test_vcvtps2dq(__m256 a);
__m256  test_vcvtdq2ps(__m256i a);


// --- AVX2 (Integer) Prototypes ---

// Arithmetic (8, 16, 32, 64)
__m256i test_vpaddb(__m256i a, __m256i b);
__m256i test_vpaddw(__m256i a, __m256i b);
__m256i test_vpaddd(__m256i a, __m256i b);
__m256i test_vpaddq(__m256i a, __m256i b);

__m256i test_vpsubb(__m256i a, __m256i b);
__m256i test_vpsubw(__m256i a, __m256i b);
__m256i test_vpsubd(__m256i a, __m256i b);
__m256i test_vpsubq(__m256i a, __m256i b);

__m256i test_vpadds_b(__m256i a, __m256i b); // Saturated
__m256i test_vpadds_w(__m256i a, __m256i b);

__m256i test_vpmulld(__m256i a, __m256i b);
__m256i test_vpmuludq(__m256i a, __m256i b);
__m256i test_vpmaddwd(__m256i a, __m256i b);

__m256i test_vpmaxub(__m256i a, __m256i b);
__m256i test_vpminub(__m256i a, __m256i b);
__m256i test_vpmaxsd(__m256i a, __m256i b);
__m256i test_vpminsd(__m256i a, __m256i b);

__m256i test_vpabsd(__m256i a);
__m256i test_vpsignb(__m256i a, __m256i b);

// Logic
__m256i test_vpand(__m256i a, __m256i b);
__m256i test_vpor(__m256i a, __m256i b);
__m256i test_vpxor(__m256i a, __m256i b);
__m256i test_vpandn(__m256i a, __m256i b);

// Shifts (Immediate and Variable)
__m256i test_vpslld_imm(__m256i a);
__m256i test_vpsrld_imm(__m256i a);
__m256i test_vpsrad_imm(__m256i a);
__m256i test_vpsllv_d(__m256i a, __m256i count);
__m256i test_vpsrlv_d(__m256i a, __m256i count);
__m256i test_vpsrav_d(__m256i a, __m256i count);

// Compare
__m256i test_vpcmpeqb(__m256i a, __m256i b);
__m256i test_vpcmpgtb(__m256i a, __m256i b);
__m256i test_vpcmpeqd(__m256i a, __m256i b);
__m256i test_vpcmpgtd(__m256i a, __m256i b);
__m256i test_vpcmpeqq(__m256i a, __m256i b);
__m256i test_vpcmpgtq(__m256i a, __m256i b);

// Permute/Shuffle
__m256i test_vpshufb(__m256i a, __m256i b);
__m256i test_vpermd(__m256i idx, __m256i a);
__m256i test_vpermq_imm(__m256i a);
__m256i test_vperm2i128_imm(__m256i a, __m256i b);
__m256i test_vpalignr_imm(__m256i a, __m256i b);
__m256i test_vpblendd_imm(__m256i a, __m256i b);

#endif // COMMON_H
