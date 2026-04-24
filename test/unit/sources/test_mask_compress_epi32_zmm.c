// AVX-512 masked compress (epi32)
#include "test_defs.h"

NOINLINE __m512i test_mask_compress_epi32_zmm(__m512i src, __mmask16 k, __m512i a) {
    return _mm512_mask_compress_epi32(src, k, a);
}
