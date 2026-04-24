#include "../common.h"

extern __m128 TEST_FUNC(__m128 a, __m128 b);

int main() {
    __m128 a = _mm_setzero_ps();
    __m128 b = _mm_setzero_ps();
    volatile __m128 res = TEST_FUNC(a, b);
    (void)res;
    return 0;
}
