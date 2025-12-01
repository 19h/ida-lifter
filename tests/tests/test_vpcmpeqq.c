#include "../common.h"

NOINLINE __m256i test_vpcmpeqq(__m256i a, __m256i b) {
    return _mm256_cmpeq_epi64(a, b);
}