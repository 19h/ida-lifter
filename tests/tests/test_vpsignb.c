#include "../common.h"

NOINLINE __m256i test_vpsignb(__m256i a, __m256i b) {
    return _mm256_sign_epi8(a, b);
}