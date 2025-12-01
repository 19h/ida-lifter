#include "../common.h"

NOINLINE __m256 test_vrsqrtps(__m256 a) {
    return _mm256_rsqrt_ps(a);
}