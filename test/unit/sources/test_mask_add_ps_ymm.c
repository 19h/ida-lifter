// AVX-512VL/AVX10 merge-masked add: vaddps ymm{k}, ymm, ymm
#include "test_defs.h"

#ifdef __AVX512VL__
NOINLINE __m256 test_mask_add_ps_ymm(__m256 src, __mmask8 k, __m256 a, __m256 b) {
    return _mm256_mask_add_ps(src, k, a, b);
}
#endif
