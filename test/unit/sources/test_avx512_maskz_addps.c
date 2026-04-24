#include "test_defs.h"

NOINLINE __m512 test_avx512_maskz_addps(__mmask16 k, __m512 a, __m512 b) {
    return _mm512_maskz_add_ps(k, a, b);
}
