// AVX-512 masked expand load (epi32)
#include "test_defs.h"

NOINLINE __m512i test_mask_expandload_epi32_zmm(__m512i src, __mmask16 k, const int *p) {
    return _mm512_mask_expandloadu_epi32(src, k, p);
}
