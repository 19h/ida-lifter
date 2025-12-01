#include "../common.h"

NOINLINE __m256i test_vpsllv_d(__m256i a, __m256i count) {
    return _mm256_sllv_epi32(a, count);
}