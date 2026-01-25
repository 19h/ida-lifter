// Stub for AVX-512VL masked FMA YMM: __m256 (*)(const __m256 a, __mmask8 k, const __m256 b, const __m256 c)
#include "../common.h"

// Declare the test function
__m256 TEST_FUNC(__m256 a, __mmask8 k, __m256 b, __m256 c);

int main(void) {
    __m256 a = _mm256_set1_ps(1.0f);
    __mmask8 k = 0xAA;  // Alternating bits
    __m256 b = _mm256_set1_ps(2.0f);
    __m256 c = _mm256_set1_ps(3.0f);
    __m256 result = TEST_FUNC(a, k, b, c);

    // Store and print first element to prevent optimization
    float out[8];
    _mm256_storeu_ps(out, result);
    printf("result[0] = %f, result[1] = %f\n", out[0], out[1]);
    return 0;
}
