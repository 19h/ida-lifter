// Stub for AVX-512 masked scatter (pd): void (*)(double *base, __mmask8 k, __m512i idx, __m512d v)
#include "../common.h"

// Declare the test function
void TEST_FUNC(double *base, __mmask8 k, __m512i idx, __m512d v);

int main(void) {
    double base[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) base[i] = 0.0;

    __mmask8 k = 0x0F; // Lower half
    __m512i idx = _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0);
    __m512d v = _mm512_set1_pd(2.0);

    TEST_FUNC(base, k, idx, v);
    printf("result[0] = %f, result[1] = %f\n", base[0], base[1]);
    return 0;
}
