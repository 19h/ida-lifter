#include "test_defs.h"

NOINLINE __m512 test_avx512_mask_addps(__m512 src, __mmask16 k, __m512 a, __m512 b) {
    return _mm512_mask_add_ps(src, k, a, b);
}
