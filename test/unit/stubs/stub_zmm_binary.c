// Stub for AVX-512: __m512 (*)(const __m512, const __m512)
#include "../common.h"

// Declare the test function
__m512 TEST_FUNC(__m512 a, __m512 b);

int main(void) {
    __m512 a = _mm512_set1_ps(1.0f);
    __m512 b = _mm512_set1_ps(2.0f);
    __m512 result = TEST_FUNC(a, b);

    // Store and print first element to prevent optimization
    float out[16];
    _mm512_storeu_ps(out, result);
    printf("result[0] = %f\n", out[0]);
    return 0;
}
