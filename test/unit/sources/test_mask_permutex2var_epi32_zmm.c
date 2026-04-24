// AVX-512 masked permute from two tables (epi32)
#include "test_defs.h"

NOINLINE __m512i test_mask_permutex2var_epi32_zmm(__m512i a, __mmask16 k, __m512i idx, __m512i b) {
    return _mm512_mask_permutex2var_epi32(a, k, idx, b);
}
