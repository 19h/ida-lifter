#include "../common.h"

NOINLINE __m256 test_vfmadd231ps(__m256 a, __m256 b, __m256 c) {
    return _mm256_fmadd_ps(a, b, c);
}