// Stub for AVX-512 masked scatter (epi64): void (*)(long long *base, __mmask8 k, __m512i idx, __m512i v)
#include "../common.h"

// Declare the test function
void TEST_FUNC(long long *base, __mmask8 k, __m512i idx, __m512i v);

int main(void) {
    long long base[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) base[i] = 0;

    __mmask8 k = 0x0F; // Lower half
    __m512i idx = _mm512_set_epi64(7, 6, 5, 4, 3, 2, 1, 0);
    __m512i v = _mm512_set1_epi64(3);

    TEST_FUNC(base, k, idx, v);
    printf("result[0] = %lld, result[1] = %lld\n", base[0], base[1]);
    return 0;
}
