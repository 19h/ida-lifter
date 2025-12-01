#include "../common.h"

NOINLINE __m256 test_vpermute2f128_ps_0x01(__m256 a, __m256 b) {
    return _mm256_permute2f128_ps(a, b, 0x01);
}