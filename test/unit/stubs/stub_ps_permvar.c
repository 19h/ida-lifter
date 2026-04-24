#include "../common.h"

extern __m256 TEST_FUNC(__m256 a, __m256i idx);

int main() {
    __m256 a = _mm256_setzero_ps();
    __m256i idx = _mm256_setr_epi32(0, 2, 4, 6, 1, 3, 5, 7);
    volatile __m256 res = TEST_FUNC(a, idx);
    (void)res;
    return 0;
}
