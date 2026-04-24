// AVX-512 masked gather (i64 index, pd)
#include "test_defs.h"

NOINLINE __m512d test_mask_i64gather_pd_zmm(__m512d src, __mmask8 k, __m512i idx, const double *base) {
    return _mm512_mask_i64gather_pd(src, k, idx, base, 8);
}
