// AVX-512 masked test: vaddps zmm{k1}{z}, zmm, zmm (zero-masking)
#include "../common.h"

__m512 __attribute__((noinline, target("avx512f"))) test_avx512_maskz_addps(__mmask16 k, __m512 a, __m512 b) {
    return _mm512_maskz_add_ps(k, a, b);
}
