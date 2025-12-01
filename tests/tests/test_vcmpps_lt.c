#include "../common.h"

NOINLINE __m256 test_vcmpps_lt(__m256 a, __m256 b) {
    return _mm256_cmp_ps(a, b, _CMP_LT_OQ);
}