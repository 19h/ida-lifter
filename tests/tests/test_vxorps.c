#include "../common.h"

NOINLINE __m256 test_vxorps(__m256 a, __m256 b) {
    return _mm256_xor_ps(a, b);
}