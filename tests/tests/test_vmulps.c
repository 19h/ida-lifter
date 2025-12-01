#include "../common.h"

NOINLINE __m256 test_vmulps(__m256 a, __m256 b) {
    return _mm256_mul_ps(a, b);
}