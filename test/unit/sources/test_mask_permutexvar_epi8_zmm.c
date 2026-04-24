// AVX-512 masked permute bytes (VBMI)
#include "test_defs.h"

NOINLINE __m512i test_mask_permutexvar_epi8_zmm(__m512i src, __mmask64 k, __m512i idx, __m512i a) {
    return _mm512_mask_permutexvar_epi8(src, k, idx, a);
}
