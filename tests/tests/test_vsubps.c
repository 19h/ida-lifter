#include "../common.h"

NOINLINE __m256 test_vsubps(__m256 a, __m256 b) {
    return _mm256_sub_ps(a, b);
}