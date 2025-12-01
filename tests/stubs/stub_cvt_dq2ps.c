#include "../common.h"

extern __m256 TEST_FUNC(__m256i a);

int main() {
    __m256i a = _mm256_setzero_si256();
    volatile __m256 res = TEST_FUNC(a);
    (void)res;
    return 0;
}
