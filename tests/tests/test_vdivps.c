#include "../common.h"

NOINLINE __m256 test_vdivps(__m256 a, __m256 b) {
    return _mm256_div_ps(a, b);
}