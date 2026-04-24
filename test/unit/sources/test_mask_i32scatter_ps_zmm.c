// AVX-512 masked scatter (i32, ps)
#include "test_defs.h"

NOINLINE void test_mask_i32scatter_ps_zmm(float *base, __mmask16 k, __m512i idx, __m512 v) {
    _mm512_mask_i32scatter_ps(base, k, idx, v, 4);
}
