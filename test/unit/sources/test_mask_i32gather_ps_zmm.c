// AVX-512 masked gather (i32, ps)
#include "test_defs.h"

NOINLINE __m512 test_mask_i32gather_ps_zmm(__m512 src, __mmask16 k, __m512i idx, const float *base) {
    return _mm512_mask_i32gather_ps(src, k, idx, base, 4);
}
