#include "../common.h"

NOINLINE __m256 test_vsqrtps(__m256 a) {
    return _mm256_sqrt_ps(a);
}