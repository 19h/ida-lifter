#include "common.h"

NOINLINE __m512 test_avx512_addps(__m512 a, __m512 b) {
    return _mm512_add_ps(a, b);
}

int main(void) {
    __m512 a = _mm512_set1_ps(1.0f);
    __m512 b = _mm512_set1_ps(2.0f);
    __m512 result = test_avx512_addps(a, b);

    float out[16];
    _mm512_storeu_ps(out, result);
    printf("result[0]=%f\n", out[0]);
    return 0;
}
