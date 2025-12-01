#include "../common.h"

NOINLINE __m256 test_vshuffle_ps_0x1B(__m256 a, __m256 b) {
    return _mm256_shuffle_ps(a, b, 0x1B);
}