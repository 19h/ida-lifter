// AVX-512 zero-masked FMA: vfmadd132ps zmm{k}{z}, zmm, zmm
#include "test_defs.h"

NOINLINE __m512 test_maskz_fmadd_ps_zmm(__mmask16 k, __m512 a, __m512 b, __m512 c) {
    return _mm512_maskz_fmadd_ps(k, a, b, c);
}
