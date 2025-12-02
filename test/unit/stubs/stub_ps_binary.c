#include "../common.h"

extern __m256 TEST_FUNC(__m256 a, __m256 b);

int main() {
    __m256 a = _mm256_setzero_ps();
    __m256 b = _mm256_setzero_ps();
    volatile __m256 res = TEST_FUNC(a, b);
    (void)res;
    return 0;
}
