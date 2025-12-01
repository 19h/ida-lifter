#include "../common.h"

NOINLINE __m256i test_vpaddq(__m256i a, __m256i b) {
    return _mm256_add_epi64(a, b);
}