// AVX-512 masked compress (epi16, vbmi2)
#include "test_defs.h"

NOINLINE __m512i test_mask_compress_epi16_zmm(__m512i src, __mmask32 k, __m512i a) {
    return _mm512_mask_compress_epi16(src, k, a);
}
