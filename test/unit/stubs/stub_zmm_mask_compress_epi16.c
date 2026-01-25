// Stub for AVX-512 masked compress (epi16): __m512i (*)(__m512i src, __mmask32 k, __m512i a)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__m512i src, __mmask32 k, __m512i a);

int main(void) {
    __m512i src = _mm512_set1_epi16(0);
    __mmask32 k = 0x00FF00FF;
    __m512i a = _mm512_set1_epi16(7);

    __m512i result = TEST_FUNC(src, k, a);
    uint16_t out[32];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
