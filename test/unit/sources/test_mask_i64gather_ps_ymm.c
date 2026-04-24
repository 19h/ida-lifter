// AVX-512 masked gather (i64 index, ps)
#include "test_defs.h"

NOINLINE __m256 test_mask_i64gather_ps_ymm(__m256 src, __mmask8 k, __m512i idx, const float *base) {
    return _mm512_mask_i64gather_ps(src, k, idx, base, 4);
}
