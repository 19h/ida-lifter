#include "../common.h"

NOINLINE __m256i test_vpsubb(__m256i a, __m256i b) {
    return _mm256_sub_epi8(a, b);
}