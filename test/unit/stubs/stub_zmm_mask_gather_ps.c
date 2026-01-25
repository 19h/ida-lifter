// Stub for AVX-512 masked gather (ps): __m512 (*)(__m512 src, __mmask16 k, __m512i idx, const float *base)
#include "../common.h"

// Declare the test function
__m512 TEST_FUNC(__m512 src, __mmask16 k, __m512i idx, const float *base);

int main(void) {
    float base[32] __attribute__((aligned(64)));
    for (int i = 0; i < 32; i++) base[i] = (float)(i + 1);

    __m512 src = _mm512_set1_ps(0.0f);
    __mmask16 k = 0xAAAA; // Alternating bits
    __m512i idx = _mm512_set_epi32(
        15, 14, 13, 12, 11, 10, 9, 8,
        7, 6, 5, 4, 3, 2, 1, 0);

    __m512 result = TEST_FUNC(src, k, idx, base);
    float out[16];
    _mm512_storeu_ps(out, result);
    printf("result[0] = %f, result[1] = %f\n", out[0], out[1]);
    return 0;
}
