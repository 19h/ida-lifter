#include "../common.h"

NOINLINE __m256i test_vpmaxsd(__m256i a, __m256i b) {
    return _mm256_max_epi32(a, b);
}