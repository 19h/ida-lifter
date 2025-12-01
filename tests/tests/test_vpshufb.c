#include "../common.h"

NOINLINE __m256i test_vpshufb(__m256i a, __m256i b) {
    return _mm256_shuffle_epi8(a, b);
}