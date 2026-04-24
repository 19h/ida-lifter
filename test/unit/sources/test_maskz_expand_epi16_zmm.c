// AVX-512 maskz expand (epi16, vbmi2)
#include "test_defs.h"

NOINLINE __m512i test_maskz_expand_epi16_zmm(__mmask32 k, __m512i a) {
    return _mm512_maskz_expand_epi16(k, a);
}
