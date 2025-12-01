#include "../common.h"

NOINLINE __m256i test_vpsubd(__m256i a, __m256i b) {
    return _mm256_sub_epi32(a, b);
}