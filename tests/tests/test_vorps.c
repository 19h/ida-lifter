#include "../common.h"

NOINLINE __m256 test_vorps(__m256 a, __m256 b) {
    return _mm256_or_ps(a, b);
}