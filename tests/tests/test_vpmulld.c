#include "../common.h"

NOINLINE __m256i test_vpmulld(__m256i a, __m256i b) {
    return _mm256_mullo_epi32(a, b);
}