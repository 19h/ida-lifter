// Stub for AVX-512 maskz expand (epi8): __m512i (*)(__mmask64 k, __m512i a)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__mmask64 k, __m512i a);

int main(void) {
    __mmask64 k = 0x00FF00FF00FF00FFULL;
    __m512i a = _mm512_set1_epi8(9);

    __m512i result = TEST_FUNC(k, a);
    uint8_t out[64];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
