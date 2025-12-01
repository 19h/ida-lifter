#include "../common.h"

NOINLINE __m256 test_vroundps_ceil(__m256 a) {
    return _mm256_round_ps(a, _MM_FROUND_TO_POS_INF | _MM_FROUND_NO_EXC);
}