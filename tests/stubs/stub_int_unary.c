#include "../common.h"

extern __m256i TEST_FUNC(__m256i a);

int main() {
    __m256i a = _mm256_setzero_si256();
    volatile __m256i res = TEST_FUNC(a);
    (void)res;
    return 0;
}
