// Stub for AVX-512 zero-masked FMA: __m512 (*)(const __mmask16 k, const __m512 a, const __m512 b, const __m512 c)
#include "../common.h"

// Declare the test function
__m512 TEST_FUNC(__mmask16 k, __m512 a, __m512 b, __m512 c);

int main(void) {
    __mmask16 k = 0xAAAA;  // Alternating bits
    __m512 a = _mm512_set1_ps(1.0f);
    __m512 b = _mm512_set1_ps(2.0f);
    __m512 c = _mm512_set1_ps(3.0f);
    __m512 result = TEST_FUNC(k, a, b, c);

    // Store and print first element to prevent optimization
    float out[16];
    _mm512_storeu_ps(out, result);
    printf("result[0] = %f, result[1] = %f\n", out[0], out[1]);
    return 0;
}
