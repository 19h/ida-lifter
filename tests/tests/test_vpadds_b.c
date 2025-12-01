#include "../common.h"

NOINLINE __m256i test_vpadds_b(__m256i a, __m256i b) {
    return _mm256_adds_epi8(a, b);
}