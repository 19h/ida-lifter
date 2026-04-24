// AVX-512 maskz expand (epi8, vbmi2)
#include "test_defs.h"

NOINLINE __m512i test_maskz_expand_epi8_zmm(__mmask64 k, __m512i a) {
    return _mm512_maskz_expand_epi8(k, a);
}
