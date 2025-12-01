#include "../common.h"

NOINLINE __m256 test_vminps(__m256 a, __m256 b) {
    return _mm256_min_ps(a, b);
}