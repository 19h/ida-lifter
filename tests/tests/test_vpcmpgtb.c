#include "../common.h"

NOINLINE __m256i test_vpcmpgtb(__m256i a, __m256i b) {
    return _mm256_cmpgt_epi8(a, b);
}