#include "../common.h"

NOINLINE __m256i test_vpermd(__m256i idx, __m256i a) {
    return _mm256_permutexvar_epi32(idx, a);
}