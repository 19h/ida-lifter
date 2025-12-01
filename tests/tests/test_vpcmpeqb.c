#include "../common.h"

NOINLINE __m256i test_vpcmpeqb(__m256i a, __m256i b) {
    return _mm256_cmpeq_epi8(a, b);
}