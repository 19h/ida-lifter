#include "../common.h"

extern __m256 TEST_FUNC(__m256 a, __m256 b, __m256 c);

int main() {
    __m256 z = _mm256_setzero_ps();
    volatile __m256 res = TEST_FUNC(z, z, z);
    (void)res;
    return 0;
}
