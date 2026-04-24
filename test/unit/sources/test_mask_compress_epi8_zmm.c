// AVX-512 masked compress (epi8, vbmi2)
#include "test_defs.h"

NOINLINE __m512i test_mask_compress_epi8_zmm(__m512i src, __mmask64 k, __m512i a) {
    return _mm512_mask_compress_epi8(src, k, a);
}
