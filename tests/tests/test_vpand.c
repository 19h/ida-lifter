#include "../common.h"

NOINLINE __m256i test_vpand(__m256i a, __m256i b) {
    return _mm256_and_si256(a, b);
}