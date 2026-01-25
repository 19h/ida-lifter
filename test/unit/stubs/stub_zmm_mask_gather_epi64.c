// Stub for AVX-512 masked gather (epi64): __m512i (*)(__m512i src, __mmask8 k, __m512i idx, const long long *base)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__m512i src, __mmask8 k, __m512i idx, const long long *base);

int main(void) {
    long long base[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) base[i] = (long long)(i + 1);

    __m512i src = _mm512_set1_epi64(0);
    __mmask8 k = 0xAA; // Alternating bits
    __m512i idx = _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0);

    __m512i result = TEST_FUNC(src, k, idx, base);
    long long out[8];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %lld, result[1] = %lld\n", out[0], out[1]);
    return 0;
}
