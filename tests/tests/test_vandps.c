#include "../common.h"

NOINLINE __m256 test_vandps(__m256 a, __m256 b) {
    return _mm256_and_ps(a, b);
}