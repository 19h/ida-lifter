// Stub for AVX-512 masked compress (epi8): __m512i (*)(__m512i src, __mmask64 k, __m512i a)
#include "../common.h"

// Declare the test function
__m512i TEST_FUNC(__m512i src, __mmask64 k, __m512i a);

int main(void) {
    __m512i src = _mm512_set1_epi8(0);
    __mmask64 k = 0x00FF00FF00FF00FFULL;
    __m512i a = _mm512_set1_epi8(5);

    __m512i result = TEST_FUNC(src, k, a);
    uint8_t out[64];
    _mm512_storeu_si512(out, result);
    printf("result[0] = %u, result[1] = %u\n", out[0], out[1]);
    return 0;
}
