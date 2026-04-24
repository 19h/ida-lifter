// AVX-512 merge-masked FMA: vfmadd132ps zmm{k}, zmm, zmm
#include "test_defs.h"

NOINLINE __m512 test_mask_fmadd_ps_zmm(__m512 a, __mmask16 k, __m512 b, __m512 c) {
    return _mm512_mask_fmadd_ps(a, k, b, c);
}
