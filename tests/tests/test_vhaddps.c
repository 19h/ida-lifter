#include "../common.h"

NOINLINE __m256 test_vhaddps(__m256 a, __m256 b) {
    return _mm256_hadd_ps(a, b);
}