// AVX-512 masked test: vaddps zmm{k1}, zmm, zmm (merge-masking)
#include "../common.h"

__m512 __attribute__((noinline, target("avx512f"))) test_avx512_mask_addps(__m512 src, __mmask16 k, __m512 a, __m512 b) {
    return _mm512_mask_add_ps(src, k, a, b);
}
