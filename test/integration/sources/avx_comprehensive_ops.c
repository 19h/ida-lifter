#include "common.h"

#define DEFINE_PS_UNARY(name, intrinsic) \
    NOINLINE __m256 name(__m256 a) { return intrinsic(a); }
#define DEFINE_PS_BINARY(name, intrinsic) \
    NOINLINE __m256 name(__m256 a, __m256 b) { return intrinsic(a, b); }
#define DEFINE_PS_TERNARY(name, intrinsic) \
    NOINLINE __m256 name(__m256 a, __m256 b, __m256 c) { return intrinsic(a, b, c); }

#define DEFINE_INT_UNARY(name, intrinsic) \
    NOINLINE __m256i name(__m256i a) { return intrinsic(a); }
#define DEFINE_INT_BINARY(name, intrinsic) \
    NOINLINE __m256i name(__m256i a, __m256i b) { return intrinsic(a, b); }

// --- AVX (Float) ---
DEFINE_PS_BINARY(test_vaddps, _mm256_add_ps)
DEFINE_PS_BINARY(test_vsubps, _mm256_sub_ps)
DEFINE_PS_BINARY(test_vmulps, _mm256_mul_ps)
DEFINE_PS_BINARY(test_vdivps, _mm256_div_ps)
DEFINE_PS_UNARY(test_vsqrtps, _mm256_sqrt_ps)
DEFINE_PS_UNARY(test_vrsqrtps, _mm256_rsqrt_ps)
DEFINE_PS_UNARY(test_vrcpps, _mm256_rcp_ps)
DEFINE_PS_BINARY(test_vmaxps, _mm256_max_ps)
DEFINE_PS_BINARY(test_vminps, _mm256_min_ps)
DEFINE_PS_BINARY(test_vhaddps, _mm256_hadd_ps)
DEFINE_PS_BINARY(test_vhsubps, _mm256_hsub_ps)
DEFINE_PS_TERNARY(test_vfmadd231ps, _mm256_fmadd_ps)

DEFINE_PS_BINARY(test_vandps, _mm256_and_ps)
DEFINE_PS_BINARY(test_vorps, _mm256_or_ps)
DEFINE_PS_BINARY(test_vxorps, _mm256_xor_ps)
DEFINE_PS_BINARY(test_vandnps, _mm256_andnot_ps)

NOINLINE __m256 test_vroundps_floor(__m256 a) {
    return _mm256_round_ps(a, _MM_FROUND_TO_NEG_INF | _MM_FROUND_NO_EXC);
}

NOINLINE __m256 test_vroundps_ceil(__m256 a) {
    return _mm256_round_ps(a, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
}

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

NOINLINE __m256 test_vpermute_ps_0x1B(__m256 a) {
    return _mm256_permute_ps(a, 0x1B);
}

NOINLINE __m256 test_vpermute2f128_ps_0x01(__m256 a, __m256 b) {
    return _mm256_permute2f128_ps(a, b, 0x01);
}

NOINLINE __m256 test_vshuffle_ps_0x1B(__m256 a, __m256 b) {
    return _mm256_shuffle_ps(a, b, 0x1B);
}

NOINLINE __m256 test_vblend_ps_0xAA(__m256 a, __m256 b) {
    return _mm256_blend_ps(a, b, 0xAA);
}

NOINLINE __m256i test_vcvtps2dq(__m256 a) {
    return _mm256_cvtps_epi32(a);
}

NOINLINE __m256 test_vcvtdq2ps(__m256i a) {
    return _mm256_cvtepi32_ps(a);
}

// --- AVX2 (Integer) ---
DEFINE_INT_BINARY(test_vpaddb, _mm256_add_epi8)
DEFINE_INT_BINARY(test_vpaddw, _mm256_add_epi16)
DEFINE_INT_BINARY(test_vpaddd, _mm256_add_epi32)
DEFINE_INT_BINARY(test_vpaddq, _mm256_add_epi64)

DEFINE_INT_BINARY(test_vpsubb, _mm256_sub_epi8)
DEFINE_INT_BINARY(test_vpsubw, _mm256_sub_epi16)
DEFINE_INT_BINARY(test_vpsubd, _mm256_sub_epi32)
DEFINE_INT_BINARY(test_vpsubq, _mm256_sub_epi64)

DEFINE_INT_BINARY(test_vpadds_b, _mm256_adds_epi8)
DEFINE_INT_BINARY(test_vpadds_w, _mm256_adds_epi16)

DEFINE_INT_BINARY(test_vpmulld, _mm256_mullo_epi32)
DEFINE_INT_BINARY(test_vpmuludq, _mm256_mul_epu32)
DEFINE_INT_BINARY(test_vpmaddwd, _mm256_madd_epi16)

DEFINE_INT_BINARY(test_vpmaxub, _mm256_max_epu8)
DEFINE_INT_BINARY(test_vpminub, _mm256_min_epu8)
DEFINE_INT_BINARY(test_vpmaxsd, _mm256_max_epi32)
DEFINE_INT_BINARY(test_vpminsd, _mm256_min_epi32)

DEFINE_INT_UNARY(test_vpabsd, _mm256_abs_epi32)
DEFINE_INT_BINARY(test_vpsignb, _mm256_sign_epi8)

DEFINE_INT_BINARY(test_vpand, _mm256_and_si256)
DEFINE_INT_BINARY(test_vpor, _mm256_or_si256)
DEFINE_INT_BINARY(test_vpxor, _mm256_xor_si256)
DEFINE_INT_BINARY(test_vpandn, _mm256_andnot_si256)

NOINLINE __m256i test_vpslld_imm(__m256i a) {
    return _mm256_slli_epi32(a, 4);
}

NOINLINE __m256i test_vpsrld_imm(__m256i a) {
    return _mm256_srli_epi32(a, 5);
}

NOINLINE __m256i test_vpsrad_imm(__m256i a) {
    return _mm256_srai_epi32(a, 6);
}

NOINLINE __m256i test_vpsllv_d(__m256i a, __m256i count) {
    return _mm256_sllv_epi32(a, count);
}

NOINLINE __m256i test_vpsrlv_d(__m256i a, __m256i count) {
    return _mm256_srlv_epi32(a, count);
}

NOINLINE __m256i test_vpsrav_d(__m256i a, __m256i count) {
    return _mm256_srav_epi32(a, count);
}

DEFINE_INT_BINARY(test_vpcmpeqb, _mm256_cmpeq_epi8)
DEFINE_INT_BINARY(test_vpcmpgtb, _mm256_cmpgt_epi8)
DEFINE_INT_BINARY(test_vpcmpeqd, _mm256_cmpeq_epi32)
DEFINE_INT_BINARY(test_vpcmpgtd, _mm256_cmpgt_epi32)
DEFINE_INT_BINARY(test_vpcmpeqq, _mm256_cmpeq_epi64)
DEFINE_INT_BINARY(test_vpcmpgtq, _mm256_cmpgt_epi64)

DEFINE_INT_BINARY(test_vpshufb, _mm256_shuffle_epi8)

NOINLINE __m256i test_vpermd(__m256i idx, __m256i a) {
    return _mm256_permutevar8x32_epi32(a, idx);
}

NOINLINE __m256i test_vpermq_imm(__m256i a) {
    return _mm256_permute4x64_epi64(a, 0x1B);
}

NOINLINE __m256i test_vperm2i128_imm(__m256i a, __m256i b) {
    return _mm256_permute2x128_si256(a, b, 0x01);
}

NOINLINE __m256i test_vpalignr_imm(__m256i a, __m256i b) {
    return _mm256_alignr_epi8(a, b, 4);
}

NOINLINE __m256i test_vpblendd_imm(__m256i a, __m256i b) {
    return _mm256_blend_epi32(a, b, 0xAA);
}

#ifndef NO_AVX512
NOINLINE __m512 test_zmm_addps(__m512 a, __m512 b) {
    return _mm512_add_ps(a, b);
}

NOINLINE __m512 test_zmm_mulps(__m512 a, __m512 b) {
    return _mm512_mul_ps(a, b);
}

NOINLINE __m512 test_zmm_subps(__m512 a, __m512 b) {
    return _mm512_sub_ps(a, b);
}

NOINLINE __m512 test_zmm_maxps(__m512 a, __m512 b) {
    return _mm512_max_ps(a, b);
}

NOINLINE __m512 test_zmm_minps(__m512 a, __m512 b) {
    return _mm512_min_ps(a, b);
}

NOINLINE __m512i test_zmm_and(__m512i a, __m512i b) {
    return _mm512_and_si512(a, b);
}

NOINLINE __m512i test_zmm_add_epi32(__m512i a, __m512i b) {
    return _mm512_add_epi32(a, b);
}

NOINLINE __m512i test_zmm_xor(__m512i a, __m512i b) {
    return _mm512_xor_si512(a, b);
}

NOINLINE __m512i test_zmm_or(__m512i a, __m512i b) {
    return _mm512_or_si512(a, b);
}

NOINLINE __m512i test_zmm_slli_epi32(__m512i a) {
    return _mm512_slli_epi32(a, 4);
}
#endif
