#include "../common.h"

NOINLINE __m256i test_vpminsd(__m256i a, __m256i b) {
    return _mm256_min_epi32(a, b);
}