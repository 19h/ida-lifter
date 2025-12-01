#include "../common.h"

NOINLINE __m256i test_vpermq_imm(__m256i a) {
    return _mm256_permute4x64_epi64(a, 0x1B);
}