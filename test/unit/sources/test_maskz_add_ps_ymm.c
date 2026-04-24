// AVX-512VL/AVX10 zero-masked add: vaddps ymm{k}{z}, ymm, ymm
#include "test_defs.h"

#ifdef __AVX512VL__
NOINLINE __m256 test_maskz_add_ps_ymm(__mmask8 k, __m256 a, __m256 b) {
    return _mm256_maskz_add_ps(k, a, b);
}
#endif
