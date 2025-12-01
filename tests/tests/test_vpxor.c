#include "../common.h"

NOINLINE __m256i test_vpxor(__m256i a, __m256i b) {
    return _mm256_xor_si256(a, b);
}