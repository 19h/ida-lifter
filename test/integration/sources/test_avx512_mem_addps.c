#include "common.h"

NOINLINE __m512 test_avx512_mem_addps(const float *ptr, __m512 a) {
    __m512 mem = _mm512_loadu_ps(ptr);
    return _mm512_add_ps(a, mem);
}

NOINLINE void test_avx512_mem_store(float *dst, __m512 value) {
    _mm512_storeu_ps(dst, value);
}

int main(void) {
    float data[16];
    for (int i = 0; i < 16; i++) {
        data[i] = (float)i;
    }

    __m512 a = _mm512_set1_ps(1.0f);
    __m512 result = test_avx512_mem_addps(data, a);

    float out[16];
    test_avx512_mem_store(out, result);
    printf("out[0]=%f out[1]=%f\n", out[0], out[1]);
    return 0;
}
