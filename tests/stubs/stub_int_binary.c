#include "../common.h"

extern __m256i TEST_FUNC(__m256i a, __m256i b);

int main() {
    __m256i a = _mm256_setzero_si256();
    __m256i b = _mm256_setzero_si256();
    volatile __m256i res = TEST_FUNC(a, b);
    (void)res;
    return 0;
}
