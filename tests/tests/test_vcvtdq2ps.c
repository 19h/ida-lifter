#include "../common.h"

NOINLINE __m256 test_vcvtdq2ps(__m256i a) {
    return _mm256_cvtepi32_ps(a);
}