// Stub for AVX-512 masked i64 gather (ps): __m256 (*)(__m256 src, __mmask8 k, __m512i idx, const float *base)
#include "../common.h"

// Declare the test function
__m256 TEST_FUNC(__m256 src, __mmask8 k, __m512i idx, const float *base);

int main(void) {
    float base[32] __attribute__((aligned(64)));
    for (int i = 0; i < 32; i++) base[i] = (float)(i + 1);

    __m256 src = _mm256_set1_ps(0.0f);
    __mmask8 k = 0xAA; // Alternating bits
    __m512i idx = _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0);

    __m256 result = TEST_FUNC(src, k, idx, base);
    float out[8];
    _mm256_storeu_ps(out, result);
    printf("result[0] = %f, result[1] = %f\n", out[0], out[1]);
    return 0;
}
