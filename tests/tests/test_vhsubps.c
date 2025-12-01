#include "../common.h"

NOINLINE __m256 test_vhsubps(__m256 a, __m256 b) {
    return _mm256_hsub_ps(a, b);
}