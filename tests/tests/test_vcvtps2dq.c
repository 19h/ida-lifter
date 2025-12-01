#include "../common.h"

NOINLINE __m256i test_vcvtps2dq(__m256 a) {
    return _mm256_cvtps_epi32(a);
}