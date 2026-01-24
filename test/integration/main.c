// main.c
#include "common.h"

#ifndef NO_AVX512
__m512 test_zmm_addps(__m512 a, __m512 b);
__m512 test_zmm_mulps(__m512 a, __m512 b);
__m512 test_zmm_subps(__m512 a, __m512 b);
__m512 test_zmm_maxps(__m512 a, __m512 b);
__m512 test_zmm_minps(__m512 a, __m512 b);
__m512i test_zmm_and(__m512i a, __m512i b);
__m512i test_zmm_add_epi32(__m512i a, __m512i b);
__m512i test_zmm_xor(__m512i a, __m512i b);
__m512i test_zmm_or(__m512i a, __m512i b);
__m512i test_zmm_slli_epi32(__m512i a);
#endif

int main(void) {
    __m256 a = _mm256_set1_ps(1.0f);
    __m256 b = _mm256_set1_ps(2.0f);
    __m256 c = test_vaddps(a, b);
    __m256 d = test_vfmadd231ps(a, b, c);

    float out_ps[8];
    _mm256_storeu_ps(out_ps, d);
    printf("ps=%f\n", out_ps[0]);

    __m256i ia = _mm256_set1_epi32(1);
    __m256i ib = _mm256_set1_epi32(2);
    __m256i ic = test_vpaddd(ia, ib);
    int out_i[8];
    _mm256_storeu_si256((__m256i *)out_i, ic);
    printf("epi32=%d\n", out_i[0]);

    __m256i count = _mm256_set1_epi32(1);
    __m256i shifted = test_vpsllv_d(ia, count);
    _mm256_storeu_si256((__m256i *)out_i, shifted);
    printf("shift=%d\n", out_i[0]);

    __m256i cvt = test_vcvtps2dq(a);
    _mm256_storeu_si256((__m256i *)out_i, cvt);
    printf("cvt=%d\n", out_i[0]);

    __m256 back = test_vcvtdq2ps(cvt);
    _mm256_storeu_ps(out_ps, back);
    printf("back=%f\n", out_ps[0]);

#ifndef NO_AVX512
    __m512 za = _mm512_set1_ps(1.0f);
    __m512 zb = _mm512_set1_ps(2.0f);
    __m512 zc = test_zmm_addps(za, zb);
    float out_z[16];
    _mm512_storeu_ps(out_z, zc);
    printf("zmm=%f\n", out_z[0]);

    __m512 zd = test_zmm_mulps(za, zb);
    _mm512_storeu_ps(out_z, zd);
    printf("zmm_mul=%f\n", out_z[0]);

    __m512 ze = test_zmm_subps(za, zb);
    _mm512_storeu_ps(out_z, ze);
    printf("zmm_sub=%f\n", out_z[0]);

    __m512 zf = test_zmm_maxps(za, zb);
    _mm512_storeu_ps(out_z, zf);
    printf("zmm_max=%f\n", out_z[0]);

    __m512 zg = test_zmm_minps(za, zb);
    _mm512_storeu_ps(out_z, zg);
    printf("zmm_min=%f\n", out_z[0]);

    __m512i zi = _mm512_set1_epi32(3);
    __m512i zj = _mm512_set1_epi32(1);
    __m512i zk = test_zmm_and(zi, zj);
    int out_z_i[16];
    _mm512_storeu_si512(out_z_i, zk);
    printf("zmm_and=%d\n", out_z_i[0]);

    __m512i zm = test_zmm_add_epi32(zi, zj);
    _mm512_storeu_si512(out_z_i, zm);
    printf("zmm_add=%d\n", out_z_i[0]);

    __m512i zn = test_zmm_xor(zi, zj);
    _mm512_storeu_si512(out_z_i, zn);
    printf("zmm_xor=%d\n", out_z_i[0]);

    __m512i zo = test_zmm_or(zi, zj);
    _mm512_storeu_si512(out_z_i, zo);
    printf("zmm_or=%d\n", out_z_i[0]);

    __m512i zl = test_zmm_slli_epi32(zi);
    _mm512_storeu_si512(out_z_i, zl);
    printf("zmm_shift=%d\n", out_z_i[0]);
#endif

    return 0;
}
