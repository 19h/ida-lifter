#include "../common.h"

NOINLINE __m256i test_vpandn(__m256i a, __m256i b) {
    return _mm256_andnot_si256(a, b);
}