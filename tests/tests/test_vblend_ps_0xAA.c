#include "../common.h"

NOINLINE __m256 test_vblend_ps_0xAA(__m256 a, __m256 b) {
    return _mm256_blend_ps(a, b, 0xAA);
}