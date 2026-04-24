// AVX-512VL/AVX10 zero-masked FMA: vfmadd132ps ymm{k}{z}, ymm, ymm
#include "test_defs.h"

#ifdef __AVX512VL__
NOINLINE __m256 test_maskz_fmadd_ps_ymm(__mmask8 k, __m256 a, __m256 b, __m256 c) {
    return _mm256_maskz_fmadd_ps(k, a, b, c);
}
#endif
