// Stub for AVX-512 masked permutex2var: __m512i (*)(__m512i a, __mmask16 k, __m512i idx, __m512i b)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__m512i a, __mmask16 k, __m512i idx, __m512i b);

int main(void) {
    __m512i a = _mm512_set1_epi32(1);
    __mmask16 k = 0xAAAA; // Alternating bits
    __m512i idx = _mm512_set1_epi32(0);
    __m512i b = _mm512_set1_epi32(2);
    __m512i result = TEST_FUNC(a, k, idx, b);

    uint32_t out[16];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
