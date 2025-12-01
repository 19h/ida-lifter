#include "../common.h"

NOINLINE __m256i test_vpmuludq(__m256i a, __m256i b) {
    return _mm256_mul_epu32(a, b);
}