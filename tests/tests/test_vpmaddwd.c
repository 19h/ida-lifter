#include "../common.h"

NOINLINE __m256i test_vpmaddwd(__m256i a, __m256i b) {
    return _mm256_madd_epi16(a, b);
}