#include "../common.h"

NOINLINE __m256i test_vpsrad_imm(__m256i a) {
    return _mm256_srai_epi32(a, 4);
}