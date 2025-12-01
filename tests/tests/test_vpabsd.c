#include "../common.h"

NOINLINE __m256i test_vpabsd(__m256i a) {
    return _mm256_abs_epi32(a);
}