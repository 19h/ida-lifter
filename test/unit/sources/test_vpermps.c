#include "test_defs.h"

NOINLINE __m256 test_vpermps(__m256 a, __m256i idx) {
    return _mm256_permutevar8x32_ps(a, idx);
}
