#include "../common.h"

NOINLINE __m256i test_vpsubw(__m256i a, __m256i b) {
    return _mm256_sub_epi16(a, b);
}