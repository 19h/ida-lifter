#include "common.h"

// --- Macros ---

#define GEN_UNARY_INT(name, intrinsic) \
    NOINLINE __m256i name(__m256i a) { \
        return intrinsic(a); \
    }

#define GEN_BINARY_INT(name, intrinsic) \
    NOINLINE __m256i name(__m256i a, __m256i b) { \
        return intrinsic(a, b); \
    }

// --- Arithmetic ---

GEN_BINARY_INT(test_vpaddb, _mm256_add_epi8)
GEN_BINARY_INT(test_vpaddw, _mm256_add_epi16)
GEN_BINARY_INT(test_vpaddd, _mm256_add_epi32)
GEN_BINARY_INT(test_vpaddq, _mm256_add_epi64)

GEN_BINARY_INT(test_vpsubb, _mm256_sub_epi8)
GEN_BINARY_INT(test_vpsubw, _mm256_sub_epi16)
GEN_BINARY_INT(test_vpsubd, _mm256_sub_epi32)
GEN_BINARY_INT(test_vpsubq, _mm256_sub_epi64)

GEN_BINARY_INT(test_vpadds_b, _mm256_adds_epi8)
GEN_BINARY_INT(test_vpadds_w, _mm256_adds_epi16)

GEN_BINARY_INT(test_vpmulld, _mm256_mullo_epi32)
GEN_BINARY_INT(test_vpmuludq, _mm256_mul_epu32)
GEN_BINARY_INT(test_vpmaddwd, _mm256_madd_epi16)

GEN_BINARY_INT(test_vpmaxub, _mm256_max_epu8)
GEN_BINARY_INT(test_vpminub, _mm256_min_epu8)
GEN_BINARY_INT(test_vpmaxsd, _mm256_max_epi32)
GEN_BINARY_INT(test_vpminsd, _mm256_min_epi32)

GEN_UNARY_INT(test_vpabsd, _mm256_abs_epi32)
GEN_BINARY_INT(test_vpsignb, _mm256_sign_epi8)

// --- Logic ---

GEN_BINARY_INT(test_vpand, _mm256_and_si256)
GEN_BINARY_INT(test_vpor, _mm256_or_si256)
GEN_BINARY_INT(test_vpxor, _mm256_xor_si256)
GEN_BINARY_INT(test_vpandn, _mm256_andnot_si256)

// --- Shifts ---

// Immediate shifts
NOINLINE __m256i test_vpslld_imm(__m256i a) {
    return _mm256_slli_epi32(a, 4);
}

NOINLINE __m256i test_vpsrld_imm(__m256i a) {
    return _mm256_srli_epi32(a, 4);
}

NOINLINE __m256i test_vpsrad_imm(__m256i a) {
    return _mm256_srai_epi32(a, 4);
}

// Variable shifts (AVX2 specific)
GEN_BINARY_INT(test_vpsllv_d, _mm256_sllv_epi32)
GEN_BINARY_INT(test_vpsrlv_d, _mm256_srlv_epi32)
GEN_BINARY_INT(test_vpsrav_d, _mm256_srav_epi32)

// --- Compare ---

GEN_BINARY_INT(test_vpcmpeqb, _mm256_cmpeq_epi8)
GEN_BINARY_INT(test_vpcmpgtb, _mm256_cmpgt_epi8)
GEN_BINARY_INT(test_vpcmpeqd, _mm256_cmpeq_epi32)
GEN_BINARY_INT(test_vpcmpgtd, _mm256_cmpgt_epi32)
GEN_BINARY_INT(test_vpcmpeqq, _mm256_cmpeq_epi64)
GEN_BINARY_INT(test_vpcmpgtq, _mm256_cmpgt_epi64)

// --- Permute / Shuffle ---

GEN_BINARY_INT(test_vpshufb, _mm256_shuffle_epi8)

NOINLINE __m256i test_vpermd(__m256i idx, __m256i a) {
    return _mm256_permutexvar_epi32(idx, a);
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
