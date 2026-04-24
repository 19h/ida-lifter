// AVX-512 masked scatter (i64 index, pd)
#include "test_defs.h"

NOINLINE void test_mask_i64scatter_pd_zmm(double *base, __mmask8 k, __m512i idx, __m512d v) {
    _mm512_mask_i64scatter_pd(base, k, idx, v, 8);
}
