#include "../common.h"

NOINLINE __m256 test_vrcpps(__m256 a) {
    return _mm256_rcp_ps(a);
}