#include "../common.h"

NOINLINE __m256i test_vpminub(__m256i a, __m256i b) {
    return _mm256_min_epu8(a, b);
}