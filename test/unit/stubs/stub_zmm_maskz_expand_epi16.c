// Stub for AVX-512 maskz expand (epi16): __m512i (*)(__mmask32 k, __m512i a)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__mmask32 k, __m512i a);

int main(void) {
    __mmask32 k = 0x00FF00FF;
    __m512i a = _mm512_set1_epi16(11);

    __m512i result = TEST_FUNC(k, a);
    uint16_t out[32];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
