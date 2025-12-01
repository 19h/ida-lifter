#include "../common.h"

NOINLINE __m256 test_vmaxps(__m256 a, __m256 b) {
    return _mm256_max_ps(a, b);
}