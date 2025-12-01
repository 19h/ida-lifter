#include "../common.h"

NOINLINE __m256 test_vandnps(__m256 a, __m256 b) {
    return _mm256_andnot_ps(a, b);
}