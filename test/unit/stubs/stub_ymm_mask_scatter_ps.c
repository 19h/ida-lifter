// Stub for AVX-512 masked i64 scatter (ps): void (*)(float *base, __mmask8 k, __m512i idx, __m256 v)
#include "../common.h"

// Declare the test function
void TEST_FUNC(float *base, __mmask8 k, __m512i idx, __m256 v);

int main(void) {
    float base[32] __attribute__((aligned(64)));
    for (int i = 0; i < 32; i++) base[i] = 0.0f;

    __mmask8 k = 0x0F; // Lower half
    __m512i idx = _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0);
    __m256 v = _mm256_set1_ps(3.0f);

    TEST_FUNC(base, k, idx, v);
    printf("result[0] = %f, result[1] = %f\n", base[0], base[1]);
    return 0;
}
