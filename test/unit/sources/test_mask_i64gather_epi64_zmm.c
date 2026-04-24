// AVX-512 masked gather (i64 index, epi64)
#include "test_defs.h"

NOINLINE __m512i test_mask_i64gather_epi64_zmm(__m512i src, __mmask8 k, __m512i idx, const long long *base) {
    return _mm512_mask_i64gather_epi64(src, k, idx, base, 8);
}
