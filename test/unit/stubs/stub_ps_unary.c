#include "../common.h"

// Prototype for the test function (defined via -DTEST_FUNC=name)
extern __m256 TEST_FUNC(__m256 a);

int main() {
    __m256 a = _mm256_setzero_ps();
    // Volatile to prevent optimization
    volatile __m256 res = TEST_FUNC(a);
    (void)res;
    return 0;
}
