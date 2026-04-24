// AVX-512 masked scatter (i64 index, ps)
#include "test_defs.h"

NOINLINE void test_mask_i64scatter_ps_ymm(float *base, __mmask8 k, __m512i idx, __m256 v) {
    _mm512_mask_i64scatter_ps(base, k, idx, v, 4);
}
