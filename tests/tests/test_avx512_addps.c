// AVX-512 test: vaddps zmm, zmm, zmm
#include "../common.h"

__m512 __attribute__((noinline, target("avx512f"))) test_avx512_addps(__m512 a, __m512 b) {
    return _mm512_add_ps(a, b);
}
