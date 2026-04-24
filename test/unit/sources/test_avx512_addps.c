#include "test_defs.h"

NOINLINE __m512 test_avx512_addps(__m512 a, __m512 b) {
    return _mm512_add_ps(a, b);
}
