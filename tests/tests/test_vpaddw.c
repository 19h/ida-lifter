#include "../common.h"

NOINLINE __m256i test_vpaddw(__m256i a, __m256i b) {
    return _mm256_add_epi16(a, b);
}