// Stub for AVX-512 masked gather (pd): __m512d (*)(__m512d src, __mmask8 k, __m512i idx, const double *base)
#include "../common.h"

// Declare the test function
__m512d TEST_FUNC(__m512d src, __mmask8 k, __m512i idx, const double *base);

int main(void) {
    double base[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) base[i] = (double)(i + 1);

    __m512d src = _mm512_set1_pd(0.0);
    __mmask8 k = 0xAA; // Alternating bits
    __m512i idx = _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0);

    __m512d result = TEST_FUNC(src, k, idx, base);
    double out[8];
    _mm512_storeu_pd(out, result);
    printf("result[0] = %f, result[1] = %f\n", out[0], out[1]);
    return 0;
}
