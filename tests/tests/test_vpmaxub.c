#include "../common.h"

NOINLINE __m256i test_vpmaxub(__m256i a, __m256i b) {
    return _mm256_max_epu8(a, b);
}