// AVX-512VL masked bitwise AND (YMM)
#include "test_defs.h"

#ifdef __AVX512VL__
NOINLINE __m256 test_mask_and_ps_ymm(__m256 src, __mmask8 k, __m256 a, __m256 b) {
    return _mm256_mask_and_ps(src, k, a, b);
}
#endif
