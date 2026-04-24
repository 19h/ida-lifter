#include "test_defs.h"

NOINLINE __m256i test_vpermd(__m256i idx, __m256i a) {
    return _mm256_permutevar8x32_epi32(a, idx);
}
