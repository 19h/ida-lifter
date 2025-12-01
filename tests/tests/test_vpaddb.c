#include "../common.h"

NOINLINE __m256i test_vpaddb(__m256i a, __m256i b) {
    return _mm256_add_epi8(a, b);
}