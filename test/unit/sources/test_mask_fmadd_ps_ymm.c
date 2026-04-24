// AVX-512VL/AVX10 merge-masked FMA: vfmadd132ps ymm{k}, ymm, ymm
#include "test_defs.h"

#ifdef __AVX512VL__
NOINLINE __m256 test_mask_fmadd_ps_ymm(__m256 a, __mmask8 k, __m256 b, __m256 c) {
    return _mm256_mask_fmadd_ps(a, k, b, c);
}
#endif
