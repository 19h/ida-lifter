// Stub for AVX-512 masked compress store (epi32): void (*)(int *dst, __mmask16 k, __m512i a)
#include "../common.h"

// Declare the test function
void TEST_FUNC(int *dst, __mmask16 k, __m512i a);

int main(void) {
    int dst[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) dst[i] = 0;

    __mmask16 k = 0x00FF; // Lower half
    __m512i a = _mm512_set1_epi32(7);
    TEST_FUNC(dst, k, a);
    printf("result[0] = %d, result[1] = %d\n", dst[0], dst[1]);
    return 0;
}
