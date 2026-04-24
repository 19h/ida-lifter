// AVX-512 masked compress store (ps)
#include "test_defs.h"

NOINLINE void test_mask_compressstore_ps_zmm(float *dst, __mmask16 k, __m512 a) {
    _mm512_mask_compressstoreu_ps(dst, k, a);
}
