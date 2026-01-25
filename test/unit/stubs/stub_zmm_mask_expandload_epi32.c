// Stub for AVX-512 masked expand load (epi32): __m512i (*)(__m512i src, __mmask16 k, const int *p)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__m512i src, __mmask16 k, const int *p);

int main(void) {
    int input[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) input[i] = i + 1;

    __m512i src = _mm512_set1_epi32(0);
    __mmask16 k = 0x00FF; // Lower half
    __m512i result = TEST_FUNC(src, k, input);
    uint32_t out[16];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
