// Stub for AVX-512 maskz expand load: __m512 (*)(__mmask16 k, const float *p)
#include "../common.h"

// Declare the test function
__m512 TEST_FUNC(__mmask16 k, const float *p);

int main(void) {
    float input[16] __attribute__((aligned(64)));
    for (int i = 0; i < 16; i++) input[i] = (float)(i + 1);
    __mmask16 k = 0xAAAA; // Alternating bits
    __m512 result = TEST_FUNC(k, input);

    float out[16];
    _mm512_storeu_ps(out, result);
    printf("result[0] = %f, result[1] = %f\n", out[0], out[1]);
    return 0;
}
