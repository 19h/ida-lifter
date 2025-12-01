#include "../common.h"

NOINLINE __m256i test_vpor(__m256i a, __m256i b) {
    return _mm256_or_si256(a, b);
}