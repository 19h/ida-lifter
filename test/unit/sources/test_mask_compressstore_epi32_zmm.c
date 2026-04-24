// AVX-512 masked compress store (epi32)
#include "test_defs.h"

NOINLINE void test_mask_compressstore_epi32_zmm(int *dst, __mmask16 k, __m512i a) {
    _mm512_mask_compressstoreu_epi32(dst, k, a);
}
