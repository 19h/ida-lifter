#include "../common.h"

NOINLINE __m256i test_vpalignr_imm(__m256i a, __m256i b) {
    return _mm256_alignr_epi8(a, b, 4);
}