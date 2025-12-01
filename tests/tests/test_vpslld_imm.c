#include "../common.h"

NOINLINE __m256i test_vpslld_imm(__m256i a) {
    return _mm256_slli_epi32(a, 4);
}