#include "../common.h"

NOINLINE __m256 test_vaddps(__m256 a, __m256 b) {
    return _mm256_add_ps(a, b);
}