// Stub for AVX-512 maskz expand (epi32): __m512i (*)(__mmask16 k, __m512i a)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__mmask16 k, __m512i a);

int main(void) {
    __mmask16 k = 0x00FF; // Lower half
    __m512i a = _mm512_set_epi32(
        16, 15, 14, 13, 12, 11, 10, 9,
        8, 7, 6, 5, 4, 3, 2, 1);

    __m512i result = TEST_FUNC(k, a);
    uint32_t out[16];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
