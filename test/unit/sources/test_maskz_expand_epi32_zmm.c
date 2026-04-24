// AVX-512 maskz expand (epi32)
#include "test_defs.h"

NOINLINE __m512i test_maskz_expand_epi32_zmm(__mmask16 k, __m512i a) {
    return _mm512_maskz_expand_epi32(k, a);
}
