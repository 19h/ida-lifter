// AVX-512 masked scatter (i64 index, epi64)
#include "test_defs.h"

NOINLINE void test_mask_i64scatter_epi64_zmm(long long *base, __mmask8 k, __m512i idx, __m512i v) {
    _mm512_mask_i64scatter_epi64(base, k, idx, v, 8);
}
