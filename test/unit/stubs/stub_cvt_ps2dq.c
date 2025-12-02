#include "../common.h"

extern __m256i TEST_FUNC(__m256 a);

int main() {
    __m256 a = _mm256_setzero_ps();
    volatile __m256i res = TEST_FUNC(a);
    (void)res;
    return 0;
}
