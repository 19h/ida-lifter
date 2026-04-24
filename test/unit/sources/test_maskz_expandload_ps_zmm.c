// AVX-512 masked expand load with zeroing (ps)
#include "test_defs.h"

NOINLINE __m512 test_maskz_expandload_ps_zmm(__mmask16 k, const float *p) {
    return _mm512_maskz_expandloadu_ps(k, p);
}
