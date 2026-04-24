// AVX-512 masked ternary logic
#include "test_defs.h"

NOINLINE __m512i test_mask_ternarylogic_epi32_zmm(
    __m512i src,
    __mmask16 k,
    __m512i b,
    __m512i c) {
    return _mm512_mask_ternarylogic_epi32(src, k, b, c, 0x96);
}
