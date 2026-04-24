// AVX-512 masked expand load (ps)
#include "test_defs.h"

NOINLINE __m512 test_mask_expandload_ps_zmm(__m512 src, __mmask16 k, const float *p) {
    return _mm512_mask_expandloadu_ps(src, k, p);
}
