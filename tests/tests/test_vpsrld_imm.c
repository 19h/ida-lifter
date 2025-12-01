#include "../common.h"

NOINLINE __m256i test_vpsrld_imm(__m256i a) {
    return _mm256_srli_epi32(a, 4);
}