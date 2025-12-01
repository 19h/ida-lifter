#include "../common.h"

NOINLINE __m256i test_vpcmpeqd(__m256i a, __m256i b) {
    return _mm256_cmpeq_epi32(a, b);
}