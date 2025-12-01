#include "../common.h"

NOINLINE __m256 test_vpermute_ps_0x1B(__m256 a) {
    return _mm256_permute_ps(a, 0x1B);
}