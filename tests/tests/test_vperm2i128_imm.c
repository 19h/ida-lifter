#include "../common.h"

NOINLINE __m256i test_vperm2i128_imm(__m256i a, __m256i b) {
    return _mm256_permute2x128_si256(a, b, 0x01);
}