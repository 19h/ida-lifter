#include "test_defs.h"

NOINLINE __m128 test_vmovlhps(__m128 a, __m128 b) {
    return _mm_movelh_ps(a, b);
}
