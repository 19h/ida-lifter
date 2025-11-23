#include "common.h"

// --- Macros for repetitive patterns ---

#define GEN_UNARY_OP(name, intrinsic) \
    NOINLINE __m256 name(__m256 a) { \
        return intrinsic(a); \
    }

#define GEN_BINARY_OP(name, intrinsic) \
    NOINLINE __m256 name(__m256 a, __m256 b) { \
        return intrinsic(a, b); \
    }

#define GEN_TERNARY_OP(name, intrinsic) \
    NOINLINE __m256 name(__m256 a, __m256 b, __m256 c) { \
        return intrinsic(a, b, c); \
    }

// --- Arithmetic ---

GEN_BINARY_OP(test_vaddps, _mm256_add_ps)
GEN_BINARY_OP(test_vsubps, _mm256_sub_ps)
GEN_BINARY_OP(test_vmulps, _mm256_mul_ps)
GEN_BINARY_OP(test_vdivps, _mm256_div_ps)

GEN_UNARY_OP(test_vsqrtps, _mm256_sqrt_ps)
GEN_UNARY_OP(test_vrsqrtps, _mm256_rsqrt_ps)
GEN_UNARY_OP(test_vrcpps, _mm256_rcp_ps)

GEN_BINARY_OP(test_vmaxps, _mm256_max_ps)
GEN_BINARY_OP(test_vminps, _mm256_min_ps)

// Rounding requires an immediate, so we hardcode typical modes
NOINLINE __m256 test_vroundps_floor(__m256 a) {
    return _mm256_round_ps(a, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
}

NOINLINE __m256 test_vroundps_ceil(__m256 a) {
    return _mm256_round_ps(a, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
}

GEN_BINARY_OP(test_vhaddps, _mm256_hadd_ps)
GEN_BINARY_OP(test_vhsubps, _mm256_hsub_ps)

GEN_TERNARY_OP(test_vfmadd231ps, _mm256_fmadd_ps)

// --- Logic ---

GEN_BINARY_OP(test_vandps, _mm256_and_ps)
GEN_BINARY_OP(test_vorps, _mm256_or_ps)
GEN_BINARY_OP(test_vxorps, _mm256_xor_ps)
GEN_BINARY_OP(test_vandnps, _mm256_andnot_ps)

// --- Compare (Immediates) ---

NOINLINE __m256 test_vcmpps_eq(__m256 a, __m256 b) {
    return _mm256_cmp_ps(a, b, _CMP_EQ_OQ);
}

NOINLINE __m256 test_vcmpps_lt(__m256 a, __m256 b) {
    return _mm256_cmp_ps(a, b, _CMP_LT_OQ);
}

NOINLINE __m256 test_vcmpps_le(__m256 a, __m256 b) {
    return _mm256_cmp_ps(a, b, _CMP_LE_OQ);
}

NOINLINE __m256 test_vcmpps_neq(__m256 a, __m256 b) {
    return _mm256_cmp_ps(a, b, _CMP_NEQ_OQ);
}

// --- Permute / Shuffle (Immediates) ---

NOINLINE __m256 test_vpermute_ps_0x1B(__m256 a) {
    // vpermilps ymm, ymm, imm8
    return _mm256_permute_ps(a, 0x1B);
}

NOINLINE __m256 test_vpermute2f128_ps_0x01(__m256 a, __m256 b) {
    // vperm2f128 ymm, ymm, ymm, imm8
    return _mm256_permute2f128_ps(a, b, 0x01);
}

NOINLINE __m256 test_vshuffle_ps_0x1B(__m256 a, __m256 b) {
    // vshufps ymm, ymm, ymm, imm8
    return _mm256_shuffle_ps(a, b, 0x1B);
}

NOINLINE __m256 test_vblend_ps_0xAA(__m256 a, __m256 b) {
    // vblendps ymm, ymm, ymm, imm8
    return _mm256_blend_ps(a, b, 0xAA);
}

// --- Conversion ---

NOINLINE __m256i test_vcvtps2dq(__m256 a) {
    return _mm256_cvtps_epi32(a);
}

NOINLINE __m256 test_vcvtdq2ps(__m256i a) {
    return _mm256_cvtepi32_ps(a);
}
