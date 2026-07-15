#include <immintrin.h>
#include <stdint.h>

/* cross-function state pools (force loads/stores + dataflow) */
__m512 g_v512f[12];
__m512d g_v512d[12];
__m512i g_v512i[12];
__m256 g_v256f[12];
__m256d g_v256d[12];
__m256i g_v256i[12];
__m128 g_v128f[12];
__m128d g_v128d[12];
__m128i g_v128i[12];
__m512h g_v512h[12];
__m256h g_v256h[12];
__m128h g_v128h[12];
__mmask8 g_m8[12];
__mmask16 g_m16[12];
__mmask32 g_m32[12];
__mmask64 g_m64[12];
static const float  fbuf[64];
static const double dbuf[32];
static const int    ibuf[64];

/* generated: funcs=400 seed=1 */

__m256d torture_0(__m256d p0, __m512d p1) {
    __mmask32 m32_1 = (__mmask32)(g_m32[7] ^ 121);
    __mmask32 m32_2 = _kxor_mask32(m32_1, m32_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[3] ^ 25);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_mask_blend_epi32(m16_3, v512i_4, v512i_4);
    __m128i v128i_6 = g_v128i[0];
    __m128i v128i_7 = _mm_add_epi64(v128i_6, v128i_6);
    __m256i v256i_8 = g_v256i[3];
    __m256i v256i_9 = _mm256_add_epi8(v256i_8, v256i_8);
    __m256i v256i_10 = _mm256_shuffle_epi8(v256i_8, v256i_8);
    __m512 v512f_11 = g_v512f[0];
    __m512 v512f_12 = _mm512_fmadd_ps(v512f_11, v512f_11, v512f_11);
    __m128d v128d_13 = g_v128d[11];
    __m128d v128d_14 = _mm_unpackhi_pd(v128d_13, v128d_13);
    __m512i v512i_15 = _mm512_rol_epi32(v512i_4, 5);
    __mmask64 m64_16 = (__mmask64)(g_m64[7] ^ 241);
    __mmask64 m64_17 = _kor_mask64(m64_16, m64_16);
    __m512 v512f_18 = _mm512_scalef_ps(v512f_11, v512f_12);
    __m256d v256d_19 = _mm256_mul_pd(p0, p0);
    __m256i v256i_20 = _mm256_mullo_epi32(v256i_8, v256i_9);
    __m512 v512f_21 = _mm512_rcp14_ps(v512f_18);
    __m512i v512i_22 = _mm512_add_epi8(v512i_4, v512i_15);
    __mmask16 m16_23 = _mm512_movepi32_mask(v512i_15);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_23) & 7); i_24++) {
    __m512i v512i_25 = _mm512_ternarylogic_epi32(v512i_5, v512i_15, v512i_15, 0x96);
    __m512d v512d_26 = _mm512_insertf64x2(p1, v128d_14, 3);
    __m512d v512d_27 = _mm512_insertf64x2(p1, v128d_14, 3);
    __m256d v256d_28 = _mm256_max_pd(v256d_19, v256d_19);
    __m256h v256h_29 = _mm512_cvtxps_ph(v512f_12);
    }
    __m512 v512f_30 = _mm512_getexp_ps(v512f_18);
    __m512d v512d_31 = _mm512_unpackhi_pd(p1, p1);
    unsigned u32_32 = 21;
    __mmask16 m16_33 = _cvtu32_mask16(u32_32);
    __m512d v512d_34 = _mm512_shuffle_f64x2(v512d_31, v512d_31, 0xEE);
    __m128i v128i_35 = _mm_abs_epi32(v128i_6);
    __m128i v128i_36 = _mm_xor_si128(v128i_6, v128i_7);
    __mmask16 m16_37 = _mm512_cmp_epu32_mask(v512i_22, v512i_5, 4);
    __m256 v256f_38 = g_v256f[0];
    __m256 v256f_39 = _mm256_add_ps(v256f_38, v256f_38);
    __m512i v512i_40 = _mm512_popcnt_epi64(v512i_5);
    for (int i_41 = 0; i_41 < (int)(((unsigned long long)m16_33) & 7); i_41++) {
    __m128i v128i_42 = _mm_mullo_epi32(v128i_35, v128i_6);
    __m128 v128f_43 = g_v128f[11];
    __m128 v128f_44 = _mm_unpacklo_ps(v128f_43, v128f_43);
    __m512 v512f_45 = _mm512_shuffle_f32x4(v512f_30, v512f_12, 0x4E);
    __m128i v128i_46 = _mm_add_epi32(v128i_6, v128i_36);
    }
    g_m32[10] = m32_2;
    g_m64[0] = m64_16;
    return v256d_19;
}

__m512 torture_1(__m256 p0, __m256d p1, __m512d p2, __m512h p3) {
    __m128 v128f_1 = _mm_loadu_ps(fbuf);
    __m128 v128f_2 = _mm_mul_ps(v128f_1, v128f_1);
    __m512 v512f_3 = _mm512_set1_ps(1.0f);
    __m256d v256d_4 = _mm256_unpackhi_pd(p1, p1);
    __m512h v512h_5 = _mm512_fmsub_ph(p3, p3, p3);
    __m128d v128d_6 = _mm_set1_pd(2.0);
    __m128i v128i_7 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_8 = _mm_slli_epi32(v128i_7, 3);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_sllv_epi32(v256i_9, v256i_9);
    __mmask16 m16_11 = (__mmask16)(g_m16[4] ^ 28);
    __m512 v512f_12 = _mm512_mask_add_ps(v512f_3, m16_11, v512f_3, v512f_3);
    __m512i v512i_13 = _mm512_loadu_si512((const void*)ibuf);
    __m512 v512f_14 = _mm512_cvtepi32_ps(v512i_13);
    __m512 v512f_15 = _mm512_max_ps(v512f_3, v512f_3);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_11) & 7); i_16++) {
    __m128i v128i_17 = _mm_sub_epi64(v128i_8, v128i_7);
    __m512i v512i_18 = _mm512_cvtepi8_epi32(v128i_17);
    __m512 v512f_19 = _mm512_shuffle_f32x4(v512f_15, v512f_12, 0x4E);
    }
    __m512h v512h_20 = _mm512_fmul_pch(p3, v512h_5);
    __m512i v512i_21 = _mm512_cvtph_epi16(v512h_20);
    __m512i v512i_22 = _mm512_mask_add_epi32(v512i_21, m16_11, v512i_13, v512i_21);
    __m512i v512i_23 = _mm512_and_si512(v512i_13, v512i_13);
    __m256i v256i_24 = _mm256_slli_epi32(v256i_9, 3);
    __m512d v512d_25 = _mm512_max_pd(p2, p2);
    __mmask16 m16_26 = _knot_mask16(m16_11);
    __m128d v128d_27 = _mm_fmadd_pd(v128d_6, v128d_6, v128d_6);
    __m512 v512f_28 = _mm512_rsqrt14_ps(v512f_3);
    __m512 v512f_29 = _mm512_insertf32x8(v512f_12, p0, 1);
    __m256 v256f_30 = _mm256_add_ps(p0, p0);
    __m256 v256f_31 = _mm256_fnmadd_ps(v256f_30, p0, p0);
    for (int i_32 = 0; i_32 < (int)(((unsigned long long)m16_26) & 7); i_32++) {
    __m512i v512i_33 = _mm512_add_epi16(v512i_23, v512i_22);
    __m512i v512i_34 = _mm512_ternarylogic_epi32(v512i_23, v512i_13, v512i_22, 0x96);
    __m512d v512d_35 = _mm512_cvtps_pd(v256f_30);
    __m256i v256i_36 = _mm256_sub_epi8(v256i_9, v256i_9);
    __m256 v256f_37 = _mm256_fnmadd_ps(v256f_30, v256f_31, p0);
    }
    g_v256i[11] = v256i_24;
    g_v128f[2] = v128f_1;
    g_v128i[5] = v128i_7;
    g_m16[5] = m16_26;
    return v512f_3;
}

__m512h torture_2(__m512h p0, __m128h p1, __m128d p2) {
    __m512d v512d_1 = g_v512d[1];
    __m256d v256d_2 = _mm512_extractf64x4_pd(v512d_1, 1);
    __m256i v256i_3 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_4 = _mm256_abs_epi32(v256i_3);
    __m512d v512d_5 = _mm512_min_pd(v512d_1, v512d_1);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_and_si512(v512i_6, v512i_6);
    __m512d v512d_8 = _mm512_max_pd(v512d_1, v512d_1);
    __m256i v256i_9 = _mm256_add_epi8(v256i_4, v256i_4);
    __m512 v512f_10 = g_v512f[1];
    __m512 v512f_11 = _mm512_roundscale_ps(v512f_10, 0x10);
    __m128i v128i_12 = g_v128i[1];
    __m128i v128i_13 = _mm_mullo_epi32(v128i_12, v128i_12);
    __m256i v256i_14 = _mm256_and_si256(v256i_3, v256i_9);
    __m256h v256h_15 = g_v256h[0];
    __m512 v512f_16 = _mm512_cvtxph_ps(v256h_15);
    int s32_17 = 15;
    for (int i_18 = 0; i_18 < (int)((s32_17) & 7); i_18++) {
    __m256 v256f_19 = _mm256_loadu_ps(fbuf);
    __m256 v256f_20 = _mm256_max_ps(v256f_19, v256f_19);
    __m512d v512d_21 = _mm512_set1_pd(2.0);
    }
    __m128i v128i_22 = _mm_sub_epi8(v128i_12, v128i_12);
    __m512 v512f_23 = _mm512_set1_ps(1.0f);
    __mmask16 m16_24 = (__mmask16)(g_m16[1] ^ 112);
    __mmask16 m16_25 = _knot_mask16(m16_24);
    __m128 v128f_26 = g_v128f[8];
    __m128 v128f_27 = _mm_and_ps(v128f_26, v128f_26);
    __m512 v512f_28 = _mm512_getexp_ps(v512f_16);
    g_v512f[10] = v512f_16;
    g_v512d[0] = v512d_5;
    g_v256i[1] = v256i_9;
    g_v128d[3] = p2;
    g_v256h[3] = v256h_15;
    g_v128h[5] = p1;
    return p0;
}

__mmask32 torture_3(__m256h p0, __m512h p1) {
    __m128 v128f_1 = g_v128f[6];
    __m128 v128f_2 = _mm_add_ps(v128f_1, v128f_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_and_ps(v512f_3, v512f_3);
    __mmask8 m8_5 = (__mmask8)(g_m8[9] ^ 229);
    __m512d v512d_6 = g_v512d[3];
    __m512d v512d_7 = _mm512_mask_blend_pd(m8_5, v512d_6, v512d_6);
    __m512i v512i_8 = g_v512i[9];
    __m512i v512i_9 = _mm512_add_epi8(v512i_8, v512i_8);
    __m256d v256d_10 = g_v256d[3];
    __m256d v256d_11 = _mm256_max_pd(v256d_10, v256d_10);
    __mmask16 m16_12 = (__mmask16)(g_m16[4] ^ 142);
    __m512 v512f_13 = _mm512_mask3_fmadd_ps(v512f_3, v512f_4, v512f_3, m16_12);
    __m512d v512d_14 = _mm512_min_pd(v512d_6, v512d_6);
    __m512h v512h_15 = _mm512_add_ph(p1, p1);
    __mmask16 m16_16 = _kshiftri_mask16(m16_12, 1);
    __m128 v128f_17 = _mm_set1_ps(1.0f);
    __m128i v128i_18 = g_v128i[2];
    __m128i v128i_19 = _mm_xor_si128(v128i_18, v128i_18);
    __m128i v128i_20 = _mm_set1_epi32(0x55);
    unsigned u32_21 = 42;
    __mmask16 m16_22 = _cvtu32_mask16(u32_21);
    __m512d v512d_23 = _mm512_max_pd(v512d_14, v512d_14);
    __m256 v256f_24 = _mm256_loadu_ps(fbuf);
    __m256 v256f_25 = _mm256_sub_ps(v256f_24, v256f_24);
    __m512i v512i_26 = _mm512_sllv_epi32(v512i_9, v512i_9);
    __m512i v512i_27 = _mm512_sub_epi32(v512i_26, v512i_26);
    __m512i v512i_28 = _mm512_cvtps_epi32(v512f_4);
    if (((unsigned long long)m16_12) & 1ULL) {
    __mmask16 m16_29 = _cvtu32_mask16(u32_21);
    __m256i v256i_30 = _mm512_cvtepi64_epi32(v512i_28);
    }
    __m256 v256f_31 = _mm256_mul_ps(v256f_25, v256f_25);
    __m512i v512i_32 = _mm512_conflict_epi32(v512i_9);
    __m512 v512f_33 = _mm512_unpacklo_ps(v512f_13, v512f_13);
    __m256d v256d_34 = _mm256_fmadd_pd(v256d_11, v256d_10, v256d_11);
    __m512 v512f_35 = _mm512_mask3_fmadd_ps(v512f_33, v512f_13, v512f_33, m16_22);
    __m128i v128i_36 = _mm_sub_epi16(v128i_18, v128i_19);
    __m128 v128f_37 = _mm_add_ps(v128f_1, v128f_2);
    for (int i_38 = 0; i_38 < (int)(((unsigned long long)m16_22) & 7); i_38++) {
    __m512d v512d_39 = _mm512_sub_pd(v512d_14, v512d_7);
    __m512 v512f_40 = _mm512_insertf32x8(v512f_4, v256f_24, 1);
    }
    g_v512d[6] = v512d_14;
    g_v128i[7] = v128i_19;
    g_v256h[9] = p0;
    g_m16[1] = m16_12;
    __mmask32 m32_41 = (__mmask32)(g_m32[8] ^ 166);
    return m32_41;
}

__m128h torture_4(__m128i p0, __m256 p1, __m256d p2) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_2 = _mm512_cmp_pd_mask(v512d_1, v512d_1, _CMP_GE_OQ);
    __m512i v512i_3 = _mm512_set1_epi32(0x55);
    __mmask16 m16_4 = _mm512_cmp_epi32_mask(v512i_3, v512i_3, 2);
    __m512i v512i_5 = _mm512_mullo_epi32(v512i_3, v512i_3);
    __m512i v512i_6 = _mm512_abs_epi32(v512i_5);
    __m128i v128i_7 = _mm_add_epi32(p0, p0);
    __m128 v128f_8 = _mm_set1_ps(1.0f);
    __m128h v128h_9 = g_v128h[6];
    __m512d v512d_10 = _mm512_cvtph_pd(v128h_9);
    __m512 v512f_11 = g_v512f[6];
    __mmask16 m16_12 = _mm512_fpclass_ps_mask(v512f_11, 0x3);
    __m512i v512i_13 = _mm512_permutex2var_epi32(v512i_3, v512i_6, v512i_6);
    __m512 v512f_14 = _mm512_and_ps(v512f_11, v512f_11);
    __m128i v128i_15 = _mm512_cvtepi32_epi8(v512i_3);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_4) & 7); i_16++) {
    unsigned u32_17 = _cvtmask16_u32(m16_4);
    __m128i v128i_18 = _mm_sub_epi32(p0, v128i_7);
    __m512i v512i_19 = _mm512_mask_add_epi32(v512i_13, m16_12, v512i_5, v512i_6);
    }
    __m512i v512i_20 = _mm512_and_si512(v512i_13, v512i_5);
    __m512i v512i_21 = _mm512_sub_epi16(v512i_13, v512i_20);
    __m512i v512i_22 = _mm512_rolv_epi32(v512i_13, v512i_3);
    __m512h v512h_23 = g_v512h[4];
    __m512h v512h_24 = _mm512_getexp_ph(v512h_23);
    __m256i v256i_25 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_26 = _mm256_sub_epi64(v256i_25, v256i_25);
    __m256 v256f_27 = _mm256_max_ps(p1, p1);
    __m512 v512f_28 = _mm512_fmsub_ps(v512f_11, v512f_14, v512f_11);
    __m256 v256f_29 = _mm256_sub_ps(v256f_27, p1);
    __m512i v512i_30 = _mm512_popcnt_epi64(v512i_5);
    __m256i v256i_31 = _mm256_add_epi16(v256i_26, v256i_26);
    __m512d v512d_32 = _mm512_cvtph_pd(v128h_9);
    g_v256d[1] = p2;
    g_v256i[11] = v256i_25;
    g_v512h[5] = v512h_23;
    g_m16[7] = m16_12;
    return v128h_9;
}

__m128h torture_5(__m128i p0, __m128 p1, __m256i p2, __m128h p3) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_dpwssd_epi32(v512i_1, v512i_1, v512i_1);
    __m256 v256f_3 = g_v256f[4];
    __m256 v256f_4 = _mm256_sqrt_ps(v256f_3);
    __m512h v512h_5 = g_v512h[9];
    __m512h v512h_6 = _mm512_add_ph(v512h_5, v512h_5);
    __m128i v128i_7 = _mm_and_si128(p0, p0);
    __mmask16 m16_8 = _mm512_cmp_epi32_mask(v512i_1, v512i_2, 2);
    __m512d v512d_9 = g_v512d[3];
    __m512i v512i_10 = _mm512_cvtpd_epi64(v512d_9);
    __m128 v128f_11 = _mm_fmadd_ps(p1, p1, p1);
    __m128d v128d_12 = g_v128d[5];
    __m128d v128d_13 = _mm_div_pd(v128d_12, v128d_12);
    __m128 v128f_14 = _mm_set1_ps(1.0f);
    __mmask16 m16_15 = _knot_mask16(m16_8);
    __m512h v512h_16 = _mm512_mul_ph(v512h_6, v512h_5);
    __m128d v128d_17 = _mm_min_pd(v128d_12, v128d_13);
    __m512 v512f_18 = g_v512f[1];
    __m512 v512f_19 = _mm512_mask_blend_ps(m16_15, v512f_18, v512f_18);
    __m128 v128f_20 = _mm_div_ps(v128f_11, v128f_14);
    __m128d v128d_21 = _mm_max_pd(v128d_17, v128d_17);
    __m256i v256i_22 = _mm256_xor_si256(p2, p2);
    __m256i v256i_23 = _mm256_add_epi32(v256i_22, p2);
    __m128i v128i_24 = _mm_or_si128(p0, v128i_7);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512h v512h_25 = _mm512_rcp_ph(v512h_6);
    __m512 v512f_26 = _mm512_max_ps(v512f_18, v512f_18);
    }
    __m128d v128d_27 = _mm_mul_pd(v128d_17, v128d_17);
    __m256 v256f_28 = _mm256_and_ps(v256f_3, v256f_3);
    __m256i v256i_29 = _mm256_set1_epi32(0x55);
    __m256i v256i_30 = _mm256_add_epi8(p2, p2);
    __mmask64 m64_31 = _mm512_cmp_epi8_mask(v512i_10, v512i_10, 1);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m256i v256i_32 = _mm256_add_epi16(v256i_23, v256i_30);
    __mmask16 m16_33 = _kxor_mask16(m16_8, m16_15);
    __m512 v512f_34 = _mm512_rsqrt14_ps(v512f_19);
    __m256 v256f_35 = _mm256_sub_ps(v256f_4, v256f_4);
    __m512d v512d_36 = _mm512_cvtps_pd(v256f_28);
    __mmask16 m16_37 = _mm512_fpclass_ps_mask(v512f_18, 0x3);
    }
    g_v128d[11] = v128d_13;
    g_m64[2] = m64_31;
    return p3;
}

__m128h torture_6(__m128 p0, __m256 p1, __m512d p2, __m128h p3) {
    __m512 v512f_1 = g_v512f[1];
    __mmask16 m16_2 = _mm512_fpclass_ps_mask(v512f_1, 0x3);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_sub_epi32(v512i_3, v512i_3);
    __m128 v128f_5 = _mm_set1_ps(1.0f);
    __m512i v512i_6 = _mm512_add_epi8(v512i_3, v512i_3);
    __m512d v512d_7 = _mm512_add_pd(p2, p2);
    __m512d v512d_8 = _mm512_cvtph_pd(p3);
    __m128 v128f_9 = _mm_set1_ps(1.0f);
    __mmask16 m16_10 = _mm512_cmp_epu32_mask(v512i_6, v512i_6, 4);
    __m512d v512d_11 = _mm512_cvtps_pd(p1);
    __m128 v128f_12 = _mm_sub_ps(v128f_9, v128f_5);
    __m512i v512i_13 = _mm512_sub_epi16(v512i_6, v512i_6);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m128i v128i_14 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_15 = _mm_sub_epi8(v128i_14, v128i_14);
    __m256d v256d_16 = g_v256d[3];
    __m256d v256d_17 = _mm256_mul_pd(v256d_16, v256d_16);
    __m128i v128i_18 = _mm_and_si128(v128i_15, v128i_14);
    __m128i v128i_19 = _mm_sub_epi8(v128i_18, v128i_15);
    __m256d v256d_20 = _mm256_set1_pd(2.0);
    }
    __m512 v512f_21 = _mm512_insertf32x8(v512f_1, p1, 1);
    __m512d v512d_22 = _mm512_fmadd_pd(p2, p2, p2);
    __m128i v128i_23 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_24 = _mm_sllv_epi32(v128i_23, v128i_23);
    __m256d v256d_25 = _mm512_extractf64x4_pd(v512d_8, 1);
    __m256 v256f_26 = _mm256_fmsub_ps(p1, p1, p1);
    __m512 v512f_27 = _mm512_mask_add_ps(v512f_1, m16_2, v512f_21, v512f_1);
    __m256h v256h_28 = _mm512_cvtxps_ph(v512f_27);
    __m512d v512d_29 = _mm512_add_pd(v512d_22, v512d_11);
    __m256d v256d_30 = _mm256_unpackhi_pd(v256d_25, v256d_25);
    __m128i v128i_31 = _mm_and_si128(v128i_24, v128i_23);
    __m512 v512f_32 = _mm512_sqrt_ps(v512f_27);
    if (((unsigned long long)m16_2) & 1ULL) {
    unsigned u32_33 = 16;
    __mmask16 m16_34 = _cvtu32_mask16(u32_33);
    __m128i v128i_35 = _mm_add_epi64(v128i_23, v128i_23);
    __m512 v512f_36 = _mm512_fmsub_ps(v512f_32, v512f_21, v512f_27);
    __m128h v128h_37 = _mm512_cvtpd_ph(v512d_7);
    }
    g_v512f[10] = v512f_32;
    g_v512i[9] = v512i_4;
    g_v256f[9] = p1;
    g_m16[1] = m16_2;
    return p3;
}

__m512d torture_7(__m128 p0, __m512 p1, __m512 p2) {
    __m512i v512i_1 = g_v512i[8];
    __m512i v512i_2 = _mm512_add_epi32(v512i_1, v512i_1);
    __m128i v128i_3 = _mm_set1_epi32(0x55);
    __m128 v128f_4 = _mm_fnmadd_ps(p0, p0, p0);
    __m512i v512i_5 = _mm512_and_si512(v512i_1, v512i_2);
    __m256h v256h_6 = _mm512_cvtxps_ph(p1);
    __m128d v128d_7 = g_v128d[11];
    __m128d v128d_8 = _mm_sub_pd(v128d_7, v128d_7);
    __m512 v512f_9 = _mm512_fmadd_ps(p2, p1, p2);
    __mmask16 m16_10 = (__mmask16)(g_m16[5] ^ 22);
    __m512 v512f_11 = _mm512_maskz_mul_ps(m16_10, p2, p1);
    __m128 v128f_12 = _mm_unpacklo_ps(p0, v128f_4);
    __m256i v256i_13 = g_v256i[4];
    __m256i v256i_14 = _mm256_unpacklo_epi32(v256i_13, v256i_13);
    __m512 v512f_15 = _mm512_mask3_fmadd_ps(p1, v512f_9, p1, m16_10);
    __m256d v256d_16 = g_v256d[8];
    __m256d v256d_17 = _mm256_max_pd(v256d_16, v256d_16);
    __m256d v256d_18 = _mm256_set1_pd(2.0);
    __m128 v128f_19 = _mm_add_ps(v128f_12, v128f_4);
    __m128 v128f_20 = _mm512_extractf32x4_ps(v512f_11, 2);
    __m512i v512i_21 = _mm512_add_epi16(v512i_1, v512i_5);
    __m512i v512i_22 = _mm512_broadcastmw_epi32(m16_10);
    __m512i v512i_23 = _mm512_shuffle_i32x4(v512i_22, v512i_22, 0x1B);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_10) & 7); i_24++) {
    __mmask16 m16_25 = _knot_mask16(m16_10);
    __m256 v256f_26 = _mm256_loadu_ps(fbuf);
    __m256 v256f_27 = _mm256_fnmadd_ps(v256f_26, v256f_26, v256f_26);
    __m128d v128d_28 = _mm_min_pd(v128d_8, v128d_7);
    __m512i v512i_29 = _mm512_madd52lo_epu64(v512i_1, v512i_1, v512i_5);
    }
    __m256i v256i_30 = _mm256_unpacklo_epi32(v256i_14, v256i_14);
    __m256i v256i_31 = _mm256_shuffle_epi8(v256i_14, v256i_14);
    __m256 v256f_32 = _mm256_set1_ps(1.0f);
    __mmask16 m16_33 = _kshiftli_mask16(m16_10, 2);
    __m512i v512i_34 = _mm512_shuffle_i32x4(v512i_1, v512i_2, 0x1B);
    __m256i v256i_35 = _mm256_shuffle_epi8(v256i_30, v256i_30);
    __m512i v512i_36 = _mm512_dpwssd_epi32(v512i_1, v512i_21, v512i_5);
    __m128i v128i_37 = _mm_sllv_epi32(v128i_3, v128i_3);
    __mmask16 m16_38 = _kxor_mask16(m16_33, m16_10);
    __m512d v512d_39 = g_v512d[2];
    __m256d v256d_40 = _mm512_extractf64x4_pd(v512d_39, 1);
    __m512 v512f_41 = _mm512_unpacklo_ps(v512f_15, v512f_11);
    __m512i v512i_42 = _mm512_madd52lo_epu64(v512i_22, v512i_21, v512i_23);
    if (((unsigned long long)m16_38) & 1ULL) {
    __m128i v128i_43 = _mm_and_si128(v128i_37, v128i_3);
    __m512 v512f_44 = _mm512_fmsub_ps(p2, v512f_9, v512f_41);
    __m256 v256f_45 = _mm256_shuffle_ps(v256f_32, v256f_32, 0x1B);
    __m256 v256f_46 = _mm256_min_ps(v256f_45, v256f_32);
    }
    g_v512i[10] = v512i_21;
    g_v128f[5] = v128f_19;
    g_v128i[3] = v128i_37;
    return v512d_39;
}

__m128 torture_8(__m128h p0, __m512d p1, __m256d p2, __m128 p3, __m256 p4) {
    __mmask16 m16_1 = (__mmask16)(g_m16[8] ^ 198);
    __mmask16 m16_2 = _kshiftli_mask16(m16_1, 2);
    __m128 v128f_3 = _mm_xor_ps(p3, p3);
    __m128d v128d_4 = g_v128d[9];
    __m128d v128d_5 = _mm_div_pd(v128d_4, v128d_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_and_ps(v512f_6, v512f_6);
    __m512 v512f_8 = _mm512_sub_ps(v512f_6, v512f_7);
    __m512i v512i_9 = g_v512i[0];
    __mmask64 m64_10 = _mm512_cmp_epi8_mask(v512i_9, v512i_9, 1);
    __m256i v256i_11 = g_v256i[11];
    __m256i v256i_12 = _mm256_xor_si256(v256i_11, v256i_11);
    unsigned u32_13 = _cvtmask16_u32(m16_2);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_2) & 7); i_14++) {
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_16 = _mm_sub_epi8(v128i_15, v128i_15);
    __m512d v512d_17 = _mm512_insertf64x2(p1, v128d_4, 3);
    __m256d v256d_18 = _mm256_sqrt_pd(p2);
    __m128d v128d_19 = _mm_unpackhi_pd(v128d_4, v128d_5);
    }
    __m128d v128d_20 = _mm_mul_pd(v128d_5, v128d_4);
    __m512i v512i_21 = _mm512_add_epi32(v512i_9, v512i_9);
    __m128 v128f_22 = _mm_unpacklo_ps(v128f_3, v128f_3);
    __m256i v256i_23 = _mm256_add_epi16(v256i_12, v256i_12);
    __mmask32 m32_24 = _mm512_kunpackw(m16_2, m16_2);
    __m128i v128i_25 = g_v128i[7];
    __m128i v128i_26 = _mm_sub_epi16(v128i_25, v128i_25);
    __m512i v512i_27 = _mm512_unpacklo_epi32(v512i_21, v512i_9);
    __m512 v512f_28 = _mm512_min_ps(v512f_6, v512f_7);
    __m128 v128f_29 = _mm_fnmadd_ps(p3, v128f_22, v128f_3);
    __m128d v128d_30 = _mm_max_pd(v128d_5, v128d_20);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_2) & 7); i_31++) {
    __m256i v256i_32 = _mm256_set1_epi32(0x55);
    unsigned u32_33 = _cvtmask16_u32(m16_2);
    __m256 v256f_34 = _mm256_unpacklo_ps(p4, p4);
    __mmask16 m16_35 = _mm512_fpclass_ps_mask(v512f_28, 0x3);
    __m512i v512i_36 = _mm512_add_epi32(v512i_9, v512i_9);
    }
    g_v512i[7] = v512i_9;
    g_v256f[11] = p4;
    g_v128d[10] = v128d_30;
    g_m32[11] = m32_24;
    return p3;
}

__mmask8 torture_9(__m256h p0, __m128d p1) {
    __m128h v128h_1 = g_v128h[8];
    __m512d v512d_2 = _mm512_cvtph_pd(v128h_1);
    __m128d v128d_3 = _mm_set1_pd(2.0);
    __m128d v128d_4 = _mm_add_pd(p1, v128d_3);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_6 = _mm512_sub_epi32(v512i_5, v512i_5);
    __m512i v512i_7 = _mm512_set1_epi32(0x55);
    __m512d v512d_8 = _mm512_shuffle_f64x2(v512d_2, v512d_2, 0xEE);
    __mmask16 m16_9 = (__mmask16)(g_m16[8] ^ 74);
    __mmask16 m16_10 = _knot_mask16(m16_9);
    __m128 v128f_11 = _mm_set1_ps(1.0f);
    __m512h v512h_12 = g_v512h[8];
    __m512h v512h_13 = _mm512_add_ph(v512h_12, v512h_12);
    __m512i v512i_14 = _mm512_mask_blend_epi32(m16_10, v512i_6, v512i_5);
    __m512 v512f_15 = g_v512f[11];
    __m512 v512f_16 = _mm512_mul_ps(v512f_15, v512f_15);
    __mmask16 m16_17 = _mm512_test_epi32_mask(v512i_7, v512i_5);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m256d v256d_18 = g_v256d[8];
    __m256d v256d_19 = _mm256_sub_pd(v256d_18, v256d_18);
    __m256i v256i_20 = g_v256i[6];
    __m256i v256i_21 = _mm256_add_epi16(v256i_20, v256i_20);
    }
    __m256d v256d_22 = g_v256d[3];
    __m256d v256d_23 = _mm256_unpackhi_pd(v256d_22, v256d_22);
    __m512 v512f_24 = _mm512_rsqrt14_ps(v512f_15);
    __m512i v512i_25 = _mm512_rolv_epi32(v512i_6, v512i_14);
    __m256i v256i_26 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_27 = _mm256_add_epi16(v256i_26, v256i_26);
    __m256i v256i_28 = _mm256_add_epi8(v256i_27, v256i_26);
    __m128d v128d_29 = _mm_set1_pd(2.0);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m512 v512f_30 = _mm512_cvtepi32_ps(v512i_6);
    __mmask16 m16_31 = _kxor_mask16(m16_17, m16_17);
    __m128i v128i_32 = g_v128i[0];
    __m128i v128i_33 = _mm_sub_epi8(v128i_32, v128i_32);
    }
    g_v512d[7] = v512d_2;
    g_v512i[2] = v512i_14;
    g_v256i[8] = v256i_26;
    g_v128d[3] = p1;
    g_m16[3] = m16_9;
    __mmask8 m8_34 = (__mmask8)(g_m8[4] ^ 175);
    return m8_34;
}

__m256h torture_10(__m512h p0, __m128h p1, __m128i p2, __m128 p3) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_mullo_epi32(v512i_1, v512i_1);
    __m512i v512i_3 = _mm512_sub_epi8(v512i_2, v512i_1);
    __m512d v512d_4 = _mm512_cvtph_pd(p1);
    __m128 v128f_5 = _mm_mul_ps(p3, p3);
    __mmask16 m16_6 = _mm512_movepi32_mask(v512i_1);
    __m256i v256i_7 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_8 = _mm256_or_si256(v256i_7, v256i_7);
    __m512i v512i_9 = _mm512_rol_epi32(v512i_1, 5);
    __m128 v128f_10 = _mm_fmadd_ps(p3, p3, p3);
    __mmask64 m64_11 = _mm512_movepi8_mask(v512i_2);
    __m512i v512i_12 = _mm512_add_epi64(v512i_3, v512i_3);
    __m512 v512f_13 = g_v512f[5];
    __m512 v512f_14 = _mm512_mul_ps(v512f_13, v512f_13);
    __m128 v128f_15 = _mm_min_ps(v128f_10, v128f_10);
    __m512i v512i_16 = _mm512_and_si512(v512i_12, v512i_3);
    __m128d v128d_17 = g_v128d[1];
    __m128d v128d_18 = _mm_div_pd(v128d_17, v128d_17);
    __m512 v512f_19 = _mm512_roundscale_ps(v512f_14, 0x10);
    __m512i v512i_20 = _mm512_conflict_epi32(v512i_9);
    __m256i v256i_21 = _mm256_xor_si256(v256i_7, v256i_8);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_6) & 7); i_22++) {
    __m256 v256f_23 = _mm256_loadu_ps(fbuf);
    __m256 v256f_24 = _mm256_sub_ps(v256f_23, v256f_23);
    __m256i v256i_25 = _mm256_sub_epi32(v256i_21, v256i_7);
    }
    g_v256i[10] = v256i_21;
    g_v512h[9] = p0;
    g_m64[4] = m64_11;
    __m256h v256h_26 = g_v256h[2];
    return v256h_26;
}

__m256d torture_11(__m128d p0, __m128 p1, __m512d p2, __m512h p3, __m512i p4) {
    __m256i v256i_1 = g_v256i[10];
    __m256i v256i_2 = _mm256_and_si256(v256i_1, v256i_1);
    __m512i v512i_3 = _mm512_conflict_epi32(p4);
    __mmask16 m16_4 = (__mmask16)(g_m16[10] ^ 34);
    __m512i v512i_5 = _mm512_broadcastmw_epi32(m16_4);
    __m128 v128f_6 = _mm_xor_ps(p1, p1);
    __m512 v512f_7 = g_v512f[9];
    __m512 v512f_8 = _mm512_fmsub_ps(v512f_7, v512f_7, v512f_7);
    __m128 v128f_9 = _mm_fnmadd_ps(v128f_6, v128f_6, v128f_6);
    __m512 v512f_10 = _mm512_shuffle_ps(v512f_7, v512f_8, 0x1B);
    unsigned u32_11 = 5;
    __mmask16 m16_12 = _cvtu32_mask16(u32_11);
    __m512h v512h_13 = _mm512_fmsub_ph(p3, p3, p3);
    __m512d v512d_14 = _mm512_insertf64x2(p2, p0, 3);
    __m512 v512f_15 = _mm512_scalef_ps(v512f_8, v512f_10);
    __m512h v512h_16 = _mm512_sqrt_ph(v512h_13);
    __m512 v512f_17 = _mm512_mask3_fmadd_ps(v512f_8, v512f_7, v512f_10, m16_4);
    __m512i v512i_18 = _mm512_sub_epi8(p4, v512i_5);
    __m256i v256i_19 = _mm256_unpacklo_epi32(v256i_2, v256i_2);
    __m256d v256d_20 = g_v256d[10];
    __m256d v256d_21 = _mm256_sqrt_pd(v256d_20);
    __m128i v128i_22 = _mm512_cvtepi32_epi8(p4);
    __m256 v256f_23 = _mm512_cvtpd_ps(v512d_14);
    __m128d v128d_24 = _mm_fmadd_pd(p0, p0, p0);
    g_v512i[10] = v512i_3;
    g_v512h[2] = p3;
    return v256d_20;
}

__m128d torture_12(__m256i p0, __m256h p1, __m256 p2, __m256h p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[7] ^ 95);
    __mmask16 m16_2 = _knot_mask16(m16_1);
    __m128 v128f_3 = _mm_set1_ps(1.0f);
    __m512h v512h_4 = g_v512h[10];
    __m512h v512h_5 = _mm512_fmul_pch(v512h_4, v512h_4);
    __m128i v128i_6 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_7 = _mm_add_epi64(v128i_6, v128i_6);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_9 = _mm512_dpwssd_epi32(v512i_8, v512i_8, v512i_8);
    __m512i v512i_10 = _mm512_shuffle_epi8(v512i_8, v512i_9);
    __m128d v128d_11 = g_v128d[1];
    __m128d v128d_12 = _mm_fmadd_pd(v128d_11, v128d_11, v128d_11);
    __m256 v256f_13 = _mm256_div_ps(p2, p2);
    __m512 v512f_14 = g_v512f[4];
    __m256h v256h_15 = _mm512_cvtxps_ph(v512f_14);
    __m512i v512i_16 = _mm512_abs_epi32(v512i_8);
    __m512i v512i_17 = _mm512_shuffle_i32x4(v512i_8, v512i_10, 0x1B);
    __m512 v512f_18 = _mm512_min_ps(v512f_14, v512f_14);
    __mmask8 m8_19 = (__mmask8)(g_m8[10] ^ 181);
    __m512i v512i_20 = _mm512_broadcastmb_epi64(m8_19);
    __mmask16 m16_21 = _kor_mask16(m16_1, m16_2);
    __m512 v512f_22 = _mm512_sub_ps(v512f_14, v512f_18);
    __m128 v128f_23 = _mm_sub_ps(v128f_3, v128f_3);
    unsigned u32_24 = _cvtmask16_u32(m16_21);
    __m512 v512f_25 = _mm512_rcp14_ps(v512f_14);
    __m256 v256f_26 = _mm256_sub_ps(v256f_13, p2);
    __m512i v512i_27 = _mm512_permutex2var_epi32(v512i_9, v512i_16, v512i_9);
    __m512 v512f_28 = _mm512_mask_add_ps(v512f_14, m16_21, v512f_18, v512f_14);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m128 v128f_29 = _mm_shuffle_ps(v128f_3, v128f_3, 0x1B);
    __m512h v512h_30 = _mm512_fmadd_ph(v512h_5, v512h_4, v512h_4);
    }
    g_v512f[4] = v512f_14;
    g_v512i[6] = v512i_16;
    g_v256h[2] = p3;
    return v128d_11;
}

__m128h torture_13(__m512i p0, __m256d p1, __m256d p2, __m256d p3) {
    __m128 v128f_1 = _mm_loadu_ps(fbuf);
    __m128 v128f_2 = _mm_unpacklo_ps(v128f_1, v128f_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_div_ps(v512f_3, v512f_3);
    __m256d v256d_5 = _mm256_sqrt_pd(p1);
    __m256 v256f_6 = _mm256_loadu_ps(fbuf);
    __m256 v256f_7 = _mm256_fmsub_ps(v256f_6, v256f_6, v256f_6);
    __mmask16 m16_8 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    __m512i v512i_9 = _mm512_madd52lo_epu64(p0, p0, p0);
    __mmask16 m16_10 = _mm512_testn_epi32_mask(p0, p0);
    __m128i v128i_11 = _mm512_cvtepi32_epi8(p0);
    __m512i v512i_12 = _mm512_abs_epi32(p0);
    __m512 v512f_13 = _mm512_fmsub_ps(v512f_4, v512f_4, v512f_4);
    __m128h v128h_14 = g_v128h[2];
    __m512d v512d_15 = _mm512_cvtph_pd(v128h_14);
    __m512 v512f_16 = _mm512_insertf32x8(v512f_13, v256f_6, 1);
    __m512h v512h_17 = g_v512h[8];
    __m512i v512i_18 = _mm512_cvtph_epi16(v512h_17);
    for (int i_19 = 0; i_19 < (int)(((unsigned long long)m16_8) & 7); i_19++) {
    __m256i v256i_20 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_21 = _mm256_sub_epi16(v256i_20, v256i_20);
    __m128 v128f_22 = _mm_max_ps(v128f_1, v128f_1);
    __m256i v256i_23 = _mm256_or_si256(v256i_20, v256i_21);
    }
    __m512i v512i_24 = _mm512_conflict_epi32(v512i_12);
    __m512d v512d_25 = _mm512_unpackhi_pd(v512d_15, v512d_15);
    __mmask16 m16_26 = _mm512_cmp_epu32_mask(v512i_18, p0, 4);
    __m256i v256i_27 = g_v256i[9];
    __m256i v256i_28 = _mm256_mullo_epi32(v256i_27, v256i_27);
    __m256 v256f_29 = _mm512_cvtpd_ps(v512d_25);
    __mmask8 m8_30 = (__mmask8)(g_m8[7] ^ 66);
    __mmask8 m8_31 = _kand_mask8(m8_30, m8_30);
    __m512 v512f_32 = _mm512_shuffle_ps(v512f_13, v512f_4, 0x1B);
    __m512i v512i_33 = _mm512_broadcastmb_epi64(m8_30);
    __m512i v512i_34 = _mm512_cvtepi8_epi32(v128i_11);
    __m512i v512i_35 = _mm512_sub_epi64(v512i_34, v512i_18);
    __m512h v512h_36 = _mm512_add_ph(v512h_17, v512h_17);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m128 v128f_37 = _mm_unpacklo_ps(v128f_1, v128f_2);
    __mmask16 m16_38 = _mm512_movepi32_mask(p0);
    __mmask16 m16_39 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    }
    g_v512i[10] = v512i_18;
    g_v128f[7] = v128f_2;
    g_v512h[0] = v512h_17;
    g_m8[11] = m8_31;
    return v128h_14;
}

__m128d torture_14(__m512d p0, __m256h p1) {
    __m128d v128d_1 = g_v128d[7];
    __m128d v128d_2 = _mm_unpackhi_pd(v128d_1, v128d_1);
    __m256 v256f_3 = g_v256f[3];
    __m256 v256f_4 = _mm256_sub_ps(v256f_3, v256f_3);
    __m512d v512d_5 = _mm512_shuffle_f64x2(p0, p0, 0xEE);
    __m256d v256d_6 = g_v256d[5];
    __m256d v256d_7 = _mm256_mul_pd(v256d_6, v256d_6);
    __m128 v128f_8 = g_v128f[7];
    __m128 v128f_9 = _mm_sub_ps(v128f_8, v128f_8);
    __m512d v512d_10 = _mm512_cvtps_pd(v256f_3);
    __m512i v512i_11 = g_v512i[4];
    __m512i v512i_12 = _mm512_unpacklo_epi32(v512i_11, v512i_11);
    __m512 v512f_13 = g_v512f[1];
    __m512 v512f_14 = _mm512_xor_ps(v512f_13, v512f_13);
    int s32_15 = 52;
    for (int i_16 = 0; i_16 < (int)((s32_15) & 7); i_16++) {
    __m128d v128d_17 = _mm_add_pd(v128d_2, v128d_2);
    __m128i v128i_18 = g_v128i[0];
    __m128i v128i_19 = _mm_xor_si128(v128i_18, v128i_18);
    __m512 v512f_20 = _mm512_min_ps(v512f_14, v512f_14);
    __m512d v512d_21 = _mm512_insertf64x2(v512d_10, v128d_2, 3);
    }
    __m512 v512f_22 = _mm512_broadcast_f32x4(v128f_9);
    __m512d v512d_23 = _mm512_mul_pd(v512d_10, v512d_5);
    __m128d v128d_24 = _mm_fmadd_pd(v128d_2, v128d_1, v128d_1);
    __m512 v512f_25 = _mm512_sqrt_ps(v512f_22);
    __m512i v512i_26 = _mm512_rol_epi32(v512i_11, 5);
    __m512i v512i_27 = _mm512_add_epi8(v512i_12, v512i_12);
    g_v256f[8] = v256f_4;
    g_v128f[2] = v128f_9;
    g_v128d[1] = v128d_24;
    g_v256h[8] = p1;
    return v128d_2;
}

__m128h torture_15(__m256d p0, __m512h p1, __m256i p2, __m512 p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[10] ^ 64);
    __mmask32 m32_2 = _mm512_kunpackw(m16_1, m16_1);
    __mmask8 m8_3 = (__mmask8)(g_m8[6] ^ 141);
    __mmask8 m8_4 = _kand_mask8(m8_3, m8_3);
    __m512i v512i_5 = g_v512i[1];
    __m512i v512i_6 = _mm512_dpwssd_epi32(v512i_5, v512i_5, v512i_5);
    __m256i v256i_7 = _mm256_add_epi8(p2, p2);
    __m512i v512i_8 = _mm512_and_si512(v512i_6, v512i_6);
    __m256d v256d_9 = _mm256_div_pd(p0, p0);
    __m256i v256i_10 = _mm256_sub_epi32(v256i_7, p2);
    __mmask16 m16_11 = _kor_mask16(m16_1, m16_1);
    __m512i v512i_12 = _mm512_lzcnt_epi32(v512i_5);
    for (int i_13 = 0; i_13 < (int)(((unsigned long long)m16_11) & 7); i_13++) {
    __m512 v512f_14 = _mm512_sqrt_ps(p3);
    __m512i v512i_15 = _mm512_broadcastmw_epi32(m16_11);
    __m512 v512f_16 = _mm512_maskz_mul_ps(m16_1, p3, v512f_14);
    }
    __mmask16 m16_17 = _kshiftli_mask16(m16_1, 2);
    __m512 v512f_18 = _mm512_rcp14_ps(p3);
    __mmask16 m16_19 = _mm512_cmp_epu32_mask(v512i_12, v512i_6, 4);
    __m256 v256f_20 = _mm256_loadu_ps(fbuf);
    __m256 v256f_21 = _mm256_fmsub_ps(v256f_20, v256f_20, v256f_20);
    __mmask64 m64_22 = _mm512_movepi8_mask(v512i_8);
    __m128i v128i_23 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_24 = _mm_mullo_epi32(v128i_23, v128i_23);
    __m128 v128f_25 = g_v128f[5];
    __m128 v128f_26 = _mm_fmadd_ps(v128f_25, v128f_25, v128f_25);
    __m512h v512h_27 = _mm512_add_ph(p1, p1);
    __m128i v128i_28 = _mm_abs_epi32(v128i_23);
    __m512i v512i_29 = _mm512_set1_epi32(0x55);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m512i v512i_30 = _mm512_shuffle_i32x4(v512i_6, v512i_5, 0x1B);
    __m512i v512i_31 = _mm512_rol_epi32(v512i_6, 5);
    }
    g_v256f[10] = v256f_21;
    g_v256d[2] = p0;
    g_v512h[7] = v512h_27;
    g_m32[10] = m32_2;
    g_m64[8] = m64_22;
    __m128h v128h_32 = g_v128h[7];
    return v128h_32;
}

__mmask64 torture_16(__m512h p0, __m128d p1, __m512i p2, __m256d p3, __m256h p4) {
    __m256i v256i_1 = g_v256i[6];
    __m256i v256i_2 = _mm256_xor_si256(v256i_1, v256i_1);
    __m256d v256d_3 = _mm256_unpackhi_pd(p3, p3);
    __m512 v512f_4 = g_v512f[7];
    __m512i v512i_5 = _mm512_cvtps_epi32(v512f_4);
    __m128i v128i_6 = g_v128i[3];
    __m128i v128i_7 = _mm_mullo_epi32(v128i_6, v128i_6);
    __m256 v256f_8 = g_v256f[5];
    __m256 v256f_9 = _mm256_and_ps(v256f_8, v256f_8);
    __m128 v128f_10 = g_v128f[0];
    __m128 v128f_11 = _mm_unpacklo_ps(v128f_10, v128f_10);
    __m128 v128f_12 = _mm_max_ps(v128f_10, v128f_11);
    __mmask16 m16_13 = (__mmask16)(g_m16[8] ^ 190);
    __mmask16 m16_14 = _kshiftri_mask16(m16_13, 1);
    __m512 v512f_15 = _mm512_shuffle_ps(v512f_4, v512f_4, 0x1B);
    __m512 v512f_16 = _mm512_div_ps(v512f_15, v512f_15);
    __m256i v256i_17 = _mm256_sllv_epi32(v256i_1, v256i_1);
    __mmask16 m16_18 = _mm512_fpclass_ps_mask(v512f_4, 0x3);
    __m256d v256d_19 = _mm256_set1_pd(2.0);
    __m512i v512i_20 = _mm512_sub_epi16(p2, p2);
    unsigned u32_21 = 89;
    __mmask16 m16_22 = _cvtu32_mask16(u32_21);
    __m256i v256i_23 = _mm256_and_si256(v256i_2, v256i_17);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_13) & 7); i_24++) {
    __mmask8 m8_25 = (__mmask8)(g_m8[8] ^ 192);
    __mmask8 m8_26 = _kand_mask8(m8_25, m8_25);
    __m256i v256i_27 = _mm256_xor_si256(v256i_17, v256i_23);
    __m128d v128d_28 = _mm_sqrt_pd(p1);
    }
    __m256 v256f_29 = _mm256_fmsub_ps(v256f_9, v256f_8, v256f_8);
    __m512d v512d_30 = _mm512_loadu_pd(dbuf);
    __m512i v512i_31 = _mm512_cvtpd_epi64(v512d_30);
    __m256d v256d_32 = _mm256_add_pd(p3, v256d_19);
    __m128d v128d_33 = _mm_sub_pd(p1, p1);
    __m256i v256i_34 = _mm512_cvtepi64_epi32(v512i_5);
    g_v512d[11] = v512d_30;
    g_m16[3] = m16_22;
    __mmask64 m64_35 = (__mmask64)(g_m64[9] ^ 192);
    return m64_35;
}

__m128i torture_17(__m256i p0, __m256 p1, __m512 p2, __m512d p3, __m512h p4) {
    __m512i v512i_1 = g_v512i[2];
    __m512i v512i_2 = _mm512_permutex2var_epi32(v512i_1, v512i_1, v512i_1);
    __m512d v512d_3 = _mm512_div_pd(p3, p3);
    __m512i v512i_4 = _mm512_cvtph_epi16(p4);
    __m128d v128d_5 = g_v128d[11];
    __m128d v128d_6 = _mm_unpackhi_pd(v128d_5, v128d_5);
    __m512d v512d_7 = _mm512_mul_pd(p3, p3);
    __m512 v512f_8 = _mm512_fnmadd_ps(p2, p2, p2);
    __m512d v512d_9 = _mm512_cvtps_pd(p1);
    __m512 v512f_10 = _mm512_rsqrt14_ps(p2);
    __m128i v128i_11 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_12 = _mm_xor_si128(v128i_11, v128i_11);
    __m256d v256d_13 = _mm512_extractf64x4_pd(v512d_7, 1);
    __m256d v256d_14 = _mm256_mul_pd(v256d_13, v256d_13);
    __m128d v128d_15 = _mm_add_pd(v128d_5, v128d_5);
    __m512 v512f_16 = _mm512_rcp14_ps(v512f_10);
    __m128i v128i_17 = _mm_sub_epi8(v128i_11, v128i_12);
    __m256 v256f_18 = _mm256_set1_ps(1.0f);
    __m128d v128d_19 = _mm_min_pd(v128d_5, v128d_5);
    int s32_20 = 65;
    for (int i_21 = 0; i_21 < (int)((s32_20) & 7); i_21++) {
    __m256 v256f_22 = _mm256_unpacklo_ps(p1, p1);
    __m256i v256i_23 = _mm256_xor_si256(p0, p0);
    __mmask16 m16_24 = (__mmask16)(g_m16[11] ^ 38);
    __m512 v512f_25 = _mm512_mask_add_ps(v512f_10, m16_24, p2, v512f_16);
    }
    __m256d v256d_26 = _mm256_mul_pd(v256d_13, v256d_14);
    __m128 v128f_27 = _mm_loadu_ps(fbuf);
    __m128 v128f_28 = _mm_mul_ps(v128f_27, v128f_27);
    __m128i v128i_29 = _mm_sub_epi32(v128i_11, v128i_11);
    __m256h v256h_30 = _mm512_cvtxps_ph(v512f_8);
    __m512i v512i_31 = _mm512_sub_epi16(v512i_1, v512i_1);
    __m512 v512f_32 = _mm512_shuffle_ps(p2, p2, 0x1B);
    g_v256d[7] = v256d_14;
    g_v128f[3] = v128f_28;
    g_v512h[6] = p4;
    return v128i_17;
}

__m128h torture_18(__m128d p0, __m256i p1, __m256i p2, __m256i p3, __m128 p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_permutexvar_epi32(v512i_1, v512i_1);
    __m256i v256i_3 = _mm256_xor_si256(p1, p2);
    __m512i v512i_4 = _mm512_dpbusd_epi32(v512i_1, v512i_1, v512i_1);
    __m128i v128i_5 = g_v128i[2];
    __m128i v128i_6 = _mm_mullo_epi32(v128i_5, v128i_5);
    __m512d v512d_7 = _mm512_set1_pd(2.0);
    __m512d v512d_8 = _mm512_fmadd_pd(v512d_7, v512d_7, v512d_7);
    __m512h v512h_9 = g_v512h[5];
    __m512h v512h_10 = _mm512_fmsub_ph(v512h_9, v512h_9, v512h_9);
    __m128i v128i_11 = _mm_slli_epi32(v128i_6, 3);
    __m512d v512d_12 = _mm512_max_pd(v512d_8, v512d_7);
    __m512 v512f_13 = _mm512_loadu_ps(fbuf);
    __m512 v512f_14 = _mm512_getexp_ps(v512f_13);
    __m128i v128i_15 = _mm_sub_epi8(v128i_5, v128i_5);
    __m128h v128h_16 = _mm512_cvtpd_ph(v512d_8);
    __m512d v512d_17 = _mm512_div_pd(v512d_12, v512d_7);
    __m128i v128i_18 = _mm_sllv_epi32(v128i_5, v128i_5);
    __mmask16 m16_19 = _mm512_movepi32_mask(v512i_1);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_19) & 7); i_20++) {
    __m512 v512f_21 = _mm512_mask_blend_ps(m16_19, v512f_14, v512f_13);
    __m256 v256f_22 = g_v256f[0];
    __m256 v256f_23 = _mm256_min_ps(v256f_22, v256f_22);
    __m256 v256f_24 = _mm512_cvtpd_ps(v512d_8);
    __m512d v512d_25 = _mm512_sub_pd(v512d_8, v512d_12);
    }
    __m512 v512f_26 = _mm512_roundscale_ps(v512f_14, 0x10);
    __m256d v256d_27 = g_v256d[5];
    __m256d v256d_28 = _mm256_max_pd(v256d_27, v256d_27);
    __m256d v256d_29 = _mm256_mul_pd(v256d_28, v256d_27);
    __m256 v256f_30 = _mm256_loadu_ps(fbuf);
    __m256 v256f_31 = _mm256_fmsub_ps(v256f_30, v256f_30, v256f_30);
    __m128 v128f_32 = _mm_fnmadd_ps(p4, p4, p4);
    __m256d v256d_33 = _mm256_sub_pd(v256d_27, v256d_28);
    __m512i v512i_34 = _mm512_dpwssd_epi32(v512i_4, v512i_4, v512i_4);
    __m512i v512i_35 = _mm512_broadcast_i32x4(v128i_18);
    __mmask8 m8_36 = (__mmask8)(g_m8[6] ^ 221);
    __m512d v512d_37 = _mm512_mask_blend_pd(m8_36, v512d_17, v512d_17);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m256i v256i_38 = _mm256_sub_epi32(p2, p2);
    __mmask64 m64_39 = (__mmask64)(g_m64[2] ^ 35);
    __mmask64 m64_40 = _kor_mask64(m64_39, m64_39);
    }
    g_v512f[5] = v512f_13;
    g_v512d[5] = v512d_8;
    g_v256f[10] = v256f_30;
    g_v128f[7] = p4;
    g_v512h[1] = v512h_10;
    return v128h_16;
}

__m512i torture_19(__m512d p0, __m256h p1, __m256h p2, __m256i p3) {
    __m256 v256f_1 = g_v256f[3];
    __m256 v256f_2 = _mm256_div_ps(v256f_1, v256f_1);
    __m256i v256i_3 = _mm256_add_epi64(p3, p3);
    __m128i v128i_4 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_5 = _mm_shuffle_epi8(v128i_4, v128i_4);
    __m256i v256i_6 = _mm256_and_si256(p3, v256i_3);
    __m512 v512f_7 = _mm512_loadu_ps(fbuf);
    __m512 v512f_8 = _mm512_unpacklo_ps(v512f_7, v512f_7);
    __m512d v512d_9 = _mm512_cvtps_pd(v256f_1);
    __m256i v256i_10 = _mm256_sub_epi64(v256i_3, v256i_6);
    __m512i v512i_11 = g_v512i[9];
    __mmask64 m64_12 = _mm512_movepi8_mask(v512i_11);
    for (int i_13 = 0; i_13 < (int)(((unsigned long long)m64_12) & 7); i_13++) {
    __m512i v512i_14 = _mm512_sllv_epi32(v512i_11, v512i_11);
    __m128d v128d_15 = g_v128d[5];
    __m128d v128d_16 = _mm_sub_pd(v128d_15, v128d_15);
    __m128d v128d_17 = _mm_sub_pd(v128d_16, v128d_15);
    __m128i v128i_18 = _mm_unpacklo_epi32(v128i_4, v128i_4);
    }
    __m512d v512d_19 = _mm512_fmadd_pd(p0, p0, v512d_9);
    __m512i v512i_20 = _mm512_dpbusd_epi32(v512i_11, v512i_11, v512i_11);
    __m512 v512f_21 = _mm512_roundscale_ps(v512f_8, 0x10);
    __m128d v128d_22 = g_v128d[0];
    __m128d v128d_23 = _mm_sub_pd(v128d_22, v128d_22);
    __m512 v512f_24 = _mm512_scalef_ps(v512f_21, v512f_21);
    __mmask16 m16_25 = (__mmask16)(g_m16[7] ^ 43);
    __mmask16 m16_26 = _kxor_mask16(m16_25, m16_25);
    __m512 v512f_27 = _mm512_shuffle_ps(v512f_21, v512f_21, 0x1B);
    for (int i_28 = 0; i_28 < (int)(((unsigned long long)m16_26) & 7); i_28++) {
    __mmask32 m32_29 = (__mmask32)(g_m32[5] ^ 50);
    __mmask32 m32_30 = _kxor_mask32(m32_29, m32_29);
    __m512i v512i_31 = _mm512_xor_si512(v512i_20, v512i_11);
    __m128 v128f_32 = g_v128f[2];
    __m128 v128f_33 = _mm_fmsub_ps(v128f_32, v128f_32, v128f_32);
    __m512 v512f_34 = _mm512_mul_ps(v512f_27, v512f_8);
    __m512d v512d_35 = _mm512_cvtps_pd(v256f_1);
    }
    g_v512d[6] = v512d_19;
    g_v512i[4] = v512i_20;
    g_v256i[11] = v256i_6;
    g_v128d[11] = v128d_23;
    g_v256h[9] = p2;
    g_m64[2] = m64_12;
    return v512i_11;
}

__m256h torture_20(__m128 p0, __m128 p1) {
    __m512i v512i_1 = g_v512i[9];
    __m512i v512i_2 = _mm512_permutex2var_epi32(v512i_1, v512i_1, v512i_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[8] ^ 114);
    __m512i v512i_4 = _mm512_broadcastmw_epi32(m16_3);
    __m128 v128f_5 = _mm_fmsub_ps(p0, p0, p0);
    __mmask64 m64_6 = _mm512_movepi8_mask(v512i_1);
    __m512i v512i_7 = _mm512_add_epi64(v512i_4, v512i_2);
    __m512i v512i_8 = _mm512_slli_epi32(v512i_7, 3);
    __m128d v128d_9 = g_v128d[2];
    __m128d v128d_10 = _mm_sub_pd(v128d_9, v128d_9);
    __m128i v128i_11 = g_v128i[9];
    __m128i v128i_12 = _mm_sub_epi64(v128i_11, v128i_11);
    __m512d v512d_13 = _mm512_loadu_pd(dbuf);
    __m512d v512d_14 = _mm512_insertf64x2(v512d_13, v128d_10, 3);
    __m128i v128i_15 = _mm_abs_epi32(v128i_12);
    __m256i v256i_16 = g_v256i[2];
    __m256i v256i_17 = _mm256_mullo_epi32(v256i_16, v256i_16);
    __m256 v256f_18 = g_v256f[0];
    __m256 v256f_19 = _mm256_div_ps(v256f_18, v256f_18);
    __m512 v512f_20 = g_v512f[8];
    __m512 v512f_21 = _mm512_maskz_mul_ps(m16_3, v512f_20, v512f_20);
    __m128i v128i_22 = _mm_sub_epi64(v128i_12, v128i_12);
    __m256i v256i_23 = _mm256_set1_epi32(0x55);
    __mmask16 m16_24 = _knot_mask16(m16_3);
    __m128 v128f_25 = _mm_shuffle_ps(p0, v128f_5, 0x1B);
    __m128i v128i_26 = _mm_or_si128(v128i_15, v128i_22);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m128i v128i_27 = _mm_add_epi32(v128i_26, v128i_22);
    __m512i v512i_28 = _mm512_ternarylogic_epi32(v512i_2, v512i_4, v512i_2, 0x96);
    __m128 v128f_29 = _mm_div_ps(p1, p1);
    __m256d v256d_30 = g_v256d[3];
    __m256d v256d_31 = _mm256_add_pd(v256d_30, v256d_30);
    __m512h v512h_32 = g_v512h[2];
    __m512h v512h_33 = _mm512_fmul_pch(v512h_32, v512h_32);
    __mmask64 m64_34 = _mm512_cmp_epi8_mask(v512i_1, v512i_7, 1);
    }
    __m128i v128i_35 = _mm_sub_epi16(v128i_12, v128i_15);
    __m512 v512f_36 = _mm512_rsqrt14_ps(v512f_21);
    __m256i v256i_37 = _mm256_sub_epi64(v256i_17, v256i_16);
    __m128 v128f_38 = _mm_shuffle_ps(v128f_25, v128f_25, 0x1B);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_24) & 7); i_39++) {
    __m256i v256i_40 = _mm256_unpacklo_epi32(v256i_23, v256i_17);
    __m512i v512i_41 = _mm512_add_epi8(v512i_7, v512i_2);
    __m128i v128i_42 = _mm_sub_epi64(v128i_12, v128i_22);
    __m512i v512i_43 = _mm512_sub_epi32(v512i_8, v512i_1);
    __m512i v512i_44 = _mm512_conflict_epi32(v512i_4);
    }
    g_v128f[11] = v128f_25;
    g_v128i[8] = v128i_12;
    g_m64[5] = m64_6;
    __m256h v256h_45 = g_v256h[9];
    return v256h_45;
}

__m512d torture_21(__m128 p0, __m512 p1, __m256i p2, __m256i p3) {
    __m128i v128i_1 = g_v128i[10];
    __m512i v512i_2 = _mm512_broadcast_i32x4(v128i_1);
    __m128d v128d_3 = g_v128d[3];
    __m128d v128d_4 = _mm_min_pd(v128d_3, v128d_3);
    __m256d v256d_5 = g_v256d[5];
    __m256d v256d_6 = _mm256_add_pd(v256d_5, v256d_5);
    __mmask64 m64_7 = _mm512_cmp_epi8_mask(v512i_2, v512i_2, 1);
    __m512 v512f_8 = _mm512_mul_ps(p1, p1);
    __m512i v512i_9 = _mm512_cvtps_epi32(p1);
    __m256d v256d_10 = _mm256_mul_pd(v256d_5, v256d_6);
    __m128 v128f_11 = _mm_unpacklo_ps(p0, p0);
    __m512i v512i_12 = _mm512_add_epi64(v512i_9, v512i_2);
    __mmask16 m16_13 = (__mmask16)(g_m16[2] ^ 29);
    __m512 v512f_14 = _mm512_mask3_fmadd_ps(v512f_8, p1, v512f_8, m16_13);
    __m512i v512i_15 = _mm512_rolv_epi32(v512i_12, v512i_2);
    __mmask16 m16_16 = _kshiftri_mask16(m16_13, 1);
    __m512i v512i_17 = _mm512_sllv_epi32(v512i_15, v512i_15);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_16) & 7); i_18++) {
    __m256 v256f_19 = _mm256_loadu_ps(fbuf);
    __m256 v256f_20 = _mm256_fmadd_ps(v256f_19, v256f_19, v256f_19);
    __m512i v512i_21 = _mm512_sub_epi16(v512i_15, v512i_17);
    __m512d v512d_22 = _mm512_loadu_pd(dbuf);
    __m512d v512d_23 = _mm512_shuffle_f64x2(v512d_22, v512d_22, 0xEE);
    }
    __m512 v512f_24 = _mm512_cvtepi32_ps(v512i_12);
    __m512 v512f_25 = _mm512_broadcast_f32x4(p0);
    __m512 v512f_26 = _mm512_max_ps(v512f_14, v512f_8);
    __m256d v256d_27 = _mm256_add_pd(v256d_5, v256d_10);
    __m128d v128d_28 = _mm_set1_pd(2.0);
    __m256 v256f_29 = _mm256_loadu_ps(fbuf);
    __m512 v512f_30 = _mm512_insertf32x8(v512f_24, v256f_29, 1);
    __m512h v512h_31 = g_v512h[1];
    __m512h v512h_32 = _mm512_fmul_pch(v512h_31, v512h_31);
    __m512i v512i_33 = _mm512_rolv_epi32(v512i_15, v512i_17);
    __m512i v512i_34 = _mm512_sllv_epi32(v512i_9, v512i_12);
    g_v256i[0] = p2;
    g_v128f[8] = v128f_11;
    g_v128d[1] = v128d_3;
    g_v512h[11] = v512h_32;
    __m512d v512d_35 = g_v512d[6];
    return v512d_35;
}

__mmask16 torture_22(__m256i p0, __m128i p1, __m256 p2, __m256 p3, __m512d p4) {
    __m256d v256d_1 = g_v256d[3];
    __m256d v256d_2 = _mm256_div_pd(v256d_1, v256d_1);
    __m256d v256d_3 = _mm256_min_pd(v256d_2, v256d_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[9] ^ 113);
    __m512 v512f_5 = g_v512f[0];
    __m512 v512f_6 = _mm512_mask_blend_ps(m16_4, v512f_5, v512f_5);
    __m512i v512i_7 = _mm512_broadcastmw_epi32(m16_4);
    __m512 v512f_8 = _mm512_xor_ps(v512f_6, v512f_6);
    __m128d v128d_9 = g_v128d[2];
    __m128d v128d_10 = _mm_fmadd_pd(v128d_9, v128d_9, v128d_9);
    __m128d v128d_11 = _mm_fmadd_pd(v128d_10, v128d_9, v128d_10);
    __m512i v512i_12 = _mm512_or_si512(v512i_7, v512i_7);
    __m128 v128f_13 = _mm_loadu_ps(fbuf);
    __m128 v128f_14 = _mm_div_ps(v128f_13, v128f_13);
    __m512h v512h_15 = g_v512h[10];
    __m512h v512h_16 = _mm512_add_ph(v512h_15, v512h_15);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_4) & 7); i_17++) {
    __m512i v512i_18 = _mm512_or_si512(v512i_7, v512i_12);
    __m512 v512f_19 = _mm512_fmadd_ps(v512f_6, v512f_8, v512f_5);
    __m128i v128i_20 = _mm_unpacklo_epi32(p1, p1);
    __m512i v512i_21 = _mm512_add_epi32(v512i_12, v512i_18);
    }
    __m512i v512i_22 = _mm512_add_epi64(v512i_7, v512i_12);
    __m512h v512h_23 = _mm512_fmadd_ph(v512h_16, v512h_15, v512h_16);
    __m128i v128i_24 = _mm_sllv_epi32(p1, p1);
    __m512i v512i_25 = _mm512_sub_epi8(v512i_7, v512i_7);
    __m512 v512f_26 = _mm512_insertf32x8(v512f_8, p3, 1);
    __m512i v512i_27 = _mm512_add_epi16(v512i_7, v512i_22);
    __m512i v512i_28 = _mm512_sub_epi32(v512i_12, v512i_7);
    __mmask16 m16_29 = _mm512_cmp_ps_mask(v512f_5, v512f_5, _CMP_LT_OQ);
    __m512i v512i_30 = _mm512_permutex2var_epi32(v512i_25, v512i_12, v512i_7);
    __m256i v256i_31 = _mm256_slli_epi32(p0, 3);
    __m512d v512d_32 = _mm512_cvtps_pd(p2);
    __m512i v512i_33 = _mm512_dpwssd_epi32(v512i_25, v512i_27, v512i_22);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_4) & 7); i_34++) {
    __m256i v256i_35 = _mm256_sub_epi32(v256i_31, v256i_31);
    __m256 v256f_36 = _mm256_shuffle_ps(p3, p3, 0x1B);
    }
    g_v512d[2] = p4;
    g_v256f[10] = p2;
    g_v128d[0] = v128d_11;
    return m16_29;
}

__m512h torture_23(__m512i p0, __m512d p1, __m128i p2, __m512i p3) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_sqrt_ps(v512f_1);
    __m512h v512h_3 = g_v512h[9];
    __mmask32 m32_4 = _mm512_cmp_ph_mask(v512h_3, v512h_3, 1);
    __m512i v512i_5 = _mm512_popcnt_epi64(p3);
    __m512i v512i_6 = _mm512_and_si512(v512i_5, p0);
    __m512i v512i_7 = _mm512_conflict_epi32(p3);
    __mmask8 m8_8 = (__mmask8)(g_m8[4] ^ 160);
    __mmask16 m16_9 = _mm512_kunpackb(m8_8, m8_8);
    __m512 v512f_10 = _mm512_mask_blend_ps(m16_9, v512f_2, v512f_1);
    __m512h v512h_11 = _mm512_fmsub_ph(v512h_3, v512h_3, v512h_3);
    __m512i v512i_12 = _mm512_sub_epi64(p0, v512i_5);
    __m512i v512i_13 = _mm512_cvtps_epi32(v512f_10);
    __m512h v512h_14 = _mm512_mul_ph(v512h_11, v512h_3);
    __m256 v256f_15 = _mm256_loadu_ps(fbuf);
    __m512d v512d_16 = _mm512_cvtps_pd(v256f_15);
    __m128 v128f_17 = g_v128f[10];
    __m128 v128f_18 = _mm_sub_ps(v128f_17, v128f_17);
    __m256 v256f_19 = _mm256_max_ps(v256f_15, v256f_15);
    __m256 v256f_20 = _mm512_cvtpd_ps(p1);
    __m512 v512f_21 = _mm512_rcp14_ps(v512f_10);
    __m256i v256i_22 = g_v256i[3];
    __m256i v256i_23 = _mm256_slli_epi32(v256i_22, 3);
    __m512d v512d_24 = _mm512_set1_pd(2.0);
    __m512i v512i_25 = _mm512_rolv_epi32(p3, p3);
    __m256 v256f_26 = _mm256_unpacklo_ps(v256f_15, v256f_20);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m256i v256i_27 = _mm256_sub_epi16(v256i_22, v256i_22);
    __m512 v512f_28 = _mm512_broadcast_f32x4(v128f_18);
    __m512d v512d_29 = _mm512_add_pd(p1, v512d_16);
    }
    g_v512i[10] = p3;
    g_v128f[5] = v128f_18;
    g_v512h[0] = v512h_3;
    g_m8[7] = m8_8;
    g_m32[11] = m32_4;
    return v512h_14;
}

__mmask8 torture_24(__m128d p0, __m256h p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_add_ps(v512f_1, v512f_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[11] ^ 75);
    unsigned u32_4 = _cvtmask16_u32(m16_3);
    __m256 v256f_5 = g_v256f[11];
    __m256 v256f_6 = _mm256_and_ps(v256f_5, v256f_5);
    __m512h v512h_7 = g_v512h[0];
    __m512h v512h_8 = _mm512_getexp_ph(v512h_7);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_mask_blend_epi32(m16_3, v512i_9, v512i_9);
    __m512i v512i_11 = _mm512_or_si512(v512i_9, v512i_10);
    __m512h v512h_12 = _mm512_fmsub_ph(v512h_7, v512h_8, v512h_8);
    __m256 v256f_13 = _mm256_fnmadd_ps(v256f_6, v256f_5, v256f_5);
    __m256d v256d_14 = g_v256d[7];
    __m256d v256d_15 = _mm256_max_pd(v256d_14, v256d_14);
    __m256 v256f_16 = _mm256_mul_ps(v256f_6, v256f_6);
    __mmask16 m16_17 = _mm512_cmp_epu32_mask(v512i_11, v512i_11, 4);
    __m512h v512h_18 = _mm512_add_ph(v512h_12, v512h_8);
    __m128d v128d_19 = _mm_mul_pd(p0, p0);
    __m128i v128i_20 = _mm512_cvtepi32_epi8(v512i_10);
    __m512i v512i_21 = _mm512_sllv_epi32(v512i_10, v512i_9);
    __m512d v512d_22 = _mm512_loadu_pd(dbuf);
    __m512d v512d_23 = _mm512_sub_pd(v512d_22, v512d_22);
    __m512h v512h_24 = _mm512_mul_ph(v512h_7, v512h_8);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_17) & 7); i_25++) {
    __m512i v512i_26 = _mm512_permutexvar_epi32(v512i_9, v512i_11);
    __m512 v512f_27 = _mm512_mask_blend_ps(m16_3, v512f_2, v512f_1);
    __m512 v512f_28 = _mm512_rsqrt14_ps(v512f_27);
    __mmask16 m16_29 = _kand_mask16(m16_3, m16_17);
    }
    __m256i v256i_30 = g_v256i[4];
    __m256i v256i_31 = _mm256_add_epi64(v256i_30, v256i_30);
    __m512i v512i_32 = _mm512_mask_add_epi32(v512i_9, m16_17, v512i_21, v512i_9);
    __m128i v128i_33 = _mm_sllv_epi32(v128i_20, v128i_20);
    __m128i v128i_34 = _mm_abs_epi32(v128i_20);
    __m512i v512i_35 = _mm512_permutexvar_epi32(v512i_9, v512i_21);
    __m512i v512i_36 = _mm512_mask_add_epi32(v512i_9, m16_17, v512i_32, v512i_9);
    __m256d v256d_37 = _mm256_fmadd_pd(v256d_14, v256d_15, v256d_14);
    __m256i v256i_38 = _mm256_and_si256(v256i_30, v256i_31);
    __mmask32 m32_39 = _mm512_kunpackw(m16_3, m16_3);
    __m512d v512d_40 = _mm512_min_pd(v512d_22, v512d_23);
    __m128 v128f_41 = _mm_loadu_ps(fbuf);
    __m128 v128f_42 = _mm_min_ps(v128f_41, v128f_41);
    g_v256f[3] = v256f_6;
    g_v256i[4] = v256i_38;
    g_v512h[3] = v512h_8;
    g_v256h[6] = p1;
    g_m16[8] = m16_3;
    g_m32[11] = m32_39;
    __mmask8 m8_43 = (__mmask8)(g_m8[3] ^ 41);
    return m8_43;
}

__m512d torture_25(__m256 p0, __m128 p1, __m512d p2, __m128h p3, __m256d p4) {
    __m512i v512i_1 = g_v512i[8];
    __mmask64 m64_2 = _mm512_movepi8_mask(v512i_1);
    __m512 v512f_3 = g_v512f[5];
    __mmask16 m16_4 = (__mmask16)(g_m16[1] ^ 18);
    __m512 v512f_5 = _mm512_mask_add_ps(v512f_3, m16_4, v512f_3, v512f_3);
    __m512i v512i_6 = _mm512_sub_epi16(v512i_1, v512i_1);
    __m512 v512f_7 = _mm512_mul_ps(v512f_3, v512f_5);
    __m128i v128i_8 = g_v128i[3];
    __m128i v128i_9 = _mm_add_epi64(v128i_8, v128i_8);
    __m256d v256d_10 = _mm256_max_pd(p4, p4);
    __m512 v512f_11 = _mm512_shuffle_f32x4(v512f_7, v512f_5, 0x4E);
    __m256i v256i_12 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_13 = _mm256_slli_epi32(v256i_12, 3);
    __m128 v128f_14 = _mm_div_ps(p1, p1);
    __m512 v512f_15 = _mm512_unpacklo_ps(v512f_3, v512f_3);
    __m128i v128i_16 = _mm512_cvtepi32_epi8(v512i_6);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m512i v512i_17 = _mm512_sub_epi8(v512i_1, v512i_1);
    __m256 v256f_18 = _mm256_fmadd_ps(p0, p0, p0);
    __m256i v256i_19 = _mm512_cvtepi64_epi32(v512i_17);
    __m128h v128h_20 = _mm512_cvtpd_ph(p2);
    }
    __m512h v512h_21 = g_v512h[0];
    __mmask32 m32_22 = _mm512_cmp_ph_mask(v512h_21, v512h_21, 1);
    __m128i v128i_23 = _mm_slli_epi32(v128i_8, 3);
    __m512i v512i_24 = _mm512_rolv_epi32(v512i_1, v512i_1);
    __m512i v512i_25 = _mm512_slli_epi32(v512i_1, 3);
    __m512 v512f_26 = _mm512_min_ps(v512f_5, v512f_7);
    __m512i v512i_27 = _mm512_madd52lo_epu64(v512i_1, v512i_24, v512i_6);
    __m128d v128d_28 = g_v128d[0];
    __m128d v128d_29 = _mm_sub_pd(v128d_28, v128d_28);
    __m128 v128f_30 = _mm_min_ps(v128f_14, v128f_14);
    __m128d v128d_31 = _mm_div_pd(v128d_29, v128d_29);
    __m512d v512d_32 = _mm512_unpackhi_pd(p2, p2);
    __m512 v512f_33 = _mm512_roundscale_ps(v512f_11, 0x10);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_4) & 7); i_34++) {
    __m256i v256i_35 = _mm256_shuffle_epi8(v256i_12, v256i_12);
    __m256i v256i_36 = _mm256_shuffle_epi8(v256i_13, v256i_12);
    }
    g_v256f[6] = p0;
    g_v256i[4] = v256i_12;
    g_v128f[4] = v128f_30;
    g_v128d[9] = v128d_31;
    g_v128i[8] = v128i_16;
    g_v512h[3] = v512h_21;
    g_v128h[10] = p3;
    g_m32[1] = m32_22;
    return v512d_32;
}

__mmask64 torture_26(__m256 p0, __m512h p1, __m128d p2) {
    __m512i v512i_1 = g_v512i[10];
    __m512i v512i_2 = _mm512_sub_epi32(v512i_1, v512i_1);
    __m256d v256d_3 = g_v256d[1];
    __m256d v256d_4 = _mm256_add_pd(v256d_3, v256d_3);
    __m128 v128f_5 = g_v128f[7];
    __m128 v128f_6 = _mm_add_ps(v128f_5, v128f_5);
    __m128i v128i_7 = g_v128i[9];
    __m128i v128i_8 = _mm_add_epi64(v128i_7, v128i_7);
    __m128i v128i_9 = _mm_sub_epi32(v128i_8, v128i_8);
    __m128 v128f_10 = _mm_max_ps(v128f_6, v128f_5);
    __m128 v128f_11 = _mm_and_ps(v128f_10, v128f_5);
    __m256 v256f_12 = _mm256_div_ps(p0, p0);
    __m512 v512f_13 = _mm512_loadu_ps(fbuf);
    __m512 v512f_14 = _mm512_fmadd_ps(v512f_13, v512f_13, v512f_13);
    __mmask64 m64_15 = (__mmask64)(g_m64[7] ^ 231);
    __mmask64 m64_16 = _kor_mask64(m64_15, m64_15);
    __m128i v128i_17 = _mm512_cvtepi32_epi8(v512i_1);
    __m128i v128i_18 = _mm_sub_epi16(v128i_9, v128i_7);
    __m256i v256i_19 = _mm256_set1_epi32(0x55);
    __m512h v512h_20 = _mm512_cvtepi16_ph(v512i_1);
    __m256i v256i_21 = _mm256_sub_epi32(v256i_19, v256i_19);
    __m512i v512i_22 = _mm512_dpwssd_epi32(v512i_1, v512i_2, v512i_2);
    __mmask16 m16_23 = _mm512_fpclass_ps_mask(v512f_14, 0x3);
    __mmask32 m32_24 = _mm512_cmp_ph_mask(p1, v512h_20, 1);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_23) & 7); i_25++) {
    __m512i v512i_26 = _mm512_sub_epi16(v512i_2, v512i_2);
    __m512 v512f_27 = _mm512_fmadd_ps(v512f_13, v512f_14, v512f_13);
    __m256 v256f_28 = _mm256_shuffle_ps(v256f_12, p0, 0x1B);
    __m256 v256f_29 = _mm256_add_ps(v256f_12, v256f_12);
    }
    __m512h v512h_30 = _mm512_mul_ph(v512h_20, v512h_20);
    __m256h v256h_31 = _mm512_cvtxps_ph(v512f_13);
    __m512 v512f_32 = _mm512_roundscale_ps(v512f_14, 0x10);
    __m512 v512f_33 = _mm512_roundscale_ps(v512f_13, 0x10);
    __m512 v512f_34 = _mm512_cvtxph_ps(v256h_31);
    __m512d v512d_35 = _mm512_loadu_pd(dbuf);
    __m512d v512d_36 = _mm512_sub_pd(v512d_35, v512d_35);
    __m256i v256i_37 = _mm256_add_epi8(v256i_21, v256i_19);
    __m512i v512i_38 = _mm512_conflict_epi32(v512i_2);
    __m512i v512i_39 = _mm512_cvtpd_epi64(v512d_36);
    __m512 v512f_40 = _mm512_min_ps(v512f_33, v512f_32);
    if (((unsigned long long)m16_23) & 1ULL) {
    __m512 v512f_41 = _mm512_max_ps(v512f_33, v512f_34);
    __m256 v256f_42 = _mm256_fmsub_ps(v256f_12, p0, p0);
    }
    g_v512f[0] = v512f_14;
    g_v512d[7] = v512d_35;
    g_v256f[6] = p0;
    g_v128f[0] = v128f_11;
    g_v512h[0] = v512h_20;
    g_m16[6] = m16_23;
    g_m64[6] = m64_15;
    return m64_16;
}

__m512h torture_27(__m128h p0, __m128d p1, __m128 p2, __m128i p3) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sub_epi16(v256i_1, v256i_1);
    __m256i v256i_3 = _mm256_add_epi64(v256i_1, v256i_1);
    __m256i v256i_4 = _mm256_add_epi64(v256i_3, v256i_2);
    __m512d v512d_5 = _mm512_set1_pd(2.0);
    __m256i v256i_6 = _mm256_add_epi64(v256i_3, v256i_4);
    __m512 v512f_7 = _mm512_loadu_ps(fbuf);
    __m512 v512f_8 = _mm512_min_ps(v512f_7, v512f_7);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_sub_epi64(v512i_9, v512i_9);
    __m128 v128f_11 = _mm_unpacklo_ps(p2, p2);
    __m512d v512d_12 = _mm512_div_pd(v512d_5, v512d_5);
    __m512 v512f_13 = _mm512_getexp_ps(v512f_8);
    __mmask16 m16_14 = (__mmask16)(g_m16[0] ^ 15);
    __m512i v512i_15 = _mm512_mask_blend_epi32(m16_14, v512i_10, v512i_9);
    __m512i v512i_16 = _mm512_rol_epi32(v512i_10, 5);
    __mmask16 m16_17 = _kxor_mask16(m16_14, m16_14);
    __m512 v512f_18 = _mm512_broadcast_f32x4(v128f_11);
    for (int i_19 = 0; i_19 < (int)(((unsigned long long)m16_17) & 7); i_19++) {
    __m256i v256i_20 = _mm256_mullo_epi32(v256i_1, v256i_2);
    __mmask16 m16_21 = _mm512_movepi32_mask(v512i_16);
    __m512 v512f_22 = _mm512_rsqrt14_ps(v512f_13);
    __m128i v128i_23 = _mm_add_epi32(p3, p3);
    __m512d v512d_24 = _mm512_insertf64x2(v512d_12, p1, 3);
    }
    __m512 v512f_25 = _mm512_shuffle_ps(v512f_13, v512f_13, 0x1B);
    __m128 v128f_26 = _mm_and_ps(p2, v128f_11);
    __m512i v512i_27 = _mm512_madd52lo_epu64(v512i_10, v512i_10, v512i_9);
    __m512i v512i_28 = _mm512_rolv_epi32(v512i_10, v512i_16);
    __mmask16 m16_29 = _mm512_cmp_epu32_mask(v512i_10, v512i_15, 4);
    __m256 v256f_30 = _mm512_cvtpd_ps(v512d_5);
    g_v512f[11] = v512f_13;
    g_v256i[2] = v256i_1;
    g_m16[9] = m16_17;
    __m512h v512h_31 = g_v512h[0];
    return v512h_31;
}

__m256d torture_28(__m512i p0, __m512d p1, __m512 p2, __m512d p3, __m512 p4) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sub_epi64(v256i_1, v256i_1);
    __m128d v128d_3 = g_v128d[11];
    __m128d v128d_4 = _mm_mul_pd(v128d_3, v128d_3);
    __mmask64 m64_5 = _mm512_cmp_epi8_mask(p0, p0, 1);
    __m512i v512i_6 = _mm512_add_epi16(p0, p0);
    __m256d v256d_7 = g_v256d[1];
    __m256d v256d_8 = _mm256_min_pd(v256d_7, v256d_7);
    __m512i v512i_9 = _mm512_xor_si512(p0, v512i_6);
    __m512h v512h_10 = g_v512h[0];
    __m512h v512h_11 = _mm512_mul_ph(v512h_10, v512h_10);
    __m128 v128f_12 = g_v128f[0];
    __m128 v128f_13 = _mm_mul_ps(v128f_12, v128f_12);
    __m256i v256i_14 = _mm256_shuffle_epi8(v256i_1, v256i_2);
    __mmask32 m32_15 = _mm512_cmp_ph_mask(v512h_11, v512h_10, 1);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m32_15) & 7); i_16++) {
    __m128d v128d_17 = _mm_unpackhi_pd(v128d_4, v128d_3);
    __m512 v512f_18 = _mm512_roundscale_ps(p2, 0x10);
    }
    __mmask16 m16_19 = (__mmask16)(g_m16[8] ^ 153);
    unsigned u32_20 = _cvtmask16_u32(m16_19);
    __m128d v128d_21 = _mm_min_pd(v128d_4, v128d_3);
    __m128d v128d_22 = _mm_div_pd(v128d_21, v128d_3);
    __m512i v512i_23 = _mm512_sllv_epi32(v512i_6, v512i_9);
    g_v512h[6] = v512h_11;
    g_m64[7] = m64_5;
    return v256d_7;
}

__m512h torture_29(__m256h p0, __m256d p1, __m128 p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_mullo_epi32(v512i_1, v512i_1);
    __m512i v512i_3 = _mm512_sub_epi16(v512i_1, v512i_2);
    __m512i v512i_4 = _mm512_shuffle_epi8(v512i_2, v512i_1);
    __mmask16 m16_5 = (__mmask16)(g_m16[3] ^ 11);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_mask_blend_ps(m16_5, v512f_6, v512f_6);
    __m512i v512i_8 = _mm512_broadcastmw_epi32(m16_5);
    __m512h v512h_9 = g_v512h[0];
    __mmask32 m32_10 = _mm512_cmp_ph_mask(v512h_9, v512h_9, 1);
    __m512 v512f_11 = _mm512_xor_ps(v512f_7, v512f_7);
    __m512i v512i_12 = _mm512_unpacklo_epi32(v512i_3, v512i_1);
    __m512 v512f_13 = _mm512_unpacklo_ps(v512f_7, v512f_11);
    __m256i v256i_14 = _mm512_cvtepi64_epi32(v512i_2);
    __m512 v512f_15 = _mm512_set1_ps(1.0f);
    __m512i v512i_16 = _mm512_conflict_epi32(v512i_8);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m128d v128d_17 = g_v128d[8];
    __m128d v128d_18 = _mm_sub_pd(v128d_17, v128d_17);
    __m256 v256f_19 = _mm256_set1_ps(1.0f);
    __m512i v512i_20 = _mm512_rol_epi32(v512i_16, 5);
    __m512d v512d_21 = g_v512d[0];
    __m512d v512d_22 = _mm512_min_pd(v512d_21, v512d_21);
    __m512 v512f_23 = _mm512_mask3_fmadd_ps(v512f_15, v512f_11, v512f_6, m16_5);
    __m512d v512d_24 = _mm512_unpackhi_pd(v512d_21, v512d_21);
    }
    __m512 v512f_25 = _mm512_getexp_ps(v512f_11);
    __m256 v256f_26 = g_v256f[4];
    __m512 v512f_27 = _mm512_insertf32x8(v512f_15, v256f_26, 1);
    __m128i v128i_28 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_29 = _mm_mullo_epi32(v128i_28, v128i_28);
    __m256i v256i_30 = _mm256_slli_epi32(v256i_14, 3);
    __m512i v512i_31 = _mm512_permutexvar_epi32(v512i_8, v512i_12);
    __m512 v512f_32 = _mm512_fmadd_ps(v512f_11, v512f_27, v512f_25);
    __m128i v128i_33 = _mm_sub_epi64(v128i_28, v128i_29);
    __mmask8 m8_34 = (__mmask8)(g_m8[10] ^ 40);
    __m512i v512i_35 = _mm512_broadcastmb_epi64(m8_34);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m256d v256d_36 = _mm256_set1_pd(2.0);
    __m512d v512d_37 = _mm512_loadu_pd(dbuf);
    __m512i v512i_38 = _mm512_cvtpd_epi64(v512d_37);
    __m256d v256d_39 = _mm256_add_pd(p1, p1);
    }
    g_v512f[2] = v512f_32;
    g_v256d[0] = p1;
    g_v128f[3] = p2;
    g_v512h[4] = v512h_9;
    g_m16[4] = m16_5;
    g_m32[6] = m32_10;
    return v512h_9;
}

__m256d torture_30(__m256h p0, __m128d p1) {
    __m512 v512f_1 = g_v512f[3];
    __m256h v256h_2 = _mm512_cvtxps_ph(v512f_1);
    __m512d v512d_3 = _mm512_set1_pd(2.0);
    __m256 v256f_4 = _mm256_loadu_ps(fbuf);
    __m256 v256f_5 = _mm256_unpacklo_ps(v256f_4, v256f_4);
    __m128 v128f_6 = g_v128f[9];
    __m128 v128f_7 = _mm_mul_ps(v128f_6, v128f_6);
    __m512 v512f_8 = _mm512_fmsub_ps(v512f_1, v512f_1, v512f_1);
    unsigned u32_9 = 77;
    __mmask16 m16_10 = _cvtu32_mask16(u32_9);
    __m512i v512i_11 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_12 = _mm512_conflict_epi32(v512i_11);
    __m512 v512f_13 = _mm512_min_ps(v512f_1, v512f_1);
    __m256i v256i_14 = g_v256i[3];
    __m256i v256i_15 = _mm256_add_epi32(v256i_14, v256i_14);
    __m128i v128i_16 = g_v128i[11];
    __m128i v128i_17 = _mm_add_epi32(v128i_16, v128i_16);
    __mmask16 m16_18 = _kshiftri_mask16(m16_10, 1);
    __m512i v512i_19 = _mm512_sub_epi32(v512i_11, v512i_12);
    __m128 v128f_20 = _mm_add_ps(v128f_7, v128f_7);
    __m128 v128f_21 = _mm_sub_ps(v128f_20, v128f_20);
    __m256d v256d_22 = g_v256d[6];
    __m256d v256d_23 = _mm256_unpackhi_pd(v256d_22, v256d_22);
    __mmask8 m8_24 = (__mmask8)(g_m8[5] ^ 158);
    __mmask16 m16_25 = _mm512_kunpackb(m8_24, m8_24);
    __m256d v256d_26 = _mm256_add_pd(v256d_23, v256d_23);
    __mmask16 m16_27 = _mm512_kunpackb(m8_24, m8_24);
    __m512i v512i_28 = _mm512_sub_epi32(v512i_19, v512i_19);
    __m512 v512f_29 = _mm512_shuffle_ps(v512f_1, v512f_13, 0x1B);
    __mmask8 m8_30 = _kand_mask8(m8_24, m8_24);
    __m256i v256i_31 = _mm256_and_si256(v256i_15, v256i_15);
    __m256i v256i_32 = _mm512_cvtepi64_epi32(v512i_11);
    __m512d v512d_33 = _mm512_div_pd(v512d_3, v512d_3);
    __mmask16 m16_34 = _kand_mask16(m16_18, m16_27);
    __m512 v512f_35 = _mm512_cvtxph_ps(v256h_2);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_27) & 7); i_36++) {
    __mmask16 m16_37 = _mm512_testn_epi32_mask(v512i_28, v512i_12);
    __m512h v512h_38 = g_v512h[4];
    __m512h v512h_39 = _mm512_sqrt_ph(v512h_38);
    __m512 v512f_40 = _mm512_rcp14_ps(v512f_1);
    }
    g_v512f[2] = v512f_13;
    g_v256f[5] = v256f_5;
    g_v256d[4] = v256d_22;
    g_v128f[4] = v128f_20;
    g_v128i[10] = v128i_17;
    g_m16[9] = m16_27;
    return v256d_22;
}

__m256h torture_31(__m128i p0, __m128h p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_and_ps(v512f_1, v512f_1);
    __m512d v512d_3 = g_v512d[5];
    __m512i v512i_4 = _mm512_cvtpd_epi64(v512d_3);
    __m512i v512i_5 = _mm512_or_si512(v512i_4, v512i_4);
    __m256d v256d_6 = g_v256d[9];
    __m256d v256d_7 = _mm256_min_pd(v256d_6, v256d_6);
    __m512 v512f_8 = _mm512_shuffle_f32x4(v512f_2, v512f_1, 0x4E);
    __m512d v512d_9 = _mm512_shuffle_f64x2(v512d_3, v512d_3, 0xEE);
    __m256i v256i_10 = _mm256_set1_epi32(0x55);
    __m512 v512f_11 = _mm512_min_ps(v512f_8, v512f_2);
    __m512 v512f_12 = _mm512_fmadd_ps(v512f_2, v512f_8, v512f_2);
    __mmask8 m8_13 = (__mmask8)(g_m8[6] ^ 75);
    __m512d v512d_14 = _mm512_mask_blend_pd(m8_13, v512d_3, v512d_3);
    __m512 v512f_15 = _mm512_sqrt_ps(v512f_11);
    __m128 v128f_16 = g_v128f[8];
    __m128 v128f_17 = _mm_shuffle_ps(v128f_16, v128f_16, 0x1B);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m8_13) & 7); i_18++) {
    __m256d v256d_19 = _mm256_sub_pd(v256d_7, v256d_7);
    __m128d v128d_20 = g_v128d[7];
    __m512d v512d_21 = _mm512_insertf64x2(v512d_14, v128d_20, 3);
    }
    __m256d v256d_22 = _mm512_extractf64x4_pd(v512d_14, 1);
    __m512h v512h_23 = g_v512h[1];
    __m512i v512i_24 = _mm512_cvtph_epi16(v512h_23);
    __m128 v128f_25 = _mm_add_ps(v128f_17, v128f_16);
    unsigned u32_26 = 31;
    __mmask16 m16_27 = _cvtu32_mask16(u32_26);
    __m512 v512f_28 = _mm512_shuffle_f32x4(v512f_11, v512f_1, 0x4E);
    __m512i v512i_29 = _mm512_mask_add_epi32(v512i_24, m16_27, v512i_24, v512i_5);
    __m256 v256f_30 = _mm256_loadu_ps(fbuf);
    __m256 v256f_31 = _mm256_unpacklo_ps(v256f_30, v256f_30);
    for (int i_32 = 0; i_32 < (int)(((unsigned long long)m16_27) & 7); i_32++) {
    __m512i v512i_33 = _mm512_cvtpd_epi64(v512d_9);
    __mmask8 m8_34 = _kand_mask8(m8_13, m8_13);
    __m256i v256i_35 = _mm256_slli_epi32(v256i_10, 3);
    }
    g_v512d[4] = v512d_9;
    g_v512i[10] = v512i_5;
    g_v128f[2] = v128f_17;
    g_v128i[11] = p0;
    g_v128h[6] = p1;
    __m256h v256h_36 = g_v256h[2];
    return v256h_36;
}

__m512d torture_32(__m256h p0, __m128 p1, __m256 p2, __m512i p3, __m256 p4) {
    __m512d v512d_1 = _mm512_cvtps_pd(p4);
    __m512i v512i_2 = _mm512_popcnt_epi64(p3);
    __m512 v512f_3 = g_v512f[3];
    __m256h v256h_4 = _mm512_cvtxps_ph(v512f_3);
    __m512i v512i_5 = _mm512_ternarylogic_epi32(v512i_2, v512i_2, p3, 0x96);
    __m512 v512f_6 = _mm512_cvtxph_ps(v256h_4);
    __m512i v512i_7 = _mm512_cvtpd_epi64(v512d_1);
    __mmask16 m16_8 = _mm512_testn_epi32_mask(v512i_2, v512i_5);
    __m128 v128f_9 = _mm_fmadd_ps(p1, p1, p1);
    __m512i v512i_10 = _mm512_conflict_epi32(p3);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m256 v256f_11 = _mm256_shuffle_ps(p2, p2, 0x1B);
    __m512 v512f_12 = _mm512_cvtepi32_ps(v512i_2);
    __m128 v128f_13 = _mm_shuffle_ps(v128f_9, v128f_9, 0x1B);
    __m256 v256f_14 = _mm256_xor_ps(v256f_11, p4);
    }
    __m512 v512f_15 = _mm512_and_ps(v512f_3, v512f_3);
    __mmask64 m64_16 = _mm512_movepi8_mask(v512i_7);
    __m128i v128i_17 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_18 = _mm_add_epi8(v128i_17, v128i_17);
    __m128d v128d_19 = g_v128d[5];
    __m128d v128d_20 = _mm_div_pd(v128d_19, v128d_19);
    __m256i v256i_21 = g_v256i[4];
    __m256i v256i_22 = _mm256_sub_epi16(v256i_21, v256i_21);
    __m512i v512i_23 = _mm512_cvtpd_epi64(v512d_1);
    __m512 v512f_24 = _mm512_mask_blend_ps(m16_8, v512f_3, v512f_6);
    __m512d v512d_25 = _mm512_sub_pd(v512d_1, v512d_1);
    __m512d v512d_26 = _mm512_max_pd(v512d_25, v512d_1);
    __m128i v128i_27 = _mm_sub_epi16(v128i_18, v128i_17);
    for (int i_28 = 0; i_28 < (int)(((unsigned long long)m16_8) & 7); i_28++) {
    __m512 v512f_29 = _mm512_mask_blend_ps(m16_8, v512f_24, v512f_24);
    __mmask8 m8_30 = (__mmask8)(g_m8[5] ^ 239);
    __mmask8 m8_31 = _kand_mask8(m8_30, m8_30);
    __mmask32 m32_32 = (__mmask32)(g_m32[3] ^ 190);
    __mmask32 m32_33 = _kxor_mask32(m32_32, m32_32);
    __m128d v128d_34 = _mm_add_pd(v128d_20, v128d_19);
    __m512d v512d_35 = _mm512_fmadd_pd(v512d_26, v512d_1, v512d_26);
    }
    g_v512i[6] = v512i_7;
    g_v256i[3] = v256i_21;
    g_v128f[10] = v128f_9;
    g_v128d[1] = v128d_19;
    g_v128i[8] = v128i_27;
    g_m16[7] = m16_8;
    g_m64[3] = m64_16;
    return v512d_26;
}

__m128d torture_33(__m512d p0, __m256d p1, __m128i p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_sub_ps(v512f_1, v512f_1);
    __m512 v512f_3 = _mm512_shuffle_ps(v512f_2, v512f_1, 0x1B);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_and_si512(v512i_4, v512i_4);
    __m128d v128d_6 = g_v128d[4];
    __m512d v512d_7 = _mm512_insertf64x2(p0, v128d_6, 3);
    __mmask64 m64_8 = (__mmask64)(g_m64[2] ^ 19);
    __mmask64 m64_9 = _kor_mask64(m64_8, m64_8);
    __mmask64 m64_10 = _kor_mask64(m64_8, m64_9);
    __m256 v256f_11 = g_v256f[8];
    __m256 v256f_12 = _mm256_unpacklo_ps(v256f_11, v256f_11);
    __m256 v256f_13 = _mm256_sub_ps(v256f_11, v256f_12);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m64_8) & 7); i_14++) {
    __m256 v256f_15 = _mm256_max_ps(v256f_13, v256f_13);
    __mmask16 m16_16 = _mm512_cmp_epi32_mask(v512i_4, v512i_5, 2);
    __m512d v512d_17 = _mm512_mul_pd(v512d_7, p0);
    }
    __m512i v512i_18 = _mm512_rol_epi32(v512i_5, 5);
    __m256 v256f_19 = _mm256_mul_ps(v256f_13, v256f_13);
    __m128i v128i_20 = _mm_xor_si128(p2, p2);
    __m512i v512i_21 = _mm512_and_si512(v512i_18, v512i_18);
    __m512i v512i_22 = _mm512_sub_epi16(v512i_21, v512i_21);
    __m128 v128f_23 = _mm_loadu_ps(fbuf);
    __m128 v128f_24 = _mm_fnmadd_ps(v128f_23, v128f_23, v128f_23);
    __m256h v256h_25 = g_v256h[9];
    __m512 v512f_26 = _mm512_cvtxph_ps(v256h_25);
    __m256 v256f_27 = _mm256_div_ps(v256f_11, v256f_11);
    __m128 v128f_28 = _mm_fnmadd_ps(v128f_24, v128f_23, v128f_24);
    if (((unsigned long long)m64_9) & 1ULL) {
    __m512i v512i_29 = _mm512_lzcnt_epi32(v512i_21);
    __m512i v512i_30 = _mm512_conflict_epi32(v512i_18);
    __m512i v512i_31 = _mm512_broadcast_i32x4(p2);
    }
    g_v512d[10] = v512d_7;
    g_v128f[10] = v128f_23;
    g_v128d[4] = v128d_6;
    g_v256h[3] = v256h_25;
    g_m64[8] = m64_10;
    return v128d_6;
}

__mmask8 torture_34(__m512 p0, __m128i p1, __m128d p2) {
    __m128 v128f_1 = g_v128f[6];
    __m128 v128f_2 = _mm_fnmadd_ps(v128f_1, v128f_1, v128f_1);
    __m128 v128f_3 = _mm_shuffle_ps(v128f_1, v128f_1, 0x1B);
    __m512 v512f_4 = _mm512_rsqrt14_ps(p0);
    __m512h v512h_5 = g_v512h[3];
    __m512h v512h_6 = _mm512_sqrt_ph(v512h_5);
    __m512h v512h_7 = _mm512_fmsub_ph(v512h_6, v512h_5, v512h_5);
    __m128i v128i_8 = _mm_add_epi16(p1, p1);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_unpacklo_epi32(v512i_9, v512i_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_12 = _mm512_cmp_pd_mask(v512d_11, v512d_11, _CMP_GE_OQ);
    __m512d v512d_13 = _mm512_min_pd(v512d_11, v512d_11);
    __m128 v128f_14 = _mm_fmsub_ps(v128f_1, v128f_1, v128f_3);
    __m128d v128d_15 = _mm_min_pd(p2, p2);
    __m256 v256f_16 = _mm256_loadu_ps(fbuf);
    __m256 v256f_17 = _mm256_fmsub_ps(v256f_16, v256f_16, v256f_16);
    __m512h v512h_18 = _mm512_fmadd_ph(v512h_7, v512h_6, v512h_6);
    __mmask16 m16_19 = (__mmask16)(g_m16[3] ^ 73);
    __m512i v512i_20 = _mm512_mask_add_epi32(v512i_9, m16_19, v512i_9, v512i_9);
    __mmask32 m32_21 = (__mmask32)(g_m32[0] ^ 160);
    __mmask32 m32_22 = _kxor_mask32(m32_21, m32_21);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m128d v128d_23 = _mm_min_pd(p2, p2);
    __m512d v512d_24 = _mm512_mask_blend_pd(m8_12, v512d_13, v512d_13);
    }
    __mmask16 m16_25 = _kshiftli_mask16(m16_19, 2);
    __m128d v128d_26 = _mm_mul_pd(v128d_15, p2);
    __m128d v128d_27 = _mm_add_pd(v128d_15, p2);
    __m256i v256i_28 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_29 = _mm256_sub_epi8(v256i_28, v256i_28);
    __m128 v128f_30 = _mm_sqrt_ps(v128f_1);
    __m512i v512i_31 = _mm512_add_epi64(v512i_10, v512i_10);
    __m256i v256i_32 = _mm256_slli_epi32(v256i_28, 3);
    __m256i v256i_33 = _mm256_or_si256(v256i_32, v256i_32);
    __m256 v256f_34 = _mm256_shuffle_ps(v256f_17, v256f_17, 0x1B);
    __m256 v256f_35 = _mm256_xor_ps(v256f_16, v256f_16);
    __m512 v512f_36 = _mm512_min_ps(p0, v512f_4);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m512i v512i_37 = _mm512_shuffle_i32x4(v512i_20, v512i_9, 0x1B);
    __mmask16 m16_38 = _mm512_fpclass_ps_mask(v512f_36, 0x3);
    __m256d v256d_39 = g_v256d[0];
    __m256d v256d_40 = _mm256_fmadd_pd(v256d_39, v256d_39, v256d_39);
    __m128 v128f_41 = _mm_mul_ps(v128f_3, v128f_14);
    __m512 v512f_42 = _mm512_sqrt_ps(v512f_4);
    __m512i v512i_43 = _mm512_sub_epi64(v512i_20, v512i_31);
    }
    g_v512f[9] = p0;
    g_v512i[2] = v512i_9;
    g_v128f[9] = v128f_3;
    g_v512h[11] = v512h_18;
    g_m8[2] = m8_12;
    g_m16[0] = m16_19;
    return m8_12;
}

__m512 torture_35(__m128h p0, __m512 p1, __m512d p2) {
    __mmask16 m16_1 = _mm512_fpclass_ps_mask(p1, 0x3);
    __m512i v512i_2 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_3 = _mm512_testn_epi32_mask(v512i_2, v512i_2);
    __m128d v128d_4 = g_v128d[6];
    __m128d v128d_5 = _mm_add_pd(v128d_4, v128d_4);
    __m512 v512f_6 = _mm512_mask_blend_ps(m16_3, p1, p1);
    __m128i v128i_7 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_8 = _mm_slli_epi32(v128i_7, 3);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_add_epi8(v256i_9, v256i_9);
    __mmask8 m8_11 = _mm512_cmp_pd_mask(p2, p2, _CMP_GE_OQ);
    __m512d v512d_12 = _mm512_sub_pd(p2, p2);
    __m128 v128f_13 = g_v128f[0];
    __m512 v512f_14 = _mm512_broadcast_f32x4(v128f_13);
    __m512i v512i_15 = _mm512_mask_blend_epi32(m16_1, v512i_2, v512i_2);
    __m512 v512f_16 = _mm512_mask3_fmadd_ps(v512f_14, v512f_14, v512f_6, m16_1);
    __m256 v256f_17 = _mm256_loadu_ps(fbuf);
    __m512 v512f_18 = _mm512_insertf32x8(v512f_14, v256f_17, 1);
    __m256 v256f_19 = _mm256_unpacklo_ps(v256f_17, v256f_17);
    __m256d v256d_20 = _mm256_set1_pd(2.0);
    __m128 v128f_21 = _mm_mul_ps(v128f_13, v128f_13);
    __m256d v256d_22 = _mm256_mul_pd(v256d_20, v256d_20);
    __m512 v512f_23 = _mm512_mask_add_ps(v512f_18, m16_3, p1, v512f_16);
    __m512i v512i_24 = _mm512_dpwssd_epi32(v512i_15, v512i_2, v512i_2);
    __m512i v512i_25 = _mm512_cvtpd_epi64(p2);
    __m128 v128f_26 = _mm_min_ps(v128f_13, v128f_21);
    __m512d v512d_27 = _mm512_cvtps_pd(v256f_17);
    __m128i v128i_28 = _mm_sub_epi16(v128i_8, v128i_7);
    __m512d v512d_29 = _mm512_mul_pd(p2, v512d_27);
    __m512h v512h_30 = g_v512h[4];
    __m512h v512h_31 = _mm512_add_ph(v512h_30, v512h_30);
    __m128d v128d_32 = _mm_add_pd(v128d_5, v128d_5);
    __m128i v128i_33 = _mm_add_epi64(v128i_28, v128i_28);
    __m512i v512i_34 = _mm512_broadcast_i32x4(v128i_7);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m128 v128f_35 = _mm_mul_ps(v128f_26, v128f_13);
    __m128i v128i_36 = _mm_sub_epi64(v128i_28, v128i_7);
    __m256d v256d_37 = _mm256_add_pd(v256d_22, v256d_22);
    __m256h v256h_38 = g_v256h[10];
    __m512 v512f_39 = _mm512_cvtxph_ps(v256h_38);
    __m512i v512i_40 = _mm512_slli_epi32(v512i_15, 3);
    }
    g_v512d[9] = p2;
    g_v512i[10] = v512i_24;
    g_v256f[9] = v256f_17;
    g_v128d[7] = v128d_5;
    g_v128h[11] = p0;
    return v512f_14;
}

__m256i torture_36(__m128h p0, __m256 p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_min_ps(v512f_1, v512f_1);
    __m512i v512i_3 = g_v512i[10];
    __mmask16 m16_4 = _mm512_test_epi32_mask(v512i_3, v512i_3);
    __m512 v512f_5 = _mm512_rcp14_ps(v512f_2);
    __m512i v512i_6 = _mm512_set1_epi32(0x55);
    __m512 v512f_7 = _mm512_mask_blend_ps(m16_4, v512f_1, v512f_1);
    __m512i v512i_8 = _mm512_permutexvar_epi32(v512i_3, v512i_3);
    __m512 v512f_9 = _mm512_getexp_ps(v512f_1);
    __m512 v512f_10 = _mm512_maskz_mul_ps(m16_4, v512f_2, v512f_9);
    __m256i v256i_11 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_12 = _mm256_add_epi8(v256i_11, v256i_11);
    __mmask16 m16_13 = _kand_mask16(m16_4, m16_4);
    __m256d v256d_14 = g_v256d[6];
    __m256d v256d_15 = _mm256_unpackhi_pd(v256d_14, v256d_14);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_4) & 7); i_16++) {
    __m512 v512f_17 = _mm512_sqrt_ps(v512f_10);
    __m512h v512h_18 = g_v512h[5];
    __m512h v512h_19 = _mm512_add_ph(v512h_18, v512h_18);
    __m256i v256i_20 = _mm256_or_si256(v256i_12, v256i_12);
    __m512 v512f_21 = _mm512_mask_add_ps(v512f_7, m16_13, v512f_5, v512f_10);
    __m512i v512i_22 = _mm512_sub_epi16(v512i_8, v512i_8);
    }
    __mmask16 m16_23 = _mm512_testn_epi32_mask(v512i_3, v512i_6);
    __m512d v512d_24 = _mm512_loadu_pd(dbuf);
    __m512d v512d_25 = _mm512_min_pd(v512d_24, v512d_24);
    __m256d v256d_26 = _mm256_max_pd(v256d_15, v256d_14);
    __m256 v256f_27 = _mm256_xor_ps(p1, p1);
    g_v256d[0] = v256d_15;
    g_v128h[4] = p0;
    return v256i_12;
}

__mmask8 torture_37(__m256 p0, __m256 p1, __m128d p2, __m512 p3, __m256d p4) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_slli_epi32(v128i_1, 3);
    __m128 v128f_3 = g_v128f[9];
    __m128 v128f_4 = _mm_min_ps(v128f_3, v128f_3);
    __m256 v256f_5 = _mm256_xor_ps(p1, p1);
    __m256 v256f_6 = _mm256_unpacklo_ps(p1, p0);
    __m512h v512h_7 = g_v512h[2];
    __mmask32 m32_8 = _mm512_cmp_ph_mask(v512h_7, v512h_7, 1);
    __m128d v128d_9 = _mm_unpackhi_pd(p2, p2);
    __mmask16 m16_10 = (__mmask16)(g_m16[1] ^ 230);
    __m512 v512f_11 = _mm512_maskz_mul_ps(m16_10, p3, p3);
    __m128 v128f_12 = _mm512_extractf32x4_ps(p3, 2);
    __mmask32 m32_13 = _kxor_mask32(m32_8, m32_8);
    __m256i v256i_14 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_15 = _mm256_xor_si256(v256i_14, v256i_14);
    __m512i v512i_16 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_17 = _mm512_test_epi32_mask(v512i_16, v512i_16);
    __m128i v128i_18 = _mm_add_epi8(v128i_2, v128i_1);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m512i v512i_19 = _mm512_madd52lo_epu64(v512i_16, v512i_16, v512i_16);
    __m256i v256i_20 = _mm256_sub_epi8(v256i_15, v256i_15);
    __m512d v512d_21 = _mm512_loadu_pd(dbuf);
    __m512d v512d_22 = _mm512_mul_pd(v512d_21, v512d_21);
    __m128i v128i_23 = _mm_sub_epi64(v128i_18, v128i_18);
    __m256i v256i_24 = _mm256_sub_epi16(v256i_20, v256i_14);
    }
    __m256i v256i_25 = _mm256_shuffle_epi8(v256i_14, v256i_14);
    __m512 v512f_26 = _mm512_set1_ps(1.0f);
    __m256d v256d_27 = _mm256_unpackhi_pd(p4, p4);
    __m512 v512f_28 = _mm512_fmsub_ps(v512f_26, v512f_11, v512f_11);
    __m256d v256d_29 = _mm256_add_pd(p4, p4);
    __m512d v512d_30 = _mm512_loadu_pd(dbuf);
    __m512d v512d_31 = _mm512_sqrt_pd(v512d_30);
    __m512i v512i_32 = _mm512_cvtps_epi32(v512f_28);
    __m512 v512f_33 = _mm512_mask_add_ps(v512f_11, m16_17, v512f_28, v512f_26);
    __m512 v512f_34 = _mm512_mul_ps(v512f_26, v512f_11);
    __m512i v512i_35 = _mm512_sub_epi16(v512i_32, v512i_32);
    __mmask16 m16_36 = _kxor_mask16(m16_17, m16_17);
    __m512 v512f_37 = _mm512_scalef_ps(p3, v512f_34);
    g_v512f[2] = v512f_11;
    g_v256d[4] = v256d_27;
    g_v256i[4] = v256i_15;
    g_v128i[4] = v128i_2;
    g_m32[2] = m32_8;
    __mmask8 m8_38 = (__mmask8)(g_m8[6] ^ 83);
    return m8_38;
}

__m256h torture_38(__m512i p0, __m256d p1, __m512 p2, __m256d p3, __m512 p4) {
    __m512h v512h_1 = g_v512h[9];
    __m512h v512h_2 = _mm512_mul_ph(v512h_1, v512h_1);
    __m128d v128d_3 = g_v128d[8];
    __m128d v128d_4 = _mm_sqrt_pd(v128d_3);
    __m128i v128i_5 = g_v128i[0];
    __m128i v128i_6 = _mm_and_si128(v128i_5, v128i_5);
    __m512i v512i_7 = _mm512_sub_epi32(p0, p0);
    __mmask8 m8_8 = (__mmask8)(g_m8[6] ^ 23);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m512d v512d_10 = _mm512_mask_blend_pd(m8_8, v512d_9, v512d_9);
    __m128 v128f_11 = g_v128f[6];
    __m128 v128f_12 = _mm_min_ps(v128f_11, v128f_11);
    __m512i v512i_13 = _mm512_popcnt_epi64(v512i_7);
    __m256 v256f_14 = g_v256f[2];
    __m256 v256f_15 = _mm256_max_ps(v256f_14, v256f_14);
    __m128i v128i_16 = _mm_xor_si128(v128i_6, v128i_6);
    __m512i v512i_17 = _mm512_cvtepi8_epi32(v128i_16);
    __m512i v512i_18 = _mm512_add_epi8(v512i_17, v512i_17);
    __m512 v512f_19 = _mm512_scalef_ps(p4, p2);
    __m512 v512f_20 = _mm512_shuffle_ps(p4, p2, 0x1B);
    __m128 v128f_21 = _mm_div_ps(v128f_11, v128f_11);
    __m128i v128i_22 = _mm_sub_epi16(v128i_6, v128i_5);
    __m512 v512f_23 = _mm512_insertf32x8(v512f_19, v256f_14, 1);
    __m512i v512i_24 = _mm512_cvtps_epi32(v512f_19);
    __m512 v512f_25 = _mm512_set1_ps(1.0f);
    __m512d v512d_26 = _mm512_set1_pd(2.0);
    __m256d v256d_27 = _mm256_set1_pd(2.0);
    __mmask16 m16_28 = _mm512_kunpackb(m8_8, m8_8);
    __mmask16 m16_29 = _kshiftli_mask16(m16_28, 2);
    __m512 v512f_30 = _mm512_rcp14_ps(v512f_23);
    g_v128f[4] = v128f_11;
    g_v128i[2] = v128i_6;
    g_m8[7] = m8_8;
    g_m16[10] = m16_29;
    __m256h v256h_31 = g_v256h[11];
    return v256h_31;
}

__mmask16 torture_39(__m128d p0, __m256d p1, __m128d p2, __m512i p3, __m256h p4) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_fmsub_ps(v256f_1, v256f_1, v256f_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_add_epi16(v128i_3, v128i_3);
    __m128i v128i_5 = _mm_sub_epi32(v128i_3, v128i_4);
    __mmask16 m16_6 = _mm512_test_epi32_mask(p3, p3);
    __m256i v256i_7 = g_v256i[10];
    __m256i v256i_8 = _mm256_sub_epi16(v256i_7, v256i_7);
    __m256d v256d_9 = _mm256_set1_pd(2.0);
    __m512i v512i_10 = _mm512_abs_epi32(p3);
    __m512h v512h_11 = g_v512h[9];
    __m512h v512h_12 = _mm512_getexp_ph(v512h_11);
    __m128d v128d_13 = _mm_add_pd(p0, p0);
    __m512 v512f_14 = g_v512f[10];
    __m512 v512f_15 = _mm512_add_ps(v512f_14, v512f_14);
    __m512d v512d_16 = g_v512d[2];
    __m512d v512d_17 = _mm512_unpackhi_pd(v512d_16, v512d_16);
    __m512d v512d_18 = _mm512_sub_pd(v512d_16, v512d_17);
    __m128 v128f_19 = g_v128f[10];
    __m128 v128f_20 = _mm_add_ps(v128f_19, v128f_19);
    __m256 v256f_21 = _mm256_fnmadd_ps(v256f_1, v256f_2, v256f_1);
    __m128 v128f_22 = _mm_min_ps(v128f_19, v128f_20);
    __m128i v128i_23 = _mm_sub_epi8(v128i_4, v128i_5);
    __m512i v512i_24 = _mm512_add_epi16(p3, p3);
    __m512i v512i_25 = _mm512_unpacklo_epi32(v512i_10, v512i_24);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m128i v128i_26 = _mm_sub_epi16(v128i_3, v128i_5);
    __mmask16 m16_27 = _mm512_cmp_epi32_mask(v512i_24, v512i_24, 2);
    __m512i v512i_28 = _mm512_mask_blend_epi32(m16_6, v512i_25, p3);
    }
    __m256i v256i_29 = _mm256_sub_epi16(v256i_7, v256i_7);
    __m256i v256i_30 = _mm512_cvtepi64_epi32(v512i_24);
    __m512 v512f_31 = _mm512_fmadd_ps(v512f_14, v512f_15, v512f_14);
    __m256d v256d_32 = _mm256_mul_pd(v256d_9, v256d_9);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m512h v512h_33 = _mm512_fmadd_ph(v512h_11, v512h_11, v512h_11);
    __m256d v256d_34 = _mm256_max_pd(v256d_32, v256d_32);
    }
    g_v256i[6] = v256i_7;
    g_v128d[8] = p2;
    g_v128i[1] = v128i_3;
    g_v256h[6] = p4;
    return m16_6;
}

__m512h torture_40(__m128d p0, __m256i p1, __m512i p2, __m256 p3, __m128d p4) {
    __m256 v256f_1 = _mm256_fnmadd_ps(p3, p3, p3);
    __mmask64 m64_2 = _mm512_cmp_epi8_mask(p2, p2, 1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_abs_epi32(v128i_3);
    __m256d v256d_5 = g_v256d[1];
    __m256d v256d_6 = _mm256_max_pd(v256d_5, v256d_5);
    __m512d v512d_7 = g_v512d[3];
    __m512d v512d_8 = _mm512_div_pd(v512d_7, v512d_7);
    __m256d v256d_9 = _mm256_set1_pd(2.0);
    __mmask16 m16_10 = (__mmask16)(g_m16[6] ^ 116);
    __mmask16 m16_11 = _kor_mask16(m16_10, m16_10);
    __m128i v128i_12 = _mm_mullo_epi32(v128i_4, v128i_4);
    __m512 v512f_13 = _mm512_loadu_ps(fbuf);
    __m512 v512f_14 = _mm512_and_ps(v512f_13, v512f_13);
    __m512i v512i_15 = _mm512_or_si512(p2, p2);
    __m256 v256f_16 = _mm256_fnmadd_ps(v256f_1, p3, p3);
    __mmask16 m16_17 = _kor_mask16(m16_10, m16_10);
    __mmask16 m16_18 = _mm512_fpclass_ps_mask(v512f_14, 0x3);
    __m256 v256f_19 = _mm256_fmadd_ps(v256f_1, v256f_1, v256f_16);
    unsigned u32_20 = 48;
    __mmask16 m16_21 = _cvtu32_mask16(u32_20);
    __m256d v256d_22 = _mm256_mul_pd(v256d_5, v256d_9);
    __m128 v128f_23 = _mm_loadu_ps(fbuf);
    __m512 v512f_24 = _mm512_broadcast_f32x4(v128f_23);
    __m256 v256f_25 = _mm256_div_ps(p3, p3);
    __m128 v128f_26 = _mm_div_ps(v128f_23, v128f_23);
    __m128i v128i_27 = _mm_abs_epi32(v128i_4);
    __mmask16 m16_28 = _cvtu32_mask16(u32_20);
    __m256d v256d_29 = _mm256_add_pd(v256d_9, v256d_5);
    __m512d v512d_30 = _mm512_sqrt_pd(v512d_7);
    if (((unsigned long long)m16_18) & 1ULL) {
    __m128d v128d_31 = _mm_mul_pd(p4, p0);
    __m512 v512f_32 = _mm512_sqrt_ps(v512f_24);
    __m128i v128i_33 = _mm_sub_epi16(v128i_12, v128i_12);
    __mmask16 m16_34 = _mm512_cmp_epi32_mask(v512i_15, v512i_15, 2);
    __m128 v128f_35 = _mm_sqrt_ps(v128f_23);
    }
    g_v512f[4] = v512f_13;
    g_v512i[7] = v512i_15;
    g_v256i[0] = p1;
    g_v128f[5] = v128f_26;
    g_m16[11] = m16_21;
    g_m64[3] = m64_2;
    __m512h v512h_36 = g_v512h[11];
    return v512h_36;
}

__m128i torture_41(__m128d p0, __m128 p1, __m512 p2) {
    __m256i v256i_1 = g_v256i[2];
    __m256i v256i_2 = _mm256_slli_epi32(v256i_1, 3);
    __m128d v128d_3 = _mm_max_pd(p0, p0);
    __m512i v512i_4 = g_v512i[7];
    __m512i v512i_5 = _mm512_ternarylogic_epi32(v512i_4, v512i_4, v512i_4, 0x96);
    __m512i v512i_6 = _mm512_sub_epi32(v512i_5, v512i_5);
    __m128i v128i_7 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_8 = _mm_and_si128(v128i_7, v128i_7);
    __mmask8 m8_9 = (__mmask8)(g_m8[6] ^ 7);
    __m512i v512i_10 = _mm512_broadcastmb_epi64(m8_9);
    __m256i v256i_11 = _mm256_add_epi64(v256i_1, v256i_2);
    __m256i v256i_12 = _mm256_add_epi32(v256i_2, v256i_11);
    __m512 v512f_13 = _mm512_fmsub_ps(p2, p2, p2);
    __m256i v256i_14 = _mm256_unpacklo_epi32(v256i_1, v256i_2);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m8_9) & 7); i_15++) {
    __m512d v512d_16 = g_v512d[7];
    __m512d v512d_17 = _mm512_fmadd_pd(v512d_16, v512d_16, v512d_16);
    __m128 v128f_18 = _mm_fmsub_ps(p1, p1, p1);
    __m256 v256f_19 = _mm256_loadu_ps(fbuf);
    __m512d v512d_20 = _mm512_cvtps_pd(v256f_19);
    }
    __m128d v128d_21 = _mm_set1_pd(2.0);
    __m512 v512f_22 = _mm512_rcp14_ps(p2);
    __m512i v512i_23 = _mm512_rol_epi32(v512i_5, 5);
    __m512 v512f_24 = _mm512_min_ps(v512f_22, p2);
    __m256 v256f_25 = g_v256f[11];
    __m256 v256f_26 = _mm256_sqrt_ps(v256f_25);
    __m128 v128f_27 = _mm_fmsub_ps(p1, p1, p1);
    __mmask16 m16_28 = _mm512_cmp_epi32_mask(v512i_5, v512i_10, 2);
    __mmask32 m32_29 = (__mmask32)(g_m32[0] ^ 20);
    __mmask32 m32_30 = _kxor_mask32(m32_29, m32_29);
    g_v512i[4] = v512i_6;
    g_v256f[7] = v256f_26;
    g_v256i[9] = v256i_11;
    g_v128f[2] = v128f_27;
    g_m32[2] = m32_30;
    return v128i_8;
}

__m256d torture_42(__m512i p0, __m256 p1, __m256i p2, __m512d p3, __m256h p4) {
    __m512 v512f_1 = g_v512f[5];
    __m512 v512f_2 = _mm512_roundscale_ps(v512f_1, 0x10);
    __m256 v256f_3 = _mm256_add_ps(p1, p1);
    __m256i v256i_4 = _mm256_xor_si256(p2, p2);
    __m256 v256f_5 = _mm512_cvtpd_ps(p3);
    __m512h v512h_6 = g_v512h[7];
    __m512h v512h_7 = _mm512_sqrt_ph(v512h_6);
    __m128 v128f_8 = _mm_loadu_ps(fbuf);
    __m128 v128f_9 = _mm_max_ps(v128f_8, v128f_8);
    __m128i v128i_10 = g_v128i[7];
    __m128i v128i_11 = _mm_shuffle_epi8(v128i_10, v128i_10);
    __m128i v128i_12 = _mm_add_epi32(v128i_11, v128i_11);
    __m512i v512i_13 = _mm512_rol_epi32(p0, 5);
    __m256i v256i_14 = _mm256_abs_epi32(p2);
    __m256 v256f_15 = _mm256_sqrt_ps(v256f_5);
    __m128 v128f_16 = _mm_xor_ps(v128f_8, v128f_8);
    __mmask16 m16_17 = (__mmask16)(g_m16[5] ^ 37);
    __m512 v512f_18 = _mm512_mask_blend_ps(m16_17, v512f_2, v512f_2);
    __m512i v512i_19 = _mm512_shuffle_i32x4(p0, v512i_13, 0x1B);
    __m512i v512i_20 = _mm512_abs_epi32(p0);
    __m256 v256f_21 = _mm256_max_ps(v256f_15, v256f_5);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m128i v128i_22 = _mm_add_epi8(v128i_10, v128i_10);
    __m512 v512f_23 = _mm512_fmadd_ps(v512f_18, v512f_2, v512f_2);
    }
    __m256d v256d_24 = g_v256d[4];
    __m256d v256d_25 = _mm256_fmadd_pd(v256d_24, v256d_24, v256d_24);
    __m128i v128i_26 = _mm_sub_epi32(v128i_11, v128i_12);
    __mmask16 m16_27 = _knot_mask16(m16_17);
    __m512i v512i_28 = _mm512_sub_epi16(v512i_19, p0);
    __m256i v256i_29 = _mm256_sub_epi16(p2, v256i_14);
    __m128d v128d_30 = g_v128d[9];
    __m512d v512d_31 = _mm512_insertf64x2(p3, v128d_30, 3);
    __m512i v512i_32 = _mm512_broadcastmw_epi32(m16_27);
    __m512 v512f_33 = _mm512_min_ps(v512f_2, v512f_2);
    if (((unsigned long long)m16_27) & 1ULL) {
    __m128 v128f_34 = _mm_sqrt_ps(v128f_8);
    __m128 v128f_35 = _mm_set1_ps(1.0f);
    __m512d v512d_36 = _mm512_add_pd(p3, v512d_31);
    __m128i v128i_37 = _mm_sub_epi32(v128i_12, v128i_10);
    }
    g_v512d[3] = p3;
    g_v256f[10] = v256f_5;
    g_v256h[0] = p4;
    g_m16[6] = m16_27;
    return v256d_25;
}

__m256h torture_43(__m256 p0, __m512d p1, __m256d p2) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_shuffle_epi8(v128i_1, v128i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_lzcnt_epi32(v512i_3);
    __m128 v128f_5 = _mm_loadu_ps(fbuf);
    __m512 v512f_6 = _mm512_broadcast_f32x4(v128f_5);
    __m512i v512i_7 = _mm512_lzcnt_epi32(v512i_3);
    __mmask16 m16_8 = _mm512_test_epi32_mask(v512i_7, v512i_3);
    __m128d v128d_9 = g_v128d[1];
    __m128d v128d_10 = _mm_fmadd_pd(v128d_9, v128d_9, v128d_9);
    __m512i v512i_11 = _mm512_add_epi64(v512i_4, v512i_7);
    __m512 v512f_12 = _mm512_rcp14_ps(v512f_6);
    __m512i v512i_13 = _mm512_unpacklo_epi32(v512i_3, v512i_3);
    __m512i v512i_14 = _mm512_add_epi8(v512i_3, v512i_7);
    __m512i v512i_15 = _mm512_mullo_epi32(v512i_14, v512i_13);
    __m512 v512f_16 = _mm512_rsqrt14_ps(v512f_12);
    __m128d v128d_17 = _mm_fmadd_pd(v128d_10, v128d_9, v128d_10);
    __m512h v512h_18 = g_v512h[7];
    __m512h v512h_19 = _mm512_fmsub_ph(v512h_18, v512h_18, v512h_18);
    __m128i v128i_20 = _mm_add_epi16(v128i_2, v128i_1);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m256i v256i_21 = _mm256_set1_epi32(0x55);
    __m256i v256i_22 = _mm256_sub_epi64(v256i_21, v256i_21);
    }
    __m512i v512i_23 = _mm512_add_epi64(v512i_15, v512i_4);
    __mmask16 m16_24 = _kand_mask16(m16_8, m16_8);
    __m256 v256f_25 = _mm256_set1_ps(1.0f);
    __m128 v128f_26 = _mm_add_ps(v128f_5, v128f_5);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m128d v128d_27 = _mm_mul_pd(v128d_9, v128d_17);
    __m128d v128d_28 = _mm_sub_pd(v128d_9, v128d_9);
    __m512i v512i_29 = _mm512_xor_si512(v512i_11, v512i_23);
    __m512h v512h_30 = _mm512_fmul_pch(v512h_19, v512h_19);
    __m512i v512i_31 = _mm512_and_si512(v512i_13, v512i_7);
    __m512i v512i_32 = _mm512_dpbusd_epi32(v512i_31, v512i_7, v512i_29);
    }
    g_v128d[6] = v128d_17;
    __m256h v256h_33 = g_v256h[5];
    return v256h_33;
}

__m256 torture_44(__m128 p0, __m256d p1, __m512i p2) {
    __m128i v128i_1 = g_v128i[8];
    __m128i v128i_2 = _mm_or_si128(v128i_1, v128i_1);
    __m256d v256d_3 = _mm256_sqrt_pd(p1);
    __mmask16 m16_4 = (__mmask16)(g_m16[11] ^ 249);
    __mmask16 m16_5 = _kor_mask16(m16_4, m16_4);
    __mmask16 m16_6 = _mm512_testn_epi32_mask(p2, p2);
    __m128i v128i_7 = _mm_sub_epi32(v128i_2, v128i_2);
    __m512h v512h_8 = _mm512_cvtepi16_ph(p2);
    __m128d v128d_9 = _mm_set1_pd(2.0);
    __m128 v128f_10 = _mm_and_ps(p0, p0);
    __m128i v128i_11 = _mm_shuffle_epi8(v128i_2, v128i_1);
    __m512i v512i_12 = _mm512_add_epi16(p2, p2);
    __m512d v512d_13 = g_v512d[11];
    __m512d v512d_14 = _mm512_max_pd(v512d_13, v512d_13);
    __m512i v512i_15 = _mm512_mullo_epi32(p2, p2);
    __m128i v128i_16 = _mm_mullo_epi32(v128i_1, v128i_2);
    __m128i v128i_17 = _mm512_cvtepi32_epi8(v512i_12);
    __mmask8 m8_18 = (__mmask8)(g_m8[7] ^ 232);
    __mmask8 m8_19 = _kand_mask8(m8_18, m8_18);
    __m256 v256f_20 = _mm256_loadu_ps(fbuf);
    __m256 v256f_21 = _mm256_min_ps(v256f_20, v256f_20);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_6) & 7); i_22++) {
    __m512 v512f_23 = _mm512_broadcast_f32x4(v128f_10);
    __m256d v256d_24 = _mm256_max_pd(p1, v256d_3);
    __m128i v128i_25 = _mm_xor_si128(v128i_2, v128i_2);
    __m256 v256f_26 = _mm256_fmsub_ps(v256f_20, v256f_21, v256f_20);
    __m256d v256d_27 = _mm256_unpackhi_pd(p1, v256d_3);
    }
    __mmask16 m16_28 = _knot_mask16(m16_4);
    __m128i v128i_29 = _mm_sub_epi16(v128i_11, v128i_17);
    __m512i v512i_30 = _mm512_add_epi32(v512i_12, v512i_15);
    __m128 v128f_31 = _mm_max_ps(p0, p0);
    __m512d v512d_32 = _mm512_mask_blend_pd(m8_18, v512d_14, v512d_13);
    __m256i v256i_33 = g_v256i[2];
    __m256i v256i_34 = _mm256_sub_epi8(v256i_33, v256i_33);
    __m128 v128f_35 = _mm_sub_ps(v128f_10, p0);
    __m512i v512i_36 = _mm512_sllv_epi32(v512i_12, v512i_15);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m128i v128i_37 = _mm_mullo_epi32(v128i_7, v128i_2);
    __m512i v512i_38 = _mm512_shuffle_i32x4(p2, p2, 0x1B);
    }
    g_v512d[3] = v512d_32;
    g_v256f[7] = v256f_21;
    g_v256d[9] = v256d_3;
    g_v128d[7] = v128d_9;
    g_v128i[5] = v128i_11;
    g_m8[6] = m8_18;
    g_m16[0] = m16_6;
    return v256f_21;
}

__m256i torture_45(__m256i p0, __m512i p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_fnmadd_ps(v512f_1, v512f_1, v512f_1);
    __mmask16 m16_3 = _mm512_movepi32_mask(p1);
    __m128i v128i_4 = g_v128i[9];
    __m128i v128i_5 = _mm_unpacklo_epi32(v128i_4, v128i_4);
    __m128 v128f_6 = g_v128f[3];
    __m128 v128f_7 = _mm_add_ps(v128f_6, v128f_6);
    __m128 v128f_8 = _mm_div_ps(v128f_6, v128f_7);
    __m512i v512i_9 = _mm512_sllv_epi32(p1, p1);
    __m128i v128i_10 = _mm_and_si128(v128i_5, v128i_5);
    __m128 v128f_11 = _mm_mul_ps(v128f_8, v128f_8);
    __m256d v256d_12 = g_v256d[1];
    __m256d v256d_13 = _mm256_mul_pd(v256d_12, v256d_12);
    __m512d v512d_14 = g_v512d[8];
    __m512d v512d_15 = _mm512_unpackhi_pd(v512d_14, v512d_14);
    __m512d v512d_16 = _mm512_fmadd_pd(v512d_14, v512d_15, v512d_14);
    __m512 v512f_17 = _mm512_rcp14_ps(v512f_2);
    unsigned u32_18 = _cvtmask16_u32(m16_3);
    __mmask8 m8_19 = (__mmask8)(g_m8[1] ^ 231);
    __m512i v512i_20 = _mm512_broadcastmb_epi64(m8_19);
    __m512i v512i_21 = _mm512_set1_epi32(0x55);
    g_v256d[3] = v256d_13;
    g_v256i[8] = p0;
    g_v128i[10] = v128i_4;
    return p0;
}

__m256d torture_46(__m256i p0, __m256h p1, __m256 p2, __m512h p3) {
    __m512h v512h_1 = _mm512_getexp_ph(p3);
    __m256i v256i_2 = _mm256_add_epi8(p0, p0);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_4 = _mm512_cvtepi8_epi32(v128i_3);
    __m128 v128f_5 = _mm_loadu_ps(fbuf);
    __m128 v128f_6 = _mm_sqrt_ps(v128f_5);
    __m256 v256f_7 = _mm256_set1_ps(1.0f);
    __m512 v512f_8 = _mm512_loadu_ps(fbuf);
    __m512 v512f_9 = _mm512_roundscale_ps(v512f_8, 0x10);
    __mmask16 m16_10 = _mm512_test_epi32_mask(v512i_4, v512i_4);
    __m128i v128i_11 = _mm_sub_epi16(v128i_3, v128i_3);
    __m256d v256d_12 = g_v256d[9];
    __m256d v256d_13 = _mm256_unpackhi_pd(v256d_12, v256d_12);
    __m256i v256i_14 = _mm256_sub_epi8(p0, v256i_2);
    __m256d v256d_15 = _mm256_add_pd(v256d_13, v256d_13);
    __m256 v256f_16 = _mm256_fmsub_ps(p2, v256f_7, p2);
    __m128d v128d_17 = g_v128d[2];
    __m128d v128d_18 = _mm_sqrt_pd(v128d_17);
    __m512i v512i_19 = _mm512_permutex2var_epi32(v512i_4, v512i_4, v512i_4);
    __m128 v128f_20 = _mm_unpacklo_ps(v128f_5, v128f_6);
    __m128 v128f_21 = _mm_mul_ps(v128f_20, v128f_20);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_10) & 7); i_22++) {
    __mmask64 m64_23 = (__mmask64)(g_m64[10] ^ 134);
    __mmask64 m64_24 = _kor_mask64(m64_23, m64_23);
    __mmask8 m8_25 = (__mmask8)(g_m8[3] ^ 125);
    __mmask8 m8_26 = _kand_mask8(m8_25, m8_25);
    }
    g_v512i[0] = v512i_19;
    g_v256i[6] = v256i_14;
    g_v128f[9] = v128f_21;
    g_v128d[1] = v128d_18;
    g_v256h[5] = p1;
    return v256d_15;
}

__m256h torture_47(__m128h p0, __m256h p1, __m256d p2, __m256 p3) {
    __m512i v512i_1 = g_v512i[10];
    __m512i v512i_2 = _mm512_slli_epi32(v512i_1, 3);
    __m128d v128d_3 = g_v128d[9];
    __m128d v128d_4 = _mm_mul_pd(v128d_3, v128d_3);
    __m256d v256d_5 = _mm256_fmadd_pd(p2, p2, p2);
    __m256 v256f_6 = _mm256_min_ps(p3, p3);
    __m128d v128d_7 = _mm_sqrt_pd(v128d_3);
    __m512i v512i_8 = _mm512_permutex2var_epi32(v512i_2, v512i_2, v512i_2);
    __m512h v512h_9 = g_v512h[11];
    __m512h v512h_10 = _mm512_rcp_ph(v512h_9);
    __m128i v128i_11 = g_v128i[5];
    __m128i v128i_12 = _mm_add_epi8(v128i_11, v128i_11);
    __m512d v512d_13 = _mm512_loadu_pd(dbuf);
    __m512d v512d_14 = _mm512_min_pd(v512d_13, v512d_13);
    __m512i v512i_15 = _mm512_and_si512(v512i_2, v512i_8);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_fnmadd_ps(v128f_16, v128f_16, v128f_16);
    unsigned u32_18 = 34;
    __mmask16 m16_19 = _cvtu32_mask16(u32_18);
    __m512d v512d_20 = _mm512_min_pd(v512d_14, v512d_14);
    __m512 v512f_21 = _mm512_loadu_ps(fbuf);
    __m256h v256h_22 = _mm512_cvtxps_ph(v512f_21);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_19) & 7); i_23++) {
    __m128d v128d_24 = _mm_unpackhi_pd(v128d_3, v128d_4);
    __mmask16 m16_25 = _mm512_cmp_epu32_mask(v512i_15, v512i_8, 4);
    __m128i v128i_26 = _mm_add_epi16(v128i_11, v128i_11);
    __m512 v512f_27 = _mm512_mask3_fmadd_ps(v512f_21, v512f_21, v512f_21, m16_25);
    }
    __m256d v256d_28 = _mm256_add_pd(p2, v256d_5);
    __m128 v128f_29 = _mm_max_ps(v128f_17, v128f_16);
    __mmask16 m16_30 = _mm512_cmp_epi32_mask(v512i_2, v512i_15, 2);
    __m512i v512i_31 = _mm512_rolv_epi32(v512i_8, v512i_8);
    __m512h v512h_32 = _mm512_getexp_ph(v512h_10);
    __m256i v256i_33 = g_v256i[7];
    __m256i v256i_34 = _mm256_xor_si256(v256i_33, v256i_33);
    __m512 v512f_35 = _mm512_shuffle_ps(v512f_21, v512f_21, 0x1B);
    __mmask16 m16_36 = _knot_mask16(m16_30);
    __mmask8 m8_37 = (__mmask8)(g_m8[3] ^ 110);
    __m512i v512i_38 = _mm512_broadcastmb_epi64(m8_37);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m128d v128d_39 = _mm_fmadd_pd(v128d_4, v128d_7, v128d_7);
    __m256i v256i_40 = _mm256_sub_epi16(v256i_34, v256i_34);
    }
    g_v512d[1] = v512d_20;
    g_v256f[5] = p3;
    g_v256i[0] = v256i_33;
    g_v128f[10] = v128f_17;
    g_v128d[0] = v128d_4;
    g_v256h[9] = v256h_22;
    return v256h_22;
}

__m512d torture_48(__m128i p0, __m256h p1, __m128i p2) {
    __m512 v512f_1 = g_v512f[6];
    __m512 v512f_2 = _mm512_max_ps(v512f_1, v512f_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_or_si512(v512i_3, v512i_3);
    __m512 v512f_5 = _mm512_rcp14_ps(v512f_1);
    __m512i v512i_6 = _mm512_cvtps_epi32(v512f_2);
    __m256i v256i_7 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_8 = _mm256_slli_epi32(v256i_7, 3);
    __m128i v128i_9 = _mm_add_epi32(p2, p0);
    __m128 v128f_10 = g_v128f[4];
    __m128 v128f_11 = _mm_xor_ps(v128f_10, v128f_10);
    __m512h v512h_12 = g_v512h[10];
    __m512h v512h_13 = _mm512_fmul_pch(v512h_12, v512h_12);
    __m256i v256i_14 = _mm256_xor_si256(v256i_7, v256i_8);
    __m512 v512f_15 = _mm512_div_ps(v512f_2, v512f_1);
    int s32_16 = 12;
    for (int i_17 = 0; i_17 < (int)((s32_16) & 7); i_17++) {
    __m128d v128d_18 = g_v128d[8];
    __m128d v128d_19 = _mm_sqrt_pd(v128d_18);
    __m128i v128i_20 = _mm_sub_epi64(v128i_9, p2);
    }
    __mmask16 m16_21 = (__mmask16)(g_m16[0] ^ 37);
    __mmask32 m32_22 = _mm512_kunpackw(m16_21, m16_21);
    __m128i v128i_23 = _mm_shuffle_epi8(v128i_9, p2);
    __m512i v512i_24 = _mm512_shuffle_epi8(v512i_6, v512i_3);
    __m256 v256f_25 = _mm256_loadu_ps(fbuf);
    __m256 v256f_26 = _mm256_sqrt_ps(v256f_25);
    __m512 v512f_27 = _mm512_mask_blend_ps(m16_21, v512f_15, v512f_15);
    __m512i v512i_28 = _mm512_sllv_epi32(v512i_4, v512i_6);
    __m128d v128d_29 = g_v128d[7];
    __m128d v128d_30 = _mm_mul_pd(v128d_29, v128d_29);
    __m512 v512f_31 = _mm512_cvtepi32_ps(v512i_24);
    __m512 v512f_32 = _mm512_insertf32x8(v512f_5, v256f_26, 1);
    __mmask32 m32_33 = _mm512_kunpackw(m16_21, m16_21);
    __m256 v256f_34 = _mm256_unpacklo_ps(v256f_25, v256f_25);
    __m512d v512d_35 = _mm512_cvtps_pd(v256f_34);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_21) & 7); i_36++) {
    __m512d v512d_37 = _mm512_sqrt_pd(v512d_35);
    unsigned u32_38 = 30;
    __mmask16 m16_39 = _cvtu32_mask16(u32_38);
    }
    g_v256f[2] = v256f_34;
    g_v256i[4] = v256i_7;
    g_v128i[9] = p2;
    g_v256h[2] = p1;
    g_m16[3] = m16_21;
    return v512d_35;
}

__mmask16 torture_49(__m128 p0, __m512h p1, __m128 p2) {
    __m256 v256f_1 = g_v256f[0];
    __m256 v256f_2 = _mm256_add_ps(v256f_1, v256f_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_add_epi8(v512i_3, v512i_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512 v512f_6 = _mm512_getexp_ps(v512f_5);
    __m128 v128f_7 = _mm_fmadd_ps(p0, p0, p0);
    __m512i v512i_8 = _mm512_permutex2var_epi32(v512i_3, v512i_4, v512i_3);
    __m128i v128i_9 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_10 = _mm_sub_epi64(v128i_9, v128i_9);
    __m256i v256i_11 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_12 = _mm256_add_epi8(v256i_11, v256i_11);
    __m512i v512i_13 = _mm512_dpwssd_epi32(v512i_8, v512i_3, v512i_8);
    __mmask16 m16_14 = (__mmask16)(g_m16[11] ^ 183);
    __m512 v512f_15 = _mm512_mask3_fmadd_ps(v512f_6, v512f_5, v512f_6, m16_14);
    __m256i v256i_16 = _mm256_sub_epi64(v256i_11, v256i_12);
    __m512 v512f_17 = _mm512_mask_blend_ps(m16_14, v512f_6, v512f_5);
    __m128i v128i_18 = _mm_mullo_epi32(v128i_10, v128i_9);
    __m256d v256d_19 = g_v256d[3];
    __m256d v256d_20 = _mm256_mul_pd(v256d_19, v256d_19);
    __m128d v128d_21 = g_v128d[1];
    __m128d v128d_22 = _mm_unpackhi_pd(v128d_21, v128d_21);
    __mmask32 m32_23 = (__mmask32)(g_m32[6] ^ 239);
    __mmask32 m32_24 = _kxor_mask32(m32_23, m32_23);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512d v512d_25 = _mm512_loadu_pd(dbuf);
    __m512d v512d_26 = _mm512_sub_pd(v512d_25, v512d_25);
    __m128i v128i_27 = _mm_add_epi64(v128i_18, v128i_18);
    __m512i v512i_28 = _mm512_permutexvar_epi32(v512i_13, v512i_8);
    }
    g_v512f[10] = v512f_5;
    g_v256f[2] = v256f_2;
    g_v256i[3] = v256i_16;
    g_v128d[6] = v128d_21;
    g_v128i[10] = v128i_18;
    g_m16[7] = m16_14;
    g_m32[4] = m32_24;
    return m16_14;
}

__m256d torture_50(__m256i p0, __m256h p1, __m256h p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512h v512h_2 = _mm512_cvtepi16_ph(v512i_1);
    __m512d v512d_3 = g_v512d[6];
    __m256d v256d_4 = _mm512_extractf64x4_pd(v512d_3, 1);
    __m512i v512i_5 = _mm512_cvtph_epi16(v512h_2);
    __m128 v128f_6 = g_v128f[4];
    __m128 v128f_7 = _mm_sqrt_ps(v128f_6);
    __m256d v256d_8 = _mm256_sub_pd(v256d_4, v256d_4);
    __mmask16 m16_9 = (__mmask16)(g_m16[5] ^ 10);
    __mmask16 m16_10 = _knot_mask16(m16_9);
    __m256d v256d_11 = _mm512_extractf64x4_pd(v512d_3, 1);
    __m512 v512f_12 = g_v512f[2];
    __m512 v512f_13 = _mm512_maskz_mul_ps(m16_9, v512f_12, v512f_12);
    __m128 v128f_14 = _mm_min_ps(v128f_6, v128f_7);
    __mmask8 m8_15 = (__mmask8)(g_m8[6] ^ 65);
    __mmask8 m8_16 = _kand_mask8(m8_15, m8_15);
    __m128d v128d_17 = g_v128d[5];
    __m128d v128d_18 = _mm_fmadd_pd(v128d_17, v128d_17, v128d_17);
    __m512d v512d_19 = _mm512_div_pd(v512d_3, v512d_3);
    __m256 v256f_20 = _mm256_loadu_ps(fbuf);
    __m256 v256f_21 = _mm256_and_ps(v256f_20, v256f_20);
    __m512h v512h_22 = _mm512_cvtepi16_ph(v512i_5);
    __m256 v256f_23 = _mm256_xor_ps(v256f_20, v256f_20);
    __m512i v512i_24 = _mm512_broadcastmb_epi64(m8_16);
    __mmask16 m16_25 = _mm512_kunpackb(m8_15, m8_16);
    __mmask16 m16_26 = _mm512_kunpackb(m8_16, m8_15);
    __m512d v512d_27 = _mm512_set1_pd(2.0);
    __mmask8 m8_28 = _kand_mask8(m8_15, m8_16);
    __m512d v512d_29 = _mm512_add_pd(v512d_27, v512d_19);
    __m512i v512i_30 = _mm512_xor_si512(v512i_5, v512i_5);
    __m256 v256f_31 = _mm256_fnmadd_ps(v256f_23, v256f_20, v256f_23);
    __m512d v512d_32 = _mm512_sub_pd(v512d_27, v512d_19);
    __m128i v128i_33 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_34 = _mm_sllv_epi32(v128i_33, v128i_33);
    __m512 v512f_35 = _mm512_getexp_ps(v512f_13);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_26) & 7); i_36++) {
    __m256 v256f_37 = _mm256_fmadd_ps(v256f_20, v256f_23, v256f_31);
    __mmask8 m8_38 = _mm512_cmp_pd_mask(v512d_3, v512d_19, _CMP_GE_OQ);
    __m512h v512h_39 = _mm512_fmsub_ph(v512h_2, v512h_22, v512h_22);
    }
    g_v512d[4] = v512d_27;
    g_v512h[7] = v512h_22;
    g_v256h[4] = p1;
    g_m8[7] = m8_15;
    g_m16[5] = m16_26;
    return v256d_11;
}

__m128 torture_51(__m256 p0, __m128 p1, __m256h p2) {
    __m256d v256d_1 = g_v256d[4];
    __m256d v256d_2 = _mm256_min_pd(v256d_1, v256d_1);
    __m512i v512i_3 = g_v512i[5];
    __m512i v512i_4 = _mm512_rolv_epi32(v512i_3, v512i_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512 v512f_6 = _mm512_min_ps(v512f_5, v512f_5);
    __m512i v512i_7 = _mm512_or_si512(v512i_3, v512i_4);
    __m128 v128f_8 = _mm_shuffle_ps(p1, p1, 0x1B);
    __m512 v512f_9 = _mm512_rsqrt14_ps(v512f_6);
    __m256 v256f_10 = _mm256_max_ps(p0, p0);
    __m256d v256d_11 = _mm256_max_pd(v256d_2, v256d_2);
    __m128 v128f_12 = _mm_fmadd_ps(v128f_8, v128f_8, p1);
    __m512 v512f_13 = _mm512_shuffle_f32x4(v512f_6, v512f_5, 0x4E);
    __m512i v512i_14 = _mm512_xor_si512(v512i_3, v512i_4);
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_16 = _mm_xor_si128(v128i_15, v128i_15);
    __m512 v512f_17 = _mm512_fnmadd_ps(v512f_5, v512f_13, v512f_13);
    g_v512f[2] = v512f_13;
    g_v512i[4] = v512i_3;
    g_v256f[3] = p0;
    g_v256h[7] = p2;
    return p1;
}

__m128i torture_52(__m256d p0, __m512h p1, __m128i p2, __m256i p3) {
    __m256i v256i_1 = _mm256_or_si256(p3, p3);
    __m128i v128i_2 = _mm_and_si128(p2, p2);
    __m256i v256i_3 = _mm256_unpacklo_epi32(v256i_1, p3);
    __m512d v512d_4 = _mm512_loadu_pd(dbuf);
    __m512d v512d_5 = _mm512_mul_pd(v512d_4, v512d_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_and_ps(v512f_6, v512f_6);
    __m128 v128f_8 = g_v128f[6];
    __m128 v128f_9 = _mm_sqrt_ps(v128f_8);
    __m256 v256f_10 = g_v256f[0];
    __m256 v256f_11 = _mm256_mul_ps(v256f_10, v256f_10);
    __m256 v256f_12 = _mm256_set1_ps(1.0f);
    int s32_13 = 30;
    for (int i_14 = 0; i_14 < (int)((s32_13) & 7); i_14++) {
    __m512i v512i_15 = _mm512_loadu_si512((const void*)ibuf);
    __m256i v256i_16 = _mm512_cvtepi64_epi32(v512i_15);
    __m128d v128d_17 = g_v128d[1];
    __m128d v128d_18 = _mm_sub_pd(v128d_17, v128d_17);
    __mmask32 m32_19 = (__mmask32)(g_m32[1] ^ 157);
    __mmask32 m32_20 = _kxor_mask32(m32_19, m32_19);
    }
    __m128d v128d_21 = g_v128d[7];
    __m512d v512d_22 = _mm512_insertf64x2(v512d_5, v128d_21, 3);
    __m512h v512h_23 = _mm512_mul_ph(p1, p1);
    __m512 v512f_24 = _mm512_fmsub_ps(v512f_7, v512f_6, v512f_6);
    __m512 v512f_25 = _mm512_xor_ps(v512f_24, v512f_7);
    __m128d v128d_26 = _mm_max_pd(v128d_21, v128d_21);
    __m512i v512i_27 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_28 = _mm512_dpbusd_epi32(v512i_27, v512i_27, v512i_27);
    if ((s32_13) & 1ULL) {
    __mmask16 m16_29 = (__mmask16)(g_m16[10] ^ 96);
    __m512i v512i_30 = _mm512_broadcastmw_epi32(m16_29);
    __m512h v512h_31 = _mm512_rcp_ph(v512h_23);
    __mmask16 m16_32 = _mm512_movepi32_mask(v512i_30);
    __mmask8 m8_33 = (__mmask8)(g_m8[3] ^ 17);
    __mmask8 m8_34 = _kand_mask8(m8_33, m8_33);
    __m256i v256i_35 = _mm256_mullo_epi32(v256i_3, p3);
    }
    g_v256d[9] = p0;
    g_v256i[1] = v256i_3;
    g_v128d[11] = v128d_21;
    return p2;
}

__mmask32 torture_53(__m512 p0, __m256 p1, __m512 p2) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m512d v512d_2 = _mm512_max_pd(v512d_1, v512d_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_add_epi8(v128i_3, v128i_3);
    __m512 v512f_5 = _mm512_sub_ps(p2, p0);
    __m256d v256d_6 = g_v256d[0];
    __m256d v256d_7 = _mm256_max_pd(v256d_6, v256d_6);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_9 = _mm512_sub_epi64(v512i_8, v512i_8);
    __m128 v128f_10 = g_v128f[1];
    __m128 v128f_11 = _mm_xor_ps(v128f_10, v128f_10);
    __m128i v128i_12 = _mm_abs_epi32(v128i_3);
    __m128i v128i_13 = _mm_or_si128(v128i_3, v128i_4);
    int s32_14 = 56;
    for (int i_15 = 0; i_15 < (int)((s32_14) & 7); i_15++) {
    __m512 v512f_16 = _mm512_min_ps(p2, v512f_5);
    __m256i v256i_17 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_18 = _mm256_and_si256(v256i_17, v256i_17);
    __m128d v128d_19 = g_v128d[5];
    __m128d v128d_20 = _mm_div_pd(v128d_19, v128d_19);
    __m512d v512d_21 = _mm512_sub_pd(v512d_2, v512d_2);
    __m512d v512d_22 = _mm512_insertf64x2(v512d_1, v128d_19, 3);
    }
    __m256 v256f_23 = _mm256_max_ps(p1, p1);
    __m128d v128d_24 = g_v128d[5];
    __m128d v128d_25 = _mm_max_pd(v128d_24, v128d_24);
    __m128 v128f_26 = _mm_fmsub_ps(v128f_11, v128f_11, v128f_10);
    __mmask16 m16_27 = (__mmask16)(g_m16[6] ^ 34);
    __m512 v512f_28 = _mm512_mask_blend_ps(m16_27, v512f_5, v512f_5);
    __m512 v512f_29 = _mm512_sqrt_ps(p2);
    __m512 v512f_30 = _mm512_unpacklo_ps(v512f_5, v512f_5);
    __m256i v256i_31 = _mm256_set1_epi32(0x55);
    __m512 v512f_32 = _mm512_and_ps(p0, p2);
    __m512i v512i_33 = _mm512_slli_epi32(v512i_8, 3);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_27) & 7); i_34++) {
    __m256 v256f_35 = _mm256_fmsub_ps(v256f_23, v256f_23, p1);
    __m256i v256i_36 = _mm256_sllv_epi32(v256i_31, v256i_31);
    __m512 v512f_37 = _mm512_fmadd_ps(v512f_29, p2, v512f_30);
    __m128i v128i_38 = _mm512_cvtepi32_epi8(v512i_8);
    __m512 v512f_39 = _mm512_fnmadd_ps(v512f_5, v512f_32, v512f_29);
    }
    g_v128d[4] = v128d_25;
    g_m16[2] = m16_27;
    __mmask32 m32_40 = (__mmask32)(g_m32[10] ^ 193);
    return m32_40;
}

__mmask8 torture_54(__m256h p0, __m512i p1, __m512h p2, __m256 p3, __m128i p4) {
    __m256 v256f_1 = _mm256_sqrt_ps(p3);
    __m512 v512f_2 = g_v512f[10];
    __mmask16 m16_3 = _mm512_cmp_ps_mask(v512f_2, v512f_2, _CMP_LT_OQ);
    __m512d v512d_4 = g_v512d[10];
    __m128h v128h_5 = _mm512_cvtpd_ph(v512d_4);
    __mmask8 m8_6 = (__mmask8)(g_m8[1] ^ 99);
    __mmask8 m8_7 = _kand_mask8(m8_6, m8_6);
    __m128 v128f_8 = g_v128f[9];
    __m128 v128f_9 = _mm_max_ps(v128f_8, v128f_8);
    __m512h v512h_10 = _mm512_mul_ph(p2, p2);
    __m256d v256d_11 = g_v256d[9];
    __m256d v256d_12 = _mm256_fmadd_pd(v256d_11, v256d_11, v256d_11);
    __m512i v512i_13 = _mm512_ternarylogic_epi32(p1, p1, p1, 0x96);
    __m512d v512d_14 = _mm512_unpackhi_pd(v512d_4, v512d_4);
    __m512i v512i_15 = _mm512_cvtph_epi16(p2);
    __m512i v512i_16 = _mm512_lzcnt_epi32(v512i_15);
    __m512d v512d_17 = _mm512_sub_pd(v512d_14, v512d_4);
    __m512i v512i_18 = _mm512_unpacklo_epi32(v512i_13, v512i_13);
    __m256h v256h_19 = _mm512_cvtxps_ph(v512f_2);
    __m256 v256f_20 = _mm512_cvtpd_ps(v512d_17);
    __m512 v512f_21 = _mm512_add_ps(v512f_2, v512f_2);
    __mmask8 m8_22 = _kand_mask8(m8_7, m8_7);
    __m512 v512f_23 = _mm512_unpacklo_ps(v512f_2, v512f_21);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m512d v512d_24 = _mm512_fmadd_pd(v512d_14, v512d_4, v512d_4);
    __m256 v256f_25 = _mm256_mul_ps(v256f_20, p3);
    __m512i v512i_26 = _mm512_madd52lo_epu64(v512i_16, v512i_13, v512i_18);
    __m512 v512f_27 = _mm512_cvtxph_ps(p0);
    __m512i v512i_28 = _mm512_abs_epi32(v512i_16);
    }
    __m128 v128f_29 = _mm_div_ps(v128f_9, v128f_8);
    __mmask64 m64_30 = _mm512_movepi8_mask(v512i_18);
    __m512i v512i_31 = _mm512_popcnt_epi64(v512i_13);
    __m128i v128i_32 = _mm_set1_epi32(0x55);
    __m256i v256i_33 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_34 = _mm256_sllv_epi32(v256i_33, v256i_33);
    for (int i_35 = 0; i_35 < (int)(((unsigned long long)m16_3) & 7); i_35++) {
    __m512i v512i_36 = _mm512_madd52lo_epu64(v512i_31, v512i_18, v512i_15);
    __m128i v128i_37 = _mm_add_epi16(v128i_32, v128i_32);
    __m512i v512i_38 = _mm512_mask_blend_epi32(m16_3, v512i_18, v512i_13);
    __m256i v256i_39 = _mm256_xor_si256(v256i_34, v256i_33);
    __m256h v256h_40 = _mm512_cvtxps_ph(v512f_21);
    }
    g_v128f[6] = v128f_9;
    g_v512h[10] = p2;
    g_v128h[4] = v128h_5;
    g_m8[3] = m8_7;
    g_m16[6] = m16_3;
    return m8_7;
}

__m512d torture_55(__m256 p0, __m128 p1, __m512h p2, __m512h p3, __m256h p4) {
    __m512d v512d_1 = g_v512d[2];
    __m256 v256f_2 = _mm512_cvtpd_ps(v512d_1);
    __m512i v512i_3 = g_v512i[6];
    __m512i v512i_4 = _mm512_xor_si512(v512i_3, v512i_3);
    __m512 v512f_5 = g_v512f[0];
    __m512 v512f_6 = _mm512_min_ps(v512f_5, v512f_5);
    __m512 v512f_7 = _mm512_add_ps(v512f_6, v512f_6);
    __m256 v256f_8 = _mm256_set1_ps(1.0f);
    __mmask16 m16_9 = (__mmask16)(g_m16[11] ^ 171);
    __m512i v512i_10 = _mm512_mask_add_epi32(v512i_3, m16_9, v512i_3, v512i_4);
    __m256i v256i_11 = _mm512_cvtepi64_epi32(v512i_3);
    __m512h v512h_12 = _mm512_getexp_ph(p2);
    __m128 v128f_13 = _mm_shuffle_ps(p1, p1, 0x1B);
    __m256i v256i_14 = _mm256_sub_epi64(v256i_11, v256i_11);
    __m256i v256i_15 = _mm256_or_si256(v256i_11, v256i_11);
    __m512i v512i_16 = _mm512_rol_epi32(v512i_10, 5);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m128i v128i_17 = g_v128i[2];
    __m128i v128i_18 = _mm_sub_epi8(v128i_17, v128i_17);
    __mmask64 m64_19 = _mm512_cmp_epi8_mask(v512i_10, v512i_16, 1);
    }
    __m512d v512d_20 = _mm512_shuffle_f64x2(v512d_1, v512d_1, 0xEE);
    __m256d v256d_21 = g_v256d[1];
    __m256d v256d_22 = _mm256_sub_pd(v256d_21, v256d_21);
    __m128i v128i_23 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_24 = _mm_slli_epi32(v128i_23, 3);
    __m512i v512i_25 = _mm512_permutex2var_epi32(v512i_10, v512i_3, v512i_10);
    __m512i v512i_26 = _mm512_cvtps_epi32(v512f_7);
    __m512 v512f_27 = _mm512_mask_add_ps(v512f_6, m16_9, v512f_6, v512f_5);
    __m128i v128i_28 = _mm_set1_epi32(0x55);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m128d v128d_29 = g_v128d[11];
    __m128d v128d_30 = _mm_min_pd(v128d_29, v128d_29);
    __mmask16 m16_31 = _mm512_cmp_epu32_mask(v512i_3, v512i_16, 4);
    __m512i v512i_32 = _mm512_sub_epi16(v512i_10, v512i_16);
    __m512 v512f_33 = _mm512_fnmadd_ps(v512f_5, v512f_7, v512f_5);
    }
    g_v256f[0] = v256f_8;
    g_v256d[6] = v256d_22;
    g_v512h[11] = p2;
    g_v256h[10] = p4;
    return v512d_20;
}

__m512 torture_56(__m128d p0, __m128h p1) {
    __m512i v512i_1 = g_v512i[2];
    __m512i v512i_2 = _mm512_or_si512(v512i_1, v512i_1);
    __m512i v512i_3 = _mm512_ternarylogic_epi32(v512i_2, v512i_2, v512i_1, 0x96);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_getexp_ps(v512f_4);
    __mmask16 m16_6 = _mm512_test_epi32_mask(v512i_1, v512i_2);
    __m512 v512f_7 = _mm512_set1_ps(1.0f);
    __m256i v256i_8 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_9 = _mm256_or_si256(v256i_8, v256i_8);
    __m128 v128f_10 = _mm512_extractf32x4_ps(v512f_5, 2);
    __m512 v512f_11 = _mm512_fnmadd_ps(v512f_7, v512f_7, v512f_5);
    __m256i v256i_12 = _mm256_and_si256(v256i_8, v256i_9);
    __m512d v512d_13 = g_v512d[11];
    __m512d v512d_14 = _mm512_unpackhi_pd(v512d_13, v512d_13);
    __mmask16 m16_15 = _mm512_cmp_epi32_mask(v512i_3, v512i_2, 2);
    __m256i v256i_16 = _mm256_add_epi64(v256i_9, v256i_8);
    __m128d v128d_17 = _mm_sqrt_pd(p0);
    __m256i v256i_18 = _mm256_and_si256(v256i_8, v256i_12);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m512 v512f_19 = _mm512_and_ps(v512f_4, v512f_5);
    __m256i v256i_20 = _mm256_slli_epi32(v256i_12, 3);
    }
    __m256 v256f_21 = _mm256_loadu_ps(fbuf);
    __m512 v512f_22 = _mm512_insertf32x8(v512f_11, v256f_21, 1);
    __m512i v512i_23 = _mm512_madd52lo_epu64(v512i_3, v512i_2, v512i_1);
    __mmask16 m16_24 = _kor_mask16(m16_6, m16_6);
    __m128d v128d_25 = _mm_fmadd_pd(v128d_17, p0, p0);
    __m512 v512f_26 = _mm512_broadcast_f32x4(v128f_10);
    __m128d v128d_27 = _mm_min_pd(v128d_17, v128d_25);
    __m128i v128i_28 = g_v128i[2];
    __m512i v512i_29 = _mm512_cvtepi8_epi32(v128i_28);
    __m128i v128i_30 = _mm_add_epi8(v128i_28, v128i_28);
    __m512i v512i_31 = _mm512_dpbusd_epi32(v512i_1, v512i_3, v512i_2);
    __m128 v128f_32 = _mm_min_ps(v128f_10, v128f_10);
    __mmask32 m32_33 = _mm512_kunpackw(m16_24, m16_6);
    g_v512d[11] = v512d_13;
    g_v512i[10] = v512i_29;
    g_v256f[2] = v256f_21;
    g_v256i[5] = v256i_9;
    g_v128d[0] = v128d_17;
    g_v128h[8] = p1;
    return v512f_7;
}

__m512h torture_57(__m512i p0, __m256i p1, __m256h p2, __m128i p3) {
    __m512i v512i_1 = _mm512_add_epi32(p0, p0);
    __mmask16 m16_2 = _mm512_cmp_epi32_mask(v512i_1, p0, 2);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_min_ps(v128f_3, v128f_3);
    __m128 v128f_5 = _mm_fmsub_ps(v128f_4, v128f_3, v128f_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_shuffle_f32x4(v512f_6, v512f_6, 0x4E);
    __m512 v512f_8 = _mm512_add_ps(v512f_6, v512f_6);
    __m128i v128i_9 = _mm_and_si128(p3, p3);
    __m128 v128f_10 = _mm_sqrt_ps(v128f_3);
    __m512i v512i_11 = _mm512_permutex2var_epi32(p0, v512i_1, p0);
    __m128i v128i_12 = _mm_add_epi16(p3, v128i_9);
    __mmask16 m16_13 = _mm512_cmp_epu32_mask(v512i_1, p0, 4);
    __m128 v128f_14 = _mm_sqrt_ps(v128f_4);
    __m512i v512i_15 = _mm512_sub_epi16(v512i_11, p0);
    if (((unsigned long long)m16_13) & 1ULL) {
    __m512 v512f_16 = _mm512_maskz_mul_ps(m16_13, v512f_6, v512f_6);
    __m512i v512i_17 = _mm512_cvtps_epi32(v512f_7);
    __m512 v512f_18 = _mm512_sub_ps(v512f_7, v512f_6);
    }
    __m512d v512d_19 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_20 = _mm512_cmp_pd_mask(v512d_19, v512d_19, _CMP_GE_OQ);
    __m256i v256i_21 = _mm256_or_si256(p1, p1);
    __m128 v128f_22 = _mm_sqrt_ps(v128f_14);
    __m128 v128f_23 = _mm_and_ps(v128f_3, v128f_14);
    __m256 v256f_24 = _mm512_cvtpd_ps(v512d_19);
    __m128 v128f_25 = _mm_add_ps(v128f_3, v128f_10);
    __m128i v128i_26 = _mm_sub_epi64(v128i_12, p3);
    __m512i v512i_27 = _mm512_set1_epi32(0x55);
    __m512 v512f_28 = _mm512_max_ps(v512f_8, v512f_7);
    __m128i v128i_29 = _mm_or_si128(v128i_9, p3);
    __m512d v512d_30 = _mm512_mul_pd(v512d_19, v512d_19);
    g_v256i[2] = p1;
    g_v128i[5] = v128i_9;
    g_m16[10] = m16_2;
    __m512h v512h_31 = g_v512h[9];
    return v512h_31;
}

__m128 torture_58(__m512h p0, __m256 p1, __m512d p2, __m128i p3) {
    __m256d v256d_1 = _mm256_set1_pd(2.0);
    __m256i v256i_2 = g_v256i[11];
    __m256i v256i_3 = _mm256_slli_epi32(v256i_2, 3);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_and_si512(v512i_4, v512i_4);
    __m512i v512i_6 = _mm512_broadcast_i32x4(p3);
    unsigned u32_7 = 83;
    __mmask16 m16_8 = _cvtu32_mask16(u32_7);
    __m512i v512i_9 = _mm512_rolv_epi32(v512i_4, v512i_4);
    __m256i v256i_10 = _mm256_shuffle_epi8(v256i_3, v256i_2);
    __m256 v256f_11 = _mm256_sqrt_ps(p1);
    for (int i_12 = 0; i_12 < (int)(((unsigned long long)m16_8) & 7); i_12++) {
    __m128 v128f_13 = _mm_set1_ps(1.0f);
    __m512i v512i_14 = _mm512_conflict_epi32(v512i_5);
    __mmask8 m8_15 = (__mmask8)(g_m8[7] ^ 28);
    __mmask8 m8_16 = _kand_mask8(m8_15, m8_15);
    __mmask64 m64_17 = (__mmask64)(g_m64[10] ^ 45);
    __mmask64 m64_18 = _kor_mask64(m64_17, m64_17);
    __m512 v512f_19 = g_v512f[4];
    __m512i v512i_20 = _mm512_cvtps_epi32(v512f_19);
    }
    __m512d v512d_21 = _mm512_div_pd(p2, p2);
    __m512 v512f_22 = _mm512_loadu_ps(fbuf);
    __m512 v512f_23 = _mm512_mask3_fmadd_ps(v512f_22, v512f_22, v512f_22, m16_8);
    __mmask32 m32_24 = _mm512_kunpackw(m16_8, m16_8);
    __m128i v128i_25 = _mm_sub_epi8(p3, p3);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m256i v256i_26 = _mm256_mullo_epi32(v256i_3, v256i_2);
    __mmask32 m32_27 = _mm512_cmp_ph_mask(p0, p0, 1);
    }
    g_v512d[11] = p2;
    g_v256i[4] = v256i_10;
    __m128 v128f_28 = _mm_loadu_ps(fbuf);
    return v128f_28;
}

__m256 torture_59(__m128i p0, __m512h p1, __m128d p2) {
    __m128i v128i_1 = _mm_or_si128(p0, p0);
    __m512d v512d_2 = _mm512_loadu_pd(dbuf);
    __m512d v512d_3 = _mm512_sqrt_pd(v512d_2);
    __m128d v128d_4 = _mm_mul_pd(p2, p2);
    __m512h v512h_5 = _mm512_sqrt_ph(p1);
    __m512i v512i_6 = g_v512i[10];
    __m512i v512i_7 = _mm512_dpwssd_epi32(v512i_6, v512i_6, v512i_6);
    __m512i v512i_8 = _mm512_popcnt_epi64(v512i_6);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m256 v256f_10 = _mm256_loadu_ps(fbuf);
    __m512 v512f_11 = _mm512_insertf32x8(v512f_9, v256f_10, 1);
    __m256i v256i_12 = g_v256i[4];
    __m256i v256i_13 = _mm256_add_epi64(v256i_12, v256i_12);
    __m512d v512d_14 = _mm512_set1_pd(2.0);
    __m128d v128d_15 = _mm_add_pd(p2, p2);
    __m256 v256f_16 = _mm256_xor_ps(v256f_10, v256f_10);
    __m256 v256f_17 = _mm256_sub_ps(v256f_16, v256f_10);
    __m256d v256d_18 = g_v256d[10];
    __m256d v256d_19 = _mm256_mul_pd(v256d_18, v256d_18);
    __m128d v128d_20 = _mm_sqrt_pd(v128d_4);
    __m512 v512f_21 = _mm512_set1_ps(1.0f);
    __m512i v512i_22 = _mm512_unpacklo_epi32(v512i_8, v512i_6);
    __m512 v512f_23 = _mm512_cvtepi32_ps(v512i_8);
    int s32_24 = 16;
    if ((s32_24) & 1ULL) {
    __m256 v256f_25 = _mm256_fnmadd_ps(v256f_16, v256f_17, v256f_17);
    __mmask16 m16_26 = (__mmask16)(g_m16[11] ^ 213);
    __m512 v512f_27 = _mm512_mask_add_ps(v512f_9, m16_26, v512f_11, v512f_11);
    __m256d v256d_28 = _mm256_set1_pd(2.0);
    __m512h v512h_29 = _mm512_rcp_ph(p1);
    __m256 v256f_30 = _mm256_fmadd_ps(v256f_25, v256f_10, v256f_17);
    }
    __m256 v256f_31 = _mm256_mul_ps(v256f_10, v256f_17);
    __mmask16 m16_32 = _mm512_cmp_ps_mask(v512f_21, v512f_23, _CMP_LT_OQ);
    __m512i v512i_33 = _mm512_popcnt_epi64(v512i_22);
    __m128d v128d_34 = _mm_div_pd(p2, v128d_15);
    __m128 v128f_35 = _mm_loadu_ps(fbuf);
    __m128 v128f_36 = _mm_add_ps(v128f_35, v128f_35);
    __m512 v512f_37 = _mm512_cvtepi32_ps(v512i_22);
    __m512 v512f_38 = _mm512_set1_ps(1.0f);
    __m128i v128i_39 = _mm_and_si128(p0, v128i_1);
    for (int i_40 = 0; i_40 < (int)(((unsigned long long)m16_32) & 7); i_40++) {
    __m256d v256d_41 = _mm256_unpackhi_pd(v256d_19, v256d_19);
    __m512i v512i_42 = _mm512_broadcastmw_epi32(m16_32);
    __mmask16 m16_43 = _knot_mask16(m16_32);
    }
    g_v512f[11] = v512f_11;
    g_v256d[8] = v256d_19;
    g_v256i[9] = v256i_12;
    return v256f_16;
}

__m512d torture_60(__m512d p0, __m512i p1, __m256h p2, __m512i p3) {
    __m256 v256f_1 = g_v256f[10];
    __m256 v256f_2 = _mm256_max_ps(v256f_1, v256f_1);
    __m512i v512i_3 = _mm512_ternarylogic_epi32(p1, p3, p3, 0x96);
    unsigned u32_4 = 81;
    __mmask16 m16_5 = _cvtu32_mask16(u32_4);
    __m512d v512d_6 = _mm512_sub_pd(p0, p0);
    __m512h v512h_7 = g_v512h[1];
    __m512h v512h_8 = _mm512_getexp_ph(v512h_7);
    __m512i v512i_9 = _mm512_broadcastmw_epi32(m16_5);
    __m128i v128i_10 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_11 = _mm_add_epi32(v128i_10, v128i_10);
    __m256i v256i_12 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_13 = _mm256_slli_epi32(v256i_12, 3);
    __m128i v128i_14 = _mm_mullo_epi32(v128i_11, v128i_11);
    __m128i v128i_15 = _mm_add_epi16(v128i_11, v128i_11);
    __m128 v128f_16 = g_v128f[2];
    __m128 v128f_17 = _mm_min_ps(v128f_16, v128f_16);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m512 v512f_18 = _mm512_loadu_ps(fbuf);
    __m512 v512f_19 = _mm512_fmsub_ps(v512f_18, v512f_18, v512f_18);
    __m128 v128f_20 = _mm_shuffle_ps(v128f_17, v128f_16, 0x1B);
    }
    __m512i v512i_21 = _mm512_permutex2var_epi32(p1, p3, v512i_9);
    __m128i v128i_22 = _mm_sub_epi16(v128i_10, v128i_11);
    __m512 v512f_23 = g_v512f[6];
    __m512 v512f_24 = _mm512_xor_ps(v512f_23, v512f_23);
    __mmask16 m16_25 = _mm512_cmp_epu32_mask(v512i_21, v512i_9, 4);
    __m512 v512f_26 = _mm512_sub_ps(v512f_23, v512f_23);
    __mmask16 m16_27 = _mm512_cmp_ps_mask(v512f_24, v512f_24, _CMP_LT_OQ);
    __m256 v256f_28 = _mm256_and_ps(v256f_1, v256f_2);
    g_v512d[5] = p0;
    g_v256i[8] = v256i_13;
    g_v256h[3] = p2;
    g_m16[6] = m16_25;
    return p0;
}

__m512h torture_61(__m128h p0, __m256h p1, __m512i p2, __m512h p3, __m512h p4) {
    __m256 v256f_1 = g_v256f[11];
    __m256 v256f_2 = _mm256_fmadd_ps(v256f_1, v256f_1, v256f_1);
    __m512i v512i_3 = _mm512_sub_epi8(p2, p2);
    __m128i v128i_4 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_5 = _mm_unpacklo_epi32(v128i_4, v128i_4);
    __m128 v128f_6 = g_v128f[0];
    __m128 v128f_7 = _mm_max_ps(v128f_6, v128f_6);
    __m128i v128i_8 = _mm_and_si128(v128i_5, v128i_4);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m256d v256d_10 = _mm512_extractf64x4_pd(v512d_9, 1);
    __m128i v128i_11 = _mm_add_epi16(v128i_4, v128i_5);
    __m128i v128i_12 = _mm_mullo_epi32(v128i_4, v128i_5);
    __m256i v256i_13 = g_v256i[1];
    __m256i v256i_14 = _mm256_sub_epi16(v256i_13, v256i_13);
    __m512h v512h_15 = _mm512_fmsub_ph(p3, p4, p4);
    __m512i v512i_16 = _mm512_permutexvar_epi32(p2, v512i_3);
    __m128 v128f_17 = _mm_max_ps(v128f_6, v128f_7);
    __m128d v128d_18 = _mm_set1_pd(2.0);
    __m512 v512f_19 = g_v512f[9];
    __mmask16 m16_20 = _mm512_fpclass_ps_mask(v512f_19, 0x3);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_20) & 7); i_21++) {
    __m128i v128i_22 = _mm_or_si128(v128i_5, v128i_5);
    __m512d v512d_23 = _mm512_insertf64x2(v512d_9, v128d_18, 3);
    __m512d v512d_24 = _mm512_sub_pd(v512d_23, v512d_9);
    __mmask64 m64_25 = (__mmask64)(g_m64[7] ^ 254);
    __mmask64 m64_26 = _kor_mask64(m64_25, m64_25);
    __m256h v256h_27 = _mm512_cvtxps_ph(v512f_19);
    }
    __m256i v256i_28 = _mm256_mullo_epi32(v256i_14, v256i_13);
    __m512d v512d_29 = _mm512_sqrt_pd(v512d_9);
    __m512i v512i_30 = _mm512_rol_epi32(p2, 5);
    __m512h v512h_31 = _mm512_rcp_ph(p3);
    __m256 v256f_32 = _mm256_add_ps(v256f_2, v256f_1);
    __m256 v256f_33 = _mm256_xor_ps(v256f_2, v256f_2);
    __m128 v128f_34 = _mm_add_ps(v128f_17, v128f_6);
    g_v256f[4] = v256f_2;
    g_v256d[9] = v256d_10;
    g_v256i[9] = v256i_28;
    g_v128i[8] = v128i_12;
    return v512h_31;
}

__m128i torture_62(__m128i p0, __m128h p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_fmadd_ps(v512f_1, v512f_1, v512f_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_add_pd(v512d_3, v512d_3);
    __m512i v512i_5 = g_v512i[7];
    __m512i v512i_6 = _mm512_sub_epi8(v512i_5, v512i_5);
    __mmask16 m16_7 = _mm512_test_epi32_mask(v512i_5, v512i_5);
    __m512 v512f_8 = _mm512_scalef_ps(v512f_2, v512f_2);
    __m128 v128f_9 = _mm_loadu_ps(fbuf);
    __m128 v128f_10 = _mm_div_ps(v128f_9, v128f_9);
    __m512 v512f_11 = _mm512_mask_blend_ps(m16_7, v512f_2, v512f_1);
    __m256d v256d_12 = g_v256d[6];
    __m256d v256d_13 = _mm256_sub_pd(v256d_12, v256d_12);
    __m128i v128i_14 = _mm_xor_si128(p0, p0);
    __m128i v128i_15 = _mm_mullo_epi32(v128i_14, p0);
    __m128i v128i_16 = _mm_sub_epi16(v128i_15, v128i_15);
    __m512d v512d_17 = _mm512_fmadd_pd(v512d_4, v512d_4, v512d_3);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m128i v128i_18 = _mm_sub_epi16(v128i_15, v128i_15);
    __m512 v512f_19 = _mm512_maskz_mul_ps(m16_7, v512f_8, v512f_2);
    __m256i v256i_20 = g_v256i[0];
    __m256i v256i_21 = _mm256_shuffle_epi8(v256i_20, v256i_20);
    }
    __m128i v128i_22 = _mm_sub_epi16(v128i_15, v128i_15);
    __m512 v512f_23 = _mm512_mask_blend_ps(m16_7, v512f_1, v512f_8);
    __m128d v128d_24 = g_v128d[11];
    __m128d v128d_25 = _mm_unpackhi_pd(v128d_24, v128d_24);
    __m256d v256d_26 = _mm256_mul_pd(v256d_13, v256d_13);
    __mmask64 m64_27 = (__mmask64)(g_m64[0] ^ 61);
    __mmask64 m64_28 = _kor_mask64(m64_27, m64_27);
    __m256 v256f_29 = g_v256f[3];
    __m256 v256f_30 = _mm256_shuffle_ps(v256f_29, v256f_29, 0x1B);
    __m256 v256f_31 = _mm256_fmadd_ps(v256f_29, v256f_29, v256f_30);
    __mmask16 m16_32 = _kand_mask16(m16_7, m16_7);
    __mmask64 m64_33 = _mm512_movepi8_mask(v512i_5);
    __m128i v128i_34 = _mm_or_si128(v128i_22, v128i_14);
    g_v512i[1] = v512i_5;
    g_v128f[9] = v128f_10;
    g_v128d[9] = v128d_25;
    g_v128i[1] = v128i_15;
    g_v128h[4] = p1;
    return p0;
}

__m512d torture_63(__m128 p0, __m256i p1) {
    __m128 v128f_1 = _mm_max_ps(p0, p0);
    __m512 v512f_2 = g_v512f[6];
    __m512 v512f_3 = _mm512_shuffle_f32x4(v512f_2, v512f_2, 0x4E);
    __m512i v512i_4 = _mm512_set1_epi32(0x55);
    __m512i v512i_5 = _mm512_shuffle_epi8(v512i_4, v512i_4);
    __m512h v512h_6 = g_v512h[3];
    __m512h v512h_7 = _mm512_rcp_ph(v512h_6);
    __mmask64 m64_8 = _mm512_cmp_epi8_mask(v512i_5, v512i_5, 1);
    __m128d v128d_9 = g_v128d[4];
    __m128d v128d_10 = _mm_unpackhi_pd(v128d_9, v128d_9);
    __m256i v256i_11 = _mm256_sllv_epi32(p1, p1);
    __m512d v512d_12 = _mm512_loadu_pd(dbuf);
    __m512d v512d_13 = _mm512_add_pd(v512d_12, v512d_12);
    __m512 v512f_14 = _mm512_add_ps(v512f_2, v512f_3);
    __m256 v256f_15 = _mm512_cvtpd_ps(v512d_12);
    __m512h v512h_16 = _mm512_fmadd_ph(v512h_6, v512h_6, v512h_6);
    __m128 v128f_17 = _mm_fmadd_ps(v128f_1, v128f_1, v128f_1);
    __m512i v512i_18 = _mm512_shuffle_epi8(v512i_5, v512i_4);
    __mmask32 m32_19 = _mm512_cmp_ph_mask(v512h_6, v512h_16, 1);
    __m128i v128i_20 = g_v128i[5];
    __m128i v128i_21 = _mm_abs_epi32(v128i_20);
    if (((unsigned long long)m32_19) & 1ULL) {
    __m256 v256f_22 = _mm256_min_ps(v256f_15, v256f_15);
    __m128h v128h_23 = _mm512_cvtpd_ph(v512d_12);
    }
    __m512d v512d_24 = _mm512_sub_pd(v512d_12, v512d_12);
    __m256i v256i_25 = _mm256_xor_si256(p1, v256i_11);
    __m256d v256d_26 = g_v256d[9];
    __m256d v256d_27 = _mm256_max_pd(v256d_26, v256d_26);
    __m256 v256f_28 = _mm256_unpacklo_ps(v256f_15, v256f_15);
    __m512i v512i_29 = _mm512_or_si512(v512i_18, v512i_18);
    __m512i v512i_30 = _mm512_shuffle_epi8(v512i_5, v512i_18);
    __m512i v512i_31 = _mm512_cvtph_epi16(v512h_6);
    __m128 v128f_32 = _mm_sqrt_ps(p0);
    __mmask16 m16_33 = (__mmask16)(g_m16[10] ^ 171);
    __mmask16 m16_34 = _kor_mask16(m16_33, m16_33);
    __m128i v128i_35 = _mm_sub_epi8(v128i_20, v128i_20);
    g_v256d[9] = v256d_26;
    g_v256i[1] = v256i_25;
    g_v128f[9] = v128f_17;
    g_m64[7] = m64_8;
    return v512d_13;
}

__m512 torture_64(__m128h p0, __m256d p1) {
    __m512 v512f_1 = g_v512f[0];
    __m512 v512f_2 = _mm512_getexp_ps(v512f_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[8] ^ 168);
    __mmask16 m16_4 = _kshiftli_mask16(m16_3, 2);
    __mmask16 m16_5 = _kshiftri_mask16(m16_4, 1);
    __m128 v128f_6 = _mm_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_broadcast_f32x4(v128f_6);
    __m256 v256f_8 = g_v256f[7];
    __m256 v256f_9 = _mm256_add_ps(v256f_8, v256f_8);
    __m128 v128f_10 = _mm_fmadd_ps(v128f_6, v128f_6, v128f_6);
    __m128i v128i_11 = g_v128i[6];
    __m128i v128i_12 = _mm_xor_si128(v128i_11, v128i_11);
    __m512i v512i_13 = _mm512_loadu_si512((const void*)ibuf);
    __m512h v512h_14 = _mm512_cvtepi16_ph(v512i_13);
    __mmask8 m8_15 = (__mmask8)(g_m8[3] ^ 207);
    __m512d v512d_16 = _mm512_loadu_pd(dbuf);
    __m512d v512d_17 = _mm512_mask_blend_pd(m8_15, v512d_16, v512d_16);
    __mmask16 m16_18 = _mm512_cmp_epi32_mask(v512i_13, v512i_13, 2);
    __m256 v256f_19 = _mm256_shuffle_ps(v256f_9, v256f_9, 0x1B);
    __m512 v512f_20 = _mm512_shuffle_ps(v512f_7, v512f_1, 0x1B);
    __m128i v128i_21 = _mm_add_epi64(v128i_12, v128i_12);
    __mmask64 m64_22 = _mm512_cmp_epi8_mask(v512i_13, v512i_13, 1);
    __m256d v256d_23 = _mm256_set1_pd(2.0);
    __m256 v256f_24 = _mm256_mul_ps(v256f_9, v256f_9);
    __m128i v128i_25 = _mm_or_si128(v128i_11, v128i_21);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_18) & 7); i_26++) {
    __m512 v512f_27 = _mm512_fmadd_ps(v512f_2, v512f_2, v512f_2);
    __m256h v256h_28 = _mm512_cvtxps_ph(v512f_1);
    __m512 v512f_29 = _mm512_broadcast_f32x4(v128f_6);
    }
    __m256 v256f_30 = _mm256_max_ps(v256f_24, v256f_19);
    __m512d v512d_31 = _mm512_sub_pd(v512d_17, v512d_17);
    __m128d v128d_32 = g_v128d[11];
    __m128d v128d_33 = _mm_sqrt_pd(v128d_32);
    __m256d v256d_34 = _mm256_div_pd(v256d_23, p1);
    __m512d v512d_35 = _mm512_div_pd(v512d_16, v512d_16);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m128i v128i_36 = _mm_add_epi16(v128i_21, v128i_12);
    __m512i v512i_37 = _mm512_cvtph_epi16(v512h_14);
    __m128d v128d_38 = _mm_sqrt_pd(v128d_32);
    __m128 v128f_39 = _mm_xor_ps(v128f_10, v128f_10);
    __m512d v512d_40 = _mm512_unpackhi_pd(v512d_35, v512d_16);
    }
    g_v512f[5] = v512f_2;
    g_v512d[3] = v512d_16;
    g_v512i[10] = v512i_13;
    g_v128h[1] = p0;
    return v512f_7;
}

__m512i torture_65(__m128d p0, __m512i p1, __m256 p2, __m512h p3, __m512 p4) {
    __m512 v512f_1 = _mm512_fmsub_ps(p4, p4, p4);
    __m128 v128f_2 = _mm_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_broadcast_f32x4(v128f_2);
    __m128 v128f_4 = _mm_shuffle_ps(v128f_2, v128f_2, 0x1B);
    __m256h v256h_5 = _mm512_cvtxps_ph(v512f_3);
    __m128 v128f_6 = _mm_sub_ps(v128f_4, v128f_4);
    __m256i v256i_7 = g_v256i[3];
    __m256i v256i_8 = _mm256_sub_epi8(v256i_7, v256i_7);
    __m512i v512i_9 = _mm512_conflict_epi32(p1);
    __m512i v512i_10 = _mm512_popcnt_epi64(p1);
    __mmask32 m32_11 = _mm512_cmp_ph_mask(p3, p3, 1);
    __m128d v128d_12 = _mm_sub_pd(p0, p0);
    __m512d v512d_13 = _mm512_loadu_pd(dbuf);
    __m512d v512d_14 = _mm512_sqrt_pd(v512d_13);
    __m128h v128h_15 = _mm512_cvtpd_ph(v512d_13);
    __m128d v128d_16 = _mm_add_pd(p0, p0);
    __m256 v256f_17 = _mm256_div_ps(p2, p2);
    __m512 v512f_18 = _mm512_rsqrt14_ps(p4);
    __m512d v512d_19 = _mm512_shuffle_f64x2(v512d_14, v512d_14, 0xEE);
    if (((unsigned long long)m32_11) & 1ULL) {
    __m512 v512f_20 = _mm512_insertf32x8(v512f_3, p2, 1);
    __mmask8 m8_21 = (__mmask8)(g_m8[11] ^ 152);
    __mmask16 m16_22 = _mm512_kunpackb(m8_21, m8_21);
    __m512d v512d_23 = _mm512_cvtps_pd(p2);
    __m512 v512f_24 = _mm512_mask_blend_ps(m16_22, v512f_3, v512f_20);
    __mmask64 m64_25 = (__mmask64)(g_m64[4] ^ 214);
    __mmask64 m64_26 = _kor_mask64(m64_25, m64_25);
    }
    __mmask64 m64_27 = (__mmask64)(g_m64[7] ^ 10);
    __mmask64 m64_28 = _kor_mask64(m64_27, m64_27);
    __m512i v512i_29 = _mm512_sub_epi8(v512i_10, v512i_10);
    __m512d v512d_30 = _mm512_unpackhi_pd(v512d_13, v512d_19);
    __m512h v512h_31 = _mm512_fmul_pch(p3, p3);
    __m512i v512i_32 = _mm512_xor_si512(v512i_29, v512i_29);
    __m256 v256f_33 = _mm512_cvtpd_ps(v512d_30);
    __m256d v256d_34 = g_v256d[10];
    __m256d v256d_35 = _mm256_add_pd(v256d_34, v256d_34);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m32_11) & 7); i_36++) {
    __m128i v128i_37 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_38 = _mm_sub_epi16(v128i_37, v128i_37);
    __m256 v256f_39 = _mm256_and_ps(v256f_33, v256f_33);
    __m512d v512d_40 = _mm512_set1_pd(2.0);
    __m256 v256f_41 = _mm256_xor_ps(v256f_17, p2);
    }
    g_v512i[2] = v512i_10;
    g_v256f[11] = v256f_17;
    g_v128f[0] = v128f_2;
    g_v128d[10] = v128d_16;
    g_v512h[9] = p3;
    g_v128h[6] = v128h_15;
    g_m64[8] = m64_28;
    return p1;
}

__m512h torture_66(__m512d p0, __m256i p1, __m256i p2, __m512h p3, __m128 p4) {
    __m512i v512i_1 = g_v512i[4];
    __m512 v512f_2 = _mm512_cvtepi32_ps(v512i_1);
    __m512i v512i_3 = _mm512_or_si512(v512i_1, v512i_1);
    __m512d v512d_4 = _mm512_set1_pd(2.0);
    __m128i v128i_5 = g_v128i[9];
    __m128i v128i_6 = _mm_sub_epi64(v128i_5, v128i_5);
    __mmask8 m8_7 = _mm512_cmp_pd_mask(p0, p0, _CMP_GE_OQ);
    __m512i v512i_8 = _mm512_rol_epi32(v512i_1, 5);
    __mmask16 m16_9 = _mm512_test_epi32_mask(v512i_8, v512i_8);
    __m512h v512h_10 = _mm512_fmsub_ph(p3, p3, p3);
    __m512i v512i_11 = _mm512_sllv_epi32(v512i_1, v512i_1);
    __m512i v512i_12 = _mm512_rolv_epi32(v512i_1, v512i_3);
    __m512 v512f_13 = _mm512_broadcast_f32x4(p4);
    __mmask16 m16_14 = _mm512_testn_epi32_mask(v512i_1, v512i_11);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_9) & 7); i_15++) {
    __m256d v256d_16 = g_v256d[1];
    __m256d v256d_17 = _mm256_max_pd(v256d_16, v256d_16);
    __m512i v512i_18 = _mm512_set1_epi32(0x55);
    __mmask16 m16_19 = _mm512_cmp_ps_mask(v512f_13, v512f_2, _CMP_LT_OQ);
    }
    g_v128f[9] = p4;
    g_m16[0] = m16_9;
    return v512h_10;
}

__mmask16 torture_67(__m512h p0, __m512d p1, __m256i p2) {
    __m128i v128i_1 = _mm_set1_epi32(0x55);
    __m128i v128i_2 = _mm_set1_epi32(0x55);
    __m512 v512f_3 = g_v512f[10];
    __m256 v256f_4 = g_v256f[2];
    __m512 v512f_5 = _mm512_insertf32x8(v512f_3, v256f_4, 1);
    __mmask16 m16_6 = (__mmask16)(g_m16[9] ^ 73);
    __mmask16 m16_7 = _kxor_mask16(m16_6, m16_6);
    __m128i v128i_8 = _mm_shuffle_epi8(v128i_2, v128i_2);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_sub_epi64(v512i_9, v512i_9);
    __m256i v256i_11 = _mm256_sllv_epi32(p2, p2);
    __m128 v128f_12 = _mm_loadu_ps(fbuf);
    __m128 v128f_13 = _mm_sqrt_ps(v128f_12);
    __m512d v512d_14 = _mm512_unpackhi_pd(p1, p1);
    __m512i v512i_15 = _mm512_ternarylogic_epi32(v512i_10, v512i_9, v512i_10, 0x96);
    __mmask16 m16_16 = _knot_mask16(m16_7);
    __m128 v128f_17 = _mm_add_ps(v128f_13, v128f_12);
    __m512 v512f_18 = _mm512_and_ps(v512f_3, v512f_3);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m256i v256i_19 = _mm256_sub_epi32(p2, v256i_11);
    __m128 v128f_20 = _mm_xor_ps(v128f_12, v128f_12);
    __m512d v512d_21 = _mm512_shuffle_f64x2(v512d_14, p1, 0xEE);
    __m128d v128d_22 = _mm_set1_pd(2.0);
    }
    __m512 v512f_23 = _mm512_roundscale_ps(v512f_3, 0x10);
    __m256i v256i_24 = _mm256_abs_epi32(p2);
    __mmask8 m8_25 = (__mmask8)(g_m8[1] ^ 142);
    __mmask16 m16_26 = _mm512_kunpackb(m8_25, m8_25);
    __m128 v128f_27 = _mm_set1_ps(1.0f);
    __m256d v256d_28 = g_v256d[7];
    __m256d v256d_29 = _mm256_max_pd(v256d_28, v256d_28);
    __m512 v512f_30 = _mm512_xor_ps(v512f_5, v512f_3);
    __m512 v512f_31 = _mm512_div_ps(v512f_23, v512f_18);
    __m256i v256i_32 = _mm256_slli_epi32(v256i_24, 3);
    __m256i v256i_33 = _mm512_cvtepi64_epi32(v512i_15);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m512i v512i_34 = _mm512_cvtepi8_epi32(v128i_1);
    unsigned u32_35 = 5;
    __mmask16 m16_36 = _cvtu32_mask16(u32_35);
    }
    g_v256f[6] = v256f_4;
    g_v256d[7] = v256d_28;
    g_m8[3] = m8_25;
    return m16_16;
}

__m512d torture_68(__m512i p0, __m512 p1, __m128 p2) {
    __m128 v128f_1 = _mm_div_ps(p2, p2);
    __m128i v128i_2 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_3 = _mm_add_epi32(v128i_2, v128i_2);
    __m512i v512i_4 = _mm512_lzcnt_epi32(p0);
    __mmask16 m16_5 = (__mmask16)(g_m16[2] ^ 189);
    __mmask16 m16_6 = _kor_mask16(m16_5, m16_5);
    __mmask16 m16_7 = _mm512_fpclass_ps_mask(p1, 0x3);
    __m128 v128f_8 = _mm_sqrt_ps(v128f_1);
    __m512 v512f_9 = _mm512_unpacklo_ps(p1, p1);
    __mmask64 m64_10 = _mm512_movepi8_mask(v512i_4);
    for (int i_11 = 0; i_11 < (int)(((unsigned long long)m16_6) & 7); i_11++) {
    __m256 v256f_12 = g_v256f[0];
    __m512d v512d_13 = _mm512_cvtps_pd(v256f_12);
    __m512 v512f_14 = _mm512_mul_ps(p1, p1);
    __m512 v512f_15 = _mm512_fmsub_ps(p1, v512f_9, p1);
    __m128i v128i_16 = _mm_sub_epi8(v128i_2, v128i_2);
    __m256 v256f_17 = _mm256_shuffle_ps(v256f_12, v256f_12, 0x1B);
    }
    __mmask32 m32_18 = _mm512_kunpackw(m16_5, m16_5);
    __m512i v512i_19 = _mm512_dpwssd_epi32(p0, v512i_4, p0);
    __m512h v512h_20 = g_v512h[2];
    __mmask32 m32_21 = _mm512_cmp_ph_mask(v512h_20, v512h_20, 1);
    __m256i v256i_22 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_23 = _mm256_unpacklo_epi32(v256i_22, v256i_22);
    __m512h v512h_24 = _mm512_getexp_ph(v512h_20);
    __m512 v512f_25 = _mm512_max_ps(v512f_9, p1);
    __m512 v512f_26 = _mm512_div_ps(v512f_9, v512f_9);
    __m512i v512i_27 = _mm512_dpwssd_epi32(v512i_4, v512i_4, v512i_4);
    __m512i v512i_28 = _mm512_sllv_epi32(p0, p0);
    __m128i v128i_29 = _mm_and_si128(v128i_3, v128i_2);
    __m512h v512h_30 = _mm512_fmsub_ph(v512h_20, v512h_24, v512h_20);
    __m512i v512i_31 = _mm512_ternarylogic_epi32(v512i_27, p0, v512i_28, 0x96);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m128i v128i_32 = _mm_slli_epi32(v128i_2, 3);
    __m512i v512i_33 = _mm512_dpbusd_epi32(v512i_4, p0, v512i_27);
    __m512 v512f_34 = _mm512_broadcast_f32x4(v128f_1);
    __m512 v512f_35 = _mm512_sqrt_ps(v512f_9);
    __m512i v512i_36 = _mm512_sub_epi8(v512i_31, v512i_27);
    }
    g_v256i[11] = v256i_22;
    g_v128f[10] = v128f_8;
    g_v512h[11] = v512h_20;
    g_m16[7] = m16_6;
    __m512d v512d_37 = g_v512d[1];
    return v512d_37;
}

__mmask8 torture_69(__m512 p0, __m128d p1) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_abs_epi32(v128i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_4 = _mm512_cmp_epi32_mask(v512i_3, v512i_3, 2);
    __m512d v512d_5 = _mm512_loadu_pd(dbuf);
    __m512d v512d_6 = _mm512_sub_pd(v512d_5, v512d_5);
    __m512d v512d_7 = _mm512_min_pd(v512d_5, v512d_6);
    __m128i v128i_8 = _mm_or_si128(v128i_1, v128i_2);
    __mmask16 m16_9 = _kor_mask16(m16_4, m16_4);
    __mmask16 m16_10 = _mm512_cmp_ps_mask(p0, p0, _CMP_LT_OQ);
    __m256 v256f_11 = g_v256f[8];
    __m256 v256f_12 = _mm256_sub_ps(v256f_11, v256f_11);
    __m256 v256f_13 = _mm256_mul_ps(v256f_11, v256f_11);
    __mmask16 m16_14 = _kxor_mask16(m16_9, m16_4);
    __m512i v512i_15 = _mm512_mullo_epi32(v512i_3, v512i_3);
    __m256d v256d_16 = g_v256d[10];
    __m256d v256d_17 = _mm256_sqrt_pd(v256d_16);
    __m256 v256f_18 = _mm256_shuffle_ps(v256f_11, v256f_13, 0x1B);
    __m128 v128f_19 = _mm_loadu_ps(fbuf);
    __m128 v128f_20 = _mm_fmadd_ps(v128f_19, v128f_19, v128f_19);
    __m512d v512d_21 = _mm512_shuffle_f64x2(v512d_7, v512d_5, 0xEE);
    __m128i v128i_22 = _mm_sub_epi64(v128i_8, v128i_2);
    __m512d v512d_23 = _mm512_unpackhi_pd(v512d_5, v512d_21);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m128 v128f_24 = _mm_xor_ps(v128f_20, v128f_19);
    __m128 v128f_25 = _mm_and_ps(v128f_20, v128f_24);
    __mmask16 m16_26 = _mm512_cmp_epi32_mask(v512i_3, v512i_3, 2);
    __m256 v256f_27 = _mm256_sub_ps(v256f_18, v256f_18);
    __m512 v512f_28 = _mm512_fmsub_ps(p0, p0, p0);
    }
    __mmask64 m64_29 = (__mmask64)(g_m64[0] ^ 130);
    __mmask64 m64_30 = _kor_mask64(m64_29, m64_29);
    __m128i v128i_31 = _mm_sub_epi8(v128i_2, v128i_22);
    __m256i v256i_32 = g_v256i[1];
    __m256i v256i_33 = _mm256_xor_si256(v256i_32, v256i_32);
    __m128i v128i_34 = _mm_add_epi32(v128i_8, v128i_22);
    g_v512i[6] = v512i_3;
    g_v256d[11] = v256d_17;
    g_v256i[11] = v256i_32;
    g_v128f[11] = v128f_19;
    g_v128d[4] = p1;
    g_m16[11] = m16_4;
    __mmask8 m8_35 = (__mmask8)(g_m8[4] ^ 91);
    return m8_35;
}

__m512h torture_70(__m256d p0, __m512i p1, __m128i p2, __m512h p3) {
    __m256i v256i_1 = _mm512_cvtepi64_epi32(p1);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512i v512i_3 = _mm512_cvtps_epi32(v512f_2);
    __m512i v512i_4 = _mm512_sub_epi16(p1, p1);
    __m512d v512d_5 = g_v512d[8];
    __m128h v128h_6 = _mm512_cvtpd_ph(v512d_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[10] ^ 249);
    __mmask16 m16_8 = _kshiftri_mask16(m16_7, 1);
    __m128 v128f_9 = _mm512_extractf32x4_ps(v512f_2, 2);
    __mmask16 m16_10 = _kor_mask16(m16_7, m16_7);
    __m256 v256f_11 = _mm256_loadu_ps(fbuf);
    __m256 v256f_12 = _mm256_mul_ps(v256f_11, v256f_11);
    __m512 v512f_13 = _mm512_and_ps(v512f_2, v512f_2);
    __m256i v256i_14 = _mm256_abs_epi32(v256i_1);
    __m512i v512i_15 = _mm512_unpacklo_epi32(v512i_3, p1);
    __mmask16 m16_16 = _mm512_testn_epi32_mask(v512i_15, v512i_3);
    __m256 v256f_17 = _mm256_set1_ps(1.0f);
    __m128d v128d_18 = g_v128d[10];
    __m512d v512d_19 = _mm512_insertf64x2(v512d_5, v128d_18, 3);
    __m256d v256d_20 = _mm256_max_pd(p0, p0);
    if (((unsigned long long)m16_16) & 1ULL) {
    __mmask16 m16_21 = _mm512_test_epi32_mask(p1, v512i_3);
    __m128i v128i_22 = _mm_sub_epi16(p2, p2);
    __m256 v256f_23 = _mm256_add_ps(v256f_11, v256f_17);
    }
    g_v256d[4] = v256d_20;
    g_v128f[10] = v128f_9;
    g_v128i[2] = p2;
    g_v128h[1] = v128h_6;
    return p3;
}

__m128d torture_71(__m256i p0, __m512i p1, __m128h p2, __m512d p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[3] ^ 78);
    __mmask16 m16_2 = _kand_mask16(m16_1, m16_1);
    __m512i v512i_3 = _mm512_broadcastmw_epi32(m16_1);
    __m512 v512f_4 = g_v512f[9];
    __m512 v512f_5 = _mm512_and_ps(v512f_4, v512f_4);
    __m256 v256f_6 = _mm256_loadu_ps(fbuf);
    __m256 v256f_7 = _mm256_xor_ps(v256f_6, v256f_6);
    __mmask8 m8_8 = (__mmask8)(g_m8[7] ^ 120);
    __mmask8 m8_9 = _kand_mask8(m8_8, m8_8);
    __m512d v512d_10 = _mm512_cvtph_pd(p2);
    __m256i v256i_11 = _mm256_set1_epi32(0x55);
    __m512d v512d_12 = _mm512_cvtph_pd(p2);
    __m512i v512i_13 = _mm512_permutexvar_epi32(p1, p1);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m256i v256i_14 = _mm256_sllv_epi32(p0, p0);
    __m128 v128f_15 = g_v128f[8];
    __m128 v128f_16 = _mm_and_ps(v128f_15, v128f_15);
    __m512d v512d_17 = _mm512_cvtps_pd(v256f_6);
    __m512d v512d_18 = _mm512_div_pd(v512d_10, v512d_10);
    __m256i v256i_19 = _mm256_unpacklo_epi32(p0, v256i_14);
    }
    __m512i v512i_20 = _mm512_mask_add_epi32(p1, m16_2, v512i_3, v512i_3);
    __mmask16 m16_21 = _mm512_fpclass_ps_mask(v512f_5, 0x3);
    __m512 v512f_22 = _mm512_unpacklo_ps(v512f_5, v512f_4);
    __mmask8 m8_23 = _kand_mask8(m8_8, m8_9);
    __m512i v512i_24 = _mm512_abs_epi32(v512i_20);
    __m128 v128f_25 = _mm_loadu_ps(fbuf);
    __m128 v128f_26 = _mm_and_ps(v128f_25, v128f_25);
    __m256d v256d_27 = g_v256d[7];
    __m256d v256d_28 = _mm256_div_pd(v256d_27, v256d_27);
    __m128d v128d_29 = g_v128d[11];
    __m128d v128d_30 = _mm_mul_pd(v128d_29, v128d_29);
    __m128i v128i_31 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_32 = _mm_or_si128(v128i_31, v128i_31);
    if (((unsigned long long)m16_2) & 1ULL) {
    __mmask64 m64_33 = _mm512_cmp_epi8_mask(v512i_24, v512i_20, 1);
    __m128i v128i_34 = _mm_shuffle_epi8(v128i_32, v128i_31);
    __m128i v128i_35 = _mm_set1_epi32(0x55);
    __m128i v128i_36 = _mm_and_si128(v128i_32, v128i_31);
    __m512h v512h_37 = g_v512h[4];
    __m512h v512h_38 = _mm512_fmadd_ph(v512h_37, v512h_37, v512h_37);
    __m256 v256f_39 = _mm256_xor_ps(v256f_7, v256f_6);
    }
    g_v512f[6] = v512f_22;
    g_v512d[9] = p3;
    g_v512i[10] = v512i_20;
    g_v256d[6] = v256d_28;
    g_v128i[4] = v128i_31;
    return v128d_30;
}

__mmask8 torture_72(__m128i p0, __m256 p1, __m256 p2, __m128 p3, __m512h p4) {
    __m512d v512d_1 = g_v512d[6];
    __m512d v512d_2 = _mm512_max_pd(v512d_1, v512d_1);
    __m512 v512f_3 = g_v512f[5];
    __m512 v512f_4 = _mm512_add_ps(v512f_3, v512f_3);
    __m512i v512i_5 = g_v512i[7];
    __m512i v512i_6 = _mm512_sub_epi32(v512i_5, v512i_5);
    __m512 v512f_7 = _mm512_shuffle_ps(v512f_3, v512f_4, 0x1B);
    __m512 v512f_8 = _mm512_and_ps(v512f_7, v512f_7);
    __m256d v256d_9 = g_v256d[3];
    __m256d v256d_10 = _mm256_unpackhi_pd(v256d_9, v256d_9);
    __mmask16 m16_11 = (__mmask16)(g_m16[8] ^ 17);
    __m512i v512i_12 = _mm512_mask_add_epi32(v512i_6, m16_11, v512i_5, v512i_6);
    __m512 v512f_13 = _mm512_xor_ps(v512f_4, v512f_3);
    __mmask16 m16_14 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    __m512 v512f_15 = _mm512_insertf32x8(v512f_13, p1, 1);
    __mmask16 m16_16 = _kshiftli_mask16(m16_11, 2);
    __m256 v256f_17 = _mm256_set1_ps(1.0f);
    __m512i v512i_18 = _mm512_slli_epi32(v512i_5, 3);
    __mmask16 m16_19 = _mm512_cmp_ps_mask(v512f_8, v512f_13, _CMP_LT_OQ);
    unsigned u32_20 = _cvtmask16_u32(m16_11);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_16) & 7); i_21++) {
    __m256 v256f_22 = _mm256_unpacklo_ps(p1, v256f_17);
    __m128i v128i_23 = _mm_unpacklo_epi32(p0, p0);
    }
    __m256i v256i_24 = g_v256i[0];
    __m256i v256i_25 = _mm256_add_epi16(v256i_24, v256i_24);
    __m256i v256i_26 = _mm256_mullo_epi32(v256i_24, v256i_24);
    __m256i v256i_27 = _mm256_shuffle_epi8(v256i_24, v256i_26);
    __m512i v512i_28 = _mm512_madd52lo_epu64(v512i_12, v512i_12, v512i_6);
    __m512 v512f_29 = _mm512_roundscale_ps(v512f_13, 0x10);
    __m512i v512i_30 = _mm512_sub_epi64(v512i_5, v512i_12);
    g_v512i[9] = v512i_12;
    g_v256d[7] = v256d_9;
    g_v128f[7] = p3;
    g_v128i[2] = p0;
    g_v512h[3] = p4;
    g_m16[5] = m16_11;
    __mmask8 m8_31 = (__mmask8)(g_m8[1] ^ 227);
    return m8_31;
}

__m256 torture_73(__m256d p0, __m128h p1, __m512i p2, __m256 p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[9] ^ 40);
    __mmask16 m16_2 = _knot_mask16(m16_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_sllv_epi32(v128i_3, v128i_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_6 = _mm512_fpclass_ps_mask(v512f_5, 0x3);
    __m256d v256d_7 = _mm256_set1_pd(2.0);
    __m128i v128i_8 = _mm512_cvtepi32_epi8(p2);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_or_si256(v256i_9, v256i_9);
    __m256i v256i_11 = _mm256_shuffle_epi8(v256i_10, v256i_9);
    __m256 v256f_12 = _mm256_add_ps(p3, p3);
    __m256d v256d_13 = _mm256_min_pd(p0, p0);
    __m512d v512d_14 = _mm512_cvtps_pd(v256f_12);
    __m128d v128d_15 = g_v128d[8];
    __m128d v128d_16 = _mm_sub_pd(v128d_15, v128d_15);
    __mmask8 m8_17 = (__mmask8)(g_m8[8] ^ 196);
    __mmask8 m8_18 = _kand_mask8(m8_17, m8_17);
    __m128 v128f_19 = g_v128f[2];
    __m128 v128f_20 = _mm_fmsub_ps(v128f_19, v128f_19, v128f_19);
    __m256 v256f_21 = _mm256_unpacklo_ps(p3, p3);
    __m256 v256f_22 = _mm256_add_ps(p3, v256f_21);
    __m512 v512f_23 = _mm512_max_ps(v512f_5, v512f_5);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m512i v512i_24 = _mm512_shuffle_i32x4(p2, p2, 0x1B);
    __mmask16 m16_25 = _kor_mask16(m16_2, m16_1);
    __m128i v128i_26 = _mm_and_si128(v128i_3, v128i_3);
    __m128i v128i_27 = _mm_unpacklo_epi32(v128i_8, v128i_26);
    }
    __m128 v128f_28 = _mm512_extractf32x4_ps(v512f_23, 2);
    __m512i v512i_29 = _mm512_slli_epi32(p2, 3);
    __m128 v128f_30 = _mm_shuffle_ps(v128f_20, v128f_19, 0x1B);
    __m512d v512d_31 = _mm512_mask_blend_pd(m8_17, v512d_14, v512d_14);
    __m256 v256f_32 = _mm256_sqrt_ps(v256f_22);
    __m512i v512i_33 = _mm512_shuffle_epi8(v512i_29, p2);
    g_v512f[7] = v512f_5;
    g_v512d[10] = v512d_14;
    g_v256f[3] = v256f_22;
    g_v256d[2] = v256d_13;
    g_v128d[8] = v128d_16;
    g_v128i[2] = v128i_3;
    return v256f_12;
}

__mmask8 torture_74(__m512 p0, __m512 p1, __m128h p2, __m128h p3, __m256 p4) {
    __m128d v128d_1 = g_v128d[5];
    __m128d v128d_2 = _mm_div_pd(v128d_1, v128d_1);
    __m256i v256i_3 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_4 = _mm256_sub_epi32(v256i_3, v256i_3);
    __m256 v256f_5 = _mm256_sub_ps(p4, p4);
    __m128 v128f_6 = g_v128f[5];
    __m128 v128f_7 = _mm_div_ps(v128f_6, v128f_6);
    __m128i v128i_8 = _mm_set1_epi32(0x55);
    __m128i v128i_9 = _mm_add_epi8(v128i_8, v128i_8);
    __mmask32 m32_10 = (__mmask32)(g_m32[1] ^ 86);
    __mmask32 m32_11 = _kxor_mask32(m32_10, m32_10);
    __m256 v256f_12 = _mm256_xor_ps(v256f_5, p4);
    __m512 v512f_13 = _mm512_rcp14_ps(p1);
    __m128d v128d_14 = _mm_set1_pd(2.0);
    __m512 v512f_15 = _mm512_rsqrt14_ps(p0);
    __m128d v128d_16 = _mm_unpackhi_pd(v128d_2, v128d_2);
    __m512 v512f_17 = _mm512_fmsub_ps(p1, p1, v512f_13);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m32_11) & 7); i_18++) {
    __m512d v512d_19 = g_v512d[1];
    __m128h v128h_20 = _mm512_cvtpd_ph(v512d_19);
    __m256i v256i_21 = _mm256_abs_epi32(v256i_3);
    __m128i v128i_22 = _mm_add_epi32(v128i_8, v128i_9);
    __m256i v256i_23 = _mm256_sllv_epi32(v256i_4, v256i_4);
    __m512d v512d_24 = _mm512_fmadd_pd(v512d_19, v512d_19, v512d_19);
    }
    unsigned u32_25 = 89;
    __mmask16 m16_26 = _cvtu32_mask16(u32_25);
    __m512 v512f_27 = _mm512_sub_ps(v512f_17, v512f_17);
    __m256d v256d_28 = g_v256d[3];
    __m256d v256d_29 = _mm256_add_pd(v256d_28, v256d_28);
    __m512i v512i_30 = _mm512_cvtps_epi32(v512f_15);
    g_v512f[1] = p1;
    g_v256d[7] = v256d_28;
    g_m32[4] = m32_11;
    __mmask8 m8_31 = (__mmask8)(g_m8[1] ^ 142);
    return m8_31;
}

__m128i torture_75(__m512i p0, __m128i p1, __m128h p2) {
    __m512i v512i_1 = _mm512_set1_epi32(0x55);
    __m128i v128i_2 = _mm_add_epi32(p1, p1);
    __m128d v128d_3 = g_v128d[3];
    __m128d v128d_4 = _mm_sub_pd(v128d_3, v128d_3);
    __m128d v128d_5 = _mm_max_pd(v128d_3, v128d_4);
    __m256d v256d_6 = g_v256d[3];
    __m256d v256d_7 = _mm256_max_pd(v256d_6, v256d_6);
    __m256i v256i_8 = g_v256i[1];
    __m256i v256i_9 = _mm256_sub_epi16(v256i_8, v256i_8);
    __m512 v512f_10 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_11 = _mm512_cmp_ps_mask(v512f_10, v512f_10, _CMP_LT_OQ);
    __mmask16 m16_12 = _kshiftri_mask16(m16_11, 1);
    __m512i v512i_13 = _mm512_or_si512(v512i_1, v512i_1);
    __m512h v512h_14 = g_v512h[0];
    __m512h v512h_15 = _mm512_add_ph(v512h_14, v512h_14);
    __m512i v512i_16 = _mm512_cvtps_epi32(v512f_10);
    __m512i v512i_17 = _mm512_or_si512(v512i_16, v512i_16);
    __m256i v256i_18 = _mm256_mullo_epi32(v256i_8, v256i_8);
    __m128i v128i_19 = _mm_xor_si128(p1, p1);
    __m512 v512f_20 = _mm512_cvtepi32_ps(v512i_13);
    __m512i v512i_21 = _mm512_shuffle_i32x4(v512i_16, v512i_16, 0x1B);
    __m512d v512d_22 = g_v512d[4];
    __m256d v256d_23 = _mm512_extractf64x4_pd(v512d_22, 1);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_11) & 7); i_24++) {
    __m512h v512h_25 = _mm512_add_ph(v512h_14, v512h_14);
    __m512 v512f_26 = _mm512_shuffle_f32x4(v512f_20, v512f_10, 0x4E);
    __mmask16 m16_27 = _mm512_fpclass_ps_mask(v512f_10, 0x3);
    }
    __m128i v128i_28 = _mm_unpacklo_epi32(v128i_19, v128i_19);
    __m256 v256f_29 = g_v256f[5];
    __m256 v256f_30 = _mm256_min_ps(v256f_29, v256f_29);
    __mmask16 m16_31 = _mm512_cmp_epu32_mask(v512i_1, v512i_21, 4);
    __m256 v256f_32 = _mm256_sub_ps(v256f_30, v256f_29);
    __m512i v512i_33 = _mm512_and_si512(v512i_1, p0);
    __m512i v512i_34 = _mm512_permutex2var_epi32(v512i_13, v512i_33, v512i_1);
    __m128d v128d_35 = _mm_div_pd(v128d_4, v128d_5);
    __m256i v256i_36 = _mm256_add_epi64(v256i_18, v256i_8);
    __m512i v512i_37 = _mm512_sub_epi8(p0, v512i_1);
    if (((unsigned long long)m16_12) & 1ULL) {
    __mmask8 m8_38 = (__mmask8)(g_m8[9] ^ 95);
    __m512i v512i_39 = _mm512_broadcastmb_epi64(m8_38);
    __m512 v512f_40 = _mm512_maskz_mul_ps(m16_12, v512f_10, v512f_10);
    __m256d v256d_41 = _mm256_unpackhi_pd(v256d_6, v256d_6);
    __m128 v128f_42 = _mm_loadu_ps(fbuf);
    __m128 v128f_43 = _mm_add_ps(v128f_42, v128f_42);
    }
    g_v512d[2] = v512d_22;
    g_v256d[5] = v256d_7;
    return v128i_19;
}

__m128 torture_76(__m256d p0, __m512 p1) {
    __m512 v512f_1 = _mm512_sqrt_ps(p1);
    __m512 v512f_2 = _mm512_sub_ps(v512f_1, p1);
    __m512i v512i_3 = g_v512i[7];
    __m512i v512i_4 = _mm512_lzcnt_epi32(v512i_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[4] ^ 213);
    __m512i v512i_6 = _mm512_mask_blend_epi32(m16_5, v512i_3, v512i_3);
    __m256d v256d_7 = _mm256_add_pd(p0, p0);
    __m512 v512f_8 = _mm512_mask_blend_ps(m16_5, v512f_2, v512f_1);
    __m256d v256d_9 = _mm256_set1_pd(2.0);
    __m512 v512f_10 = _mm512_sqrt_ps(v512f_8);
    __m256i v256i_11 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_12 = _mm256_mullo_epi32(v256i_11, v256i_11);
    __m512 v512f_13 = _mm512_fmadd_ps(v512f_8, p1, p1);
    __m256 v256f_14 = g_v256f[3];
    __m256 v256f_15 = _mm256_sub_ps(v256f_14, v256f_14);
    __m128d v128d_16 = g_v128d[9];
    __m128d v128d_17 = _mm_div_pd(v128d_16, v128d_16);
    __m512i v512i_18 = _mm512_abs_epi32(v512i_3);
    __m256 v256f_19 = _mm256_xor_ps(v256f_14, v256f_15);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_5) & 7); i_20++) {
    __m512d v512d_21 = g_v512d[3];
    __m512d v512d_22 = _mm512_mul_pd(v512d_21, v512d_21);
    __m512i v512i_23 = _mm512_add_epi64(v512i_4, v512i_6);
    __m128 v128f_24 = g_v128f[11];
    __m128 v128f_25 = _mm_mul_ps(v128f_24, v128f_24);
    __m128d v128d_26 = _mm_unpackhi_pd(v128d_17, v128d_17);
    }
    __m512h v512h_27 = g_v512h[3];
    __m512h v512h_28 = _mm512_getexp_ph(v512h_27);
    __m512d v512d_29 = _mm512_loadu_pd(dbuf);
    __m512d v512d_30 = _mm512_max_pd(v512d_29, v512d_29);
    __m128i v128i_31 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_32 = _mm512_cvtepi8_epi32(v128i_31);
    __m512 v512f_33 = _mm512_mul_ps(v512f_10, v512f_10);
    __m256i v256i_34 = _mm256_shuffle_epi8(v256i_11, v256i_12);
    __m512i v512i_35 = _mm512_dpwssd_epi32(v512i_6, v512i_6, v512i_4);
    __m512h v512h_36 = _mm512_getexp_ph(v512h_28);
    __m256i v256i_37 = _mm256_abs_epi32(v256i_34);
    __m512 v512f_38 = _mm512_getexp_ps(v512f_2);
    g_v512f[11] = v512f_1;
    g_v512i[7] = v512i_4;
    g_v256f[0] = v256f_15;
    g_v256d[6] = v256d_7;
    g_v256i[9] = v256i_11;
    g_v128d[6] = v128d_16;
    g_m16[6] = m16_5;
    __m128 v128f_39 = g_v128f[5];
    return v128f_39;
}

__m512d torture_77(__m128d p0, __m128i p1, __m128h p2, __m256i p3) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_sub_ps(v512f_1, v512f_1);
    __mmask32 m32_3 = (__mmask32)(g_m32[0] ^ 9);
    __mmask32 m32_4 = _kxor_mask32(m32_3, m32_3);
    __m512h v512h_5 = g_v512h[11];
    __m512h v512h_6 = _mm512_sqrt_ph(v512h_5);
    __m128i v128i_7 = _mm_add_epi8(p1, p1);
    __m512d v512d_8 = _mm512_loadu_pd(dbuf);
    __m128h v128h_9 = _mm512_cvtpd_ph(v512d_8);
    __m128 v128f_10 = _mm_loadu_ps(fbuf);
    __m128 v128f_11 = _mm_sqrt_ps(v128f_10);
    __m128 v128f_12 = _mm_unpacklo_ps(v128f_11, v128f_10);
    __m128d v128d_13 = _mm_set1_pd(2.0);
    __m256 v256f_14 = g_v256f[3];
    __m256 v256f_15 = _mm256_div_ps(v256f_14, v256f_14);
    __m512i v512i_16 = g_v512i[5];
    __m512i v512i_17 = _mm512_add_epi8(v512i_16, v512i_16);
    __m256 v256f_18 = _mm256_div_ps(v256f_15, v256f_15);
    __mmask16 m16_19 = _mm512_testn_epi32_mask(v512i_17, v512i_16);
    __m512i v512i_20 = _mm512_conflict_epi32(v512i_16);
    __m512 v512f_21 = _mm512_maskz_mul_ps(m16_19, v512f_2, v512f_1);
    __m512h v512h_22 = _mm512_fmul_pch(v512h_5, v512h_5);
    __m256 v256f_23 = _mm256_fnmadd_ps(v256f_18, v256f_15, v256f_15);
    __mmask64 m64_24 = (__mmask64)(g_m64[0] ^ 83);
    __mmask64 m64_25 = _kor_mask64(m64_24, m64_24);
    __m512d v512d_26 = _mm512_max_pd(v512d_8, v512d_8);
    if (((unsigned long long)m16_19) & 1ULL) {
    __mmask16 m16_27 = _kxor_mask16(m16_19, m16_19);
    __m512i v512i_28 = _mm512_set1_epi32(0x55);
    __m128i v128i_29 = _mm_sub_epi8(p1, p1);
    __m512i v512i_30 = _mm512_add_epi16(v512i_20, v512i_20);
    }
    __mmask64 m64_31 = _mm512_movepi8_mask(v512i_17);
    __m256 v256f_32 = _mm256_fnmadd_ps(v256f_23, v256f_15, v256f_14);
    __m128d v128d_33 = _mm_fmadd_pd(p0, v128d_13, v128d_13);
    __m128i v128i_34 = _mm_sub_epi8(v128i_7, p1);
    __m128i v128i_35 = _mm_add_epi64(v128i_7, v128i_7);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_19) & 7); i_36++) {
    __m512h v512h_37 = _mm512_getexp_ph(v512h_6);
    __m512i v512i_38 = _mm512_add_epi8(v512i_16, v512i_17);
    __mmask32 m32_39 = _mm512_kunpackw(m16_19, m16_19);
    __m128d v128d_40 = _mm_sub_pd(v128d_13, v128d_13);
    __m256 v256f_41 = _mm256_sqrt_ps(v256f_15);
    }
    g_v256i[4] = p3;
    g_v128h[4] = p2;
    g_m16[9] = m16_19;
    g_m32[7] = m32_4;
    g_m64[5] = m64_31;
    return v512d_8;
}

__m256h torture_78(__m128i p0, __m512i p1, __m128 p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_sqrt_ps(v512f_1);
    __m512 v512f_3 = _mm512_fnmadd_ps(v512f_2, v512f_2, v512f_1);
    __m128i v128i_4 = _mm_set1_epi32(0x55);
    __m128i v128i_5 = _mm_and_si128(v128i_4, v128i_4);
    __m512i v512i_6 = _mm512_lzcnt_epi32(p1);
    __m128d v128d_7 = g_v128d[10];
    __m128d v128d_8 = _mm_div_pd(v128d_7, v128d_7);
    __m256d v256d_9 = g_v256d[7];
    __m256d v256d_10 = _mm256_sqrt_pd(v256d_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_fmadd_pd(v512d_11, v512d_11, v512d_11);
    __m512d v512d_13 = _mm512_mul_pd(v512d_12, v512d_11);
    __m512d v512d_14 = _mm512_sub_pd(v512d_13, v512d_12);
    __m256i v256i_15 = g_v256i[2];
    __m256i v256i_16 = _mm256_abs_epi32(v256i_15);
    __m128i v128i_17 = _mm_add_epi32(p0, v128i_4);
    int s32_18 = 29;
    if ((s32_18) & 1ULL) {
    __m256i v256i_19 = _mm256_or_si256(v256i_16, v256i_16);
    __m128i v128i_20 = _mm_shuffle_epi8(v128i_17, v128i_5);
    __m512 v512f_21 = _mm512_min_ps(v512f_2, v512f_2);
    __m128i v128i_22 = _mm_sub_epi16(v128i_20, v128i_17);
    __mmask16 m16_23 = (__mmask16)(g_m16[9] ^ 56);
    __mmask16 m16_24 = _kshiftli_mask16(m16_23, 2);
    }
    __m512h v512h_25 = g_v512h[6];
    __m512h v512h_26 = _mm512_add_ph(v512h_25, v512h_25);
    __m256 v256f_27 = g_v256f[7];
    __m256 v256f_28 = _mm256_sub_ps(v256f_27, v256f_27);
    __m256i v256i_29 = _mm512_cvtepi64_epi32(p1);
    __m256 v256f_30 = _mm256_unpacklo_ps(v256f_27, v256f_28);
    __mmask32 m32_31 = _mm512_cmp_ph_mask(v512h_25, v512h_26, 1);
    __m512d v512d_32 = _mm512_mul_pd(v512d_13, v512d_11);
    __m512i v512i_33 = _mm512_add_epi32(v512i_6, p1);
    __mmask16 m16_34 = _mm512_cmp_epi32_mask(p1, v512i_6, 2);
    __m512i v512i_35 = _mm512_or_si512(p1, v512i_6);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_34) & 7); i_36++) {
    __m512 v512f_37 = _mm512_div_ps(v512f_2, v512f_3);
    __m256i v256i_38 = _mm256_sub_epi64(v256i_16, v256i_15);
    }
    g_v512d[0] = v512d_13;
    g_v256d[11] = v256d_10;
    g_v256i[9] = v256i_16;
    g_v128f[7] = p2;
    g_v128i[6] = v128i_5;
    g_v512h[7] = v512h_26;
    g_m16[11] = m16_34;
    g_m32[2] = m32_31;
    __m256h v256h_39 = g_v256h[5];
    return v256h_39;
}

__m512 torture_79(__m512i p0, __m256 p1) {
    __m512 v512f_1 = g_v512f[6];
    __m512 v512f_2 = _mm512_rsqrt14_ps(v512f_1);
    __mmask8 m8_3 = (__mmask8)(g_m8[2] ^ 34);
    __mmask8 m8_4 = _kand_mask8(m8_3, m8_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_xor_si128(v128i_5, v128i_5);
    __m128 v128f_7 = _mm_loadu_ps(fbuf);
    __m128 v128f_8 = _mm_shuffle_ps(v128f_7, v128f_7, 0x1B);
    __mmask16 m16_9 = (__mmask16)(g_m16[9] ^ 228);
    __mmask16 m16_10 = _kshiftli_mask16(m16_9, 2);
    __m512i v512i_11 = _mm512_xor_si512(p0, p0);
    __m256i v256i_12 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_13 = _mm256_and_si256(v256i_12, v256i_12);
    __m512d v512d_14 = g_v512d[9];
    __m128h v128h_15 = _mm512_cvtpd_ph(v512d_14);
    __m128i v128i_16 = _mm_sub_epi32(v128i_5, v128i_5);
    __m256 v256f_17 = _mm256_fmadd_ps(p1, p1, p1);
    __m128d v128d_18 = g_v128d[10];
    __m512d v512d_19 = _mm512_insertf64x2(v512d_14, v128d_18, 3);
    __m512 v512f_20 = _mm512_rcp14_ps(v512f_1);
    __m256i v256i_21 = _mm256_slli_epi32(v256i_12, 3);
    __m128d v128d_22 = _mm_fmadd_pd(v128d_18, v128d_18, v128d_18);
    g_v512d[11] = v512d_14;
    g_v128d[3] = v128d_18;
    g_m16[2] = m16_10;
    return v512f_2;
}

__m128i torture_80(__m128i p0, __m128h p1, __m256i p2, __m512h p3, __m256i p4) {
    __m512h v512h_1 = _mm512_getexp_ph(p3);
    __m128d v128d_2 = g_v128d[3];
    __m128d v128d_3 = _mm_fmadd_pd(v128d_2, v128d_2, v128d_2);
    __m128i v128i_4 = _mm_sllv_epi32(p0, p0);
    __m256 v256f_5 = _mm256_loadu_ps(fbuf);
    __m256 v256f_6 = _mm256_max_ps(v256f_5, v256f_5);
    __m256i v256i_7 = _mm256_set1_epi32(0x55);
    __m128i v128i_8 = _mm_or_si128(p0, p0);
    __m256i v256i_9 = _mm256_add_epi32(v256i_7, v256i_7);
    __m256d v256d_10 = g_v256d[7];
    __m256d v256d_11 = _mm256_max_pd(v256d_10, v256d_10);
    __m512i v512i_12 = g_v512i[2];
    __m512i v512i_13 = _mm512_and_si512(v512i_12, v512i_12);
    __m512i v512i_14 = _mm512_permutexvar_epi32(v512i_13, v512i_13);
    __mmask16 m16_15 = (__mmask16)(g_m16[10] ^ 235);
    __mmask16 m16_16 = _kxor_mask16(m16_15, m16_15);
    __m512 v512f_17 = _mm512_loadu_ps(fbuf);
    __m512 v512f_18 = _mm512_scalef_ps(v512f_17, v512f_17);
    __m512i v512i_19 = _mm512_rol_epi32(v512i_14, 5);
    __m128 v128f_20 = g_v128f[8];
    __m128 v128f_21 = _mm_and_ps(v128f_20, v128f_20);
    __m128 v128f_22 = _mm_sub_ps(v128f_21, v128f_21);
    __m512i v512i_23 = _mm512_dpwssd_epi32(v512i_14, v512i_19, v512i_14);
    __m256d v256d_24 = _mm256_max_pd(v256d_10, v256d_11);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_16) & 7); i_25++) {
    __m512i v512i_26 = _mm512_popcnt_epi64(v512i_23);
    __m512d v512d_27 = g_v512d[7];
    __m512d v512d_28 = _mm512_fmadd_pd(v512d_27, v512d_27, v512d_27);
    __m512d v512d_29 = _mm512_max_pd(v512d_27, v512d_27);
    }
    __m128 v128f_30 = _mm_min_ps(v128f_21, v128f_22);
    __m512 v512f_31 = _mm512_shuffle_f32x4(v512f_17, v512f_17, 0x4E);
    __m512h v512h_32 = _mm512_add_ph(p3, p3);
    __m256 v256f_33 = _mm256_sqrt_ps(v256f_5);
    __m512i v512i_34 = _mm512_dpbusd_epi32(v512i_19, v512i_13, v512i_19);
    __m128d v128d_35 = _mm_max_pd(v128d_3, v128d_2);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512 v512f_36 = _mm512_scalef_ps(v512f_18, v512f_17);
    __m512i v512i_37 = _mm512_permutexvar_epi32(v512i_12, v512i_12);
    __m128 v128f_38 = _mm512_extractf32x4_ps(v512f_17, 2);
    __m512 v512f_39 = _mm512_min_ps(v512f_36, v512f_18);
    __m512d v512d_40 = g_v512d[2];
    __m512d v512d_41 = _mm512_sqrt_pd(v512d_40);
    }
    g_v512f[6] = v512f_31;
    g_v512i[10] = v512i_14;
    g_v128f[5] = v128f_22;
    g_v512h[1] = v512h_1;
    g_v128h[10] = p1;
    g_m16[2] = m16_15;
    return v128i_8;
}

__mmask8 torture_81(__m256 p0, __m128 p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_2 = _mm512_testn_epi32_mask(v512i_1, v512i_1);
    __m128d v128d_3 = g_v128d[1];
    __m128d v128d_4 = _mm_sub_pd(v128d_3, v128d_3);
    __m128 v128f_5 = _mm_fmadd_ps(p1, p1, p1);
    __m256 v256f_6 = _mm256_xor_ps(p0, p0);
    __m512 v512f_7 = g_v512f[5];
    __m512 v512f_8 = _mm512_fmsub_ps(v512f_7, v512f_7, v512f_7);
    __m512i v512i_9 = _mm512_slli_epi32(v512i_1, 3);
    __m256 v256f_10 = _mm256_unpacklo_ps(p0, v256f_6);
    __m256 v256f_11 = _mm256_shuffle_ps(v256f_10, p0, 0x1B);
    __m128i v128i_12 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_13 = _mm_and_si128(v128i_12, v128i_12);
    __m128d v128d_14 = _mm_add_pd(v128d_3, v128d_3);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m256i v256i_15 = g_v256i[1];
    __m256i v256i_16 = _mm256_unpacklo_epi32(v256i_15, v256i_15);
    __m128i v128i_17 = _mm_xor_si128(v128i_12, v128i_12);
    __m256i v256i_18 = _mm256_add_epi16(v256i_16, v256i_15);
    __m512i v512i_19 = _mm512_popcnt_epi64(v512i_9);
    }
    __m512i v512i_20 = _mm512_slli_epi32(v512i_9, 3);
    __mmask64 m64_21 = _mm512_movepi8_mask(v512i_9);
    __m512 v512f_22 = _mm512_max_ps(v512f_8, v512f_8);
    __m512 v512f_23 = _mm512_fmsub_ps(v512f_22, v512f_7, v512f_7);
    __m512 v512f_24 = _mm512_unpacklo_ps(v512f_22, v512f_8);
    __m512i v512i_25 = _mm512_broadcastmw_epi32(m16_2);
    __m256i v256i_26 = g_v256i[3];
    __m256i v256i_27 = _mm256_and_si256(v256i_26, v256i_26);
    __m256 v256f_28 = _mm256_min_ps(p0, v256f_10);
    __m512i v512i_29 = _mm512_or_si512(v512i_9, v512i_1);
    __m256 v256f_30 = _mm256_fmsub_ps(v256f_6, v256f_6, v256f_6);
    __m512h v512h_31 = g_v512h[3];
    __m512h v512h_32 = _mm512_rcp_ph(v512h_31);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m256i v256i_33 = _mm256_sub_epi16(v256i_27, v256i_26);
    __mmask64 m64_34 = _mm512_movepi8_mask(v512i_29);
    __m512i v512i_35 = _mm512_shuffle_epi8(v512i_25, v512i_29);
    __m256h v256h_36 = _mm512_cvtxps_ph(v512f_24);
    __mmask32 m32_37 = (__mmask32)(g_m32[2] ^ 237);
    __mmask32 m32_38 = _kxor_mask32(m32_37, m32_37);
    __m128 v128f_39 = _mm_unpacklo_ps(v128f_5, p1);
    }
    g_v256i[1] = v256i_27;
    g_v128d[2] = v128d_4;
    g_m16[3] = m16_2;
    g_m64[6] = m64_21;
    __mmask8 m8_40 = (__mmask8)(g_m8[4] ^ 11);
    return m8_40;
}

__m512d torture_82(__m128d p0, __m256h p1, __m512d p2, __m128 p3, __m512i p4) {
    __m256 v256f_1 = g_v256f[0];
    __m256 v256f_2 = _mm256_xor_ps(v256f_1, v256f_1);
    __m256i v256i_3 = g_v256i[3];
    __m256i v256i_4 = _mm256_add_epi8(v256i_3, v256i_3);
    __m512 v512f_5 = g_v512f[10];
    __mmask16 m16_6 = _mm512_fpclass_ps_mask(v512f_5, 0x3);
    __m128i v128i_7 = g_v128i[10];
    __m128i v128i_8 = _mm_xor_si128(v128i_7, v128i_7);
    __m128i v128i_9 = _mm_set1_epi32(0x55);
    __mmask16 m16_10 = _kxor_mask16(m16_6, m16_6);
    __m128 v128f_11 = _mm_sqrt_ps(p3);
    __m512i v512i_12 = _mm512_cvtpd_epi64(p2);
    __m512 v512f_13 = _mm512_mask3_fmadd_ps(v512f_5, v512f_5, v512f_5, m16_10);
    __m512 v512f_14 = _mm512_add_ps(v512f_13, v512f_5);
    __m256 v256f_15 = _mm256_fmsub_ps(v256f_2, v256f_1, v256f_2);
    __m512h v512h_16 = g_v512h[3];
    __m512h v512h_17 = _mm512_fmul_pch(v512h_16, v512h_16);
    __m512d v512d_18 = _mm512_cvtps_pd(v256f_1);
    __m512h v512h_19 = _mm512_fmadd_ph(v512h_17, v512h_17, v512h_16);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_6) & 7); i_20++) {
    __m128i v128i_21 = _mm_sllv_epi32(v128i_9, v128i_9);
    __m128d v128d_22 = _mm_add_pd(p0, p0);
    __m128 v128f_23 = _mm_fmadd_ps(v128f_11, p3, v128f_11);
    __m512i v512i_24 = _mm512_cvtps_epi32(v512f_5);
    }
    __m128i v128i_25 = _mm_sub_epi64(v128i_9, v128i_8);
    __m256 v256f_26 = _mm256_fmadd_ps(v256f_2, v256f_15, v256f_1);
    __m256 v256f_27 = _mm256_fmadd_ps(v256f_26, v256f_2, v256f_26);
    __m512 v512f_28 = _mm512_shuffle_ps(v512f_5, v512f_5, 0x1B);
    __m512h v512h_29 = _mm512_getexp_ph(v512h_16);
    __m512i v512i_30 = _mm512_sllv_epi32(v512i_12, p4);
    g_v512f[10] = v512f_5;
    g_v512d[3] = v512d_18;
    g_v512i[5] = v512i_12;
    g_v256f[4] = v256f_15;
    g_v256i[9] = v256i_3;
    g_v128f[0] = p3;
    g_m16[6] = m16_10;
    return p2;
}

__mmask16 torture_83(__m128h p0, __m256i p1, __m128 p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask64 m64_2 = _mm512_cmp_epi8_mask(v512i_1, v512i_1, 1);
    __m256 v256f_3 = _mm256_set1_ps(1.0f);
    __mmask8 m8_4 = (__mmask8)(g_m8[10] ^ 185);
    __m512d v512d_5 = g_v512d[11];
    __m512d v512d_6 = _mm512_mask_blend_pd(m8_4, v512d_5, v512d_5);
    __m512d v512d_7 = _mm512_add_pd(v512d_5, v512d_6);
    __mmask16 m16_8 = (__mmask16)(g_m16[0] ^ 147);
    __mmask16 m16_9 = _kor_mask16(m16_8, m16_8);
    __m128i v128i_10 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_11 = _mm_add_epi64(v128i_10, v128i_10);
    __m512 v512f_12 = _mm512_loadu_ps(fbuf);
    __m512 v512f_13 = _mm512_sqrt_ps(v512f_12);
    __m512h v512h_14 = g_v512h[1];
    __m512h v512h_15 = _mm512_fmadd_ph(v512h_14, v512h_14, v512h_14);
    __mmask16 m16_16 = _kshiftri_mask16(m16_8, 1);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_8) & 7); i_17++) {
    __m512h v512h_18 = _mm512_sqrt_ph(v512h_14);
    __m256d v256d_19 = g_v256d[3];
    __m256d v256d_20 = _mm256_add_pd(v256d_19, v256d_19);
    }
    __m256d v256d_21 = g_v256d[6];
    __m256d v256d_22 = _mm256_max_pd(v256d_21, v256d_21);
    __m512h v512h_23 = _mm512_fmsub_ph(v512h_14, v512h_15, v512h_14);
    __m512i v512i_24 = _mm512_shuffle_i32x4(v512i_1, v512i_1, 0x1B);
    __m512 v512f_25 = _mm512_fnmadd_ps(v512f_13, v512f_12, v512f_12);
    __m128i v128i_26 = _mm_sllv_epi32(v128i_11, v128i_10);
    __mmask16 m16_27 = _mm512_cmp_ps_mask(v512f_13, v512f_13, _CMP_LT_OQ);
    __m256 v256f_28 = _mm256_add_ps(v256f_3, v256f_3);
    __m512i v512i_29 = _mm512_sub_epi32(v512i_1, v512i_24);
    __m512 v512f_30 = _mm512_rcp14_ps(v512f_13);
    __m512 v512f_31 = _mm512_broadcast_f32x4(p2);
    __m512i v512i_32 = _mm512_or_si512(v512i_29, v512i_1);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m256 v256f_33 = _mm256_fmadd_ps(v256f_28, v256f_3, v256f_28);
    __m512 v512f_34 = _mm512_unpacklo_ps(v512f_13, v512f_12);
    __m512i v512i_35 = _mm512_abs_epi32(v512i_24);
    __m128i v128i_36 = _mm_sub_epi16(v128i_10, v128i_10);
    }
    g_v256f[0] = v256f_28;
    g_v256d[9] = v256d_21;
    g_v256i[2] = p1;
    g_v128h[0] = p0;
    g_m8[9] = m8_4;
    return m16_8;
}

__m128d torture_84(__m256d p0, __m128i p1, __m128 p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_max_ps(v512f_1, v512f_1);
    __m128i v128i_3 = _mm_add_epi16(p1, p1);
    __m512i v512i_4 = g_v512i[2];
    __m512i v512i_5 = _mm512_unpacklo_epi32(v512i_4, v512i_4);
    __m256i v256i_6 = g_v256i[7];
    __m256i v256i_7 = _mm256_shuffle_epi8(v256i_6, v256i_6);
    __m512i v512i_8 = _mm512_add_epi8(v512i_5, v512i_4);
    __m512i v512i_9 = _mm512_permutex2var_epi32(v512i_5, v512i_8, v512i_8);
    __m256d v256d_10 = _mm256_fmadd_pd(p0, p0, p0);
    __m256 v256f_11 = g_v256f[5];
    __m256 v256f_12 = _mm256_and_ps(v256f_11, v256f_11);
    __mmask32 m32_13 = (__mmask32)(g_m32[6] ^ 215);
    __mmask32 m32_14 = _kxor_mask32(m32_13, m32_13);
    __m512d v512d_15 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_16 = _mm512_cmp_pd_mask(v512d_15, v512d_15, _CMP_GE_OQ);
    __m128 v128f_17 = _mm_sqrt_ps(p2);
    __m256i v256i_18 = _mm256_add_epi64(v256i_6, v256i_7);
    __m512i v512i_19 = _mm512_sub_epi8(v512i_8, v512i_5);
    __mmask16 m16_20 = (__mmask16)(g_m16[9] ^ 38);
    __mmask16 m16_21 = _kor_mask16(m16_20, m16_20);
    __m512i v512i_22 = _mm512_broadcastmw_epi32(m16_20);
    __m256i v256i_23 = _mm256_add_epi64(v256i_18, v256i_6);
    __m256d v256d_24 = _mm256_add_pd(v256d_10, p0);
    __m512h v512h_25 = g_v512h[10];
    __m512h v512h_26 = _mm512_sqrt_ph(v512h_25);
    __m256d v256d_27 = _mm256_sqrt_pd(v256d_24);
    __m256i v256i_28 = _mm256_add_epi16(v256i_23, v256i_18);
    __m256 v256f_29 = _mm256_add_ps(v256f_12, v256f_12);
    __m512 v512f_30 = _mm512_sub_ps(v512f_1, v512f_2);
    __mmask8 m8_31 = _kand_mask8(m8_16, m8_16);
    __mmask16 m16_32 = _mm512_cmp_epi32_mask(v512i_8, v512i_5, 2);
    __m512i v512i_33 = _mm512_and_si512(v512i_22, v512i_19);
    __m512i v512i_34 = _mm512_sub_epi8(v512i_4, v512i_19);
    __m512i v512i_35 = _mm512_madd52lo_epu64(v512i_5, v512i_5, v512i_8);
    if (((unsigned long long)m16_20) & 1ULL) {
    __m128d v128d_36 = g_v128d[6];
    __m128d v128d_37 = _mm_max_pd(v128d_36, v128d_36);
    __m128i v128i_38 = _mm_add_epi16(v128i_3, p1);
    __mmask16 m16_39 = _mm512_movepi32_mask(v512i_35);
    __m256h v256h_40 = g_v256h[11];
    __m512 v512f_41 = _mm512_cvtxph_ps(v256h_40);
    }
    g_v512i[9] = v512i_33;
    g_v256f[7] = v256f_29;
    g_v256d[4] = v256d_24;
    g_v128f[5] = p2;
    g_v128i[8] = p1;
    g_v512h[8] = v512h_26;
    g_m8[9] = m8_16;
    __m128d v128d_42 = g_v128d[5];
    return v128d_42;
}

__mmask64 torture_85(__m512i p0, __m512h p1, __m256i p2, __m128h p3, __m256 p4) {
    __mmask16 m16_1 = (__mmask16)(g_m16[4] ^ 216);
    unsigned u32_2 = _cvtmask16_u32(m16_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_sqrt_pd(v512d_3);
    __m512 v512f_5 = g_v512f[7];
    __m512 v512f_6 = _mm512_sub_ps(v512f_5, v512f_5);
    __m256i v256i_7 = _mm512_cvtepi64_epi32(p0);
    __m256i v256i_8 = _mm256_unpacklo_epi32(v256i_7, p2);
    __m512i v512i_9 = _mm512_or_si512(p0, p0);
    __m512h v512h_10 = _mm512_add_ph(p1, p1);
    __m256i v256i_11 = _mm256_shuffle_epi8(v256i_7, v256i_8);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m512 v512f_12 = _mm512_min_ps(v512f_5, v512f_5);
    __mmask16 m16_13 = _cvtu32_mask16(u32_2);
    __mmask16 m16_14 = _mm512_movepi32_mask(v512i_9);
    __mmask32 m32_15 = (__mmask32)(g_m32[4] ^ 238);
    __mmask32 m32_16 = _kxor_mask32(m32_15, m32_15);
    __m512i v512i_17 = _mm512_add_epi8(p0, v512i_9);
    }
    __mmask32 m32_18 = _mm512_cmp_ph_mask(v512h_10, v512h_10, 1);
    __m128 v128f_19 = _mm_loadu_ps(fbuf);
    __m128 v128f_20 = _mm_unpacklo_ps(v128f_19, v128f_19);
    __m512 v512f_21 = _mm512_sub_ps(v512f_5, v512f_5);
    __m128 v128f_22 = _mm_shuffle_ps(v128f_19, v128f_20, 0x1B);
    __m128i v128i_23 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_24 = _mm_sllv_epi32(v128i_23, v128i_23);
    __m128d v128d_25 = g_v128d[7];
    __m128d v128d_26 = _mm_sub_pd(v128d_25, v128d_25);
    __m128i v128i_27 = _mm_add_epi8(v128i_23, v128i_24);
    __m128i v128i_28 = _mm_unpacklo_epi32(v128i_24, v128i_24);
    __m512d v512d_29 = _mm512_mul_pd(v512d_3, v512d_3);
    __m512i v512i_30 = _mm512_add_epi16(v512i_9, p0);
    g_v512f[6] = v512f_5;
    g_v128f[5] = v128f_19;
    g_v128d[3] = v128d_26;
    g_v128h[4] = p3;
    g_m32[7] = m32_18;
    __mmask64 m64_31 = (__mmask64)(g_m64[7] ^ 124);
    return m64_31;
}

__m512d torture_86(__m128d p0, __m128d p1, __m256i p2, __m512 p3) {
    __m256 v256f_1 = g_v256f[2];
    __m256 v256f_2 = _mm256_fmsub_ps(v256f_1, v256f_1, v256f_1);
    __m256i v256i_3 = _mm256_sllv_epi32(p2, p2);
    __m256i v256i_4 = _mm256_unpacklo_epi32(v256i_3, v256i_3);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_6 = _mm512_mullo_epi32(v512i_5, v512i_5);
    __m128i v128i_7 = g_v128i[2];
    __m128i v128i_8 = _mm_slli_epi32(v128i_7, 3);
    __m512 v512f_9 = _mm512_rsqrt14_ps(p3);
    __m256d v256d_10 = g_v256d[4];
    __m256d v256d_11 = _mm256_min_pd(v256d_10, v256d_10);
    __mmask8 m8_12 = (__mmask8)(g_m8[2] ^ 113);
    __mmask8 m8_13 = _kand_mask8(m8_12, m8_12);
    __mmask32 m32_14 = (__mmask32)(g_m32[7] ^ 56);
    __mmask32 m32_15 = _kxor_mask32(m32_14, m32_14);
    __m512 v512f_16 = _mm512_unpacklo_ps(v512f_9, v512f_9);
    __m512 v512f_17 = _mm512_add_ps(v512f_9, v512f_16);
    __m256 v256f_18 = _mm256_min_ps(v256f_2, v256f_2);
    __m256i v256i_19 = _mm256_mullo_epi32(p2, v256i_3);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m8_13) & 7); i_20++) {
    __m256i v256i_21 = _mm256_sub_epi8(v256i_4, v256i_19);
    __m512h v512h_22 = g_v512h[3];
    __m512h v512h_23 = _mm512_fmul_pch(v512h_22, v512h_22);
    __m256 v256f_24 = _mm256_sqrt_ps(v256f_2);
    __m128 v128f_25 = _mm_loadu_ps(fbuf);
    __m128 v128f_26 = _mm_xor_ps(v128f_25, v128f_25);
    __m512 v512f_27 = _mm512_rcp14_ps(p3);
    }
    __m128 v128f_28 = _mm_loadu_ps(fbuf);
    __m128 v128f_29 = _mm_shuffle_ps(v128f_28, v128f_28, 0x1B);
    __m256h v256h_30 = g_v256h[1];
    __m512 v512f_31 = _mm512_cvtxph_ps(v256h_30);
    __mmask16 m16_32 = (__mmask16)(g_m16[0] ^ 59);
    __mmask16 m16_33 = _kor_mask16(m16_32, m16_32);
    __m512 v512f_34 = _mm512_mask_blend_ps(m16_33, v512f_16, v512f_16);
    __m512i v512i_35 = _mm512_lzcnt_epi32(v512i_6);
    __m512d v512d_36 = g_v512d[4];
    __m512d v512d_37 = _mm512_add_pd(v512d_36, v512d_36);
    __m512h v512h_38 = g_v512h[6];
    __m512h v512h_39 = _mm512_sqrt_ph(v512h_38);
    __m512 v512f_40 = _mm512_broadcast_f32x4(v128f_29);
    __mmask16 m16_41 = _mm512_kunpackb(m8_12, m8_13);
    g_v512d[0] = v512d_36;
    g_v512i[3] = v512i_35;
    g_v256i[8] = v256i_19;
    g_v128d[6] = p1;
    g_v128i[2] = v128i_7;
    g_v512h[6] = v512h_39;
    g_v256h[8] = v256h_30;
    g_m32[5] = m32_15;
    return v512d_36;
}

__m128 torture_87(__m512d p0, __m512i p1) {
    __m256 v256f_1 = g_v256f[9];
    __m256 v256f_2 = _mm256_sqrt_ps(v256f_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[2] ^ 120);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_maskz_mul_ps(m16_3, v512f_4, v512f_4);
    unsigned u32_6 = 59;
    __mmask16 m16_7 = _cvtu32_mask16(u32_6);
    __m512i v512i_8 = _mm512_unpacklo_epi32(p1, p1);
    __m512 v512f_9 = _mm512_getexp_ps(v512f_5);
    __m128i v128i_10 = _mm512_cvtepi32_epi8(v512i_8);
    __m256h v256h_11 = g_v256h[11];
    __m512 v512f_12 = _mm512_cvtxph_ps(v256h_11);
    __m128i v128i_13 = _mm_sub_epi32(v128i_10, v128i_10);
    __m512 v512f_14 = _mm512_sqrt_ps(v512f_9);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512 v512f_15 = _mm512_add_ps(v512f_9, v512f_4);
    __m512d v512d_16 = _mm512_sub_pd(p0, p0);
    __m256 v256f_17 = _mm256_sqrt_ps(v256f_1);
    __mmask16 m16_18 = _kxor_mask16(m16_7, m16_7);
    __mmask16 m16_19 = _kor_mask16(m16_18, m16_18);
    }
    __m512h v512h_20 = g_v512h[10];
    __m512h v512h_21 = _mm512_fmul_pch(v512h_20, v512h_20);
    __m512i v512i_22 = _mm512_xor_si512(p1, p1);
    __mmask16 m16_23 = _mm512_fpclass_ps_mask(v512f_5, 0x3);
    __m512 v512f_24 = _mm512_insertf32x8(v512f_12, v256f_2, 1);
    __m128 v128f_25 = g_v128f[9];
    __m128 v128f_26 = _mm_mul_ps(v128f_25, v128f_25);
    __m512 v512f_27 = _mm512_sqrt_ps(v512f_4);
    __m512i v512i_28 = _mm512_mullo_epi32(p1, v512i_22);
    __mmask8 m8_29 = (__mmask8)(g_m8[9] ^ 18);
    __m512i v512i_30 = _mm512_broadcastmb_epi64(m8_29);
    __m512i v512i_31 = _mm512_shuffle_epi8(p1, p1);
    __m512d v512d_32 = _mm512_mask_blend_pd(m8_29, p0, p0);
    g_v512i[7] = v512i_8;
    g_v128f[4] = v128f_26;
    g_v128i[3] = v128i_10;
    g_v512h[4] = v512h_21;
    g_v256h[8] = v256h_11;
    return v128f_25;
}

__m128d torture_88(__m512 p0, __m256i p1, __m128 p2, __m512d p3, __m128h p4) {
    __m512h v512h_1 = g_v512h[9];
    __m512h v512h_2 = _mm512_fmsub_ph(v512h_1, v512h_1, v512h_1);
    __m256d v256d_3 = g_v256d[7];
    __m256d v256d_4 = _mm256_div_pd(v256d_3, v256d_3);
    __mmask8 m8_5 = (__mmask8)(g_m8[2] ^ 80);
    __mmask8 m8_6 = _kand_mask8(m8_5, m8_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_dpbusd_epi32(v512i_7, v512i_7, v512i_7);
    __m512i v512i_9 = _mm512_sub_epi64(v512i_8, v512i_7);
    __mmask64 m64_10 = _mm512_cmp_epi8_mask(v512i_8, v512i_8, 1);
    __m512i v512i_11 = _mm512_lzcnt_epi32(v512i_9);
    __m128i v128i_12 = g_v128i[4];
    __m128i v128i_13 = _mm_or_si128(v128i_12, v128i_12);
    __m256 v256f_14 = g_v256f[11];
    __m256 v256f_15 = _mm256_sqrt_ps(v256f_14);
    __m256 v256f_16 = _mm256_mul_ps(v256f_14, v256f_14);
    __m256i v256i_17 = _mm256_sllv_epi32(p1, p1);
    __mmask16 m16_18 = (__mmask16)(g_m16[4] ^ 60);
    __mmask16 m16_19 = _kshiftli_mask16(m16_18, 2);
    __m256 v256f_20 = _mm256_and_ps(v256f_16, v256f_15);
    __m256 v256f_21 = _mm256_fmsub_ps(v256f_15, v256f_15, v256f_20);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_18) & 7); i_22++) {
    __m256 v256f_23 = _mm256_and_ps(v256f_15, v256f_20);
    __m128i v128i_24 = _mm_mullo_epi32(v128i_12, v128i_13);
    __m512d v512d_25 = _mm512_add_pd(p3, p3);
    }
    __mmask16 m16_26 = _mm512_test_epi32_mask(v512i_8, v512i_8);
    __m512 v512f_27 = _mm512_mask3_fmadd_ps(p0, p0, p0, m16_26);
    __m256i v256i_28 = _mm256_unpacklo_epi32(v256i_17, p1);
    __m256 v256f_29 = _mm256_fnmadd_ps(v256f_14, v256f_20, v256f_16);
    __m512i v512i_30 = _mm512_add_epi32(v512i_7, v512i_9);
    __m512 v512f_31 = _mm512_shuffle_f32x4(p0, v512f_27, 0x4E);
    __m256i v256i_32 = _mm256_slli_epi32(v256i_17, 3);
    __m128i v128i_33 = _mm_xor_si128(v128i_13, v128i_12);
    __m128i v128i_34 = _mm_and_si128(v128i_13, v128i_13);
    __m512i v512i_35 = _mm512_rol_epi32(v512i_8, 5);
    __m128d v128d_36 = g_v128d[1];
    __m128d v128d_37 = _mm_fmadd_pd(v128d_36, v128d_36, v128d_36);
    __mmask8 m8_38 = _kand_mask8(m8_6, m8_6);
    g_v512f[9] = p0;
    g_v512d[5] = p3;
    g_v512i[7] = v512i_7;
    g_v256f[5] = v256f_16;
    g_v256i[0] = p1;
    g_v128d[9] = v128d_37;
    g_v128i[2] = v128i_12;
    g_v128h[7] = p4;
    return v128d_37;
}

__m256 torture_89(__m128 p0, __m128 p1, __m512d p2, __m512i p3) {
    __m128 v128f_1 = _mm_unpacklo_ps(p0, p1);
    __m128i v128i_2 = g_v128i[10];
    __m128i v128i_3 = _mm_sub_epi8(v128i_2, v128i_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[8] ^ 122);
    __mmask16 m16_5 = _kor_mask16(m16_4, m16_4);
    __m512i v512i_6 = _mm512_set1_epi32(0x55);
    __m512i v512i_7 = _mm512_mullo_epi32(p3, p3);
    __m512i v512i_8 = _mm512_mullo_epi32(p3, v512i_6);
    __m128 v128f_9 = _mm_fnmadd_ps(p0, v128f_1, p0);
    __m128d v128d_10 = g_v128d[9];
    __m512d v512d_11 = _mm512_insertf64x2(p2, v128d_10, 3);
    __mmask8 m8_12 = _mm512_cmp_pd_mask(p2, v512d_11, _CMP_GE_OQ);
    __m512 v512f_13 = _mm512_loadu_ps(fbuf);
    __m512 v512f_14 = _mm512_mask_blend_ps(m16_5, v512f_13, v512f_13);
    __mmask16 m16_15 = _mm512_test_epi32_mask(v512i_6, v512i_6);
    __m512 v512f_16 = _mm512_cvtepi32_ps(v512i_8);
    __m128 v128f_17 = _mm_sub_ps(p1, p0);
    __mmask16 m16_18 = _mm512_kunpackb(m8_12, m8_12);
    __m512 v512f_19 = _mm512_shuffle_ps(v512f_13, v512f_16, 0x1B);
    __m128i v128i_20 = _mm_add_epi8(v128i_2, v128i_2);
    __mmask16 m16_21 = _mm512_kunpackb(m8_12, m8_12);
    __m128d v128d_22 = _mm_div_pd(v128d_10, v128d_10);
    __m128 v128f_23 = _mm_sqrt_ps(v128f_9);
    __m128i v128i_24 = _mm_sub_epi64(v128i_20, v128i_20);
    __m128d v128d_25 = _mm_sqrt_pd(v128d_10);
    __m512i v512i_26 = _mm512_mask_add_epi32(v512i_8, m16_4, v512i_8, p3);
    __m512 v512f_27 = _mm512_mask_blend_ps(m16_4, v512f_19, v512f_13);
    g_m8[5] = m8_12;
    __m256 v256f_28 = _mm256_loadu_ps(fbuf);
    return v256f_28;
}

__m512d torture_90(__m256i p0, __m512h p1, __m256d p2) {
    __m256 v256f_1 = g_v256f[7];
    __m256 v256f_2 = _mm256_xor_ps(v256f_1, v256f_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_ternarylogic_epi32(v512i_3, v512i_3, v512i_3, 0x96);
    __m128d v128d_5 = g_v128d[10];
    __m128d v128d_6 = _mm_unpackhi_pd(v128d_5, v128d_5);
    __m128i v128i_7 = g_v128i[3];
    __m512i v512i_8 = _mm512_cvtepi8_epi32(v128i_7);
    __m512i v512i_9 = _mm512_and_si512(v512i_4, v512i_4);
    __m256d v256d_10 = _mm256_div_pd(p2, p2);
    __m512d v512d_11 = g_v512d[10];
    __m128h v128h_12 = _mm512_cvtpd_ph(v512d_11);
    __m256i v256i_13 = _mm256_set1_epi32(0x55);
    __m512d v512d_14 = _mm512_shuffle_f64x2(v512d_11, v512d_11, 0xEE);
    __m256i v256i_15 = _mm512_cvtepi64_epi32(v512i_9);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m512 v512f_17 = _mm512_broadcast_f32x4(v128f_16);
    __m256i v256i_18 = _mm256_add_epi32(p0, v256i_15);
    __m256d v256d_19 = _mm256_add_pd(v256d_10, v256d_10);
    __m256i v256i_20 = _mm256_xor_si256(v256i_13, v256i_13);
    __m512h v512h_21 = _mm512_fmul_pch(p1, p1);
    __m256 v256f_22 = _mm256_sub_ps(v256f_1, v256f_1);
    __m128i v128i_23 = _mm_or_si128(v128i_7, v128i_7);
    __m512h v512h_24 = _mm512_getexp_ph(p1);
    int s32_25 = 51;
    if ((s32_25) & 1ULL) {
    __m256i v256i_26 = _mm256_unpacklo_epi32(v256i_18, p0);
    __m128 v128f_27 = _mm_set1_ps(1.0f);
    __m128 v128f_28 = _mm_fmadd_ps(v128f_16, v128f_16, v128f_16);
    }
    __m128i v128i_29 = _mm_add_epi16(v128i_7, v128i_23);
    __mmask64 m64_30 = _mm512_cmp_epi8_mask(v512i_8, v512i_4, 1);
    __m512i v512i_31 = _mm512_abs_epi32(v512i_8);
    __m512 v512f_32 = _mm512_shuffle_f32x4(v512f_17, v512f_17, 0x4E);
    if (((unsigned long long)m64_30) & 1ULL) {
    __m512i v512i_33 = _mm512_conflict_epi32(v512i_9);
    __m256 v256f_34 = _mm256_fmsub_ps(v256f_2, v256f_22, v256f_2);
    __m128i v128i_35 = _mm_add_epi16(v128i_29, v128i_7);
    __m512 v512f_36 = _mm512_sub_ps(v512f_17, v512f_32);
    }
    g_v512f[9] = v512f_17;
    g_v128i[1] = v128i_29;
    g_m64[9] = m64_30;
    return v512d_11;
}

__m256d torture_91(__m512d p0, __m512i p1, __m128d p2, __m256h p3, __m256d p4) {
    __m128 v128f_1 = g_v128f[8];
    __m128 v128f_2 = _mm_sqrt_ps(v128f_1);
    __m256d v256d_3 = _mm256_fmadd_pd(p4, p4, p4);
    __m256i v256i_4 = g_v256i[2];
    __m256i v256i_5 = _mm256_add_epi64(v256i_4, v256i_4);
    __m512d v512d_6 = _mm512_div_pd(p0, p0);
    __m256d v256d_7 = _mm256_max_pd(p4, p4);
    __mmask16 m16_8 = (__mmask16)(g_m16[5] ^ 59);
    __mmask16 m16_9 = _kor_mask16(m16_8, m16_8);
    __m512i v512i_10 = _mm512_mullo_epi32(p1, p1);
    __m512d v512d_11 = _mm512_shuffle_f64x2(v512d_6, p0, 0xEE);
    __m128i v128i_12 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_13 = _mm_add_epi16(v128i_12, v128i_12);
    __m128d v128d_14 = _mm_mul_pd(p2, p2);
    __mmask16 m16_15 = _knot_mask16(m16_9);
    __m128i v128i_16 = _mm_add_epi8(v128i_13, v128i_12);
    __m512 v512f_17 = _mm512_cvtxph_ps(p3);
    __m256i v256i_18 = _mm256_or_si256(v256i_4, v256i_4);
    __mmask16 m16_19 = _knot_mask16(m16_15);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_9) & 7); i_20++) {
    __m256 v256f_21 = g_v256f[5];
    __m256 v256f_22 = _mm256_and_ps(v256f_21, v256f_21);
    __m512i v512i_23 = _mm512_madd52lo_epu64(p1, p1, p1);
    __m256 v256f_24 = _mm256_max_ps(v256f_22, v256f_22);
    __m512i v512i_25 = _mm512_mullo_epi32(p1, p1);
    }
    __m512 v512f_26 = _mm512_mask_add_ps(v512f_17, m16_19, v512f_17, v512f_17);
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m512 v512f_28 = _mm512_insertf32x8(v512f_17, v256f_27, 1);
    __m128 v128f_29 = _mm_sqrt_ps(v128f_1);
    __m256 v256f_30 = _mm256_sub_ps(v256f_27, v256f_27);
    __m512i v512i_31 = _mm512_mask_blend_epi32(m16_9, v512i_10, p1);
    __m256d v256d_32 = _mm256_sqrt_pd(v256d_7);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_9) & 7); i_33++) {
    __m512i v512i_34 = _mm512_permutex2var_epi32(p1, v512i_10, v512i_10);
    __m512i v512i_35 = _mm512_dpwssd_epi32(v512i_10, v512i_34, v512i_34);
    __m256d v256d_36 = _mm256_set1_pd(2.0);
    }
    g_v512f[1] = v512f_28;
    g_v512i[5] = v512i_31;
    g_v128d[5] = v128d_14;
    g_v128i[5] = v128i_16;
    g_m16[0] = m16_15;
    return v256d_7;
}

__mmask32 torture_92(__m256d p0, __m512 p1, __m128d p2) {
    __m128i v128i_1 = g_v128i[5];
    __m128i v128i_2 = _mm_add_epi32(v128i_1, v128i_1);
    __m128d v128d_3 = _mm_unpackhi_pd(p2, p2);
    __m512i v512i_4 = g_v512i[0];
    __m512i v512i_5 = _mm512_shuffle_i32x4(v512i_4, v512i_4, 0x1B);
    __m128i v128i_6 = _mm512_cvtepi32_epi8(v512i_4);
    __m128i v128i_7 = _mm512_cvtepi32_epi8(v512i_4);
    __m512h v512h_8 = g_v512h[3];
    __mmask32 m32_9 = _mm512_cmp_ph_mask(v512h_8, v512h_8, 1);
    __m512i v512i_10 = _mm512_add_epi8(v512i_4, v512i_5);
    __mmask16 m16_11 = (__mmask16)(g_m16[3] ^ 80);
    __mmask16 m16_12 = _kshiftli_mask16(m16_11, 2);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m512h v512h_13 = _mm512_sqrt_ph(v512h_8);
    __m512d v512d_14 = g_v512d[9];
    __m512d v512d_15 = _mm512_div_pd(v512d_14, v512d_14);
    __m512 v512f_16 = _mm512_mask_add_ps(p1, m16_12, p1, p1);
    __m128i v128i_17 = _mm_add_epi64(v128i_2, v128i_1);
    }
    unsigned u32_18 = _cvtmask16_u32(m16_12);
    __m512i v512i_19 = _mm512_abs_epi32(v512i_4);
    __m256 v256f_20 = g_v256f[6];
    __m256 v256f_21 = _mm256_fmsub_ps(v256f_20, v256f_20, v256f_20);
    __mmask32 m32_22 = _kxor_mask32(m32_9, m32_9);
    __m512 v512f_23 = _mm512_set1_ps(1.0f);
    __mmask8 m8_24 = (__mmask8)(g_m8[8] ^ 178);
    __mmask16 m16_25 = _mm512_kunpackb(m8_24, m8_24);
    __mmask16 m16_26 = _mm512_cmp_epi32_mask(v512i_4, v512i_5, 2);
    __m512 v512f_27 = _mm512_unpacklo_ps(p1, v512f_23);
    if (((unsigned long long)m16_25) & 1ULL) {
    __m256 v256f_28 = _mm256_mul_ps(v256f_21, v256f_20);
    __m256 v256f_29 = _mm256_add_ps(v256f_20, v256f_28);
    __m256i v256i_30 = g_v256i[1];
    __m256i v256i_31 = _mm256_slli_epi32(v256i_30, 3);
    __m512d v512d_32 = g_v512d[2];
    __m512d v512d_33 = _mm512_sub_pd(v512d_32, v512d_32);
    __m128 v128f_34 = g_v128f[8];
    __m128 v128f_35 = _mm_max_ps(v128f_34, v128f_34);
    __m512i v512i_36 = _mm512_broadcast_i32x4(v128i_1);
    }
    g_v256d[6] = p0;
    g_v128d[2] = p2;
    g_m8[0] = m8_24;
    return m32_9;
}

__m512i torture_93(__m256 p0, __m512d p1, __m256d p2, __m512d p3) {
    __m128i v128i_1 = g_v128i[0];
    __m128i v128i_2 = _mm_abs_epi32(v128i_1);
    __m512i v512i_3 = g_v512i[8];
    __m512 v512f_4 = _mm512_cvtepi32_ps(v512i_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[6] ^ 217);
    __m512i v512i_6 = _mm512_mask_blend_epi32(m16_5, v512i_3, v512i_3);
    __m512h v512h_7 = g_v512h[2];
    __m512h v512h_8 = _mm512_fmadd_ph(v512h_7, v512h_7, v512h_7);
    __m128 v128f_9 = _mm_loadu_ps(fbuf);
    __m128 v128f_10 = _mm_and_ps(v128f_9, v128f_9);
    __mmask32 m32_11 = _mm512_cmp_ph_mask(v512h_8, v512h_7, 1);
    __m512i v512i_12 = _mm512_or_si512(v512i_6, v512i_6);
    __m512i v512i_13 = _mm512_shuffle_i32x4(v512i_12, v512i_6, 0x1B);
    __mmask16 m16_14 = _kshiftri_mask16(m16_5, 1);
    __m256i v256i_15 = g_v256i[10];
    __m256i v256i_16 = _mm256_sub_epi32(v256i_15, v256i_15);
    __mmask16 m16_17 = _kshiftri_mask16(m16_14, 1);
    __m512 v512f_18 = _mm512_max_ps(v512f_4, v512f_4);
    __m256i v256i_19 = _mm256_add_epi16(v256i_15, v256i_16);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512i v512i_20 = _mm512_conflict_epi32(v512i_6);
    __m256d v256d_21 = _mm256_min_pd(p2, p2);
    }
    __m512i v512i_22 = _mm512_mullo_epi32(v512i_13, v512i_13);
    __m128i v128i_23 = _mm_add_epi8(v128i_2, v128i_2);
    __m256h v256h_24 = g_v256h[0];
    __m512 v512f_25 = _mm512_cvtxph_ps(v256h_24);
    __m128 v128f_26 = _mm_min_ps(v128f_10, v128f_10);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512 v512f_27 = _mm512_unpacklo_ps(v512f_4, v512f_25);
    __m128 v128f_28 = _mm512_extractf32x4_ps(v512f_25, 2);
    __m256i v256i_29 = _mm256_sllv_epi32(v256i_15, v256i_19);
    __m128d v128d_30 = g_v128d[1];
    __m128d v128d_31 = _mm_add_pd(v128d_30, v128d_30);
    __m512h v512h_32 = _mm512_sqrt_ph(v512h_8);
    }
    g_v512i[8] = v512i_3;
    g_v256d[11] = p2;
    g_v128f[0] = v128f_9;
    g_v128i[9] = v128i_1;
    g_m16[3] = m16_14;
    g_m32[6] = m32_11;
    return v512i_13;
}

__mmask8 torture_94(__m256 p0, __m512h p1, __m512i p2, __m128d p3, __m512i p4) {
    __m256 v256f_1 = _mm256_fmsub_ps(p0, p0, p0);
    __m256d v256d_2 = g_v256d[7];
    __m256d v256d_3 = _mm256_sub_pd(v256d_2, v256d_2);
    __m128i v128i_4 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_5 = _mm_slli_epi32(v128i_4, 3);
    __m128i v128i_6 = _mm_set1_epi32(0x55);
    __m128i v128i_7 = _mm_mullo_epi32(v128i_4, v128i_6);
    __m256 v256f_8 = _mm256_sub_ps(v256f_1, v256f_1);
    __m128i v128i_9 = _mm_and_si128(v128i_5, v128i_4);
    __m512d v512d_10 = _mm512_loadu_pd(dbuf);
    __m256d v256d_11 = _mm512_extractf64x4_pd(v512d_10, 1);
    __m512 v512f_12 = g_v512f[9];
    __m512 v512f_13 = _mm512_and_ps(v512f_12, v512f_12);
    int s32_14 = 48;
    if ((s32_14) & 1ULL) {
    __m256d v256d_15 = _mm256_set1_pd(2.0);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_sqrt_ps(v128f_16);
    __mmask16 m16_18 = (__mmask16)(g_m16[1] ^ 24);
    __m512 v512f_19 = _mm512_mask_blend_ps(m16_18, v512f_13, v512f_12);
    }
    __m512i v512i_20 = _mm512_cvtph_epi16(p1);
    __m128 v128f_21 = _mm_loadu_ps(fbuf);
    __m128 v128f_22 = _mm_fmsub_ps(v128f_21, v128f_21, v128f_21);
    __mmask16 m16_23 = _mm512_fpclass_ps_mask(v512f_13, 0x3);
    __mmask8 m8_24 = (__mmask8)(g_m8[7] ^ 214);
    __mmask8 m8_25 = _kand_mask8(m8_24, m8_24);
    __m128i v128i_26 = _mm_sllv_epi32(v128i_4, v128i_4);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_23) & 7); i_27++) {
    __m256 v256f_28 = _mm256_set1_ps(1.0f);
    __m128 v128f_29 = _mm_div_ps(v128f_21, v128f_22);
    __m512i v512i_30 = _mm512_permutexvar_epi32(p4, p4);
    __m512 v512f_31 = _mm512_fnmadd_ps(v512f_12, v512f_12, v512f_12);
    __m512i v512i_32 = _mm512_add_epi16(p4, v512i_20);
    }
    g_v512i[5] = p4;
    g_v256f[1] = v256f_8;
    g_v256d[10] = v256d_2;
    g_m16[8] = m16_23;
    return m8_25;
}

__m512d torture_95(__m256 p0, __m256h p1, __m128 p2, __m128h p3) {
    __m128d v128d_1 = g_v128d[7];
    __m128d v128d_2 = _mm_add_pd(v128d_1, v128d_1);
    __m256i v256i_3 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_4 = _mm256_or_si256(v256i_3, v256i_3);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_6 = _mm512_dpwssd_epi32(v512i_5, v512i_5, v512i_5);
    __m256d v256d_7 = g_v256d[7];
    __m256d v256d_8 = _mm256_max_pd(v256d_7, v256d_7);
    __m256i v256i_9 = _mm256_unpacklo_epi32(v256i_3, v256i_4);
    __m512h v512h_10 = g_v512h[2];
    __m512h v512h_11 = _mm512_getexp_ph(v512h_10);
    __m256 v256f_12 = _mm256_sub_ps(p0, p0);
    __m512i v512i_13 = _mm512_ternarylogic_epi32(v512i_5, v512i_6, v512i_6, 0x96);
    int s32_14 = 43;
    if ((s32_14) & 1ULL) {
    __m512h v512h_15 = _mm512_mul_ph(v512h_10, v512h_10);
    __m512d v512d_16 = _mm512_loadu_pd(dbuf);
    __m256 v256f_17 = _mm512_cvtpd_ps(v512d_16);
    __m512i v512i_18 = _mm512_lzcnt_epi32(v512i_13);
    __mmask16 m16_19 = (__mmask16)(g_m16[11] ^ 171);
    __mmask16 m16_20 = _kshiftri_mask16(m16_19, 1);
    }
    __m128 v128f_21 = _mm_and_ps(p2, p2);
    __m512i v512i_22 = _mm512_add_epi64(v512i_6, v512i_13);
    __m512i v512i_23 = _mm512_sub_epi16(v512i_13, v512i_6);
    __m512 v512f_24 = _mm512_loadu_ps(fbuf);
    __m512 v512f_25 = _mm512_shuffle_ps(v512f_24, v512f_24, 0x1B);
    __mmask16 m16_26 = _mm512_testn_epi32_mask(v512i_22, v512i_6);
    __m256d v256d_27 = _mm256_div_pd(v256d_7, v256d_7);
    __m256i v256i_28 = _mm256_add_epi8(v256i_4, v256i_3);
    __m512i v512i_29 = _mm512_broadcastmw_epi32(m16_26);
    g_v512f[0] = v512f_25;
    g_v512i[2] = v512i_22;
    g_v256i[2] = v256i_4;
    g_v128f[2] = p2;
    g_v256h[3] = p1;
    g_v128h[10] = p3;
    __m512d v512d_30 = _mm512_loadu_pd(dbuf);
    return v512d_30;
}

__m128 torture_96(__m512d p0, __m128 p1) {
    __m128i v128i_1 = g_v128i[8];
    __m128i v128i_2 = _mm_add_epi64(v128i_1, v128i_1);
    __m256 v256f_3 = g_v256f[7];
    __m256 v256f_4 = _mm256_sub_ps(v256f_3, v256f_3);
    __m512i v512i_5 = g_v512i[4];
    __m512i v512i_6 = _mm512_sub_epi32(v512i_5, v512i_5);
    __m128i v128i_7 = _mm512_cvtepi32_epi8(v512i_6);
    __m256i v256i_8 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_9 = _mm256_sllv_epi32(v256i_8, v256i_8);
    __m256 v256f_10 = _mm256_xor_ps(v256f_4, v256f_3);
    __m256 v256f_11 = _mm512_cvtpd_ps(p0);
    __m128i v128i_12 = _mm_sub_epi8(v128i_7, v128i_2);
    __m128 v128f_13 = _mm_unpacklo_ps(p1, p1);
    __m128i v128i_14 = _mm_add_epi8(v128i_12, v128i_2);
    __m128 v128f_15 = _mm_min_ps(v128f_13, v128f_13);
    __m512i v512i_16 = _mm512_mullo_epi32(v512i_5, v512i_6);
    __mmask16 m16_17 = _mm512_cmp_epu32_mask(v512i_6, v512i_6, 4);
    __m512i v512i_18 = _mm512_dpwssd_epi32(v512i_5, v512i_6, v512i_5);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m128d v128d_19 = g_v128d[8];
    __m128d v128d_20 = _mm_fmadd_pd(v128d_19, v128d_19, v128d_19);
    __m512i v512i_21 = _mm512_or_si512(v512i_6, v512i_6);
    __m512 v512f_22 = g_v512f[5];
    __m512 v512f_23 = _mm512_min_ps(v512f_22, v512f_22);
    __m256i v256i_24 = _mm256_or_si256(v256i_9, v256i_8);
    __m512h v512h_25 = g_v512h[0];
    __m512h v512h_26 = _mm512_getexp_ph(v512h_25);
    __m512 v512f_27 = _mm512_unpacklo_ps(v512f_22, v512f_23);
    }
    __m256 v256f_28 = _mm256_fmadd_ps(v256f_4, v256f_4, v256f_10);
    __m512i v512i_29 = _mm512_permutex2var_epi32(v512i_18, v512i_6, v512i_5);
    __m256i v256i_30 = _mm256_sub_epi16(v256i_8, v256i_8);
    __m512i v512i_31 = _mm512_sub_epi32(v512i_16, v512i_16);
    __m512 v512f_32 = g_v512f[4];
    __m512 v512f_33 = _mm512_mask_blend_ps(m16_17, v512f_32, v512f_32);
    __m256 v256f_34 = _mm256_fmsub_ps(v256f_10, v256f_3, v256f_10);
    __m128d v128d_35 = g_v128d[1];
    __m128d v128d_36 = _mm_min_pd(v128d_35, v128d_35);
    __m256h v256h_37 = g_v256h[9];
    __m512 v512f_38 = _mm512_cvtxph_ps(v256h_37);
    __m512 v512f_39 = _mm512_and_ps(v512f_32, v512f_33);
    __mmask8 m8_40 = (__mmask8)(g_m8[4] ^ 236);
    __mmask16 m16_41 = _mm512_kunpackb(m8_40, m8_40);
    __m512 v512f_42 = _mm512_mask3_fmadd_ps(v512f_32, v512f_39, v512f_39, m16_17);
    __m512 v512f_43 = _mm512_mask_blend_ps(m16_41, v512f_42, v512f_32);
    if (((unsigned long long)m16_41) & 1ULL) {
    __m512d v512d_44 = _mm512_mask_blend_pd(m8_40, p0, p0);
    __m256 v256f_45 = _mm256_fmsub_ps(v256f_3, v256f_3, v256f_11);
    __m512 v512f_46 = _mm512_mask_add_ps(v512f_33, m16_17, v512f_32, v512f_42);
    __m256i v256i_47 = _mm256_add_epi16(v256i_9, v256i_8);
    __m256 v256f_48 = _mm256_shuffle_ps(v256f_3, v256f_28, 0x1B);
    }
    g_v512f[5] = v512f_33;
    g_v512d[2] = p0;
    g_v256f[8] = v256f_28;
    g_v256i[6] = v256i_9;
    g_v128f[9] = v128f_15;
    g_v128i[2] = v128i_2;
    g_m8[2] = m8_40;
    return v128f_15;
}

__m512 torture_97(__m128i p0, __m256d p1, __m256h p2, __m512i p3, __m128i p4) {
    __mmask16 m16_1 = (__mmask16)(g_m16[11] ^ 28);
    __mmask16 m16_2 = _kand_mask16(m16_1, m16_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_xor_ps(v512f_3, v512f_3);
    __m512 v512f_5 = _mm512_fmadd_ps(v512f_3, v512f_3, v512f_3);
    __m128i v128i_6 = _mm_and_si128(p4, p4);
    __m512i v512i_7 = _mm512_cvtps_epi32(v512f_3);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_mul_ps(v256f_8, v256f_8);
    __mmask16 m16_10 = _mm512_movepi32_mask(p3);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_add_pd(v512d_11, v512d_11);
    __m512 v512f_13 = _mm512_max_ps(v512f_4, v512f_3);
    __m512i v512i_14 = _mm512_dpwssd_epi32(p3, p3, v512i_7);
    __m128d v128d_15 = g_v128d[5];
    __m128d v128d_16 = _mm_min_pd(v128d_15, v128d_15);
    __m128 v128f_17 = g_v128f[4];
    __m128 v128f_18 = _mm_unpacklo_ps(v128f_17, v128f_17);
    __mmask16 m16_19 = _mm512_movepi32_mask(v512i_7);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m256 v256f_20 = _mm256_add_ps(v256f_9, v256f_8);
    __m256i v256i_21 = g_v256i[5];
    __m256i v256i_22 = _mm256_shuffle_epi8(v256i_21, v256i_21);
    __m256 v256f_23 = _mm256_and_ps(v256f_8, v256f_9);
    __m256i v256i_24 = _mm256_xor_si256(v256i_22, v256i_21);
    }
    __m256 v256f_25 = _mm256_min_ps(v256f_9, v256f_8);
    __m512 v512f_26 = _mm512_set1_ps(1.0f);
    __m256i v256i_27 = g_v256i[1];
    __m256i v256i_28 = _mm256_sub_epi64(v256i_27, v256i_27);
    __m128 v128f_29 = _mm_and_ps(v128f_18, v128f_18);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_10) & 7); i_30++) {
    __m256d v256d_31 = _mm256_sub_pd(p1, p1);
    __m256 v256f_32 = _mm256_set1_ps(1.0f);
    __m128d v128d_33 = _mm_add_pd(v128d_15, v128d_16);
    __m256d v256d_34 = _mm256_set1_pd(2.0);
    __m128i v128i_35 = _mm_sub_epi8(p0, v128i_6);
    }
    g_v512d[0] = v512d_11;
    g_v256f[10] = v256f_8;
    g_v128i[1] = v128i_6;
    g_v256h[0] = p2;
    g_m16[4] = m16_1;
    return v512f_26;
}

__m512h torture_98(__m128 p0, __m128h p1, __m128d p2) {
    __m128 v128f_1 = _mm_and_ps(p0, p0);
    __m256 v256f_2 = _mm256_loadu_ps(fbuf);
    __m256 v256f_3 = _mm256_and_ps(v256f_2, v256f_2);
    __m512i v512i_4 = g_v512i[7];
    __mmask16 m16_5 = _mm512_cmp_epi32_mask(v512i_4, v512i_4, 2);
    __m256i v256i_6 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_7 = _mm256_slli_epi32(v256i_6, 3);
    __mmask16 m16_8 = _mm512_test_epi32_mask(v512i_4, v512i_4);
    __m512i v512i_9 = _mm512_or_si512(v512i_4, v512i_4);
    __m256d v256d_10 = g_v256d[10];
    __m256d v256d_11 = _mm256_div_pd(v256d_10, v256d_10);
    __m512 v512f_12 = g_v512f[5];
    __m512 v512f_13 = _mm512_shuffle_f32x4(v512f_12, v512f_12, 0x4E);
    __m128i v128i_14 = g_v128i[0];
    __m512i v512i_15 = _mm512_broadcast_i32x4(v128i_14);
    __m128 v128f_16 = _mm_div_ps(p0, v128f_1);
    __m512i v512i_17 = _mm512_broadcastmw_epi32(m16_8);
    __mmask16 m16_18 = _mm512_testn_epi32_mask(v512i_4, v512i_9);
    __m256i v256i_19 = _mm256_or_si256(v256i_7, v256i_7);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_18) & 7); i_20++) {
    __m512 v512f_21 = _mm512_add_ps(v512f_13, v512f_13);
    __m512 v512f_22 = _mm512_sub_ps(v512f_12, v512f_12);
    }
    __m512i v512i_23 = _mm512_sub_epi32(v512i_4, v512i_15);
    __m512 v512f_24 = _mm512_shuffle_ps(v512f_13, v512f_13, 0x1B);
    __m512 v512f_25 = _mm512_shuffle_ps(v512f_12, v512f_12, 0x1B);
    __mmask16 m16_26 = _kor_mask16(m16_5, m16_8);
    __m256i v256i_27 = _mm256_sub_epi64(v256i_7, v256i_19);
    __m256d v256d_28 = _mm256_add_pd(v256d_11, v256d_11);
    __m128 v128f_29 = _mm_set1_ps(1.0f);
    __m512 v512f_30 = _mm512_fnmadd_ps(v512f_13, v512f_13, v512f_13);
    g_v512i[5] = v512i_4;
    g_v256d[3] = v256d_28;
    g_v256i[3] = v256i_7;
    g_v128f[5] = v128f_1;
    g_v128d[10] = p2;
    __m512h v512h_31 = g_v512h[6];
    return v512h_31;
}

__m256 torture_99(__m128h p0, __m256h p1, __m256i p2) {
    __m128 v128f_1 = g_v128f[6];
    __m128 v128f_2 = _mm_div_ps(v128f_1, v128f_1);
    __m256i v256i_3 = _mm256_slli_epi32(p2, 3);
    __m128i v128i_4 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_5 = _mm_unpacklo_epi32(v128i_4, v128i_4);
    __m256 v256f_6 = _mm256_set1_ps(1.0f);
    __m256 v256f_7 = _mm256_div_ps(v256f_6, v256f_6);
    __m128d v128d_8 = g_v128d[2];
    __m128d v128d_9 = _mm_min_pd(v128d_8, v128d_8);
    __m512d v512d_10 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_11 = _mm512_cmp_pd_mask(v512d_10, v512d_10, _CMP_GE_OQ);
    __m128d v128d_12 = _mm_div_pd(v128d_8, v128d_8);
    __m512 v512f_13 = g_v512f[11];
    __m512i v512i_14 = _mm512_cvtps_epi32(v512f_13);
    __mmask16 m16_15 = (__mmask16)(g_m16[9] ^ 40);
    __mmask16 m16_16 = _kand_mask16(m16_15, m16_15);
    __m128 v128f_17 = _mm_sqrt_ps(v128f_2);
    __mmask16 m16_18 = _mm512_fpclass_ps_mask(v512f_13, 0x3);
    __m256i v256i_19 = _mm256_slli_epi32(v256i_3, 3);
    __m512 v512f_20 = _mm512_fnmadd_ps(v512f_13, v512f_13, v512f_13);
    __m256i v256i_21 = _mm256_slli_epi32(v256i_3, 3);
    __m512d v512d_22 = _mm512_cvtps_pd(v256f_6);
    __m128 v128f_23 = _mm_mul_ps(v128f_2, v128f_1);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m128d v128d_24 = _mm_set1_pd(2.0);
    __m512i v512i_25 = _mm512_cvtps_epi32(v512f_13);
    __m512i v512i_26 = _mm512_add_epi8(v512i_14, v512i_25);
    __m128d v128d_27 = _mm_max_pd(v128d_9, v128d_12);
    __m128d v128d_28 = _mm_unpackhi_pd(v128d_27, v128d_9);
    __m512i v512i_29 = _mm512_set1_epi32(0x55);
    }
    __m512 v512f_30 = _mm512_unpacklo_ps(v512f_20, v512f_20);
    __m512i v512i_31 = _mm512_sub_epi8(v512i_14, v512i_14);
    __m512h v512h_32 = g_v512h[9];
    __m512h v512h_33 = _mm512_rcp_ph(v512h_32);
    __m512 v512f_34 = _mm512_and_ps(v512f_13, v512f_20);
    __m512i v512i_35 = _mm512_dpbusd_epi32(v512i_31, v512i_31, v512i_14);
    __m512i v512i_36 = _mm512_rol_epi32(v512i_14, 5);
    for (int i_37 = 0; i_37 < (int)(((unsigned long long)m16_16) & 7); i_37++) {
    __m128 v128f_38 = _mm_fmsub_ps(v128f_2, v128f_23, v128f_2);
    __mmask16 m16_39 = _mm512_cmp_epu32_mask(v512i_14, v512i_35, 4);
    }
    g_v256i[8] = p2;
    g_v128d[1] = v128d_9;
    g_m16[1] = m16_16;
    return v256f_6;
}

__m256h torture_100(__m512 p0, __m128d p1, __m512h p2, __m128h p3, __m128i p4) {
    __m512 v512f_1 = _mm512_div_ps(p0, p0);
    __m128i v128i_2 = _mm_xor_si128(p4, p4);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_div_ps(v128f_3, v128f_3);
    __m512i v512i_5 = g_v512i[8];
    __mmask16 m16_6 = _mm512_test_epi32_mask(v512i_5, v512i_5);
    __mmask16 m16_7 = _mm512_cmp_epi32_mask(v512i_5, v512i_5, 2);
    __m512 v512f_8 = _mm512_rsqrt14_ps(p0);
    __m512i v512i_9 = _mm512_broadcastmw_epi32(m16_6);
    __m512d v512d_10 = _mm512_loadu_pd(dbuf);
    __m512d v512d_11 = _mm512_fmadd_pd(v512d_10, v512d_10, v512d_10);
    __m512i v512i_12 = _mm512_mask_add_epi32(v512i_5, m16_6, v512i_5, v512i_5);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m256 v256f_13 = _mm256_loadu_ps(fbuf);
    __m256 v256f_14 = _mm256_unpacklo_ps(v256f_13, v256f_13);
    __mmask32 m32_15 = _mm512_cmp_ph_mask(p2, p2, 1);
    __m128i v128i_16 = _mm_set1_epi32(0x55);
    }
    __m256d v256d_17 = g_v256d[0];
    __m256d v256d_18 = _mm256_mul_pd(v256d_17, v256d_17);
    __m512i v512i_19 = _mm512_set1_epi32(0x55);
    __m512 v512f_20 = _mm512_mask_add_ps(p0, m16_6, v512f_1, p0);
    __m256 v256f_21 = g_v256f[0];
    __m256 v256f_22 = _mm256_and_ps(v256f_21, v256f_21);
    g_v512f[0] = v512f_8;
    g_v512i[0] = v512i_5;
    g_v128i[0] = v128i_2;
    g_m16[5] = m16_7;
    __m256h v256h_23 = g_v256h[0];
    return v256h_23;
}

__m256 torture_101(__m256i p0, __m512i p1, __m128i p2, __m128i p3) {
    __m512i v512i_1 = _mm512_add_epi32(p1, p1);
    __m128i v128i_2 = _mm_add_epi16(p2, p2);
    __m512 v512f_3 = g_v512f[1];
    __m512 v512f_4 = _mm512_shuffle_ps(v512f_3, v512f_3, 0x1B);
    __m512i v512i_5 = _mm512_sllv_epi32(p1, v512i_1);
    __m128h v128h_6 = g_v128h[4];
    __m512d v512d_7 = _mm512_cvtph_pd(v128h_6);
    __m128i v128i_8 = _mm512_cvtepi32_epi8(v512i_5);
    __m512 v512f_9 = _mm512_rsqrt14_ps(v512f_3);
    __m256i v256i_10 = _mm256_sub_epi8(p0, p0);
    __m512i v512i_11 = _mm512_cvtpd_epi64(v512d_7);
    __m512i v512i_12 = _mm512_permutex2var_epi32(v512i_5, v512i_11, v512i_11);
    __m256 v256f_13 = _mm512_cvtpd_ps(v512d_7);
    __m128i v128i_14 = _mm_add_epi64(v128i_8, p3);
    __m512 v512f_15 = _mm512_fmsub_ps(v512f_4, v512f_3, v512f_9);
    __m256 v256f_16 = _mm256_add_ps(v256f_13, v256f_13);
    int s32_17 = 80;
    if ((s32_17) & 1ULL) {
    __m128d v128d_18 = g_v128d[6];
    __m128d v128d_19 = _mm_fmadd_pd(v128d_18, v128d_18, v128d_18);
    __m512i v512i_20 = _mm512_cvtps_epi32(v512f_9);
    }
    __m512h v512h_21 = g_v512h[0];
    __mmask32 m32_22 = _mm512_cmp_ph_mask(v512h_21, v512h_21, 1);
    __m512d v512d_23 = _mm512_sqrt_pd(v512d_7);
    __mmask16 m16_24 = _mm512_cmp_epi32_mask(v512i_1, v512i_11, 2);
    __m256 v256f_25 = _mm256_fnmadd_ps(v256f_13, v256f_13, v256f_16);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_24) & 7); i_26++) {
    __m128 v128f_27 = _mm_loadu_ps(fbuf);
    __m128 v128f_28 = _mm_add_ps(v128f_27, v128f_27);
    __mmask16 m16_29 = _kxor_mask16(m16_24, m16_24);
    __m128i v128i_30 = _mm_sllv_epi32(p3, p2);
    }
    g_v512f[8] = v512f_15;
    return v256f_25;
}

__m128i torture_102(__m128 p0, __m512i p1, __m512h p2, __m512h p3, __m512 p4) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_or_si256(v256i_1, v256i_1);
    __mmask64 m64_3 = (__mmask64)(g_m64[6] ^ 100);
    __mmask64 m64_4 = _kor_mask64(m64_3, m64_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[8] ^ 157);
    __mmask16 m16_6 = _kor_mask16(m16_5, m16_5);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_fmadd_ps(v256f_7, v256f_7, v256f_7);
    __m256d v256d_9 = g_v256d[11];
    __m256d v256d_10 = _mm256_mul_pd(v256d_9, v256d_9);
    __mmask8 m8_11 = (__mmask8)(g_m8[1] ^ 73);
    __mmask8 m8_12 = _kand_mask8(m8_11, m8_11);
    __m512i v512i_13 = _mm512_mullo_epi32(p1, p1);
    __m256 v256f_14 = _mm256_and_ps(v256f_8, v256f_8);
    __m128d v128d_15 = g_v128d[4];
    __m128d v128d_16 = _mm_add_pd(v128d_15, v128d_15);
    __m256d v256d_17 = _mm256_div_pd(v256d_10, v256d_9);
    __m512i v512i_18 = _mm512_permutexvar_epi32(v512i_13, v512i_13);
    __m512h v512h_19 = _mm512_rcp_ph(p3);
    __m256d v256d_20 = _mm256_sqrt_pd(v256d_10);
    __m512i v512i_21 = _mm512_rol_epi32(v512i_13, 5);
    __m256d v256d_22 = _mm256_min_pd(v256d_17, v256d_17);
    __m512i v512i_23 = _mm512_broadcastmb_epi64(m8_11);
    __m256d v256d_24 = _mm256_sqrt_pd(v256d_22);
    __m512i v512i_25 = _mm512_sub_epi16(v512i_21, v512i_18);
    __m256i v256i_26 = _mm256_or_si256(v256i_1, v256i_1);
    __m256i v256i_27 = _mm256_add_epi8(v256i_26, v256i_2);
    __m128d v128d_28 = _mm_div_pd(v128d_15, v128d_15);
    __m256i v256i_29 = _mm256_set1_epi32(0x55);
    __m256i v256i_30 = _mm256_add_epi64(v256i_1, v256i_26);
    __m256i v256i_31 = _mm256_or_si256(v256i_2, v256i_29);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m128 v128f_32 = _mm_fnmadd_ps(p0, p0, p0);
    __m256 v256f_33 = _mm256_unpacklo_ps(v256f_14, v256f_14);
    __m128 v128f_34 = _mm_min_ps(p0, v128f_32);
    __m128d v128d_35 = _mm_min_pd(v128d_28, v128d_28);
    __m256i v256i_36 = _mm256_shuffle_epi8(v256i_27, v256i_1);
    __m512i v512i_37 = _mm512_sub_epi16(v512i_18, v512i_23);
    }
    g_v256f[2] = v256f_14;
    g_v256d[10] = v256d_10;
    g_m8[1] = m8_11;
    g_m64[0] = m64_4;
    __m128i v128i_38 = _mm_loadu_si128((const void*)ibuf);
    return v128i_38;
}

__m256i torture_103(__m512i p0, __m512h p1) {
    __m256 v256f_1 = g_v256f[3];
    __m256 v256f_2 = _mm256_sqrt_ps(v256f_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_mul_ps(v512f_3, v512f_3);
    __m512d v512d_5 = _mm512_loadu_pd(dbuf);
    __m128d v128d_6 = g_v128d[6];
    __m512d v512d_7 = _mm512_insertf64x2(v512d_5, v128d_6, 3);
    __m512i v512i_8 = _mm512_shuffle_i32x4(p0, p0, 0x1B);
    __m128d v128d_9 = _mm_mul_pd(v128d_6, v128d_6);
    __mmask32 m32_10 = (__mmask32)(g_m32[3] ^ 237);
    __mmask32 m32_11 = _kxor_mask32(m32_10, m32_10);
    __mmask16 m16_12 = (__mmask16)(g_m16[5] ^ 8);
    __m512 v512f_13 = _mm512_maskz_mul_ps(m16_12, v512f_4, v512f_4);
    __m128 v128f_14 = _mm_loadu_ps(fbuf);
    __m128 v128f_15 = _mm_fmsub_ps(v128f_14, v128f_14, v128f_14);
    __m256 v256f_16 = _mm256_mul_ps(v256f_2, v256f_1);
    __m256d v256d_17 = g_v256d[10];
    __m256d v256d_18 = _mm256_mul_pd(v256d_17, v256d_17);
    __m256 v256f_19 = _mm256_unpacklo_ps(v256f_2, v256f_1);
    __m256i v256i_20 = g_v256i[9];
    __m256i v256i_21 = _mm256_shuffle_epi8(v256i_20, v256i_20);
    __m512h v512h_22 = _mm512_getexp_ph(p1);
    __m128i v128i_23 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_24 = _mm_sllv_epi32(v128i_23, v128i_23);
    __m256d v256d_25 = _mm256_min_pd(v256d_17, v256d_18);
    __m512 v512f_26 = _mm512_div_ps(v512f_3, v512f_4);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_12) & 7); i_27++) {
    __m128d v128d_28 = _mm_div_pd(v128d_9, v128d_9);
    __m128i v128i_29 = _mm_unpacklo_epi32(v128i_24, v128i_24);
    __m128i v128i_30 = _mm_or_si128(v128i_29, v128i_29);
    __m256d v256d_31 = _mm256_unpackhi_pd(v256d_25, v256d_17);
    __m512i v512i_32 = _mm512_mask_blend_epi32(m16_12, p0, v512i_8);
    }
    g_v512d[6] = v512d_5;
    g_v256d[2] = v256d_18;
    g_v128f[9] = v128f_15;
    g_v128i[11] = v128i_23;
    g_m16[6] = m16_12;
    return v256i_21;
}

__m256d torture_104(__m256i p0, __m256i p1, __m256d p2) {
    __m512 v512f_1 = g_v512f[3];
    __m512 v512f_2 = _mm512_fmadd_ps(v512f_1, v512f_1, v512f_1);
    __m512 v512f_3 = _mm512_shuffle_f32x4(v512f_1, v512f_1, 0x4E);
    __m512h v512h_4 = g_v512h[3];
    __m512h v512h_5 = _mm512_fmul_pch(v512h_4, v512h_4);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_ternarylogic_epi32(v512i_6, v512i_6, v512i_6, 0x96);
    __mmask16 m16_8 = _mm512_cmp_ps_mask(v512f_3, v512f_1, _CMP_LT_OQ);
    __m512 v512f_9 = _mm512_rsqrt14_ps(v512f_3);
    __m256 v256f_10 = _mm256_loadu_ps(fbuf);
    __m512d v512d_11 = _mm512_cvtps_pd(v256f_10);
    __mmask16 m16_12 = _mm512_movepi32_mask(v512i_7);
    for (int i_13 = 0; i_13 < (int)(((unsigned long long)m16_8) & 7); i_13++) {
    __m128d v128d_14 = g_v128d[0];
    __m128d v128d_15 = _mm_add_pd(v128d_14, v128d_14);
    __m512 v512f_16 = _mm512_scalef_ps(v512f_9, v512f_1);
    }
    __m512i v512i_17 = _mm512_unpacklo_epi32(v512i_6, v512i_7);
    __m512 v512f_18 = _mm512_add_ps(v512f_9, v512f_9);
    __m256i v256i_19 = _mm512_cvtepi64_epi32(v512i_17);
    __m512i v512i_20 = _mm512_sllv_epi32(v512i_6, v512i_17);
    __m512i v512i_21 = _mm512_sub_epi16(v512i_17, v512i_20);
    __m128i v128i_22 = g_v128i[1];
    __m128i v128i_23 = _mm_add_epi8(v128i_22, v128i_22);
    __m128 v128f_24 = _mm_loadu_ps(fbuf);
    __m128 v128f_25 = _mm_min_ps(v128f_24, v128f_24);
    __m256 v256f_26 = _mm256_unpacklo_ps(v256f_10, v256f_10);
    g_v128i[5] = v128i_22;
    g_v512h[6] = v512h_4;
    return p2;
}

__m128d torture_105(__m512h p0, __m128h p1, __m512i p2) {
    __m512 v512f_1 = _mm512_set1_ps(1.0f);
    __m256 v256f_2 = _mm256_loadu_ps(fbuf);
    __m256 v256f_3 = _mm256_add_ps(v256f_2, v256f_2);
    __m512i v512i_4 = _mm512_rolv_epi32(p2, p2);
    __m128 v128f_5 = _mm_loadu_ps(fbuf);
    __m128 v128f_6 = _mm_sqrt_ps(v128f_5);
    __mmask8 m8_7 = (__mmask8)(g_m8[8] ^ 95);
    __m512d v512d_8 = _mm512_loadu_pd(dbuf);
    __m512d v512d_9 = _mm512_mask_blend_pd(m8_7, v512d_8, v512d_8);
    __m128i v128i_10 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_11 = _mm_xor_si128(v128i_10, v128i_10);
    __m256d v256d_12 = g_v256d[8];
    __m256d v256d_13 = _mm256_div_pd(v256d_12, v256d_12);
    __m512h v512h_14 = _mm512_cvtepi16_ph(p2);
    __m256 v256f_15 = _mm256_fmsub_ps(v256f_2, v256f_2, v256f_2);
    __m128 v128f_16 = _mm_add_ps(v128f_6, v128f_6);
    __m128i v128i_17 = _mm512_cvtepi32_epi8(v512i_4);
    __m128i v128i_18 = _mm_or_si128(v128i_10, v128i_17);
    __m256 v256f_19 = _mm256_max_ps(v256f_2, v256f_3);
    __mmask16 m16_20 = _mm512_kunpackb(m8_7, m8_7);
    __m256 v256f_21 = _mm256_fnmadd_ps(v256f_19, v256f_2, v256f_3);
    if (((unsigned long long)m16_20) & 1ULL) {
    __mmask16 m16_22 = _mm512_cmp_ps_mask(v512f_1, v512f_1, _CMP_LT_OQ);
    __m256 v256f_23 = _mm256_add_ps(v256f_19, v256f_15);
    __m128h v128h_24 = _mm512_cvtpd_ph(v512d_9);
    __m512 v512f_25 = _mm512_min_ps(v512f_1, v512f_1);
    __m256i v256i_26 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_27 = _mm256_and_si256(v256i_26, v256i_26);
    __m256i v256i_28 = _mm256_unpacklo_epi32(v256i_26, v256i_26);
    }
    __m512i v512i_29 = _mm512_broadcastmw_epi32(m16_20);
    __m128i v128i_30 = _mm_mullo_epi32(v128i_18, v128i_11);
    __mmask16 m16_31 = _mm512_movepi32_mask(v512i_29);
    __m256d v256d_32 = _mm256_set1_pd(2.0);
    __m256i v256i_33 = g_v256i[2];
    __m256i v256i_34 = _mm256_sub_epi64(v256i_33, v256i_33);
    __m128 v128f_35 = _mm_unpacklo_ps(v128f_6, v128f_5);
    __m512i v512i_36 = _mm512_cvtpd_epi64(v512d_8);
    __m128i v128i_37 = _mm_sub_epi32(v128i_30, v128i_18);
    __m128 v128f_38 = _mm_mul_ps(v128f_35, v128f_16);
    __m128 v128f_39 = _mm_shuffle_ps(v128f_16, v128f_5, 0x1B);
    __m512 v512f_40 = _mm512_add_ps(v512f_1, v512f_1);
    if (((unsigned long long)m16_20) & 1ULL) {
    __m128i v128i_41 = _mm_abs_epi32(v128i_37);
    __mmask16 m16_42 = _mm512_cmp_epi32_mask(v512i_36, v512i_36, 2);
    }
    g_v512f[3] = v512f_40;
    g_v256i[2] = v256i_33;
    g_v128f[9] = v128f_35;
    g_v128i[1] = v128i_17;
    __m128d v128d_43 = g_v128d[7];
    return v128d_43;
}

__m512 torture_106(__m256h p0, __m256h p1, __m512h p2, __m256 p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[5] ^ 3);
    __m512 v512f_2 = g_v512f[6];
    __m512 v512f_3 = _mm512_maskz_mul_ps(m16_1, v512f_2, v512f_2);
    __m256d v256d_4 = g_v256d[7];
    __m256d v256d_5 = _mm256_sub_pd(v256d_4, v256d_4);
    __mmask8 m8_6 = (__mmask8)(g_m8[2] ^ 223);
    __mmask16 m16_7 = _mm512_kunpackb(m8_6, m8_6);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_9 = _mm512_lzcnt_epi32(v512i_8);
    __m128 v128f_10 = g_v128f[3];
    __m128 v128f_11 = _mm_max_ps(v128f_10, v128f_10);
    __m512d v512d_12 = g_v512d[6];
    __m512i v512i_13 = _mm512_cvtpd_epi64(v512d_12);
    __m128i v128i_14 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_15 = _mm_add_epi64(v128i_14, v128i_14);
    __m128d v128d_16 = g_v128d[2];
    __m128d v128d_17 = _mm_div_pd(v128d_16, v128d_16);
    __m512i v512i_18 = _mm512_shuffle_i32x4(v512i_9, v512i_9, 0x1B);
    for (int i_19 = 0; i_19 < (int)(((unsigned long long)m16_7) & 7); i_19++) {
    __m512i v512i_20 = _mm512_mask_add_epi32(v512i_18, m16_1, v512i_13, v512i_8);
    __m512i v512i_21 = _mm512_set1_epi32(0x55);
    __m512i v512i_22 = _mm512_abs_epi32(v512i_13);
    __m512i v512i_23 = _mm512_broadcastmw_epi32(m16_1);
    __m512d v512d_24 = _mm512_unpackhi_pd(v512d_12, v512d_12);
    }
    __m512i v512i_25 = _mm512_cvtps_epi32(v512f_2);
    __m512 v512f_26 = _mm512_shuffle_f32x4(v512f_3, v512f_3, 0x4E);
    __m512 v512f_27 = _mm512_scalef_ps(v512f_3, v512f_3);
    __m256i v256i_28 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_29 = _mm256_sllv_epi32(v256i_28, v256i_28);
    __mmask64 m64_30 = (__mmask64)(g_m64[11] ^ 193);
    __mmask64 m64_31 = _kor_mask64(m64_30, m64_30);
    __m256i v256i_32 = _mm256_xor_si256(v256i_29, v256i_29);
    __mmask8 m8_33 = _kand_mask8(m8_6, m8_6);
    __m256 v256f_34 = _mm256_unpacklo_ps(p3, p3);
    __m512i v512i_35 = _mm512_sub_epi8(v512i_25, v512i_8);
    __m256d v256d_36 = _mm256_set1_pd(2.0);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512i v512i_37 = _mm512_mullo_epi32(v512i_18, v512i_13);
    __m256i v256i_38 = _mm256_sub_epi8(v256i_29, v256i_28);
    }
    g_v512f[1] = v512f_26;
    g_v128i[3] = v128i_15;
    g_v256h[0] = p1;
    g_m8[11] = m8_6;
    g_m16[1] = m16_1;
    return v512f_2;
}

__m512h torture_107(__m128h p0, __m128h p1, __m128d p2) {
    __m256 v256f_1 = g_v256f[9];
    __m256 v256f_2 = _mm256_min_ps(v256f_1, v256f_1);
    __m256 v256f_3 = _mm256_max_ps(v256f_2, v256f_2);
    __m512h v512h_4 = g_v512h[9];
    __m512h v512h_5 = _mm512_sqrt_ph(v512h_4);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512h v512h_7 = _mm512_cvtepi16_ph(v512i_6);
    __m256h v256h_8 = g_v256h[2];
    __m512 v512f_9 = _mm512_cvtxph_ps(v256h_8);
    __m128i v128i_10 = g_v128i[4];
    __m128i v128i_11 = _mm_sllv_epi32(v128i_10, v128i_10);
    __m256i v256i_12 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_13 = _mm256_add_epi32(v256i_12, v256i_12);
    __m512d v512d_14 = g_v512d[2];
    __m512d v512d_15 = _mm512_shuffle_f64x2(v512d_14, v512d_14, 0xEE);
    __m512 v512f_16 = _mm512_set1_ps(1.0f);
    __m512 v512f_17 = _mm512_scalef_ps(v512f_16, v512f_16);
    int s32_18 = 31;
    if ((s32_18) & 1ULL) {
    __m512i v512i_19 = _mm512_cvtph_epi16(v512h_4);
    __m512i v512i_20 = _mm512_cvtps_epi32(v512f_9);
    __m256 v256f_21 = _mm256_sub_ps(v256f_2, v256f_2);
    __m512i v512i_22 = _mm512_sllv_epi32(v512i_20, v512i_20);
    __m256i v256i_23 = _mm512_cvtepi64_epi32(v512i_19);
    __mmask32 m32_24 = _mm512_cmp_ph_mask(v512h_7, v512h_5, 1);
    }
    __m512i v512i_25 = _mm512_add_epi16(v512i_6, v512i_6);
    __m512h v512h_26 = _mm512_getexp_ph(v512h_5);
    __m256i v256i_27 = _mm256_and_si256(v256i_12, v256i_13);
    __mmask8 m8_28 = (__mmask8)(g_m8[5] ^ 173);
    __m512d v512d_29 = _mm512_mask_blend_pd(m8_28, v512d_15, v512d_14);
    __m256 v256f_30 = _mm256_set1_ps(1.0f);
    __m512 v512f_31 = _mm512_min_ps(v512f_9, v512f_16);
    __m128i v128i_32 = _mm_mullo_epi32(v128i_10, v128i_11);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m8_28) & 7); i_33++) {
    __mmask16 m16_34 = (__mmask16)(g_m16[10] ^ 38);
    __mmask16 m16_35 = _knot_mask16(m16_34);
    __m128i v128i_36 = _mm_shuffle_epi8(v128i_32, v128i_10);
    __m128 v128f_37 = _mm_loadu_ps(fbuf);
    __m128 v128f_38 = _mm_fmsub_ps(v128f_37, v128f_37, v128f_37);
    }
    g_v512f[9] = v512f_16;
    g_v256f[4] = v256f_2;
    g_v256i[10] = v256i_12;
    g_v128d[5] = p2;
    g_v512h[10] = v512h_7;
    return v512h_26;
}

__m512h torture_108(__m512i p0, __m256h p1, __m128i p2, __m128 p3) {
    __m512i v512i_1 = _mm512_add_epi64(p0, p0);
    __m256d v256d_2 = g_v256d[10];
    __m256d v256d_3 = _mm256_max_pd(v256d_2, v256d_2);
    __m128d v128d_4 = g_v128d[4];
    __m128d v128d_5 = _mm_sqrt_pd(v128d_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m256h v256h_7 = _mm512_cvtxps_ph(v512f_6);
    __m512i v512i_8 = _mm512_xor_si512(v512i_1, p0);
    __m256d v256d_9 = _mm256_div_pd(v256d_3, v256d_3);
    __m512i v512i_10 = _mm512_dpbusd_epi32(p0, v512i_8, v512i_1);
    __m128 v128f_11 = _mm_add_ps(p3, p3);
    int s32_12 = 15;
    for (int i_13 = 0; i_13 < (int)((s32_12) & 7); i_13++) {
    __m512d v512d_14 = _mm512_set1_pd(2.0);
    __m512d v512d_15 = _mm512_insertf64x2(v512d_14, v128d_4, 3);
    __m256 v256f_16 = g_v256f[3];
    __m256 v256f_17 = _mm256_unpacklo_ps(v256f_16, v256f_16);
    }
    __m256 v256f_18 = g_v256f[9];
    __m256 v256f_19 = _mm256_fmsub_ps(v256f_18, v256f_18, v256f_18);
    __m128d v128d_20 = _mm_max_pd(v128d_4, v128d_5);
    __m128d v128d_21 = _mm_unpackhi_pd(v128d_4, v128d_4);
    __m128 v128f_22 = _mm_set1_ps(1.0f);
    __m128i v128i_23 = _mm_add_epi64(p2, p2);
    __m512h v512h_24 = g_v512h[7];
    __m512h v512h_25 = _mm512_sqrt_ph(v512h_24);
    __m512i v512i_26 = _mm512_popcnt_epi64(v512i_10);
    __m256d v256d_27 = _mm256_unpackhi_pd(v256d_2, v256d_3);
    __m128 v128f_28 = _mm_fmsub_ps(p3, v128f_11, v128f_11);
    __mmask64 m64_29 = _mm512_movepi8_mask(v512i_1);
    __m128d v128d_30 = _mm_sub_pd(v128d_20, v128d_21);
    __mmask16 m16_31 = _mm512_cmp_epi32_mask(p0, v512i_8, 2);
    for (int i_32 = 0; i_32 < (int)(((unsigned long long)m16_31) & 7); i_32++) {
    __m512i v512i_33 = _mm512_ternarylogic_epi32(v512i_10, v512i_10, v512i_1, 0x96);
    __m256d v256d_34 = _mm256_fmadd_pd(v256d_2, v256d_9, v256d_2);
    }
    g_v256d[6] = v256d_3;
    g_m64[0] = m64_29;
    return v512h_24;
}

__mmask64 torture_109(__m512 p0, __m256 p1, __m512 p2) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_mullo_epi32(v128i_1, v128i_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[11] ^ 187);
    __mmask32 m32_4 = _mm512_kunpackw(m16_3, m16_3);
    __mmask8 m8_5 = (__mmask8)(g_m8[5] ^ 133);
    __mmask8 m8_6 = _kand_mask8(m8_5, m8_5);
    __m128 v128f_7 = g_v128f[10];
    __m128 v128f_8 = _mm_mul_ps(v128f_7, v128f_7);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_mullo_epi32(v512i_9, v512i_9);
    __m512d v512d_11 = g_v512d[0];
    __m512d v512d_12 = _mm512_sqrt_pd(v512d_11);
    __m512 v512f_13 = _mm512_rcp14_ps(p2);
    __m512 v512f_14 = _mm512_set1_ps(1.0f);
    __m512i v512i_15 = _mm512_add_epi8(v512i_9, v512i_9);
    __m512d v512d_16 = _mm512_set1_pd(2.0);
    __mmask16 m16_17 = _mm512_cmp_epu32_mask(v512i_10, v512i_15, 4);
    __m256i v256i_18 = _mm512_cvtepi64_epi32(v512i_9);
    __m512 v512f_19 = _mm512_max_ps(p0, v512f_14);
    __m512i v512i_20 = _mm512_add_epi32(v512i_10, v512i_10);
    __m512i v512i_21 = _mm512_conflict_epi32(v512i_15);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_17) & 7); i_22++) {
    __m512h v512h_23 = g_v512h[0];
    __m512h v512h_24 = _mm512_fmadd_ph(v512h_23, v512h_23, v512h_23);
    __m512d v512d_25 = _mm512_max_pd(v512d_16, v512d_12);
    __m128 v128f_26 = _mm_fmsub_ps(v128f_8, v128f_8, v128f_7);
    }
    __m512i v512i_27 = _mm512_conflict_epi32(v512i_9);
    __mmask16 m16_28 = _mm512_kunpackb(m8_5, m8_6);
    __m256d v256d_29 = g_v256d[6];
    __m256d v256d_30 = _mm256_sqrt_pd(v256d_29);
    __m512i v512i_31 = _mm512_popcnt_epi64(v512i_27);
    __m256d v256d_32 = _mm256_mul_pd(v256d_30, v256d_30);
    __m512i v512i_33 = _mm512_add_epi8(v512i_27, v512i_10);
    __m256d v256d_34 = _mm256_unpackhi_pd(v256d_29, v256d_30);
    if (((unsigned long long)m16_28) & 1ULL) {
    __m512i v512i_35 = _mm512_madd52lo_epu64(v512i_33, v512i_21, v512i_31);
    __m256d v256d_36 = _mm256_div_pd(v256d_32, v256d_30);
    __m256 v256f_37 = _mm256_add_ps(p1, p1);
    __m512d v512d_38 = _mm512_sqrt_pd(v512d_12);
    }
    g_v512f[10] = p2;
    g_v512i[5] = v512i_10;
    g_v128f[5] = v128f_8;
    g_v128i[1] = v128i_1;
    g_m16[6] = m16_17;
    __mmask64 m64_39 = (__mmask64)(g_m64[1] ^ 178);
    return m64_39;
}

__mmask32 torture_110(__m128h p0, __m256 p1) {
    __m128 v128f_1 = _mm_loadu_ps(fbuf);
    __m128 v128f_2 = _mm_xor_ps(v128f_1, v128f_1);
    __m512 v512f_3 = g_v512f[5];
    __m512 v512f_4 = _mm512_rsqrt14_ps(v512f_3);
    __mmask32 m32_5 = (__mmask32)(g_m32[9] ^ 68);
    __mmask32 m32_6 = _kxor_mask32(m32_5, m32_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_mullo_epi32(v512i_7, v512i_7);
    __m256 v256f_9 = _mm256_add_ps(p1, p1);
    __m256 v256f_10 = _mm256_div_ps(p1, p1);
    __m256 v256f_11 = _mm256_sub_ps(v256f_10, p1);
    __m512i v512i_12 = _mm512_rol_epi32(v512i_8, 5);
    __m512 v512f_13 = _mm512_xor_ps(v512f_4, v512f_4);
    __m256 v256f_14 = _mm256_fnmadd_ps(v256f_10, v256f_9, v256f_11);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m32_5) & 7); i_15++) {
    __m128i v128i_16 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_17 = _mm_add_epi8(v128i_16, v128i_16);
    __m512i v512i_18 = _mm512_rol_epi32(v512i_12, 5);
    }
    __m512 v512f_19 = _mm512_max_ps(v512f_13, v512f_4);
    __m512i v512i_20 = _mm512_dpwssd_epi32(v512i_7, v512i_7, v512i_7);
    __mmask16 m16_21 = (__mmask16)(g_m16[7] ^ 174);
    __mmask16 m16_22 = _kor_mask16(m16_21, m16_21);
    __m512 v512f_23 = _mm512_broadcast_f32x4(v128f_1);
    __m128i v128i_24 = g_v128i[6];
    __m128i v128i_25 = _mm_unpacklo_epi32(v128i_24, v128i_24);
    unsigned u32_26 = _cvtmask16_u32(m16_22);
    __m512i v512i_27 = _mm512_lzcnt_epi32(v512i_12);
    __m512 v512f_28 = _mm512_xor_ps(v512f_13, v512f_13);
    g_v512f[3] = v512f_19;
    g_v512i[2] = v512i_27;
    g_m16[8] = m16_22;
    g_m32[5] = m32_5;
    return m32_6;
}

__m256h torture_111(__m512 p0, __m256 p1, __m128i p2) {
    __m512 v512f_1 = _mm512_getexp_ps(p0);
    __m512d v512d_2 = _mm512_loadu_pd(dbuf);
    __m512d v512d_3 = _mm512_unpackhi_pd(v512d_2, v512d_2);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_5 = _mm512_cmp_epu32_mask(v512i_4, v512i_4, 4);
    __m256 v256f_6 = _mm256_sqrt_ps(p1);
    __m256 v256f_7 = _mm256_unpacklo_ps(v256f_6, v256f_6);
    __mmask32 m32_8 = (__mmask32)(g_m32[7] ^ 18);
    __mmask32 m32_9 = _kxor_mask32(m32_8, m32_8);
    __m256i v256i_10 = g_v256i[2];
    __m256i v256i_11 = _mm256_add_epi8(v256i_10, v256i_10);
    __mmask8 m8_12 = (__mmask8)(g_m8[5] ^ 50);
    __mmask8 m8_13 = _kand_mask8(m8_12, m8_12);
    __m256i v256i_14 = _mm256_add_epi64(v256i_11, v256i_11);
    __m256 v256f_15 = _mm256_sqrt_ps(v256f_7);
    __m512i v512i_16 = _mm512_shuffle_i32x4(v512i_4, v512i_4, 0x1B);
    __mmask16 m16_17 = _mm512_kunpackb(m8_13, m8_13);
    __m512i v512i_18 = _mm512_sub_epi16(v512i_16, v512i_4);
    __m128 v128f_19 = g_v128f[4];
    __m128 v128f_20 = _mm_and_ps(v128f_19, v128f_19);
    __m512h v512h_21 = g_v512h[10];
    __m512h v512h_22 = _mm512_mul_ph(v512h_21, v512h_21);
    __m512i v512i_23 = _mm512_ternarylogic_epi32(v512i_4, v512i_4, v512i_16, 0x96);
    __m256d v256d_24 = _mm256_set1_pd(2.0);
    __m512 v512f_25 = _mm512_maskz_mul_ps(m16_5, v512f_1, v512f_1);
    __m256d v256d_26 = _mm256_div_pd(v256d_24, v256d_24);
    __m128i v128i_27 = _mm_add_epi16(p2, p2);
    __m512h v512h_28 = _mm512_mul_ph(v512h_21, v512h_22);
    __m512i v512i_29 = _mm512_add_epi8(v512i_16, v512i_16);
    __m512h v512h_30 = _mm512_sqrt_ph(v512h_22);
    __m512 v512f_31 = _mm512_max_ps(v512f_1, v512f_25);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m512 v512f_32 = _mm512_fmadd_ps(v512f_31, v512f_25, v512f_31);
    __m512 v512f_33 = _mm512_mul_ps(p0, v512f_31);
    __m256i v256i_34 = _mm256_mullo_epi32(v256i_10, v256i_11);
    }
    g_v512d[11] = v512d_2;
    g_v512i[2] = v512i_16;
    g_v256f[4] = v256f_15;
    g_v128f[3] = v128f_19;
    g_v512h[3] = v512h_28;
    g_m16[1] = m16_17;
    __m256h v256h_35 = g_v256h[0];
    return v256h_35;
}

__mmask64 torture_112(__m128d p0, __m128d p1) {
    __m256 v256f_1 = g_v256f[4];
    __m256 v256f_2 = _mm256_mul_ps(v256f_1, v256f_1);
    __m512 v512f_3 = _mm512_set1_ps(1.0f);
    __m128 v128f_4 = _mm_set1_ps(1.0f);
    __m256h v256h_5 = g_v256h[1];
    __m512 v512f_6 = _mm512_cvtxph_ps(v256h_5);
    __m512i v512i_7 = g_v512i[7];
    __m512i v512i_8 = _mm512_permutex2var_epi32(v512i_7, v512i_7, v512i_7);
    __mmask16 m16_9 = (__mmask16)(g_m16[1] ^ 195);
    __mmask16 m16_10 = _kshiftli_mask16(m16_9, 2);
    __m128d v128d_11 = _mm_mul_pd(p0, p0);
    __m512 v512f_12 = _mm512_unpacklo_ps(v512f_6, v512f_3);
    __m512 v512f_13 = _mm512_getexp_ps(v512f_12);
    __m512i v512i_14 = _mm512_madd52lo_epu64(v512i_7, v512i_7, v512i_7);
    __m512 v512f_15 = _mm512_insertf32x8(v512f_12, v256f_1, 1);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_10) & 7); i_16++) {
    __m256d v256d_17 = g_v256d[3];
    __m256d v256d_18 = _mm256_add_pd(v256d_17, v256d_17);
    __mmask8 m8_19 = (__mmask8)(g_m8[6] ^ 13);
    __m512d v512d_20 = _mm512_loadu_pd(dbuf);
    __m512d v512d_21 = _mm512_mask_blend_pd(m8_19, v512d_20, v512d_20);
    __m256d v256d_22 = _mm256_add_pd(v256d_17, v256d_17);
    }
    __mmask16 m16_23 = _knot_mask16(m16_9);
    __m256 v256f_24 = _mm256_shuffle_ps(v256f_2, v256f_2, 0x1B);
    __m512d v512d_25 = g_v512d[9];
    __m512d v512d_26 = _mm512_shuffle_f64x2(v512d_25, v512d_25, 0xEE);
    __m512 v512f_27 = _mm512_add_ps(v512f_6, v512f_12);
    __m128i v128i_28 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_29 = _mm_sub_epi32(v128i_28, v128i_28);
    __m256i v256i_30 = _mm512_cvtepi64_epi32(v512i_7);
    __m512i v512i_31 = _mm512_abs_epi32(v512i_7);
    __m512i v512i_32 = _mm512_permutex2var_epi32(v512i_14, v512i_8, v512i_31);
    __m128i v128i_33 = _mm_and_si128(v128i_29, v128i_29);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_10) & 7); i_34++) {
    __m512i v512i_35 = _mm512_cvtps_epi32(v512f_15);
    __m512 v512f_36 = _mm512_and_ps(v512f_3, v512f_12);
    __m128 v128f_37 = _mm_and_ps(v128f_4, v128f_4);
    __m128 v128f_38 = _mm_div_ps(v128f_4, v128f_4);
    }
    g_v256f[10] = v256f_24;
    g_v256i[1] = v256i_30;
    g_v128d[5] = v128d_11;
    g_m16[0] = m16_9;
    __mmask64 m64_39 = (__mmask64)(g_m64[6] ^ 29);
    return m64_39;
}

__m128d torture_113(__m128h p0, __m256i p1) {
    __m128 v128f_1 = g_v128f[11];
    __m128 v128f_2 = _mm_unpacklo_ps(v128f_1, v128f_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_and_ps(v512f_3, v512f_3);
    unsigned u32_5 = 29;
    __mmask16 m16_6 = _cvtu32_mask16(u32_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_add_epi8(v512i_7, v512i_7);
    __m128i v128i_9 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_10 = _mm512_cvtepi8_epi32(v128i_9);
    __m512 v512f_11 = _mm512_shuffle_ps(v512f_3, v512f_4, 0x1B);
    __m512d v512d_12 = g_v512d[11];
    __m512d v512d_13 = _mm512_div_pd(v512d_12, v512d_12);
    __m256d v256d_14 = g_v256d[9];
    __m256d v256d_15 = _mm256_sub_pd(v256d_14, v256d_14);
    __m512 v512f_16 = _mm512_fmsub_ps(v512f_4, v512f_3, v512f_11);
    __m512h v512h_17 = g_v512h[4];
    __mmask32 m32_18 = _mm512_cmp_ph_mask(v512h_17, v512h_17, 1);
    if (((unsigned long long)m16_6) & 1ULL) {
    __mmask16 m16_19 = _mm512_cmp_epu32_mask(v512i_8, v512i_10, 4);
    __m256i v256i_20 = _mm256_add_epi8(p1, p1);
    __m512d v512d_21 = _mm512_shuffle_f64x2(v512d_12, v512d_13, 0xEE);
    __m256i v256i_22 = _mm256_shuffle_epi8(v256i_20, p1);
    __m128d v128d_23 = g_v128d[2];
    __m512d v512d_24 = _mm512_insertf64x2(v512d_12, v128d_23, 3);
    __m512 v512f_25 = _mm512_fnmadd_ps(v512f_3, v512f_4, v512f_16);
    }
    __m512i v512i_26 = _mm512_mask_add_epi32(v512i_8, m16_6, v512i_7, v512i_7);
    __m512i v512i_27 = _mm512_sllv_epi32(v512i_7, v512i_10);
    __m256d v256d_28 = _mm256_sub_pd(v256d_15, v256d_14);
    __m128i v128i_29 = _mm_add_epi8(v128i_9, v128i_9);
    __m256d v256d_30 = _mm256_unpackhi_pd(v256d_28, v256d_14);
    __m256 v256f_31 = _mm256_loadu_ps(fbuf);
    __m256 v256f_32 = _mm256_fmadd_ps(v256f_31, v256f_31, v256f_31);
    __m512d v512d_33 = _mm512_cvtph_pd(p0);
    __m512i v512i_34 = _mm512_cvtps_epi32(v512f_4);
    __m512 v512f_35 = _mm512_rsqrt14_ps(v512f_4);
    __mmask8 m8_36 = (__mmask8)(g_m8[4] ^ 147);
    __m512i v512i_37 = _mm512_broadcastmb_epi64(m8_36);
    __m512 v512f_38 = _mm512_min_ps(v512f_35, v512f_11);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_6) & 7); i_39++) {
    __m256i v256i_40 = _mm256_add_epi32(p1, p1);
    __m512h v512h_41 = _mm512_sqrt_ph(v512h_17);
    __m256 v256f_42 = _mm256_and_ps(v256f_31, v256f_32);
    __m128i v128i_43 = _mm_xor_si128(v128i_29, v128i_29);
    __m512 v512f_44 = _mm512_cvtepi32_ps(v512i_8);
    }
    g_v512h[7] = v512h_17;
    g_v128h[8] = p0;
    g_m8[4] = m8_36;
    __m128d v128d_45 = g_v128d[10];
    return v128d_45;
}

__m256d torture_114(__m512 p0, __m512i p1, __m512i p2, __m256h p3, __m512 p4) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_xor_ps(v256f_1, v256f_1);
    __m512 v512f_3 = _mm512_insertf32x8(p4, v256f_1, 1);
    __m512d v512d_4 = g_v512d[3];
    __m256d v256d_5 = _mm512_extractf64x4_pd(v512d_4, 1);
    __m256d v256d_6 = _mm256_max_pd(v256d_5, v256d_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[10] ^ 154);
    __mmask16 m16_8 = _kshiftri_mask16(m16_7, 1);
    __m256i v256i_9 = g_v256i[3];
    __m256i v256i_10 = _mm256_sllv_epi32(v256i_9, v256i_9);
    __m512d v512d_11 = _mm512_sub_pd(v512d_4, v512d_4);
    __m512i v512i_12 = _mm512_sub_epi32(p2, p1);
    __m512 v512f_13 = _mm512_shuffle_f32x4(v512f_3, v512f_3, 0x4E);
    __m256i v256i_14 = _mm256_mullo_epi32(v256i_9, v256i_10);
    __mmask16 m16_15 = _kshiftri_mask16(m16_7, 1);
    __m256i v256i_16 = _mm256_sub_epi64(v256i_9, v256i_10);
    __m512d v512d_17 = _mm512_cvtps_pd(v256f_2);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512i v512i_18 = _mm512_add_epi64(p2, p1);
    __m256 v256f_19 = _mm256_sqrt_ps(v256f_1);
    }
    __m128 v128f_20 = _mm_loadu_ps(fbuf);
    __m128 v128f_21 = _mm_min_ps(v128f_20, v128f_20);
    __m128 v128f_22 = _mm_fmadd_ps(v128f_21, v128f_20, v128f_21);
    __m512i v512i_23 = _mm512_lzcnt_epi32(v512i_12);
    __m512d v512d_24 = _mm512_add_pd(v512d_4, v512d_17);
    __m512h v512h_25 = g_v512h[6];
    __m512i v512i_26 = _mm512_cvtph_epi16(v512h_25);
    __m128i v128i_27 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_28 = _mm_xor_si128(v128i_27, v128i_27);
    __m512i v512i_29 = _mm512_add_epi32(p1, v512i_26);
    __m128i v128i_30 = _mm_set1_epi32(0x55);
    __m128h v128h_31 = g_v128h[5];
    __m512d v512d_32 = _mm512_cvtph_pd(v128h_31);
    __m512i v512i_33 = _mm512_unpacklo_epi32(v512i_26, v512i_12);
    __m256 v256f_34 = _mm256_mul_ps(v256f_2, v256f_1);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512i v512i_35 = _mm512_cvtpd_epi64(v512d_32);
    __m256d v256d_36 = _mm256_sub_pd(v256d_6, v256d_5);
    __m512d v512d_37 = _mm512_min_pd(v512d_24, v512d_24);
    }
    g_v512d[8] = v512d_32;
    g_v256f[10] = v256f_2;
    g_v128f[8] = v128f_21;
    g_v512h[0] = v512h_25;
    g_m16[6] = m16_15;
    return v256d_6;
}

__mmask64 torture_115(__m128d p0, __m128i p1, __m512d p2, __m128d p3, __m256 p4) {
    __m512d v512d_1 = _mm512_min_pd(p2, p2);
    __mmask8 m8_2 = (__mmask8)(g_m8[2] ^ 103);
    __mmask8 m8_3 = _kand_mask8(m8_2, m8_2);
    __m512d v512d_4 = _mm512_div_pd(v512d_1, v512d_1);
    __m256d v256d_5 = g_v256d[4];
    __m256d v256d_6 = _mm256_sqrt_pd(v256d_5);
    __m128i v128i_7 = _mm_abs_epi32(p1);
    __m256d v256d_8 = _mm512_extractf64x4_pd(p2, 1);
    __m128 v128f_9 = g_v128f[5];
    __m128 v128f_10 = _mm_fmadd_ps(v128f_9, v128f_9, v128f_9);
    __m512i v512i_11 = g_v512i[10];
    __mmask64 m64_12 = _mm512_cmp_epi8_mask(v512i_11, v512i_11, 1);
    __m256i v256i_13 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_14 = _mm256_shuffle_epi8(v256i_13, v256i_13);
    __m512h v512h_15 = _mm512_cvtepi16_ph(v512i_11);
    __m256i v256i_16 = _mm256_slli_epi32(v256i_14, 3);
    __m512i v512i_17 = _mm512_add_epi32(v512i_11, v512i_11);
    __m128i v128i_18 = _mm_or_si128(v128i_7, v128i_7);
    __m512 v512f_19 = g_v512f[10];
    __m512 v512f_20 = _mm512_shuffle_f32x4(v512f_19, v512f_19, 0x4E);
    __mmask16 m16_21 = (__mmask16)(g_m16[8] ^ 6);
    __m512i v512i_22 = _mm512_mask_blend_epi32(m16_21, v512i_11, v512i_11);
    __m512 v512f_23 = _mm512_shuffle_f32x4(v512f_19, v512f_20, 0x4E);
    __m512d v512d_24 = _mm512_mul_pd(p2, p2);
    __m512h v512h_25 = _mm512_rcp_ph(v512h_15);
    if (((unsigned long long)m16_21) & 1ULL) {
    __mmask32 m32_26 = _mm512_cmp_ph_mask(v512h_25, v512h_15, 1);
    __m256d v256d_27 = _mm256_fmadd_pd(v256d_5, v256d_8, v256d_6);
    __m512i v512i_28 = _mm512_add_epi32(v512i_11, v512i_22);
    }
    __mmask16 m16_29 = _mm512_cmp_epi32_mask(v512i_11, v512i_17, 2);
    __m512i v512i_30 = _mm512_xor_si512(v512i_22, v512i_17);
    __mmask32 m32_31 = _mm512_cmp_ph_mask(v512h_25, v512h_15, 1);
    __m512i v512i_32 = _mm512_mullo_epi32(v512i_11, v512i_11);
    __m512 v512f_33 = _mm512_min_ps(v512f_23, v512f_20);
    __m512d v512d_34 = _mm512_max_pd(v512d_4, v512d_24);
    __m512 v512f_35 = _mm512_insertf32x8(v512f_19, p4, 1);
    __m512i v512i_36 = _mm512_dpwssd_epi32(v512i_22, v512i_30, v512i_32);
    g_v512d[7] = p2;
    g_v256d[8] = v256d_6;
    g_v256i[3] = v256i_14;
    g_m8[1] = m8_3;
    g_m32[7] = m32_31;
    return m64_12;
}

__mmask8 torture_116(__m512h p0, __m256h p1, __m512 p2, __m128 p3, __m512d p4) {
    __m512i v512i_1 = g_v512i[5];
    __m512i v512i_2 = _mm512_add_epi32(v512i_1, v512i_1);
    __m128d v128d_3 = g_v128d[9];
    __m128d v128d_4 = _mm_sub_pd(v128d_3, v128d_3);
    __m256d v256d_5 = g_v256d[9];
    __m256d v256d_6 = _mm256_sub_pd(v256d_5, v256d_5);
    __m256i v256i_7 = g_v256i[1];
    __m256i v256i_8 = _mm256_xor_si256(v256i_7, v256i_7);
    __mmask16 m16_9 = (__mmask16)(g_m16[9] ^ 218);
    __mmask16 m16_10 = _kshiftli_mask16(m16_9, 2);
    __mmask8 m8_11 = (__mmask8)(g_m8[6] ^ 156);
    __mmask16 m16_12 = _mm512_kunpackb(m8_11, m8_11);
    __m128 v128f_13 = _mm_and_ps(p3, p3);
    __m512i v512i_14 = _mm512_slli_epi32(v512i_2, 3);
    __m512d v512d_15 = _mm512_div_pd(p4, p4);
    __m256i v256i_16 = _mm256_sub_epi64(v256i_7, v256i_7);
    __m512i v512i_17 = _mm512_cvtph_epi16(p0);
    __m128i v128i_18 = _mm512_cvtepi32_epi8(v512i_1);
    __mmask16 m16_19 = _kxor_mask16(m16_9, m16_12);
    __m256i v256i_20 = _mm256_sub_epi64(v256i_16, v256i_8);
    __m256d v256d_21 = _mm256_fmadd_pd(v256d_5, v256d_6, v256d_6);
    __m512i v512i_22 = _mm512_xor_si512(v512i_14, v512i_2);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m256 v256f_23 = _mm256_loadu_ps(fbuf);
    __m256 v256f_24 = _mm256_unpacklo_ps(v256f_23, v256f_23);
    __m128d v128d_25 = _mm_min_pd(v128d_4, v128d_3);
    __m128 v128f_26 = _mm_fmsub_ps(p3, p3, p3);
    __m128i v128i_27 = _mm_unpacklo_epi32(v128i_18, v128i_18);
    __mmask32 m32_28 = _mm512_kunpackw(m16_19, m16_10);
    }
    __m256 v256f_29 = _mm256_loadu_ps(fbuf);
    __m256 v256f_30 = _mm256_sub_ps(v256f_29, v256f_29);
    __m128i v128i_31 = _mm_xor_si128(v128i_18, v128i_18);
    __m512d v512d_32 = _mm512_max_pd(v512d_15, v512d_15);
    __m512i v512i_33 = _mm512_conflict_epi32(v512i_1);
    __m128d v128d_34 = _mm_unpackhi_pd(v128d_4, v128d_3);
    __m256d v256d_35 = _mm256_unpackhi_pd(v256d_21, v256d_21);
    __m512 v512f_36 = _mm512_maskz_mul_ps(m16_12, p2, p2);
    for (int i_37 = 0; i_37 < (int)(((unsigned long long)m16_19) & 7); i_37++) {
    __mmask32 m32_38 = _mm512_kunpackw(m16_12, m16_19);
    __m128 v128f_39 = _mm_mul_ps(v128f_13, p3);
    }
    g_v512f[1] = p2;
    g_v512d[4] = p4;
    g_v256f[8] = v256f_29;
    g_v128f[10] = v128f_13;
    g_v128i[10] = v128i_18;
    g_v512h[5] = p0;
    g_m8[11] = m8_11;
    return m8_11;
}

__m256i torture_117(__m256d p0, __m256 p1, __m256 p2, __m128h p3) {
    __m512i v512i_1 = g_v512i[2];
    __m512i v512i_2 = _mm512_abs_epi32(v512i_1);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_div_ps(v128f_3, v128f_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512i v512i_6 = _mm512_cvtps_epi32(v512f_5);
    __m512i v512i_7 = _mm512_abs_epi32(v512i_1);
    __m256i v256i_8 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_9 = _mm256_add_epi16(v256i_8, v256i_8);
    __m512i v512i_10 = _mm512_add_epi64(v512i_6, v512i_2);
    __m512d v512d_11 = g_v512d[4];
    __m512d v512d_12 = _mm512_sqrt_pd(v512d_11);
    __m512i v512i_13 = _mm512_cvtpd_epi64(v512d_12);
    __m256i v256i_14 = _mm256_sub_epi32(v256i_9, v256i_9);
    __m256i v256i_15 = _mm256_set1_epi32(0x55);
    __m128d v128d_16 = g_v128d[7];
    __m128d v128d_17 = _mm_sqrt_pd(v128d_16);
    __m512 v512f_18 = _mm512_sub_ps(v512f_5, v512f_5);
    __m512 v512f_19 = _mm512_and_ps(v512f_18, v512f_18);
    __m512i v512i_20 = _mm512_slli_epi32(v512i_10, 3);
    __m256i v256i_21 = _mm256_and_si256(v256i_8, v256i_15);
    int s32_22 = 52;
    for (int i_23 = 0; i_23 < (int)((s32_22) & 7); i_23++) {
    __m512i v512i_24 = _mm512_add_epi16(v512i_20, v512i_20);
    __mmask16 m16_25 = (__mmask16)(g_m16[10] ^ 18);
    __m512i v512i_26 = _mm512_mask_blend_epi32(m16_25, v512i_20, v512i_24);
    __m256i v256i_27 = _mm256_slli_epi32(v256i_9, 3);
    __m512 v512f_28 = _mm512_scalef_ps(v512f_19, v512f_18);
    __mmask16 m16_29 = _kshiftli_mask16(m16_25, 2);
    }
    __m512i v512i_30 = _mm512_xor_si512(v512i_6, v512i_20);
    __mmask16 m16_31 = (__mmask16)(g_m16[7] ^ 37);
    unsigned u32_32 = _cvtmask16_u32(m16_31);
    __m512h v512h_33 = g_v512h[4];
    __m512h v512h_34 = _mm512_fmadd_ph(v512h_33, v512h_33, v512h_33);
    __m128i v128i_35 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_36 = _mm_add_epi32(v128i_35, v128i_35);
    __m256 v256f_37 = _mm256_sqrt_ps(p1);
    __m512d v512d_38 = _mm512_cvtph_pd(p3);
    __m512i v512i_39 = _mm512_sub_epi16(v512i_20, v512i_1);
    __m128i v128i_40 = _mm_abs_epi32(v128i_35);
    g_v512d[2] = v512d_12;
    g_v256f[7] = p2;
    g_v128d[5] = v128d_17;
    g_v512h[3] = v512h_34;
    g_m16[4] = m16_31;
    return v256i_21;
}

__m256d torture_118(__m512i p0, __m128 p1, __m256h p2, __m128d p3) {
    __m512i v512i_1 = _mm512_sub_epi16(p0, p0);
    __mmask8 m8_2 = (__mmask8)(g_m8[10] ^ 166);
    __mmask8 m8_3 = _kand_mask8(m8_2, m8_2);
    __m512i v512i_4 = _mm512_sub_epi16(p0, p0);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512i v512i_6 = _mm512_cvtps_epi32(v512f_5);
    __m128i v128i_7 = g_v128i[11];
    __m128i v128i_8 = _mm_sllv_epi32(v128i_7, v128i_7);
    __m512 v512f_9 = _mm512_getexp_ps(v512f_5);
    __m128i v128i_10 = _mm_add_epi32(v128i_8, v128i_7);
    __m256 v256f_11 = _mm256_loadu_ps(fbuf);
    __m256 v256f_12 = _mm256_unpacklo_ps(v256f_11, v256f_11);
    __m512i v512i_13 = _mm512_conflict_epi32(v512i_6);
    __m128 v128f_14 = _mm_add_ps(p1, p1);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m8_2) & 7); i_15++) {
    __m512d v512d_16 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_17 = _mm512_cmp_pd_mask(v512d_16, v512d_16, _CMP_GE_OQ);
    __m256i v256i_18 = g_v256i[3];
    __m256i v256i_19 = _mm256_sub_epi64(v256i_18, v256i_18);
    __m512i v512i_20 = _mm512_xor_si512(v512i_13, v512i_4);
    __m256i v256i_21 = _mm256_shuffle_epi8(v256i_18, v256i_18);
    }
    __m128 v128f_22 = _mm_fnmadd_ps(v128f_14, p1, p1);
    __m512d v512d_23 = _mm512_cvtps_pd(v256f_11);
    __m256h v256h_24 = _mm512_cvtxps_ph(v512f_9);
    __m256 v256f_25 = _mm256_sub_ps(v256f_12, v256f_12);
    __m512d v512d_26 = _mm512_shuffle_f64x2(v512d_23, v512d_23, 0xEE);
    __m512h v512h_27 = g_v512h[8];
    __m512h v512h_28 = _mm512_sqrt_ph(v512h_27);
    __m512i v512i_29 = _mm512_slli_epi32(p0, 3);
    if (((unsigned long long)m8_2) & 1ULL) {
    __m512i v512i_30 = _mm512_slli_epi32(v512i_1, 3);
    __m512i v512i_31 = _mm512_lzcnt_epi32(v512i_1);
    __m512i v512i_32 = _mm512_add_epi32(v512i_4, v512i_13);
    __m256d v256d_33 = g_v256d[11];
    __m256d v256d_34 = _mm256_sqrt_pd(v256d_33);
    __mmask32 m32_35 = _mm512_cmp_ph_mask(v512h_28, v512h_28, 1);
    }
    g_v512d[2] = v512d_26;
    g_v128f[6] = v128f_14;
    g_v128d[6] = p3;
    g_v256h[11] = v256h_24;
    __m256d v256d_36 = g_v256d[3];
    return v256d_36;
}

__m256d torture_119(__m128h p0, __m256h p1, __m512h p2) {
    __m128i v128i_1 = g_v128i[4];
    __m128i v128i_2 = _mm_shuffle_epi8(v128i_1, v128i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512i v512i_4 = _mm512_cvtps_epi32(v512f_3);
    __m128 v128f_5 = _mm512_extractf32x4_ps(v512f_3, 2);
    __m256i v256i_6 = g_v256i[10];
    __m256i v256i_7 = _mm256_and_si256(v256i_6, v256i_6);
    __m512 v512f_8 = _mm512_fmadd_ps(v512f_3, v512f_3, v512f_3);
    __mmask16 m16_9 = (__mmask16)(g_m16[7] ^ 73);
    __mmask16 m16_10 = _kshiftri_mask16(m16_9, 1);
    __mmask16 m16_11 = _mm512_cmp_ps_mask(v512f_8, v512f_8, _CMP_LT_OQ);
    __m512d v512d_12 = g_v512d[10];
    __m256 v256f_13 = _mm512_cvtpd_ps(v512d_12);
    __m512d v512d_14 = _mm512_mul_pd(v512d_12, v512d_12);
    __m128i v128i_15 = _mm_unpacklo_epi32(v128i_1, v128i_2);
    if (((unsigned long long)m16_11) & 1ULL) {
    __m512 v512f_16 = _mm512_insertf32x8(v512f_8, v256f_13, 1);
    __m512d v512d_17 = _mm512_mul_pd(v512d_14, v512d_12);
    __m512i v512i_18 = _mm512_cvtps_epi32(v512f_16);
    }
    __m512i v512i_19 = _mm512_xor_si512(v512i_4, v512i_4);
    __m256d v256d_20 = g_v256d[0];
    __m256d v256d_21 = _mm256_sqrt_pd(v256d_20);
    __m512i v512i_22 = _mm512_xor_si512(v512i_4, v512i_4);
    __m256 v256f_23 = _mm256_add_ps(v256f_13, v256f_13);
    g_v512d[6] = v512d_14;
    g_v256d[5] = v256d_20;
    g_v256h[1] = p1;
    g_v128h[2] = p0;
    g_m16[10] = m16_10;
    return v256d_21;
}

__mmask16 torture_120(__m256h p0, __m512i p1, __m128h p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_2 = _mm512_fpclass_ps_mask(v512f_1, 0x3);
    __m256d v256d_3 = g_v256d[2];
    __m256d v256d_4 = _mm256_min_pd(v256d_3, v256d_3);
    __m128 v128f_5 = _mm_loadu_ps(fbuf);
    __m128 v128f_6 = _mm_and_ps(v128f_5, v128f_5);
    __m128 v128f_7 = _mm_sub_ps(v128f_5, v128f_5);
    __m128i v128i_8 = _mm512_cvtepi32_epi8(p1);
    __m512 v512f_9 = _mm512_scalef_ps(v512f_1, v512f_1);
    __m256 v256f_10 = g_v256f[8];
    __m512 v512f_11 = _mm512_insertf32x8(v512f_1, v256f_10, 1);
    __m512i v512i_12 = _mm512_popcnt_epi64(p1);
    __mmask16 m16_13 = _mm512_test_epi32_mask(v512i_12, p1);
    __m512 v512f_14 = _mm512_insertf32x8(v512f_11, v256f_10, 1);
    __m128d v128d_15 = g_v128d[6];
    __m128d v128d_16 = _mm_sub_pd(v128d_15, v128d_15);
    __m512i v512i_17 = _mm512_abs_epi32(p1);
    __m256i v256i_18 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_19 = _mm256_add_epi64(v256i_18, v256i_18);
    __m256i v256i_20 = _mm256_unpacklo_epi32(v256i_19, v256i_18);
    __m512i v512i_21 = _mm512_permutexvar_epi32(v512i_17, v512i_12);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m512i v512i_22 = _mm512_sllv_epi32(p1, v512i_12);
    __m512 v512f_23 = _mm512_add_ps(v512f_14, v512f_11);
    __m256i v256i_24 = _mm256_sub_epi64(v256i_18, v256i_20);
    }
    __m512h v512h_25 = _mm512_cvtepi16_ph(p1);
    __m512d v512d_26 = g_v512d[6];
    __m256 v256f_27 = _mm512_cvtpd_ps(v512d_26);
    __m256 v256f_28 = _mm256_min_ps(v256f_27, v256f_10);
    __m128d v128d_29 = _mm_min_pd(v128d_16, v128d_15);
    __m256i v256i_30 = _mm256_mullo_epi32(v256i_19, v256i_20);
    g_v512f[4] = v512f_14;
    g_v512d[7] = v512d_26;
    g_v256f[7] = v256f_27;
    g_v256d[2] = v256d_3;
    g_v128h[10] = p2;
    return m16_2;
}

__m512d torture_121(__m256h p0, __m256 p1) {
    __m128 v128f_1 = _mm_loadu_ps(fbuf);
    __m128 v128f_2 = _mm_fmadd_ps(v128f_1, v128f_1, v128f_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_add_ps(v512f_3, v512f_3);
    __m256i v256i_5 = g_v256i[8];
    __m256i v256i_6 = _mm256_mullo_epi32(v256i_5, v256i_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_sub_epi8(v512i_7, v512i_7);
    __m128i v128i_9 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_10 = _mm512_cvtepi8_epi32(v128i_9);
    __mmask16 m16_11 = _mm512_cmp_epu32_mask(v512i_8, v512i_8, 4);
    __m512i v512i_12 = _mm512_add_epi8(v512i_10, v512i_8);
    __m512 v512f_13 = _mm512_min_ps(v512f_4, v512f_3);
    __m128d v128d_14 = g_v128d[5];
    __m128d v128d_15 = _mm_sqrt_pd(v128d_14);
    __m512i v512i_16 = _mm512_ternarylogic_epi32(v512i_10, v512i_8, v512i_12, 0x96);
    if (((unsigned long long)m16_11) & 1ULL) {
    __m512i v512i_17 = _mm512_conflict_epi32(v512i_12);
    __m512h v512h_18 = g_v512h[7];
    __m512h v512h_19 = _mm512_fmadd_ph(v512h_18, v512h_18, v512h_18);
    __mmask16 m16_20 = _kand_mask16(m16_11, m16_11);
    }
    __m128 v128f_21 = _mm_shuffle_ps(v128f_2, v128f_1, 0x1B);
    __m128 v128f_22 = _mm_fnmadd_ps(v128f_1, v128f_21, v128f_1);
    __m512 v512f_23 = _mm512_max_ps(v512f_3, v512f_13);
    __m256d v256d_24 = g_v256d[4];
    __m256d v256d_25 = _mm256_mul_pd(v256d_24, v256d_24);
    __m256 v256f_26 = _mm256_fnmadd_ps(p1, p1, p1);
    __m512 v512f_27 = _mm512_shuffle_ps(v512f_23, v512f_3, 0x1B);
    __m128i v128i_28 = _mm_sub_epi8(v128i_9, v128i_9);
    __m512i v512i_29 = _mm512_conflict_epi32(v512i_16);
    __m512i v512i_30 = _mm512_sub_epi32(v512i_29, v512i_8);
    __mmask16 m16_31 = _kand_mask16(m16_11, m16_11);
    __m512i v512i_32 = _mm512_shuffle_i32x4(v512i_29, v512i_30, 0x1B);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_11) & 7); i_33++) {
    __mmask32 m32_34 = _mm512_kunpackw(m16_31, m16_11);
    __mmask16 m16_35 = _mm512_fpclass_ps_mask(v512f_13, 0x3);
    }
    g_v512f[0] = v512f_27;
    g_v512i[9] = v512i_7;
    g_v128f[10] = v128f_2;
    g_v128d[1] = v128d_15;
    __m512d v512d_36 = g_v512d[8];
    return v512d_36;
}

__m256 torture_122(__m256i p0, __m256d p1, __m256i p2, __m256i p3) {
    __m512d v512d_1 = _mm512_set1_pd(2.0);
    __m256i v256i_2 = _mm256_mullo_epi32(p3, p3);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_4 = _mm512_broadcast_i32x4(v128i_3);
    __mmask16 m16_5 = _mm512_cmp_epu32_mask(v512i_4, v512i_4, 4);
    __mmask16 m16_6 = _mm512_cmp_epu32_mask(v512i_4, v512i_4, 4);
    __m512 v512f_7 = _mm512_loadu_ps(fbuf);
    __m512 v512f_8 = _mm512_max_ps(v512f_7, v512f_7);
    __m512d v512d_9 = _mm512_max_pd(v512d_1, v512d_1);
    __m512h v512h_10 = g_v512h[8];
    __m512h v512h_11 = _mm512_mul_ph(v512h_10, v512h_10);
    __m256 v256f_12 = _mm256_loadu_ps(fbuf);
    __m256 v256f_13 = _mm256_sqrt_ps(v256f_12);
    __m256d v256d_14 = _mm256_mul_pd(p1, p1);
    __m128i v128i_15 = _mm_add_epi32(v128i_3, v128i_3);
    __mmask8 m8_16 = (__mmask8)(g_m8[0] ^ 10);
    __m512d v512d_17 = _mm512_mask_blend_pd(m8_16, v512d_1, v512d_9);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m256i v256i_18 = _mm256_mullo_epi32(p0, p3);
    __m256 v256f_19 = _mm256_min_ps(v256f_13, v256f_12);
    __m512h v512h_20 = _mm512_rcp_ph(v512h_10);
    __m512i v512i_21 = _mm512_sllv_epi32(v512i_4, v512i_4);
    __m128 v128f_22 = _mm512_extractf32x4_ps(v512f_7, 2);
    }
    __m256i v256i_23 = _mm256_sub_epi32(v256i_2, p3);
    __m512i v512i_24 = _mm512_sub_epi16(v512i_4, v512i_4);
    __m512i v512i_25 = _mm512_abs_epi32(v512i_24);
    __m512d v512d_26 = _mm512_min_pd(v512d_1, v512d_1);
    __m512 v512f_27 = _mm512_getexp_ps(v512f_8);
    unsigned u32_28 = _cvtmask16_u32(m16_5);
    __m256 v256f_29 = _mm256_xor_ps(v256f_12, v256f_13);
    __m512i v512i_30 = _mm512_slli_epi32(v512i_4, 3);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m128i v128i_31 = _mm_sllv_epi32(v128i_15, v128i_15);
    __m128 v128f_32 = g_v128f[8];
    __m128 v128f_33 = _mm_max_ps(v128f_32, v128f_32);
    __m128 v128f_34 = _mm_sub_ps(v128f_32, v128f_32);
    }
    g_v512d[2] = v512d_9;
    g_m8[3] = m8_16;
    g_m16[11] = m16_6;
    return v256f_29;
}

__m256h torture_123(__m128d p0, __m512i p1) {
    __m256h v256h_1 = g_v256h[10];
    __m512 v512f_2 = _mm512_cvtxph_ps(v256h_1);
    __m512h v512h_3 = g_v512h[10];
    __m512h v512h_4 = _mm512_fmadd_ph(v512h_3, v512h_3, v512h_3);
    __m512i v512i_5 = _mm512_sllv_epi32(p1, p1);
    __m512i v512i_6 = _mm512_or_si512(v512i_5, p1);
    __mmask8 m8_7 = (__mmask8)(g_m8[0] ^ 164);
    __m512i v512i_8 = _mm512_broadcastmb_epi64(m8_7);
    __m512 v512f_9 = _mm512_unpacklo_ps(v512f_2, v512f_2);
    __mmask64 m64_10 = (__mmask64)(g_m64[6] ^ 21);
    __mmask64 m64_11 = _kor_mask64(m64_10, m64_10);
    __m256i v256i_12 = g_v256i[10];
    __m256i v256i_13 = _mm256_shuffle_epi8(v256i_12, v256i_12);
    __m512d v512d_14 = _mm512_loadu_pd(dbuf);
    __m512d v512d_15 = _mm512_sqrt_pd(v512d_14);
    __m512i v512i_16 = _mm512_xor_si512(v512i_8, v512i_6);
    __m256i v256i_17 = _mm256_add_epi32(v256i_12, v256i_12);
    if (((unsigned long long)m8_7) & 1ULL) {
    __mmask16 m16_18 = _mm512_testn_epi32_mask(v512i_16, v512i_16);
    __mmask32 m32_19 = _mm512_kunpackw(m16_18, m16_18);
    __m256d v256d_20 = g_v256d[7];
    __m256d v256d_21 = _mm256_unpackhi_pd(v256d_20, v256d_20);
    __m128h v128h_22 = _mm512_cvtpd_ph(v512d_14);
    __m512 v512f_23 = _mm512_max_ps(v512f_2, v512f_9);
    }
    __m128 v128f_24 = g_v128f[0];
    __m128 v128f_25 = _mm_add_ps(v128f_24, v128f_24);
    __m512 v512f_26 = _mm512_max_ps(v512f_2, v512f_9);
    __m128 v128f_27 = _mm_shuffle_ps(v128f_24, v128f_25, 0x1B);
    __m256 v256f_28 = _mm256_loadu_ps(fbuf);
    __m256 v256f_29 = _mm256_fmsub_ps(v256f_28, v256f_28, v256f_28);
    __m256 v256f_30 = _mm512_cvtpd_ps(v512d_14);
    __mmask16 m16_31 = _mm512_fpclass_ps_mask(v512f_2, 0x3);
    __mmask8 m8_32 = _mm512_cmp_pd_mask(v512d_14, v512d_14, _CMP_GE_OQ);
    __m128i v128i_33 = _mm512_cvtepi32_epi8(v512i_16);
    __m512i v512i_34 = _mm512_cvtps_epi32(v512f_26);
    __m128 v128f_35 = _mm_fmsub_ps(v128f_25, v128f_24, v128f_25);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_31) & 7); i_36++) {
    __m128i v128i_37 = _mm_sub_epi32(v128i_33, v128i_33);
    __m512 v512f_38 = _mm512_add_ps(v512f_2, v512f_26);
    }
    g_v256f[9] = v256f_28;
    g_v128i[6] = v128i_33;
    g_v512h[8] = v512h_4;
    g_m8[0] = m8_32;
    g_m16[0] = m16_31;
    return v256h_1;
}

__mmask64 torture_124(__m512d p0, __m256i p1, __m512 p2, __m512h p3, __m256 p4) {
    __m256i v256i_1 = _mm256_slli_epi32(p1, 3);
    __m128i v128i_2 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_3 = _mm_add_epi16(v128i_2, v128i_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[10] ^ 23);
    __m512i v512i_5 = _mm512_broadcastmw_epi32(m16_4);
    __m512 v512f_6 = _mm512_set1_ps(1.0f);
    __m128 v128f_7 = g_v128f[8];
    __m128 v128f_8 = _mm_xor_ps(v128f_7, v128f_7);
    __mmask16 m16_9 = _mm512_cmp_epi32_mask(v512i_5, v512i_5, 2);
    __m128 v128f_10 = _mm_sub_ps(v128f_7, v128f_8);
    __m256i v256i_11 = _mm256_xor_si256(p1, p1);
    __m128 v128f_12 = _mm_sub_ps(v128f_7, v128f_8);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m256d v256d_13 = g_v256d[7];
    __m256d v256d_14 = _mm256_add_pd(v256d_13, v256d_13);
    __m256i v256i_15 = _mm256_set1_epi32(0x55);
    __mmask16 m16_16 = _kor_mask16(m16_9, m16_9);
    }
    __m256 v256f_17 = _mm256_fmsub_ps(p4, p4, p4);
    __m512 v512f_18 = _mm512_div_ps(p2, p2);
    __m512i v512i_19 = _mm512_madd52lo_epu64(v512i_5, v512i_5, v512i_5);
    __m512 v512f_20 = _mm512_fnmadd_ps(v512f_6, v512f_6, v512f_18);
    __m512d v512d_21 = _mm512_shuffle_f64x2(p0, p0, 0xEE);
    __m512 v512f_22 = _mm512_fmsub_ps(v512f_6, v512f_18, v512f_18);
    __m512h v512h_23 = _mm512_fmsub_ph(p3, p3, p3);
    __m512i v512i_24 = _mm512_shuffle_i32x4(v512i_5, v512i_5, 0x1B);
    __m128d v128d_25 = g_v128d[6];
    __m128d v128d_26 = _mm_min_pd(v128d_25, v128d_25);
    __m256 v256f_27 = _mm256_fmsub_ps(v256f_17, p4, v256f_17);
    __m128i v128i_28 = _mm_add_epi8(v128i_2, v128i_2);
    g_v512f[9] = v512f_18;
    g_v512d[11] = p0;
    g_v256i[4] = v256i_11;
    g_v128f[9] = v128f_8;
    g_v512h[5] = v512h_23;
    g_m16[0] = m16_9;
    __mmask64 m64_29 = (__mmask64)(g_m64[11] ^ 64);
    return m64_29;
}

__m256i torture_125(__m256d p0, __m256d p1, __m128i p2) {
    __m512i v512i_1 = g_v512i[5];
    __m512i v512i_2 = _mm512_and_si512(v512i_1, v512i_1);
    __m128i v128i_3 = _mm_sub_epi16(p2, p2);
    __m128i v128i_4 = _mm_xor_si128(p2, v128i_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_6 = (__mmask16)(g_m16[11] ^ 158);
    __m512 v512f_7 = _mm512_mask3_fmadd_ps(v512f_5, v512f_5, v512f_5, m16_6);
    __m128i v128i_8 = _mm_sub_epi16(v128i_3, v128i_4);
    __m512i v512i_9 = _mm512_shuffle_epi8(v512i_2, v512i_2);
    unsigned u32_10 = _cvtmask16_u32(m16_6);
    __m512 v512f_11 = _mm512_add_ps(v512f_7, v512f_5);
    __m512h v512h_12 = g_v512h[3];
    __m512h v512h_13 = _mm512_fmsub_ph(v512h_12, v512h_12, v512h_12);
    __m512i v512i_14 = _mm512_add_epi8(v512i_1, v512i_1);
    __m256d v256d_15 = _mm256_fmadd_pd(p0, p1, p1);
    __m512 v512f_16 = _mm512_shuffle_ps(v512f_11, v512f_11, 0x1B);
    __m256i v256i_17 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_18 = _mm256_add_epi64(v256i_17, v256i_17);
    __mmask8 m8_19 = (__mmask8)(g_m8[5] ^ 130);
    __m512i v512i_20 = _mm512_broadcastmb_epi64(m8_19);
    __m512h v512h_21 = _mm512_cvtepi16_ph(v512i_2);
    __m512d v512d_22 = _mm512_loadu_pd(dbuf);
    __m256d v256d_23 = _mm512_extractf64x4_pd(v512d_22, 1);
    __m128 v128f_24 = _mm_loadu_ps(fbuf);
    __m128 v128f_25 = _mm_add_ps(v128f_24, v128f_24);
    __m512i v512i_26 = _mm512_permutexvar_epi32(v512i_1, v512i_9);
    if (((unsigned long long)m16_6) & 1ULL) {
    __mmask16 m16_27 = _cvtu32_mask16(u32_10);
    __m512h v512h_28 = _mm512_getexp_ph(v512h_21);
    __m512 v512f_29 = _mm512_broadcast_f32x4(v128f_24);
    }
    __m128i v128i_30 = _mm_unpacklo_epi32(v128i_4, v128i_4);
    __m128 v128f_31 = _mm512_extractf32x4_ps(v512f_5, 2);
    __m512i v512i_32 = _mm512_permutex2var_epi32(v512i_1, v512i_2, v512i_9);
    __m256i v256i_33 = _mm256_or_si256(v256i_17, v256i_17);
    __m512 v512f_34 = _mm512_div_ps(v512f_11, v512f_16);
    __m128i v128i_35 = _mm_add_epi8(v128i_4, v128i_8);
    __m128 v128f_36 = _mm_unpacklo_ps(v128f_24, v128f_31);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m256i v256i_37 = _mm256_add_epi64(v256i_18, v256i_17);
    __m256h v256h_38 = _mm512_cvtxps_ph(v512f_34);
    __mmask32 m32_39 = (__mmask32)(g_m32[3] ^ 98);
    __mmask32 m32_40 = _kxor_mask32(m32_39, m32_39);
    __m256d v256d_41 = _mm256_mul_pd(p1, v256d_15);
    }
    g_v512i[7] = v512i_32;
    g_v256d[0] = p0;
    g_v128i[9] = v128i_30;
    return v256i_33;
}

__mmask16 torture_126(__m256 p0, __m512d p1, __m256h p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512 v512f_2 = _mm512_cvtepi32_ps(v512i_1);
    __m128i v128i_3 = g_v128i[8];
    __m128i v128i_4 = _mm_add_epi16(v128i_3, v128i_3);
    __m512i v512i_5 = _mm512_permutexvar_epi32(v512i_1, v512i_1);
    __m512 v512f_6 = _mm512_sqrt_ps(v512f_2);
    __m256 v256f_7 = _mm256_sub_ps(p0, p0);
    __m256i v256i_8 = g_v256i[7];
    __m256i v256i_9 = _mm256_add_epi64(v256i_8, v256i_8);
    __m128i v128i_10 = _mm_add_epi32(v128i_4, v128i_3);
    __m512 v512f_11 = _mm512_roundscale_ps(v512f_6, 0x10);
    __m128 v128f_12 = _mm_loadu_ps(fbuf);
    __m128 v128f_13 = _mm_fmsub_ps(v128f_12, v128f_12, v128f_12);
    int s32_14 = 89;
    for (int i_15 = 0; i_15 < (int)((s32_14) & 7); i_15++) {
    __m128 v128f_16 = _mm_xor_ps(v128f_13, v128f_13);
    __m128i v128i_17 = _mm_or_si128(v128i_4, v128i_3);
    }
    __m256d v256d_18 = g_v256d[11];
    __m256d v256d_19 = _mm256_div_pd(v256d_18, v256d_18);
    __m512 v512f_20 = _mm512_set1_ps(1.0f);
    __m512i v512i_21 = _mm512_add_epi64(v512i_1, v512i_1);
    __mmask16 m16_22 = (__mmask16)(g_m16[7] ^ 46);
    __mmask16 m16_23 = _knot_mask16(m16_22);
    __m512i v512i_24 = _mm512_and_si512(v512i_1, v512i_21);
    __m256d v256d_25 = _mm256_set1_pd(2.0);
    __m256 v256f_26 = _mm256_fmadd_ps(p0, v256f_7, p0);
    __m256 v256f_27 = _mm256_xor_ps(v256f_26, v256f_26);
    __m512d v512d_28 = _mm512_sub_pd(p1, p1);
    for (int i_29 = 0; i_29 < (int)(((unsigned long long)m16_22) & 7); i_29++) {
    __m128 v128f_30 = _mm_add_ps(v128f_13, v128f_12);
    __m256d v256d_31 = _mm512_extractf64x4_pd(v512d_28, 1);
    __m256i v256i_32 = _mm256_sub_epi32(v256i_8, v256i_9);
    __m512 v512f_33 = _mm512_xor_ps(v512f_6, v512f_2);
    __m256i v256i_34 = _mm256_or_si256(v256i_32, v256i_8);
    }
    g_v512d[6] = v512d_28;
    g_v512i[9] = v512i_5;
    g_v256f[3] = v256f_7;
    g_v128f[11] = v128f_13;
    g_v128i[9] = v128i_10;
    g_v256h[0] = p2;
    return m16_22;
}

__m512 torture_127(__m256d p0, __m512i p1) {
    __m512i v512i_1 = _mm512_add_epi8(p1, p1);
    __m256i v256i_2 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_3 = _mm256_mullo_epi32(v256i_2, v256i_2);
    __m512 v512f_4 = g_v512f[0];
    __mmask16 m16_5 = _mm512_cmp_ps_mask(v512f_4, v512f_4, _CMP_LT_OQ);
    __m128d v128d_6 = g_v128d[2];
    __m128d v128d_7 = _mm_fmadd_pd(v128d_6, v128d_6, v128d_6);
    __mmask16 m16_8 = _mm512_cmp_epi32_mask(p1, p1, 2);
    __m256d v256d_9 = _mm256_sqrt_pd(p0);
    __mmask32 m32_10 = (__mmask32)(g_m32[7] ^ 56);
    __mmask32 m32_11 = _kxor_mask32(m32_10, m32_10);
    __m128d v128d_12 = _mm_mul_pd(v128d_7, v128d_6);
    __m256 v256f_13 = _mm256_loadu_ps(fbuf);
    __m256 v256f_14 = _mm256_max_ps(v256f_13, v256f_13);
    __m512d v512d_15 = _mm512_loadu_pd(dbuf);
    __m512d v512d_16 = _mm512_unpackhi_pd(v512d_15, v512d_15);
    __m256d v256d_17 = _mm256_mul_pd(p0, v256d_9);
    __m128 v128f_18 = _mm_loadu_ps(fbuf);
    __m128 v128f_19 = _mm_fnmadd_ps(v128f_18, v128f_18, v128f_18);
    __m256 v256f_20 = _mm256_max_ps(v256f_13, v256f_14);
    __m512i v512i_21 = _mm512_permutex2var_epi32(p1, v512i_1, v512i_1);
    __m256i v256i_22 = _mm256_add_epi8(v256i_3, v256i_2);
    __mmask16 m16_23 = _mm512_cmp_epu32_mask(p1, v512i_21, 4);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512i v512i_24 = _mm512_sub_epi8(v512i_21, v512i_21);
    __m256i v256i_25 = _mm256_sub_epi64(v256i_3, v256i_2);
    __m256 v256f_26 = _mm256_div_ps(v256f_14, v256f_13);
    }
    __m256d v256d_27 = _mm512_extractf64x4_pd(v512d_15, 1);
    __m512h v512h_28 = g_v512h[10];
    __m512h v512h_29 = _mm512_add_ph(v512h_28, v512h_28);
    __m256 v256f_30 = _mm256_fnmadd_ps(v256f_14, v256f_14, v256f_14);
    __m128d v128d_31 = _mm_sqrt_pd(v128d_12);
    __m256 v256f_32 = _mm256_add_ps(v256f_30, v256f_14);
    g_v256i[7] = v256i_22;
    g_m16[8] = m16_5;
    g_m32[0] = m32_11;
    return v512f_4;
}

__m512h torture_128(__m512h p0, __m128 p1, __m512h p2, __m512d p3) {
    __m512d v512d_1 = _mm512_sub_pd(p3, p3);
    __m512i v512i_2 = g_v512i[10];
    __m512i v512i_3 = _mm512_sub_epi8(v512i_2, v512i_2);
    __m128i v128i_4 = g_v128i[10];
    __m128i v128i_5 = _mm_and_si128(v128i_4, v128i_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_7 = _mm512_cmp_ps_mask(v512f_6, v512f_6, _CMP_LT_OQ);
    __m256d v256d_8 = _mm256_set1_pd(2.0);
    __m512h v512h_9 = _mm512_mul_ph(p0, p2);
    __m256 v256f_10 = g_v256f[2];
    __m256 v256f_11 = _mm256_shuffle_ps(v256f_10, v256f_10, 0x1B);
    __m512i v512i_12 = _mm512_permutex2var_epi32(v512i_3, v512i_2, v512i_2);
    __m128i v128i_13 = _mm_add_epi32(v128i_4, v128i_5);
    __m512i v512i_14 = _mm512_add_epi64(v512i_12, v512i_2);
    __m256d v256d_15 = _mm256_sqrt_pd(v256d_8);
    __m256 v256f_16 = _mm256_xor_ps(v256f_11, v256f_10);
    __m128d v128d_17 = g_v128d[6];
    __m128d v128d_18 = _mm_sqrt_pd(v128d_17);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m128i v128i_19 = _mm512_cvtepi32_epi8(v512i_14);
    __m512i v512i_20 = _mm512_madd52lo_epu64(v512i_2, v512i_12, v512i_2);
    __m128i v128i_21 = _mm_shuffle_epi8(v128i_5, v128i_13);
    __m256 v256f_22 = _mm256_and_ps(v256f_10, v256f_11);
    __m128 v128f_23 = _mm_unpacklo_ps(p1, p1);
    }
    __m256 v256f_24 = _mm256_max_ps(v256f_16, v256f_10);
    __m512 v512f_25 = _mm512_shuffle_ps(v512f_6, v512f_6, 0x1B);
    __m512 v512f_26 = _mm512_xor_ps(v512f_25, v512f_25);
    __m512i v512i_27 = _mm512_set1_epi32(0x55);
    __m512d v512d_28 = _mm512_fmadd_pd(p3, p3, p3);
    __mmask8 m8_29 = (__mmask8)(g_m8[2] ^ 99);
    __mmask16 m16_30 = _mm512_kunpackb(m8_29, m8_29);
    __mmask64 m64_31 = _mm512_cmp_epi8_mask(v512i_14, v512i_14, 1);
    __m512 v512f_32 = _mm512_roundscale_ps(v512f_25, 0x10);
    __m256i v256i_33 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_34 = _mm256_add_epi32(v256i_33, v256i_33);
    __m128 v128f_35 = _mm_unpacklo_ps(p1, p1);
    g_v512d[11] = v512d_1;
    g_v512i[4] = v512i_14;
    g_v256f[7] = v256f_11;
    g_m64[0] = m64_31;
    return v512h_9;
}

__m512 torture_129(__m128h p0, __m128d p1, __m128i p2, __m256d p3) {
    __m128d v128d_1 = _mm_sub_pd(p1, p1);
    __m128d v128d_2 = _mm_min_pd(p1, p1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_xor_si512(v512i_3, v512i_3);
    __m128d v128d_5 = _mm_unpackhi_pd(v128d_1, v128d_1);
    __m512 v512f_6 = g_v512f[8];
    __m512 v512f_7 = _mm512_getexp_ps(v512f_6);
    __m512h v512h_8 = g_v512h[1];
    __m512h v512h_9 = _mm512_mul_ph(v512h_8, v512h_8);
    __m512 v512f_10 = _mm512_sub_ps(v512f_6, v512f_6);
    __m512i v512i_11 = _mm512_sub_epi64(v512i_4, v512i_4);
    __m512 v512f_12 = _mm512_rcp14_ps(v512f_10);
    __m128 v128f_13 = g_v128f[4];
    __m512 v512f_14 = _mm512_broadcast_f32x4(v128f_13);
    __mmask16 m16_15 = (__mmask16)(g_m16[1] ^ 190);
    __mmask16 m16_16 = _knot_mask16(m16_15);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_15) & 7); i_17++) {
    __m512i v512i_18 = _mm512_conflict_epi32(v512i_11);
    __m256i v256i_19 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_20 = _mm256_add_epi64(v256i_19, v256i_19);
    __m256 v256f_21 = g_v256f[2];
    __m256 v256f_22 = _mm256_mul_ps(v256f_21, v256f_21);
    __m128i v128i_23 = _mm_add_epi16(p2, p2);
    __mmask16 m16_24 = _kand_mask16(m16_16, m16_16);
    }
    __m128i v128i_25 = _mm_add_epi16(p2, p2);
    __m512i v512i_26 = _mm512_rol_epi32(v512i_3, 5);
    __mmask8 m8_27 = (__mmask8)(g_m8[9] ^ 241);
    __mmask8 m8_28 = _kand_mask8(m8_27, m8_27);
    __mmask64 m64_29 = _mm512_cmp_epi8_mask(v512i_4, v512i_26, 1);
    __m512 v512f_30 = _mm512_sqrt_ps(v512f_12);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_15) & 7); i_31++) {
    __m128i v128i_32 = _mm_unpacklo_epi32(v128i_25, v128i_25);
    __m512 v512f_33 = _mm512_sqrt_ps(v512f_12);
    __m512d v512d_34 = _mm512_loadu_pd(dbuf);
    __m512d v512d_35 = _mm512_add_pd(v512d_34, v512d_34);
    __m512 v512f_36 = _mm512_rcp14_ps(v512f_14);
    __m128i v128i_37 = _mm_add_epi32(v128i_25, p2);
    }
    g_v512f[4] = v512f_6;
    g_v128f[8] = v128f_13;
    g_v128d[2] = v128d_2;
    g_v128i[7] = p2;
    g_m8[0] = m8_28;
    return v512f_12;
}

__m256h torture_130(__m256i p0, __m128 p1, __m512h p2, __m128i p3, __m256d p4) {
    __m512d v512d_1 = _mm512_set1_pd(2.0);
    __m128i v128i_2 = _mm_and_si128(p3, p3);
    __m256i v256i_3 = _mm256_and_si256(p0, p0);
    __m512 v512f_4 = g_v512f[9];
    __m512 v512f_5 = _mm512_and_ps(v512f_4, v512f_4);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_add_epi32(v512i_6, v512i_6);
    __m128 v128f_8 = _mm_fmsub_ps(p1, p1, p1);
    __m256h v256h_9 = g_v256h[1];
    __m512 v512f_10 = _mm512_cvtxph_ps(v256h_9);
    __m128 v128f_11 = _mm_sqrt_ps(p1);
    __mmask16 m16_12 = _mm512_testn_epi32_mask(v512i_6, v512i_6);
    __m128 v128f_13 = _mm_mul_ps(v128f_11, v128f_8);
    __m512 v512f_14 = _mm512_div_ps(v512f_10, v512f_4);
    __m128i v128i_15 = _mm_xor_si128(p3, v128i_2);
    __m512i v512i_16 = _mm512_cvtps_epi32(v512f_4);
    __m256 v256f_17 = g_v256f[6];
    __m512d v512d_18 = _mm512_cvtps_pd(v256f_17);
    __m128 v128f_19 = _mm_div_ps(v128f_11, v128f_11);
    __m512h v512h_20 = _mm512_add_ph(p2, p2);
    __m128d v128d_21 = g_v128d[3];
    __m128d v128d_22 = _mm_max_pd(v128d_21, v128d_21);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_12) & 7); i_23++) {
    __m128h v128h_24 = g_v128h[0];
    __m512d v512d_25 = _mm512_cvtph_pd(v128h_24);
    __m128d v128d_26 = _mm_mul_pd(v128d_21, v128d_21);
    __m256 v256f_27 = _mm256_and_ps(v256f_17, v256f_17);
    }
    __m128i v128i_28 = _mm_mullo_epi32(v128i_15, v128i_2);
    __m512d v512d_29 = _mm512_min_pd(v512d_1, v512d_1);
    __m512i v512i_30 = _mm512_dpbusd_epi32(v512i_7, v512i_6, v512i_6);
    __mmask16 m16_31 = _mm512_movepi32_mask(v512i_16);
    __m128d v128d_32 = _mm_sqrt_pd(v128d_21);
    __m256d v256d_33 = _mm256_unpackhi_pd(p4, p4);
    g_v512f[2] = v512f_4;
    g_v256f[7] = v256f_17;
    g_v256d[9] = p4;
    g_v512h[2] = p2;
    return v256h_9;
}

__m256i torture_131(__m512i p0, __m128d p1, __m512 p2, __m512d p3, __m128 p4) {
    __m512i v512i_1 = _mm512_add_epi8(p0, p0);
    __mmask16 m16_2 = (__mmask16)(g_m16[10] ^ 56);
    __m512i v512i_3 = _mm512_mask_blend_epi32(m16_2, p0, p0);
    __m256i v256i_4 = _mm512_cvtepi64_epi32(p0);
    __m512 v512f_5 = _mm512_rcp14_ps(p2);
    __m256 v256f_6 = _mm256_set1_ps(1.0f);
    __m256d v256d_7 = g_v256d[7];
    __m256d v256d_8 = _mm256_min_pd(v256d_7, v256d_7);
    __m128 v128f_9 = _mm_fmsub_ps(p4, p4, p4);
    __mmask16 m16_10 = _mm512_testn_epi32_mask(v512i_3, p0);
    __m512d v512d_11 = _mm512_insertf64x2(p3, p1, 3);
    __m128i v128i_12 = g_v128i[0];
    __m128i v128i_13 = _mm_add_epi8(v128i_12, v128i_12);
    __m128i v128i_14 = _mm_slli_epi32(v128i_12, 3);
    __m128 v128f_15 = _mm_shuffle_ps(v128f_9, v128f_9, 0x1B);
    __m128d v128d_16 = _mm_sub_pd(p1, p1);
    __m128 v128f_17 = _mm_set1_ps(1.0f);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m128i v128i_18 = _mm_abs_epi32(v128i_14);
    __m256i v256i_19 = _mm256_slli_epi32(v256i_4, 3);
    __m512i v512i_20 = _mm512_set1_epi32(0x55);
    }
    __mmask8 m8_21 = (__mmask8)(g_m8[4] ^ 21);
    __mmask16 m16_22 = _mm512_kunpackb(m8_21, m8_21);
    __m128 v128f_23 = _mm_xor_ps(v128f_15, p4);
    __m512d v512d_24 = _mm512_sqrt_pd(v512d_11);
    __m512 v512f_25 = _mm512_sqrt_ps(p2);
    __m512 v512f_26 = _mm512_scalef_ps(p2, v512f_5);
    __m256 v256f_27 = _mm256_fmsub_ps(v256f_6, v256f_6, v256f_6);
    __mmask16 m16_28 = _kshiftri_mask16(m16_10, 1);
    __m512i v512i_29 = _mm512_add_epi8(v512i_3, v512i_1);
    __m512i v512i_30 = _mm512_ternarylogic_epi32(v512i_3, p0, v512i_1, 0x96);
    __m512i v512i_31 = _mm512_popcnt_epi64(v512i_30);
    __m128 v128f_32 = _mm_shuffle_ps(v128f_9, v128f_23, 0x1B);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m512h v512h_33 = g_v512h[3];
    __m512h v512h_34 = _mm512_rcp_ph(v512h_33);
    __m256 v256f_35 = _mm256_max_ps(v256f_27, v256f_6);
    __m128i v128i_36 = _mm_add_epi16(v128i_14, v128i_14);
    }
    g_v512i[2] = p0;
    g_v128f[10] = v128f_17;
    g_v128i[0] = v128i_12;
    return v256i_4;
}

__m128d torture_132(__m512d p0, __m512d p1) {
    __mmask16 m16_1 = (__mmask16)(g_m16[8] ^ 189);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_maskz_mul_ps(m16_1, v512f_2, v512f_2);
    __m128i v128i_4 = g_v128i[5];
    __m128i v128i_5 = _mm_mullo_epi32(v128i_4, v128i_4);
    __m512 v512f_6 = _mm512_roundscale_ps(v512f_2, 0x10);
    __mmask8 m8_7 = (__mmask8)(g_m8[1] ^ 218);
    __m512i v512i_8 = _mm512_broadcastmb_epi64(m8_7);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_add_epi16(v256i_9, v256i_9);
    __mmask16 m16_11 = _knot_mask16(m16_1);
    __m128d v128d_12 = g_v128d[10];
    __m128d v128d_13 = _mm_mul_pd(v128d_12, v128d_12);
    __m128i v128i_14 = _mm_or_si128(v128i_5, v128i_5);
    __m512 v512f_15 = _mm512_xor_ps(v512f_2, v512f_2);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_1) & 7); i_16++) {
    __m512 v512f_17 = _mm512_set1_ps(1.0f);
    __m512d v512d_18 = _mm512_sqrt_pd(p1);
    __mmask16 m16_19 = _kor_mask16(m16_11, m16_11);
    __m128i v128i_20 = _mm512_cvtepi32_epi8(v512i_8);
    __m512i v512i_21 = _mm512_cvtepi8_epi32(v128i_20);
    }
    __m128i v128i_22 = _mm512_cvtepi32_epi8(v512i_8);
    __m256d v256d_23 = g_v256d[4];
    __m256d v256d_24 = _mm256_max_pd(v256d_23, v256d_23);
    __m512 v512f_25 = _mm512_min_ps(v512f_15, v512f_15);
    __m512 v512f_26 = _mm512_fmsub_ps(v512f_3, v512f_15, v512f_3);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_1) & 7); i_27++) {
    __m512i v512i_28 = _mm512_xor_si512(v512i_8, v512i_8);
    __m128 v128f_29 = _mm_loadu_ps(fbuf);
    __m128 v128f_30 = _mm_min_ps(v128f_29, v128f_29);
    __m128 v128f_31 = _mm_add_ps(v128f_30, v128f_29);
    __m256i v256i_32 = _mm256_add_epi32(v256i_9, v256i_10);
    }
    g_v512f[4] = v512f_26;
    g_v512i[9] = v512i_8;
    g_v256i[7] = v256i_9;
    return v128d_13;
}

__m512i torture_133(__m128i p0, __m512h p1, __m256i p2, __m512 p3) {
    __m512 v512f_1 = _mm512_max_ps(p3, p3);
    __m256d v256d_2 = g_v256d[9];
    __m256d v256d_3 = _mm256_mul_pd(v256d_2, v256d_2);
    __m512i v512i_4 = g_v512i[6];
    __m512i v512i_5 = _mm512_and_si512(v512i_4, v512i_4);
    __m128d v128d_6 = g_v128d[5];
    __m128d v128d_7 = _mm_unpackhi_pd(v128d_6, v128d_6);
    __mmask32 m32_8 = _mm512_cmp_ph_mask(p1, p1, 1);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m512d v512d_10 = _mm512_add_pd(v512d_9, v512d_9);
    __m512 v512f_11 = _mm512_and_ps(p3, p3);
    __m256 v256f_12 = _mm256_set1_ps(1.0f);
    __m128d v128d_13 = _mm_unpackhi_pd(v128d_6, v128d_6);
    __m128i v128i_14 = _mm_slli_epi32(p0, 3);
    __m128 v128f_15 = g_v128f[5];
    __m128 v128f_16 = _mm_add_ps(v128f_15, v128f_15);
    __m512i v512i_17 = _mm512_shuffle_epi8(v512i_4, v512i_4);
    __m512i v512i_18 = _mm512_set1_epi32(0x55);
    if (((unsigned long long)m32_8) & 1ULL) {
    __m512i v512i_19 = _mm512_cvtpd_epi64(v512d_9);
    __m128 v128f_20 = _mm_mul_ps(v128f_16, v128f_16);
    __mmask16 m16_21 = (__mmask16)(g_m16[5] ^ 11);
    __m512i v512i_22 = _mm512_broadcastmw_epi32(m16_21);
    __m512i v512i_23 = _mm512_add_epi16(v512i_18, v512i_18);
    __m128d v128d_24 = _mm_set1_pd(2.0);
    }
    __mmask16 m16_25 = _mm512_testn_epi32_mask(v512i_5, v512i_5);
    __m512i v512i_26 = _mm512_sub_epi32(v512i_4, v512i_4);
    __m128i v128i_27 = _mm_unpacklo_epi32(v128i_14, p0);
    __m512i v512i_28 = _mm512_shuffle_i32x4(v512i_26, v512i_4, 0x1B);
    __m128 v128f_29 = _mm_unpacklo_ps(v128f_16, v128f_15);
    __m512 v512f_30 = _mm512_getexp_ps(p3);
    __m128d v128d_31 = _mm_add_pd(v128d_7, v128d_13);
    __m512h v512h_32 = _mm512_fmul_pch(p1, p1);
    __m512 v512f_33 = _mm512_div_ps(p3, v512f_11);
    __mmask32 m32_34 = _mm512_cmp_ph_mask(p1, v512h_32, 1);
    __m512i v512i_35 = _mm512_permutexvar_epi32(v512i_5, v512i_17);
    if (((unsigned long long)m16_25) & 1ULL) {
    __m128i v128i_36 = _mm_slli_epi32(v128i_14, 3);
    unsigned u32_37 = _cvtmask16_u32(m16_25);
    __m256 v256f_38 = _mm256_mul_ps(v256f_12, v256f_12);
    __m256 v256f_39 = _mm256_div_ps(v256f_12, v256f_12);
    __mmask16 m16_40 = _mm512_cmp_ps_mask(v512f_1, v512f_1, _CMP_LT_OQ);
    __m256i v256i_41 = _mm256_sub_epi64(p2, p2);
    }
    g_v512d[0] = v512d_9;
    g_v256f[3] = v256f_12;
    g_v256d[8] = v256d_2;
    g_v128i[7] = p0;
    g_m16[11] = m16_25;
    g_m32[0] = m32_8;
    return v512i_17;
}

__mmask32 torture_134(__m256d p0, __m256h p1, __m256i p2, __m512i p3) {
    __mmask64 m64_1 = (__mmask64)(g_m64[5] ^ 113);
    __mmask64 m64_2 = _kor_mask64(m64_1, m64_1);
    __m512 v512f_3 = g_v512f[0];
    __m512 v512f_4 = _mm512_add_ps(v512f_3, v512f_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_add_epi8(v128i_5, v128i_5);
    __m256i v256i_7 = _mm256_sub_epi8(p2, p2);
    __m128i v128i_8 = _mm_shuffle_epi8(v128i_6, v128i_5);
    __m256 v256f_9 = g_v256f[2];
    __m256 v256f_10 = _mm256_min_ps(v256f_9, v256f_9);
    __mmask16 m16_11 = (__mmask16)(g_m16[1] ^ 101);
    __mmask16 m16_12 = _kor_mask16(m16_11, m16_11);
    __m256i v256i_13 = _mm256_sub_epi8(v256i_7, p2);
    __m128d v128d_14 = g_v128d[4];
    __m128d v128d_15 = _mm_sub_pd(v128d_14, v128d_14);
    __m128d v128d_16 = _mm_min_pd(v128d_14, v128d_14);
    __m128d v128d_17 = _mm_add_pd(v128d_16, v128d_14);
    __m512i v512i_18 = _mm512_or_si512(p3, p3);
    __m512d v512d_19 = g_v512d[7];
    __m512i v512i_20 = _mm512_cvtpd_epi64(v512d_19);
    __m256 v256f_21 = _mm256_max_ps(v256f_10, v256f_9);
    __m512 v512f_22 = _mm512_shuffle_f32x4(v512f_4, v512f_3, 0x4E);
    if (((unsigned long long)m16_11) & 1ULL) {
    __m512d v512d_23 = _mm512_insertf64x2(v512d_19, v128d_17, 3);
    __m512d v512d_24 = _mm512_cvtps_pd(v256f_9);
    __m128i v128i_25 = _mm_sub_epi8(v128i_5, v128i_5);
    unsigned u32_26 = 72;
    __mmask16 m16_27 = _cvtu32_mask16(u32_26);
    __m256i v256i_28 = _mm256_unpacklo_epi32(v256i_13, v256i_13);
    __m128h v128h_29 = _mm512_cvtpd_ph(v512d_24);
    }
    __m512i v512i_30 = _mm512_sub_epi16(v512i_20, v512i_18);
    __m128d v128d_31 = _mm_set1_pd(2.0);
    unsigned u32_32 = 86;
    __mmask16 m16_33 = _cvtu32_mask16(u32_32);
    __m256d v256d_34 = _mm256_div_pd(p0, p0);
    __m256i v256i_35 = _mm256_shuffle_epi8(v256i_13, v256i_7);
    __m256 v256f_36 = _mm256_max_ps(v256f_9, v256f_10);
    __m128 v128f_37 = g_v128f[7];
    __m128 v128f_38 = _mm_mul_ps(v128f_37, v128f_37);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_12) & 7); i_39++) {
    __m128i v128i_40 = _mm_add_epi32(v128i_5, v128i_8);
    __m256 v256f_41 = _mm512_cvtpd_ps(v512d_19);
    }
    g_v512d[5] = v512d_19;
    g_v256i[9] = v256i_7;
    g_v128f[4] = v128f_38;
    g_v128d[2] = v128d_15;
    __mmask32 m32_42 = (__mmask32)(g_m32[2] ^ 112);
    return m32_42;
}

__mmask8 torture_135(__m512h p0, __m128i p1) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_sub_ps(v256f_1, v256f_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_unpacklo_ps(v512f_3, v512f_3);
    __m512i v512i_5 = g_v512i[10];
    __m512i v512i_6 = _mm512_sub_epi16(v512i_5, v512i_5);
    __mmask16 m16_7 = _mm512_test_epi32_mask(v512i_5, v512i_6);
    __m128 v128f_8 = g_v128f[6];
    __m128 v128f_9 = _mm_sqrt_ps(v128f_8);
    __m256i v256i_10 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_11 = _mm256_abs_epi32(v256i_10);
    __m512h v512h_12 = _mm512_cvtepi16_ph(v512i_6);
    __m128 v128f_13 = _mm_mul_ps(v128f_8, v128f_8);
    __m256 v256f_14 = _mm256_shuffle_ps(v256f_2, v256f_1, 0x1B);
    __m512 v512f_15 = _mm512_rsqrt14_ps(v512f_3);
    __m256 v256f_16 = _mm256_min_ps(v256f_1, v256f_2);
    __m512i v512i_17 = _mm512_add_epi16(v512i_6, v512i_5);
    __m256 v256f_18 = _mm256_sqrt_ps(v256f_16);
    __m512i v512i_19 = _mm512_rolv_epi32(v512i_6, v512i_17);
    __m256d v256d_20 = _mm256_set1_pd(2.0);
    __m512 v512f_21 = _mm512_and_ps(v512f_15, v512f_4);
    __m512 v512f_22 = _mm512_scalef_ps(v512f_3, v512f_15);
    __m512 v512f_23 = _mm512_mask_blend_ps(m16_7, v512f_22, v512f_4);
    __mmask8 m8_24 = (__mmask8)(g_m8[5] ^ 226);
    __m512i v512i_25 = _mm512_broadcastmb_epi64(m8_24);
    __mmask32 m32_26 = (__mmask32)(g_m32[11] ^ 144);
    __mmask32 m32_27 = _kxor_mask32(m32_26, m32_26);
    __m256i v256i_28 = _mm256_sllv_epi32(v256i_10, v256i_11);
    __m128 v128f_29 = _mm_min_ps(v128f_13, v128f_13);
    __m256 v256f_30 = _mm256_set1_ps(1.0f);
    __m128d v128d_31 = _mm_set1_pd(2.0);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512 v512f_32 = _mm512_insertf32x8(v512f_21, v256f_16, 1);
    __m512d v512d_33 = _mm512_set1_pd(2.0);
    __m256d v256d_34 = _mm256_div_pd(v256d_20, v256d_20);
    __m512 v512f_35 = _mm512_roundscale_ps(v512f_15, 0x10);
    }
    g_v512i[7] = v512i_17;
    g_v256i[11] = v256i_10;
    g_v128d[6] = v128d_31;
    g_v512h[11] = p0;
    return m8_24;
}

__m512 torture_136(__m128d p0, __m512i p1) {
    __mmask8 m8_1 = (__mmask8)(g_m8[6] ^ 63);
    __mmask8 m8_2 = _kand_mask8(m8_1, m8_1);
    __m512h v512h_3 = g_v512h[2];
    __m512h v512h_4 = _mm512_sqrt_ph(v512h_3);
    __m512d v512d_5 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_6 = _mm512_cmp_pd_mask(v512d_5, v512d_5, _CMP_GE_OQ);
    __m512i v512i_7 = _mm512_or_si512(p1, p1);
    __m128i v128i_8 = g_v128i[4];
    __m128i v128i_9 = _mm_abs_epi32(v128i_8);
    __mmask64 m64_10 = _mm512_cmp_epi8_mask(v512i_7, v512i_7, 1);
    __m512i v512i_11 = _mm512_shuffle_epi8(v512i_7, p1);
    __m512i v512i_12 = _mm512_slli_epi32(v512i_11, 3);
    __m256i v256i_13 = _mm256_set1_epi32(0x55);
    __m512 v512f_14 = _mm512_loadu_ps(fbuf);
    __m128 v128f_15 = _mm512_extractf32x4_ps(v512f_14, 2);
    __m512 v512f_16 = _mm512_cvtepi32_ps(v512i_12);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m8_1) & 7); i_17++) {
    __m512i v512i_18 = _mm512_cvtps_epi32(v512f_16);
    __m512h v512h_19 = _mm512_mul_ph(v512h_3, v512h_3);
    __m256d v256d_20 = g_v256d[2];
    __m256d v256d_21 = _mm256_add_pd(v256d_20, v256d_20);
    __m512 v512f_22 = _mm512_div_ps(v512f_14, v512f_16);
    }
    __m512d v512d_23 = _mm512_min_pd(v512d_5, v512d_5);
    __m512 v512f_24 = _mm512_mul_ps(v512f_14, v512f_16);
    __m512i v512i_25 = _mm512_mullo_epi32(v512i_12, v512i_12);
    __m256i v256i_26 = _mm256_slli_epi32(v256i_13, 3);
    __m128i v128i_27 = _mm_or_si128(v128i_8, v128i_9);
    __m256 v256f_28 = g_v256f[0];
    __m256 v256f_29 = _mm256_and_ps(v256f_28, v256f_28);
    __m128d v128d_30 = _mm_sub_pd(p0, p0);
    __m512i v512i_31 = _mm512_cvtepi8_epi32(v128i_8);
    __m128 v128f_32 = _mm_max_ps(v128f_15, v128f_15);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m8_2) & 7); i_33++) {
    __m256d v256d_34 = g_v256d[0];
    __m256d v256d_35 = _mm256_sqrt_pd(v256d_34);
    __m128 v128f_36 = _mm_xor_ps(v128f_32, v128f_32);
    __m512 v512f_37 = _mm512_shuffle_ps(v512f_16, v512f_16, 0x1B);
    __m256i v256i_38 = _mm256_add_epi8(v256i_13, v256i_26);
    __m512d v512d_39 = _mm512_shuffle_f64x2(v512d_5, v512d_5, 0xEE);
    }
    g_v512d[4] = v512d_5;
    g_v512i[11] = v512i_7;
    g_v256f[5] = v256f_29;
    g_v128d[4] = v128d_30;
    g_v128i[4] = v128i_27;
    g_m8[0] = m8_1;
    return v512f_24;
}

__mmask16 torture_137(__m128 p0, __m256d p1, __m256h p2) {
    __m128 v128f_1 = _mm_fnmadd_ps(p0, p0, p0);
    __m512d v512d_2 = _mm512_loadu_pd(dbuf);
    __m512d v512d_3 = _mm512_div_pd(v512d_2, v512d_2);
    __m256 v256f_4 = g_v256f[0];
    __m256 v256f_5 = _mm256_min_ps(v256f_4, v256f_4);
    __m512i v512i_6 = g_v512i[1];
    __m512i v512i_7 = _mm512_rol_epi32(v512i_6, 5);
    __m128 v128f_8 = _mm_sqrt_ps(v128f_1);
    __m128i v128i_9 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_10 = _mm_add_epi64(v128i_9, v128i_9);
    __m256d v256d_11 = _mm256_set1_pd(2.0);
    __m256 v256f_12 = _mm256_sub_ps(v256f_4, v256f_4);
    __m128 v128f_13 = _mm_mul_ps(v128f_1, p0);
    int s32_14 = 4;
    for (int i_15 = 0; i_15 < (int)((s32_14) & 7); i_15++) {
    __m512i v512i_16 = _mm512_cvtpd_epi64(v512d_3);
    __mmask8 m8_17 = _mm512_cmp_pd_mask(v512d_2, v512d_3, _CMP_GE_OQ);
    __m256d v256d_18 = _mm256_div_pd(p1, p1);
    }
    __m256i v256i_19 = g_v256i[6];
    __m256i v256i_20 = _mm256_slli_epi32(v256i_19, 3);
    __m256i v256i_21 = _mm256_slli_epi32(v256i_19, 3);
    __m512 v512f_22 = _mm512_loadu_ps(fbuf);
    __m512 v512f_23 = _mm512_xor_ps(v512f_22, v512f_22);
    __m256d v256d_24 = _mm512_extractf64x4_pd(v512d_3, 1);
    __m512h v512h_25 = g_v512h[9];
    __m512h v512h_26 = _mm512_fmadd_ph(v512h_25, v512h_25, v512h_25);
    __m512h v512h_27 = _mm512_add_ph(v512h_26, v512h_26);
    __m512 v512f_28 = _mm512_cvtepi32_ps(v512i_7);
    if ((s32_14) & 1ULL) {
    __m128 v128f_29 = _mm_add_ps(v128f_13, v128f_8);
    __m128d v128d_30 = g_v128d[8];
    __m128d v128d_31 = _mm_sqrt_pd(v128d_30);
    }
    g_v256d[9] = v256d_11;
    g_v256i[11] = v256i_19;
    g_v128f[8] = v128f_13;
    g_v256h[8] = p2;
    __mmask16 m16_32 = (__mmask16)(g_m16[7] ^ 48);
    return m16_32;
}

__mmask8 torture_138(__m128 p0, __m512 p1, __m128d p2) {
    __mmask16 m16_1 = (__mmask16)(g_m16[3] ^ 33);
    __m512 v512f_2 = _mm512_maskz_mul_ps(m16_1, p1, p1);
    __m512h v512h_3 = g_v512h[3];
    __m512h v512h_4 = _mm512_fmadd_ph(v512h_3, v512h_3, v512h_3);
    __m512i v512i_5 = g_v512i[8];
    __m512 v512f_6 = _mm512_cvtepi32_ps(v512i_5);
    __m512d v512d_7 = _mm512_loadu_pd(dbuf);
    __m512d v512d_8 = _mm512_shuffle_f64x2(v512d_7, v512d_7, 0xEE);
    __m256d v256d_9 = g_v256d[3];
    __m256d v256d_10 = _mm256_max_pd(v256d_9, v256d_9);
    __m512i v512i_11 = _mm512_add_epi32(v512i_5, v512i_5);
    __m512 v512f_12 = _mm512_shuffle_ps(p1, p1, 0x1B);
    __m128d v128d_13 = _mm_div_pd(p2, p2);
    __mmask8 m8_14 = (__mmask8)(g_m8[8] ^ 75);
    __mmask8 m8_15 = _kand_mask8(m8_14, m8_14);
    if (((unsigned long long)m16_1) & 1ULL) {
    __mmask16 m16_16 = _kxor_mask16(m16_1, m16_1);
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_8, v512d_8, 0xEE);
    __m512h v512h_18 = _mm512_cvtepi16_ph(v512i_11);
    }
    __m512 v512f_19 = _mm512_broadcast_f32x4(p0);
    __m512h v512h_20 = _mm512_getexp_ph(v512h_4);
    __m256h v256h_21 = _mm512_cvtxps_ph(v512f_12);
    __m256i v256i_22 = g_v256i[4];
    __m256i v256i_23 = _mm256_xor_si256(v256i_22, v256i_22);
    __m512i v512i_24 = _mm512_mullo_epi32(v512i_11, v512i_5);
    __mmask16 m16_25 = _kand_mask16(m16_1, m16_1);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_25) & 7); i_26++) {
    __mmask32 m32_27 = _mm512_cmp_ph_mask(v512h_4, v512h_4, 1);
    __m128i v128i_28 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_29 = _mm_add_epi32(v128i_28, v128i_28);
    __m128i v128i_30 = _mm_sub_epi32(v128i_29, v128i_29);
    __m512 v512f_31 = _mm512_set1_ps(1.0f);
    __m128 v128f_32 = _mm_add_ps(p0, p0);
    }
    g_v512f[7] = v512f_12;
    g_v512i[8] = v512i_5;
    g_v256i[8] = v256i_23;
    g_v128f[6] = p0;
    g_m8[0] = m8_15;
    return m8_15;
}

__m128 torture_139(__m512 p0, __m128 p1, __m256i p2, __m512h p3) {
    __m128 v128f_1 = _mm_max_ps(p1, p1);
    __m128 v128f_2 = _mm_xor_ps(v128f_1, p1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_fmadd_pd(v512d_3, v512d_3, v512d_3);
    __m256d v256d_5 = g_v256d[2];
    __m256d v256d_6 = _mm256_max_pd(v256d_5, v256d_5);
    __m128 v128f_7 = _mm_mul_ps(p1, v128f_2);
    __m512h v512h_8 = _mm512_add_ph(p3, p3);
    __mmask16 m16_9 = _mm512_cmp_ps_mask(p0, p0, _CMP_LT_OQ);
    __m512i v512i_10 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_11 = _mm512_sllv_epi32(v512i_10, v512i_10);
    __m256 v256f_12 = _mm256_loadu_ps(fbuf);
    __m256 v256f_13 = _mm256_sqrt_ps(v256f_12);
    __m256i v256i_14 = _mm256_xor_si256(p2, p2);
    __m512 v512f_15 = _mm512_rsqrt14_ps(p0);
    __m512i v512i_16 = _mm512_sub_epi64(v512i_10, v512i_10);
    __m256 v256f_17 = _mm256_mul_ps(v256f_12, v256f_13);
    __m512i v512i_18 = _mm512_add_epi8(v512i_11, v512i_11);
    __m256h v256h_19 = g_v256h[4];
    __m512 v512f_20 = _mm512_cvtxph_ps(v256h_19);
    __m512d v512d_21 = _mm512_unpackhi_pd(v512d_4, v512d_3);
    __m128 v128f_22 = _mm_max_ps(v128f_1, v128f_2);
    __mmask16 m16_23 = _mm512_fpclass_ps_mask(p0, 0x3);
    __m512i v512i_24 = _mm512_permutex2var_epi32(v512i_18, v512i_18, v512i_18);
    __m128i v128i_25 = g_v128i[2];
    __m128i v128i_26 = _mm_sub_epi64(v128i_25, v128i_25);
    __m128i v128i_27 = _mm_slli_epi32(v128i_25, 3);
    __m256 v256f_28 = _mm256_add_ps(v256f_13, v256f_12);
    __m128 v128f_29 = _mm_shuffle_ps(v128f_22, v128f_1, 0x1B);
    __m512i v512i_30 = _mm512_rolv_epi32(v512i_16, v512i_11);
    __mmask16 m16_31 = _kand_mask16(m16_9, m16_9);
    __m256d v256d_32 = _mm256_max_pd(v256d_5, v256d_5);
    __m512i v512i_33 = _mm512_broadcast_i32x4(v128i_27);
    __m256d v256d_34 = _mm256_max_pd(v256d_32, v256d_6);
    g_v512d[9] = v512d_4;
    g_v256f[9] = v256f_12;
    g_v256d[9] = v256d_6;
    g_m16[4] = m16_31;
    return v128f_22;
}

__m128d torture_140(__m128h p0, __m128h p1, __m512h p2, __m512h p3, __m128d p4) {
    __m512 v512f_1 = g_v512f[11];
    __m512 v512f_2 = _mm512_shuffle_f32x4(v512f_1, v512f_1, 0x4E);
    __m256d v256d_3 = g_v256d[3];
    __m256d v256d_4 = _mm256_min_pd(v256d_3, v256d_3);
    __m512d v512d_5 = _mm512_set1_pd(2.0);
    __m128 v128f_6 = _mm_loadu_ps(fbuf);
    __m128 v128f_7 = _mm_xor_ps(v128f_6, v128f_6);
    __m512i v512i_8 = g_v512i[0];
    __m512i v512i_9 = _mm512_madd52lo_epu64(v512i_8, v512i_8, v512i_8);
    __m256d v256d_10 = _mm256_unpackhi_pd(v256d_4, v256d_4);
    __m128i v128i_11 = _mm_set1_epi32(0x55);
    __m256 v256f_12 = g_v256f[0];
    __m256 v256f_13 = _mm256_unpacklo_ps(v256f_12, v256f_12);
    __m512i v512i_14 = _mm512_add_epi64(v512i_9, v512i_9);
    __m512i v512i_15 = _mm512_add_epi16(v512i_9, v512i_9);
    int s32_16 = 51;
    for (int i_17 = 0; i_17 < (int)((s32_16) & 7); i_17++) {
    __m256i v256i_18 = g_v256i[7];
    __m256i v256i_19 = _mm256_sub_epi16(v256i_18, v256i_18);
    __m512i v512i_20 = _mm512_shuffle_epi8(v512i_9, v512i_15);
    __m512i v512i_21 = _mm512_sllv_epi32(v512i_20, v512i_20);
    __m128i v128i_22 = _mm_add_epi64(v128i_11, v128i_11);
    }
    __m128i v128i_23 = _mm_add_epi64(v128i_11, v128i_11);
    __m512i v512i_24 = _mm512_xor_si512(v512i_9, v512i_15);
    __m512d v512d_25 = _mm512_shuffle_f64x2(v512d_5, v512d_5, 0xEE);
    __m512 v512f_26 = _mm512_roundscale_ps(v512f_2, 0x10);
    __m128 v128f_27 = _mm512_extractf32x4_ps(v512f_2, 2);
    __mmask32 m32_28 = (__mmask32)(g_m32[9] ^ 237);
    __mmask32 m32_29 = _kxor_mask32(m32_28, m32_28);
    g_v128i[9] = v128i_11;
    g_v512h[0] = p2;
    return p4;
}

__m256h torture_141(__m256h p0, __m128i p1, __m256i p2, __m256d p3) {
    __m128i v128i_1 = _mm_unpacklo_epi32(p1, p1);
    __m512i v512i_2 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_3 = _mm512_abs_epi32(v512i_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[5] ^ 88);
    __mmask32 m32_5 = _mm512_kunpackw(m16_4, m16_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m256 v256f_7 = g_v256f[6];
    __m512 v512f_8 = _mm512_insertf32x8(v512f_6, v256f_7, 1);
    __m512i v512i_9 = _mm512_and_si512(v512i_3, v512i_2);
    __m128i v128i_10 = _mm_or_si128(p1, p1);
    __m256 v256f_11 = _mm256_mul_ps(v256f_7, v256f_7);
    __m256 v256f_12 = _mm256_div_ps(v256f_11, v256f_7);
    __mmask16 m16_13 = _mm512_cmp_epu32_mask(v512i_9, v512i_9, 4);
    __m512d v512d_14 = g_v512d[7];
    __m128h v128h_15 = _mm512_cvtpd_ph(v512d_14);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_min_ps(v128f_16, v128f_16);
    __m256i v256i_18 = _mm256_or_si256(p2, p2);
    __m512i v512i_19 = _mm512_permutexvar_epi32(v512i_3, v512i_2);
    __m512h v512h_20 = g_v512h[1];
    __mmask32 m32_21 = _mm512_cmp_ph_mask(v512h_20, v512h_20, 1);
    __m128d v128d_22 = _mm_set1_pd(2.0);
    __m256i v256i_23 = _mm256_add_epi8(v256i_18, p2);
    __m512 v512f_24 = _mm512_add_ps(v512f_8, v512f_8);
    __m128d v128d_25 = _mm_div_pd(v128d_22, v128d_22);
    __m256 v256f_26 = _mm256_shuffle_ps(v256f_11, v256f_7, 0x1B);
    __mmask16 m16_27 = _knot_mask16(m16_13);
    __m512i v512i_28 = _mm512_cvtpd_epi64(v512d_14);
    __m256i v256i_29 = _mm256_sub_epi64(p2, v256i_23);
    __m512 v512f_30 = _mm512_scalef_ps(v512f_6, v512f_24);
    __m128i v128i_31 = _mm_or_si128(p1, v128i_10);
    __m128i v128i_32 = _mm_abs_epi32(p1);
    __m128i v128i_33 = _mm_sub_epi32(v128i_32, v128i_31);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_27) & 7); i_34++) {
    __m128i v128i_35 = _mm_slli_epi32(v128i_31, 3);
    __m512 v512f_36 = _mm512_mask_blend_ps(m16_4, v512f_6, v512f_30);
    __m512d v512d_37 = _mm512_max_pd(v512d_14, v512d_14);
    __m256 v256f_38 = _mm256_add_ps(v256f_7, v256f_7);
    __mmask16 m16_39 = _mm512_cmp_epu32_mask(v512i_28, v512i_19, 4);
    }
    g_v512f[10] = v512f_8;
    g_v512i[5] = v512i_19;
    g_v128d[4] = v128d_22;
    g_v256h[6] = p0;
    g_v128h[7] = v128h_15;
    g_m16[5] = m16_4;
    return p0;
}

__m512h torture_142(__m256d p0, __m512d p1, __m128d p2, __m128i p3, __m256h p4) {
    __m128h v128h_1 = g_v128h[8];
    __m512d v512d_2 = _mm512_cvtph_pd(v128h_1);
    __m128d v128d_3 = _mm_fmadd_pd(p2, p2, p2);
    __m256 v256f_4 = g_v256f[5];
    __m256 v256f_5 = _mm256_fnmadd_ps(v256f_4, v256f_4, v256f_4);
    __m256i v256i_6 = g_v256i[5];
    __m256i v256i_7 = _mm256_or_si256(v256i_6, v256i_6);
    __mmask32 m32_8 = (__mmask32)(g_m32[4] ^ 145);
    __mmask32 m32_9 = _kxor_mask32(m32_8, m32_8);
    __m256 v256f_10 = _mm256_add_ps(v256f_4, v256f_5);
    __m512i v512i_11 = g_v512i[11];
    __m512i v512i_12 = _mm512_madd52lo_epu64(v512i_11, v512i_11, v512i_11);
    __m512i v512i_13 = _mm512_add_epi8(v512i_12, v512i_12);
    if (((unsigned long long)m32_9) & 1ULL) {
    __m512h v512h_14 = g_v512h[4];
    __m512i v512i_15 = _mm512_cvtph_epi16(v512h_14);
    __mmask8 m8_16 = (__mmask8)(g_m8[8] ^ 118);
    __mmask16 m16_17 = _mm512_kunpackb(m8_16, m8_16);
    }
    __m512 v512f_18 = _mm512_loadu_ps(fbuf);
    __m512 v512f_19 = _mm512_div_ps(v512f_18, v512f_18);
    __m512i v512i_20 = _mm512_mullo_epi32(v512i_12, v512i_12);
    __m512h v512h_21 = g_v512h[5];
    __m512h v512h_22 = _mm512_add_ph(v512h_21, v512h_21);
    __m256i v256i_23 = _mm256_add_epi32(v256i_6, v256i_7);
    __m128i v128i_24 = _mm_xor_si128(p3, p3);
    __mmask8 m8_25 = (__mmask8)(g_m8[8] ^ 45);
    __m512i v512i_26 = _mm512_broadcastmb_epi64(m8_25);
    __m512 v512f_27 = _mm512_roundscale_ps(v512f_19, 0x10);
    __m256i v256i_28 = _mm256_shuffle_epi8(v256i_6, v256i_6);
    __m128i v128i_29 = _mm_set1_epi32(0x55);
    __m256d v256d_30 = _mm256_sub_pd(p0, p0);
    __m512 v512f_31 = _mm512_fmadd_ps(v512f_27, v512f_18, v512f_27);
    for (int i_32 = 0; i_32 < (int)(((unsigned long long)m8_25) & 7); i_32++) {
    __m256i v256i_33 = _mm256_add_epi64(v256i_28, v256i_6);
    __m512h v512h_34 = _mm512_cvtepi16_ph(v512i_11);
    __m256d v256d_35 = _mm256_unpackhi_pd(p0, p0);
    __m128i v128i_36 = _mm_and_si128(v128i_29, p3);
    __mmask64 m64_37 = _mm512_cmp_epi8_mask(v512i_11, v512i_13, 1);
    }
    g_v512d[2] = v512d_2;
    g_v256f[7] = v256f_5;
    g_v128d[11] = v128d_3;
    g_v512h[5] = v512h_21;
    g_m32[8] = m32_9;
    return v512h_22;
}

__mmask16 torture_143(__m128h p0, __m128 p1, __m256i p2, __m512d p3, __m512h p4) {
    __m128 v128f_1 = _mm_mul_ps(p1, p1);
    __m256 v256f_2 = _mm256_loadu_ps(fbuf);
    __m256 v256f_3 = _mm256_sqrt_ps(v256f_2);
    unsigned u32_4 = 94;
    __mmask16 m16_5 = _cvtu32_mask16(u32_4);
    __m512i v512i_6 = g_v512i[4];
    __mmask16 m16_7 = _mm512_movepi32_mask(v512i_6);
    __m256 v256f_8 = _mm256_sub_ps(v256f_3, v256f_2);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m256h v256h_10 = _mm512_cvtxps_ph(v512f_9);
    __m128d v128d_11 = g_v128d[5];
    __m512d v512d_12 = _mm512_insertf64x2(p3, v128d_11, 3);
    __m512d v512d_13 = _mm512_cvtph_pd(p0);
    __m512d v512d_14 = _mm512_max_pd(p3, p3);
    __m128d v128d_15 = _mm_sub_pd(v128d_11, v128d_11);
    __m256 v256f_16 = _mm256_max_ps(v256f_2, v256f_8);
    __m512d v512d_17 = _mm512_mul_pd(p3, p3);
    __m256i v256i_18 = _mm256_add_epi16(p2, p2);
    __m512 v512f_19 = _mm512_maskz_mul_ps(m16_5, v512f_9, v512f_9);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_7) & 7); i_20++) {
    unsigned u32_21 = _cvtmask16_u32(m16_7);
    __m256 v256f_22 = _mm256_fnmadd_ps(v256f_8, v256f_3, v256f_3);
    }
    __m256 v256f_23 = _mm256_set1_ps(1.0f);
    __m256i v256i_24 = _mm256_shuffle_epi8(p2, p2);
    __m256i v256i_25 = _mm256_or_si256(v256i_24, v256i_24);
    __m256i v256i_26 = _mm256_add_epi32(p2, v256i_18);
    __m512d v512d_27 = _mm512_fmadd_pd(v512d_17, v512d_14, v512d_13);
    __m128i v128i_28 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_29 = _mm_and_si128(v128i_28, v128i_28);
    __m512i v512i_30 = _mm512_add_epi8(v512i_6, v512i_6);
    __m512i v512i_31 = _mm512_xor_si512(v512i_6, v512i_30);
    __m512 v512f_32 = _mm512_and_ps(v512f_19, v512f_19);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512 v512f_33 = _mm512_sub_ps(v512f_32, v512f_32);
    __m512 v512f_34 = _mm512_div_ps(v512f_9, v512f_33);
    __m256i v256i_35 = _mm256_slli_epi32(v256i_24, 3);
    __m256d v256d_36 = _mm256_set1_pd(2.0);
    __m256i v256i_37 = _mm256_slli_epi32(v256i_18, 3);
    __m512i v512i_38 = _mm512_lzcnt_epi32(v512i_30);
    }
    g_v512f[8] = v512f_32;
    g_v512i[1] = v512i_6;
    g_v256h[6] = v256h_10;
    g_v128h[3] = p0;
    g_m16[5] = m16_7;
    return m16_5;
}

__m128 torture_144(__m512h p0, __m512h p1, __m512 p2) {
    __m128i v128i_1 = g_v128i[4];
    __m128i v128i_2 = _mm_sub_epi8(v128i_1, v128i_1);
    __m256d v256d_3 = g_v256d[0];
    __m256d v256d_4 = _mm256_unpackhi_pd(v256d_3, v256d_3);
    __m128d v128d_5 = g_v128d[10];
    __m128d v128d_6 = _mm_fmadd_pd(v128d_5, v128d_5, v128d_5);
    __m512i v512i_7 = g_v512i[3];
    __m512i v512i_8 = _mm512_sub_epi64(v512i_7, v512i_7);
    __m256i v256i_9 = g_v256i[2];
    __m256i v256i_10 = _mm256_and_si256(v256i_9, v256i_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512i v512i_12 = _mm512_cvtpd_epi64(v512d_11);
    __m256i v256i_13 = _mm256_xor_si256(v256i_10, v256i_9);
    __m256 v256f_14 = _mm256_loadu_ps(fbuf);
    __m256 v256f_15 = _mm256_sqrt_ps(v256f_14);
    __m512 v512f_16 = _mm512_getexp_ps(p2);
    __m128d v128d_17 = _mm_fmadd_pd(v128d_5, v128d_6, v128d_5);
    __m256i v256i_18 = _mm256_or_si256(v256i_10, v256i_10);
    __m512i v512i_19 = _mm512_add_epi32(v512i_8, v512i_12);
    int s32_20 = 95;
    for (int i_21 = 0; i_21 < (int)((s32_20) & 7); i_21++) {
    __m256i v256i_22 = _mm256_and_si256(v256i_9, v256i_10);
    __m128d v128d_23 = _mm_sub_pd(v128d_6, v128d_5);
    __m512 v512f_24 = _mm512_set1_ps(1.0f);
    __m512i v512i_25 = _mm512_mullo_epi32(v512i_7, v512i_7);
    }
    __m512 v512f_26 = _mm512_set1_ps(1.0f);
    __m256i v256i_27 = _mm256_sub_epi64(v256i_13, v256i_13);
    __m512h v512h_28 = _mm512_getexp_ph(p1);
    __m256 v256f_29 = _mm256_mul_ps(v256f_15, v256f_14);
    __m128d v128d_30 = _mm_div_pd(v128d_5, v128d_17);
    __mmask8 m8_31 = (__mmask8)(g_m8[10] ^ 120);
    __mmask8 m8_32 = _kand_mask8(m8_31, m8_31);
    __m512i v512i_33 = _mm512_sub_epi32(v512i_12, v512i_12);
    __m256d v256d_34 = _mm256_mul_pd(v256d_3, v256d_3);
    __m512i v512i_35 = _mm512_abs_epi32(v512i_12);
    __m128i v128i_36 = _mm_abs_epi32(v128i_1);
    __m512i v512i_37 = _mm512_mullo_epi32(v512i_35, v512i_8);
    __m128i v128i_38 = _mm_unpacklo_epi32(v128i_2, v128i_2);
    g_v512i[5] = v512i_33;
    g_v256f[11] = v256f_15;
    g_v256i[8] = v256i_27;
    g_v128d[5] = v128d_17;
    g_v128i[3] = v128i_2;
    g_v512h[1] = v512h_28;
    g_m8[9] = m8_31;
    __m128 v128f_39 = _mm_loadu_ps(fbuf);
    return v128f_39;
}

__m256h torture_145(__m256i p0, __m256h p1, __m128i p2) {
    __m128i v128i_1 = _mm_sub_epi8(p2, p2);
    __m128i v128i_2 = _mm_add_epi8(v128i_1, p2);
    __m128i v128i_3 = _mm_sub_epi8(v128i_1, p2);
    __m512i v512i_4 = g_v512i[11];
    __mmask16 m16_5 = _mm512_cmp_epu32_mask(v512i_4, v512i_4, 4);
    __m512i v512i_6 = _mm512_xor_si512(v512i_4, v512i_4);
    __m512i v512i_7 = _mm512_sub_epi64(v512i_6, v512i_4);
    __m128h v128h_8 = g_v128h[10];
    __m512d v512d_9 = _mm512_cvtph_pd(v128h_8);
    __m512 v512f_10 = _mm512_loadu_ps(fbuf);
    __m512 v512f_11 = _mm512_div_ps(v512f_10, v512f_10);
    __mmask16 m16_12 = _kxor_mask16(m16_5, m16_5);
    __m256 v256f_13 = _mm512_cvtpd_ps(v512d_9);
    __m512h v512h_14 = g_v512h[3];
    __m512h v512h_15 = _mm512_mul_ph(v512h_14, v512h_14);
    __mmask16 m16_16 = _mm512_cmp_epu32_mask(v512i_6, v512i_6, 4);
    __m512i v512i_17 = _mm512_lzcnt_epi32(v512i_4);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_5) & 7); i_18++) {
    __mmask16 m16_19 = _mm512_cmp_epi32_mask(v512i_17, v512i_17, 2);
    __mmask16 m16_20 = _kshiftli_mask16(m16_12, 2);
    }
    __m512i v512i_21 = _mm512_add_epi8(v512i_7, v512i_17);
    unsigned u32_22 = 32;
    __mmask16 m16_23 = _cvtu32_mask16(u32_22);
    __m256d v256d_24 = g_v256d[5];
    __m256d v256d_25 = _mm256_add_pd(v256d_24, v256d_24);
    __m512i v512i_26 = _mm512_rol_epi32(v512i_21, 5);
    __m256i v256i_27 = _mm256_unpacklo_epi32(p0, p0);
    __mmask8 m8_28 = _mm512_cmp_pd_mask(v512d_9, v512d_9, _CMP_GE_OQ);
    __m512i v512i_29 = _mm512_shuffle_i32x4(v512i_7, v512i_17, 0x1B);
    __m256i v256i_30 = _mm256_add_epi8(p0, v256i_27);
    __m512 v512f_31 = _mm512_add_ps(v512f_10, v512f_11);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m512 v512f_32 = _mm512_cvtxph_ps(p1);
    __m512h v512h_33 = _mm512_add_ph(v512h_15, v512h_14);
    __m512i v512i_34 = _mm512_permutexvar_epi32(v512i_4, v512i_4);
    }
    g_v512f[4] = v512f_31;
    g_v512d[5] = v512d_9;
    g_v256d[0] = v256d_25;
    g_v256i[9] = v256i_30;
    g_v512h[0] = v512h_14;
    g_v256h[4] = p1;
    g_m8[2] = m8_28;
    g_m16[0] = m16_5;
    return p1;
}

__m256 torture_146(__m128h p0, __m512h p1, __m256 p2, __m128d p3, __m512h p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_2 = _mm512_cmp_epi32_mask(v512i_1, v512i_1, 2);
    __mmask16 m16_3 = _knot_mask16(m16_2);
    __m128d v128d_4 = _mm_div_pd(p3, p3);
    __m256 v256f_5 = _mm256_mul_ps(p2, p2);
    __mmask16 m16_6 = _mm512_cmp_epu32_mask(v512i_1, v512i_1, 4);
    __m256i v256i_7 = g_v256i[4];
    __m256i v256i_8 = _mm256_abs_epi32(v256i_7);
    __m128i v128i_9 = g_v128i[6];
    __m128i v128i_10 = _mm_shuffle_epi8(v128i_9, v128i_9);
    __m512i v512i_11 = _mm512_add_epi32(v512i_1, v512i_1);
    __m512 v512f_12 = g_v512f[0];
    __m512 v512f_13 = _mm512_sub_ps(v512f_12, v512f_12);
    __mmask8 m8_14 = (__mmask8)(g_m8[6] ^ 171);
    __m512i v512i_15 = _mm512_broadcastmb_epi64(m8_14);
    __m256d v256d_16 = g_v256d[6];
    __m256d v256d_17 = _mm256_min_pd(v256d_16, v256d_16);
    __m128i v128i_18 = _mm_xor_si128(v128i_9, v128i_10);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m128 v128f_19 = g_v128f[11];
    __m128 v128f_20 = _mm_unpacklo_ps(v128f_19, v128f_19);
    __m128 v128f_21 = _mm_set1_ps(1.0f);
    __mmask16 m16_22 = _mm512_movepi32_mask(v512i_11);
    }
    __m512d v512d_23 = _mm512_loadu_pd(dbuf);
    __m256d v256d_24 = _mm512_extractf64x4_pd(v512d_23, 1);
    __m512i v512i_25 = _mm512_and_si512(v512i_15, v512i_1);
    __m512h v512h_26 = _mm512_cvtepi16_ph(v512i_25);
    __m512i v512i_27 = _mm512_sllv_epi32(v512i_15, v512i_15);
    __m128 v128f_28 = _mm_set1_ps(1.0f);
    g_v512d[0] = v512d_23;
    g_v512i[9] = v512i_27;
    g_v256f[9] = p2;
    g_v128f[11] = v128f_28;
    g_v128d[0] = v128d_4;
    g_v512h[6] = v512h_26;
    g_v128h[5] = p0;
    g_m8[9] = m8_14;
    return p2;
}

__m128 torture_147(__m128h p0, __m256d p1, __m512 p2, __m256 p3, __m512i p4) {
    __m128 v128f_1 = _mm_loadu_ps(fbuf);
    __m128 v128f_2 = _mm_fmsub_ps(v128f_1, v128f_1, v128f_1);
    __mmask8 m8_3 = (__mmask8)(g_m8[8] ^ 132);
    __mmask16 m16_4 = _mm512_kunpackb(m8_3, m8_3);
    __m512 v512f_5 = _mm512_xor_ps(p2, p2);
    __m512i v512i_6 = _mm512_abs_epi32(p4);
    __m512 v512f_7 = _mm512_mask_add_ps(p2, m16_4, v512f_5, v512f_5);
    __m512 v512f_8 = _mm512_mask_add_ps(p2, m16_4, v512f_7, v512f_7);
    __m256 v256f_9 = _mm256_mul_ps(p3, p3);
    __m256d v256d_10 = _mm256_sqrt_pd(p1);
    __mmask16 m16_11 = _mm512_fpclass_ps_mask(v512f_7, 0x3);
    __m512i v512i_12 = _mm512_xor_si512(v512i_6, v512i_6);
    __m512 v512f_13 = _mm512_broadcast_f32x4(v128f_1);
    __m512d v512d_14 = _mm512_loadu_pd(dbuf);
    __m512d v512d_15 = _mm512_shuffle_f64x2(v512d_14, v512d_14, 0xEE);
    __m512i v512i_16 = _mm512_broadcastmw_epi32(m16_11);
    __m256 v256f_17 = _mm256_fmsub_ps(p3, v256f_9, p3);
    __m512h v512h_18 = g_v512h[5];
    __m512h v512h_19 = _mm512_getexp_ph(v512h_18);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_4) & 7); i_20++) {
    __m512d v512d_21 = _mm512_cvtph_pd(p0);
    __mmask32 m32_22 = (__mmask32)(g_m32[10] ^ 27);
    __mmask32 m32_23 = _kxor_mask32(m32_22, m32_22);
    __m512d v512d_24 = _mm512_shuffle_f64x2(v512d_15, v512d_14, 0xEE);
    }
    __m512i v512i_25 = _mm512_sllv_epi32(v512i_6, v512i_16);
    __m128i v128i_26 = g_v128i[6];
    __m128i v128i_27 = _mm_xor_si128(v128i_26, v128i_26);
    __m256 v256f_28 = _mm256_unpacklo_ps(v256f_9, p3);
    __m512d v512d_29 = _mm512_mul_pd(v512d_15, v512d_15);
    __m256d v256d_30 = _mm256_sqrt_pd(v256d_10);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_4) & 7); i_31++) {
    __m512 v512f_32 = _mm512_shuffle_ps(p2, p2, 0x1B);
    __m128i v128i_33 = _mm_sub_epi32(v128i_26, v128i_26);
    __m128i v128i_34 = _mm_add_epi64(v128i_33, v128i_33);
    }
    g_v128f[11] = v128f_1;
    g_v128h[11] = p0;
    return v128f_2;
}

__m128i torture_148(__m512h p0, __m512h p1, __m512 p2, __m512h p3, __m512i p4) {
    __mmask16 m16_1 = (__mmask16)(g_m16[5] ^ 44);
    __m512i v512i_2 = _mm512_broadcastmw_epi32(m16_1);
    __m256i v256i_3 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_4 = _mm256_unpacklo_epi32(v256i_3, v256i_3);
    __m128d v128d_5 = g_v128d[5];
    __m128d v128d_6 = _mm_div_pd(v128d_5, v128d_5);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_max_ps(v256f_7, v256f_7);
    __m128 v128f_9 = _mm_loadu_ps(fbuf);
    __m128 v128f_10 = _mm_fmsub_ps(v128f_9, v128f_9, v128f_9);
    __m512 v512f_11 = _mm512_sub_ps(p2, p2);
    __m512i v512i_12 = _mm512_add_epi16(p4, p4);
    __m128i v128i_13 = g_v128i[3];
    __m128i v128i_14 = _mm_mullo_epi32(v128i_13, v128i_13);
    __m512 v512f_15 = _mm512_fmadd_ps(p2, v512f_11, p2);
    __m512i v512i_16 = _mm512_broadcastmw_epi32(m16_1);
    if (((unsigned long long)m16_1) & 1ULL) {
    __mmask8 m8_17 = (__mmask8)(g_m8[2] ^ 184);
    __mmask16 m16_18 = _mm512_kunpackb(m8_17, m8_17);
    __m128i v128i_19 = _mm_abs_epi32(v128i_14);
    __m512 v512f_20 = _mm512_mask3_fmadd_ps(p2, p2, p2, m16_1);
    __m128 v128f_21 = _mm_max_ps(v128f_9, v128f_9);
    }
    __m512 v512f_22 = _mm512_add_ps(v512f_15, p2);
    __m512d v512d_23 = _mm512_loadu_pd(dbuf);
    __m512d v512d_24 = _mm512_add_pd(v512d_23, v512d_23);
    __m512i v512i_25 = _mm512_dpwssd_epi32(p4, v512i_12, p4);
    __m512i v512i_26 = _mm512_sub_epi64(v512i_25, v512i_2);
    __mmask16 m16_27 = _mm512_cmp_epu32_mask(v512i_16, p4, 4);
    g_v512d[3] = v512d_24;
    g_v512i[11] = v512i_2;
    g_v512h[7] = p0;
    return v128i_13;
}

__m256d torture_149(__m256d p0, __m512d p1) {
    __m512d v512d_1 = _mm512_set1_pd(2.0);
    __m512 v512f_2 = g_v512f[9];
    __m512 v512f_3 = _mm512_shuffle_f32x4(v512f_2, v512f_2, 0x4E);
    __m256 v256f_4 = _mm512_cvtpd_ps(p1);
    __m128 v128f_5 = g_v128f[7];
    __m128 v128f_6 = _mm_sqrt_ps(v128f_5);
    __m512h v512h_7 = g_v512h[6];
    __m512h v512h_8 = _mm512_sqrt_ph(v512h_7);
    __m128d v128d_9 = g_v128d[5];
    __m128d v128d_10 = _mm_mul_pd(v128d_9, v128d_9);
    __m512h v512h_11 = _mm512_mul_ph(v512h_8, v512h_8);
    __m512 v512f_12 = _mm512_fmsub_ps(v512f_2, v512f_2, v512f_3);
    __mmask16 m16_13 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    if (((unsigned long long)m16_13) & 1ULL) {
    __m128 v128f_14 = _mm_sub_ps(v128f_6, v128f_5);
    __m512i v512i_15 = g_v512i[4];
    __m512i v512i_16 = _mm512_add_epi32(v512i_15, v512i_15);
    }
    __m512 v512f_17 = _mm512_rsqrt14_ps(v512f_12);
    __m128i v128i_18 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_19 = _mm_add_epi64(v128i_18, v128i_18);
    __mmask16 m16_20 = _mm512_fpclass_ps_mask(v512f_2, 0x3);
    __m512i v512i_21 = _mm512_broadcast_i32x4(v128i_18);
    __m512i v512i_22 = _mm512_cvtps_epi32(v512f_12);
    __mmask16 m16_23 = _kshiftli_mask16(m16_20, 2);
    __m512d v512d_24 = _mm512_sub_pd(v512d_1, p1);
    g_v256f[8] = v256f_4;
    g_v128f[6] = v128f_5;
    g_m16[4] = m16_23;
    return p0;
}

__m256d torture_150(__m128h p0, __m256h p1, __m256d p2, __m512d p3, __m256d p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_shuffle_i32x4(v512i_1, v512i_1, 0x1B);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_add_epi8(v128i_3, v128i_3);
    __m128d v128d_5 = g_v128d[5];
    __m512d v512d_6 = _mm512_insertf64x2(p3, v128d_5, 3);
    __m128i v128i_7 = _mm_sllv_epi32(v128i_3, v128i_4);
    __m256 v256f_8 = g_v256f[1];
    __m256 v256f_9 = _mm256_max_ps(v256f_8, v256f_8);
    __m512 v512f_10 = g_v512f[1];
    __m512 v512f_11 = _mm512_shuffle_ps(v512f_10, v512f_10, 0x1B);
    __m128i v128i_12 = _mm_abs_epi32(v128i_7);
    __m512 v512f_13 = _mm512_insertf32x8(v512f_11, v256f_9, 1);
    __mmask16 m16_14 = _mm512_fpclass_ps_mask(v512f_11, 0x3);
    __m128 v128f_15 = _mm_loadu_ps(fbuf);
    __m128 v128f_16 = _mm_unpacklo_ps(v128f_15, v128f_15);
    __m512i v512i_17 = _mm512_mask_add_epi32(v512i_1, m16_14, v512i_2, v512i_2);
    __m256i v256i_18 = g_v256i[11];
    __m256i v256i_19 = _mm256_add_epi64(v256i_18, v256i_18);
    __m256d v256d_20 = _mm256_min_pd(p4, p4);
    __mmask8 m8_21 = (__mmask8)(g_m8[6] ^ 144);
    __mmask16 m16_22 = _mm512_kunpackb(m8_21, m8_21);
    __m512i v512i_23 = _mm512_slli_epi32(v512i_2, 3);
    __m128h v128h_24 = _mm512_cvtpd_ph(v512d_6);
    __m256 v256f_25 = _mm256_max_ps(v256f_8, v256f_9);
    __m512d v512d_26 = _mm512_sqrt_pd(v512d_6);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m256 v256f_27 = _mm512_cvtpd_ps(v512d_6);
    __m128i v128i_28 = _mm_add_epi16(v128i_7, v128i_7);
    __m256 v256f_29 = _mm256_sqrt_ps(v256f_27);
    }
    __m128d v128d_30 = _mm_sub_pd(v128d_5, v128d_5);
    __m256d v256d_31 = _mm256_max_pd(v256d_20, p2);
    __mmask16 m16_32 = _mm512_testn_epi32_mask(v512i_1, v512i_23);
    __m128d v128d_33 = _mm_fmadd_pd(v128d_5, v128d_30, v128d_5);
    __mmask8 m8_34 = _kand_mask8(m8_21, m8_21);
    __m256i v256i_35 = _mm256_add_epi64(v256i_19, v256i_19);
    __m512i v512i_36 = _mm512_cvtpd_epi64(v512d_26);
    __m512 v512f_37 = _mm512_cvtepi32_ps(v512i_23);
    g_v512f[0] = v512f_11;
    g_v128f[9] = v128f_16;
    g_v128i[10] = v128i_7;
    g_m8[11] = m8_34;
    return v256d_20;
}

__mmask8 torture_151(__m128d p0, __m512d p1, __m512 p2) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_unpacklo_epi32(v128i_1, v128i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_4 = _mm512_cmp_epu32_mask(v512i_3, v512i_3, 4);
    __m128 v128f_5 = g_v128f[4];
    __m128 v128f_6 = _mm_fnmadd_ps(v128f_5, v128f_5, v128f_5);
    __m512i v512i_7 = _mm512_dpbusd_epi32(v512i_3, v512i_3, v512i_3);
    __m128 v128f_8 = _mm_set1_ps(1.0f);
    __m512 v512f_9 = _mm512_sub_ps(p2, p2);
    __mmask16 m16_10 = _mm512_cmp_ps_mask(v512f_9, v512f_9, _CMP_LT_OQ);
    __m128i v128i_11 = _mm_mullo_epi32(v128i_1, v128i_1);
    __m128d v128d_12 = _mm_mul_pd(p0, p0);
    __m128 v128f_13 = _mm_sub_ps(v128f_8, v128f_8);
    __m512i v512i_14 = _mm512_sllv_epi32(v512i_3, v512i_3);
    __m128 v128f_15 = _mm_fmsub_ps(v128f_8, v128f_8, v128f_13);
    __m256 v256f_16 = g_v256f[0];
    __m256 v256f_17 = _mm256_max_ps(v256f_16, v256f_16);
    __m128 v128f_18 = _mm_mul_ps(v128f_15, v128f_5);
    __m128i v128i_19 = _mm_add_epi32(v128i_11, v128i_1);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m512 v512f_20 = _mm512_mask_add_ps(p2, m16_4, p2, v512f_9);
    __m512 v512f_21 = _mm512_shuffle_ps(v512f_20, v512f_20, 0x1B);
    __mmask8 m8_22 = _mm512_cmp_pd_mask(p1, p1, _CMP_GE_OQ);
    }
    __m512i v512i_23 = _mm512_add_epi8(v512i_3, v512i_14);
    __m512d v512d_24 = _mm512_max_pd(p1, p1);
    __m256 v256f_25 = _mm256_fmsub_ps(v256f_16, v256f_17, v256f_17);
    __m256 v256f_26 = _mm256_add_ps(v256f_17, v256f_17);
    unsigned u32_27 = 43;
    __mmask16 m16_28 = _cvtu32_mask16(u32_27);
    __m512i v512i_29 = _mm512_shuffle_i32x4(v512i_23, v512i_23, 0x1B);
    __mmask16 m16_30 = _kshiftri_mask16(m16_28, 1);
    __m256i v256i_31 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_32 = _mm256_mullo_epi32(v256i_31, v256i_31);
    __m128 v128f_33 = _mm_min_ps(v128f_8, v128f_18);
    __m256 v256f_34 = _mm256_mul_ps(v256f_26, v256f_16);
    __m128i v128i_35 = _mm_or_si128(v128i_19, v128i_2);
    __m128 v128f_36 = _mm_fmadd_ps(v128f_8, v128f_13, v128f_5);
    g_v512f[7] = p2;
    g_v512d[6] = p1;
    g_v512i[4] = v512i_14;
    g_v256f[3] = v256f_34;
    g_v256i[8] = v256i_32;
    g_v128f[3] = v128f_33;
    g_v128d[9] = p0;
    g_m16[9] = m16_30;
    __mmask8 m8_37 = (__mmask8)(g_m8[0] ^ 117);
    return m8_37;
}

__m512 torture_152(__m256h p0, __m512 p1, __m256h p2, __m512d p3) {
    __m512 v512f_1 = _mm512_min_ps(p1, p1);
    __mmask16 m16_2 = (__mmask16)(g_m16[11] ^ 245);
    __mmask32 m32_3 = _mm512_kunpackw(m16_2, m16_2);
    __mmask64 m64_4 = (__mmask64)(g_m64[6] ^ 200);
    __mmask64 m64_5 = _kor_mask64(m64_4, m64_4);
    __m128i v128i_6 = g_v128i[2];
    __m128i v128i_7 = _mm_sub_epi64(v128i_6, v128i_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m512d v512d_9 = _mm512_cvtps_pd(v256f_8);
    __m512h v512h_10 = g_v512h[2];
    __m512h v512h_11 = _mm512_rcp_ph(v512h_10);
    __m512d v512d_12 = _mm512_max_pd(v512d_9, p3);
    __m512i v512i_13 = _mm512_set1_epi32(0x55);
    __m512 v512f_14 = _mm512_set1_ps(1.0f);
    __m128 v128f_15 = _mm_loadu_ps(fbuf);
    __m128 v128f_16 = _mm_and_ps(v128f_15, v128f_15);
    __m128 v128f_17 = _mm_set1_ps(1.0f);
    __m128i v128i_18 = _mm_sub_epi16(v128i_7, v128i_6);
    __m128d v128d_19 = g_v128d[3];
    __m128d v128d_20 = _mm_unpackhi_pd(v128d_19, v128d_19);
    __m128 v128f_21 = _mm_unpacklo_ps(v128f_15, v128f_16);
    __m128i v128i_22 = _mm_sub_epi16(v128i_18, v128i_6);
    unsigned u32_23 = 38;
    __mmask16 m16_24 = _cvtu32_mask16(u32_23);
    __mmask16 m16_25 = _mm512_testn_epi32_mask(v512i_13, v512i_13);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_25) & 7); i_26++) {
    __m256d v256d_27 = g_v256d[2];
    __m256d v256d_28 = _mm256_max_pd(v256d_27, v256d_27);
    __m512 v512f_29 = _mm512_add_ps(p1, p1);
    __m512 v512f_30 = _mm512_shuffle_f32x4(v512f_29, v512f_29, 0x4E);
    }
    __m512i v512i_31 = _mm512_add_epi16(v512i_13, v512i_13);
    __m256i v256i_32 = g_v256i[2];
    __m256i v256i_33 = _mm256_abs_epi32(v256i_32);
    __mmask8 m8_34 = _mm512_cmp_pd_mask(v512d_12, p3, _CMP_GE_OQ);
    __mmask16 m16_35 = _kxor_mask16(m16_2, m16_2);
    __m256i v256i_36 = _mm256_add_epi32(v256i_32, v256i_33);
    __m512d v512d_37 = _mm512_fmadd_pd(v512d_12, v512d_9, v512d_9);
    __m512d v512d_38 = _mm512_insertf64x2(v512d_12, v128d_19, 3);
    __m512d v512d_39 = _mm512_sqrt_pd(v512d_9);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m512i v512i_40 = _mm512_sub_epi64(v512i_13, v512i_13);
    __m128i v128i_41 = _mm_sub_epi16(v128i_18, v128i_18);
    __m256i v256i_42 = _mm256_add_epi32(v256i_32, v256i_33);
    }
    g_v512d[0] = v512d_12;
    g_v512i[0] = v512i_13;
    g_v256f[5] = v256f_8;
    g_v256i[1] = v256i_36;
    g_v128i[1] = v128i_7;
    g_v256h[8] = p0;
    g_m8[4] = m8_34;
    return v512f_1;
}

__mmask8 torture_153(__m256h p0, __m256i p1, __m256 p2, __m512d p3, __m512 p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_add_epi64(v512i_1, v512i_1);
    __m128d v128d_3 = g_v128d[5];
    __m128d v128d_4 = _mm_fmadd_pd(v128d_3, v128d_3, v128d_3);
    __mmask64 m64_5 = _mm512_movepi8_mask(v512i_2);
    __m128i v128i_6 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_7 = _mm_mullo_epi32(v128i_6, v128i_6);
    __m128i v128i_8 = _mm_add_epi64(v128i_6, v128i_6);
    __mmask16 m16_9 = (__mmask16)(g_m16[5] ^ 50);
    __m512 v512f_10 = _mm512_maskz_mul_ps(m16_9, p4, p4);
    __m512i v512i_11 = _mm512_abs_epi32(v512i_2);
    __m128i v128i_12 = _mm_or_si128(v128i_7, v128i_8);
    __m512 v512f_13 = _mm512_mul_ps(p4, v512f_10);
    __m128h v128h_14 = g_v128h[5];
    __m512d v512d_15 = _mm512_cvtph_pd(v128h_14);
    __m128i v128i_16 = _mm_slli_epi32(v128i_6, 3);
    __m512d v512d_17 = _mm512_min_pd(p3, p3);
    __m512i v512i_18 = _mm512_add_epi32(v512i_11, v512i_1);
    __m256d v256d_19 = g_v256d[3];
    __m256d v256d_20 = _mm256_mul_pd(v256d_19, v256d_19);
    __m128 v128f_21 = _mm_loadu_ps(fbuf);
    __m128 v128f_22 = _mm_min_ps(v128f_21, v128f_21);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m512 v512f_23 = _mm512_set1_ps(1.0f);
    __m512 v512f_24 = _mm512_set1_ps(1.0f);
    __m512i v512i_25 = _mm512_or_si512(v512i_2, v512i_1);
    __mmask8 m8_26 = (__mmask8)(g_m8[3] ^ 190);
    __m512d v512d_27 = _mm512_mask_blend_pd(m8_26, v512d_17, p3);
    __m128 v128f_28 = _mm_fmsub_ps(v128f_21, v128f_21, v128f_21);
    }
    __m512i v512i_29 = _mm512_sllv_epi32(v512i_2, v512i_18);
    __mmask8 m8_30 = (__mmask8)(g_m8[8] ^ 178);
    __m512d v512d_31 = _mm512_mask_blend_pd(m8_30, v512d_15, v512d_17);
    __m256d v256d_32 = _mm256_sub_pd(v256d_19, v256d_19);
    __m512i v512i_33 = _mm512_abs_epi32(v512i_18);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_9) & 7); i_34++) {
    __m128 v128f_35 = _mm_min_ps(v128f_21, v128f_21);
    __m512i v512i_36 = _mm512_ternarylogic_epi32(v512i_2, v512i_11, v512i_18, 0x96);
    }
    g_v512d[2] = v512d_31;
    g_v128i[8] = v128i_6;
    g_m64[2] = m64_5;
    return m8_30;
}

__mmask32 torture_154(__m512i p0, __m512i p1, __m256i p2, __m256 p3) {
    __m512i v512i_1 = _mm512_set1_epi32(0x55);
    __m128d v128d_2 = g_v128d[4];
    __m128d v128d_3 = _mm_sqrt_pd(v128d_2);
    __m128 v128f_4 = g_v128f[11];
    __m128 v128f_5 = _mm_sub_ps(v128f_4, v128f_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m128 v128f_7 = _mm512_extractf32x4_ps(v512f_6, 2);
    __m128 v128f_8 = _mm_min_ps(v128f_7, v128f_7);
    __m512 v512f_9 = _mm512_cvtepi32_ps(v512i_1);
    __m512 v512f_10 = _mm512_cvtepi32_ps(p1);
    __m256i v256i_11 = _mm256_unpacklo_epi32(p2, p2);
    __m512i v512i_12 = _mm512_shuffle_i32x4(p0, p1, 0x1B);
    __m256d v256d_13 = g_v256d[11];
    __m256d v256d_14 = _mm256_div_pd(v256d_13, v256d_13);
    __m512d v512d_15 = g_v512d[7];
    __mmask8 m8_16 = _mm512_cmp_pd_mask(v512d_15, v512d_15, _CMP_GE_OQ);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m8_16) & 7); i_17++) {
    __m128 v128f_18 = _mm_fmsub_ps(v128f_5, v128f_7, v128f_5);
    __m128i v128i_19 = _mm512_cvtepi32_epi8(v512i_1);
    __m128 v128f_20 = _mm_min_ps(v128f_18, v128f_7);
    }
    __m128i v128i_21 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_22 = _mm_xor_si128(v128i_21, v128i_21);
    __mmask16 m16_23 = (__mmask16)(g_m16[6] ^ 120);
    __mmask16 m16_24 = _kshiftri_mask16(m16_23, 1);
    __mmask16 m16_25 = _kxor_mask16(m16_23, m16_23);
    __m256h v256h_26 = g_v256h[6];
    __m512 v512f_27 = _mm512_cvtxph_ps(v256h_26);
    __m512d v512d_28 = _mm512_unpackhi_pd(v512d_15, v512d_15);
    __m256 v256f_29 = _mm256_set1_ps(1.0f);
    __m128d v128d_30 = _mm_sqrt_pd(v128d_3);
    __m256i v256i_31 = _mm256_add_epi16(p2, v256i_11);
    __m128 v128f_32 = _mm512_extractf32x4_ps(v512f_6, 2);
    if (((unsigned long long)m16_23) & 1ULL) {
    __m512h v512h_33 = g_v512h[5];
    __m512h v512h_34 = _mm512_add_ph(v512h_33, v512h_33);
    __m512d v512d_35 = _mm512_sqrt_pd(v512d_28);
    __m128d v128d_36 = _mm_unpackhi_pd(v128d_2, v128d_30);
    __m128 v128f_37 = _mm_fmsub_ps(v128f_8, v128f_7, v128f_5);
    }
    g_v512d[11] = v512d_15;
    g_v256d[6] = v256d_14;
    g_v128d[11] = v128d_2;
    g_v256h[11] = v256h_26;
    g_m8[0] = m8_16;
    __mmask32 m32_38 = (__mmask32)(g_m32[2] ^ 94);
    return m32_38;
}

__m512i torture_155(__m256h p0, __m256h p1, __m256h p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_2 = (__mmask16)(g_m16[8] ^ 83);
    __m512 v512f_3 = _mm512_mask3_fmadd_ps(v512f_1, v512f_1, v512f_1, m16_2);
    __m512i v512i_4 = g_v512i[11];
    __m512i v512i_5 = _mm512_rolv_epi32(v512i_4, v512i_4);
    __m512h v512h_6 = g_v512h[5];
    __m512h v512h_7 = _mm512_getexp_ph(v512h_6);
    __m256d v256d_8 = g_v256d[7];
    __m256d v256d_9 = _mm256_unpackhi_pd(v256d_8, v256d_8);
    __m512d v512d_10 = _mm512_loadu_pd(dbuf);
    __m512d v512d_11 = _mm512_max_pd(v512d_10, v512d_10);
    __m512 v512f_12 = _mm512_sqrt_ps(v512f_3);
    __m512i v512i_13 = _mm512_or_si512(v512i_4, v512i_5);
    __m512d v512d_14 = _mm512_unpackhi_pd(v512d_10, v512d_11);
    __m512 v512f_15 = _mm512_scalef_ps(v512f_3, v512f_3);
    __m256i v256i_16 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_17 = _mm256_slli_epi32(v256i_16, 3);
    __m128i v128i_18 = g_v128i[8];
    __m128i v128i_19 = _mm_add_epi32(v128i_18, v128i_18);
    __m256 v256f_20 = g_v256f[7];
    __m256 v256f_21 = _mm256_unpacklo_ps(v256f_20, v256f_20);
    unsigned u32_22 = 63;
    __mmask16 m16_23 = _cvtu32_mask16(u32_22);
    __m128d v128d_24 = g_v128d[1];
    __m128d v128d_25 = _mm_div_pd(v128d_24, v128d_24);
    __m128i v128i_26 = _mm_set1_epi32(0x55);
    __m128d v128d_27 = _mm_add_pd(v128d_25, v128d_25);
    __m512i v512i_28 = _mm512_permutexvar_epi32(v512i_13, v512i_13);
    for (int i_29 = 0; i_29 < (int)(((unsigned long long)m16_23) & 7); i_29++) {
    __m128d v128d_30 = _mm_sqrt_pd(v128d_27);
    __m512d v512d_31 = _mm512_sqrt_pd(v512d_10);
    __m128i v128i_32 = _mm_set1_epi32(0x55);
    __m128 v128f_33 = _mm_set1_ps(1.0f);
    __m512h v512h_34 = _mm512_sqrt_ph(v512h_7);
    }
    g_v512f[6] = v512f_1;
    g_v512d[10] = v512d_11;
    g_v256f[3] = v256f_21;
    g_v128i[5] = v128i_18;
    g_v256h[1] = p0;
    return v512i_13;
}

__mmask32 torture_156(__m256d p0, __m256 p1, __m256i p2, __m256i p3, __m512d p4) {
    __m128i v128i_1 = g_v128i[1];
    __m128i v128i_2 = _mm_sub_epi16(v128i_1, v128i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_and_ps(v512f_3, v512f_3);
    __m256i v256i_5 = _mm256_set1_epi32(0x55);
    __m512d v512d_6 = _mm512_unpackhi_pd(p4, p4);
    __m256i v256i_7 = _mm256_add_epi8(p3, p2);
    __m128h v128h_8 = _mm512_cvtpd_ph(v512d_6);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_add_epi8(v512i_9, v512i_9);
    __m512i v512i_11 = _mm512_cvtpd_epi64(v512d_6);
    __m256d v256d_12 = _mm256_min_pd(p0, p0);
    int s32_13 = 94;
    if ((s32_13) & 1ULL) {
    __m512d v512d_14 = _mm512_mul_pd(p4, v512d_6);
    __m128 v128f_15 = _mm_loadu_ps(fbuf);
    __m128 v128f_16 = _mm_min_ps(v128f_15, v128f_15);
    __m128i v128i_17 = _mm_slli_epi32(v128i_2, 3);
    __m512d v512d_18 = _mm512_set1_pd(2.0);
    __mmask16 m16_19 = _mm512_cmp_ps_mask(v512f_3, v512f_4, _CMP_LT_OQ);
    }
    __m128i v128i_20 = _mm_sub_epi8(v128i_1, v128i_2);
    __mmask16 m16_21 = _mm512_cmp_ps_mask(v512f_4, v512f_3, _CMP_LT_OQ);
    __m512i v512i_22 = _mm512_conflict_epi32(v512i_9);
    __m256 v256f_23 = _mm256_fmsub_ps(p1, p1, p1);
    __m128h v128h_24 = _mm512_cvtpd_ph(v512d_6);
    __m256i v256i_25 = _mm256_sub_epi16(v256i_5, v256i_7);
    g_v512i[11] = v512i_22;
    g_v256f[3] = p1;
    __mmask32 m32_26 = (__mmask32)(g_m32[10] ^ 23);
    return m32_26;
}

__mmask32 torture_157(__m256d p0, __m128i p1, __m512i p2, __m128i p3, __m256 p4) {
    __m128i v128i_1 = _mm_xor_si128(p3, p3);
    __mmask16 m16_2 = (__mmask16)(g_m16[4] ^ 121);
    __mmask16 m16_3 = _kand_mask16(m16_2, m16_2);
    __m256d v256d_4 = _mm256_unpackhi_pd(p0, p0);
    __m512h v512h_5 = g_v512h[10];
    __m512h v512h_6 = _mm512_add_ph(v512h_5, v512h_5);
    __m512h v512h_7 = _mm512_fmadd_ph(v512h_6, v512h_6, v512h_5);
    __m256d v256d_8 = _mm256_min_pd(v256d_4, p0);
    __m128 v128f_9 = g_v128f[3];
    __m128 v128f_10 = _mm_shuffle_ps(v128f_9, v128f_9, 0x1B);
    __mmask32 m32_11 = _mm512_cmp_ph_mask(v512h_5, v512h_7, 1);
    __m512i v512i_12 = _mm512_dpbusd_epi32(p2, p2, p2);
    __m128d v128d_13 = g_v128d[10];
    __m128d v128d_14 = _mm_add_pd(v128d_13, v128d_13);
    __m128i v128i_15 = _mm_sllv_epi32(p1, p3);
    __mmask8 m8_16 = (__mmask8)(g_m8[5] ^ 15);
    __mmask8 m8_17 = _kand_mask8(m8_16, m8_16);
    __m128 v128f_18 = _mm_sqrt_ps(v128f_9);
    __m256i v256i_19 = g_v256i[7];
    __m256i v256i_20 = _mm256_abs_epi32(v256i_19);
    __m128i v128i_21 = _mm_mullo_epi32(p1, p3);
    __m128i v128i_22 = _mm_add_epi64(v128i_1, v128i_1);
    __mmask16 m16_23 = _kshiftri_mask16(m16_3, 1);
    __m512d v512d_24 = _mm512_set1_pd(2.0);
    __m256 v256f_25 = _mm256_min_ps(p4, p4);
    __m128i v128i_26 = _mm_sub_epi8(p1, v128i_15);
    g_v512i[0] = v512i_12;
    g_v256i[5] = v256i_20;
    g_m8[6] = m8_16;
    return m32_11;
}

__m256 torture_158(__m256 p0, __m512i p1, __m128i p2, __m512h p3, __m256h p4) {
    __m256 v256f_1 = _mm256_sqrt_ps(p0);
    __m512i v512i_2 = _mm512_rol_epi32(p1, 5);
    __m256i v256i_3 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_4 = _mm256_sub_epi8(v256i_3, v256i_3);
    __m512i v512i_5 = _mm512_add_epi64(p1, v512i_2);
    __m128d v128d_6 = g_v128d[8];
    __m128d v128d_7 = _mm_mul_pd(v128d_6, v128d_6);
    __mmask16 m16_8 = _mm512_movepi32_mask(v512i_5);
    __m512 v512f_9 = g_v512f[7];
    __m512 v512f_10 = _mm512_sqrt_ps(v512f_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_sqrt_pd(v512d_11);
    __m256i v256i_13 = _mm256_add_epi8(v256i_3, v256i_3);
    __m128i v128i_14 = _mm_add_epi32(p2, p2);
    __m512i v512i_15 = _mm512_popcnt_epi64(v512i_5);
    __m512h v512h_16 = _mm512_sqrt_ph(p3);
    unsigned u32_17 = _cvtmask16_u32(m16_8);
    __m128 v128f_18 = g_v128f[4];
    __m128 v128f_19 = _mm_max_ps(v128f_18, v128f_18);
    __m512i v512i_20 = _mm512_and_si512(v512i_15, v512i_5);
    __m128i v128i_21 = _mm_set1_epi32(0x55);
    __m512 v512f_22 = _mm512_div_ps(v512f_10, v512f_9);
    __m512i v512i_23 = _mm512_add_epi16(v512i_20, v512i_20);
    __m512 v512f_24 = _mm512_getexp_ps(v512f_9);
    __m512h v512h_25 = _mm512_mul_ph(p3, v512h_16);
    __m128i v128i_26 = _mm_add_epi16(v128i_21, p2);
    __m512 v512f_27 = _mm512_and_ps(v512f_22, v512f_24);
    __m512i v512i_28 = _mm512_add_epi16(v512i_2, v512i_5);
    __m128d v128d_29 = _mm_div_pd(v128d_7, v128d_7);
    __m512 v512f_30 = _mm512_insertf32x8(v512f_27, v256f_1, 1);
    __m256 v256f_31 = _mm256_fmadd_ps(v256f_1, p0, p0);
    __m512d v512d_32 = _mm512_div_pd(v512d_12, v512d_12);
    g_v512f[7] = v512f_27;
    g_v512d[0] = v512d_32;
    g_v512i[8] = v512i_23;
    g_v256f[3] = v256f_1;
    g_v128f[7] = v128f_18;
    g_v128d[9] = v128d_29;
    g_v128i[6] = p2;
    g_v256h[10] = p4;
    return v256f_31;
}

__m256 torture_159(__m256 p0, __m512 p1) {
    __m512d v512d_1 = _mm512_set1_pd(2.0);
    __m512i v512i_2 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_3 = _mm512_unpacklo_epi32(v512i_2, v512i_2);
    __m512h v512h_4 = g_v512h[8];
    __m512h v512h_5 = _mm512_mul_ph(v512h_4, v512h_4);
    __m512i v512i_6 = _mm512_add_epi8(v512i_2, v512i_2);
    __mmask16 m16_7 = _mm512_cmp_epu32_mask(v512i_6, v512i_6, 4);
    __m512 v512f_8 = _mm512_getexp_ps(p1);
    __m256 v256f_9 = _mm256_shuffle_ps(p0, p0, 0x1B);
    __m512i v512i_10 = _mm512_rolv_epi32(v512i_2, v512i_2);
    __mmask8 m8_11 = _mm512_cmp_pd_mask(v512d_1, v512d_1, _CMP_GE_OQ);
    __m512i v512i_12 = _mm512_set1_epi32(0x55);
    __m512d v512d_13 = _mm512_div_pd(v512d_1, v512d_1);
    __m512 v512f_14 = _mm512_set1_ps(1.0f);
    __m512 v512f_15 = _mm512_maskz_mul_ps(m16_7, p1, p1);
    __mmask16 m16_16 = _mm512_cmp_ps_mask(v512f_14, v512f_8, _CMP_LT_OQ);
    __m256i v256i_17 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_18 = _mm256_add_epi32(v256i_17, v256i_17);
    __mmask32 m32_19 = (__mmask32)(g_m32[11] ^ 218);
    __mmask32 m32_20 = _kxor_mask32(m32_19, m32_19);
    __m128i v128i_21 = _mm_set1_epi32(0x55);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512i v512i_22 = _mm512_conflict_epi32(v512i_6);
    __m512 v512f_23 = _mm512_shuffle_ps(v512f_15, p1, 0x1B);
    __m512i v512i_24 = _mm512_unpacklo_epi32(v512i_12, v512i_10);
    __m128d v128d_25 = g_v128d[8];
    __m128d v128d_26 = _mm_mul_pd(v128d_25, v128d_25);
    __mmask16 m16_27 = _mm512_cmp_epu32_mask(v512i_12, v512i_2, 4);
    }
    g_v512f[7] = v512f_8;
    g_v512i[5] = v512i_6;
    g_v256i[2] = v256i_18;
    g_m8[10] = m8_11;
    g_m16[9] = m16_7;
    return v256f_9;
}

__m128 torture_160(__m128h p0, __m128h p1) {
    __m256d v256d_1 = g_v256d[4];
    __m256d v256d_2 = _mm256_mul_pd(v256d_1, v256d_1);
    __m128d v128d_3 = _mm_set1_pd(2.0);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_lzcnt_epi32(v512i_4);
    __m256d v256d_6 = _mm256_max_pd(v256d_2, v256d_1);
    __mmask16 m16_7 = _mm512_cmp_epi32_mask(v512i_4, v512i_5, 2);
    __m512 v512f_8 = _mm512_cvtepi32_ps(v512i_4);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m512d v512d_10 = _mm512_min_pd(v512d_9, v512d_9);
    __m256i v256i_11 = g_v256i[1];
    __m256i v256i_12 = _mm256_sub_epi16(v256i_11, v256i_11);
    for (int i_13 = 0; i_13 < (int)(((unsigned long long)m16_7) & 7); i_13++) {
    __m128 v128f_14 = _mm_loadu_ps(fbuf);
    __m128 v128f_15 = _mm_sub_ps(v128f_14, v128f_14);
    __m256 v256f_16 = _mm512_cvtpd_ps(v512d_10);
    __m128i v128i_17 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_18 = _mm_add_epi32(v128i_17, v128i_17);
    __m512d v512d_19 = _mm512_div_pd(v512d_10, v512d_9);
    }
    __m512i v512i_20 = _mm512_abs_epi32(v512i_4);
    __m128 v128f_21 = g_v128f[6];
    __m128 v128f_22 = _mm_fnmadd_ps(v128f_21, v128f_21, v128f_21);
    __m512i v512i_23 = _mm512_cvtps_epi32(v512f_8);
    __m256i v256i_24 = _mm256_abs_epi32(v256i_12);
    __m256 v256f_25 = _mm256_loadu_ps(fbuf);
    __m256 v256f_26 = _mm256_add_ps(v256f_25, v256f_25);
    __m256 v256f_27 = _mm256_sqrt_ps(v256f_26);
    __m512d v512d_28 = _mm512_shuffle_f64x2(v512d_10, v512d_9, 0xEE);
    __m512i v512i_29 = _mm512_add_epi64(v512i_5, v512i_4);
    __m128d v128d_30 = _mm_min_pd(v128d_3, v128d_3);
    __m512i v512i_31 = _mm512_rolv_epi32(v512i_5, v512i_29);
    __m128i v128i_32 = g_v128i[4];
    __m128i v128i_33 = _mm_abs_epi32(v128i_32);
    __m512i v512i_34 = _mm512_dpwssd_epi32(v512i_4, v512i_20, v512i_5);
    return v128f_21;
}

__m256d torture_161(__m128 p0, __m256d p1, __m128i p2, __m256h p3, __m512h p4) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_xor_ps(v512f_1, v512f_1);
    __m512i v512i_3 = _mm512_cvtepi8_epi32(p2);
    __m256d v256d_4 = _mm256_sqrt_pd(p1);
    __m128i v128i_5 = _mm_or_si128(p2, p2);
    __m512i v512i_6 = _mm512_permutex2var_epi32(v512i_3, v512i_3, v512i_3);
    __m512i v512i_7 = _mm512_sub_epi8(v512i_6, v512i_6);
    __m128i v128i_8 = _mm_add_epi16(p2, v128i_5);
    __m128h v128h_9 = g_v128h[1];
    __m512d v512d_10 = _mm512_cvtph_pd(v128h_9);
    __m128d v128d_11 = g_v128d[4];
    __m128d v128d_12 = _mm_sub_pd(v128d_11, v128d_11);
    __m512i v512i_13 = _mm512_sub_epi64(v512i_3, v512i_3);
    __mmask16 m16_14 = (__mmask16)(g_m16[9] ^ 166);
    __mmask16 m16_15 = _kxor_mask16(m16_14, m16_14);
    __m256 v256f_16 = _mm256_loadu_ps(fbuf);
    __m256 v256f_17 = _mm256_add_ps(v256f_16, v256f_16);
    __mmask16 m16_18 = _mm512_cmp_epu32_mask(v512i_7, v512i_3, 4);
    __m128 v128f_19 = _mm512_extractf32x4_ps(v512f_2, 2);
    __m256d v256d_20 = _mm256_unpackhi_pd(v256d_4, v256d_4);
    __m256 v256f_21 = _mm256_set1_ps(1.0f);
    __m512 v512f_22 = _mm512_roundscale_ps(v512f_2, 0x10);
    __m128 v128f_23 = _mm_mul_ps(p0, p0);
    if (((unsigned long long)m16_18) & 1ULL) {
    __m512 v512f_24 = _mm512_getexp_ps(v512f_22);
    __m512 v512f_25 = _mm512_getexp_ps(v512f_2);
    __m512 v512f_26 = _mm512_rcp14_ps(v512f_1);
    }
    __m256i v256i_27 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_28 = _mm256_add_epi8(v256i_27, v256i_27);
    __m512 v512f_29 = _mm512_rsqrt14_ps(v512f_1);
    __mmask32 m32_30 = (__mmask32)(g_m32[0] ^ 162);
    __mmask32 m32_31 = _kxor_mask32(m32_30, m32_30);
    __mmask8 m8_32 = _mm512_cmp_pd_mask(v512d_10, v512d_10, _CMP_GE_OQ);
    __m128 v128f_33 = _mm_xor_ps(p0, v128f_23);
    __m256d v256d_34 = _mm256_sub_pd(v256d_20, p1);
    __mmask32 m32_35 = _mm512_kunpackw(m16_14, m16_18);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_15) & 7); i_36++) {
    __mmask32 m32_37 = _mm512_cmp_ph_mask(p4, p4, 1);
    __m128i v128i_38 = _mm_add_epi32(v128i_8, v128i_5);
    __m256 v256f_39 = _mm256_fmsub_ps(v256f_21, v256f_17, v256f_21);
    __m128d v128d_40 = _mm_max_pd(v128d_12, v128d_12);
    }
    g_v512f[3] = v512f_2;
    g_v256f[1] = v256f_16;
    g_v256d[10] = p1;
    g_v256i[9] = v256i_27;
    g_m32[11] = m32_31;
    return p1;
}

__m512d torture_162(__m512d p0, __m128h p1, __m128d p2, __m512d p3, __m512 p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_2 = _mm512_cmp_epu32_mask(v512i_1, v512i_1, 4);
    __mmask8 m8_3 = (__mmask8)(g_m8[10] ^ 230);
    __mmask8 m8_4 = _kand_mask8(m8_3, m8_3);
    __m128d v128d_5 = _mm_unpackhi_pd(p2, p2);
    __m512d v512d_6 = _mm512_set1_pd(2.0);
    __m256 v256f_7 = g_v256f[11];
    __m256 v256f_8 = _mm256_add_ps(v256f_7, v256f_7);
    __m128i v128i_9 = g_v128i[10];
    __m128i v128i_10 = _mm_add_epi16(v128i_9, v128i_9);
    __m512 v512f_11 = _mm512_mul_ps(p4, p4);
    __m512d v512d_12 = _mm512_cvtph_pd(p1);
    __m256i v256i_13 = g_v256i[0];
    __m256i v256i_14 = _mm256_slli_epi32(v256i_13, 3);
    __mmask16 m16_15 = _mm512_movepi32_mask(v512i_1);
    __m512i v512i_16 = _mm512_add_epi8(v512i_1, v512i_1);
    __m256i v256i_17 = _mm256_sub_epi32(v256i_14, v256i_14);
    __m512h v512h_18 = g_v512h[11];
    __m512h v512h_19 = _mm512_fmsub_ph(v512h_18, v512h_18, v512h_18);
    __m256i v256i_20 = _mm512_cvtepi64_epi32(v512i_16);
    __m512 v512f_21 = _mm512_shuffle_f32x4(v512f_11, p4, 0x4E);
    __m512i v512i_22 = _mm512_shuffle_i32x4(v512i_16, v512i_16, 0x1B);
    __m512 v512f_23 = _mm512_mask_add_ps(v512f_21, m16_2, v512f_21, p4);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512i v512i_24 = _mm512_sub_epi8(v512i_1, v512i_16);
    __m512i v512i_25 = _mm512_cvtepi8_epi32(v128i_10);
    __m512i v512i_26 = _mm512_or_si512(v512i_1, v512i_25);
    __mmask16 m16_27 = _mm512_cmp_epi32_mask(v512i_26, v512i_24, 2);
    __mmask16 m16_28 = _mm512_cmp_ps_mask(v512f_23, v512f_21, _CMP_LT_OQ);
    __m512 v512f_29 = _mm512_fmadd_ps(v512f_21, p4, v512f_23);
    }
    __m512i v512i_30 = _mm512_set1_epi32(0x55);
    __m128d v128d_31 = _mm_min_pd(p2, p2);
    __mmask64 m64_32 = (__mmask64)(g_m64[6] ^ 80);
    __mmask64 m64_33 = _kor_mask64(m64_32, m64_32);
    __m256 v256f_34 = _mm256_max_ps(v256f_7, v256f_8);
    __m256 v256f_35 = _mm256_min_ps(v256f_8, v256f_8);
    __m512h v512h_36 = _mm512_cvtepi16_ph(v512i_22);
    __m128h v128h_37 = _mm512_cvtpd_ph(p0);
    __m128i v128i_38 = _mm_slli_epi32(v128i_9, 3);
    __m256 v256f_39 = _mm256_fmsub_ps(v256f_8, v256f_7, v256f_35);
    for (int i_40 = 0; i_40 < (int)(((unsigned long long)m16_15) & 7); i_40++) {
    __m128d v128d_41 = _mm_unpackhi_pd(v128d_31, v128d_5);
    __m512h v512h_42 = _mm512_add_ph(v512h_18, v512h_19);
    __m512 v512f_43 = _mm512_and_ps(v512f_11, v512f_23);
    __m512 v512f_44 = _mm512_and_ps(v512f_21, v512f_21);
    }
    g_v512f[6] = v512f_21;
    g_v256i[4] = v256i_17;
    g_v128d[0] = p2;
    g_v512h[6] = v512h_19;
    g_v128h[8] = p1;
    g_m8[7] = m8_3;
    g_m16[7] = m16_15;
    return p0;
}

__mmask32 torture_163(__m128i p0, __m256h p1) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sllv_epi32(v256i_1, v256i_1);
    __m512i v512i_3 = g_v512i[8];
    __m512i v512i_4 = _mm512_or_si512(v512i_3, v512i_3);
    __m512 v512f_5 = _mm512_cvtepi32_ps(v512i_4);
    __m512h v512h_6 = g_v512h[4];
    __m512h v512h_7 = _mm512_sqrt_ph(v512h_6);
    __m512i v512i_8 = _mm512_cvtepi8_epi32(p0);
    __m256i v256i_9 = _mm256_mullo_epi32(v256i_2, v256i_2);
    __m512 v512f_10 = _mm512_unpacklo_ps(v512f_5, v512f_5);
    __m512d v512d_11 = g_v512d[10];
    __m128h v128h_12 = _mm512_cvtpd_ph(v512d_11);
    __m128i v128i_13 = _mm_or_si128(p0, p0);
    __m128i v128i_14 = _mm_and_si128(p0, v128i_13);
    __m128i v128i_15 = _mm_add_epi64(v128i_14, p0);
    int s32_16 = 55;
    for (int i_17 = 0; i_17 < (int)((s32_16) & 7); i_17++) {
    __mmask64 m64_18 = _mm512_cmp_epi8_mask(v512i_3, v512i_4, 1);
    __m128i v128i_19 = _mm_xor_si128(v128i_13, p0);
    __m512 v512f_20 = _mm512_and_ps(v512f_10, v512f_5);
    __m512i v512i_21 = _mm512_permutexvar_epi32(v512i_8, v512i_8);
    }
    __m512d v512d_22 = _mm512_sub_pd(v512d_11, v512d_11);
    __m128i v128i_23 = _mm_add_epi8(v128i_13, p0);
    __m512i v512i_24 = _mm512_cvtph_epi16(v512h_7);
    __m512h v512h_25 = _mm512_sqrt_ph(v512h_7);
    __m512i v512i_26 = _mm512_add_epi16(v512i_24, v512i_8);
    __m512i v512i_27 = _mm512_cvtepi8_epi32(v128i_23);
    __m256 v256f_28 = g_v256f[5];
    __m256 v256f_29 = _mm256_shuffle_ps(v256f_28, v256f_28, 0x1B);
    __m256d v256d_30 = g_v256d[8];
    __m256d v256d_31 = _mm256_min_pd(v256d_30, v256d_30);
    __m128d v128d_32 = g_v128d[4];
    __m128d v128d_33 = _mm_max_pd(v128d_32, v128d_32);
    g_v512d[0] = v512d_22;
    g_v512i[2] = v512i_4;
    g_v256d[5] = v256d_31;
    g_v128i[2] = v128i_13;
    __mmask32 m32_34 = (__mmask32)(g_m32[10] ^ 72);
    return m32_34;
}

__m512d torture_164(__m128h p0, __m256i p1, __m512h p2, __m256i p3) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_shuffle_ps(v512f_1, v512f_1, 0x1B);
    __mmask8 m8_3 = (__mmask8)(g_m8[8] ^ 103);
    __mmask16 m16_4 = _mm512_kunpackb(m8_3, m8_3);
    __m512h v512h_5 = _mm512_fmul_pch(p2, p2);
    __m256i v256i_6 = _mm256_sub_epi8(p1, p3);
    __m128d v128d_7 = _mm_set1_pd(2.0);
    __m512i v512i_8 = _mm512_broadcastmw_epi32(m16_4);
    __m256d v256d_9 = g_v256d[2];
    __m256d v256d_10 = _mm256_max_pd(v256d_9, v256d_9);
    __m256i v256i_11 = _mm256_add_epi64(p1, p1);
    __m512 v512f_12 = _mm512_rsqrt14_ps(v512f_1);
    __m512i v512i_13 = _mm512_permutex2var_epi32(v512i_8, v512i_8, v512i_8);
    __m256i v256i_14 = _mm256_sub_epi16(v256i_11, v256i_11);
    __m512i v512i_15 = _mm512_broadcastmw_epi32(m16_4);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m256 v256f_16 = g_v256f[0];
    __m256 v256f_17 = _mm256_max_ps(v256f_16, v256f_16);
    __m512 v512f_18 = _mm512_rcp14_ps(v512f_1);
    __m512d v512d_19 = _mm512_loadu_pd(dbuf);
    __m512d v512d_20 = _mm512_max_pd(v512d_19, v512d_19);
    __m128d v128d_21 = _mm_set1_pd(2.0);
    }
    __m128d v128d_22 = _mm_add_pd(v128d_7, v128d_7);
    __m512d v512d_23 = _mm512_loadu_pd(dbuf);
    __m256d v256d_24 = _mm512_extractf64x4_pd(v512d_23, 1);
    __m512 v512f_25 = _mm512_min_ps(v512f_1, v512f_12);
    __m128i v128i_26 = _mm_set1_epi32(0x55);
    __mmask16 m16_27 = _mm512_movepi32_mask(v512i_8);
    __m512h v512h_28 = _mm512_fmul_pch(p2, p2);
    __m256i v256i_29 = _mm256_add_epi8(v256i_11, v256i_11);
    g_v512d[1] = v512d_23;
    g_v512i[3] = v512i_15;
    g_v128i[0] = v128i_26;
    g_v512h[3] = v512h_28;
    g_m8[1] = m8_3;
    return v512d_23;
}

__m128d torture_165(__m512h p0, __m256h p1) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m256 v256f_2 = _mm512_cvtpd_ps(v512d_1);
    __m512i v512i_3 = g_v512i[9];
    __m512i v512i_4 = _mm512_permutex2var_epi32(v512i_3, v512i_3, v512i_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[7] ^ 56);
    __mmask16 m16_6 = _kshiftri_mask16(m16_5, 1);
    __m128d v128d_7 = g_v128d[9];
    __m128d v128d_8 = _mm_fmadd_pd(v128d_7, v128d_7, v128d_7);
    __m128i v128i_9 = g_v128i[9];
    __m128i v128i_10 = _mm_add_epi16(v128i_9, v128i_9);
    __m512i v512i_11 = _mm512_mullo_epi32(v512i_3, v512i_3);
    __m512i v512i_12 = _mm512_permutex2var_epi32(v512i_11, v512i_3, v512i_4);
    __m256i v256i_13 = g_v256i[1];
    __m256i v256i_14 = _mm256_add_epi16(v256i_13, v256i_13);
    __m512i v512i_15 = _mm512_permutexvar_epi32(v512i_3, v512i_4);
    __m512 v512f_16 = g_v512f[9];
    __m512 v512f_17 = _mm512_shuffle_ps(v512f_16, v512f_16, 0x1B);
    __m512 v512f_18 = _mm512_max_ps(v512f_17, v512f_16);
    __m256d v256d_19 = g_v256d[9];
    __m256d v256d_20 = _mm256_sqrt_pd(v256d_19);
    __mmask16 m16_21 = _mm512_cmp_ps_mask(v512f_18, v512f_17, _CMP_LT_OQ);
    __mmask32 m32_22 = _mm512_cmp_ph_mask(p0, p0, 1);
    __m512i v512i_23 = _mm512_permutexvar_epi32(v512i_15, v512i_15);
    unsigned u32_24 = _cvtmask16_u32(m16_21);
    __mmask16 m16_25 = _mm512_test_epi32_mask(v512i_4, v512i_12);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m128i v128i_26 = _mm_xor_si128(v128i_10, v128i_10);
    __m128i v128i_27 = _mm_sub_epi16(v128i_9, v128i_26);
    __m512i v512i_28 = _mm512_permutex2var_epi32(v512i_12, v512i_11, v512i_11);
    __m128d v128d_29 = _mm_sub_pd(v128d_7, v128d_8);
    }
    __m512i v512i_30 = _mm512_sub_epi16(v512i_15, v512i_15);
    __m512i v512i_31 = _mm512_broadcast_i32x4(v128i_9);
    __m256i v256i_32 = _mm256_xor_si256(v256i_13, v256i_14);
    __m512 v512f_33 = _mm512_cvtxph_ps(p1);
    __m256d v256d_34 = _mm256_unpackhi_pd(v256d_19, v256d_20);
    __m256i v256i_35 = _mm256_sllv_epi32(v256i_32, v256i_13);
    __m128 v128f_36 = _mm_loadu_ps(fbuf);
    __m128 v128f_37 = _mm_min_ps(v128f_36, v128f_36);
    __m256d v256d_38 = _mm256_fmadd_pd(v256d_20, v256d_34, v256d_20);
    __m256 v256f_39 = _mm256_and_ps(v256f_2, v256f_2);
    __m128i v128i_40 = _mm_sub_epi16(v128i_10, v128i_10);
    __m128i v128i_41 = _mm512_cvtepi32_epi8(v512i_12);
    __m256d v256d_42 = _mm256_sqrt_pd(v256d_34);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m512 v512f_43 = _mm512_fnmadd_ps(v512f_33, v512f_33, v512f_16);
    __m128i v128i_44 = _mm_sub_epi32(v128i_10, v128i_40);
    }
    g_v256f[3] = v256f_39;
    g_v256d[10] = v256d_19;
    g_v128f[10] = v128f_37;
    g_v128d[1] = v128d_8;
    g_v128i[9] = v128i_9;
    g_v512h[4] = p0;
    return v128d_7;
}

__mmask32 torture_166(__m128h p0, __m256h p1, __m512d p2, __m128 p3) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_rcp14_ps(v512f_1);
    __m512 v512f_3 = _mm512_rcp14_ps(v512f_2);
    __m256 v256f_4 = _mm256_loadu_ps(fbuf);
    __m256 v256f_5 = _mm256_sub_ps(v256f_4, v256f_4);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_sub_epi64(v512i_6, v512i_6);
    __m256d v256d_8 = _mm512_extractf64x4_pd(p2, 1);
    __m256i v256i_9 = _mm512_cvtepi64_epi32(v512i_7);
    __m128d v128d_10 = _mm_set1_pd(2.0);
    __m128i v128i_11 = g_v128i[0];
    __m512i v512i_12 = _mm512_cvtepi8_epi32(v128i_11);
    __m512 v512f_13 = _mm512_add_ps(v512f_1, v512f_3);
    __m256 v256f_14 = _mm256_fmsub_ps(v256f_5, v256f_4, v256f_5);
    __mmask16 m16_15 = _mm512_cmp_epi32_mask(v512i_12, v512i_7, 2);
    __m512i v512i_16 = _mm512_sub_epi64(v512i_7, v512i_12);
    __m512h v512h_17 = g_v512h[4];
    __m512h v512h_18 = _mm512_fmul_pch(v512h_17, v512h_17);
    __mmask16 m16_19 = _kshiftri_mask16(m16_15, 1);
    __m128d v128d_20 = _mm_unpackhi_pd(v128d_10, v128d_10);
    __m512 v512f_21 = _mm512_fmadd_ps(v512f_1, v512f_2, v512f_3);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512 v512f_22 = _mm512_maskz_mul_ps(m16_19, v512f_21, v512f_2);
    __m128i v128i_23 = _mm_set1_epi32(0x55);
    }
    __m256d v256d_24 = _mm256_div_pd(v256d_8, v256d_8);
    __m256i v256i_25 = _mm256_unpacklo_epi32(v256i_9, v256i_9);
    __m512i v512i_26 = _mm512_mask_blend_epi32(m16_15, v512i_16, v512i_7);
    __mmask16 m16_27 = _mm512_cmp_epi32_mask(v512i_7, v512i_7, 2);
    __m512i v512i_28 = _mm512_lzcnt_epi32(v512i_12);
    __m512i v512i_29 = _mm512_mullo_epi32(v512i_12, v512i_12);
    __m128i v128i_30 = _mm_sub_epi16(v128i_11, v128i_11);
    __m512i v512i_31 = _mm512_dpbusd_epi32(v512i_12, v512i_12, v512i_12);
    __m128d v128d_32 = _mm_fmadd_pd(v128d_20, v128d_10, v128d_20);
    __mmask32 m32_33 = (__mmask32)(g_m32[9] ^ 142);
    __mmask32 m32_34 = _kxor_mask32(m32_33, m32_33);
    __m512i v512i_35 = _mm512_cvtph_epi16(v512h_17);
    g_v256f[10] = v256f_5;
    g_v256d[2] = v256d_8;
    g_v256i[5] = v256i_25;
    g_v128f[11] = p3;
    g_v512h[7] = v512h_18;
    g_v256h[0] = p1;
    g_v128h[7] = p0;
    return m32_33;
}

__mmask8 torture_167(__m256h p0, __m512 p1, __m512h p2, __m256h p3, __m512h p4) {
    __m512 v512f_1 = _mm512_fnmadd_ps(p1, p1, p1);
    __m512 v512f_2 = _mm512_sub_ps(v512f_1, p1);
    __m512h v512h_3 = _mm512_fmul_pch(p2, p2);
    __m128i v128i_4 = g_v128i[3];
    __m128i v128i_5 = _mm_and_si128(v128i_4, v128i_4);
    __m128 v128f_6 = _mm_loadu_ps(fbuf);
    __m128 v128f_7 = _mm_mul_ps(v128f_6, v128f_6);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_9 = _mm512_rol_epi32(v512i_8, 5);
    __m512d v512d_10 = _mm512_set1_pd(2.0);
    __m256i v256i_11 = g_v256i[8];
    __m256i v256i_12 = _mm256_unpacklo_epi32(v256i_11, v256i_11);
    __m512h v512h_13 = _mm512_fmul_pch(p2, p2);
    __m512 v512f_14 = _mm512_fmadd_ps(v512f_2, v512f_1, v512f_1);
    __m128i v128i_15 = _mm_add_epi32(v128i_4, v128i_5);
    __m512i v512i_16 = _mm512_add_epi64(v512i_8, v512i_8);
    __m256 v256f_17 = _mm256_set1_ps(1.0f);
    __m512 v512f_18 = _mm512_insertf32x8(v512f_14, v256f_17, 1);
    int s32_19 = 11;
    if ((s32_19) & 1ULL) {
    __m512 v512f_20 = _mm512_unpacklo_ps(v512f_14, p1);
    __mmask16 m16_21 = (__mmask16)(g_m16[8] ^ 50);
    __m512 v512f_22 = _mm512_mask_add_ps(v512f_14, m16_21, v512f_18, p1);
    __m512i v512i_23 = _mm512_mask_add_epi32(v512i_16, m16_21, v512i_8, v512i_9);
    }
    __m256d v256d_24 = _mm256_set1_pd(2.0);
    __m512 v512f_25 = _mm512_max_ps(v512f_18, v512f_18);
    __m256 v256f_26 = _mm512_cvtpd_ps(v512d_10);
    __m128i v128i_27 = _mm_sub_epi16(v128i_4, v128i_5);
    __m512d v512d_28 = _mm512_min_pd(v512d_10, v512d_10);
    __m512h v512h_29 = _mm512_sqrt_ph(v512h_3);
    __mmask32 m32_30 = _mm512_cmp_ph_mask(p2, v512h_3, 1);
    __mmask8 m8_31 = (__mmask8)(g_m8[11] ^ 130);
    __m512i v512i_32 = _mm512_broadcastmb_epi64(m8_31);
    __m512 v512f_33 = _mm512_shuffle_f32x4(v512f_1, v512f_14, 0x4E);
    if (((unsigned long long)m8_31) & 1ULL) {
    __m128 v128f_34 = _mm_div_ps(v128f_7, v128f_6);
    __m128 v128f_35 = _mm512_extractf32x4_ps(v512f_18, 2);
    }
    g_v512f[7] = v512f_1;
    g_v512d[6] = v512d_28;
    g_v512i[0] = v512i_32;
    g_v256d[4] = v256d_24;
    g_v256i[0] = v256i_11;
    g_v128i[5] = v128i_5;
    g_v512h[2] = v512h_29;
    return m8_31;
}

__m128d torture_168(__m512h p0, __m256d p1, __m256 p2, __m128 p3, __m256i p4) {
    __m256 v256f_1 = _mm256_mul_ps(p2, p2);
    __m128i v128i_2 = g_v128i[6];
    __m128i v128i_3 = _mm_xor_si128(v128i_2, v128i_2);
    __m512h v512h_4 = _mm512_fmadd_ph(p0, p0, p0);
    __m512d v512d_5 = _mm512_loadu_pd(dbuf);
    __m512d v512d_6 = _mm512_unpackhi_pd(v512d_5, v512d_5);
    __m512i v512i_7 = _mm512_set1_epi32(0x55);
    __m512i v512i_8 = _mm512_sub_epi8(v512i_7, v512i_7);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m512 v512f_10 = _mm512_mul_ps(v512f_9, v512f_9);
    __m512 v512f_11 = _mm512_add_ps(v512f_9, v512f_10);
    __m512d v512d_12 = _mm512_sub_pd(v512d_5, v512d_6);
    __m128i v128i_13 = _mm_xor_si128(v128i_2, v128i_3);
    __m256d v256d_14 = _mm256_sub_pd(p1, p1);
    __m512d v512d_15 = _mm512_set1_pd(2.0);
    __m512 v512f_16 = _mm512_fmadd_ps(v512f_9, v512f_10, v512f_9);
    __m256 v256f_17 = _mm256_add_ps(p2, v256f_1);
    __m128 v128f_18 = _mm_shuffle_ps(p3, p3, 0x1B);
    __m512i v512i_19 = _mm512_permutexvar_epi32(v512i_8, v512i_7);
    __m512i v512i_20 = _mm512_sub_epi16(v512i_8, v512i_7);
    __m512i v512i_21 = _mm512_madd52lo_epu64(v512i_8, v512i_19, v512i_20);
    int s32_22 = 64;
    if ((s32_22) & 1ULL) {
    __m512i v512i_23 = _mm512_unpacklo_epi32(v512i_20, v512i_20);
    __m256 v256f_24 = _mm256_div_ps(v256f_1, p2);
    __m256d v256d_25 = _mm256_max_pd(v256d_14, p1);
    __m128 v128f_26 = _mm_fnmadd_ps(p3, p3, p3);
    __m512d v512d_27 = _mm512_sqrt_pd(v512d_6);
    __m128 v128f_28 = _mm_xor_ps(v128f_26, p3);
    }
    __m512 v512f_29 = _mm512_fmsub_ps(v512f_10, v512f_10, v512f_9);
    __mmask16 m16_30 = (__mmask16)(g_m16[2] ^ 6);
    __m512 v512f_31 = _mm512_maskz_mul_ps(m16_30, v512f_16, v512f_9);
    __m512i v512i_32 = _mm512_ternarylogic_epi32(v512i_7, v512i_7, v512i_7, 0x96);
    __m512 v512f_33 = _mm512_broadcast_f32x4(v128f_18);
    __m512i v512i_34 = _mm512_or_si512(v512i_20, v512i_8);
    __m128i v128i_35 = _mm_unpacklo_epi32(v128i_3, v128i_13);
    __m256 v256f_36 = _mm256_set1_ps(1.0f);
    __m128 v128f_37 = _mm_div_ps(p3, p3);
    __m512i v512i_38 = _mm512_conflict_epi32(v512i_34);
    __m512 v512f_39 = _mm512_unpacklo_ps(v512f_16, v512f_31);
    __m256i v256i_40 = _mm256_shuffle_epi8(p4, p4);
    g_v512i[0] = v512i_21;
    __m128d v128d_41 = g_v128d[8];
    return v128d_41;
}

__mmask32 torture_169(__m512h p0, __m512 p1, __m256 p2, __m128d p3, __m128h p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_and_si512(v512i_1, v512i_1);
    __m256i v256i_3 = g_v256i[5];
    __m256i v256i_4 = _mm256_add_epi64(v256i_3, v256i_3);
    __m512d v512d_5 = g_v512d[1];
    __m512d v512d_6 = _mm512_fmadd_pd(v512d_5, v512d_5, v512d_5);
    __m512 v512f_7 = _mm512_shuffle_ps(p1, p1, 0x1B);
    __m512 v512f_8 = _mm512_set1_ps(1.0f);
    __m512h v512h_9 = _mm512_mul_ph(p0, p0);
    __m512i v512i_10 = _mm512_sub_epi16(v512i_1, v512i_2);
    __mmask16 m16_11 = (__mmask16)(g_m16[6] ^ 98);
    __m512 v512f_12 = _mm512_mask_add_ps(p1, m16_11, v512f_7, p1);
    __m512h v512h_13 = _mm512_fmul_pch(p0, p0);
    __m128h v128h_14 = _mm512_cvtpd_ph(v512d_6);
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_16 = _mm_or_si128(v128i_15, v128i_15);
    __m256d v256d_17 = g_v256d[8];
    __m256d v256d_18 = _mm256_mul_pd(v256d_17, v256d_17);
    __m512d v512d_19 = _mm512_fmadd_pd(v512d_6, v512d_5, v512d_5);
    __m128d v128d_20 = _mm_max_pd(p3, p3);
    __m512 v512f_21 = _mm512_unpacklo_ps(p1, v512f_7);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_11) & 7); i_22++) {
    __m512i v512i_23 = _mm512_add_epi8(v512i_1, v512i_10);
    __m512i v512i_24 = _mm512_add_epi64(v512i_23, v512i_10);
    __m128d v128d_25 = _mm_unpackhi_pd(v128d_20, v128d_20);
    __m256i v256i_26 = _mm256_add_epi16(v256i_3, v256i_3);
    }
    __m128i v128i_27 = _mm_sub_epi8(v128i_16, v128i_15);
    __m512i v512i_28 = _mm512_dpbusd_epi32(v512i_10, v512i_10, v512i_10);
    __m128i v128i_29 = _mm_add_epi8(v128i_16, v128i_27);
    __m256i v256i_30 = _mm256_mullo_epi32(v256i_4, v256i_3);
    __m512h v512h_31 = _mm512_add_ph(p0, v512h_9);
    g_v512f[4] = p1;
    g_v512d[4] = v512d_5;
    g_v256d[3] = v256d_17;
    g_v256i[3] = v256i_3;
    g_v128d[0] = p3;
    g_v512h[0] = p0;
    g_v128h[5] = v128h_14;
    __mmask32 m32_32 = (__mmask32)(g_m32[3] ^ 233);
    return m32_32;
}

__mmask32 torture_170(__m512 p0, __m512 p1) {
    __m128d v128d_1 = g_v128d[11];
    __m128d v128d_2 = _mm_min_pd(v128d_1, v128d_1);
    __m128i v128i_3 = g_v128i[10];
    __m128i v128i_4 = _mm_abs_epi32(v128i_3);
    __m512d v512d_5 = g_v512d[7];
    __m512i v512i_6 = _mm512_cvtpd_epi64(v512d_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[8] ^ 31);
    __m512 v512f_8 = _mm512_mask3_fmadd_ps(p1, p1, p0, m16_7);
    __m128i v128i_9 = _mm_add_epi8(v128i_3, v128i_3);
    __m512i v512i_10 = _mm512_dpbusd_epi32(v512i_6, v512i_6, v512i_6);
    __m256 v256f_11 = g_v256f[1];
    __m256 v256f_12 = _mm256_fmsub_ps(v256f_11, v256f_11, v256f_11);
    __m512i v512i_13 = _mm512_rol_epi32(v512i_10, 5);
    __m256d v256d_14 = g_v256d[6];
    __m256d v256d_15 = _mm256_unpackhi_pd(v256d_14, v256d_14);
    __mmask16 m16_16 = _mm512_test_epi32_mask(v512i_6, v512i_6);
    __m128 v128f_17 = g_v128f[0];
    __m128 v128f_18 = _mm_min_ps(v128f_17, v128f_17);
    __m128 v128f_19 = _mm_max_ps(v128f_17, v128f_18);
    __m128i v128i_20 = _mm_abs_epi32(v128i_9);
    __m512d v512d_21 = _mm512_mul_pd(v512d_5, v512d_5);
    __m512 v512f_22 = _mm512_div_ps(v512f_8, v512f_8);
    __m512 v512f_23 = _mm512_getexp_ps(v512f_22);
    __m512i v512i_24 = _mm512_cvtepi8_epi32(v128i_9);
    __m512i v512i_25 = _mm512_broadcastmw_epi32(m16_7);
    __m256h v256h_26 = g_v256h[3];
    __m512 v512f_27 = _mm512_cvtxph_ps(v256h_26);
    __m512i v512i_28 = _mm512_permutex2var_epi32(v512i_24, v512i_6, v512i_6);
    __m256i v256i_29 = g_v256i[10];
    __m256i v256i_30 = _mm256_abs_epi32(v256i_29);
    __m128i v128i_31 = _mm_unpacklo_epi32(v128i_20, v128i_4);
    __m128d v128d_32 = _mm_sqrt_pd(v128d_1);
    __m256 v256f_33 = _mm256_xor_ps(v256f_12, v256f_12);
    __m512i v512i_34 = _mm512_set1_epi32(0x55);
    __m256i v256i_35 = _mm256_unpacklo_epi32(v256i_29, v256i_30);
    __m512 v512f_36 = _mm512_cvtxph_ps(v256h_26);
    for (int i_37 = 0; i_37 < (int)(((unsigned long long)m16_7) & 7); i_37++) {
    __m512 v512f_38 = _mm512_and_ps(p0, v512f_23);
    __m512i v512i_39 = _mm512_sub_epi64(v512i_24, v512i_24);
    }
    g_v512d[1] = v512d_5;
    g_v128f[6] = v128f_19;
    g_v256h[5] = v256h_26;
    g_m16[8] = m16_16;
    __mmask32 m32_40 = (__mmask32)(g_m32[7] ^ 187);
    return m32_40;
}

__m512h torture_171(__m512h p0, __m512d p1) {
    __mmask32 m32_1 = (__mmask32)(g_m32[10] ^ 242);
    __mmask32 m32_2 = _kxor_mask32(m32_1, m32_1);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_div_ps(v128f_3, v128f_3);
    __mmask8 m8_5 = (__mmask8)(g_m8[2] ^ 167);
    __mmask16 m16_6 = _mm512_kunpackb(m8_5, m8_5);
    __m256 v256f_7 = g_v256f[1];
    __m256 v256f_8 = _mm256_add_ps(v256f_7, v256f_7);
    __m512 v512f_9 = g_v512f[3];
    __m512 v512f_10 = _mm512_sub_ps(v512f_9, v512f_9);
    __m512i v512i_11 = g_v512i[2];
    __m512i v512i_12 = _mm512_conflict_epi32(v512i_11);
    __m128i v128i_13 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_14 = _mm_add_epi64(v128i_13, v128i_13);
    __m128 v128f_15 = _mm_fmadd_ps(v128f_4, v128f_4, v128f_4);
    __m512 v512f_16 = _mm512_rsqrt14_ps(v512f_9);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m256i v256i_17 = _mm256_set1_epi32(0x55);
    __m128 v128f_18 = _mm_and_ps(v128f_4, v128f_4);
    }
    __m256 v256f_19 = _mm512_cvtpd_ps(p1);
    __mmask16 m16_20 = _mm512_cmp_ps_mask(v512f_10, v512f_16, _CMP_LT_OQ);
    __mmask16 m16_21 = _mm512_movepi32_mask(v512i_11);
    __m256 v256f_22 = _mm256_set1_ps(1.0f);
    __m512d v512d_23 = _mm512_cvtps_pd(v256f_7);
    __m128i v128i_24 = _mm_sub_epi64(v128i_14, v128i_13);
    __m512i v512i_25 = _mm512_rol_epi32(v512i_11, 5);
    __m256i v256i_26 = g_v256i[9];
    __m256i v256i_27 = _mm256_and_si256(v256i_26, v256i_26);
    __m256h v256h_28 = _mm512_cvtxps_ph(v512f_10);
    for (int i_29 = 0; i_29 < (int)(((unsigned long long)m16_21) & 7); i_29++) {
    __m128i v128i_30 = _mm_add_epi32(v128i_14, v128i_13);
    __m512i v512i_31 = _mm512_permutexvar_epi32(v512i_25, v512i_12);
    __m256 v256f_32 = _mm256_sqrt_ps(v256f_22);
    }
    g_v512i[9] = v512i_11;
    g_v256f[6] = v256f_7;
    g_v128i[1] = v128i_14;
    g_m8[1] = m8_5;
    return p0;
}

__m512i torture_172(__m256 p0, __m128h p1, __m256d p2) {
    __m512i v512i_1 = g_v512i[2];
    __m512i v512i_2 = _mm512_add_epi32(v512i_1, v512i_1);
    __m512i v512i_3 = _mm512_permutex2var_epi32(v512i_1, v512i_1, v512i_1);
    __m128 v128f_4 = g_v128f[1];
    __m128 v128f_5 = _mm_xor_ps(v128f_4, v128f_4);
    __m512d v512d_6 = g_v512d[1];
    __m512i v512i_7 = _mm512_cvtpd_epi64(v512d_6);
    __m256 v256f_8 = _mm256_sqrt_ps(p0);
    __m256i v256i_9 = _mm512_cvtepi64_epi32(v512i_2);
    __m128i v128i_10 = g_v128i[4];
    __m128i v128i_11 = _mm_sllv_epi32(v128i_10, v128i_10);
    __mmask16 m16_12 = (__mmask16)(g_m16[5] ^ 89);
    __m512i v512i_13 = _mm512_mask_blend_epi32(m16_12, v512i_3, v512i_1);
    __mmask16 m16_14 = _mm512_cmp_epi32_mask(v512i_3, v512i_13, 2);
    __m256i v256i_15 = _mm256_sub_epi64(v256i_9, v256i_9);
    __mmask64 m64_16 = (__mmask64)(g_m64[4] ^ 199);
    __mmask64 m64_17 = _kor_mask64(m64_16, m64_16);
    __m512d v512d_18 = _mm512_sqrt_pd(v512d_6);
    __m128 v128f_19 = _mm_fnmadd_ps(v128f_5, v128f_4, v128f_5);
    __mmask16 m16_20 = _kor_mask16(m16_14, m16_12);
    __m256 v256f_21 = _mm256_mul_ps(p0, v256f_8);
    __m256d v256d_22 = _mm256_sqrt_pd(p2);
    if (((unsigned long long)m16_12) & 1ULL) {
    __mmask8 m8_23 = (__mmask8)(g_m8[0] ^ 50);
    __mmask16 m16_24 = _mm512_kunpackb(m8_23, m8_23);
    __m256h v256h_25 = g_v256h[5];
    __m512 v512f_26 = _mm512_cvtxph_ps(v256h_25);
    __m512h v512h_27 = g_v512h[8];
    __m512i v512i_28 = _mm512_cvtph_epi16(v512h_27);
    __m512 v512f_29 = _mm512_scalef_ps(v512f_26, v512f_26);
    __m512h v512h_30 = _mm512_cvtepi16_ph(v512i_28);
    }
    __m128i v128i_31 = _mm_sllv_epi32(v128i_10, v128i_11);
    __m512i v512i_32 = _mm512_permutex2var_epi32(v512i_3, v512i_2, v512i_1);
    __m512i v512i_33 = _mm512_conflict_epi32(v512i_1);
    __m512 v512f_34 = g_v512f[4];
    __m512 v512f_35 = _mm512_sqrt_ps(v512f_34);
    __mmask8 m8_36 = (__mmask8)(g_m8[10] ^ 102);
    __mmask8 m8_37 = _kand_mask8(m8_36, m8_36);
    g_v512d[6] = v512d_18;
    g_v512i[4] = v512i_13;
    g_v128h[8] = p1;
    g_m8[5] = m8_36;
    g_m64[8] = m64_17;
    return v512i_3;
}

__m256i torture_173(__m512 p0, __m512 p1, __m128 p2, __m512 p3, __m256d p4) {
    __m128d v128d_1 = g_v128d[7];
    __m128d v128d_2 = _mm_min_pd(v128d_1, v128d_1);
    __m128 v128f_3 = _mm_div_ps(p2, p2);
    __m512 v512f_4 = _mm512_fmadd_ps(p3, p3, p3);
    __m512 v512f_5 = _mm512_fnmadd_ps(p0, p1, p0);
    __m256i v256i_6 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_7 = _mm256_add_epi16(v256i_6, v256i_6);
    __mmask16 m16_8 = (__mmask16)(g_m16[10] ^ 243);
    unsigned u32_9 = _cvtmask16_u32(m16_8);
    __m512h v512h_10 = g_v512h[10];
    __m512h v512h_11 = _mm512_getexp_ph(v512h_10);
    __m128 v128f_12 = _mm512_extractf32x4_ps(v512f_4, 2);
    __m512 v512f_13 = _mm512_add_ps(v512f_5, p1);
    __m512i v512i_14 = _mm512_cvtps_epi32(v512f_4);
    __m512h v512h_15 = _mm512_sqrt_ph(v512h_11);
    __m256 v256f_16 = g_v256f[6];
    __m256 v256f_17 = _mm256_unpacklo_ps(v256f_16, v256f_16);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512d v512d_18 = _mm512_loadu_pd(dbuf);
    __m128h v128h_19 = _mm512_cvtpd_ph(v512d_18);
    __m512 v512f_20 = _mm512_scalef_ps(p3, p0);
    __m256 v256f_21 = _mm256_div_ps(v256f_17, v256f_16);
    __m256i v256i_22 = _mm256_unpacklo_epi32(v256i_7, v256i_6);
    __m256 v256f_23 = _mm256_max_ps(v256f_21, v256f_16);
    __m512i v512i_24 = _mm512_sllv_epi32(v512i_14, v512i_14);
    }
    __m512i v512i_25 = _mm512_lzcnt_epi32(v512i_14);
    __m256d v256d_26 = _mm256_unpackhi_pd(p4, p4);
    __m256i v256i_27 = _mm256_sllv_epi32(v256i_7, v256i_6);
    __m128d v128d_28 = _mm_sub_pd(v128d_1, v128d_2);
    __m256d v256d_29 = _mm256_fmadd_pd(v256d_26, p4, v256d_26);
    __mmask8 m8_30 = (__mmask8)(g_m8[9] ^ 163);
    __m512i v512i_31 = _mm512_broadcastmb_epi64(m8_30);
    __mmask16 m16_32 = _kxor_mask16(m16_8, m16_8);
    __m512i v512i_33 = _mm512_add_epi32(v512i_25, v512i_25);
    __m128 v128f_34 = _mm_min_ps(v128f_3, v128f_3);
    __m512 v512f_35 = _mm512_insertf32x8(p1, v256f_16, 1);
    __m512i v512i_36 = _mm512_or_si512(v512i_33, v512i_33);
    g_v512f[2] = v512f_35;
    g_v512i[3] = v512i_25;
    g_v256f[4] = v256f_17;
    g_v128d[8] = v128d_28;
    g_v512h[4] = v512h_15;
    g_m16[6] = m16_32;
    return v256i_6;
}

__m256 torture_174(__m256 p0, __m256i p1, __m512d p2, __m128i p3) {
    __m512i v512i_1 = g_v512i[4];
    __m512i v512i_2 = _mm512_add_epi16(v512i_1, v512i_1);
    __m512h v512h_3 = g_v512h[9];
    __m512h v512h_4 = _mm512_mul_ph(v512h_3, v512h_3);
    __mmask64 m64_5 = _mm512_cmp_epi8_mask(v512i_2, v512i_2, 1);
    __m512i v512i_6 = _mm512_lzcnt_epi32(v512i_2);
    __m512i v512i_7 = _mm512_cvtph_epi16(v512h_3);
    __m256i v256i_8 = _mm256_sub_epi32(p1, p1);
    __m128d v128d_9 = g_v128d[11];
    __m128d v128d_10 = _mm_add_pd(v128d_9, v128d_9);
    __m512i v512i_11 = _mm512_add_epi8(v512i_7, v512i_7);
    __m512i v512i_12 = _mm512_shuffle_epi8(v512i_1, v512i_6);
    __mmask16 m16_13 = (__mmask16)(g_m16[2] ^ 142);
    __mmask16 m16_14 = _kshiftli_mask16(m16_13, 2);
    __m256i v256i_15 = _mm256_slli_epi32(p1, 3);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_xor_ps(v128f_16, v128f_16);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_13) & 7); i_18++) {
    __m128i v128i_19 = _mm_add_epi64(p3, p3);
    __m512i v512i_20 = _mm512_ternarylogic_epi32(v512i_2, v512i_2, v512i_11, 0x96);
    }
    __m512 v512f_21 = _mm512_set1_ps(1.0f);
    __m256 v256f_22 = _mm256_add_ps(p0, p0);
    __m128d v128d_23 = _mm_sub_pd(v128d_9, v128d_9);
    __m512 v512f_24 = _mm512_rsqrt14_ps(v512f_21);
    __m256i v256i_25 = _mm256_xor_si256(v256i_15, v256i_8);
    __m512i v512i_26 = _mm512_mullo_epi32(v512i_7, v512i_12);
    unsigned u32_27 = 66;
    __mmask16 m16_28 = _cvtu32_mask16(u32_27);
    __mmask16 m16_29 = _mm512_testn_epi32_mask(v512i_12, v512i_11);
    __m512i v512i_30 = _mm512_madd52lo_epu64(v512i_7, v512i_26, v512i_11);
    __m512d v512d_31 = _mm512_insertf64x2(p2, v128d_23, 3);
    g_v256i[11] = v256i_15;
    return p0;
}

__mmask16 torture_175(__m256d p0, __m128 p1, __m128h p2) {
    __m128 v128f_1 = _mm_xor_ps(p1, p1);
    __m128 v128f_2 = _mm_mul_ps(v128f_1, v128f_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_sqrt_pd(v512d_3);
    __m512 v512f_5 = g_v512f[8];
    __mmask16 m16_6 = (__mmask16)(g_m16[3] ^ 65);
    __m512 v512f_7 = _mm512_mask3_fmadd_ps(v512f_5, v512f_5, v512f_5, m16_6);
    __m128 v128f_8 = _mm_and_ps(v128f_1, v128f_2);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_sub_epi16(v256i_9, v256i_9);
    __m128 v128f_11 = _mm_max_ps(v128f_2, v128f_2);
    __m256d v256d_12 = _mm256_min_pd(p0, p0);
    __m512i v512i_13 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_14 = _mm512_dpwssd_epi32(v512i_13, v512i_13, v512i_13);
    __m512 v512f_15 = _mm512_unpacklo_ps(v512f_5, v512f_5);
    __m512 v512f_16 = _mm512_shuffle_ps(v512f_15, v512f_15, 0x1B);
    __m512h v512h_17 = g_v512h[4];
    __m512h v512h_18 = _mm512_fmadd_ph(v512h_17, v512h_17, v512h_17);
    __m256i v256i_19 = _mm512_cvtepi64_epi32(v512i_13);
    __m256i v256i_20 = _mm256_sllv_epi32(v256i_10, v256i_10);
    __m512i v512i_21 = _mm512_unpacklo_epi32(v512i_13, v512i_14);
    __m256i v256i_22 = _mm256_set1_epi32(0x55);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_6) & 7); i_23++) {
    __m512d v512d_24 = _mm512_cvtph_pd(p2);
    __m512h v512h_25 = _mm512_fmadd_ph(v512h_18, v512h_17, v512h_17);
    __m256i v256i_26 = _mm256_sub_epi16(v256i_10, v256i_19);
    __m512h v512h_27 = _mm512_fmadd_ph(v512h_17, v512h_17, v512h_18);
    __m128 v128f_28 = _mm_shuffle_ps(v128f_8, v128f_11, 0x1B);
    }
    __m128 v128f_29 = _mm_fnmadd_ps(v128f_2, v128f_1, p1);
    __m512i v512i_30 = _mm512_sub_epi8(v512i_13, v512i_21);
    __m512 v512f_31 = _mm512_and_ps(v512f_5, v512f_15);
    __m512i v512i_32 = _mm512_add_epi16(v512i_30, v512i_30);
    __m512i v512i_33 = _mm512_mask_blend_epi32(m16_6, v512i_32, v512i_30);
    __m128 v128f_34 = _mm_div_ps(v128f_8, v128f_1);
    __m256 v256f_35 = _mm256_set1_ps(1.0f);
    __m512 v512f_36 = _mm512_mask_blend_ps(m16_6, v512f_31, v512f_7);
    __m512 v512f_37 = _mm512_div_ps(v512f_31, v512f_7);
    __m512 v512f_38 = _mm512_maskz_mul_ps(m16_6, v512f_36, v512f_37);
    __m256i v256i_39 = _mm256_sub_epi16(v256i_9, v256i_19);
    g_v512i[6] = v512i_30;
    g_v256d[1] = v256d_12;
    g_v256i[10] = v256i_19;
    g_v128f[8] = v128f_1;
    return m16_6;
}

__mmask16 torture_176(__m512d p0, __m512d p1, __m128 p2, __m256d p3, __m512 p4) {
    __m128 v128f_1 = _mm_sub_ps(p2, p2);
    __m512i v512i_2 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_3 = _mm512_conflict_epi32(v512i_2);
    __m512i v512i_4 = _mm512_ternarylogic_epi32(v512i_3, v512i_3, v512i_3, 0x96);
    __m512i v512i_5 = _mm512_sub_epi32(v512i_2, v512i_2);
    __m128 v128f_6 = _mm_shuffle_ps(p2, p2, 0x1B);
    __m128d v128d_7 = g_v128d[4];
    __m128d v128d_8 = _mm_mul_pd(v128d_7, v128d_7);
    __m512 v512f_9 = _mm512_min_ps(p4, p4);
    __m128h v128h_10 = g_v128h[4];
    __m512d v512d_11 = _mm512_cvtph_pd(v128h_10);
    __m512d v512d_12 = _mm512_set1_pd(2.0);
    __m128 v128f_13 = _mm_fmadd_ps(v128f_6, v128f_1, v128f_1);
    __m256 v256f_14 = g_v256f[10];
    __m256 v256f_15 = _mm256_max_ps(v256f_14, v256f_14);
    __m256 v256f_16 = _mm256_div_ps(v256f_15, v256f_14);
    __m512i v512i_17 = _mm512_rolv_epi32(v512i_4, v512i_4);
    unsigned u32_18 = 65;
    __mmask16 m16_19 = _cvtu32_mask16(u32_18);
    __m512i v512i_20 = _mm512_dpbusd_epi32(v512i_5, v512i_17, v512i_4);
    __m256d v256d_21 = _mm256_max_pd(p3, p3);
    __m512i v512i_22 = _mm512_conflict_epi32(v512i_5);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m128i v128i_23 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_24 = _mm_sub_epi8(v128i_23, v128i_23);
    __m512i v512i_25 = _mm512_madd52lo_epu64(v512i_5, v512i_22, v512i_22);
    __m256 v256f_26 = _mm256_mul_ps(v256f_14, v256f_16);
    __m512 v512f_27 = _mm512_max_ps(v512f_9, v512f_9);
    __mmask64 m64_28 = _mm512_cmp_epi8_mask(v512i_25, v512i_17, 1);
    __m256i v256i_29 = g_v256i[9];
    __m256i v256i_30 = _mm256_add_epi64(v256i_29, v256i_29);
    }
    __m512i v512i_31 = _mm512_popcnt_epi64(v512i_2);
    __m256i v256i_32 = _mm256_set1_epi32(0x55);
    __mmask32 m32_33 = (__mmask32)(g_m32[6] ^ 59);
    __mmask32 m32_34 = _kxor_mask32(m32_33, m32_33);
    __m512i v512i_35 = _mm512_abs_epi32(v512i_17);
    __m512 v512f_36 = _mm512_insertf32x8(v512f_9, v256f_16, 1);
    __m512 v512f_37 = _mm512_mask3_fmadd_ps(v512f_36, v512f_36, p4, m16_19);
    __mmask16 m16_38 = _knot_mask16(m16_19);
    __m512 v512f_39 = _mm512_mask3_fmadd_ps(v512f_37, v512f_9, v512f_37, m16_38);
    __m128d v128d_40 = _mm_div_pd(v128d_7, v128d_8);
    __m512i v512i_41 = _mm512_lzcnt_epi32(v512i_3);
    for (int i_42 = 0; i_42 < (int)(((unsigned long long)m16_38) & 7); i_42++) {
    __m512 v512f_43 = _mm512_mask3_fmadd_ps(v512f_9, p4, v512f_9, m16_19);
    __m512d v512d_44 = _mm512_mul_pd(v512d_11, p1);
    __m512d v512d_45 = _mm512_mul_pd(v512d_12, v512d_12);
    }
    g_v512f[3] = v512f_9;
    g_v512i[4] = v512i_31;
    g_v256f[5] = v256f_15;
    g_v128d[9] = v128d_7;
    return m16_38;
}

__m512h torture_177(__m512i p0, __m512h p1, __m256d p2, __m512i p3, __m512i p4) {
    __m128i v128i_1 = g_v128i[4];
    __m512i v512i_2 = _mm512_cvtepi8_epi32(v128i_1);
    __m128d v128d_3 = g_v128d[2];
    __m128d v128d_4 = _mm_sqrt_pd(v128d_3);
    __m512i v512i_5 = _mm512_cvtepi8_epi32(v128i_1);
    __m256i v256i_6 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_7 = _mm256_add_epi64(v256i_6, v256i_6);
    __m512 v512f_8 = _mm512_cvtepi32_ps(v512i_2);
    __m128i v128i_9 = _mm_sub_epi8(v128i_1, v128i_1);
    __m512 v512f_10 = _mm512_cvtepi32_ps(v512i_5);
    __m512i v512i_11 = _mm512_add_epi32(v512i_5, v512i_2);
    __m512i v512i_12 = _mm512_dpbusd_epi32(p0, v512i_11, v512i_2);
    __m128 v128f_13 = _mm_loadu_ps(fbuf);
    __m512 v512f_14 = _mm512_broadcast_f32x4(v128f_13);
    __mmask16 m16_15 = (__mmask16)(g_m16[8] ^ 35);
    __m512 v512f_16 = _mm512_mask_add_ps(v512f_10, m16_15, v512f_8, v512f_14);
    __m128i v128i_17 = _mm_add_epi16(v128i_9, v128i_1);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512 v512f_18 = _mm512_rsqrt14_ps(v512f_10);
    __m256i v256i_19 = _mm256_sllv_epi32(v256i_7, v256i_6);
    __m128 v128f_20 = _mm_unpacklo_ps(v128f_13, v128f_13);
    __mmask16 m16_21 = _mm512_movepi32_mask(p4);
    }
    __m512i v512i_22 = _mm512_set1_epi32(0x55);
    __m512 v512f_23 = _mm512_div_ps(v512f_16, v512f_16);
    __m512 v512f_24 = _mm512_broadcast_f32x4(v128f_13);
    __m128d v128d_25 = _mm_fmadd_pd(v128d_4, v128d_4, v128d_3);
    __m512h v512h_26 = _mm512_cvtepi16_ph(v512i_11);
    __m512i v512i_27 = _mm512_mask_add_epi32(v512i_11, m16_15, v512i_12, v512i_22);
    __mmask8 m8_28 = (__mmask8)(g_m8[8] ^ 52);
    __m512d v512d_29 = _mm512_loadu_pd(dbuf);
    __m512d v512d_30 = _mm512_mask_blend_pd(m8_28, v512d_29, v512d_29);
    __m256i v256i_31 = _mm256_add_epi8(v256i_6, v256i_6);
    __m256i v256i_32 = _mm256_sub_epi8(v256i_31, v256i_31);
    __m256i v256i_33 = _mm256_or_si256(v256i_6, v256i_6);
    __m256i v256i_34 = _mm256_add_epi8(v256i_33, v256i_6);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m128i v128i_35 = _mm_set1_epi32(0x55);
    __m128d v128d_36 = _mm_set1_pd(2.0);
    __mmask16 m16_37 = _mm512_movepi32_mask(v512i_11);
    __m512i v512i_38 = _mm512_permutex2var_epi32(p3, p4, v512i_5);
    }
    g_v512d[10] = v512d_30;
    g_v512i[1] = v512i_11;
    g_v128f[2] = v128f_13;
    g_m8[4] = m8_28;
    return p1;
}

__m512d torture_178(__m256h p0, __m512 p1, __m512h p2, __m512h p3, __m256h p4) {
    __m512i v512i_1 = g_v512i[9];
    __m512h v512h_2 = _mm512_cvtepi16_ph(v512i_1);
    __m128i v128i_3 = g_v128i[6];
    __m128i v128i_4 = _mm_slli_epi32(v128i_3, 3);
    __m256i v256i_5 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_6 = _mm256_and_si256(v256i_5, v256i_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[9] ^ 82);
    __m512i v512i_8 = _mm512_mask_blend_epi32(m16_7, v512i_1, v512i_1);
    __m512 v512f_9 = _mm512_rsqrt14_ps(p1);
    __m128i v128i_10 = _mm_sub_epi8(v128i_3, v128i_4);
    __mmask8 m8_11 = (__mmask8)(g_m8[8] ^ 191);
    __m512d v512d_12 = _mm512_loadu_pd(dbuf);
    __m512d v512d_13 = _mm512_mask_blend_pd(m8_11, v512d_12, v512d_12);
    __m256 v256f_14 = g_v256f[2];
    __m512 v512f_15 = _mm512_insertf32x8(p1, v256f_14, 1);
    __m128i v128i_16 = _mm_sub_epi8(v128i_4, v128i_10);
    __m128d v128d_17 = g_v128d[9];
    __m512d v512d_18 = _mm512_insertf64x2(v512d_13, v128d_17, 3);
    __m256 v256f_19 = _mm256_min_ps(v256f_14, v256f_14);
    __m256 v256f_20 = _mm512_cvtpd_ps(v512d_13);
    __m128 v128f_21 = _mm_loadu_ps(fbuf);
    __m128 v128f_22 = _mm_max_ps(v128f_21, v128f_21);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_7) & 7); i_23++) {
    __m512 v512f_24 = _mm512_roundscale_ps(v512f_15, 0x10);
    __m512i v512i_25 = _mm512_add_epi64(v512i_1, v512i_8);
    __m128i v128i_26 = _mm_add_epi32(v128i_4, v128i_10);
    __m512i v512i_27 = _mm512_broadcast_i32x4(v128i_10);
    }
    __m256 v256f_28 = _mm256_xor_ps(v256f_14, v256f_19);
    __m256 v256f_29 = _mm256_shuffle_ps(v256f_20, v256f_20, 0x1B);
    __m256 v256f_30 = _mm256_fmadd_ps(v256f_14, v256f_29, v256f_28);
    __m512h v512h_31 = _mm512_cvtepi16_ph(v512i_8);
    __m128d v128d_32 = _mm_unpackhi_pd(v128d_17, v128d_17);
    __m256 v256f_33 = _mm256_and_ps(v256f_14, v256f_28);
    __m512h v512h_34 = _mm512_fmul_pch(v512h_2, v512h_2);
    g_v128d[11] = v128d_32;
    g_v512h[8] = v512h_34;
    return v512d_18;
}

__m256h torture_179(__m256h p0, __m128d p1, __m256 p2) {
    __m512 v512f_1 = g_v512f[5];
    __m512 v512f_2 = _mm512_max_ps(v512f_1, v512f_1);
    __m256 v256f_3 = _mm256_fnmadd_ps(p2, p2, p2);
    __m256i v256i_4 = g_v256i[6];
    __m256i v256i_5 = _mm256_or_si256(v256i_4, v256i_4);
    __m128d v128d_6 = _mm_unpackhi_pd(p1, p1);
    __m256d v256d_7 = g_v256d[0];
    __m256d v256d_8 = _mm256_mul_pd(v256d_7, v256d_7);
    __m512i v512i_9 = g_v512i[3];
    __m512h v512h_10 = _mm512_cvtepi16_ph(v512i_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_min_pd(v512d_11, v512d_11);
    __mmask16 m16_13 = (__mmask16)(g_m16[3] ^ 73);
    __m512 v512f_14 = _mm512_mask_add_ps(v512f_2, m16_13, v512f_1, v512f_1);
    __m128d v128d_15 = _mm_add_pd(p1, p1);
    __m256 v256f_16 = _mm256_xor_ps(v256f_3, v256f_3);
    __mmask8 m8_17 = _mm512_cmp_pd_mask(v512d_11, v512d_12, _CMP_GE_OQ);
    __m128i v128i_18 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_19 = _mm_sub_epi64(v128i_18, v128i_18);
    if (((unsigned long long)m16_13) & 1ULL) {
    __m128 v128f_20 = _mm_loadu_ps(fbuf);
    __m128 v128f_21 = _mm_fnmadd_ps(v128f_20, v128f_20, v128f_20);
    __m512i v512i_22 = _mm512_mullo_epi32(v512i_9, v512i_9);
    __m128 v128f_23 = _mm_sqrt_ps(v128f_21);
    __m256i v256i_24 = _mm256_add_epi64(v256i_5, v256i_5);
    __m512i v512i_25 = _mm512_broadcast_i32x4(v128i_18);
    __m128h v128h_26 = _mm512_cvtpd_ph(v512d_11);
    }
    __m512i v512i_27 = _mm512_add_epi8(v512i_9, v512i_9);
    __m256 v256f_28 = _mm256_xor_ps(v256f_16, v256f_16);
    __m512i v512i_29 = _mm512_and_si512(v512i_9, v512i_27);
    __m128 v128f_30 = _mm_loadu_ps(fbuf);
    __m128 v128f_31 = _mm_add_ps(v128f_30, v128f_30);
    __m512i v512i_32 = _mm512_popcnt_epi64(v512i_9);
    __m512i v512i_33 = _mm512_madd52lo_epu64(v512i_27, v512i_32, v512i_27);
    __m512 v512f_34 = _mm512_maskz_mul_ps(m16_13, v512f_2, v512f_14);
    __m128d v128d_35 = _mm_div_pd(p1, v128d_15);
    __m512i v512i_36 = _mm512_dpbusd_epi32(v512i_27, v512i_32, v512i_9);
    __m128i v128i_37 = _mm_set1_epi32(0x55);
    __m256 v256f_38 = _mm256_fmsub_ps(v256f_16, v256f_16, p2);
    if (((unsigned long long)m16_13) & 1ULL) {
    __m512 v512f_39 = _mm512_mask_add_ps(v512f_14, m16_13, v512f_14, v512f_2);
    __mmask16 m16_40 = _mm512_cmp_epu32_mask(v512i_9, v512i_32, 4);
    __m256i v256i_41 = _mm256_or_si256(v256i_5, v256i_4);
    __m256 v256f_42 = _mm256_shuffle_ps(p2, p2, 0x1B);
    __m128i v128i_43 = _mm_slli_epi32(v128i_18, 3);
    __m256i v256i_44 = _mm256_slli_epi32(v256i_4, 3);
    }
    g_v128f[8] = v128f_31;
    g_v512h[8] = v512h_10;
    g_v256h[1] = p0;
    g_m8[5] = m8_17;
    g_m16[6] = m16_13;
    return p0;
}

__m256 torture_180(__m512 p0, __m256 p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_sub_epi8(v512i_1, v512i_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_max_pd(v512d_3, v512d_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_mullo_epi32(v128i_5, v128i_5);
    __mmask8 m8_7 = (__mmask8)(g_m8[3] ^ 248);
    __mmask16 m16_8 = _mm512_kunpackb(m8_7, m8_7);
    __m128 v128f_9 = _mm_loadu_ps(fbuf);
    __m128 v128f_10 = _mm_max_ps(v128f_9, v128f_9);
    __m512i v512i_11 = _mm512_xor_si512(v512i_2, v512i_1);
    __m128i v128i_12 = _mm_add_epi32(v128i_5, v128i_5);
    __m512 v512f_13 = _mm512_set1_ps(1.0f);
    __m512 v512f_14 = _mm512_mask3_fmadd_ps(p0, p0, v512f_13, m16_8);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_8) & 7); i_15++) {
    __m512i v512i_16 = _mm512_cvtps_epi32(v512f_14);
    __mmask8 m8_17 = _mm512_cmp_pd_mask(v512d_3, v512d_3, _CMP_GE_OQ);
    __m512 v512f_18 = _mm512_sub_ps(v512f_13, v512f_14);
    __m256 v256f_19 = _mm256_div_ps(p1, p1);
    __m512 v512f_20 = _mm512_roundscale_ps(v512f_14, 0x10);
    }
    __m128i v128i_21 = _mm_sub_epi64(v128i_5, v128i_5);
    __m128 v128f_22 = _mm_shuffle_ps(v128f_10, v128f_9, 0x1B);
    __m512h v512h_23 = g_v512h[1];
    __m512h v512h_24 = _mm512_getexp_ph(v512h_23);
    __m256i v256i_25 = g_v256i[11];
    __m256i v256i_26 = _mm256_sub_epi8(v256i_25, v256i_25);
    __m256d v256d_27 = g_v256d[5];
    __m256d v256d_28 = _mm256_sub_pd(v256d_27, v256d_27);
    __m128i v128i_29 = _mm_unpacklo_epi32(v128i_12, v128i_5);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m256i v256i_30 = _mm256_add_epi64(v256i_26, v256i_26);
    __m512i v512i_31 = _mm512_add_epi8(v512i_11, v512i_1);
    __m512i v512i_32 = _mm512_cvtps_epi32(v512f_14);
    __m512h v512h_33 = _mm512_fmsub_ph(v512h_24, v512h_24, v512h_24);
    __m256i v256i_34 = _mm512_cvtepi64_epi32(v512i_32);
    }
    g_v512d[0] = v512d_3;
    g_v256i[2] = v256i_25;
    g_v128f[2] = v128f_10;
    g_v128i[9] = v128i_5;
    g_v512h[4] = v512h_24;
    g_m8[10] = m8_7;
    return p1;
}

__m256 torture_181(__m128i p0, __m512i p1) {
    __mmask16 m16_1 = (__mmask16)(g_m16[8] ^ 213);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_maskz_mul_ps(m16_1, v512f_2, v512f_2);
    __mmask64 m64_4 = _mm512_cmp_epi8_mask(p1, p1, 1);
    __m512 v512f_5 = _mm512_shuffle_f32x4(v512f_2, v512f_2, 0x4E);
    __m512 v512f_6 = _mm512_add_ps(v512f_5, v512f_2);
    __m128i v128i_7 = _mm_add_epi32(p0, p0);
    __m256i v256i_8 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_9 = _mm256_slli_epi32(v256i_8, 3);
    __mmask32 m32_10 = (__mmask32)(g_m32[5] ^ 1);
    __mmask32 m32_11 = _kxor_mask32(m32_10, m32_10);
    __m512 v512f_12 = _mm512_getexp_ps(v512f_5);
    __m128 v128f_13 = _mm_loadu_ps(fbuf);
    __m128 v128f_14 = _mm_max_ps(v128f_13, v128f_13);
    __m256h v256h_15 = _mm512_cvtxps_ph(v512f_6);
    __m256d v256d_16 = g_v256d[5];
    __m256d v256d_17 = _mm256_max_pd(v256d_16, v256d_16);
    __m128 v128f_18 = _mm_mul_ps(v128f_14, v128f_14);
    __m512d v512d_19 = g_v512d[7];
    __m512i v512i_20 = _mm512_cvtpd_epi64(v512d_19);
    __m512h v512h_21 = g_v512h[5];
    __m512h v512h_22 = _mm512_fmul_pch(v512h_21, v512h_21);
    __m128 v128f_23 = _mm_max_ps(v128f_14, v128f_14);
    __m512i v512i_24 = _mm512_dpbusd_epi32(v512i_20, v512i_20, p1);
    __m256 v256f_25 = _mm256_loadu_ps(fbuf);
    __m256 v256f_26 = _mm256_and_ps(v256f_25, v256f_25);
    __m128i v128i_27 = _mm_add_epi64(p0, p0);
    __m256d v256d_28 = _mm256_add_pd(v256d_16, v256d_17);
    __m256d v256d_29 = _mm256_max_pd(v256d_28, v256d_28);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m512 v512f_30 = _mm512_xor_ps(v512f_12, v512f_6);
    __mmask16 m16_31 = _knot_mask16(m16_1);
    }
    g_v256f[11] = v256f_26;
    g_v256d[4] = v256d_29;
    g_v256i[1] = v256i_9;
    g_v128f[8] = v128f_13;
    g_v128i[4] = p0;
    g_v512h[8] = v512h_22;
    return v256f_26;
}

__m128d torture_182(__m128i p0, __m256i p1, __m256 p2, __m256i p3) {
    __mmask64 m64_1 = (__mmask64)(g_m64[8] ^ 135);
    __mmask64 m64_2 = _kor_mask64(m64_1, m64_1);
    __m512i v512i_3 = g_v512i[0];
    __m512i v512i_4 = _mm512_rolv_epi32(v512i_3, v512i_3);
    __mmask64 m64_5 = _mm512_cmp_epi8_mask(v512i_4, v512i_3, 1);
    __m512i v512i_6 = _mm512_dpwssd_epi32(v512i_4, v512i_3, v512i_4);
    __m128i v128i_7 = _mm_sub_epi8(p0, p0);
    __mmask16 m16_8 = _mm512_movepi32_mask(v512i_3);
    __m128i v128i_9 = _mm_add_epi64(p0, v128i_7);
    __m128 v128f_10 = _mm_set1_ps(1.0f);
    __m256d v256d_11 = g_v256d[4];
    __m256d v256d_12 = _mm256_div_pd(v256d_11, v256d_11);
    __m128 v128f_13 = _mm_min_ps(v128f_10, v128f_10);
    __mmask16 m16_14 = _mm512_cmp_epi32_mask(v512i_3, v512i_3, 2);
    __m512h v512h_15 = g_v512h[9];
    __m512h v512h_16 = _mm512_mul_ph(v512h_15, v512h_15);
    __m512h v512h_17 = _mm512_cvtepi16_ph(v512i_6);
    __m128d v128d_18 = g_v128d[5];
    __m128d v128d_19 = _mm_fmadd_pd(v128d_18, v128d_18, v128d_18);
    __m512i v512i_20 = _mm512_mask_blend_epi32(m16_8, v512i_3, v512i_4);
    __m256i v256i_21 = _mm256_xor_si256(p3, p1);
    __m128i v128i_22 = _mm_abs_epi32(p0);
    __m512i v512i_23 = _mm512_and_si512(v512i_3, v512i_6);
    __m512 v512f_24 = _mm512_loadu_ps(fbuf);
    __m512 v512f_25 = _mm512_mask_add_ps(v512f_24, m16_8, v512f_24, v512f_24);
    __m128i v128i_26 = _mm_and_si128(p0, p0);
    __m512 v512f_27 = _mm512_min_ps(v512f_24, v512f_25);
    __m512i v512i_28 = _mm512_and_si512(v512i_3, v512i_6);
    for (int i_29 = 0; i_29 < (int)(((unsigned long long)m16_8) & 7); i_29++) {
    __m512 v512f_30 = _mm512_mask_blend_ps(m16_14, v512f_24, v512f_25);
    __m128i v128i_31 = _mm_or_si128(v128i_22, v128i_26);
    __m512h v512h_32 = _mm512_cvtepi16_ph(v512i_3);
    }
    g_v512f[4] = v512f_24;
    g_v512i[8] = v512i_20;
    g_v256d[11] = v256d_11;
    g_v128f[7] = v128f_10;
    g_v128i[7] = v128i_26;
    g_m16[9] = m16_14;
    return v128d_19;
}

__m128i torture_183(__m256d p0, __m256d p1, __m512i p2) {
    __m512i v512i_1 = _mm512_permutexvar_epi32(p2, p2);
    __m256i v256i_2 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_3 = _mm256_sllv_epi32(v256i_2, v256i_2);
    __m256 v256f_4 = _mm256_loadu_ps(fbuf);
    __m256 v256f_5 = _mm256_and_ps(v256f_4, v256f_4);
    __m512d v512d_6 = _mm512_loadu_pd(dbuf);
    __m256d v256d_7 = _mm512_extractf64x4_pd(v512d_6, 1);
    __m128 v128f_8 = g_v128f[9];
    __m128 v128f_9 = _mm_fmsub_ps(v128f_8, v128f_8, v128f_8);
    __m512 v512f_10 = g_v512f[8];
    __m512 v512f_11 = _mm512_sqrt_ps(v512f_10);
    __m256d v256d_12 = _mm256_set1_pd(2.0);
    __m256d v256d_13 = _mm256_set1_pd(2.0);
    __m512d v512d_14 = _mm512_unpackhi_pd(v512d_6, v512d_6);
    __m512i v512i_15 = _mm512_sub_epi8(p2, v512i_1);
    __mmask16 m16_16 = (__mmask16)(g_m16[9] ^ 15);
    __mmask16 m16_17 = _kor_mask16(m16_16, m16_16);
    __m512i v512i_18 = _mm512_add_epi8(p2, p2);
    __m256d v256d_19 = _mm256_sub_pd(p0, v256d_7);
    __m512h v512h_20 = g_v512h[0];
    __m512i v512i_21 = _mm512_cvtph_epi16(v512h_20);
    __m128i v128i_22 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_23 = _mm_add_epi16(v128i_22, v128i_22);
    __m512i v512i_24 = _mm512_broadcastmw_epi32(m16_16);
    __m256 v256f_25 = _mm256_max_ps(v256f_5, v256f_4);
    __m128i v128i_26 = _mm_unpacklo_epi32(v128i_22, v128i_23);
    __m128 v128f_27 = _mm_max_ps(v128f_9, v128f_8);
    __m256d v256d_28 = _mm256_unpackhi_pd(v256d_19, p0);
    __m512 v512f_29 = _mm512_shuffle_f32x4(v512f_11, v512f_10, 0x4E);
    __mmask16 m16_30 = _mm512_test_epi32_mask(v512i_15, p2);
    __m512i v512i_31 = _mm512_add_epi64(v512i_15, v512i_1);
    __m256d v256d_32 = _mm256_add_pd(v256d_7, p1);
    __m512i v512i_33 = _mm512_cvtepi8_epi32(v128i_23);
    __m256 v256f_34 = _mm256_add_ps(v256f_5, v256f_4);
    __m512i v512i_35 = _mm512_shuffle_i32x4(v512i_24, v512i_24, 0x1B);
    __m256 v256f_36 = _mm256_div_ps(v256f_34, v256f_5);
    for (int i_37 = 0; i_37 < (int)(((unsigned long long)m16_17) & 7); i_37++) {
    __m256d v256d_38 = _mm256_sub_pd(v256d_12, v256d_13);
    __mmask16 m16_39 = _knot_mask16(m16_16);
    __m256i v256i_40 = _mm256_unpacklo_epi32(v256i_3, v256i_2);
    __m512 v512f_41 = _mm512_sub_ps(v512f_10, v512f_11);
    __m256i v256i_42 = _mm256_add_epi16(v256i_2, v256i_3);
    }
    g_v512f[11] = v512f_10;
    g_v512i[4] = v512i_21;
    g_v256f[7] = v256f_25;
    g_v256d[10] = p1;
    g_v512h[0] = v512h_20;
    return v128i_26;
}

__m256h torture_184(__m128 p0, __m256 p1, __m256i p2, __m128d p3, __m128d p4) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_add_ps(v512f_1, v512f_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_conflict_epi32(v512i_3);
    __m256 v256f_5 = _mm256_mul_ps(p1, p1);
    __m512d v512d_6 = _mm512_cvtps_pd(p1);
    __m512i v512i_7 = _mm512_add_epi8(v512i_4, v512i_4);
    __m512i v512i_8 = _mm512_set1_epi32(0x55);
    __m256d v256d_9 = g_v256d[0];
    __m256d v256d_10 = _mm256_sqrt_pd(v256d_9);
    __m512d v512d_11 = _mm512_fmadd_pd(v512d_6, v512d_6, v512d_6);
    __m512 v512f_12 = _mm512_max_ps(v512f_1, v512f_2);
    __m128 v128f_13 = _mm_add_ps(p0, p0);
    __m512i v512i_14 = _mm512_sub_epi32(v512i_3, v512i_4);
    __m512i v512i_15 = _mm512_or_si512(v512i_4, v512i_7);
    __m512 v512f_16 = _mm512_scalef_ps(v512f_2, v512f_2);
    __mmask16 m16_17 = (__mmask16)(g_m16[9] ^ 187);
    __mmask16 m16_18 = _kor_mask16(m16_17, m16_17);
    __m512d v512d_19 = _mm512_add_pd(v512d_11, v512d_11);
    __m256i v256i_20 = _mm256_sllv_epi32(p2, p2);
    __m256i v256i_21 = _mm256_add_epi32(p2, v256i_20);
    __m256i v256i_22 = _mm256_sub_epi16(v256i_20, p2);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_18) & 7); i_23++) {
    __m512i v512i_24 = _mm512_set1_epi32(0x55);
    __m128i v128i_25 = g_v128i[5];
    __m128i v128i_26 = _mm_shuffle_epi8(v128i_25, v128i_25);
    __m512 v512f_27 = _mm512_cvtepi32_ps(v512i_4);
    __m256i v256i_28 = _mm256_shuffle_epi8(v256i_21, v256i_21);
    __m128 v128f_29 = _mm_add_ps(v128f_13, p0);
    }
    __mmask64 m64_30 = _mm512_cmp_epi8_mask(v512i_4, v512i_14, 1);
    __m512i v512i_31 = _mm512_cvtps_epi32(v512f_1);
    __mmask16 m16_32 = _kand_mask16(m16_17, m16_18);
    __m512i v512i_33 = _mm512_permutexvar_epi32(v512i_7, v512i_14);
    __m512 v512f_34 = _mm512_scalef_ps(v512f_12, v512f_16);
    __m256d v256d_35 = _mm256_add_pd(v256d_9, v256d_9);
    if (((unsigned long long)m16_18) & 1ULL) {
    __mmask16 m16_36 = _mm512_testn_epi32_mask(v512i_33, v512i_33);
    __m512i v512i_37 = _mm512_rol_epi32(v512i_33, 5);
    }
    g_v512f[6] = v512f_34;
    g_v256f[9] = p1;
    g_v256i[9] = v256i_20;
    g_v128f[2] = v128f_13;
    g_v128d[2] = p3;
    g_m64[6] = m64_30;
    __m256h v256h_38 = g_v256h[4];
    return v256h_38;
}

__m256 torture_185(__m256h p0, __m256h p1, __m256d p2, __m128d p3, __m256d p4) {
    __m256d v256d_1 = _mm256_div_pd(p2, p2);
    __m512 v512f_2 = g_v512f[0];
    __m512i v512i_3 = _mm512_cvtps_epi32(v512f_2);
    __m256 v256f_4 = g_v256f[9];
    __m512d v512d_5 = _mm512_cvtps_pd(v256f_4);
    __m512i v512i_6 = _mm512_mullo_epi32(v512i_3, v512i_3);
    __m128i v128i_7 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_8 = _mm_sub_epi64(v128i_7, v128i_7);
    __m256i v256i_9 = _mm256_set1_epi32(0x55);
    __m512d v512d_10 = _mm512_insertf64x2(v512d_5, p3, 3);
    __m512 v512f_11 = _mm512_div_ps(v512f_2, v512f_2);
    __m128 v128f_12 = g_v128f[7];
    __m128 v128f_13 = _mm_add_ps(v128f_12, v128f_12);
    __m256i v256i_14 = _mm256_and_si256(v256i_9, v256i_9);
    __m512 v512f_15 = _mm512_set1_ps(1.0f);
    __m512h v512h_16 = _mm512_cvtepi16_ph(v512i_6);
    __m256i v256i_17 = _mm256_abs_epi32(v256i_9);
    __m512i v512i_18 = _mm512_abs_epi32(v512i_6);
    __m512i v512i_19 = _mm512_permutexvar_epi32(v512i_3, v512i_3);
    __m512 v512f_20 = _mm512_fmsub_ps(v512f_2, v512f_2, v512f_11);
    int s32_21 = 78;
    if ((s32_21) & 1ULL) {
    __m256i v256i_22 = _mm256_shuffle_epi8(v256i_14, v256i_9);
    __m512i v512i_23 = _mm512_broadcast_i32x4(v128i_7);
    unsigned u32_24 = 79;
    __mmask16 m16_25 = _cvtu32_mask16(u32_24);
    __m512 v512f_26 = _mm512_cvtepi32_ps(v512i_6);
    __m128 v128f_27 = _mm_set1_ps(1.0f);
    __m256i v256i_28 = _mm256_set1_epi32(0x55);
    }
    __m512 v512f_29 = _mm512_max_ps(v512f_11, v512f_11);
    __m512d v512d_30 = _mm512_insertf64x2(v512d_5, p3, 3);
    __m128d v128d_31 = _mm_unpackhi_pd(p3, p3);
    __m512i v512i_32 = _mm512_lzcnt_epi32(v512i_18);
    __mmask16 m16_33 = (__mmask16)(g_m16[11] ^ 28);
    __mmask16 m16_34 = _kand_mask16(m16_33, m16_33);
    __m512i v512i_35 = _mm512_sub_epi8(v512i_3, v512i_19);
    __m128d v128d_36 = _mm_unpackhi_pd(v128d_31, v128d_31);
    for (int i_37 = 0; i_37 < (int)(((unsigned long long)m16_33) & 7); i_37++) {
    __m256i v256i_38 = _mm256_abs_epi32(v256i_9);
    __m512i v512i_39 = _mm512_dpwssd_epi32(v512i_35, v512i_6, v512i_35);
    }
    g_v512f[5] = v512f_15;
    g_v256f[2] = v256f_4;
    g_v256d[4] = p2;
    g_v128d[7] = v128d_36;
    g_v512h[3] = v512h_16;
    return v256f_4;
}

__m128i torture_186(__m128i p0, __m256 p1, __m128i p2, __m256 p3) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sllv_epi32(v256i_1, v256i_1);
    unsigned u32_3 = 68;
    __mmask16 m16_4 = _cvtu32_mask16(u32_3);
    __mmask64 m64_5 = (__mmask64)(g_m64[3] ^ 104);
    __mmask64 m64_6 = _kor_mask64(m64_5, m64_5);
    __m512 v512f_7 = g_v512f[3];
    __m512 v512f_8 = _mm512_fmadd_ps(v512f_7, v512f_7, v512f_7);
    __m128i v128i_9 = _mm_add_epi64(p2, p2);
    __m128 v128f_10 = g_v128f[10];
    __m128 v128f_11 = _mm_fmsub_ps(v128f_10, v128f_10, v128f_10);
    __m512d v512d_12 = g_v512d[3];
    __m512d v512d_13 = _mm512_max_pd(v512d_12, v512d_12);
    __m256d v256d_14 = g_v256d[2];
    __m256d v256d_15 = _mm256_sub_pd(v256d_14, v256d_14);
    __m512 v512f_16 = _mm512_maskz_mul_ps(m16_4, v512f_7, v512f_7);
    __m512i v512i_17 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_18 = _mm512_sub_epi8(v512i_17, v512i_17);
    __m512h v512h_19 = g_v512h[9];
    __m512h v512h_20 = _mm512_rcp_ph(v512h_19);
    __m256 v256f_21 = _mm256_xor_ps(p1, p3);
    __m256 v256f_22 = _mm256_and_ps(p3, p3);
    __m256 v256f_23 = _mm256_xor_ps(v256f_21, v256f_21);
    __m512 v512f_24 = _mm512_maskz_mul_ps(m16_4, v512f_8, v512f_16);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_4) & 7); i_25++) {
    __mmask16 m16_26 = _mm512_movepi32_mask(v512i_18);
    __mmask16 m16_27 = _kshiftli_mask16(m16_4, 2);
    __m128 v128f_28 = _mm_mul_ps(v128f_11, v128f_10);
    __m256i v256i_29 = _mm256_add_epi8(v256i_2, v256i_2);
    __m512i v512i_30 = _mm512_or_si512(v512i_17, v512i_17);
    }
    __m128 v128f_31 = _mm_sqrt_ps(v128f_11);
    __m128 v128f_32 = _mm_xor_ps(v128f_31, v128f_11);
    __mmask32 m32_33 = (__mmask32)(g_m32[11] ^ 116);
    __mmask32 m32_34 = _kxor_mask32(m32_33, m32_33);
    __mmask8 m8_35 = (__mmask8)(g_m8[11] ^ 140);
    __mmask16 m16_36 = _mm512_kunpackb(m8_35, m8_35);
    __m512h v512h_37 = _mm512_getexp_ph(v512h_19);
    __m512i v512i_38 = _mm512_add_epi32(v512i_17, v512i_18);
    __mmask16 m16_39 = _kand_mask16(m16_36, m16_36);
    if (((unsigned long long)m16_39) & 1ULL) {
    __m512i v512i_40 = _mm512_shuffle_i32x4(v512i_17, v512i_38, 0x1B);
    __m256i v256i_41 = _mm256_add_epi32(v256i_2, v256i_1);
    }
    g_v512d[7] = v512d_12;
    g_v128f[5] = v128f_32;
    g_v512h[4] = v512h_19;
    return p2;
}

__m128 torture_187(__m512 p0, __m128i p1, __m128i p2, __m256h p3) {
    __m512i v512i_1 = g_v512i[6];
    __m512i v512i_2 = _mm512_or_si512(v512i_1, v512i_1);
    __m512i v512i_3 = _mm512_slli_epi32(v512i_2, 3);
    __m128d v128d_4 = g_v128d[11];
    __m128d v128d_5 = _mm_fmadd_pd(v128d_4, v128d_4, v128d_4);
    __m128i v128i_6 = _mm512_cvtepi32_epi8(v512i_1);
    __m256d v256d_7 = _mm256_set1_pd(2.0);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_max_ps(v256f_8, v256f_8);
    __m128 v128f_10 = _mm_loadu_ps(fbuf);
    __m128 v128f_11 = _mm_fmadd_ps(v128f_10, v128f_10, v128f_10);
    __m256 v256f_12 = _mm256_and_ps(v256f_9, v256f_9);
    __m512i v512i_13 = _mm512_shuffle_epi8(v512i_2, v512i_1);
    __m256i v256i_14 = g_v256i[8];
    __m256i v256i_15 = _mm256_sub_epi64(v256i_14, v256i_14);
    __m512d v512d_16 = g_v512d[5];
    __m512d v512d_17 = _mm512_mul_pd(v512d_16, v512d_16);
    __m256i v256i_18 = _mm512_cvtepi64_epi32(v512i_13);
    __m256i v256i_19 = _mm256_and_si256(v256i_14, v256i_18);
    __m256d v256d_20 = _mm256_max_pd(v256d_7, v256d_7);
    __m512i v512i_21 = _mm512_permutex2var_epi32(v512i_3, v512i_13, v512i_13);
    __mmask16 m16_22 = (__mmask16)(g_m16[5] ^ 185);
    __m512 v512f_23 = _mm512_maskz_mul_ps(m16_22, p0, p0);
    __m512h v512h_24 = g_v512h[11];
    __m512h v512h_25 = _mm512_fmul_pch(v512h_24, v512h_24);
    __m256d v256d_26 = _mm256_sqrt_pd(v256d_20);
    __m512 v512f_27 = _mm512_rsqrt14_ps(p0);
    __m512 v512f_28 = _mm512_cvtepi32_ps(v512i_13);
    if (((unsigned long long)m16_22) & 1ULL) {
    __m512d v512d_29 = _mm512_min_pd(v512d_16, v512d_16);
    __m128 v128f_30 = _mm_unpacklo_ps(v128f_10, v128f_10);
    __m128d v128d_31 = _mm_sqrt_pd(v128d_5);
    __m512d v512d_32 = _mm512_set1_pd(2.0);
    __mmask16 m16_33 = _mm512_fpclass_ps_mask(v512f_27, 0x3);
    }
    g_v512f[5] = v512f_23;
    g_v128i[7] = p2;
    g_v512h[9] = v512h_25;
    g_m16[9] = m16_22;
    return v128f_10;
}

__m256d torture_188(__m512i p0, __m512d p1) {
    __m128 v128f_1 = g_v128f[6];
    __m128 v128f_2 = _mm_mul_ps(v128f_1, v128f_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[4] ^ 55);
    __m512i v512i_4 = _mm512_broadcastmw_epi32(m16_3);
    __m256i v256i_5 = g_v256i[1];
    __m256i v256i_6 = _mm256_add_epi16(v256i_5, v256i_5);
    __m128i v128i_7 = g_v128i[2];
    __m128i v128i_8 = _mm_abs_epi32(v128i_7);
    __m512i v512i_9 = _mm512_lzcnt_epi32(v512i_4);
    __m256i v256i_10 = _mm256_set1_epi32(0x55);
    __m256i v256i_11 = _mm256_sllv_epi32(v256i_5, v256i_5);
    __m128i v128i_12 = _mm_mullo_epi32(v128i_7, v128i_8);
    __m512i v512i_13 = _mm512_add_epi64(v512i_9, v512i_4);
    __m128d v128d_14 = g_v128d[7];
    __m512d v512d_15 = _mm512_insertf64x2(p1, v128d_14, 3);
    __m512i v512i_16 = _mm512_shuffle_epi8(p0, p0);
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_15, p1, 0xEE);
    __m512 v512f_18 = g_v512f[7];
    __m512 v512f_19 = _mm512_maskz_mul_ps(m16_3, v512f_18, v512f_18);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m512i v512i_20 = _mm512_add_epi16(v512i_13, p0);
    __m128 v128f_21 = _mm512_extractf32x4_ps(v512f_19, 2);
    }
    __m128i v128i_22 = _mm_add_epi8(v128i_8, v128i_12);
    __m128 v128f_23 = _mm512_extractf32x4_ps(v512f_19, 2);
    __m128i v128i_24 = _mm_sub_epi32(v128i_7, v128i_22);
    __m256d v256d_25 = _mm256_set1_pd(2.0);
    __m512 v512f_26 = _mm512_xor_ps(v512f_19, v512f_18);
    __m512h v512h_27 = g_v512h[10];
    __m512h v512h_28 = _mm512_add_ph(v512h_27, v512h_27);
    __m512h v512h_29 = _mm512_mul_ph(v512h_28, v512h_27);
    g_v512d[0] = v512d_15;
    g_v512i[9] = v512i_16;
    g_v128f[11] = v128f_1;
    g_v128d[10] = v128d_14;
    g_v128i[0] = v128i_24;
    g_v512h[1] = v512h_28;
    g_m16[5] = m16_3;
    return v256d_25;
}

__m256 torture_189(__m256h p0, __m256i p1, __m512i p2, __m256 p3, __m256 p4) {
    __mmask8 m8_1 = (__mmask8)(g_m8[8] ^ 58);
    __mmask16 m16_2 = _mm512_kunpackb(m8_1, m8_1);
    __m512i v512i_3 = _mm512_add_epi16(p2, p2);
    __m256 v256f_4 = _mm256_xor_ps(p3, p4);
    __m512d v512d_5 = g_v512d[1];
    __m512d v512d_6 = _mm512_fmadd_pd(v512d_5, v512d_5, v512d_5);
    __m256d v256d_7 = _mm256_set1_pd(2.0);
    __m512 v512f_8 = _mm512_loadu_ps(fbuf);
    __m128 v128f_9 = _mm512_extractf32x4_ps(v512f_8, 2);
    __m512i v512i_10 = _mm512_add_epi32(v512i_3, v512i_3);
    __m128 v128f_11 = _mm_shuffle_ps(v128f_9, v128f_9, 0x1B);
    __m512 v512f_12 = _mm512_shuffle_f32x4(v512f_8, v512f_8, 0x4E);
    __mmask16 m16_13 = _kxor_mask16(m16_2, m16_2);
    __m128i v128i_14 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_15 = _mm_or_si128(v128i_14, v128i_14);
    __m512i v512i_16 = _mm512_ternarylogic_epi32(v512i_10, v512i_10, p2, 0x96);
    __m256d v256d_17 = _mm256_fmadd_pd(v256d_7, v256d_7, v256d_7);
    __m128d v128d_18 = g_v128d[6];
    __m128d v128d_19 = _mm_add_pd(v128d_18, v128d_18);
    __m256 v256f_20 = _mm256_mul_ps(v256f_4, p4);
    __m512 v512f_21 = _mm512_fmsub_ps(v512f_8, v512f_8, v512f_12);
    __m256 v256f_22 = _mm256_unpacklo_ps(v256f_4, v256f_20);
    __m256h v256h_23 = _mm512_cvtxps_ph(v512f_12);
    if (((unsigned long long)m16_2) & 1ULL) {
    __mmask16 m16_24 = _mm512_test_epi32_mask(p2, v512i_16);
    __m512 v512f_25 = _mm512_set1_ps(1.0f);
    }
    g_v512f[9] = v512f_8;
    g_v512d[3] = v512d_5;
    g_v512i[1] = v512i_10;
    g_v256f[4] = v256f_22;
    g_v256d[5] = v256d_7;
    g_v128f[1] = v128f_9;
    g_v128i[7] = v128i_14;
    g_v256h[0] = p0;
    g_m16[10] = m16_2;
    return p4;
}

__mmask32 torture_190(__m512h p0, __m512h p1) {
    __m128 v128f_1 = _mm_loadu_ps(fbuf);
    __m128 v128f_2 = _mm_unpacklo_ps(v128f_1, v128f_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512i v512i_4 = _mm512_cvtpd_epi64(v512d_3);
    __m512i v512i_5 = _mm512_sub_epi32(v512i_4, v512i_4);
    __m512 v512f_6 = g_v512f[6];
    __m512 v512f_7 = _mm512_rsqrt14_ps(v512f_6);
    __m512d v512d_8 = _mm512_mul_pd(v512d_3, v512d_3);
    __m512i v512i_9 = _mm512_set1_epi32(0x55);
    __m512 v512f_10 = _mm512_sub_ps(v512f_7, v512f_7);
    __m128 v128f_11 = _mm_sub_ps(v128f_1, v128f_1);
    __m128i v128i_12 = _mm512_cvtepi32_epi8(v512i_9);
    __m512 v512f_13 = _mm512_rcp14_ps(v512f_7);
    int s32_14 = 96;
    if ((s32_14) & 1ULL) {
    __m128d v128d_15 = g_v128d[6];
    __m128d v128d_16 = _mm_min_pd(v128d_15, v128d_15);
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_3, v512d_8, 0xEE);
    }
    __m256 v256f_18 = _mm256_loadu_ps(fbuf);
    __m256 v256f_19 = _mm256_xor_ps(v256f_18, v256f_18);
    __m256 v256f_20 = _mm256_set1_ps(1.0f);
    __m512d v512d_21 = _mm512_shuffle_f64x2(v512d_3, v512d_3, 0xEE);
    __m512i v512i_22 = _mm512_unpacklo_epi32(v512i_9, v512i_5);
    __m128 v128f_23 = _mm512_extractf32x4_ps(v512f_13, 2);
    __m512 v512f_24 = _mm512_and_ps(v512f_6, v512f_10);
    __m128d v128d_25 = _mm_set1_pd(2.0);
    __m128d v128d_26 = _mm_sqrt_pd(v128d_25);
    g_v512f[4] = v512f_24;
    g_v512i[5] = v512i_5;
    g_v128i[5] = v128i_12;
    g_v512h[0] = p0;
    __mmask32 m32_27 = (__mmask32)(g_m32[1] ^ 77);
    return m32_27;
}

__m512h torture_191(__m512i p0, __m512d p1, __m512 p2, __m256 p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[0] ^ 166);
    __m512 v512f_2 = _mm512_mask_blend_ps(m16_1, p2, p2);
    __m512i v512i_3 = _mm512_broadcastmw_epi32(m16_1);
    __m128 v128f_4 = _mm_set1_ps(1.0f);
    __m128 v128f_5 = _mm_unpacklo_ps(v128f_4, v128f_4);
    __m512i v512i_6 = _mm512_mask_blend_epi32(m16_1, p0, p0);
    __mmask8 m8_7 = (__mmask8)(g_m8[0] ^ 90);
    __mmask8 m8_8 = _kand_mask8(m8_7, m8_7);
    __m256i v256i_9 = _mm512_cvtepi64_epi32(p0);
    __m256i v256i_10 = _mm256_set1_epi32(0x55);
    __m256d v256d_11 = _mm512_extractf64x4_pd(p1, 1);
    if (((unsigned long long)m16_1) & 1ULL) {
    __mmask16 m16_12 = _mm512_kunpackb(m8_8, m8_8);
    __mmask16 m16_13 = _kxor_mask16(m16_12, m16_1);
    __m128i v128i_14 = g_v128i[2];
    __m128i v128i_15 = _mm_add_epi16(v128i_14, v128i_14);
    __m128 v128f_16 = _mm_add_ps(v128f_5, v128f_4);
    __m128d v128d_17 = g_v128d[3];
    __m128d v128d_18 = _mm_min_pd(v128d_17, v128d_17);
    __m256i v256i_19 = _mm256_sub_epi8(v256i_9, v256i_9);
    }
    __m256d v256d_20 = _mm256_fmadd_pd(v256d_11, v256d_11, v256d_11);
    __m512i v512i_21 = _mm512_abs_epi32(v512i_3);
    __m512d v512d_22 = _mm512_fmadd_pd(p1, p1, p1);
    __mmask16 m16_23 = _kor_mask16(m16_1, m16_1);
    __m512i v512i_24 = _mm512_cvtps_epi32(p2);
    __m512h v512h_25 = g_v512h[11];
    __m512h v512h_26 = _mm512_sqrt_ph(v512h_25);
    __mmask32 m32_27 = _mm512_cmp_ph_mask(v512h_26, v512h_25, 1);
    __m128i v128i_28 = g_v128i[5];
    __m128i v128i_29 = _mm_shuffle_epi8(v128i_28, v128i_28);
    __m512i v512i_30 = _mm512_unpacklo_epi32(p0, v512i_21);
    __m512d v512d_31 = _mm512_sub_pd(v512d_22, v512d_22);
    __m512i v512i_32 = _mm512_unpacklo_epi32(v512i_6, v512i_30);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m512d v512d_33 = _mm512_cvtps_pd(p3);
    __mmask16 m16_34 = _mm512_fpclass_ps_mask(v512f_2, 0x3);
    }
    g_v512i[2] = v512i_3;
    g_m16[2] = m16_23;
    g_m32[6] = m32_27;
    return v512h_26;
}

__m256 torture_192(__m256i p0, __m256d p1, __m512i p2, __m128h p3, __m256h p4) {
    __mmask64 m64_1 = _mm512_movepi8_mask(p2);
    __m128d v128d_2 = g_v128d[0];
    __m128d v128d_3 = _mm_mul_pd(v128d_2, v128d_2);
    __m256d v256d_4 = _mm256_add_pd(p1, p1);
    __m512i v512i_5 = _mm512_permutexvar_epi32(p2, p2);
    __mmask16 m16_6 = _mm512_cmp_epi32_mask(p2, p2, 2);
    __m256i v256i_7 = _mm256_add_epi8(p0, p0);
    __m512 v512f_8 = _mm512_loadu_ps(fbuf);
    __m512 v512f_9 = _mm512_fnmadd_ps(v512f_8, v512f_8, v512f_8);
    __m256i v256i_10 = _mm512_cvtepi64_epi32(v512i_5);
    __m128 v128f_11 = _mm_loadu_ps(fbuf);
    __m128 v128f_12 = _mm_and_ps(v128f_11, v128f_11);
    __m128i v128i_13 = g_v128i[10];
    __m512i v512i_14 = _mm512_cvtepi8_epi32(v128i_13);
    __m256 v256f_15 = _mm256_set1_ps(1.0f);
    __m512i v512i_16 = _mm512_popcnt_epi64(v512i_14);
    __mmask16 m16_17 = _knot_mask16(m16_6);
    __m128i v128i_18 = _mm512_cvtepi32_epi8(v512i_5);
    __m256 v256f_19 = _mm256_shuffle_ps(v256f_15, v256f_15, 0x1B);
    __m128d v128d_20 = _mm_sub_pd(v128d_2, v128d_2);
    __m512i v512i_21 = _mm512_broadcastmw_epi32(m16_17);
    __m512h v512h_22 = g_v512h[8];
    __m512h v512h_23 = _mm512_mul_ph(v512h_22, v512h_22);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_17) & 7); i_24++) {
    __m128i v128i_25 = _mm_set1_epi32(0x55);
    __m512 v512f_26 = _mm512_fmadd_ps(v512f_8, v512f_9, v512f_8);
    __mmask16 m16_27 = _mm512_cmp_epi32_mask(v512i_21, v512i_16, 2);
    __m128i v128i_28 = _mm_sub_epi64(v128i_25, v128i_18);
    __m512d v512d_29 = _mm512_cvtps_pd(v256f_15);
    }
    g_v512i[1] = v512i_16;
    g_v256f[3] = v256f_19;
    g_v256i[4] = v256i_10;
    g_v128i[7] = v128i_13;
    g_v256h[11] = p4;
    g_m16[6] = m16_17;
    return v256f_15;
}

__m256i torture_193(__m256i p0, __m256d p1, __m256 p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_rol_epi32(v512i_1, 5);
    __mmask16 m16_3 = (__mmask16)(g_m16[3] ^ 108);
    __mmask16 m16_4 = _kshiftli_mask16(m16_3, 2);
    __m512i v512i_5 = _mm512_add_epi16(v512i_2, v512i_1);
    __mmask16 m16_6 = _kand_mask16(m16_3, m16_4);
    __m256 v256f_7 = _mm256_add_ps(p2, p2);
    __m512i v512i_8 = _mm512_sub_epi32(v512i_2, v512i_5);
    __m512 v512f_9 = g_v512f[9];
    __m512 v512f_10 = _mm512_insertf32x8(v512f_9, p2, 1);
    __m128 v128f_11 = _mm_loadu_ps(fbuf);
    __m128 v128f_12 = _mm_unpacklo_ps(v128f_11, v128f_11);
    __m512d v512d_13 = g_v512d[10];
    __m256 v256f_14 = _mm512_cvtpd_ps(v512d_13);
    __m512i v512i_15 = _mm512_set1_epi32(0x55);
    __m512 v512f_16 = _mm512_mask_blend_ps(m16_6, v512f_9, v512f_10);
    __m512i v512i_17 = _mm512_slli_epi32(v512i_15, 3);
    __m512 v512f_18 = _mm512_sub_ps(v512f_9, v512f_16);
    __m512i v512i_19 = _mm512_dpbusd_epi32(v512i_1, v512i_17, v512i_17);
    __m256i v256i_20 = _mm256_shuffle_epi8(p0, p0);
    __mmask8 m8_21 = (__mmask8)(g_m8[4] ^ 4);
    __m512i v512i_22 = _mm512_broadcastmb_epi64(m8_21);
    __m512h v512h_23 = g_v512h[10];
    __m512h v512h_24 = _mm512_add_ph(v512h_23, v512h_23);
    g_v512d[0] = v512d_13;
    g_v512i[7] = v512i_17;
    g_m16[11] = m16_6;
    return v256i_20;
}

__mmask8 torture_194(__m512i p0, __m256i p1) {
    __m256d v256d_1 = g_v256d[2];
    __m256d v256d_2 = _mm256_sqrt_pd(v256d_1);
    __m512 v512f_3 = _mm512_set1_ps(1.0f);
    __m256 v256f_4 = _mm256_loadu_ps(fbuf);
    __m256 v256f_5 = _mm256_sub_ps(v256f_4, v256f_4);
    __mmask16 m16_6 = (__mmask16)(g_m16[1] ^ 134);
    __mmask16 m16_7 = _kor_mask16(m16_6, m16_6);
    unsigned u32_8 = 86;
    __mmask16 m16_9 = _cvtu32_mask16(u32_8);
    __m512 v512f_10 = _mm512_insertf32x8(v512f_3, v256f_4, 1);
    __m256d v256d_11 = _mm256_set1_pd(2.0);
    __m512 v512f_12 = _mm512_shuffle_f32x4(v512f_3, v512f_10, 0x4E);
    __m256 v256f_13 = _mm256_sqrt_ps(v256f_4);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m512h v512h_14 = g_v512h[0];
    __m512h v512h_15 = _mm512_fmsub_ph(v512h_14, v512h_14, v512h_14);
    __m128 v128f_16 = g_v128f[4];
    __m128 v128f_17 = _mm_and_ps(v128f_16, v128f_16);
    __m512i v512i_18 = _mm512_xor_si512(p0, p0);
    }
    __m128i v128i_19 = g_v128i[11];
    __m128i v128i_20 = _mm_add_epi8(v128i_19, v128i_19);
    __m128i v128i_21 = _mm_add_epi64(v128i_20, v128i_19);
    __m128 v128f_22 = _mm_loadu_ps(fbuf);
    __m128 v128f_23 = _mm_div_ps(v128f_22, v128f_22);
    __m256 v256f_24 = _mm256_min_ps(v256f_4, v256f_5);
    __m256d v256d_25 = _mm256_max_pd(v256d_2, v256d_1);
    g_v512f[5] = v512f_3;
    g_v128f[8] = v128f_22;
    g_v128i[2] = v128i_19;
    __mmask8 m8_26 = (__mmask8)(g_m8[9] ^ 170);
    return m8_26;
}

__m256 torture_195(__m512i p0, __m128 p1, __m512i p2, __m256i p3) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_sub_epi8(v128i_1, v128i_1);
    __m512h v512h_3 = g_v512h[11];
    __m512h v512h_4 = _mm512_add_ph(v512h_3, v512h_3);
    __m512i v512i_5 = _mm512_or_si512(p2, p2);
    __m256d v256d_6 = g_v256d[6];
    __m256d v256d_7 = _mm256_fmadd_pd(v256d_6, v256d_6, v256d_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_sub_ps(v256f_8, v256f_8);
    __m512d v512d_10 = g_v512d[9];
    __m512d v512d_11 = _mm512_div_pd(v512d_10, v512d_10);
    __m512d v512d_12 = _mm512_fmadd_pd(v512d_10, v512d_10, v512d_10);
    __m256i v256i_13 = _mm256_sub_epi64(p3, p3);
    __m512 v512f_14 = _mm512_loadu_ps(fbuf);
    __m512 v512f_15 = _mm512_fmsub_ps(v512f_14, v512f_14, v512f_14);
    __m256i v256i_16 = _mm256_slli_epi32(v256i_13, 3);
    __mmask16 m16_17 = _mm512_cmp_epu32_mask(v512i_5, v512i_5, 4);
    __m256 v256f_18 = _mm512_cvtpd_ps(v512d_11);
    __m128 v128f_19 = _mm_and_ps(p1, p1);
    __m256i v256i_20 = _mm256_sub_epi8(v256i_16, p3);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m256 v256f_21 = _mm256_fmsub_ps(v256f_8, v256f_18, v256f_18);
    __m256 v256f_22 = _mm512_cvtpd_ps(v512d_11);
    __m512i v512i_23 = _mm512_set1_epi32(0x55);
    __m512 v512f_24 = _mm512_roundscale_ps(v512f_15, 0x10);
    __m128d v128d_25 = g_v128d[6];
    __m128d v128d_26 = _mm_mul_pd(v128d_25, v128d_25);
    __m512 v512f_27 = _mm512_cvtepi32_ps(v512i_5);
    }
    __m512d v512d_28 = _mm512_mul_pd(v512d_10, v512d_12);
    __m256 v256f_29 = _mm256_fnmadd_ps(v256f_9, v256f_8, v256f_8);
    __mmask16 m16_30 = _mm512_cmp_ps_mask(v512f_14, v512f_14, _CMP_LT_OQ);
    __m128 v128f_31 = _mm_xor_ps(v128f_19, p1);
    __m256i v256i_32 = _mm512_cvtepi64_epi32(p2);
    __m128d v128d_33 = g_v128d[6];
    __m128d v128d_34 = _mm_mul_pd(v128d_33, v128d_33);
    __m256d v256d_35 = _mm256_add_pd(v256d_7, v256d_7);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_30) & 7); i_36++) {
    __m512h v512h_37 = _mm512_mul_ph(v512h_3, v512h_3);
    __m512i v512i_38 = _mm512_permutex2var_epi32(v512i_5, v512i_5, p0);
    __m512 v512f_39 = _mm512_sub_ps(v512f_15, v512f_15);
    }
    g_v512f[10] = v512f_14;
    g_v512i[11] = p0;
    g_v256f[2] = v256f_8;
    g_v256i[11] = v256i_32;
    g_v512h[8] = v512h_4;
    return v256f_18;
}

__m256i torture_196(__m512h p0, __m128d p1) {
    __m512d v512d_1 = g_v512d[10];
    __m512d v512d_2 = _mm512_div_pd(v512d_1, v512d_1);
    __m128 v128f_3 = g_v128f[5];
    __m128 v128f_4 = _mm_unpacklo_ps(v128f_3, v128f_3);
    __m512i v512i_5 = g_v512i[3];
    __m512i v512i_6 = _mm512_abs_epi32(v512i_5);
    __m256d v256d_7 = _mm512_extractf64x4_pd(v512d_2, 1);
    __m128i v128i_8 = g_v128i[6];
    __m128i v128i_9 = _mm_add_epi16(v128i_8, v128i_8);
    __m512i v512i_10 = _mm512_dpbusd_epi32(v512i_5, v512i_6, v512i_5);
    __m512i v512i_11 = _mm512_rolv_epi32(v512i_10, v512i_6);
    __m256d v256d_12 = _mm256_max_pd(v256d_7, v256d_7);
    __mmask16 m16_13 = (__mmask16)(g_m16[1] ^ 60);
    __m512 v512f_14 = _mm512_loadu_ps(fbuf);
    __m512 v512f_15 = _mm512_mask_blend_ps(m16_13, v512f_14, v512f_14);
    __m128d v128d_16 = _mm_unpackhi_pd(p1, p1);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_13) & 7); i_17++) {
    __mmask16 m16_18 = _kor_mask16(m16_13, m16_13);
    __m512i v512i_19 = _mm512_broadcast_i32x4(v128i_9);
    __m128i v128i_20 = _mm_set1_epi32(0x55);
    __m128i v128i_21 = _mm_add_epi8(v128i_9, v128i_8);
    }
    __m512d v512d_22 = _mm512_insertf64x2(v512d_1, v128d_16, 3);
    __m256d v256d_23 = _mm256_mul_pd(v256d_12, v256d_7);
    __m128d v128d_24 = _mm_unpackhi_pd(p1, v128d_16);
    __m512 v512f_25 = _mm512_roundscale_ps(v512f_14, 0x10);
    __m128d v128d_26 = _mm_min_pd(v128d_16, v128d_24);
    __m128h v128h_27 = _mm512_cvtpd_ph(v512d_22);
    __m128i v128i_28 = _mm_mullo_epi32(v128i_9, v128i_9);
    __m512d v512d_29 = _mm512_mul_pd(v512d_22, v512d_1);
    __m512i v512i_30 = _mm512_set1_epi32(0x55);
    __m512 v512f_31 = _mm512_cvtepi32_ps(v512i_6);
    __m128i v128i_32 = _mm_unpacklo_epi32(v128i_9, v128i_9);
    g_v512d[0] = v512d_2;
    g_v128f[11] = v128f_3;
    g_m16[10] = m16_13;
    __m256i v256i_33 = g_v256i[6];
    return v256i_33;
}

__m256 torture_197(__m128 p0, __m512h p1, __m512h p2, __m512h p3) {
    __m128d v128d_1 = g_v128d[3];
    __m128d v128d_2 = _mm_div_pd(v128d_1, v128d_1);
    __m256 v256f_3 = g_v256f[3];
    __m256 v256f_4 = _mm256_sub_ps(v256f_3, v256f_3);
    __m128i v128i_5 = g_v128i[4];
    __m128i v128i_6 = _mm_unpacklo_epi32(v128i_5, v128i_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_sub_epi8(v512i_7, v512i_7);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m512 v512f_10 = _mm512_rcp14_ps(v512f_9);
    __m512i v512i_11 = _mm512_sub_epi8(v512i_8, v512i_7);
    __m128 v128f_12 = _mm512_extractf32x4_ps(v512f_10, 2);
    __mmask16 m16_13 = (__mmask16)(g_m16[1] ^ 66);
    __m512i v512i_14 = _mm512_mask_blend_epi32(m16_13, v512i_8, v512i_11);
    __m512d v512d_15 = g_v512d[8];
    __m512i v512i_16 = _mm512_cvtpd_epi64(v512d_15);
    unsigned u32_17 = 37;
    __mmask16 m16_18 = _cvtu32_mask16(u32_17);
    __m512 v512f_19 = _mm512_add_ps(v512f_10, v512f_9);
    __m128i v128i_20 = _mm_add_epi8(v128i_5, v128i_6);
    __m512i v512i_21 = _mm512_shuffle_i32x4(v512i_7, v512i_14, 0x1B);
    __m256 v256f_22 = _mm256_min_ps(v256f_4, v256f_4);
    __m128i v128i_23 = _mm_and_si128(v128i_5, v128i_6);
    __m256 v256f_24 = _mm512_cvtpd_ps(v512d_15);
    if (((unsigned long long)m16_13) & 1ULL) {
    __m512 v512f_25 = _mm512_insertf32x8(v512f_19, v256f_22, 1);
    __m512i v512i_26 = _mm512_unpacklo_epi32(v512i_21, v512i_21);
    __m512 v512f_27 = _mm512_max_ps(v512f_9, v512f_19);
    __m512i v512i_28 = _mm512_mask_add_epi32(v512i_26, m16_13, v512i_26, v512i_8);
    __m256 v256f_29 = _mm256_fmadd_ps(v256f_4, v256f_3, v256f_4);
    }
    __m256i v256i_30 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_31 = _mm256_unpacklo_epi32(v256i_30, v256i_30);
    __m256i v256i_32 = _mm256_add_epi16(v256i_30, v256i_30);
    __m256 v256f_33 = _mm256_shuffle_ps(v256f_22, v256f_3, 0x1B);
    __m512h v512h_34 = _mm512_fmsub_ph(p1, p3, p3);
    __m512i v512i_35 = _mm512_mullo_epi32(v512i_21, v512i_7);
    __mmask8 m8_36 = (__mmask8)(g_m8[5] ^ 34);
    __mmask16 m16_37 = _mm512_kunpackb(m8_36, m8_36);
    __mmask8 m8_38 = _mm512_cmp_pd_mask(v512d_15, v512d_15, _CMP_GE_OQ);
    __m128i v128i_39 = _mm_unpacklo_epi32(v128i_5, v128i_20);
    if (((unsigned long long)m16_18) & 1ULL) {
    __m512i v512i_40 = _mm512_xor_si512(v512i_35, v512i_11);
    __m512 v512f_41 = _mm512_maskz_mul_ps(m16_18, v512f_10, v512f_10);
    __m256d v256d_42 = g_v256d[1];
    __m256d v256d_43 = _mm256_min_pd(v256d_42, v256d_42);
    __m128i v128i_44 = _mm_sub_epi8(v128i_5, v128i_39);
    __m512h v512h_45 = _mm512_getexp_ph(p2);
    __m512 v512f_46 = _mm512_sub_ps(v512f_41, v512f_9);
    }
    g_v256i[3] = v256i_30;
    g_v128f[7] = v128f_12;
    g_m8[11] = m8_38;
    return v256f_22;
}

__m256 torture_198(__m256h p0, __m256i p1) {
    __m256i v256i_1 = _mm256_shuffle_epi8(p1, p1);
    __m256i v256i_2 = _mm256_add_epi8(p1, p1);
    __m512i v512i_3 = g_v512i[2];
    __m512i v512i_4 = _mm512_popcnt_epi64(v512i_3);
    __m128 v128f_5 = _mm_loadu_ps(fbuf);
    __m128 v128f_6 = _mm_shuffle_ps(v128f_5, v128f_5, 0x1B);
    __m256d v256d_7 = _mm256_set1_pd(2.0);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_9 = _mm_add_epi64(v128i_8, v128i_8);
    __m128d v128d_10 = g_v128d[0];
    __m128d v128d_11 = _mm_mul_pd(v128d_10, v128d_10);
    __m256i v256i_12 = _mm256_or_si256(p1, v256i_2);
    __m512i v512i_13 = _mm512_or_si512(v512i_3, v512i_4);
    __m128 v128f_14 = _mm_sub_ps(v128f_5, v128f_6);
    __m256 v256f_15 = g_v256f[0];
    __m256 v256f_16 = _mm256_sub_ps(v256f_15, v256f_15);
    __m512i v512i_17 = _mm512_sllv_epi32(v512i_13, v512i_4);
    __m256i v256i_18 = _mm256_abs_epi32(v256i_12);
    __m512 v512f_19 = g_v512f[2];
    __m512 v512f_20 = _mm512_rsqrt14_ps(v512f_19);
    __m256d v256d_21 = _mm256_div_pd(v256d_7, v256d_7);
    __mmask32 m32_22 = (__mmask32)(g_m32[1] ^ 196);
    __mmask32 m32_23 = _kxor_mask32(m32_22, m32_22);
    __m512d v512d_24 = g_v512d[1];
    __m512d v512d_25 = _mm512_div_pd(v512d_24, v512d_24);
    if (((unsigned long long)m32_23) & 1ULL) {
    __m512 v512f_26 = _mm512_unpacklo_ps(v512f_19, v512f_20);
    __m512i v512i_27 = _mm512_popcnt_epi64(v512i_13);
    }
    __m512 v512f_28 = _mm512_scalef_ps(v512f_20, v512f_20);
    __m512i v512i_29 = _mm512_and_si512(v512i_13, v512i_13);
    __m512 v512f_30 = _mm512_broadcast_f32x4(v128f_6);
    __m512 v512f_31 = _mm512_add_ps(v512f_19, v512f_20);
    g_v256d[2] = v256d_7;
    g_v128f[8] = v128f_14;
    g_v128d[6] = v128d_10;
    g_v256h[8] = p0;
    return v256f_16;
}

__m256i torture_199(__m256i p0, __m256d p1, __m256i p2, __m512i p3) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_xor_ps(v256f_1, v256f_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_sub_epi64(v128i_3, v128i_3);
    __m512i v512i_5 = _mm512_set1_epi32(0x55);
    __m512h v512h_6 = g_v512h[2];
    __m512h v512h_7 = _mm512_fmsub_ph(v512h_6, v512h_6, v512h_6);
    __m512i v512i_8 = _mm512_permutex2var_epi32(v512i_5, v512i_5, p3);
    __m128 v128f_9 = _mm_loadu_ps(fbuf);
    __m128 v128f_10 = _mm_max_ps(v128f_9, v128f_9);
    __mmask8 m8_11 = (__mmask8)(g_m8[6] ^ 139);
    __mmask8 m8_12 = _kand_mask8(m8_11, m8_11);
    __m512i v512i_13 = _mm512_shuffle_i32x4(v512i_5, v512i_5, 0x1B);
    __m512 v512f_14 = _mm512_loadu_ps(fbuf);
    __m512 v512f_15 = _mm512_sqrt_ps(v512f_14);
    __m512i v512i_16 = _mm512_dpbusd_epi32(v512i_5, v512i_8, p3);
    __m256 v256f_17 = _mm256_div_ps(v256f_1, v256f_1);
    __m128i v128i_18 = _mm_or_si128(v128i_4, v128i_3);
    __m512i v512i_19 = _mm512_permutex2var_epi32(v512i_16, v512i_13, p3);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m8_12) & 7); i_20++) {
    __m512 v512f_21 = _mm512_xor_ps(v512f_15, v512f_15);
    __m256i v256i_22 = _mm256_add_epi32(p2, p2);
    __m128 v128f_23 = _mm_fnmadd_ps(v128f_9, v128f_10, v128f_9);
    __m256 v256f_24 = _mm256_sqrt_ps(v256f_1);
    }
    __m512i v512i_25 = _mm512_conflict_epi32(p3);
    __m512i v512i_26 = _mm512_add_epi64(v512i_5, v512i_19);
    __m256d v256d_27 = _mm256_fmadd_pd(p1, p1, p1);
    __mmask64 m64_28 = (__mmask64)(g_m64[5] ^ 238);
    __mmask64 m64_29 = _kor_mask64(m64_28, m64_28);
    __mmask16 m16_30 = _mm512_testn_epi32_mask(v512i_5, v512i_13);
    __m256i v256i_31 = _mm256_add_epi64(p2, p0);
    __m512d v512d_32 = g_v512d[3];
    __m512d v512d_33 = _mm512_max_pd(v512d_32, v512d_32);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_30) & 7); i_34++) {
    __m512i v512i_35 = _mm512_or_si512(v512i_26, v512i_13);
    __m512h v512h_36 = _mm512_add_ph(v512h_6, v512h_6);
    __m512i v512i_37 = _mm512_cvtph_epi16(v512h_6);
    }
    g_v512i[1] = v512i_25;
    g_v256f[3] = v256f_17;
    g_v256d[9] = v256d_27;
    g_v256i[10] = p2;
    g_v512h[7] = v512h_6;
    return p0;
}

__m256h torture_200(__m256d p0, __m128h p1, __m256 p2) {
    __m128i v128i_1 = g_v128i[2];
    __m128i v128i_2 = _mm_add_epi8(v128i_1, v128i_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_div_pd(v512d_3, v512d_3);
    __m256d v256d_5 = _mm256_div_pd(p0, p0);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_add_epi64(v512i_6, v512i_6);
    __m128i v128i_8 = _mm_slli_epi32(v128i_2, 3);
    __m512h v512h_9 = _mm512_cvtepi16_ph(v512i_7);
    __m256 v256f_10 = _mm256_div_ps(p2, p2);
    __m512h v512h_11 = _mm512_rcp_ph(v512h_9);
    __m512 v512f_12 = g_v512f[9];
    __m512 v512f_13 = _mm512_max_ps(v512f_12, v512f_12);
    __m512i v512i_14 = _mm512_cvtepi8_epi32(v128i_8);
    __mmask16 m16_15 = (__mmask16)(g_m16[1] ^ 165);
    __mmask16 m16_16 = _kxor_mask16(m16_15, m16_15);
    __m512i v512i_17 = _mm512_shuffle_epi8(v512i_6, v512i_14);
    __m128i v128i_18 = _mm_add_epi8(v128i_8, v128i_1);
    __m512 v512f_19 = _mm512_rcp14_ps(v512f_13);
    __m256i v256i_20 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_21 = _mm256_abs_epi32(v256i_20);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512 v512f_22 = _mm512_sqrt_ps(v512f_13);
    __mmask32 m32_23 = (__mmask32)(g_m32[5] ^ 209);
    __mmask32 m32_24 = _kxor_mask32(m32_23, m32_23);
    __m128d v128d_25 = g_v128d[10];
    __m128d v128d_26 = _mm_add_pd(v128d_25, v128d_25);
    }
    __m512 v512f_27 = _mm512_add_ps(v512f_19, v512f_12);
    __mmask16 m16_28 = _mm512_test_epi32_mask(v512i_17, v512i_6);
    __mmask16 m16_29 = _mm512_cmp_ps_mask(v512f_27, v512f_13, _CMP_LT_OQ);
    __mmask32 m32_30 = _mm512_cmp_ph_mask(v512h_9, v512h_9, 1);
    __m512i v512i_31 = _mm512_add_epi32(v512i_7, v512i_6);
    __mmask16 m16_32 = _kshiftri_mask16(m16_28, 1);
    __m512h v512h_33 = _mm512_rcp_ph(v512h_9);
    __m256d v256d_34 = _mm256_div_pd(p0, p0);
    g_v512f[3] = v512f_27;
    g_v256i[5] = v256i_21;
    g_v128i[5] = v128i_1;
    g_m16[9] = m16_28;
    __m256h v256h_35 = g_v256h[3];
    return v256h_35;
}

__mmask16 torture_201(__m256i p0, __m128h p1, __m512d p2, __m512d p3) {
    __m512 v512f_1 = g_v512f[6];
    __m512 v512f_2 = _mm512_rcp14_ps(v512f_1);
    __m512 v512f_3 = _mm512_and_ps(v512f_2, v512f_1);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_shuffle_epi8(v512i_4, v512i_4);
    __mmask16 m16_6 = _mm512_testn_epi32_mask(v512i_4, v512i_5);
    __m512i v512i_7 = _mm512_unpacklo_epi32(v512i_4, v512i_4);
    __m512i v512i_8 = _mm512_popcnt_epi64(v512i_4);
    __m128d v128d_9 = g_v128d[3];
    __m128d v128d_10 = _mm_add_pd(v128d_9, v128d_9);
    __m512 v512f_11 = _mm512_fnmadd_ps(v512f_2, v512f_2, v512f_1);
    __m512h v512h_12 = g_v512h[0];
    __m512i v512i_13 = _mm512_cvtph_epi16(v512h_12);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_6) & 7); i_14++) {
    __m256i v256i_15 = _mm256_sllv_epi32(p0, p0);
    __m256d v256d_16 = _mm512_extractf64x4_pd(p2, 1);
    __m512i v512i_17 = _mm512_cvtpd_epi64(p3);
    }
    __m128i v128i_18 = g_v128i[2];
    __m128i v128i_19 = _mm_shuffle_epi8(v128i_18, v128i_18);
    __m512 v512f_20 = _mm512_div_ps(v512f_2, v512f_11);
    __m256 v256f_21 = _mm256_set1_ps(1.0f);
    __m512i v512i_22 = _mm512_broadcastmw_epi32(m16_6);
    __m512 v512f_23 = _mm512_roundscale_ps(v512f_1, 0x10);
    __m512 v512f_24 = _mm512_cvtepi32_ps(v512i_4);
    g_v128d[1] = v128d_10;
    g_v128i[2] = v128i_18;
    return m16_6;
}

__m512h torture_202(__m512h p0, __m256d p1, __m256i p2, __m512i p3, __m512 p4) {
    __m128i v128i_1 = g_v128i[2];
    __m128i v128i_2 = _mm_unpacklo_epi32(v128i_1, v128i_1);
    __m512h v512h_3 = _mm512_fmul_pch(p0, p0);
    __mmask16 m16_4 = (__mmask16)(g_m16[6] ^ 195);
    __m512 v512f_5 = _mm512_maskz_mul_ps(m16_4, p4, p4);
    __m512d v512d_6 = _mm512_loadu_pd(dbuf);
    __m512d v512d_7 = _mm512_min_pd(v512d_6, v512d_6);
    __m128 v128f_8 = g_v128f[6];
    __m128 v128f_9 = _mm_div_ps(v128f_8, v128f_8);
    __m512i v512i_10 = _mm512_shuffle_epi8(p3, p3);
    __m512 v512f_11 = _mm512_fmsub_ps(v512f_5, v512f_5, p4);
    __m512i v512i_12 = _mm512_madd52lo_epu64(v512i_10, v512i_10, p3);
    __m512 v512f_13 = _mm512_scalef_ps(v512f_11, v512f_5);
    __mmask64 m64_14 = (__mmask64)(g_m64[10] ^ 185);
    __mmask64 m64_15 = _kor_mask64(m64_14, m64_14);
    __m512h v512h_16 = _mm512_mul_ph(p0, v512h_3);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_4) & 7); i_17++) {
    __m512i v512i_18 = _mm512_xor_si512(v512i_10, v512i_12);
    __m512i v512i_19 = _mm512_permutexvar_epi32(v512i_12, p3);
    __m128 v128f_20 = _mm_sub_ps(v128f_9, v128f_8);
    __mmask16 m16_21 = _mm512_test_epi32_mask(v512i_12, v512i_12);
    __m128 v128f_22 = _mm_sub_ps(v128f_9, v128f_8);
    }
    __m512d v512d_23 = _mm512_mul_pd(v512d_7, v512d_6);
    __m256 v256f_24 = _mm512_cvtpd_ps(v512d_6);
    __m128d v128d_25 = g_v128d[9];
    __m128d v128d_26 = _mm_mul_pd(v128d_25, v128d_25);
    __mmask32 m32_27 = _mm512_kunpackw(m16_4, m16_4);
    __m128i v128i_28 = _mm_sub_epi64(v128i_1, v128i_1);
    __m512h v512h_29 = _mm512_fmul_pch(p0, v512h_3);
    g_v512d[7] = v512d_23;
    g_v256d[0] = p1;
    g_v128f[4] = v128f_8;
    g_v128i[9] = v128i_2;
    g_m64[8] = m64_14;
    return p0;
}

__m128i torture_203(__m256d p0, __m512h p1) {
    __m128i v128i_1 = g_v128i[9];
    __m128i v128i_2 = _mm_sllv_epi32(v128i_1, v128i_1);
    __m512i v512i_3 = _mm512_broadcast_i32x4(v128i_2);
    __m512 v512f_4 = g_v512f[0];
    __m512 v512f_5 = _mm512_shuffle_f32x4(v512f_4, v512f_4, 0x4E);
    __mmask16 m16_6 = _mm512_test_epi32_mask(v512i_3, v512i_3);
    __m512d v512d_7 = _mm512_loadu_pd(dbuf);
    __m512d v512d_8 = _mm512_max_pd(v512d_7, v512d_7);
    __m256 v256f_9 = _mm256_loadu_ps(fbuf);
    __m256 v256f_10 = _mm256_sqrt_ps(v256f_9);
    __m256i v256i_11 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_12 = _mm256_add_epi16(v256i_11, v256i_11);
    __m512i v512i_13 = _mm512_broadcastmw_epi32(m16_6);
    __m128i v128i_14 = _mm_slli_epi32(v128i_2, 3);
    __mmask16 m16_15 = _mm512_test_epi32_mask(v512i_13, v512i_13);
    __m512i v512i_16 = _mm512_permutexvar_epi32(v512i_3, v512i_3);
    __m256d v256d_17 = _mm256_div_pd(p0, p0);
    __mmask8 m8_18 = (__mmask8)(g_m8[9] ^ 165);
    __m512d v512d_19 = _mm512_mask_blend_pd(m8_18, v512d_8, v512d_7);
    __m256d v256d_20 = _mm256_sqrt_pd(v256d_17);
    __m256 v256f_21 = _mm512_cvtpd_ps(v512d_19);
    __m512 v512f_22 = _mm512_getexp_ps(v512f_4);
    __m512i v512i_23 = _mm512_unpacklo_epi32(v512i_16, v512i_3);
    __m256 v256f_24 = _mm256_fmsub_ps(v256f_10, v256f_10, v256f_10);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m256i v256i_25 = _mm256_shuffle_epi8(v256i_12, v256i_11);
    __m128i v128i_26 = _mm_sub_epi8(v128i_2, v128i_2);
    __m512i v512i_27 = _mm512_rol_epi32(v512i_16, 5);
    __m512 v512f_28 = _mm512_fmadd_ps(v512f_22, v512f_4, v512f_22);
    __m512i v512i_29 = _mm512_rolv_epi32(v512i_16, v512i_23);
    }
    __m128i v128i_30 = _mm_slli_epi32(v128i_1, 3);
    __mmask16 m16_31 = _mm512_testn_epi32_mask(v512i_16, v512i_23);
    __m256 v256f_32 = _mm256_fnmadd_ps(v256f_24, v256f_10, v256f_21);
    __m512i v512i_33 = _mm512_ternarylogic_epi32(v512i_3, v512i_16, v512i_16, 0x96);
    __m128 v128f_34 = _mm_loadu_ps(fbuf);
    __m128 v128f_35 = _mm_sub_ps(v128f_34, v128f_34);
    __m512i v512i_36 = _mm512_permutex2var_epi32(v512i_23, v512i_16, v512i_3);
    __mmask32 m32_37 = (__mmask32)(g_m32[9] ^ 203);
    __mmask32 m32_38 = _kxor_mask32(m32_37, m32_37);
    __m128d v128d_39 = g_v128d[0];
    __m128d v128d_40 = _mm_add_pd(v128d_39, v128d_39);
    __mmask16 m16_41 = _mm512_cmp_epi32_mask(v512i_13, v512i_23, 2);
    if (((unsigned long long)m16_41) & 1ULL) {
    __m512i v512i_42 = _mm512_rol_epi32(v512i_16, 5);
    __m128d v128d_43 = _mm_div_pd(v128d_39, v128d_40);
    __m128d v128d_44 = _mm_sqrt_pd(v128d_40);
    __m512 v512f_45 = _mm512_add_ps(v512f_22, v512f_4);
    __m128i v128i_46 = _mm_sub_epi64(v128i_2, v128i_1);
    }
    g_v512f[2] = v512f_22;
    g_v512i[0] = v512i_33;
    g_v256f[9] = v256f_21;
    g_v128f[7] = v128f_34;
    g_m8[7] = m8_18;
    return v128i_30;
}

__m512 torture_204(__m256h p0, __m256h p1, __m128d p2, __m512 p3, __m128i p4) {
    __m512i v512i_1 = g_v512i[9];
    __m512i v512i_2 = _mm512_permutex2var_epi32(v512i_1, v512i_1, v512i_1);
    __m512d v512d_3 = g_v512d[8];
    __m512d v512d_4 = _mm512_insertf64x2(v512d_3, p2, 3);
    __m256i v256i_5 = g_v256i[8];
    __m256i v256i_6 = _mm256_abs_epi32(v256i_5);
    __m512 v512f_7 = _mm512_scalef_ps(p3, p3);
    __mmask16 m16_8 = _mm512_fpclass_ps_mask(v512f_7, 0x3);
    __m512i v512i_9 = _mm512_and_si512(v512i_1, v512i_1);
    __m128 v128f_10 = _mm_set1_ps(1.0f);
    __mmask8 m8_11 = (__mmask8)(g_m8[4] ^ 182);
    __mmask8 m8_12 = _kand_mask8(m8_11, m8_11);
    __mmask16 m16_13 = _knot_mask16(m16_8);
    __m512d v512d_14 = _mm512_div_pd(v512d_3, v512d_4);
    __m256 v256f_15 = _mm256_loadu_ps(fbuf);
    __m256 v256f_16 = _mm256_shuffle_ps(v256f_15, v256f_15, 0x1B);
    __m256i v256i_17 = _mm256_mullo_epi32(v256i_5, v256i_5);
    __m256 v256f_18 = _mm512_cvtpd_ps(v512d_4);
    __m128 v128f_19 = _mm_sqrt_ps(v128f_10);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512i v512i_20 = _mm512_shuffle_epi8(v512i_9, v512i_1);
    __mmask64 m64_21 = (__mmask64)(g_m64[0] ^ 8);
    __mmask64 m64_22 = _kor_mask64(m64_21, m64_21);
    __m256 v256f_23 = _mm256_xor_ps(v256f_15, v256f_15);
    __m512d v512d_24 = _mm512_sub_pd(v512d_3, v512d_14);
    __m256 v256f_25 = _mm256_min_ps(v256f_18, v256f_18);
    __m512h v512h_26 = _mm512_cvtepi16_ph(v512i_20);
    }
    __m256 v256f_27 = _mm512_cvtpd_ps(v512d_14);
    __m512d v512d_28 = _mm512_add_pd(v512d_14, v512d_3);
    __m512i v512i_29 = _mm512_rolv_epi32(v512i_9, v512i_1);
    unsigned u32_30 = _cvtmask16_u32(m16_8);
    __m512 v512f_31 = _mm512_shuffle_f32x4(v512f_7, v512f_7, 0x4E);
    __m512 v512f_32 = _mm512_max_ps(v512f_31, v512f_31);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_13) & 7); i_33++) {
    __m512i v512i_34 = _mm512_sllv_epi32(v512i_1, v512i_1);
    __m128 v128f_35 = _mm_sub_ps(v128f_10, v128f_19);
    }
    g_v512f[8] = v512f_7;
    g_v512d[11] = v512d_4;
    g_v512i[2] = v512i_1;
    g_v256f[2] = v256f_27;
    g_v128d[9] = p2;
    g_v256h[4] = p0;
    return v512f_32;
}

__m512i torture_205(__m512i p0, __m256h p1, __m256d p2) {
    __m256d v256d_1 = _mm256_div_pd(p2, p2);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_3 = _mm512_fpclass_ps_mask(v512f_2, 0x3);
    __mmask32 m32_4 = (__mmask32)(g_m32[9] ^ 229);
    __mmask32 m32_5 = _kxor_mask32(m32_4, m32_4);
    __m512 v512f_6 = _mm512_fmadd_ps(v512f_2, v512f_2, v512f_2);
    __m512d v512d_7 = g_v512d[6];
    __m512d v512d_8 = _mm512_min_pd(v512d_7, v512d_7);
    __m256i v256i_9 = g_v256i[9];
    __m256i v256i_10 = _mm256_abs_epi32(v256i_9);
    __m256 v256f_11 = g_v256f[1];
    __m256 v256f_12 = _mm256_and_ps(v256f_11, v256f_11);
    __m512d v512d_13 = _mm512_fmadd_pd(v512d_7, v512d_7, v512d_8);
    __m512i v512i_14 = _mm512_dpwssd_epi32(p0, p0, p0);
    __m512i v512i_15 = _mm512_add_epi8(p0, v512i_14);
    __mmask64 m64_16 = _mm512_cmp_epi8_mask(v512i_14, v512i_14, 1);
    __m128i v128i_17 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_18 = _mm_sub_epi8(v128i_17, v128i_17);
    __m512i v512i_19 = _mm512_mask_blend_epi32(m16_3, v512i_14, v512i_14);
    __mmask16 m16_20 = _mm512_movepi32_mask(v512i_14);
    __m512 v512f_21 = _mm512_set1_ps(1.0f);
    if (((unsigned long long)m16_20) & 1ULL) {
    __m256d v256d_22 = _mm512_extractf64x4_pd(v512d_7, 1);
    __m512i v512i_23 = _mm512_mask_add_epi32(v512i_15, m16_3, v512i_15, v512i_14);
    __mmask32 m32_24 = _kxor_mask32(m32_5, m32_5);
    __m256h v256h_25 = _mm512_cvtxps_ph(v512f_21);
    __m512 v512f_26 = _mm512_add_ps(v512f_6, v512f_2);
    }
    __m256i v256i_27 = _mm256_sub_epi64(v256i_9, v256i_10);
    __m128d v128d_28 = g_v128d[0];
    __m128d v128d_29 = _mm_add_pd(v128d_28, v128d_28);
    __mmask16 m16_30 = _kor_mask16(m16_20, m16_3);
    __m256i v256i_31 = _mm256_abs_epi32(v256i_10);
    __m128 v128f_32 = g_v128f[3];
    __m128 v128f_33 = _mm_fmadd_ps(v128f_32, v128f_32, v128f_32);
    __m512i v512i_34 = _mm512_rol_epi32(p0, 5);
    __m512 v512f_35 = _mm512_mul_ps(v512f_2, v512f_2);
    __m512i v512i_36 = _mm512_popcnt_epi64(v512i_15);
    __m256d v256d_37 = _mm256_unpackhi_pd(v256d_1, v256d_1);
    __m512i v512i_38 = _mm512_shuffle_epi8(p0, p0);
    g_v512i[11] = v512i_15;
    g_v256f[5] = v256f_11;
    g_v256d[5] = v256d_37;
    g_v128d[1] = v128d_28;
    g_m32[3] = m32_4;
    g_m64[5] = m64_16;
    return v512i_14;
}

__m512i torture_206(__m128h p0, __m256 p1, __m256h p2, __m512i p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[3] ^ 213);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_maskz_mul_ps(m16_1, v512f_2, v512f_2);
    __m512d v512d_4 = _mm512_set1_pd(2.0);
    __m256i v256i_5 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_6 = _mm256_mullo_epi32(v256i_5, v256i_5);
    __m256i v256i_7 = _mm256_add_epi64(v256i_5, v256i_6);
    __m512d v512d_8 = _mm512_unpackhi_pd(v512d_4, v512d_4);
    __m512 v512f_9 = _mm512_set1_ps(1.0f);
    __m512d v512d_10 = _mm512_fmadd_pd(v512d_8, v512d_8, v512d_4);
    __m512d v512d_11 = _mm512_max_pd(v512d_10, v512d_4);
    __m256 v256f_12 = _mm256_min_ps(p1, p1);
    __mmask16 m16_13 = _kor_mask16(m16_1, m16_1);
    __m512 v512f_14 = _mm512_rcp14_ps(v512f_9);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_13) & 7); i_15++) {
    __m128d v128d_16 = g_v128d[4];
    __m128d v128d_17 = _mm_unpackhi_pd(v128d_16, v128d_16);
    __mmask16 m16_18 = _kor_mask16(m16_13, m16_13);
    __m128i v128i_19 = g_v128i[2];
    __m512i v512i_20 = _mm512_broadcast_i32x4(v128i_19);
    __m256 v256f_21 = _mm256_max_ps(v256f_12, v256f_12);
    }
    __m128d v128d_22 = g_v128d[10];
    __m128d v128d_23 = _mm_div_pd(v128d_22, v128d_22);
    __m512 v512f_24 = _mm512_mask_blend_ps(m16_1, v512f_14, v512f_14);
    __m256 v256f_25 = _mm256_fnmadd_ps(v256f_12, v256f_12, p1);
    __m128i v128i_26 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_27 = _mm_or_si128(v128i_26, v128i_26);
    __m512 v512f_28 = _mm512_div_ps(v512f_9, v512f_14);
    __m256i v256i_29 = _mm256_sllv_epi32(v256i_6, v256i_6);
    __m512h v512h_30 = g_v512h[6];
    __m512h v512h_31 = _mm512_sqrt_ph(v512h_30);
    __m128i v128i_32 = _mm_slli_epi32(v128i_26, 3);
    __m512i v512i_33 = _mm512_unpacklo_epi32(p3, p3);
    __m512 v512f_34 = _mm512_rcp14_ps(v512f_14);
    __m512 v512f_35 = _mm512_sqrt_ps(v512f_24);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_1) & 7); i_36++) {
    __m512i v512i_37 = _mm512_sub_epi16(p3, v512i_33);
    __m128 v128f_38 = _mm_loadu_ps(fbuf);
    __m128 v128f_39 = _mm_and_ps(v128f_38, v128f_38);
    }
    g_v512i[4] = v512i_33;
    g_v256f[9] = v256f_12;
    g_v128d[5] = v128d_22;
    g_v128h[4] = p0;
    return v512i_33;
}

__m256d torture_207(__m512d p0, __m512h p1, __m128 p2, __m512i p3, __m128 p4) {
    __m128h v128h_1 = g_v128h[7];
    __m512d v512d_2 = _mm512_cvtph_pd(v128h_1);
    __m256d v256d_3 = g_v256d[6];
    __m256d v256d_4 = _mm256_fmadd_pd(v256d_3, v256d_3, v256d_3);
    __m512i v512i_5 = _mm512_set1_epi32(0x55);
    __mmask64 m64_6 = (__mmask64)(g_m64[7] ^ 50);
    __mmask64 m64_7 = _kor_mask64(m64_6, m64_6);
    __m512i v512i_8 = _mm512_dpbusd_epi32(v512i_5, p3, v512i_5);
    __m256 v256f_9 = g_v256f[7];
    __m256 v256f_10 = _mm256_xor_ps(v256f_9, v256f_9);
    __m512i v512i_11 = _mm512_sub_epi8(p3, v512i_8);
    __m128 v128f_12 = _mm_shuffle_ps(p4, p2, 0x1B);
    __m512i v512i_13 = _mm512_cvtpd_epi64(p0);
    __mmask16 m16_14 = (__mmask16)(g_m16[1] ^ 126);
    __m512i v512i_15 = _mm512_mask_blend_epi32(m16_14, v512i_13, v512i_5);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512 v512f_16 = _mm512_loadu_ps(fbuf);
    __m512 v512f_17 = _mm512_rsqrt14_ps(v512f_16);
    __m512 v512f_18 = _mm512_xor_ps(v512f_17, v512f_17);
    __m512 v512f_19 = _mm512_scalef_ps(v512f_17, v512f_17);
    __m512i v512i_20 = _mm512_abs_epi32(v512i_8);
    __m512i v512i_21 = _mm512_cvtpd_epi64(p0);
    }
    __m256d v256d_22 = _mm256_add_pd(v256d_4, v256d_3);
    __m256 v256f_23 = _mm256_sub_ps(v256f_10, v256f_9);
    __m512 v512f_24 = g_v512f[8];
    __m512 v512f_25 = _mm512_scalef_ps(v512f_24, v512f_24);
    __m256d v256d_26 = _mm256_sub_pd(v256d_3, v256d_3);
    __mmask8 m8_27 = _mm512_cmp_pd_mask(v512d_2, v512d_2, _CMP_GE_OQ);
    __m512 v512f_28 = _mm512_shuffle_ps(v512f_25, v512f_24, 0x1B);
    __m256d v256d_29 = _mm256_max_pd(v256d_3, v256d_22);
    __m256 v256f_30 = _mm256_div_ps(v256f_23, v256f_9);
    __m256 v256f_31 = _mm256_sqrt_ps(v256f_9);
    __m512i v512i_32 = _mm512_sllv_epi32(v512i_15, v512i_11);
    __m256i v256i_33 = g_v256i[3];
    __m256i v256i_34 = _mm256_shuffle_epi8(v256i_33, v256i_33);
    __m128i v128i_35 = g_v128i[10];
    __m128i v128i_36 = _mm_slli_epi32(v128i_35, 3);
    for (int i_37 = 0; i_37 < (int)(((unsigned long long)m16_14) & 7); i_37++) {
    __m512 v512f_38 = _mm512_mask_add_ps(v512f_24, m16_14, v512f_24, v512f_24);
    __m512i v512i_39 = _mm512_mullo_epi32(v512i_8, v512i_8);
    __m256 v256f_40 = _mm256_fmsub_ps(v256f_9, v256f_30, v256f_23);
    }
    g_v512f[1] = v512f_28;
    g_v512i[1] = v512i_8;
    g_v256f[11] = v256f_9;
    g_v256i[1] = v256i_34;
    g_v128i[5] = v128i_36;
    g_m16[0] = m16_14;
    return v256d_22;
}

__m256d torture_208(__m256i p0, __m256 p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_lzcnt_epi32(v512i_1);
    __m512h v512h_3 = _mm512_cvtepi16_ph(v512i_1);
    __m256i v256i_4 = _mm256_sub_epi16(p0, p0);
    __m128i v128i_5 = g_v128i[2];
    __m128i v128i_6 = _mm_add_epi8(v128i_5, v128i_5);
    __m256i v256i_7 = _mm256_set1_epi32(0x55);
    __m256i v256i_8 = _mm256_sllv_epi32(v256i_4, p0);
    __m256 v256f_9 = _mm256_shuffle_ps(p1, p1, 0x1B);
    __m512h v512h_10 = _mm512_fmadd_ph(v512h_3, v512h_3, v512h_3);
    __m256 v256f_11 = _mm256_fmadd_ps(p1, v256f_9, p1);
    __m256i v256i_12 = _mm256_sllv_epi32(v256i_4, v256i_7);
    __m256i v256i_13 = _mm256_xor_si256(v256i_4, v256i_8);
    __m128d v128d_14 = g_v128d[1];
    __m128d v128d_15 = _mm_max_pd(v128d_14, v128d_14);
    __m256 v256f_16 = _mm256_mul_ps(p1, v256f_9);
    __m512 v512f_17 = g_v512f[11];
    __m512 v512f_18 = _mm512_rsqrt14_ps(v512f_17);
    __mmask8 m8_19 = (__mmask8)(g_m8[10] ^ 37);
    __m512i v512i_20 = _mm512_broadcastmb_epi64(m8_19);
    __m256 v256f_21 = _mm256_div_ps(v256f_11, v256f_16);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m8_19) & 7); i_22++) {
    __m256d v256d_23 = g_v256d[1];
    __m256d v256d_24 = _mm256_fmadd_pd(v256d_23, v256d_23, v256d_23);
    __mmask16 m16_25 = (__mmask16)(g_m16[8] ^ 30);
    __mmask16 m16_26 = _knot_mask16(m16_25);
    __m512 v512f_27 = _mm512_unpacklo_ps(v512f_18, v512f_17);
    }
    __m512 v512f_28 = _mm512_min_ps(v512f_17, v512f_18);
    __m512i v512i_29 = _mm512_or_si512(v512i_1, v512i_20);
    __m256d v256d_30 = g_v256d[7];
    __m256d v256d_31 = _mm256_fmadd_pd(v256d_30, v256d_30, v256d_30);
    __m128i v128i_32 = _mm_add_epi64(v128i_6, v128i_6);
    __m128d v128d_33 = _mm_max_pd(v128d_14, v128d_15);
    __mmask16 m16_34 = (__mmask16)(g_m16[2] ^ 177);
    __mmask16 m16_35 = _knot_mask16(m16_34);
    __m512d v512d_36 = _mm512_loadu_pd(dbuf);
    __m512d v512d_37 = _mm512_sub_pd(v512d_36, v512d_36);
    __m512d v512d_38 = _mm512_shuffle_f64x2(v512d_36, v512d_36, 0xEE);
    __m128d v128d_39 = _mm_unpackhi_pd(v128d_15, v128d_15);
    __m128 v128f_40 = _mm_loadu_ps(fbuf);
    __m128 v128f_41 = _mm_mul_ps(v128f_40, v128f_40);
    for (int i_42 = 0; i_42 < (int)(((unsigned long long)m16_34) & 7); i_42++) {
    __m256 v256f_43 = _mm256_shuffle_ps(p1, v256f_21, 0x1B);
    __m256i v256i_44 = _mm256_and_si256(v256i_4, v256i_7);
    __m512 v512f_45 = _mm512_xor_ps(v512f_18, v512f_18);
    __m128 v128f_46 = _mm_unpacklo_ps(v128f_41, v128f_40);
    }
    g_v512i[3] = v512i_20;
    g_v256d[8] = v256d_31;
    g_v128f[8] = v128f_40;
    g_v512h[9] = v512h_10;
    g_m16[4] = m16_35;
    return v256d_31;
}

__m128h torture_209(__m256d p0, __m256h p1, __m128d p2) {
    __m128 v128f_1 = g_v128f[10];
    __m128 v128f_2 = _mm_fnmadd_ps(v128f_1, v128f_1, v128f_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_4 = _mm512_testn_epi32_mask(v512i_3, v512i_3);
    __m512h v512h_5 = g_v512h[9];
    __m512i v512i_6 = _mm512_cvtph_epi16(v512h_5);
    __m128 v128f_7 = _mm_fmsub_ps(v128f_2, v128f_1, v128f_2);
    __m256i v256i_8 = g_v256i[8];
    __m256i v256i_9 = _mm256_slli_epi32(v256i_8, 3);
    __m128 v128f_10 = _mm_sub_ps(v128f_2, v128f_1);
    __mmask16 m16_11 = _kand_mask16(m16_4, m16_4);
    __m128i v128i_12 = g_v128i[1];
    __m128i v128i_13 = _mm_add_epi16(v128i_12, v128i_12);
    __mmask8 m8_14 = (__mmask8)(g_m8[5] ^ 220);
    __mmask16 m16_15 = _mm512_kunpackb(m8_14, m8_14);
    __m128 v128f_16 = _mm_max_ps(v128f_7, v128f_7);
    __m256i v256i_17 = _mm256_sub_epi64(v256i_9, v256i_9);
    __m512i v512i_18 = _mm512_cvtph_epi16(v512h_5);
    __m128 v128f_19 = _mm_shuffle_ps(v128f_7, v128f_2, 0x1B);
    __m512i v512i_20 = _mm512_rol_epi32(v512i_18, 5);
    __m512 v512f_21 = _mm512_loadu_ps(fbuf);
    __m512 v512f_22 = _mm512_and_ps(v512f_21, v512f_21);
    __m128 v128f_23 = _mm512_extractf32x4_ps(v512f_21, 2);
    __m512 v512f_24 = _mm512_xor_ps(v512f_22, v512f_22);
    __m512i v512i_25 = _mm512_and_si512(v512i_18, v512i_6);
    __m512i v512i_26 = _mm512_sub_epi32(v512i_20, v512i_3);
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m512d v512d_28 = _mm512_cvtps_pd(v256f_27);
    __m512i v512i_29 = _mm512_and_si512(v512i_18, v512i_18);
    __m256d v256d_30 = _mm256_sqrt_pd(p0);
    __m128d v128d_31 = _mm_min_pd(p2, p2);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m128i v128i_32 = _mm_or_si128(v128i_12, v128i_12);
    __m128 v128f_33 = _mm_sub_ps(v128f_1, v128f_1);
    __m512h v512h_34 = _mm512_fmadd_ph(v512h_5, v512h_5, v512h_5);
    }
    g_v512d[6] = v512d_28;
    g_v512i[2] = v512i_25;
    g_v256f[10] = v256f_27;
    g_v256d[5] = v256d_30;
    g_v128f[11] = v128f_2;
    g_v256h[4] = p1;
    g_m8[11] = m8_14;
    __m128h v128h_35 = g_v128h[11];
    return v128h_35;
}

__mmask8 torture_210(__m128d p0, __m256i p1, __m512d p2) {
    __m512 v512f_1 = g_v512f[2];
    __mmask16 m16_2 = (__mmask16)(g_m16[0] ^ 204);
    __m512 v512f_3 = _mm512_mask_add_ps(v512f_1, m16_2, v512f_1, v512f_1);
    __m512h v512h_4 = g_v512h[5];
    __m512h v512h_5 = _mm512_add_ph(v512h_4, v512h_4);
    __m128 v128f_6 = _mm_set1_ps(1.0f);
    __mmask16 m16_7 = _knot_mask16(m16_2);
    __m512 v512f_8 = _mm512_getexp_ps(v512f_3);
    __m256i v256i_9 = _mm256_sub_epi64(p1, p1);
    __m512i v512i_10 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_11 = _mm512_permutexvar_epi32(v512i_10, v512i_10);
    __m512i v512i_12 = _mm512_set1_epi32(0x55);
    unsigned u32_13 = _cvtmask16_u32(m16_7);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_7) & 7); i_14++) {
    __m512h v512h_15 = _mm512_cvtepi16_ph(v512i_10);
    __m512 v512f_16 = _mm512_add_ps(v512f_1, v512f_3);
    __m256 v256f_17 = g_v256f[11];
    __m256 v256f_18 = _mm256_fmadd_ps(v256f_17, v256f_17, v256f_17);
    __m512d v512d_19 = _mm512_sub_pd(p2, p2);
    __m128 v128f_20 = _mm_set1_ps(1.0f);
    }
    __m256 v256f_21 = g_v256f[1];
    __m256 v256f_22 = _mm256_and_ps(v256f_21, v256f_21);
    __m512i v512i_23 = _mm512_abs_epi32(v512i_10);
    __m256i v256i_24 = _mm256_sub_epi8(v256i_9, v256i_9);
    __m128 v128f_25 = _mm_add_ps(v128f_6, v128f_6);
    __mmask16 m16_26 = _kshiftri_mask16(m16_2, 1);
    __m256i v256i_27 = _mm256_sub_epi64(v256i_9, v256i_24);
    g_v512d[2] = p2;
    g_v512i[10] = v512i_10;
    g_v256f[11] = v256f_22;
    g_v256i[6] = v256i_27;
    g_v512h[2] = v512h_4;
    __mmask8 m8_28 = (__mmask8)(g_m8[4] ^ 211);
    return m8_28;
}

__m256d torture_211(__m512d p0, __m512d p1, __m128h p2) {
    __m128i v128i_1 = g_v128i[3];
    __m128i v128i_2 = _mm_sub_epi64(v128i_1, v128i_1);
    __m256 v256f_3 = _mm256_set1_ps(1.0f);
    __m512h v512h_4 = g_v512h[9];
    __m512h v512h_5 = _mm512_add_ph(v512h_4, v512h_4);
    __m512h v512h_6 = _mm512_getexp_ph(v512h_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[1] ^ 250);
    __mmask16 m16_8 = _kand_mask16(m16_7, m16_7);
    __m512i v512i_9 = g_v512i[9];
    __m512i v512i_10 = _mm512_xor_si512(v512i_9, v512i_9);
    __m256i v256i_11 = g_v256i[0];
    __m256i v256i_12 = _mm256_add_epi16(v256i_11, v256i_11);
    __m512i v512i_13 = _mm512_add_epi8(v512i_9, v512i_10);
    __m512 v512f_14 = g_v512f[1];
    __m128 v128f_15 = _mm512_extractf32x4_ps(v512f_14, 2);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_7) & 7); i_16++) {
    __m512h v512h_17 = _mm512_fmsub_ph(v512h_4, v512h_5, v512h_4);
    __m512d v512d_18 = _mm512_div_pd(p1, p1);
    __m256d v256d_19 = g_v256d[9];
    __m256d v256d_20 = _mm256_sub_pd(v256d_19, v256d_19);
    __m128 v128f_21 = _mm_mul_ps(v128f_15, v128f_15);
    }
    __mmask64 m64_22 = _mm512_cmp_epi8_mask(v512i_10, v512i_13, 1);
    __m512i v512i_23 = _mm512_add_epi32(v512i_10, v512i_13);
    __m128 v128f_24 = _mm_fnmadd_ps(v128f_15, v128f_15, v128f_15);
    __m512h v512h_25 = _mm512_getexp_ph(v512h_6);
    unsigned u32_26 = _cvtmask16_u32(m16_8);
    __m256i v256i_27 = _mm256_sub_epi64(v256i_12, v256i_12);
    __m512h v512h_28 = _mm512_getexp_ph(v512h_4);
    __m512 v512f_29 = _mm512_add_ps(v512f_14, v512f_14);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m256i v256i_30 = _mm256_unpacklo_epi32(v256i_12, v256i_12);
    __m128d v128d_31 = g_v128d[10];
    __m128d v128d_32 = _mm_div_pd(v128d_31, v128d_31);
    __m256i v256i_33 = _mm256_sllv_epi32(v256i_12, v256i_30);
    __mmask16 m16_34 = _mm512_fpclass_ps_mask(v512f_29, 0x3);
    __m512d v512d_35 = _mm512_set1_pd(2.0);
    __m512h v512h_36 = _mm512_fmul_pch(v512h_25, v512h_25);
    }
    g_v512f[5] = v512f_29;
    g_v512i[2] = v512i_13;
    g_v256f[6] = v256f_3;
    g_v512h[6] = v512h_5;
    __m256d v256d_37 = g_v256d[5];
    return v256d_37;
}

__m512 torture_212(__m512d p0, __m256i p1, __m512 p2, __m128i p3) {
    __m256i v256i_1 = _mm256_sub_epi8(p1, p1);
    __m128 v128f_2 = g_v128f[8];
    __m128 v128f_3 = _mm_min_ps(v128f_2, v128f_2);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_add_epi32(v512i_4, v512i_4);
    __m128 v128f_6 = _mm_fmadd_ps(v128f_3, v128f_3, v128f_2);
    __m512 v512f_7 = _mm512_rcp14_ps(p2);
    __m128 v128f_8 = _mm_sub_ps(v128f_6, v128f_2);
    __mmask64 m64_9 = (__mmask64)(g_m64[3] ^ 44);
    __mmask64 m64_10 = _kor_mask64(m64_9, m64_9);
    __m128 v128f_11 = _mm512_extractf32x4_ps(v512f_7, 2);
    __m256 v256f_12 = g_v256f[11];
    __m256 v256f_13 = _mm256_fmadd_ps(v256f_12, v256f_12, v256f_12);
    __m256i v256i_14 = _mm512_cvtepi64_epi32(v512i_5);
    __m128d v128d_15 = g_v128d[3];
    __m128d v128d_16 = _mm_max_pd(v128d_15, v128d_15);
    __mmask16 m16_17 = _mm512_test_epi32_mask(v512i_4, v512i_4);
    __m512h v512h_18 = g_v512h[6];
    __m512h v512h_19 = _mm512_fmul_pch(v512h_18, v512h_18);
    __m512i v512i_20 = _mm512_rol_epi32(v512i_4, 5);
    __m128 v128f_21 = _mm_and_ps(v128f_6, v128f_2);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m512d v512d_22 = _mm512_cvtps_pd(v256f_13);
    __m512 v512f_23 = _mm512_mask3_fmadd_ps(p2, v512f_7, p2, m16_17);
    __m512d v512d_24 = _mm512_sqrt_pd(p0);
    __m256i v256i_25 = _mm256_slli_epi32(v256i_14, 3);
    __m512i v512i_26 = _mm512_sub_epi16(v512i_4, v512i_4);
    }
    __m512i v512i_27 = _mm512_dpbusd_epi32(v512i_5, v512i_20, v512i_20);
    __m512i v512i_28 = _mm512_shuffle_i32x4(v512i_20, v512i_20, 0x1B);
    __m512i v512i_29 = _mm512_permutexvar_epi32(v512i_27, v512i_28);
    __mmask16 m16_30 = _mm512_test_epi32_mask(v512i_29, v512i_5);
    __m256i v256i_31 = _mm256_slli_epi32(p1, 3);
    __m256 v256f_32 = _mm256_mul_ps(v256f_13, v256f_12);
    __m128 v128f_33 = _mm_and_ps(v128f_11, v128f_21);
    __m512 v512f_34 = _mm512_mask3_fmadd_ps(p2, v512f_7, v512f_7, m16_17);
    if (((unsigned long long)m16_30) & 1ULL) {
    __m512 v512f_35 = _mm512_shuffle_f32x4(p2, p2, 0x4E);
    __m512i v512i_36 = _mm512_ternarylogic_epi32(v512i_28, v512i_5, v512i_28, 0x96);
    __m512 v512f_37 = _mm512_scalef_ps(v512f_34, p2);
    }
    g_v512f[6] = p2;
    g_v512d[8] = p0;
    g_v512i[5] = v512i_20;
    g_v128d[0] = v128d_16;
    g_v128i[8] = p3;
    g_v512h[2] = v512h_18;
    return p2;
}

__m256h torture_213(__m128 p0, __m512d p1) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_fnmadd_ps(v256f_1, v256f_1, v256f_1);
    __m256 v256f_3 = _mm256_and_ps(v256f_2, v256f_2);
    __m512h v512h_4 = g_v512h[4];
    __mmask32 m32_5 = _mm512_cmp_ph_mask(v512h_4, v512h_4, 1);
    __mmask32 m32_6 = _kxor_mask32(m32_5, m32_5);
    __m512 v512f_7 = g_v512f[4];
    __mmask16 m16_8 = (__mmask16)(g_m16[4] ^ 108);
    __m512 v512f_9 = _mm512_mask_add_ps(v512f_7, m16_8, v512f_7, v512f_7);
    __m256 v256f_10 = _mm256_fnmadd_ps(v256f_1, v256f_1, v256f_3);
    __m512 v512f_11 = _mm512_sqrt_ps(v512f_7);
    __m128d v128d_12 = _mm_set1_pd(2.0);
    __m128 v128f_13 = _mm_xor_ps(p0, p0);
    __m512i v512i_14 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_15 = _mm512_shuffle_epi8(v512i_14, v512i_14);
    __m512h v512h_16 = _mm512_mul_ph(v512h_4, v512h_4);
    __m256i v256i_17 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_18 = _mm256_add_epi32(v256i_17, v256i_17);
    __m128 v128f_19 = _mm_set1_ps(1.0f);
    __m512d v512d_20 = _mm512_max_pd(p1, p1);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512i v512i_21 = _mm512_set1_epi32(0x55);
    __m128i v128i_22 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_23 = _mm_sub_epi32(v128i_22, v128i_22);
    __mmask16 m16_24 = _kand_mask16(m16_8, m16_8);
    __m512i v512i_25 = _mm512_broadcast_i32x4(v128i_23);
    __m512 v512f_26 = _mm512_insertf32x8(v512f_11, v256f_10, 1);
    __m256 v256f_27 = _mm256_max_ps(v256f_3, v256f_10);
    }
    __m128i v128i_28 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_29 = _mm_sub_epi32(v128i_28, v128i_28);
    __m256i v256i_30 = _mm256_mullo_epi32(v256i_18, v256i_17);
    __m256i v256i_31 = _mm256_sub_epi32(v256i_18, v256i_17);
    __m512h v512h_32 = _mm512_cvtepi16_ph(v512i_15);
    __m512i v512i_33 = _mm512_rol_epi32(v512i_14, 5);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512d v512d_34 = _mm512_sub_pd(v512d_20, p1);
    __m128 v128f_35 = _mm_sqrt_ps(p0);
    __m512i v512i_36 = _mm512_abs_epi32(v512i_14);
    __m512d v512d_37 = _mm512_sub_pd(p1, v512d_34);
    }
    g_v512d[4] = p1;
    g_v512i[7] = v512i_14;
    g_v256f[7] = v256f_3;
    g_v128f[3] = p0;
    g_v128i[11] = v128i_29;
    g_v512h[2] = v512h_32;
    g_m32[2] = m32_6;
    __m256h v256h_38 = g_v256h[4];
    return v256h_38;
}

__mmask64 torture_214(__m256 p0, __m128d p1, __m256h p2, __m128i p3, __m128h p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_sub_epi8(v512i_1, v512i_1);
    __m256d v256d_3 = g_v256d[11];
    __m256d v256d_4 = _mm256_div_pd(v256d_3, v256d_3);
    __m256i v256i_5 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_6 = _mm256_slli_epi32(v256i_5, 3);
    __m128i v128i_7 = _mm_and_si128(p3, p3);
    __m128i v128i_8 = _mm_shuffle_epi8(p3, v128i_7);
    __m512 v512f_9 = g_v512f[3];
    __m512 v512f_10 = _mm512_div_ps(v512f_9, v512f_9);
    __mmask16 m16_11 = (__mmask16)(g_m16[5] ^ 91);
    __m512i v512i_12 = _mm512_mask_blend_epi32(m16_11, v512i_2, v512i_1);
    __mmask8 m8_13 = (__mmask8)(g_m8[4] ^ 83);
    __m512d v512d_14 = g_v512d[6];
    __m512d v512d_15 = _mm512_mask_blend_pd(m8_13, v512d_14, v512d_14);
    __m256d v256d_16 = _mm256_div_pd(v256d_4, v256d_3);
    __m256d v256d_17 = _mm256_sub_pd(v256d_4, v256d_3);
    __m512i v512i_18 = _mm512_broadcastmb_epi64(m8_13);
    __m256i v256i_19 = _mm512_cvtepi64_epi32(v512i_2);
    __m512 v512f_20 = _mm512_mask_blend_ps(m16_11, v512f_10, v512f_10);
    if (((unsigned long long)m16_11) & 1ULL) {
    __m512h v512h_21 = g_v512h[6];
    __m512h v512h_22 = _mm512_add_ph(v512h_21, v512h_21);
    __m512 v512f_23 = _mm512_unpacklo_ps(v512f_9, v512f_9);
    __m512h v512h_24 = _mm512_fmul_pch(v512h_22, v512h_21);
    }
    __m256 v256f_25 = _mm256_mul_ps(p0, p0);
    __m128i v128i_26 = _mm_slli_epi32(p3, 3);
    __m256 v256f_27 = _mm256_fmsub_ps(p0, p0, p0);
    __mmask16 m16_28 = _mm512_cmp_epu32_mask(v512i_2, v512i_2, 4);
    __mmask8 m8_29 = _kand_mask8(m8_13, m8_13);
    g_v512f[1] = v512f_9;
    g_v512d[1] = v512d_14;
    g_v256d[8] = v256d_17;
    g_v128i[1] = v128i_8;
    g_v256h[2] = p2;
    g_v128h[9] = p4;
    __mmask64 m64_30 = (__mmask64)(g_m64[2] ^ 29);
    return m64_30;
}

__mmask32 torture_215(__m512h p0, __m512 p1, __m256 p2) {
    __m512 v512f_1 = _mm512_fnmadd_ps(p1, p1, p1);
    __m512i v512i_2 = g_v512i[2];
    __m512i v512i_3 = _mm512_sub_epi32(v512i_2, v512i_2);
    __m256d v256d_4 = g_v256d[10];
    __m256d v256d_5 = _mm256_min_pd(v256d_4, v256d_4);
    __m512i v512i_6 = _mm512_ternarylogic_epi32(v512i_3, v512i_2, v512i_2, 0x96);
    __m128i v128i_7 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_8 = _mm_or_si128(v128i_7, v128i_7);
    __m512d v512d_9 = _mm512_set1_pd(2.0);
    __m512i v512i_10 = _mm512_rolv_epi32(v512i_2, v512i_3);
    __m128 v128f_11 = g_v128f[10];
    __m128 v128f_12 = _mm_fnmadd_ps(v128f_11, v128f_11, v128f_11);
    __m512i v512i_13 = _mm512_set1_epi32(0x55);
    __m512i v512i_14 = _mm512_rolv_epi32(v512i_6, v512i_10);
    __m128 v128f_15 = _mm_fmadd_ps(v128f_12, v128f_11, v128f_12);
    int s32_16 = 65;
    for (int i_17 = 0; i_17 < (int)((s32_16) & 7); i_17++) {
    __m256 v256f_18 = _mm256_mul_ps(p2, p2);
    __m128i v128i_19 = _mm_add_epi32(v128i_7, v128i_7);
    }
    __m512i v512i_20 = _mm512_add_epi16(v512i_14, v512i_6);
    __m512i v512i_21 = _mm512_permutexvar_epi32(v512i_13, v512i_13);
    __m128d v128d_22 = g_v128d[8];
    __m512d v512d_23 = _mm512_insertf64x2(v512d_9, v128d_22, 3);
    __m512h v512h_24 = _mm512_add_ph(p0, p0);
    __m512 v512f_25 = _mm512_cvtepi32_ps(v512i_21);
    __m512 v512f_26 = _mm512_rsqrt14_ps(v512f_25);
    for (int i_27 = 0; i_27 < (int)((s32_16) & 7); i_27++) {
    __m128 v128f_28 = _mm_fnmadd_ps(v128f_11, v128f_12, v128f_15);
    __m512 v512f_29 = _mm512_min_ps(v512f_26, p1);
    __m512d v512d_30 = _mm512_fmadd_pd(v512d_23, v512d_23, v512d_9);
    __m256i v256i_31 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_32 = _mm256_abs_epi32(v256i_31);
    }
    g_v512d[11] = v512d_23;
    g_v128f[10] = v128f_12;
    g_v128d[5] = v128d_22;
    g_v512h[3] = p0;
    __mmask32 m32_33 = (__mmask32)(g_m32[1] ^ 12);
    return m32_33;
}

__m512 torture_216(__m128 p0, __m512 p1, __m256i p2, __m128 p3) {
    __m256d v256d_1 = g_v256d[7];
    __m256d v256d_2 = _mm256_div_pd(v256d_1, v256d_1);
    __mmask8 m8_3 = (__mmask8)(g_m8[1] ^ 157);
    __m512d v512d_4 = _mm512_loadu_pd(dbuf);
    __m512d v512d_5 = _mm512_mask_blend_pd(m8_3, v512d_4, v512d_4);
    __m256 v256f_6 = g_v256f[2];
    __m256 v256f_7 = _mm256_min_ps(v256f_6, v256f_6);
    __m128 v128f_8 = _mm_max_ps(p3, p3);
    __m256d v256d_9 = _mm256_set1_pd(2.0);
    __m512i v512i_10 = g_v512i[6];
    __m512i v512i_11 = _mm512_ternarylogic_epi32(v512i_10, v512i_10, v512i_10, 0x96);
    __m128d v128d_12 = g_v128d[9];
    __m128d v128d_13 = _mm_div_pd(v128d_12, v128d_12);
    __m256i v256i_14 = _mm256_mullo_epi32(p2, p2);
    __m256i v256i_15 = _mm256_add_epi8(p2, p2);
    __m256 v256f_16 = _mm256_fmsub_ps(v256f_6, v256f_7, v256f_6);
    __m256 v256f_17 = _mm256_sub_ps(v256f_7, v256f_7);
    __m512i v512i_18 = _mm512_sllv_epi32(v512i_10, v512i_11);
    __mmask16 m16_19 = (__mmask16)(g_m16[1] ^ 156);
    unsigned u32_20 = _cvtmask16_u32(m16_19);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_19) & 7); i_21++) {
    __m256 v256f_22 = _mm256_fnmadd_ps(v256f_17, v256f_16, v256f_6);
    __m512h v512h_23 = g_v512h[9];
    __m512h v512h_24 = _mm512_add_ph(v512h_23, v512h_23);
    __m256d v256d_25 = _mm256_max_pd(v256d_9, v256d_1);
    __m128i v128i_26 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_27 = _mm_add_epi32(v128i_26, v128i_26);
    }
    __m256d v256d_28 = _mm256_add_pd(v256d_1, v256d_2);
    __m512 v512f_29 = _mm512_max_ps(p1, p1);
    __m512i v512i_30 = _mm512_xor_si512(v512i_10, v512i_11);
    __m256 v256f_31 = _mm256_fnmadd_ps(v256f_7, v256f_17, v256f_6);
    __m128 v128f_32 = _mm_min_ps(p3, p0);
    __m128d v128d_33 = _mm_sqrt_pd(v128d_12);
    __mmask16 m16_34 = _mm512_kunpackb(m8_3, m8_3);
    __m128 v128f_35 = _mm_min_ps(p3, v128f_32);
    __m256i v256i_36 = _mm256_abs_epi32(p2);
    unsigned u32_37 = _cvtmask16_u32(m16_34);
    for (int i_38 = 0; i_38 < (int)(((unsigned long long)m16_19) & 7); i_38++) {
    __m128i v128i_39 = _mm_set1_epi32(0x55);
    __mmask16 m16_40 = _mm512_kunpackb(m8_3, m8_3);
    }
    g_v256d[7] = v256d_9;
    g_v128f[0] = v128f_32;
    return p1;
}

__m512d torture_217(__m128 p0, __m256h p1, __m512d p2, __m512d p3) {
    __m128 v128f_1 = _mm_xor_ps(p0, p0);
    __m128d v128d_2 = g_v128d[10];
    __m128d v128d_3 = _mm_unpackhi_pd(v128d_2, v128d_2);
    __m512d v512d_4 = _mm512_max_pd(p3, p2);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_6 = _mm512_permutex2var_epi32(v512i_5, v512i_5, v512i_5);
    __m256d v256d_7 = _mm256_set1_pd(2.0);
    __m256i v256i_8 = g_v256i[10];
    __m256i v256i_9 = _mm256_add_epi32(v256i_8, v256i_8);
    __m512 v512f_10 = _mm512_loadu_ps(fbuf);
    __m512i v512i_11 = _mm512_cvtps_epi32(v512f_10);
    __m128 v128f_12 = _mm_fnmadd_ps(p0, p0, p0);
    __m512i v512i_13 = _mm512_dpbusd_epi32(v512i_5, v512i_5, v512i_5);
    __m256d v256d_14 = _mm256_sqrt_pd(v256d_7);
    __m256d v256d_15 = _mm256_unpackhi_pd(v256d_14, v256d_14);
    __mmask16 m16_16 = (__mmask16)(g_m16[8] ^ 85);
    __m512 v512f_17 = _mm512_mask_blend_ps(m16_16, v512f_10, v512f_10);
    __m128i v128i_18 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_19 = _mm_shuffle_epi8(v128i_18, v128i_18);
    __mmask64 m64_20 = _mm512_movepi8_mask(v512i_6);
    __m512i v512i_21 = _mm512_madd52lo_epu64(v512i_11, v512i_13, v512i_5);
    __m256i v256i_22 = _mm256_add_epi8(v256i_8, v256i_8);
    __m512 v512f_23 = _mm512_mask3_fmadd_ps(v512f_10, v512f_17, v512f_10, m16_16);
    __m128i v128i_24 = _mm_add_epi32(v128i_18, v128i_19);
    __mmask8 m8_25 = (__mmask8)(g_m8[5] ^ 242);
    __mmask16 m16_26 = _mm512_kunpackb(m8_25, m8_25);
    __m512i v512i_27 = _mm512_dpwssd_epi32(v512i_13, v512i_21, v512i_11);
    g_v512d[7] = p3;
    g_v256d[10] = v256d_14;
    g_v128i[8] = v128i_19;
    g_m8[5] = m8_25;
    g_m16[2] = m16_26;
    return p2;
}

__m512h torture_218(__m512h p0, __m512h p1) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_unpacklo_epi32(v256i_1, v256i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_mul_ps(v512f_3, v512f_3);
    __mmask16 m16_5 = _mm512_cmp_ps_mask(v512f_3, v512f_3, _CMP_LT_OQ);
    __m128i v128i_6 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_7 = _mm_unpacklo_epi32(v128i_6, v128i_6);
    __m512h v512h_8 = _mm512_getexp_ph(p0);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_10 = _mm512_mask_blend_epi32(m16_5, v512i_9, v512i_9);
    __m256d v256d_11 = g_v256d[7];
    __m256d v256d_12 = _mm256_unpackhi_pd(v256d_11, v256d_11);
    __m256d v256d_13 = _mm256_sub_pd(v256d_11, v256d_12);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_5) & 7); i_14++) {
    __m128i v128i_15 = _mm_add_epi32(v128i_6, v128i_6);
    __m512d v512d_16 = _mm512_loadu_pd(dbuf);
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_16, v512d_16, 0xEE);
    __m128 v128f_18 = _mm_loadu_ps(fbuf);
    __m128 v128f_19 = _mm_add_ps(v128f_18, v128f_18);
    }
    __m256h v256h_20 = _mm512_cvtxps_ph(v512f_4);
    __m512 v512f_21 = _mm512_fnmadd_ps(v512f_3, v512f_4, v512f_4);
    __m128d v128d_22 = g_v128d[8];
    __m128d v128d_23 = _mm_add_pd(v128d_22, v128d_22);
    __mmask16 m16_24 = _kshiftli_mask16(m16_5, 2);
    __m128 v128f_25 = _mm_loadu_ps(fbuf);
    __m128 v128f_26 = _mm_fmadd_ps(v128f_25, v128f_25, v128f_25);
    __m256 v256f_27 = g_v256f[5];
    __m256 v256f_28 = _mm256_sqrt_ps(v256f_27);
    __m256 v256f_29 = _mm256_sqrt_ps(v256f_27);
    __mmask16 m16_30 = _mm512_cmp_ps_mask(v512f_21, v512f_4, _CMP_LT_OQ);
    __m256i v256i_31 = _mm256_slli_epi32(v256i_2, 3);
    __m128i v128i_32 = _mm_slli_epi32(v128i_7, 3);
    g_v256d[8] = v256d_12;
    g_v128d[5] = v128d_23;
    g_v512h[5] = p0;
    g_v256h[5] = v256h_20;
    return p0;
}

__m512d torture_219(__m256d p0, __m128 p1) {
    __m256i v256i_1 = g_v256i[10];
    __m256i v256i_2 = _mm256_add_epi8(v256i_1, v256i_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[6] ^ 84);
    __mmask16 m16_4 = _kshiftri_mask16(m16_3, 1);
    __mmask16 m16_5 = _kshiftri_mask16(m16_4, 1);
    __m256d v256d_6 = _mm256_mul_pd(p0, p0);
    __m256i v256i_7 = _mm256_abs_epi32(v256i_2);
    __m128 v128f_8 = _mm_unpacklo_ps(p1, p1);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m512 v512f_10 = _mm512_fnmadd_ps(v512f_9, v512f_9, v512f_9);
    __m256 v256f_11 = g_v256f[8];
    __m256 v256f_12 = _mm256_max_ps(v256f_11, v256f_11);
    __m256i v256i_13 = _mm256_xor_si256(v256i_1, v256i_1);
    __m256i v256i_14 = _mm256_set1_epi32(0x55);
    __m256i v256i_15 = _mm256_add_epi64(v256i_1, v256i_13);
    __m128d v128d_16 = _mm_set1_pd(2.0);
    __m128d v128d_17 = _mm_add_pd(v128d_16, v128d_16);
    __m128 v128f_18 = _mm512_extractf32x4_ps(v512f_10, 2);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m128 v128f_19 = _mm_shuffle_ps(p1, v128f_8, 0x1B);
    __m512i v512i_20 = g_v512i[9];
    __m512i v512i_21 = _mm512_permutexvar_epi32(v512i_20, v512i_20);
    }
    __m128i v128i_22 = g_v128i[10];
    __m128i v128i_23 = _mm_slli_epi32(v128i_22, 3);
    __m128d v128d_24 = _mm_sqrt_pd(v128d_17);
    __m512 v512f_25 = _mm512_shuffle_f32x4(v512f_9, v512f_10, 0x4E);
    __m512 v512f_26 = _mm512_fmadd_ps(v512f_9, v512f_10, v512f_9);
    __m128i v128i_27 = _mm_set1_epi32(0x55);
    __m512d v512d_28 = g_v512d[5];
    __m512d v512d_29 = _mm512_fmadd_pd(v512d_28, v512d_28, v512d_28);
    __mmask32 m32_30 = _mm512_kunpackw(m16_4, m16_5);
    __m512h v512h_31 = g_v512h[2];
    __m512h v512h_32 = _mm512_rcp_ph(v512h_31);
    __m256 v256f_33 = _mm256_set1_ps(1.0f);
    __m256 v256f_34 = _mm256_and_ps(v256f_33, v256f_11);
    __m256i v256i_35 = _mm256_xor_si256(v256i_13, v256i_13);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_3) & 7); i_36++) {
    __m512 v512f_37 = _mm512_mul_ps(v512f_9, v512f_26);
    __m512d v512d_38 = _mm512_cvtps_pd(v256f_33);
    }
    g_v512f[5] = v512f_26;
    g_v256i[7] = v256i_13;
    g_v128i[9] = v128i_27;
    g_v512h[11] = v512h_32;
    g_m32[0] = m32_30;
    return v512d_28;
}

__m128h torture_220(__m256h p0, __m256i p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_and_ps(v512f_1, v512f_1);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_broadcast_f32x4(v128f_3);
    __m128i v128i_5 = g_v128i[7];
    __m512i v512i_6 = _mm512_cvtepi8_epi32(v128i_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[10] ^ 152);
    __mmask32 m32_8 = _mm512_kunpackw(m16_7, m16_7);
    __m512 v512f_9 = _mm512_shuffle_f32x4(v512f_2, v512f_2, 0x4E);
    unsigned u32_10 = 70;
    __mmask16 m16_11 = _cvtu32_mask16(u32_10);
    __m512i v512i_12 = _mm512_dpbusd_epi32(v512i_6, v512i_6, v512i_6);
    __mmask16 m16_13 = _mm512_movepi32_mask(v512i_12);
    __m512i v512i_14 = _mm512_dpbusd_epi32(v512i_12, v512i_12, v512i_6);
    __m128i v128i_15 = _mm_abs_epi32(v128i_5);
    __m256i v256i_16 = _mm256_unpacklo_epi32(p1, p1);
    if (((unsigned long long)m16_13) & 1ULL) {
    __m128d v128d_17 = g_v128d[9];
    __m128d v128d_18 = _mm_max_pd(v128d_17, v128d_17);
    __m512i v512i_19 = _mm512_sub_epi64(v512i_6, v512i_6);
    __m256d v256d_20 = g_v256d[7];
    __m256d v256d_21 = _mm256_mul_pd(v256d_20, v256d_20);
    }
    __m512d v512d_22 = g_v512d[2];
    __m512d v512d_23 = _mm512_shuffle_f64x2(v512d_22, v512d_22, 0xEE);
    __m256d v256d_24 = g_v256d[1];
    __m256d v256d_25 = _mm256_max_pd(v256d_24, v256d_24);
    __m512 v512f_26 = _mm512_sqrt_ps(v512f_9);
    __m512 v512f_27 = _mm512_cvtxph_ps(p0);
    __m128 v128f_28 = _mm_unpacklo_ps(v128f_3, v128f_3);
    __m512d v512d_29 = _mm512_mul_pd(v512d_23, v512d_23);
    __m128 v128f_30 = _mm_sqrt_ps(v128f_28);
    g_v512f[7] = v512f_27;
    g_v512i[5] = v512i_6;
    g_v256i[5] = v256i_16;
    g_v128i[4] = v128i_5;
    g_m32[5] = m32_8;
    __m128h v128h_31 = g_v128h[7];
    return v128h_31;
}

__m128h torture_221(__m512 p0, __m256d p1, __m128d p2, __m512i p3, __m128h p4) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m512d v512d_2 = _mm512_mul_pd(v512d_1, v512d_1);
    __m512i v512i_3 = _mm512_and_si512(p3, p3);
    __m512h v512h_4 = g_v512h[6];
    __m512h v512h_5 = _mm512_fmsub_ph(v512h_4, v512h_4, v512h_4);
    __m128d v128d_6 = _mm_add_pd(p2, p2);
    __m512i v512i_7 = _mm512_slli_epi32(p3, 3);
    __m128i v128i_8 = g_v128i[1];
    __m512i v512i_9 = _mm512_broadcast_i32x4(v128i_8);
    __m256 v256f_10 = _mm256_loadu_ps(fbuf);
    __m256 v256f_11 = _mm256_sub_ps(v256f_10, v256f_10);
    __m512d v512d_12 = _mm512_cvtph_pd(p4);
    __mmask64 m64_13 = _mm512_movepi8_mask(v512i_7);
    __m256i v256i_14 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_15 = _mm256_sub_epi16(v256i_14, v256i_14);
    __m512d v512d_16 = _mm512_sqrt_pd(v512d_12);
    __m256i v256i_17 = _mm256_add_epi32(v256i_14, v256i_15);
    if (((unsigned long long)m64_13) & 1ULL) {
    __m512d v512d_18 = _mm512_sqrt_pd(v512d_12);
    __m128d v128d_19 = _mm_mul_pd(v128d_6, p2);
    __m256i v256i_20 = _mm256_sub_epi8(v256i_17, v256i_15);
    __m256 v256f_21 = _mm256_mul_ps(v256f_11, v256f_10);
    __m512 v512f_22 = _mm512_shuffle_f32x4(p0, p0, 0x4E);
    __m256i v256i_23 = _mm256_mullo_epi32(v256i_15, v256i_15);
    }
    __m256i v256i_24 = _mm256_and_si256(v256i_17, v256i_15);
    __m512d v512d_25 = _mm512_shuffle_f64x2(v512d_1, v512d_1, 0xEE);
    __m512d v512d_26 = _mm512_unpackhi_pd(v512d_1, v512d_1);
    __m256d v256d_27 = _mm256_set1_pd(2.0);
    __m128i v128i_28 = _mm_or_si128(v128i_8, v128i_8);
    __m512 v512f_29 = _mm512_set1_ps(1.0f);
    __m512d v512d_30 = _mm512_unpackhi_pd(v512d_2, v512d_2);
    __m256d v256d_31 = _mm256_sqrt_pd(p1);
    if (((unsigned long long)m64_13) & 1ULL) {
    __m512i v512i_32 = _mm512_add_epi64(v512i_3, v512i_7);
    __m256i v256i_33 = _mm256_sub_epi16(v256i_15, v256i_17);
    }
    g_v512d[3] = v512d_12;
    g_v512i[0] = v512i_9;
    g_v256f[4] = v256f_11;
    g_v256d[8] = v256d_27;
    g_v128i[6] = v128i_8;
    g_v512h[9] = v512h_4;
    g_m64[1] = m64_13;
    return p4;
}

__m256 torture_222(__m128i p0, __m256 p1, __m512 p2) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m512d v512d_2 = _mm512_min_pd(v512d_1, v512d_1);
    __m256d v256d_3 = g_v256d[10];
    __m256d v256d_4 = _mm256_mul_pd(v256d_3, v256d_3);
    __m256i v256i_5 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_6 = _mm256_add_epi8(v256i_5, v256i_5);
    __m512i v512i_7 = g_v512i[9];
    __m512i v512i_8 = _mm512_sub_epi32(v512i_7, v512i_7);
    __m512h v512h_9 = _mm512_cvtepi16_ph(v512i_8);
    __m512 v512f_10 = _mm512_mul_ps(p2, p2);
    __m128d v128d_11 = g_v128d[8];
    __m512d v512d_12 = _mm512_insertf64x2(v512d_2, v128d_11, 3);
    __mmask16 m16_13 = (__mmask16)(g_m16[7] ^ 218);
    __mmask16 m16_14 = _kxor_mask16(m16_13, m16_13);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_14) & 7); i_15++) {
    __m128d v128d_16 = _mm_set1_pd(2.0);
    __m128i v128i_17 = _mm_sub_epi64(p0, p0);
    __m256i v256i_18 = _mm256_add_epi64(v256i_6, v256i_6);
    }
    __m128 v128f_19 = _mm_loadu_ps(fbuf);
    __m128 v128f_20 = _mm_shuffle_ps(v128f_19, v128f_19, 0x1B);
    __m256d v256d_21 = _mm256_sub_pd(v256d_3, v256d_3);
    __m256i v256i_22 = _mm256_slli_epi32(v256i_5, 3);
    __m128d v128d_23 = _mm_sub_pd(v128d_11, v128d_11);
    __m512i v512i_24 = _mm512_slli_epi32(v512i_7, 3);
    __m256i v256i_25 = _mm256_sub_epi8(v256i_6, v256i_6);
    __m256i v256i_26 = _mm256_add_epi16(v256i_6, v256i_5);
    __m512i v512i_27 = _mm512_conflict_epi32(v512i_24);
    __m128d v128d_28 = _mm_set1_pd(2.0);
    __m256 v256f_29 = _mm256_fmadd_ps(p1, p1, p1);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_14) & 7); i_30++) {
    __mmask16 m16_31 = _kshiftri_mask16(m16_13, 1);
    __m512 v512f_32 = _mm512_fnmadd_ps(v512f_10, p2, v512f_10);
    __m128 v128f_33 = _mm_sub_ps(v128f_19, v128f_20);
    }
    g_v256d[10] = v256d_3;
    g_v256i[9] = v256i_6;
    g_v128f[2] = v128f_20;
    g_v128i[7] = p0;
    return v256f_29;
}

__m128h torture_223(__m512 p0, __m256 p1, __m128 p2, __m512 p3, __m512i p4) {
    __m256 v256f_1 = _mm256_unpacklo_ps(p1, p1);
    __m512 v512f_2 = _mm512_div_ps(p3, p0);
    __m512i v512i_3 = _mm512_abs_epi32(p4);
    __mmask16 m16_4 = _mm512_cmp_epi32_mask(p4, p4, 2);
    __m512d v512d_5 = g_v512d[1];
    __m128h v128h_6 = _mm512_cvtpd_ph(v512d_5);
    __m512 v512f_7 = _mm512_rcp14_ps(p3);
    __m128i v128i_8 = _mm_set1_epi32(0x55);
    __m512 v512f_9 = _mm512_div_ps(v512f_2, v512f_2);
    __m512d v512d_10 = _mm512_sub_pd(v512d_5, v512d_5);
    __m512 v512f_11 = _mm512_and_ps(v512f_2, v512f_7);
    unsigned u32_12 = _cvtmask16_u32(m16_4);
    __m256i v256i_13 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_14 = _mm256_add_epi8(v256i_13, v256i_13);
    __m256 v256f_15 = _mm256_min_ps(v256f_1, v256f_1);
    __m256i v256i_16 = _mm256_abs_epi32(v256i_14);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_4) & 7); i_17++) {
    __m512h v512h_18 = g_v512h[0];
    __m512h v512h_19 = _mm512_add_ph(v512h_18, v512h_18);
    __m256i v256i_20 = _mm256_or_si256(v256i_14, v256i_14);
    __m512d v512d_21 = _mm512_sub_pd(v512d_10, v512d_5);
    __mmask64 m64_22 = _mm512_cmp_epi8_mask(v512i_3, v512i_3, 1);
    __m256i v256i_23 = _mm256_sub_epi16(v256i_16, v256i_13);
    }
    __m128i v128i_24 = _mm_sub_epi64(v128i_8, v128i_8);
    __mmask32 m32_25 = (__mmask32)(g_m32[3] ^ 238);
    __mmask32 m32_26 = _kxor_mask32(m32_25, m32_25);
    __m128i v128i_27 = _mm_or_si128(v128i_8, v128i_24);
    __m512i v512i_28 = _mm512_and_si512(v512i_3, v512i_3);
    __m512 v512f_29 = _mm512_mask3_fmadd_ps(p0, p3, v512f_9, m16_4);
    __m128 v128f_30 = _mm_fmsub_ps(p2, p2, p2);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_4) & 7); i_31++) {
    __m512i v512i_32 = _mm512_sub_epi32(p4, v512i_28);
    __m256 v256f_33 = _mm256_fmadd_ps(v256f_1, p1, v256f_1);
    __m512d v512d_34 = _mm512_add_pd(v512d_10, v512d_5);
    __m512i v512i_35 = _mm512_add_epi64(v512i_3, v512i_32);
    __m512h v512h_36 = _mm512_cvtepi16_ph(v512i_32);
    }
    g_v512d[4] = v512d_5;
    g_v512i[6] = p4;
    g_v256i[3] = v256i_14;
    g_v128i[7] = v128i_27;
    g_v128h[2] = v128h_6;
    g_m32[10] = m32_25;
    return v128h_6;
}

__m128d torture_224(__m512d p0, __m128 p1, __m256i p2, __m128d p3, __m512i p4) {
    __m512i v512i_1 = _mm512_mullo_epi32(p4, p4);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_3 = (__mmask16)(g_m16[2] ^ 147);
    __m512 v512f_4 = _mm512_mask3_fmadd_ps(v512f_2, v512f_2, v512f_2, m16_3);
    __m512i v512i_5 = _mm512_add_epi32(v512i_1, v512i_1);
    __m512h v512h_6 = g_v512h[5];
    __mmask32 m32_7 = _mm512_cmp_ph_mask(v512h_6, v512h_6, 1);
    __m128i v128i_8 = g_v128i[2];
    __m128i v128i_9 = _mm_slli_epi32(v128i_8, 3);
    __m128i v128i_10 = _mm_or_si128(v128i_9, v128i_9);
    __m512h v512h_11 = _mm512_add_ph(v512h_6, v512h_6);
    __m128d v128d_12 = _mm_unpackhi_pd(p3, p3);
    __mmask32 m32_13 = _kxor_mask32(m32_7, m32_7);
    __mmask32 m32_14 = _mm512_kunpackw(m16_3, m16_3);
    __m512i v512i_15 = _mm512_set1_epi32(0x55);
    __m512i v512i_16 = _mm512_lzcnt_epi32(v512i_5);
    __mmask16 m16_17 = _knot_mask16(m16_3);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_3) & 7); i_18++) {
    __mmask32 m32_19 = _mm512_cmp_ph_mask(v512h_6, v512h_6, 1);
    __m256i v256i_20 = _mm256_add_epi8(p2, p2);
    }
    __m512i v512i_21 = _mm512_mask_add_epi32(v512i_16, m16_17, v512i_1, v512i_5);
    __m256d v256d_22 = _mm256_set1_pd(2.0);
    __m128d v128d_23 = _mm_min_pd(p3, p3);
    __m128 v128f_24 = _mm_add_ps(p1, p1);
    __m128 v128f_25 = _mm_sqrt_ps(v128f_24);
    __m512 v512f_26 = _mm512_fnmadd_ps(v512f_4, v512f_2, v512f_4);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_3) & 7); i_27++) {
    __m512i v512i_28 = _mm512_mullo_epi32(v512i_5, v512i_15);
    __m128 v128f_29 = _mm512_extractf32x4_ps(v512f_2, 2);
    __m512h v512h_30 = _mm512_fmsub_ph(v512h_6, v512h_6, v512h_6);
    __mmask32 m32_31 = _mm512_cmp_ph_mask(v512h_6, v512h_11, 1);
    __m512 v512f_32 = _mm512_mul_ps(v512f_26, v512f_4);
    }
    g_v512d[0] = p0;
    g_v512i[8] = v512i_16;
    g_v128d[2] = p3;
    return v128d_12;
}

__mmask32 torture_225(__m128i p0, __m256 p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_rsqrt14_ps(v512f_1);
    __m512d v512d_3 = g_v512d[6];
    __m512d v512d_4 = _mm512_mul_pd(v512d_3, v512d_3);
    __m256i v256i_5 = _mm256_set1_epi32(0x55);
    __m512d v512d_6 = _mm512_shuffle_f64x2(v512d_4, v512d_3, 0xEE);
    __mmask8 m8_7 = (__mmask8)(g_m8[1] ^ 51);
    __mmask8 m8_8 = _kand_mask8(m8_7, m8_7);
    __m256 v256f_9 = _mm256_max_ps(p1, p1);
    __m512d v512d_10 = _mm512_unpackhi_pd(v512d_3, v512d_4);
    __mmask16 m16_11 = (__mmask16)(g_m16[7] ^ 85);
    __m512 v512f_12 = _mm512_mask_blend_ps(m16_11, v512f_1, v512f_1);
    __m512 v512f_13 = _mm512_max_ps(v512f_12, v512f_1);
    __m128 v128f_14 = g_v128f[7];
    __m128 v128f_15 = _mm_mul_ps(v128f_14, v128f_14);
    __m256d v256d_16 = g_v256d[6];
    __m256d v256d_17 = _mm256_div_pd(v256d_16, v256d_16);
    __m512h v512h_18 = g_v512h[7];
    __m512h v512h_19 = _mm512_fmsub_ph(v512h_18, v512h_18, v512h_18);
    g_v512f[0] = v512f_13;
    g_v512d[11] = v512d_10;
    g_v256d[10] = v256d_17;
    g_v256i[9] = v256i_5;
    g_v128i[9] = p0;
    g_v512h[5] = v512h_19;
    __mmask32 m32_20 = (__mmask32)(g_m32[5] ^ 2);
    return m32_20;
}

__m512d torture_226(__m128 p0, __m256i p1, __m128h p2, __m256i p3) {
    __m512 v512f_1 = g_v512f[2];
    __m128 v128f_2 = _mm512_extractf32x4_ps(v512f_1, 2);
    __m128i v128i_3 = g_v128i[1];
    __m128i v128i_4 = _mm_unpacklo_epi32(v128i_3, v128i_3);
    __m512i v512i_5 = _mm512_set1_epi32(0x55);
    __m512 v512f_6 = _mm512_and_ps(v512f_1, v512f_1);
    __mmask16 m16_7 = _mm512_movepi32_mask(v512i_5);
    __m512i v512i_8 = _mm512_shuffle_epi8(v512i_5, v512i_5);
    __m256i v256i_9 = _mm256_and_si256(p3, p3);
    __m512i v512i_10 = _mm512_abs_epi32(v512i_5);
    __m512h v512h_11 = g_v512h[4];
    __m512i v512i_12 = _mm512_cvtph_epi16(v512h_11);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m512 v512f_13 = _mm512_unpacklo_ps(v512f_6, v512f_1);
    __m512 v512f_14 = _mm512_cvtepi32_ps(v512i_10);
    __m128i v128i_15 = _mm_sllv_epi32(v128i_3, v128i_3);
    }
    __m128 v128f_16 = _mm_sub_ps(v128f_2, v128f_2);
    __m512i v512i_17 = _mm512_add_epi8(v512i_5, v512i_10);
    __m512i v512i_18 = _mm512_cvtps_epi32(v512f_1);
    __m512i v512i_19 = _mm512_sub_epi8(v512i_17, v512i_10);
    __mmask16 m16_20 = _kshiftli_mask16(m16_7, 2);
    unsigned u32_21 = _cvtmask16_u32(m16_7);
    __m512d v512d_22 = g_v512d[8];
    return v512d_22;
}

__mmask32 torture_227(__m512i p0, __m128i p1, __m128 p2, __m256i p3, __m512i p4) {
    __m128 v128f_1 = _mm_set1_ps(1.0f);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_getexp_ps(v512f_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[0] ^ 103);
    __m512i v512i_5 = _mm512_mask_blend_epi32(m16_4, p4, p0);
    __m256 v256f_6 = _mm256_loadu_ps(fbuf);
    __m256 v256f_7 = _mm256_div_ps(v256f_6, v256f_6);
    __mmask16 m16_8 = _mm512_movepi32_mask(p0);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m512d v512d_10 = _mm512_min_pd(v512d_9, v512d_9);
    __m512h v512h_11 = g_v512h[10];
    __m512i v512i_12 = _mm512_cvtph_epi16(v512h_11);
    __m256i v256i_13 = _mm256_sub_epi64(p3, p3);
    __m128 v128f_14 = _mm_and_ps(p2, v128f_1);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_4) & 7); i_15++) {
    __m512 v512f_16 = _mm512_mask_blend_ps(m16_4, v512f_3, v512f_2);
    __m128 v128f_17 = _mm_div_ps(v128f_14, v128f_1);
    __m256h v256h_18 = _mm512_cvtxps_ph(v512f_16);
    }
    __m128i v128i_19 = _mm_mullo_epi32(p1, p1);
    __m256d v256d_20 = g_v256d[11];
    __m256d v256d_21 = _mm256_sub_pd(v256d_20, v256d_20);
    __m128 v128f_22 = _mm_fnmadd_ps(v128f_14, v128f_14, v128f_1);
    __m256d v256d_23 = _mm256_sub_pd(v256d_21, v256d_20);
    __mmask64 m64_24 = _mm512_movepi8_mask(v512i_5);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m128 v128f_25 = _mm_set1_ps(1.0f);
    __m128i v128i_26 = _mm_set1_epi32(0x55);
    __m128 v128f_27 = _mm512_extractf32x4_ps(v512f_3, 2);
    __m128i v128i_28 = _mm512_cvtepi32_epi8(p4);
    }
    g_v256f[0] = v256f_7;
    g_v128f[1] = v128f_22;
    g_v512h[9] = v512h_11;
    g_m64[9] = m64_24;
    __mmask32 m32_29 = (__mmask32)(g_m32[11] ^ 239);
    return m32_29;
}

__m256 torture_228(__m128h p0, __m512 p1, __m256 p2) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_add_epi32(v256i_1, v256i_1);
    __m256i v256i_3 = _mm256_set1_epi32(0x55);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_5 = _mm512_cmp_epu32_mask(v512i_4, v512i_4, 4);
    __mmask16 m16_6 = _mm512_cmp_epi32_mask(v512i_4, v512i_4, 2);
    __m512 v512f_7 = _mm512_set1_ps(1.0f);
    __m512d v512d_8 = _mm512_cvtph_pd(p0);
    __mmask16 m16_9 = _kor_mask16(m16_5, m16_6);
    __m128h v128h_10 = _mm512_cvtpd_ph(v512d_8);
    __mmask64 m64_11 = _mm512_movepi8_mask(v512i_4);
    __m256 v256f_12 = _mm256_div_ps(p2, p2);
    __m512 v512f_13 = _mm512_add_ps(v512f_7, p1);
    __m256i v256i_14 = _mm256_add_epi64(v256i_3, v256i_3);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_6) & 7); i_15++) {
    __m512i v512i_16 = _mm512_set1_epi32(0x55);
    __m128 v128f_17 = _mm_loadu_ps(fbuf);
    __m128 v128f_18 = _mm_shuffle_ps(v128f_17, v128f_17, 0x1B);
    }
    __m256d v256d_19 = g_v256d[1];
    __m256d v256d_20 = _mm256_sub_pd(v256d_19, v256d_19);
    __m128d v128d_21 = g_v128d[0];
    __m128d v128d_22 = _mm_sqrt_pd(v128d_21);
    __m512d v512d_23 = _mm512_sqrt_pd(v512d_8);
    __m512 v512f_24 = _mm512_mask3_fmadd_ps(v512f_13, v512f_7, v512f_13, m16_6);
    __m128 v128f_25 = g_v128f[5];
    __m128 v128f_26 = _mm_xor_ps(v128f_25, v128f_25);
    __m512i v512i_27 = _mm512_rol_epi32(v512i_4, 5);
    unsigned u32_28 = _cvtmask16_u32(m16_9);
    __m512d v512d_29 = _mm512_cvtps_pd(p2);
    __mmask16 m16_30 = _mm512_test_epi32_mask(v512i_27, v512i_4);
    __m512 v512f_31 = _mm512_rcp14_ps(v512f_13);
    __m512 v512f_32 = _mm512_roundscale_ps(p1, 0x10);
    g_v512f[2] = v512f_32;
    g_v256f[3] = p2;
    g_v256d[11] = v256d_20;
    g_v128d[7] = v128d_22;
    return v256f_12;
}

__mmask64 torture_229(__m128d p0, __m128i p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_permutexvar_epi32(v512i_1, v512i_1);
    __m128i v128i_3 = _mm_mullo_epi32(p1, p1);
    __m512 v512f_4 = g_v512f[6];
    __mmask16 m16_5 = (__mmask16)(g_m16[6] ^ 190);
    __m512 v512f_6 = _mm512_mask_add_ps(v512f_4, m16_5, v512f_4, v512f_4);
    __m512h v512h_7 = _mm512_cvtepi16_ph(v512i_1);
    __m256i v256i_8 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_9 = _mm256_or_si256(v256i_8, v256i_8);
    __mmask16 m16_10 = _mm512_testn_epi32_mask(v512i_1, v512i_2);
    __m256i v256i_11 = _mm256_sllv_epi32(v256i_9, v256i_9);
    __m512 v512f_12 = _mm512_fnmadd_ps(v512f_4, v512f_4, v512f_4);
    __m512i v512i_13 = _mm512_add_epi32(v512i_2, v512i_1);
    __m256 v256f_14 = _mm256_loadu_ps(fbuf);
    __m256 v256f_15 = _mm256_sub_ps(v256f_14, v256f_14);
    __mmask32 m32_16 = _mm512_cmp_ph_mask(v512h_7, v512h_7, 1);
    __m128 v128f_17 = g_v128f[9];
    __m128 v128f_18 = _mm_fnmadd_ps(v128f_17, v128f_17, v128f_17);
    __m128 v128f_19 = _mm_sub_ps(v128f_17, v128f_18);
    __m128i v128i_20 = _mm_add_epi8(v128i_3, v128i_3);
    __m128i v128i_21 = _mm_add_epi32(v128i_20, p1);
    __m256 v256f_22 = _mm256_min_ps(v256f_14, v256f_15);
    __m512 v512f_23 = _mm512_mul_ps(v512f_12, v512f_6);
    __m512 v512f_24 = _mm512_min_ps(v512f_12, v512f_23);
    __m512i v512i_25 = _mm512_broadcastmw_epi32(m16_10);
    __m128i v128i_26 = _mm_add_epi32(p1, v128i_20);
    __m512i v512i_27 = _mm512_conflict_epi32(v512i_1);
    for (int i_28 = 0; i_28 < (int)(((unsigned long long)m16_10) & 7); i_28++) {
    __m128 v128f_29 = _mm_and_ps(v128f_19, v128f_17);
    __mmask16 m16_30 = _mm512_cmp_epu32_mask(v512i_2, v512i_1, 4);
    __m256i v256i_31 = _mm256_mullo_epi32(v256i_8, v256i_11);
    __m256h v256h_32 = _mm512_cvtxps_ph(v512f_6);
    }
    g_v512i[5] = v512i_2;
    g_v256i[10] = v256i_11;
    g_v128f[11] = v128f_17;
    g_v512h[11] = v512h_7;
    __mmask64 m64_33 = (__mmask64)(g_m64[1] ^ 63);
    return m64_33;
}

__m128d torture_230(__m512 p0, __m256h p1, __m512 p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_2 = _mm512_cmp_epi32_mask(v512i_1, v512i_1, 2);
    __m512 v512f_3 = _mm512_and_ps(p0, p0);
    __m512 v512f_4 = _mm512_fnmadd_ps(p2, v512f_3, p2);
    __m128i v128i_5 = g_v128i[6];
    __m128i v128i_6 = _mm_add_epi8(v128i_5, v128i_5);
    __mmask16 m16_7 = _mm512_test_epi32_mask(v512i_1, v512i_1);
    __m128d v128d_8 = g_v128d[3];
    __m128d v128d_9 = _mm_max_pd(v128d_8, v128d_8);
    __m512i v512i_10 = _mm512_or_si512(v512i_1, v512i_1);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_fmadd_pd(v512d_11, v512d_11, v512d_11);
    __m128i v128i_13 = _mm_shuffle_epi8(v128i_6, v128i_5);
    __m128i v128i_14 = _mm_set1_epi32(0x55);
    __m512i v512i_15 = _mm512_add_epi64(v512i_10, v512i_1);
    __mmask16 m16_16 = _mm512_cmp_ps_mask(v512f_4, p0, _CMP_LT_OQ);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m256d v256d_17 = g_v256d[0];
    __m256d v256d_18 = _mm256_sqrt_pd(v256d_17);
    __m128 v128f_19 = g_v128f[5];
    __m128 v128f_20 = _mm_and_ps(v128f_19, v128f_19);
    __m128d v128d_21 = _mm_set1_pd(2.0);
    __m128d v128d_22 = _mm_mul_pd(v128d_21, v128d_21);
    __m128d v128d_23 = _mm_sqrt_pd(v128d_22);
    }
    __m256 v256f_24 = g_v256f[6];
    __m256 v256f_25 = _mm256_xor_ps(v256f_24, v256f_24);
    __m512d v512d_26 = _mm512_shuffle_f64x2(v512d_11, v512d_11, 0xEE);
    __mmask16 m16_27 = _kor_mask16(m16_7, m16_2);
    __m128d v128d_28 = _mm_add_pd(v128d_9, v128d_8);
    __m512 v512f_29 = _mm512_set1_ps(1.0f);
    __m256 v256f_30 = _mm256_unpacklo_ps(v256f_24, v256f_24);
    __m128 v128f_31 = g_v128f[2];
    __m128 v128f_32 = _mm_xor_ps(v128f_31, v128f_31);
    __m512i v512i_33 = _mm512_abs_epi32(v512i_15);
    __m256d v256d_34 = _mm256_set1_pd(2.0);
    g_v512d[1] = v512d_11;
    g_v512i[4] = v512i_33;
    g_v256f[1] = v256f_30;
    g_v256d[7] = v256d_34;
    g_v128f[11] = v128f_31;
    g_v128i[2] = v128i_13;
    return v128d_28;
}

__mmask8 torture_231(__m128h p0, __m512h p1, __m512d p2, __m512d p3, __m512 p4) {
    __m256d v256d_1 = g_v256d[4];
    __m256d v256d_2 = _mm256_fmadd_pd(v256d_1, v256d_1, v256d_1);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_max_ps(v128f_3, v128f_3);
    __m128d v128d_5 = g_v128d[4];
    __m128d v128d_6 = _mm_add_pd(v128d_5, v128d_5);
    __m256 v256f_7 = g_v256f[1];
    __m256 v256f_8 = _mm256_min_ps(v256f_7, v256f_7);
    __m512i v512i_9 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_10 = _mm512_cmp_epu32_mask(v512i_9, v512i_9, 4);
    __m512d v512d_11 = _mm512_div_pd(p2, p3);
    __m256 v256f_12 = _mm256_max_ps(v256f_8, v256f_8);
    __m128i v128i_13 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_14 = _mm512_broadcast_i32x4(v128i_13);
    __m256i v256i_15 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_16 = _mm256_sllv_epi32(v256i_15, v256i_15);
    __m512i v512i_17 = _mm512_xor_si512(v512i_14, v512i_14);
    __m128i v128i_18 = _mm_xor_si128(v128i_13, v128i_13);
    __m128d v128d_19 = _mm_max_pd(v128d_6, v128d_6);
    __m256i v256i_20 = _mm256_and_si256(v256i_15, v256i_15);
    __mmask8 m8_21 = (__mmask8)(g_m8[4] ^ 47);
    __m512d v512d_22 = _mm512_mask_blend_pd(m8_21, p2, v512d_11);
    __mmask16 m16_23 = _kor_mask16(m16_10, m16_10);
    __m512 v512f_24 = _mm512_div_ps(p4, p4);
    __m128 v128f_25 = _mm_min_ps(v128f_4, v128f_3);
    __m512i v512i_26 = _mm512_sub_epi16(v512i_9, v512i_17);
    __m512 v512f_27 = _mm512_max_ps(v512f_24, p4);
    __m512i v512i_28 = _mm512_shuffle_epi8(v512i_9, v512i_9);
    __m256i v256i_29 = _mm256_add_epi64(v256i_15, v256i_15);
    __mmask16 m16_30 = _kor_mask16(m16_23, m16_10);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_23) & 7); i_31++) {
    __m512i v512i_32 = _mm512_add_epi8(v512i_28, v512i_14);
    __m256 v256f_33 = _mm256_sub_ps(v256f_8, v256f_12);
    }
    g_v512f[8] = p4;
    g_v256d[2] = v256d_2;
    g_v128f[4] = v128f_25;
    g_v128d[1] = v128d_19;
    g_v128h[1] = p0;
    g_m8[9] = m8_21;
    return m8_21;
}

__mmask32 torture_232(__m128 p0, __m128i p1) {
    __m128i v128i_1 = _mm_add_epi8(p1, p1);
    __m512 v512f_2 = g_v512f[7];
    __m512 v512f_3 = _mm512_roundscale_ps(v512f_2, 0x10);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_popcnt_epi64(v512i_4);
    __m512i v512i_6 = _mm512_ternarylogic_epi32(v512i_5, v512i_5, v512i_5, 0x96);
    __mmask8 m8_7 = (__mmask8)(g_m8[4] ^ 23);
    __m512i v512i_8 = _mm512_broadcastmb_epi64(m8_7);
    __m512h v512h_9 = g_v512h[4];
    __m512h v512h_10 = _mm512_fmadd_ph(v512h_9, v512h_9, v512h_9);
    __m512d v512d_11 = g_v512d[10];
    __m512d v512d_12 = _mm512_fmadd_pd(v512d_11, v512d_11, v512d_11);
    __m128 v128f_13 = _mm_set1_ps(1.0f);
    __m512i v512i_14 = _mm512_madd52lo_epu64(v512i_6, v512i_6, v512i_4);
    __m128i v128i_15 = _mm_add_epi32(v128i_1, v128i_1);
    __m128 v128f_16 = _mm_min_ps(v128f_13, v128f_13);
    __m256 v256f_17 = g_v256f[5];
    __m256 v256f_18 = _mm256_min_ps(v256f_17, v256f_17);
    __mmask16 m16_19 = (__mmask16)(g_m16[9] ^ 142);
    __m512i v512i_20 = _mm512_mask_add_epi32(v512i_8, m16_19, v512i_8, v512i_5);
    __m128i v128i_21 = _mm_abs_epi32(v128i_15);
    __mmask16 m16_22 = _kxor_mask16(m16_19, m16_19);
    __m256i v256i_23 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_24 = _mm256_and_si256(v256i_23, v256i_23);
    __m512 v512f_25 = _mm512_getexp_ps(v512f_2);
    unsigned u32_26 = 57;
    __mmask16 m16_27 = _cvtu32_mask16(u32_26);
    __m256 v256f_28 = _mm256_unpacklo_ps(v256f_18, v256f_18);
    __mmask32 m32_29 = _mm512_cmp_ph_mask(v512h_10, v512h_9, 1);
    __m512 v512f_30 = _mm512_fnmadd_ps(v512f_3, v512f_3, v512f_3);
    __m128 v128f_31 = _mm_div_ps(p0, v128f_16);
    __m128i v128i_32 = _mm_and_si128(v128i_15, p1);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_19) & 7); i_33++) {
    __m512i v512i_34 = _mm512_abs_epi32(v512i_5);
    __m512i v512i_35 = _mm512_add_epi8(v512i_34, v512i_6);
    __m512i v512i_36 = _mm512_add_epi16(v512i_20, v512i_20);
    __m512i v512i_37 = _mm512_rolv_epi32(v512i_5, v512i_14);
    __m256i v256i_38 = _mm256_abs_epi32(v256i_23);
    }
    g_v512d[3] = v512d_11;
    g_v256f[11] = v256f_17;
    g_v256i[10] = v256i_23;
    g_v128i[9] = v128i_32;
    g_m32[10] = m32_29;
    return m32_29;
}

__m128 torture_233(__m512h p0, __m512d p1, __m512h p2, __m512 p3, __m256 p4) {
    __m512d v512d_1 = _mm512_sqrt_pd(p1);
    __m512d v512d_2 = _mm512_add_pd(v512d_1, p1);
    __m512d v512d_3 = _mm512_div_pd(v512d_2, v512d_1);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_5 = _mm512_rolv_epi32(v512i_4, v512i_4);
    __mmask16 m16_6 = _mm512_test_epi32_mask(v512i_4, v512i_4);
    __m512i v512i_7 = _mm512_shuffle_epi8(v512i_5, v512i_5);
    __m512i v512i_8 = _mm512_add_epi16(v512i_7, v512i_5);
    __mmask32 m32_9 = _mm512_kunpackw(m16_6, m16_6);
    __m512 v512f_10 = _mm512_maskz_mul_ps(m16_6, p3, p3);
    __m128 v128f_11 = g_v128f[7];
    __m128 v128f_12 = _mm_xor_ps(v128f_11, v128f_11);
    __m512 v512f_13 = _mm512_mask_add_ps(v512f_10, m16_6, v512f_10, p3);
    __m128d v128d_14 = g_v128d[8];
    __m512d v512d_15 = _mm512_insertf64x2(p1, v128d_14, 3);
    __m512 v512f_16 = _mm512_fnmadd_ps(v512f_10, p3, v512f_10);
    __m128i v128i_17 = g_v128i[0];
    __m128i v128i_18 = _mm_shuffle_epi8(v128i_17, v128i_17);
    __m128 v128f_19 = _mm512_extractf32x4_ps(v512f_16, 2);
    __m512 v512f_20 = _mm512_rcp14_ps(v512f_10);
    __m128i v128i_21 = _mm_add_epi16(v128i_17, v128i_18);
    __mmask16 m16_22 = _kand_mask16(m16_6, m16_6);
    __m128d v128d_23 = _mm_max_pd(v128d_14, v128d_14);
    __m512d v512d_24 = _mm512_add_pd(v512d_1, p1);
    __m256 v256f_25 = _mm256_add_ps(p4, p4);
    __m256d v256d_26 = _mm256_set1_pd(2.0);
    __m128i v128i_27 = _mm_shuffle_epi8(v128i_21, v128i_21);
    __m128 v128f_28 = _mm_min_ps(v128f_12, v128f_19);
    __m128i v128i_29 = _mm_and_si128(v128i_18, v128i_21);
    __m128d v128d_30 = _mm_set1_pd(2.0);
    if (((unsigned long long)m16_22) & 1ULL) {
    __m128i v128i_31 = _mm_sub_epi32(v128i_21, v128i_29);
    __m128 v128f_32 = _mm_sqrt_ps(v128f_19);
    __m512i v512i_33 = _mm512_shuffle_i32x4(v512i_7, v512i_8, 0x1B);
    __m512i v512i_34 = _mm512_sllv_epi32(v512i_33, v512i_33);
    __m512 v512f_35 = _mm512_xor_ps(v512f_13, v512f_16);
    __m512 v512f_36 = _mm512_and_ps(v512f_16, v512f_20);
    }
    g_v512d[10] = v512d_15;
    g_v256d[10] = v256d_26;
    g_v128f[2] = v128f_12;
    g_v512h[8] = p0;
    g_m16[5] = m16_22;
    return v128f_19;
}

__m256i torture_234(__m256h p0, __m128h p1, __m256d p2) {
    __m512i v512i_1 = g_v512i[8];
    __mmask16 m16_2 = _mm512_test_epi32_mask(v512i_1, v512i_1);
    __m256i v256i_3 = g_v256i[3];
    __m256i v256i_4 = _mm256_and_si256(v256i_3, v256i_3);
    __m512i v512i_5 = _mm512_dpbusd_epi32(v512i_1, v512i_1, v512i_1);
    __m512d v512d_6 = _mm512_set1_pd(2.0);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_min_ps(v256f_7, v256f_7);
    __m512d v512d_9 = _mm512_set1_pd(2.0);
    __m512 v512f_10 = _mm512_cvtxph_ps(p0);
    __m256 v256f_11 = _mm256_and_ps(v256f_7, v256f_7);
    __m512h v512h_12 = _mm512_cvtepi16_ph(v512i_5);
    __m128i v128i_13 = g_v128i[1];
    __m128i v128i_14 = _mm_sub_epi64(v128i_13, v128i_13);
    __m512 v512f_15 = _mm512_getexp_ps(v512f_10);
    __m512i v512i_16 = _mm512_cvtps_epi32(v512f_10);
    __m128i v128i_17 = _mm_sub_epi16(v128i_14, v128i_13);
    __m512 v512f_18 = _mm512_mask_blend_ps(m16_2, v512f_15, v512f_15);
    __m256i v256i_19 = _mm256_abs_epi32(v256i_4);
    __m512 v512f_20 = _mm512_fnmadd_ps(v512f_10, v512f_18, v512f_10);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m256i v256i_21 = _mm256_add_epi64(v256i_3, v256i_4);
    __m256d v256d_22 = _mm256_mul_pd(p2, p2);
    __m512i v512i_23 = _mm512_set1_epi32(0x55);
    __m512i v512i_24 = _mm512_add_epi8(v512i_1, v512i_23);
    }
    __m512 v512f_25 = _mm512_min_ps(v512f_15, v512f_10);
    __mmask16 m16_26 = _mm512_testn_epi32_mask(v512i_5, v512i_16);
    __m512 v512f_27 = _mm512_rsqrt14_ps(v512f_10);
    __m512 v512f_28 = _mm512_xor_ps(v512f_20, v512f_25);
    __mmask32 m32_29 = _mm512_cmp_ph_mask(v512h_12, v512h_12, 1);
    __m256i v256i_30 = _mm256_add_epi32(v256i_4, v256i_4);
    g_v512f[8] = v512f_15;
    g_v512i[3] = v512i_5;
    g_v256i[4] = v256i_3;
    g_m32[3] = m32_29;
    return v256i_30;
}

__mmask32 torture_235(__m128i p0, __m128h p1, __m512d p2) {
    __m256d v256d_1 = _mm256_set1_pd(2.0);
    __m512i v512i_2 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_3 = _mm512_test_epi32_mask(v512i_2, v512i_2);
    __m512h v512h_4 = g_v512h[0];
    __m512i v512i_5 = _mm512_cvtph_epi16(v512h_4);
    __m128i v128i_6 = _mm_and_si128(p0, p0);
    __m512h v512h_7 = _mm512_getexp_ph(v512h_4);
    __m128i v128i_8 = _mm_xor_si128(v128i_6, v128i_6);
    __m512i v512i_9 = _mm512_popcnt_epi64(v512i_2);
    __m512i v512i_10 = _mm512_madd52lo_epu64(v512i_2, v512i_5, v512i_5);
    __m512 v512f_11 = g_v512f[8];
    __m512 v512f_12 = _mm512_rcp14_ps(v512f_11);
    __m512i v512i_13 = _mm512_permutex2var_epi32(v512i_9, v512i_5, v512i_9);
    __m512h v512h_14 = _mm512_fmsub_ph(v512h_7, v512h_4, v512h_4);
    __mmask16 m16_15 = _mm512_testn_epi32_mask(v512i_9, v512i_10);
    __mmask64 m64_16 = _mm512_cmp_epi8_mask(v512i_5, v512i_13, 1);
    __m512i v512i_17 = _mm512_shuffle_i32x4(v512i_9, v512i_13, 0x1B);
    __m512 v512f_18 = _mm512_maskz_mul_ps(m16_15, v512f_12, v512f_11);
    __m512i v512i_19 = _mm512_cvtpd_epi64(p2);
    __m256i v256i_20 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_21 = _mm256_mullo_epi32(v256i_20, v256i_20);
    __m512i v512i_22 = _mm512_add_epi32(v512i_13, v512i_2);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m512i v512i_23 = _mm512_xor_si512(v512i_10, v512i_5);
    __mmask16 m16_24 = _kxor_mask16(m16_15, m16_3);
    __m256d v256d_25 = _mm256_sqrt_pd(v256d_1);
    __m512i v512i_26 = _mm512_mullo_epi32(v512i_5, v512i_17);
    }
    __m256i v256i_27 = _mm512_cvtepi64_epi32(v512i_22);
    __m512i v512i_28 = _mm512_broadcast_i32x4(v128i_6);
    __m512i v512i_29 = _mm512_add_epi16(v512i_2, v512i_17);
    __m512i v512i_30 = _mm512_popcnt_epi64(v512i_9);
    __m128 v128f_31 = _mm512_extractf32x4_ps(v512f_12, 2);
    __m256i v256i_32 = _mm256_unpacklo_epi32(v256i_21, v256i_27);
    __m128i v128i_33 = _mm_and_si128(p0, v128i_8);
    __mmask16 m16_34 = _mm512_movepi32_mask(v512i_30);
    for (int i_35 = 0; i_35 < (int)(((unsigned long long)m16_15) & 7); i_35++) {
    __m128 v128f_36 = _mm_set1_ps(1.0f);
    __m256 v256f_37 = _mm256_loadu_ps(fbuf);
    __m256 v256f_38 = _mm256_max_ps(v256f_37, v256f_37);
    __m128i v128i_39 = _mm_xor_si128(p0, v128i_8);
    __m512i v512i_40 = _mm512_sub_epi8(v512i_10, v512i_22);
    }
    g_v512f[3] = v512f_12;
    g_v256d[1] = v256d_1;
    g_v128f[5] = v128f_31;
    g_v128i[9] = p0;
    g_v512h[6] = v512h_7;
    __mmask32 m32_41 = (__mmask32)(g_m32[9] ^ 187);
    return m32_41;
}

__m256i torture_236(__m512i p0, __m256h p1, __m256i p2, __m128d p3, __m128i p4) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_2 = _mm512_cmp_ps_mask(v512f_1, v512f_1, _CMP_LT_OQ);
    __m128i v128i_3 = _mm_xor_si128(p4, p4);
    __m128i v128i_4 = _mm_add_epi16(v128i_3, p4);
    __m128d v128d_5 = _mm_fmadd_pd(p3, p3, p3);
    __m128 v128f_6 = _mm_loadu_ps(fbuf);
    __m128 v128f_7 = _mm_div_ps(v128f_6, v128f_6);
    __mmask16 m16_8 = _kand_mask16(m16_2, m16_2);
    __mmask8 m8_9 = (__mmask8)(g_m8[6] ^ 194);
    __mmask8 m8_10 = _kand_mask8(m8_9, m8_9);
    __mmask16 m16_11 = _mm512_testn_epi32_mask(p0, p0);
    __m128i v128i_12 = _mm_or_si128(v128i_3, p4);
    __m256 v256f_13 = _mm256_loadu_ps(fbuf);
    __m256 v256f_14 = _mm256_sqrt_ps(v256f_13);
    __m256 v256f_15 = _mm256_max_ps(v256f_14, v256f_14);
    __m512d v512d_16 = _mm512_loadu_pd(dbuf);
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_16, v512d_16, 0xEE);
    __m256i v256i_18 = _mm256_add_epi8(p2, p2);
    __m512 v512f_19 = _mm512_max_ps(v512f_1, v512f_1);
    __mmask16 m16_20 = _mm512_movepi32_mask(p0);
    __m128h v128h_21 = g_v128h[10];
    __m512d v512d_22 = _mm512_cvtph_pd(v128h_21);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_20) & 7); i_23++) {
    __m512i v512i_24 = _mm512_mask_add_epi32(p0, m16_11, p0, p0);
    __m512 v512f_25 = _mm512_fmsub_ps(v512f_1, v512f_19, v512f_1);
    __m512 v512f_26 = _mm512_mask_add_ps(v512f_1, m16_11, v512f_1, v512f_19);
    }
    __m256i v256i_27 = _mm256_sub_epi8(v256i_18, v256i_18);
    __m256d v256d_28 = g_v256d[0];
    __m256d v256d_29 = _mm256_unpackhi_pd(v256d_28, v256d_28);
    __m512h v512h_30 = g_v512h[5];
    __m512h v512h_31 = _mm512_add_ph(v512h_30, v512h_30);
    __mmask16 m16_32 = _mm512_cmp_epi32_mask(p0, p0, 2);
    __m128i v128i_33 = _mm_add_epi8(v128i_4, v128i_4);
    __m128d v128d_34 = _mm_sub_pd(p3, v128d_5);
    __m512i v512i_35 = _mm512_broadcastmw_epi32(m16_11);
    __m256i v256i_36 = _mm256_sub_epi64(v256i_18, v256i_18);
    __m256i v256i_37 = _mm256_sub_epi16(v256i_18, v256i_36);
    if (((unsigned long long)m16_32) & 1ULL) {
    __mmask16 m16_38 = _mm512_fpclass_ps_mask(v512f_1, 0x3);
    __m256d v256d_39 = _mm256_sqrt_pd(v256d_28);
    }
    g_v256f[7] = v256f_15;
    g_v256i[3] = v256i_36;
    g_m16[3] = m16_2;
    return v256i_37;
}

__m512d torture_237(__m256 p0, __m512d p1, __m256 p2) {
    __m256i v256i_1 = g_v256i[5];
    __m256i v256i_2 = _mm256_mullo_epi32(v256i_1, v256i_1);
    __m128 v128f_3 = g_v128f[3];
    __m128 v128f_4 = _mm_min_ps(v128f_3, v128f_3);
    __m128 v128f_5 = _mm_max_ps(v128f_3, v128f_3);
    __mmask64 m64_6 = (__mmask64)(g_m64[3] ^ 75);
    __mmask64 m64_7 = _kor_mask64(m64_6, m64_6);
    unsigned u32_8 = 18;
    __mmask16 m16_9 = _cvtu32_mask16(u32_8);
    __m512 v512f_10 = g_v512f[5];
    __m512 v512f_11 = _mm512_sqrt_ps(v512f_10);
    __m256i v256i_12 = _mm256_xor_si256(v256i_1, v256i_1);
    __m512d v512d_13 = _mm512_fmadd_pd(p1, p1, p1);
    __m512i v512i_14 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_15 = _mm512_popcnt_epi64(v512i_14);
    __m512 v512f_16 = _mm512_div_ps(v512f_10, v512f_10);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m256 v256f_17 = _mm256_fmsub_ps(p0, p2, p0);
    __m512d v512d_18 = _mm512_div_pd(v512d_13, p1);
    __m512i v512i_19 = _mm512_add_epi64(v512i_15, v512i_15);
    __m512 v512f_20 = _mm512_mask3_fmadd_ps(v512f_11, v512f_11, v512f_11, m16_9);
    __m512i v512i_21 = _mm512_sllv_epi32(v512i_15, v512i_19);
    }
    __m128d v128d_22 = g_v128d[9];
    __m128d v128d_23 = _mm_sqrt_pd(v128d_22);
    __m512d v512d_24 = _mm512_shuffle_f64x2(p1, p1, 0xEE);
    __mmask32 m32_25 = _mm512_kunpackw(m16_9, m16_9);
    __m512i v512i_26 = _mm512_sub_epi16(v512i_15, v512i_14);
    __m128 v128f_27 = _mm_max_ps(v128f_4, v128f_5);
    __m512 v512f_28 = _mm512_maskz_mul_ps(m16_9, v512f_16, v512f_11);
    __m256 v256f_29 = _mm256_mul_ps(p2, p0);
    __m128i v128i_30 = g_v128i[6];
    __m128i v128i_31 = _mm_add_epi16(v128i_30, v128i_30);
    __m128i v128i_32 = _mm_or_si128(v128i_31, v128i_30);
    __mmask8 m8_33 = _mm512_cmp_pd_mask(v512d_13, p1, _CMP_GE_OQ);
    __m512d v512d_34 = _mm512_unpackhi_pd(v512d_13, v512d_13);
    g_v512i[0] = v512i_15;
    g_v256f[6] = p2;
    g_v128d[0] = v128d_23;
    g_m16[8] = m16_9;
    return v512d_13;
}

__mmask16 torture_238(__m128i p0, __m512h p1) {
    __m256d v256d_1 = g_v256d[7];
    __m256d v256d_2 = _mm256_unpackhi_pd(v256d_1, v256d_1);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_and_ps(v256f_3, v256f_3);
    __m512i v512i_5 = g_v512i[5];
    __m512i v512i_6 = _mm512_and_si512(v512i_5, v512i_5);
    __m128 v128f_7 = g_v128f[6];
    __m512 v512f_8 = _mm512_broadcast_f32x4(v128f_7);
    __m512 v512f_9 = _mm512_sub_ps(v512f_8, v512f_8);
    __m256d v256d_10 = _mm256_unpackhi_pd(v256d_1, v256d_1);
    __m128 v128f_11 = _mm_and_ps(v128f_7, v128f_7);
    __m512d v512d_12 = _mm512_loadu_pd(dbuf);
    __m512d v512d_13 = _mm512_min_pd(v512d_12, v512d_12);
    __m512 v512f_14 = _mm512_rsqrt14_ps(v512f_9);
    int s32_15 = 87;
    if ((s32_15) & 1ULL) {
    __m512 v512f_16 = _mm512_unpacklo_ps(v512f_9, v512f_9);
    __m512d v512d_17 = _mm512_sqrt_pd(v512d_13);
    __m512d v512d_18 = _mm512_set1_pd(2.0);
    __m128i v128i_19 = _mm_add_epi32(p0, p0);
    __m128 v128f_20 = _mm_sqrt_ps(v128f_7);
    }
    __m512i v512i_21 = _mm512_cvtph_epi16(p1);
    __m512 v512f_22 = _mm512_fmsub_ps(v512f_9, v512f_8, v512f_14);
    __m128 v128f_23 = _mm_add_ps(v128f_7, v128f_7);
    __m512d v512d_24 = _mm512_cvtps_pd(v256f_3);
    if ((s32_15) & 1ULL) {
    __m128h v128h_25 = g_v128h[6];
    __m512d v512d_26 = _mm512_cvtph_pd(v128h_25);
    __m128i v128i_27 = _mm_mullo_epi32(p0, p0);
    __m128d v128d_28 = g_v128d[1];
    __m128d v128d_29 = _mm_add_pd(v128d_28, v128d_28);
    }
    g_v512d[4] = v512d_12;
    g_v256d[11] = v256d_2;
    g_v512h[7] = p1;
    __mmask16 m16_30 = (__mmask16)(g_m16[1] ^ 53);
    return m16_30;
}

__m256h torture_239(__m128d p0, __m256 p1, __m128d p2, __m512h p3, __m256d p4) {
    __m256 v256f_1 = _mm256_sub_ps(p1, p1);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_max_ps(v512f_2, v512f_2);
    __m512i v512i_4 = g_v512i[7];
    __m512i v512i_5 = _mm512_and_si512(v512i_4, v512i_4);
    __m256i v256i_6 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_7 = _mm256_unpacklo_epi32(v256i_6, v256i_6);
    __m256i v256i_8 = _mm256_unpacklo_epi32(v256i_6, v256i_7);
    __mmask16 m16_9 = (__mmask16)(g_m16[4] ^ 9);
    __mmask16 m16_10 = _kor_mask16(m16_9, m16_9);
    __m512i v512i_11 = _mm512_add_epi16(v512i_4, v512i_5);
    __m128i v128i_12 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_13 = _mm_add_epi8(v128i_12, v128i_12);
    __m512 v512f_14 = _mm512_mask_add_ps(v512f_2, m16_9, v512f_2, v512f_3);
    __m512h v512h_15 = _mm512_add_ph(p3, p3);
    __m256d v256d_16 = _mm256_unpackhi_pd(p4, p4);
    __m512 v512f_17 = _mm512_maskz_mul_ps(m16_9, v512f_2, v512f_3);
    unsigned u32_18 = 88;
    __mmask16 m16_19 = _cvtu32_mask16(u32_18);
    __m256 v256f_20 = _mm256_and_ps(p1, p1);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m128i v128i_21 = _mm_add_epi8(v128i_13, v128i_13);
    __m128 v128f_22 = _mm_loadu_ps(fbuf);
    __m128 v128f_23 = _mm_fmsub_ps(v128f_22, v128f_22, v128f_22);
    __m512d v512d_24 = _mm512_loadu_pd(dbuf);
    __m512d v512d_25 = _mm512_max_pd(v512d_24, v512d_24);
    }
    __m512i v512i_26 = _mm512_broadcastmw_epi32(m16_10);
    __m128h v128h_27 = g_v128h[10];
    __m512d v512d_28 = _mm512_cvtph_pd(v128h_27);
    __m256i v256i_29 = _mm256_and_si256(v256i_6, v256i_8);
    __m512 v512f_30 = _mm512_mul_ps(v512f_2, v512f_17);
    __m512i v512i_31 = _mm512_cvtepi8_epi32(v128i_12);
    __m512 v512f_32 = _mm512_maskz_mul_ps(m16_10, v512f_14, v512f_17);
    __m128 v128f_33 = g_v128f[3];
    __m128 v128f_34 = _mm_fmsub_ps(v128f_33, v128f_33, v128f_33);
    for (int i_35 = 0; i_35 < (int)(((unsigned long long)m16_9) & 7); i_35++) {
    __m512i v512i_36 = _mm512_dpbusd_epi32(v512i_4, v512i_4, v512i_4);
    __m512 v512f_37 = _mm512_mask_add_ps(v512f_32, m16_9, v512f_14, v512f_30);
    __m512i v512i_38 = _mm512_madd52lo_epu64(v512i_31, v512i_4, v512i_26);
    __mmask16 m16_39 = _cvtu32_mask16(u32_18);
    }
    g_v512i[0] = v512i_4;
    g_v256d[2] = v256d_16;
    g_v256i[3] = v256i_29;
    g_v128f[11] = v128f_33;
    g_m16[2] = m16_10;
    __m256h v256h_40 = g_v256h[6];
    return v256h_40;
}

__mmask32 torture_240(__m512 p0, __m128h p1, __m256d p2, __m128h p3, __m256 p4) {
    __m128i v128i_1 = g_v128i[3];
    __m128i v128i_2 = _mm_add_epi64(v128i_1, v128i_1);
    __m256 v256f_3 = _mm256_shuffle_ps(p4, p4, 0x1B);
    __m256i v256i_4 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_5 = _mm256_add_epi16(v256i_4, v256i_4);
    __m512 v512f_6 = _mm512_min_ps(p0, p0);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_or_si512(v512i_7, v512i_7);
    __m128i v128i_9 = _mm_sub_epi64(v128i_2, v128i_2);
    __mmask32 m32_10 = (__mmask32)(g_m32[11] ^ 175);
    __mmask32 m32_11 = _kxor_mask32(m32_10, m32_10);
    __m128d v128d_12 = g_v128d[8];
    __m128d v128d_13 = _mm_unpackhi_pd(v128d_12, v128d_12);
    __m512i v512i_14 = _mm512_popcnt_epi64(v512i_7);
    __m256d v256d_15 = _mm256_sub_pd(p2, p2);
    __m128i v128i_16 = _mm_add_epi16(v128i_9, v128i_9);
    __m128 v128f_17 = _mm_loadu_ps(fbuf);
    __m128 v128f_18 = _mm_div_ps(v128f_17, v128f_17);
    __m512d v512d_19 = _mm512_set1_pd(2.0);
    __m512i v512i_20 = _mm512_lzcnt_epi32(v512i_7);
    __m512i v512i_21 = _mm512_mullo_epi32(v512i_7, v512i_7);
    __m128d v128d_22 = _mm_sub_pd(v128d_12, v128d_13);
    __m256i v256i_23 = _mm256_add_epi16(v256i_5, v256i_4);
    __m256i v256i_24 = _mm256_sub_epi64(v256i_23, v256i_4);
    __m256i v256i_25 = _mm256_and_si256(v256i_23, v256i_5);
    __m512i v512i_26 = _mm512_dpwssd_epi32(v512i_8, v512i_21, v512i_14);
    __mmask16 m16_27 = _mm512_cmp_epi32_mask(v512i_14, v512i_14, 2);
    __m256i v256i_28 = _mm512_cvtepi64_epi32(v512i_20);
    __m512d v512d_29 = _mm512_unpackhi_pd(v512d_19, v512d_19);
    __m256i v256i_30 = _mm256_sub_epi16(v256i_23, v256i_23);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_27) & 7); i_31++) {
    __m512h v512h_32 = g_v512h[7];
    __m512h v512h_33 = _mm512_getexp_ph(v512h_32);
    __m512i v512i_34 = _mm512_broadcastmw_epi32(m16_27);
    __m512i v512i_35 = _mm512_xor_si512(v512i_34, v512i_20);
    __mmask16 m16_36 = _kand_mask16(m16_27, m16_27);
    }
    g_v512i[11] = v512i_21;
    g_v256f[2] = p4;
    g_v256d[8] = p2;
    g_v256i[1] = v256i_30;
    g_v128f[7] = v128f_18;
    g_v128i[5] = v128i_9;
    g_v128h[7] = p1;
    g_m32[8] = m32_10;
    return m32_10;
}

__m256i torture_241(__m512h p0, __m256i p1, __m512i p2, __m256h p3) {
    __m128i v128i_1 = g_v128i[5];
    __m128i v128i_2 = _mm_unpacklo_epi32(v128i_1, v128i_1);
    __m256d v256d_3 = g_v256d[4];
    __m256d v256d_4 = _mm256_min_pd(v256d_3, v256d_3);
    __m512 v512f_5 = _mm512_cvtepi32_ps(p2);
    __m128i v128i_6 = _mm_sub_epi8(v128i_1, v128i_2);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_and_ps(v256f_7, v256f_7);
    __mmask16 m16_9 = (__mmask16)(g_m16[0] ^ 166);
    __m512 v512f_10 = _mm512_mask_blend_ps(m16_9, v512f_5, v512f_5);
    __m256i v256i_11 = _mm256_sub_epi16(p1, p1);
    __m256i v256i_12 = _mm256_sub_epi16(p1, v256i_11);
    __m256i v256i_13 = _mm256_add_epi64(v256i_11, v256i_11);
    __m256 v256f_14 = _mm256_unpacklo_ps(v256f_8, v256f_7);
    __m512i v512i_15 = _mm512_add_epi8(p2, p2);
    __m256i v256i_16 = _mm256_add_epi16(v256i_11, p1);
    __m512i v512i_17 = _mm512_cvtepi8_epi32(v128i_2);
    __m256i v256i_18 = _mm256_add_epi32(v256i_13, v256i_13);
    __m512d v512d_19 = g_v512d[8];
    __m512d v512d_20 = _mm512_mul_pd(v512d_19, v512d_19);
    __m512i v512i_21 = _mm512_dpwssd_epi32(p2, p2, v512i_17);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m256i v256i_22 = _mm256_sub_epi32(v256i_11, p1);
    __m512i v512i_23 = _mm512_ternarylogic_epi32(v512i_17, v512i_17, v512i_17, 0x96);
    }
    __mmask16 m16_24 = _kxor_mask16(m16_9, m16_9);
    __m256d v256d_25 = _mm256_min_pd(v256d_4, v256d_4);
    __m512 v512f_26 = _mm512_fmadd_ps(v512f_10, v512f_5, v512f_5);
    __m128d v128d_27 = _mm_set1_pd(2.0);
    g_v512i[4] = v512i_17;
    g_v256f[6] = v256f_14;
    g_v128d[0] = v128d_27;
    g_v256h[7] = p3;
    g_m16[6] = m16_9;
    return p1;
}

__m128i torture_242(__m256 p0, __m256 p1) {
    __m256d v256d_1 = g_v256d[3];
    __m256d v256d_2 = _mm256_mul_pd(v256d_1, v256d_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[6] ^ 44);
    __m512i v512i_4 = _mm512_broadcastmw_epi32(m16_3);
    __m256 v256f_5 = _mm256_and_ps(p1, p1);
    __m512 v512f_6 = g_v512f[11];
    __m512 v512f_7 = _mm512_getexp_ps(v512f_6);
    __mmask16 m16_8 = _mm512_cmp_ps_mask(v512f_7, v512f_6, _CMP_LT_OQ);
    __m512 v512f_9 = _mm512_roundscale_ps(v512f_7, 0x10);
    __m512 v512f_10 = _mm512_add_ps(v512f_9, v512f_7);
    __m128 v128f_11 = g_v128f[3];
    __m128 v128f_12 = _mm_min_ps(v128f_11, v128f_11);
    __m256d v256d_13 = _mm256_sub_pd(v256d_2, v256d_1);
    __m128 v128f_14 = _mm_sub_ps(v128f_12, v128f_11);
    __mmask16 m16_15 = _mm512_cmp_ps_mask(v512f_9, v512f_10, _CMP_LT_OQ);
    __m256 v256f_16 = _mm256_sub_ps(p0, v256f_5);
    __m512 v512f_17 = _mm512_max_ps(v512f_9, v512f_10);
    __m512 v512f_18 = _mm512_unpacklo_ps(v512f_17, v512f_10);
    __m128i v128i_19 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_20 = _mm_or_si128(v128i_19, v128i_19);
    __m512i v512i_21 = _mm512_sllv_epi32(v512i_4, v512i_4);
    __m256d v256d_22 = _mm256_min_pd(v256d_1, v256d_2);
    g_v256f[4] = p0;
    return v128i_19;
}

__m512 torture_243(__m512i p0, __m128i p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_unpacklo_ps(v512f_1, v512f_1);
    __m512d v512d_3 = g_v512d[2];
    __m128d v128d_4 = g_v128d[9];
    __m512d v512d_5 = _mm512_insertf64x2(v512d_3, v128d_4, 3);
    __mmask16 m16_6 = (__mmask16)(g_m16[9] ^ 242);
    __m512 v512f_7 = _mm512_mask3_fmadd_ps(v512f_2, v512f_1, v512f_2, m16_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_max_ps(v256f_8, v256f_8);
    __m128 v128f_10 = _mm_loadu_ps(fbuf);
    __m128 v128f_11 = _mm_div_ps(v128f_10, v128f_10);
    __m256i v256i_12 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_13 = _mm256_add_epi8(v256i_12, v256i_12);
    __m512h v512h_14 = g_v512h[5];
    __m512h v512h_15 = _mm512_add_ph(v512h_14, v512h_14);
    __mmask64 m64_16 = (__mmask64)(g_m64[5] ^ 119);
    __mmask64 m64_17 = _kor_mask64(m64_16, m64_16);
    __m256d v256d_18 = g_v256d[3];
    __m256d v256d_19 = _mm256_sub_pd(v256d_18, v256d_18);
    __mmask8 m8_20 = (__mmask8)(g_m8[11] ^ 94);
    __m512d v512d_21 = _mm512_mask_blend_pd(m8_20, v512d_3, v512d_5);
    __m256d v256d_22 = _mm256_mul_pd(v256d_19, v256d_18);
    __m512d v512d_23 = _mm512_mul_pd(v512d_3, v512d_21);
    __m512d v512d_24 = _mm512_max_pd(v512d_5, v512d_3);
    __m128i v128i_25 = _mm_add_epi64(p1, p1);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_6) & 7); i_26++) {
    __m128 v128f_27 = _mm_fmsub_ps(v128f_11, v128f_11, v128f_10);
    unsigned u32_28 = 74;
    __mmask16 m16_29 = _cvtu32_mask16(u32_28);
    __m512i v512i_30 = _mm512_popcnt_epi64(p0);
    __m512d v512d_31 = _mm512_set1_pd(2.0);
    }
    __m512i v512i_32 = _mm512_xor_si512(p0, p0);
    __m512i v512i_33 = _mm512_mask_add_epi32(p0, m16_6, p0, p0);
    __m512i v512i_34 = _mm512_broadcast_i32x4(v128i_25);
    __m512i v512i_35 = _mm512_shuffle_epi8(v512i_34, p0);
    __m512 v512f_36 = _mm512_max_ps(v512f_1, v512f_7);
    __mmask8 m8_37 = _mm512_cmp_pd_mask(v512d_5, v512d_23, _CMP_GE_OQ);
    __m128 v128f_38 = _mm512_extractf32x4_ps(v512f_1, 2);
    __m512i v512i_39 = _mm512_add_epi8(v512i_33, v512i_35);
    __m512i v512i_40 = _mm512_mask_add_epi32(v512i_32, m16_6, v512i_34, v512i_35);
    __m512i v512i_41 = _mm512_add_epi16(v512i_32, v512i_34);
    g_v512d[4] = v512d_23;
    g_v256i[10] = v256i_13;
    g_v128f[9] = v128f_10;
    g_v128i[10] = p1;
    g_m8[6] = m8_20;
    g_m64[6] = m64_16;
    return v512f_1;
}

__m128 torture_244(__m512 p0, __m128 p1, __m512i p2, __m256i p3) {
    __m512 v512f_1 = _mm512_rsqrt14_ps(p0);
    __m512 v512f_2 = _mm512_div_ps(p0, v512f_1);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_unpacklo_ps(v256f_3, v256f_3);
    __m512i v512i_5 = _mm512_or_si512(p2, p2);
    __m128d v128d_6 = g_v128d[7];
    __m128d v128d_7 = _mm_max_pd(v128d_6, v128d_6);
    __m512d v512d_8 = g_v512d[1];
    __m512d v512d_9 = _mm512_max_pd(v512d_8, v512d_8);
    __m256i v256i_10 = _mm256_add_epi32(p3, p3);
    __m512i v512i_11 = _mm512_cvtpd_epi64(v512d_8);
    __m128i v128i_12 = g_v128i[4];
    __m128i v128i_13 = _mm_unpacklo_epi32(v128i_12, v128i_12);
    __m128i v128i_14 = _mm_sllv_epi32(v128i_13, v128i_12);
    __m512i v512i_15 = _mm512_madd52lo_epu64(v512i_11, v512i_5, p2);
    __mmask16 m16_16 = (__mmask16)(g_m16[4] ^ 146);
    __m512 v512f_17 = _mm512_mask_blend_ps(m16_16, p0, p0);
    __m512 v512f_18 = _mm512_add_ps(p0, v512f_1);
    __m512i v512i_19 = _mm512_or_si512(v512i_15, v512i_5);
    __m256d v256d_20 = g_v256d[3];
    __m256d v256d_21 = _mm256_mul_pd(v256d_20, v256d_20);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m256 v256f_22 = _mm256_fmsub_ps(v256f_3, v256f_4, v256f_4);
    __m128d v128d_23 = _mm_unpackhi_pd(v128d_6, v128d_7);
    __m512i v512i_24 = _mm512_sub_epi32(v512i_11, v512i_15);
    __m128i v128i_25 = _mm_add_epi8(v128i_14, v128i_13);
    }
    __m512 v512f_26 = _mm512_getexp_ps(v512f_2);
    __m256 v256f_27 = _mm256_and_ps(v256f_4, v256f_4);
    __mmask8 m8_28 = (__mmask8)(g_m8[10] ^ 81);
    __m512d v512d_29 = _mm512_mask_blend_pd(m8_28, v512d_9, v512d_8);
    __m256 v256f_30 = _mm256_set1_ps(1.0f);
    __m512d v512d_31 = _mm512_add_pd(v512d_8, v512d_9);
    __m512i v512i_32 = _mm512_sllv_epi32(v512i_19, v512i_5);
    __m128d v128d_33 = _mm_add_pd(v128d_7, v128d_7);
    __m128d v128d_34 = _mm_sub_pd(v128d_33, v128d_6);
    __m512h v512h_35 = g_v512h[8];
    __mmask32 m32_36 = _mm512_cmp_ph_mask(v512h_35, v512h_35, 1);
    __m256i v256i_37 = _mm256_unpacklo_epi32(v256i_10, v256i_10);
    __mmask16 m16_38 = _kor_mask16(m16_16, m16_16);
    __m512 v512f_39 = _mm512_set1_ps(1.0f);
    g_v512f[5] = v512f_17;
    g_v256f[6] = v256f_30;
    g_v256i[9] = v256i_37;
    g_v128f[4] = p1;
    g_v128d[0] = v128d_34;
    g_v128i[0] = v128i_14;
    g_m8[1] = m8_28;
    g_m32[2] = m32_36;
    return p1;
}

__m128d torture_245(__m256i p0, __m256i p1, __m128 p2, __m256 p3) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m512d v512d_2 = _mm512_add_pd(v512d_1, v512d_1);
    __m512i v512i_3 = g_v512i[3];
    __m512i v512i_4 = _mm512_shuffle_i32x4(v512i_3, v512i_3, 0x1B);
    __m512d v512d_5 = _mm512_sub_pd(v512d_1, v512d_2);
    __mmask32 m32_6 = (__mmask32)(g_m32[1] ^ 134);
    __mmask32 m32_7 = _kxor_mask32(m32_6, m32_6);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_9 = _mm_or_si128(v128i_8, v128i_8);
    __m128i v128i_10 = _mm_or_si128(v128i_9, v128i_8);
    __m512i v512i_11 = _mm512_add_epi8(v512i_4, v512i_4);
    __m512i v512i_12 = _mm512_popcnt_epi64(v512i_3);
    __m128i v128i_13 = _mm_add_epi64(v128i_8, v128i_10);
    __mmask16 m16_14 = (__mmask16)(g_m16[6] ^ 162);
    __m512i v512i_15 = _mm512_mask_add_epi32(v512i_4, m16_14, v512i_11, v512i_3);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512i v512i_16 = _mm512_sub_epi16(v512i_4, v512i_3);
    __m128i v128i_17 = _mm_and_si128(v128i_9, v128i_10);
    __m512 v512f_18 = g_v512f[3];
    __m512 v512f_19 = _mm512_add_ps(v512f_18, v512f_18);
    __m512i v512i_20 = _mm512_add_epi64(v512i_4, v512i_12);
    __m256 v256f_21 = _mm256_and_ps(p3, p3);
    }
    __mmask16 m16_22 = _kshiftri_mask16(m16_14, 1);
    __m512i v512i_23 = _mm512_set1_epi32(0x55);
    __mmask16 m16_24 = _knot_mask16(m16_14);
    __m512i v512i_25 = _mm512_add_epi64(v512i_15, v512i_4);
    __m256d v256d_26 = g_v256d[6];
    __m256d v256d_27 = _mm256_div_pd(v256d_26, v256d_26);
    __m128i v128i_28 = _mm_shuffle_epi8(v128i_9, v128i_10);
    __mmask16 m16_29 = _mm512_cmp_epu32_mask(v512i_15, v512i_12, 4);
    g_v512d[4] = v512d_2;
    g_m32[11] = m32_6;
    __m128d v128d_30 = g_v128d[2];
    return v128d_30;
}

__m128 torture_246(__m128d p0, __m256h p1, __m256i p2, __m128 p3) {
    __m512i v512i_1 = g_v512i[10];
    __m512i v512i_2 = _mm512_shuffle_epi8(v512i_1, v512i_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_max_pd(v512d_3, v512d_3);
    __m256d v256d_5 = g_v256d[11];
    __m256d v256d_6 = _mm256_max_pd(v256d_5, v256d_5);
    __m512i v512i_7 = _mm512_sub_epi64(v512i_2, v512i_1);
    __m128d v128d_8 = _mm_max_pd(p0, p0);
    __m512i v512i_9 = _mm512_shuffle_epi8(v512i_7, v512i_7);
    unsigned u32_10 = 88;
    __mmask16 m16_11 = _cvtu32_mask16(u32_10);
    __mmask16 m16_12 = _mm512_test_epi32_mask(v512i_9, v512i_9);
    __m512h v512h_13 = g_v512h[3];
    __m512h v512h_14 = _mm512_fmul_pch(v512h_13, v512h_13);
    __m512i v512i_15 = _mm512_sub_epi16(v512i_2, v512i_1);
    __m512 v512f_16 = g_v512f[0];
    __m512 v512f_17 = _mm512_rcp14_ps(v512f_16);
    __m512i v512i_18 = _mm512_add_epi64(v512i_1, v512i_15);
    __m256 v256f_19 = _mm512_cvtpd_ps(v512d_3);
    __m128 v128f_20 = _mm_min_ps(p3, p3);
    __m512 v512f_21 = _mm512_add_ps(v512f_17, v512f_16);
    g_v512f[5] = v512f_17;
    g_v512d[9] = v512d_3;
    g_v512i[11] = v512i_18;
    g_v256f[9] = v256f_19;
    g_v256h[6] = p1;
    g_m16[7] = m16_11;
    return v128f_20;
}

__mmask16 torture_247(__m256d p0, __m512d p1, __m128 p2, __m256 p3) {
    __m256 v256f_1 = _mm256_shuffle_ps(p3, p3, 0x1B);
    __m256h v256h_2 = g_v256h[11];
    __m512 v512f_3 = _mm512_cvtxph_ps(v256h_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[10] ^ 127);
    __mmask16 m16_5 = _kor_mask16(m16_4, m16_4);
    __m256 v256f_6 = _mm256_fmsub_ps(v256f_1, p3, v256f_1);
    __m512d v512d_7 = _mm512_fmadd_pd(p1, p1, p1);
    __m512 v512f_8 = _mm512_fmadd_ps(v512f_3, v512f_3, v512f_3);
    __m512i v512i_9 = g_v512i[0];
    __m512h v512h_10 = _mm512_cvtepi16_ph(v512i_9);
    __mmask16 m16_11 = _knot_mask16(m16_5);
    __m256i v256i_12 = g_v256i[3];
    __m256i v256i_13 = _mm256_add_epi8(v256i_12, v256i_12);
    __m128d v128d_14 = g_v128d[2];
    __m128d v128d_15 = _mm_add_pd(v128d_14, v128d_14);
    __m512i v512i_16 = _mm512_broadcastmw_epi32(m16_4);
    __m512i v512i_17 = _mm512_broadcastmw_epi32(m16_5);
    __m256 v256f_18 = _mm512_cvtpd_ps(v512d_7);
    __m256d v256d_19 = _mm256_fmadd_pd(p0, p0, p0);
    __mmask32 m32_20 = _mm512_kunpackw(m16_11, m16_5);
    __m256 v256f_21 = _mm256_min_ps(p3, p3);
    __m256d v256d_22 = _mm256_add_pd(v256d_19, p0);
    __m128 v128f_23 = _mm_sqrt_ps(p2);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_5) & 7); i_24++) {
    __mmask64 m64_25 = _mm512_movepi8_mask(v512i_17);
    __m512i v512i_26 = _mm512_sllv_epi32(v512i_9, v512i_9);
    }
    g_v512d[9] = v512d_7;
    g_v512i[9] = v512i_17;
    g_v256d[10] = v256d_19;
    g_v256i[4] = v256i_13;
    g_v128f[8] = p2;
    g_v128d[4] = v128d_14;
    g_v256h[3] = v256h_2;
    g_m32[6] = m32_20;
    return m16_4;
}

__m256h torture_248(__m512 p0, __m128i p1, __m512i p2) {
    __m128i v128i_1 = _mm_or_si128(p1, p1);
    __m256i v256i_2 = g_v256i[7];
    __m256i v256i_3 = _mm256_sub_epi16(v256i_2, v256i_2);
    __m512d v512d_4 = g_v512d[8];
    __m512d v512d_5 = _mm512_add_pd(v512d_4, v512d_4);
    __m128i v128i_6 = _mm_add_epi16(v128i_1, p1);
    __m128i v128i_7 = _mm_add_epi32(v128i_6, v128i_6);
    __m256i v256i_8 = _mm256_add_epi16(v256i_2, v256i_3);
    __m128 v128f_9 = g_v128f[3];
    __m128 v128f_10 = _mm_sub_ps(v128f_9, v128f_9);
    __m512i v512i_11 = _mm512_xor_si512(p2, p2);
    __m512 v512f_12 = _mm512_and_ps(p0, p0);
    __m512d v512d_13 = _mm512_min_pd(v512d_5, v512d_4);
    __m512d v512d_14 = _mm512_max_pd(v512d_4, v512d_13);
    int s32_15 = 96;
    for (int i_16 = 0; i_16 < (int)((s32_15) & 7); i_16++) {
    __m256 v256f_17 = _mm256_loadu_ps(fbuf);
    __m256 v256f_18 = _mm256_fmadd_ps(v256f_17, v256f_17, v256f_17);
    __m128d v128d_19 = g_v128d[9];
    __m128d v128d_20 = _mm_max_pd(v128d_19, v128d_19);
    __m256i v256i_21 = _mm256_set1_epi32(0x55);
    }
    __m256 v256f_22 = _mm256_loadu_ps(fbuf);
    __m512d v512d_23 = _mm512_cvtps_pd(v256f_22);
    __m512 v512f_24 = _mm512_min_ps(v512f_12, p0);
    __m512i v512i_25 = _mm512_dpbusd_epi32(p2, v512i_11, p2);
    __m128i v128i_26 = _mm_sub_epi16(v128i_7, v128i_1);
    __m512h v512h_27 = _mm512_cvtepi16_ph(v512i_11);
    __m128 v128f_28 = _mm_xor_ps(v128f_10, v128f_9);
    __m512i v512i_29 = _mm512_conflict_epi32(p2);
    if ((s32_15) & 1ULL) {
    __m128d v128d_30 = g_v128d[9];
    __m128d v128d_31 = _mm_unpackhi_pd(v128d_30, v128d_30);
    __m512i v512i_32 = _mm512_sub_epi32(v512i_25, p2);
    __m512i v512i_33 = _mm512_dpwssd_epi32(v512i_29, p2, p2);
    __m256i v256i_34 = _mm256_slli_epi32(v256i_2, 3);
    __m512d v512d_35 = _mm512_shuffle_f64x2(v512d_5, v512d_5, 0xEE);
    __m512 v512f_36 = _mm512_getexp_ps(v512f_12);
    }
    g_v512d[11] = v512d_23;
    g_v512i[5] = p2;
    g_v256f[3] = v256f_22;
    g_v128f[0] = v128f_9;
    g_v128i[1] = p1;
    g_v512h[8] = v512h_27;
    __m256h v256h_37 = g_v256h[2];
    return v256h_37;
}

__m512i torture_249(__m512 p0, __m512i p1, __m512d p2, __m128h p3) {
    __m256 v256f_1 = g_v256f[6];
    __m256 v256f_2 = _mm256_sub_ps(v256f_1, v256f_1);
    __m512i v512i_3 = _mm512_madd52lo_epu64(p1, p1, p1);
    __m512d v512d_4 = _mm512_cvtph_pd(p3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_abs_epi32(v128i_5);
    __m128i v128i_7 = _mm_mullo_epi32(v128i_5, v128i_6);
    __m256i v256i_8 = g_v256i[10];
    __m256i v256i_9 = _mm256_add_epi64(v256i_8, v256i_8);
    __m256d v256d_10 = g_v256d[2];
    __m256d v256d_11 = _mm256_fmadd_pd(v256d_10, v256d_10, v256d_10);
    __m512 v512f_12 = _mm512_div_ps(p0, p0);
    __m256 v256f_13 = _mm256_xor_ps(v256f_1, v256f_1);
    __mmask16 m16_14 = (__mmask16)(g_m16[10] ^ 99);
    __m512 v512f_15 = _mm512_mask3_fmadd_ps(v512f_12, v512f_12, p0, m16_14);
    __m256i v256i_16 = _mm256_add_epi32(v256i_8, v256i_9);
    __m256h v256h_17 = _mm512_cvtxps_ph(v512f_12);
    __m256d v256d_18 = _mm256_mul_pd(v256d_11, v256d_10);
    __m256 v256f_19 = _mm256_max_ps(v256f_13, v256f_1);
    __m512 v512f_20 = _mm512_fnmadd_ps(p0, v512f_15, v512f_12);
    __m512h v512h_21 = g_v512h[7];
    __m512i v512i_22 = _mm512_cvtph_epi16(v512h_21);
    __m512i v512i_23 = _mm512_cvtpd_epi64(p2);
    __m256d v256d_24 = _mm256_sqrt_pd(v256d_10);
    __m256i v256i_25 = _mm256_sllv_epi32(v256i_8, v256i_16);
    __mmask16 m16_26 = _mm512_testn_epi32_mask(p1, v512i_3);
    __m256i v256i_27 = _mm256_and_si256(v256i_16, v256i_25);
    __m128d v128d_28 = g_v128d[1];
    __m128d v128d_29 = _mm_fmadd_pd(v128d_28, v128d_28, v128d_28);
    __mmask32 m32_30 = _mm512_kunpackw(m16_14, m16_26);
    __m256i v256i_31 = _mm256_sub_epi8(v256i_16, v256i_16);
    __m128 v128f_32 = g_v128f[0];
    __m128 v128f_33 = _mm_fnmadd_ps(v128f_32, v128f_32, v128f_32);
    __m512h v512h_34 = _mm512_getexp_ph(v512h_21);
    g_v512d[10] = v512d_4;
    g_v256d[8] = v256d_11;
    g_v128f[5] = v128f_33;
    g_m16[7] = m16_14;
    return v512i_22;
}

__mmask8 torture_250(__m256 p0, __m512h p1, __m256 p2, __m256 p3, __m512i p4) {
    __m128i v128i_1 = g_v128i[0];
    __m128i v128i_2 = _mm_or_si128(v128i_1, v128i_1);
    __m256i v256i_3 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_4 = _mm256_slli_epi32(v256i_3, 3);
    __m256 v256f_5 = _mm256_set1_ps(1.0f);
    __m128i v128i_6 = _mm_and_si128(v128i_2, v128i_2);
    __m256d v256d_7 = g_v256d[7];
    __m256d v256d_8 = _mm256_max_pd(v256d_7, v256d_7);
    __mmask8 m8_9 = (__mmask8)(g_m8[9] ^ 217);
    __m512d v512d_10 = g_v512d[6];
    __m512d v512d_11 = _mm512_mask_blend_pd(m8_9, v512d_10, v512d_10);
    __m256d v256d_12 = _mm256_unpackhi_pd(v256d_8, v256d_8);
    __m512i v512i_13 = _mm512_add_epi32(p4, p4);
    __m512d v512d_14 = _mm512_sqrt_pd(v512d_11);
    __m256 v256f_15 = _mm256_sqrt_ps(p0);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m8_9) & 7); i_16++) {
    __m256d v256d_17 = _mm256_set1_pd(2.0);
    __m128d v128d_18 = g_v128d[11];
    __m128d v128d_19 = _mm_max_pd(v128d_18, v128d_18);
    __mmask16 m16_20 = (__mmask16)(g_m16[11] ^ 92);
    __mmask16 m16_21 = _kshiftri_mask16(m16_20, 1);
    }
    __m512 v512f_22 = _mm512_loadu_ps(fbuf);
    __m512 v512f_23 = _mm512_rcp14_ps(v512f_22);
    __mmask16 m16_24 = _mm512_test_epi32_mask(v512i_13, p4);
    __m512i v512i_25 = _mm512_cvtps_epi32(v512f_23);
    __m256 v256f_26 = _mm256_min_ps(v256f_15, v256f_15);
    __m256 v256f_27 = _mm256_div_ps(p0, v256f_15);
    __mmask16 m16_28 = _kshiftli_mask16(m16_24, 2);
    __mmask16 m16_29 = _mm512_test_epi32_mask(v512i_25, p4);
    __m512i v512i_30 = _mm512_popcnt_epi64(p4);
    __m128 v128f_31 = g_v128f[9];
    __m128 v128f_32 = _mm_add_ps(v128f_31, v128f_31);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_28) & 7); i_33++) {
    __mmask16 m16_34 = _mm512_test_epi32_mask(v512i_30, p4);
    __m512i v512i_35 = _mm512_sllv_epi32(v512i_13, v512i_25);
    __m512h v512h_36 = _mm512_sqrt_ph(p1);
    __m512i v512i_37 = _mm512_conflict_epi32(p4);
    __m128h v128h_38 = g_v128h[10];
    __m512d v512d_39 = _mm512_cvtph_pd(v128h_38);
    }
    g_v512d[5] = v512d_14;
    g_v256f[1] = v256f_15;
    g_v256d[8] = v256d_12;
    g_v512h[9] = p1;
    return m8_9;
}

__m512d torture_251(__m128h p0, __m128d p1) {
    __m128d v128d_1 = _mm_fmadd_pd(p1, p1, p1);
    __m256 v256f_2 = _mm256_loadu_ps(fbuf);
    __m256 v256f_3 = _mm256_fmadd_ps(v256f_2, v256f_2, v256f_2);
    __m256d v256d_4 = _mm256_set1_pd(2.0);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_or_si128(v128i_5, v128i_5);
    __m256i v256i_7 = g_v256i[3];
    __m256i v256i_8 = _mm256_add_epi8(v256i_7, v256i_7);
    __m256i v256i_9 = _mm256_or_si256(v256i_7, v256i_8);
    __m128 v128f_10 = _mm_set1_ps(1.0f);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __mmask8 m8_12 = _mm512_cmp_pd_mask(v512d_11, v512d_11, _CMP_GE_OQ);
    __m512i v512i_13 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_14 = _mm512_slli_epi32(v512i_13, 3);
    __m512i v512i_15 = _mm512_ternarylogic_epi32(v512i_14, v512i_14, v512i_14, 0x96);
    __m256d v256d_16 = _mm512_extractf64x4_pd(v512d_11, 1);
    __m512i v512i_17 = _mm512_set1_epi32(0x55);
    __m128 v128f_18 = _mm_sqrt_ps(v128f_10);
    __m512 v512f_19 = g_v512f[4];
    __m512 v512f_20 = _mm512_unpacklo_ps(v512f_19, v512f_19);
    __m256i v256i_21 = _mm256_add_epi16(v256i_8, v256i_9);
    __m256i v256i_22 = _mm256_shuffle_epi8(v256i_9, v256i_21);
    __m128i v128i_23 = _mm_add_epi64(v128i_5, v128i_6);
    if (((unsigned long long)m8_12) & 1ULL) {
    __m128i v128i_24 = _mm_unpacklo_epi32(v128i_6, v128i_23);
    __m512d v512d_25 = _mm512_min_pd(v512d_11, v512d_11);
    __m128 v128f_26 = _mm_add_ps(v128f_10, v128f_10);
    __m128i v128i_27 = _mm_add_epi16(v128i_23, v128i_5);
    __m512i v512i_28 = _mm512_cvtps_epi32(v512f_20);
    __m512i v512i_29 = _mm512_lzcnt_epi32(v512i_17);
    }
    __m512d v512d_30 = _mm512_fmadd_pd(v512d_11, v512d_11, v512d_11);
    __m256 v256f_31 = _mm512_cvtpd_ps(v512d_30);
    __m128i v128i_32 = _mm_add_epi32(v128i_6, v128i_6);
    __mmask16 m16_33 = (__mmask16)(g_m16[0] ^ 44);
    __mmask16 m16_34 = _kxor_mask16(m16_33, m16_33);
    if (((unsigned long long)m16_34) & 1ULL) {
    __m512 v512f_35 = _mm512_fmsub_ps(v512f_19, v512f_20, v512f_19);
    __m256h v256h_36 = g_v256h[7];
    __m512 v512f_37 = _mm512_cvtxph_ps(v256h_36);
    __m128d v128d_38 = _mm_unpackhi_pd(v128d_1, v128d_1);
    }
    g_v512f[4] = v512f_19;
    g_v512i[8] = v512i_14;
    g_v256f[6] = v256f_2;
    return v512d_11;
}

__m128 torture_252(__m256d p0, __m128h p1, __m256d p2, __m128 p3) {
    __m512i v512i_1 = g_v512i[3];
    __mmask16 m16_2 = _mm512_movepi32_mask(v512i_1);
    __mmask8 m8_3 = (__mmask8)(g_m8[11] ^ 240);
    __mmask16 m16_4 = _mm512_kunpackb(m8_3, m8_3);
    __m512d v512d_5 = g_v512d[8];
    __m512i v512i_6 = _mm512_cvtpd_epi64(v512d_5);
    __m512 v512f_7 = g_v512f[3];
    __m512 v512f_8 = _mm512_rsqrt14_ps(v512f_7);
    __mmask8 m8_9 = _mm512_cmp_pd_mask(v512d_5, v512d_5, _CMP_GE_OQ);
    __m512i v512i_10 = _mm512_rolv_epi32(v512i_1, v512i_6);
    __m256i v256i_11 = g_v256i[8];
    __m256i v256i_12 = _mm256_sub_epi32(v256i_11, v256i_11);
    __m512 v512f_13 = _mm512_sqrt_ps(v512f_7);
    __m256d v256d_14 = _mm256_min_pd(p0, p0);
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_16 = _mm_slli_epi32(v128i_15, 3);
    __m256 v256f_17 = g_v256f[0];
    __m256 v256f_18 = _mm256_fnmadd_ps(v256f_17, v256f_17, v256f_17);
    __m512 v512f_19 = _mm512_rsqrt14_ps(v512f_8);
    __m128d v128d_20 = g_v128d[1];
    __m128d v128d_21 = _mm_min_pd(v128d_20, v128d_20);
    __m512i v512i_22 = _mm512_xor_si512(v512i_1, v512i_6);
    __m128i v128i_23 = _mm_unpacklo_epi32(v128i_15, v128i_16);
    __m512 v512f_24 = _mm512_rcp14_ps(v512f_7);
    __m128i v128i_25 = _mm_mullo_epi32(v128i_16, v128i_16);
    __m512i v512i_26 = _mm512_sub_epi64(v512i_10, v512i_1);
    __m512 v512f_27 = _mm512_scalef_ps(v512f_8, v512f_13);
    __m512i v512i_28 = _mm512_dpbusd_epi32(v512i_26, v512i_1, v512i_26);
    __m256i v256i_29 = _mm256_sub_epi8(v256i_11, v256i_11);
    __mmask16 m16_30 = _mm512_kunpackb(m8_3, m8_9);
    if (((unsigned long long)m16_30) & 1ULL) {
    __mmask8 m8_31 = _kand_mask8(m8_9, m8_9);
    __m128i v128i_32 = _mm_sub_epi16(v128i_15, v128i_23);
    __m128 v128f_33 = _mm_shuffle_ps(p3, p3, 0x1B);
    __m128 v128f_34 = _mm_sqrt_ps(p3);
    }
    g_v512d[10] = v512d_5;
    g_v256d[11] = v256d_14;
    g_v128f[3] = p3;
    g_v128d[6] = v128d_20;
    g_v128h[9] = p1;
    return p3;
}

__mmask8 torture_253(__m128 p0, __m256 p1, __m128d p2) {
    __m512d v512d_1 = g_v512d[1];
    __m512d v512d_2 = _mm512_max_pd(v512d_1, v512d_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_or_si512(v512i_3, v512i_3);
    __m512i v512i_5 = _mm512_shuffle_epi8(v512i_3, v512i_3);
    __m512d v512d_6 = _mm512_insertf64x2(v512d_1, p2, 3);
    __m512i v512i_7 = _mm512_dpbusd_epi32(v512i_5, v512i_5, v512i_5);
    __m128i v128i_8 = g_v128i[5];
    __m128i v128i_9 = _mm_abs_epi32(v128i_8);
    __m512d v512d_10 = _mm512_sub_pd(v512d_2, v512d_1);
    __m512d v512d_11 = _mm512_fmadd_pd(v512d_2, v512d_2, v512d_2);
    __m128h v128h_12 = _mm512_cvtpd_ph(v512d_10);
    __m512h v512h_13 = g_v512h[7];
    __m512h v512h_14 = _mm512_rcp_ph(v512h_13);
    __m128i v128i_15 = _mm_or_si128(v128i_8, v128i_8);
    __m256d v256d_16 = g_v256d[11];
    __m256d v256d_17 = _mm256_max_pd(v256d_16, v256d_16);
    __m512d v512d_18 = _mm512_add_pd(v512d_2, v512d_1);
    __mmask16 m16_19 = (__mmask16)(g_m16[11] ^ 45);
    __mmask16 m16_20 = _knot_mask16(m16_19);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_20) & 7); i_21++) {
    __m512i v512i_22 = _mm512_dpwssd_epi32(v512i_4, v512i_3, v512i_5);
    __m128i v128i_23 = _mm_shuffle_epi8(v128i_15, v128i_9);
    __m256i v256i_24 = g_v256i[1];
    __m256i v256i_25 = _mm256_add_epi8(v256i_24, v256i_24);
    }
    __m512 v512f_26 = _mm512_loadu_ps(fbuf);
    __m512 v512f_27 = _mm512_rcp14_ps(v512f_26);
    __m128d v128d_28 = _mm_sub_pd(p2, p2);
    __m512h v512h_29 = _mm512_mul_ph(v512h_13, v512h_13);
    __m256 v256f_30 = _mm256_max_ps(p1, p1);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_19) & 7); i_31++) {
    __m512 v512f_32 = _mm512_and_ps(v512f_26, v512f_26);
    __m128i v128i_33 = _mm_sub_epi32(v128i_15, v128i_9);
    }
    g_v512i[5] = v512i_5;
    g_v128f[1] = p0;
    g_v128d[10] = v128d_28;
    g_v128h[11] = v128h_12;
    __mmask8 m8_34 = (__mmask8)(g_m8[6] ^ 166);
    return m8_34;
}

__m128 torture_254(__m128h p0, __m512h p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m128i v128i_2 = _mm512_cvtepi32_epi8(v512i_1);
    __m256i v256i_3 = _mm512_cvtepi64_epi32(v512i_1);
    __m256d v256d_4 = g_v256d[7];
    __m256d v256d_5 = _mm256_fmadd_pd(v256d_4, v256d_4, v256d_4);
    __m512 v512f_6 = g_v512f[5];
    __m512 v512f_7 = _mm512_sub_ps(v512f_6, v512f_6);
    __m256d v256d_8 = _mm256_set1_pd(2.0);
    __mmask16 m16_9 = (__mmask16)(g_m16[7] ^ 173);
    __m512i v512i_10 = _mm512_broadcastmw_epi32(m16_9);
    __m512h v512h_11 = _mm512_rcp_ph(p1);
    __m512 v512f_12 = _mm512_mask3_fmadd_ps(v512f_6, v512f_7, v512f_7, m16_9);
    __m512 v512f_13 = _mm512_mul_ps(v512f_7, v512f_12);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_9) & 7); i_14++) {
    __m256 v256f_15 = g_v256f[6];
    __m256 v256f_16 = _mm256_shuffle_ps(v256f_15, v256f_15, 0x1B);
    __mmask16 m16_17 = _kand_mask16(m16_9, m16_9);
    __m128i v128i_18 = _mm_sub_epi16(v128i_2, v128i_2);
    }
    __m256d v256d_19 = _mm256_sub_pd(v256d_8, v256d_4);
    __m512d v512d_20 = g_v512d[8];
    __m512d v512d_21 = _mm512_fmadd_pd(v512d_20, v512d_20, v512d_20);
    __m512i v512i_22 = _mm512_sub_epi8(v512i_10, v512i_1);
    __m128i v128i_23 = _mm_set1_epi32(0x55);
    __m128i v128i_24 = _mm_mullo_epi32(v128i_2, v128i_23);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m512i v512i_25 = _mm512_and_si512(v512i_10, v512i_10);
    __m256i v256i_26 = _mm512_cvtepi64_epi32(v512i_22);
    __mmask16 m16_27 = _knot_mask16(m16_9);
    __m512 v512f_28 = _mm512_maskz_mul_ps(m16_27, v512f_13, v512f_7);
    __m512h v512h_29 = _mm512_sqrt_ph(v512h_11);
    __m512i v512i_30 = _mm512_set1_epi32(0x55);
    }
    g_v512f[9] = v512f_13;
    g_v512i[5] = v512i_22;
    g_v128i[3] = v128i_23;
    __m128 v128f_31 = g_v128f[4];
    return v128f_31;
}

__m128d torture_255(__m128i p0, __m128h p1, __m256h p2, __m512 p3, __m128i p4) {
    __m512i v512i_1 = _mm512_cvtepi8_epi32(p4);
    __m256i v256i_2 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_3 = _mm256_and_si256(v256i_2, v256i_2);
    __m512i v512i_4 = _mm512_shuffle_i32x4(v512i_1, v512i_1, 0x1B);
    __m128 v128f_5 = g_v128f[2];
    __m512 v512f_6 = _mm512_broadcast_f32x4(v128f_5);
    __m512i v512i_7 = _mm512_shuffle_epi8(v512i_4, v512i_1);
    __m512d v512d_8 = _mm512_set1_pd(2.0);
    __m256 v256f_9 = g_v256f[1];
    __m256 v256f_10 = _mm256_fnmadd_ps(v256f_9, v256f_9, v256f_9);
    __m128i v128i_11 = _mm_sllv_epi32(p4, p0);
    __m128i v128i_12 = _mm_slli_epi32(v128i_11, 3);
    __m128d v128d_13 = g_v128d[4];
    __m128d v128d_14 = _mm_fmadd_pd(v128d_13, v128d_13, v128d_13);
    __m512h v512h_15 = g_v512h[4];
    __m512h v512h_16 = _mm512_sqrt_ph(v512h_15);
    __m256i v256i_17 = _mm256_sub_epi16(v256i_3, v256i_2);
    int s32_18 = 36;
    if ((s32_18) & 1ULL) {
    __m512d v512d_19 = _mm512_max_pd(v512d_8, v512d_8);
    __mmask16 m16_20 = _mm512_movepi32_mask(v512i_1);
    __m256i v256i_21 = _mm256_add_epi32(v256i_17, v256i_2);
    }
    __m256 v256f_22 = _mm256_shuffle_ps(v256f_9, v256f_9, 0x1B);
    __m256d v256d_23 = g_v256d[8];
    __m256d v256d_24 = _mm256_sqrt_pd(v256d_23);
    __m512 v512f_25 = _mm512_insertf32x8(v512f_6, v256f_22, 1);
    __mmask16 m16_26 = (__mmask16)(g_m16[10] ^ 34);
    __mmask16 m16_27 = _kshiftli_mask16(m16_26, 2);
    __m512i v512i_28 = _mm512_rol_epi32(v512i_4, 5);
    for (int i_29 = 0; i_29 < (int)(((unsigned long long)m16_26) & 7); i_29++) {
    __m512i v512i_30 = _mm512_sub_epi8(v512i_1, v512i_1);
    __m512d v512d_31 = _mm512_insertf64x2(v512d_8, v128d_13, 3);
    __m512h v512h_32 = _mm512_getexp_ph(v512h_16);
    __m256i v256i_33 = _mm256_unpacklo_epi32(v256i_2, v256i_2);
    __mmask8 m8_34 = (__mmask8)(g_m8[1] ^ 118);
    __m512i v512i_35 = _mm512_broadcastmb_epi64(m8_34);
    }
    g_v512f[5] = v512f_6;
    g_v256i[0] = v256i_2;
    g_v128d[10] = v128d_13;
    g_v128i[11] = p4;
    g_v128h[4] = p1;
    return v128d_14;
}

__mmask16 torture_256(__m256h p0, __m512i p1, __m256 p2) {
    __m512i v512i_1 = _mm512_shuffle_epi8(p1, p1);
    __m256i v256i_2 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_3 = _mm256_sub_epi32(v256i_2, v256i_2);
    __m256i v256i_4 = _mm256_shuffle_epi8(v256i_3, v256i_2);
    __m256 v256f_5 = _mm256_and_ps(p2, p2);
    __m512d v512d_6 = _mm512_set1_pd(2.0);
    __m512 v512f_7 = g_v512f[0];
    __mmask16 m16_8 = _mm512_fpclass_ps_mask(v512f_7, 0x3);
    __m128i v128i_9 = g_v128i[8];
    __m128i v128i_10 = _mm_add_epi32(v128i_9, v128i_9);
    __m512h v512h_11 = g_v512h[2];
    __m512h v512h_12 = _mm512_getexp_ph(v512h_11);
    __m512i v512i_13 = _mm512_set1_epi32(0x55);
    __m512 v512f_14 = _mm512_rsqrt14_ps(v512f_7);
    __m128h v128h_15 = g_v128h[9];
    __m512d v512d_16 = _mm512_cvtph_pd(v128h_15);
    __m512h v512h_17 = _mm512_getexp_ph(v512h_11);
    unsigned u32_18 = _cvtmask16_u32(m16_8);
    __m128i v128i_19 = _mm_and_si128(v128i_10, v128i_9);
    __m512 v512f_20 = _mm512_sqrt_ps(v512f_14);
    __m128 v128f_21 = g_v128f[10];
    __m128 v128f_22 = _mm_div_ps(v128f_21, v128f_21);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_8) & 7); i_23++) {
    __m512i v512i_24 = _mm512_add_epi16(p1, v512i_1);
    __m128 v128f_25 = _mm_fmadd_ps(v128f_21, v128f_22, v128f_22);
    }
    __mmask64 m64_26 = _mm512_cmp_epi8_mask(v512i_1, v512i_13, 1);
    __m512i v512i_27 = _mm512_broadcast_i32x4(v128i_19);
    __mmask16 m16_28 = _mm512_cmp_epu32_mask(v512i_13, v512i_27, 4);
    __m256h v256h_29 = _mm512_cvtxps_ph(v512f_7);
    __m512i v512i_30 = _mm512_slli_epi32(v512i_1, 3);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_8) & 7); i_31++) {
    __m128i v128i_32 = _mm_mullo_epi32(v128i_19, v128i_9);
    __m256i v256i_33 = _mm256_xor_si256(v256i_4, v256i_3);
    __m256i v256i_34 = _mm256_abs_epi32(v256i_4);
    __m512d v512d_35 = _mm512_fmadd_pd(v512d_6, v512d_16, v512d_16);
    }
    g_v512f[3] = v512f_20;
    g_v512i[6] = v512i_1;
    g_v256f[6] = v256f_5;
    g_v256i[11] = v256i_3;
    g_v256h[10] = p0;
    g_v128h[4] = v128h_15;
    return m16_28;
}

__m256h torture_257(__m512h p0, __m256i p1) {
    __m256i v256i_1 = _mm256_add_epi8(p1, p1);
    __mmask16 m16_2 = (__mmask16)(g_m16[2] ^ 174);
    __mmask16 m16_3 = _kor_mask16(m16_2, m16_2);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_fnmadd_ps(v512f_4, v512f_4, v512f_4);
    __m256i v256i_6 = _mm256_add_epi8(v256i_1, p1);
    __m256h v256h_7 = g_v256h[5];
    __m512 v512f_8 = _mm512_cvtxph_ps(v256h_7);
    __m512i v512i_9 = g_v512i[10];
    __m512i v512i_10 = _mm512_shuffle_epi8(v512i_9, v512i_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_shuffle_f64x2(v512d_11, v512d_11, 0xEE);
    __m256i v256i_13 = _mm512_cvtepi64_epi32(v512i_10);
    __m512i v512i_14 = _mm512_sub_epi32(v512i_10, v512i_9);
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_16 = _mm_abs_epi32(v128i_15);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m128i v128i_17 = _mm_set1_epi32(0x55);
    __m256i v256i_18 = _mm256_and_si256(p1, v256i_13);
    __m512 v512f_19 = _mm512_fnmadd_ps(v512f_4, v512f_4, v512f_8);
    __m512i v512i_20 = _mm512_sllv_epi32(v512i_9, v512i_9);
    __m128i v128i_21 = _mm_sub_epi32(v128i_15, v128i_17);
    }
    __m256i v256i_22 = _mm256_abs_epi32(v256i_1);
    __m512i v512i_23 = _mm512_cvtps_epi32(v512f_5);
    __mmask16 m16_24 = _mm512_movepi32_mask(v512i_14);
    __m256d v256d_25 = g_v256d[6];
    __m256d v256d_26 = _mm256_div_pd(v256d_25, v256d_25);
    __m128 v128f_27 = g_v128f[1];
    __m128 v128f_28 = _mm_min_ps(v128f_27, v128f_27);
    __m512i v512i_29 = _mm512_shuffle_epi8(v512i_14, v512i_9);
    __m128d v128d_30 = g_v128d[0];
    __m128d v128d_31 = _mm_fmadd_pd(v128d_30, v128d_30, v128d_30);
    __m128i v128i_32 = _mm_or_si128(v128i_15, v128i_15);
    __m512i v512i_33 = _mm512_sub_epi32(v512i_29, v512i_10);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m256 v256f_34 = g_v256f[0];
    __m256 v256f_35 = _mm256_fmsub_ps(v256f_34, v256f_34, v256f_34);
    __m512 v512f_36 = _mm512_mul_ps(v512f_8, v512f_4);
    }
    g_v512f[2] = v512f_4;
    g_v512d[9] = v512d_11;
    g_v512i[4] = v512i_29;
    g_v128f[1] = v128f_27;
    g_m16[6] = m16_24;
    return v256h_7;
}

__m256 torture_258(__m256i p0, __m128h p1, __m128d p2) {
    __m512d v512d_1 = g_v512d[8];
    __mmask8 m8_2 = _mm512_cmp_pd_mask(v512d_1, v512d_1, _CMP_GE_OQ);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_xor_ps(v512f_3, v512f_3);
    __m512i v512i_5 = g_v512i[2];
    __m512i v512i_6 = _mm512_lzcnt_epi32(v512i_5);
    __m512h v512h_7 = g_v512h[10];
    __m512h v512h_8 = _mm512_getexp_ph(v512h_7);
    __m128d v128d_9 = _mm_max_pd(p2, p2);
    __mmask16 m16_10 = (__mmask16)(g_m16[10] ^ 14);
    __mmask16 m16_11 = _kxor_mask16(m16_10, m16_10);
    __m128 v128f_12 = g_v128f[2];
    __m128 v128f_13 = _mm_div_ps(v128f_12, v128f_12);
    __m512i v512i_14 = _mm512_mask_add_epi32(v512i_6, m16_11, v512i_6, v512i_5);
    __m512 v512f_15 = _mm512_rsqrt14_ps(v512f_4);
    __m512d v512d_16 = _mm512_unpackhi_pd(v512d_1, v512d_1);
    __mmask16 m16_17 = _kshiftri_mask16(m16_10, 1);
    __m256 v256f_18 = g_v256f[4];
    __m256 v256f_19 = _mm256_sub_ps(v256f_18, v256f_18);
    __m512 v512f_20 = _mm512_rsqrt14_ps(v512f_3);
    __m256 v256f_21 = _mm256_fnmadd_ps(v256f_19, v256f_18, v256f_18);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m512i v512i_22 = _mm512_xor_si512(v512i_6, v512i_6);
    __m512 v512f_23 = _mm512_and_ps(v512f_4, v512f_20);
    __mmask16 m16_24 = _knot_mask16(m16_17);
    __m512h v512h_25 = _mm512_sqrt_ph(v512h_8);
    __m128h v128h_26 = _mm512_cvtpd_ph(v512d_16);
    }
    __m256 v256f_27 = _mm256_fmadd_ps(v256f_19, v256f_21, v256f_19);
    __m512 v512f_28 = _mm512_mask3_fmadd_ps(v512f_20, v512f_3, v512f_15, m16_10);
    __m256d v256d_29 = _mm512_extractf64x4_pd(v512d_1, 1);
    __m512 v512f_30 = _mm512_shuffle_ps(v512f_3, v512f_4, 0x1B);
    __m128i v128i_31 = g_v128i[8];
    __m128i v128i_32 = _mm_unpacklo_epi32(v128i_31, v128i_31);
    __m256h v256h_33 = _mm512_cvtxps_ph(v512f_4);
    __m256i v256i_34 = _mm256_shuffle_epi8(p0, p0);
    __m512 v512f_35 = _mm512_fnmadd_ps(v512f_30, v512f_30, v512f_20);
    __m256 v256f_36 = _mm256_max_ps(v256f_19, v256f_21);
    g_v512d[11] = v512d_1;
    g_v256i[9] = v256i_34;
    g_v128d[4] = p2;
    g_v128i[6] = v128i_32;
    g_v128h[3] = p1;
    g_m16[11] = m16_10;
    return v256f_18;
}

__m512i torture_259(__m256d p0, __m256d p1, __m128 p2, __m128h p3, __m512 p4) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_shuffle_ps(v256f_1, v256f_1, 0x1B);
    __m512i v512i_3 = g_v512i[4];
    __mmask16 m16_4 = _mm512_movepi32_mask(v512i_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_sub_epi16(v128i_5, v128i_5);
    __mmask16 m16_7 = _mm512_cmp_epu32_mask(v512i_3, v512i_3, 4);
    __m512 v512f_8 = _mm512_rcp14_ps(p4);
    __mmask16 m16_9 = _kshiftri_mask16(m16_4, 1);
    __mmask16 m16_10 = _kor_mask16(m16_9, m16_4);
    __m512d v512d_11 = g_v512d[8];
    __mmask8 m8_12 = _mm512_cmp_pd_mask(v512d_11, v512d_11, _CMP_GE_OQ);
    __m512i v512i_13 = _mm512_permutexvar_epi32(v512i_3, v512i_3);
    __m512i v512i_14 = _mm512_shuffle_epi8(v512i_3, v512i_13);
    __m128 v128f_15 = _mm_xor_ps(p2, p2);
    __m512i v512i_16 = _mm512_add_epi32(v512i_14, v512i_14);
    __m512i v512i_17 = _mm512_abs_epi32(v512i_16);
    __m512 v512f_18 = _mm512_min_ps(p4, v512f_8);
    __m512i v512i_19 = _mm512_add_epi8(v512i_13, v512i_3);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_7) & 7); i_20++) {
    __m512h v512h_21 = g_v512h[8];
    __m512i v512i_22 = _mm512_cvtph_epi16(v512h_21);
    __m512i v512i_23 = _mm512_permutex2var_epi32(v512i_19, v512i_19, v512i_16);
    __m128d v128d_24 = _mm_set1_pd(2.0);
    __m512 v512f_25 = _mm512_max_ps(v512f_18, v512f_18);
    __m512d v512d_26 = _mm512_shuffle_f64x2(v512d_11, v512d_11, 0xEE);
    }
    __m128d v128d_27 = g_v128d[1];
    __m512d v512d_28 = _mm512_insertf64x2(v512d_11, v128d_27, 3);
    __m512h v512h_29 = g_v512h[11];
    __m512h v512h_30 = _mm512_mul_ph(v512h_29, v512h_29);
    __m128 v128f_31 = _mm_sqrt_ps(v128f_15);
    __m256h v256h_32 = g_v256h[0];
    __m512 v512f_33 = _mm512_cvtxph_ps(v256h_32);
    __m512h v512h_34 = _mm512_add_ph(v512h_30, v512h_29);
    __m256 v256f_35 = _mm256_unpacklo_ps(v256f_1, v256f_2);
    __m128 v128f_36 = _mm_xor_ps(p2, v128f_31);
    __m128i v128i_37 = _mm_set1_epi32(0x55);
    __m512 v512f_38 = _mm512_cvtxph_ps(v256h_32);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_7) & 7); i_39++) {
    __m512i v512i_40 = _mm512_mask_add_epi32(v512i_14, m16_7, v512i_19, v512i_13);
    __m128i v128i_41 = _mm_unpacklo_epi32(v128i_37, v128i_37);
    __m128i v128i_42 = _mm_xor_si128(v128i_37, v128i_5);
    __m256i v256i_43 = g_v256i[1];
    __m256i v256i_44 = _mm256_unpacklo_epi32(v256i_43, v256i_43);
    }
    g_v256d[5] = p1;
    g_v128d[6] = v128d_27;
    g_m8[9] = m8_12;
    return v512i_16;
}

__mmask16 torture_260(__m256i p0, __m128h p1, __m256d p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_xor_si512(v512i_1, v512i_1);
    __m512i v512i_3 = _mm512_rol_epi32(v512i_1, 5);
    __m512i v512i_4 = _mm512_madd52lo_epu64(v512i_3, v512i_3, v512i_1);
    __m512i v512i_5 = _mm512_add_epi16(v512i_1, v512i_3);
    __m512 v512f_6 = g_v512f[1];
    __m512 v512f_7 = _mm512_unpacklo_ps(v512f_6, v512f_6);
    __m128 v128f_8 = g_v128f[2];
    __m128 v128f_9 = _mm_sqrt_ps(v128f_8);
    __m512h v512h_10 = g_v512h[7];
    __m512h v512h_11 = _mm512_getexp_ph(v512h_10);
    __m512d v512d_12 = _mm512_loadu_pd(dbuf);
    __m512d v512d_13 = _mm512_min_pd(v512d_12, v512d_12);
    __m512i v512i_14 = _mm512_add_epi64(v512i_4, v512i_4);
    __mmask16 m16_15 = (__mmask16)(g_m16[10] ^ 65);
    __mmask16 m16_16 = _kor_mask16(m16_15, m16_15);
    __m256 v256f_17 = g_v256f[10];
    __m256 v256f_18 = _mm256_xor_ps(v256f_17, v256f_17);
    __m512 v512f_19 = _mm512_unpacklo_ps(v512f_7, v512f_6);
    __m512i v512i_20 = _mm512_cvtph_epi16(v512h_10);
    __m128d v128d_21 = g_v128d[7];
    __m128d v128d_22 = _mm_div_pd(v128d_21, v128d_21);
    __m128i v128i_23 = _mm_set1_epi32(0x55);
    __m128i v128i_24 = _mm_sub_epi8(v128i_23, v128i_23);
    if (((unsigned long long)m16_15) & 1ULL) {
    __m512 v512f_25 = _mm512_max_ps(v512f_19, v512f_19);
    __m128h v128h_26 = _mm512_cvtpd_ph(v512d_13);
    __m512d v512d_27 = _mm512_cvtph_pd(v128h_26);
    __m512 v512f_28 = _mm512_cvtepi32_ps(v512i_4);
    __mmask16 m16_29 = _mm512_movepi32_mask(v512i_5);
    }
    __m128i v128i_30 = _mm_sub_epi16(v128i_23, v128i_24);
    __m256i v256i_31 = _mm256_unpacklo_epi32(p0, p0);
    __m256 v256f_32 = _mm256_set1_ps(1.0f);
    __mmask16 m16_33 = _kand_mask16(m16_16, m16_16);
    __mmask8 m8_34 = (__mmask8)(g_m8[11] ^ 214);
    __mmask8 m8_35 = _kand_mask8(m8_34, m8_34);
    __m256i v256i_36 = _mm256_or_si256(p0, v256i_31);
    __m128i v128i_37 = _mm_mullo_epi32(v128i_24, v128i_30);
    __m256i v256i_38 = _mm512_cvtepi64_epi32(v512i_4);
    g_v512f[4] = v512f_19;
    g_v512d[8] = v512d_13;
    g_v512i[0] = v512i_4;
    g_v128d[5] = v128d_21;
    g_v128h[10] = p1;
    g_m16[0] = m16_16;
    return m16_33;
}

__m256h torture_261(__m128i p0, __m256d p1, __m512i p2, __m128 p3) {
    __m128 v128f_1 = _mm_xor_ps(p3, p3);
    __m256i v256i_2 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_3 = _mm256_sub_epi8(v256i_2, v256i_2);
    __m512i v512i_4 = _mm512_permutex2var_epi32(p2, p2, p2);
    __m512 v512f_5 = g_v512f[9];
    __m512 v512f_6 = _mm512_xor_ps(v512f_5, v512f_5);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_sub_ps(v256f_7, v256f_7);
    __m256 v256f_9 = _mm256_fmadd_ps(v256f_8, v256f_7, v256f_7);
    __m256i v256i_10 = _mm256_add_epi64(v256i_2, v256i_2);
    __mmask16 m16_11 = _mm512_movepi32_mask(p2);
    __mmask8 m8_12 = (__mmask8)(g_m8[11] ^ 36);
    __m512d v512d_13 = g_v512d[8];
    __m512d v512d_14 = _mm512_mask_blend_pd(m8_12, v512d_13, v512d_13);
    __m512i v512i_15 = _mm512_mullo_epi32(p2, p2);
    __m512h v512h_16 = g_v512h[10];
    __m512h v512h_17 = _mm512_fmsub_ph(v512h_16, v512h_16, v512h_16);
    __mmask8 m8_18 = _kand_mask8(m8_12, m8_12);
    for (int i_19 = 0; i_19 < (int)(((unsigned long long)m16_11) & 7); i_19++) {
    __m256i v256i_20 = _mm256_add_epi16(v256i_10, v256i_10);
    __mmask16 m16_21 = _kshiftli_mask16(m16_11, 2);
    __m512 v512f_22 = _mm512_fnmadd_ps(v512f_5, v512f_5, v512f_6);
    __m512h v512h_23 = _mm512_sqrt_ph(v512h_16);
    __m256 v256f_24 = _mm256_shuffle_ps(v256f_7, v256f_9, 0x1B);
    }
    __m256h v256h_25 = _mm512_cvtxps_ph(v512f_6);
    __m256 v256f_26 = _mm256_fmadd_ps(v256f_7, v256f_9, v256f_7);
    __m512d v512d_27 = _mm512_shuffle_f64x2(v512d_13, v512d_13, 0xEE);
    __m256d v256d_28 = _mm256_fmadd_pd(p1, p1, p1);
    __m512 v512f_29 = _mm512_unpacklo_ps(v512f_6, v512f_6);
    __m128d v128d_30 = g_v128d[8];
    __m128d v128d_31 = _mm_add_pd(v128d_30, v128d_30);
    __m128d v128d_32 = _mm_max_pd(v128d_30, v128d_30);
    __m256i v256i_33 = _mm256_sub_epi64(v256i_10, v256i_2);
    __m512i v512i_34 = _mm512_add_epi32(v512i_15, v512i_4);
    __m512i v512i_35 = _mm512_permutex2var_epi32(v512i_15, v512i_4, v512i_34);
    if (((unsigned long long)m16_11) & 1ULL) {
    __m256i v256i_36 = _mm256_and_si256(v256i_3, v256i_33);
    __m512i v512i_37 = _mm512_mask_add_epi32(p2, m16_11, v512i_35, v512i_4);
    __m512h v512h_38 = _mm512_getexp_ph(v512h_16);
    }
    g_v512h[0] = v512h_16;
    g_v256h[7] = v256h_25;
    g_m8[7] = m8_18;
    return v256h_25;
}

__mmask32 torture_262(__m256d p0, __m256d p1) {
    __mmask8 m8_1 = (__mmask8)(g_m8[4] ^ 165);
    __m512d v512d_2 = _mm512_loadu_pd(dbuf);
    __m512d v512d_3 = _mm512_mask_blend_pd(m8_1, v512d_2, v512d_2);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_unpacklo_ps(v512f_4, v512f_4);
    __m512i v512i_6 = g_v512i[7];
    __m512i v512i_7 = _mm512_sub_epi8(v512i_6, v512i_6);
    __mmask16 m16_8 = (__mmask16)(g_m16[10] ^ 85);
    __mmask16 m16_9 = _kshiftri_mask16(m16_8, 1);
    __m256d v256d_10 = _mm256_mul_pd(p0, p1);
    __m256i v256i_11 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_12 = _mm256_shuffle_epi8(v256i_11, v256i_11);
    __m128h v128h_13 = _mm512_cvtpd_ph(v512d_2);
    __m512 v512f_14 = _mm512_shuffle_ps(v512f_4, v512f_4, 0x1B);
    __m128 v128f_15 = _mm512_extractf32x4_ps(v512f_5, 2);
    __m512i v512i_16 = _mm512_lzcnt_epi32(v512i_6);
    unsigned u32_17 = 72;
    __mmask16 m16_18 = _cvtu32_mask16(u32_17);
    __m512 v512f_19 = _mm512_shuffle_f32x4(v512f_4, v512f_5, 0x4E);
    __m128i v128i_20 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_21 = _mm_sub_epi8(v128i_20, v128i_20);
    __m512i v512i_22 = _mm512_sub_epi8(v512i_7, v512i_16);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_9) & 7); i_23++) {
    __m512i v512i_24 = _mm512_madd52lo_epu64(v512i_22, v512i_16, v512i_7);
    __mmask16 m16_25 = _cvtu32_mask16(u32_17);
    }
    __mmask16 m16_26 = _kshiftli_mask16(m16_18, 2);
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m256 v256f_28 = _mm256_fnmadd_ps(v256f_27, v256f_27, v256f_27);
    __m512d v512d_29 = _mm512_unpackhi_pd(v512d_2, v512d_2);
    __m256d v256d_30 = _mm256_set1_pd(2.0);
    __mmask16 m16_31 = _mm512_cmp_epu32_mask(v512i_22, v512i_22, 4);
    __m512i v512i_32 = _mm512_slli_epi32(v512i_7, 3);
    __m128i v128i_33 = _mm_sub_epi8(v128i_20, v128i_20);
    __m128d v128d_34 = g_v128d[0];
    __m128d v128d_35 = _mm_mul_pd(v128d_34, v128d_34);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_9) & 7); i_36++) {
    __m128i v128i_37 = _mm_mullo_epi32(v128i_21, v128i_20);
    __m512 v512f_38 = _mm512_sub_ps(v512f_5, v512f_19);
    }
    g_v256f[8] = v256f_28;
    g_v256i[1] = v256i_11;
    g_v128d[7] = v128d_35;
    g_v128h[3] = v128h_13;
    g_m8[6] = m8_1;
    __mmask32 m32_39 = (__mmask32)(g_m32[4] ^ 80);
    return m32_39;
}

__m512i torture_263(__m128d p0, __m128i p1, __m512i p2, __m128h p3) {
    __m512i v512i_1 = _mm512_rolv_epi32(p2, p2);
    unsigned u32_2 = 12;
    __mmask16 m16_3 = _cvtu32_mask16(u32_2);
    __m256i v256i_4 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_5 = _mm256_slli_epi32(v256i_4, 3);
    __m512i v512i_6 = _mm512_cvtepi8_epi32(p1);
    __m512d v512d_7 = g_v512d[5];
    __m512d v512d_8 = _mm512_min_pd(v512d_7, v512d_7);
    __m512d v512d_9 = _mm512_set1_pd(2.0);
    __m512i v512i_10 = _mm512_sub_epi16(v512i_6, v512i_1);
    __m128i v128i_11 = _mm_sllv_epi32(p1, p1);
    __mmask8 m8_12 = _mm512_cmp_pd_mask(v512d_9, v512d_8, _CMP_GE_OQ);
    __m256 v256f_13 = _mm256_loadu_ps(fbuf);
    __m256 v256f_14 = _mm256_xor_ps(v256f_13, v256f_13);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m128d v128d_15 = _mm_mul_pd(p0, p0);
    __m512i v512i_16 = _mm512_or_si512(v512i_6, v512i_1);
    __m128d v128d_17 = _mm_add_pd(p0, p0);
    __m512i v512i_18 = _mm512_lzcnt_epi32(v512i_1);
    }
    __m128 v128f_19 = g_v128f[2];
    __m128 v128f_20 = _mm_fmsub_ps(v128f_19, v128f_19, v128f_19);
    __m128i v128i_21 = _mm_sub_epi32(p1, v128i_11);
    __m128i v128i_22 = _mm_shuffle_epi8(v128i_11, v128i_21);
    __m512 v512f_23 = _mm512_loadu_ps(fbuf);
    __m128 v128f_24 = _mm512_extractf32x4_ps(v512f_23, 2);
    __mmask16 m16_25 = _mm512_kunpackb(m8_12, m8_12);
    __m512d v512d_26 = _mm512_mask_blend_pd(m8_12, v512d_7, v512d_7);
    __m512i v512i_27 = _mm512_rolv_epi32(v512i_1, p2);
    if (((unsigned long long)m16_25) & 1ULL) {
    __mmask16 m16_28 = _cvtu32_mask16(u32_2);
    __m512 v512f_29 = _mm512_sub_ps(v512f_23, v512f_23);
    }
    g_v512f[1] = v512f_23;
    g_v512d[8] = v512d_8;
    g_v512i[4] = v512i_6;
    g_v128f[8] = v128f_24;
    g_v128d[1] = p0;
    g_v128h[8] = p3;
    g_m8[11] = m8_12;
    return p2;
}

__mmask8 torture_264(__m512i p0, __m128i p1, __m128i p2, __m256 p3) {
    __m128i v128i_1 = _mm_slli_epi32(p2, 3);
    __m512d v512d_2 = g_v512d[9];
    __m128d v128d_3 = g_v128d[1];
    __m512d v512d_4 = _mm512_insertf64x2(v512d_2, v128d_3, 3);
    __m512h v512h_5 = g_v512h[10];
    __m512h v512h_6 = _mm512_fmadd_ph(v512h_5, v512h_5, v512h_5);
    __m128d v128d_7 = _mm_div_pd(v128d_3, v128d_3);
    __m256d v256d_8 = g_v256d[2];
    __m256d v256d_9 = _mm256_div_pd(v256d_8, v256d_8);
    __m256 v256f_10 = _mm256_sub_ps(p3, p3);
    __m512d v512d_11 = _mm512_set1_pd(2.0);
    __m256 v256f_12 = _mm256_min_ps(p3, p3);
    __m128d v128d_13 = _mm_unpackhi_pd(v128d_3, v128d_7);
    int s32_14 = 33;
    if ((s32_14) & 1ULL) {
    __m128i v128i_15 = _mm_add_epi16(p1, v128i_1);
    __m512d v512d_16 = _mm512_shuffle_f64x2(v512d_4, v512d_11, 0xEE);
    __m512 v512f_17 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_18 = _mm512_fpclass_ps_mask(v512f_17, 0x3);
    __m512 v512f_19 = _mm512_scalef_ps(v512f_17, v512f_17);
    __m512i v512i_20 = _mm512_rolv_epi32(p0, p0);
    }
    __mmask32 m32_21 = _mm512_cmp_ph_mask(v512h_6, v512h_6, 1);
    __m256i v256i_22 = g_v256i[5];
    __m256i v256i_23 = _mm256_add_epi64(v256i_22, v256i_22);
    __m512 v512f_24 = _mm512_loadu_ps(fbuf);
    __m512 v512f_25 = _mm512_scalef_ps(v512f_24, v512f_24);
    __m512 v512f_26 = _mm512_max_ps(v512f_25, v512f_25);
    __m512 v512f_27 = _mm512_add_ps(v512f_25, v512f_25);
    __m256i v256i_28 = _mm256_unpacklo_epi32(v256i_23, v256i_22);
    __m512i v512i_29 = _mm512_slli_epi32(p0, 3);
    __m512i v512i_30 = _mm512_and_si512(p0, v512i_29);
    __m512 v512f_31 = _mm512_shuffle_f32x4(v512f_26, v512f_26, 0x4E);
    __mmask64 m64_32 = _mm512_cmp_epi8_mask(v512i_30, p0, 1);
    g_v256d[10] = v256d_9;
    g_v128d[0] = v128d_3;
    g_v128i[1] = p2;
    g_v512h[8] = v512h_5;
    __mmask8 m8_33 = (__mmask8)(g_m8[10] ^ 250);
    return m8_33;
}

__m512h torture_265(__m512h p0, __m256 p1) {
    __m512i v512i_1 = g_v512i[0];
    __m128i v128i_2 = _mm512_cvtepi32_epi8(v512i_1);
    __m256 v256f_3 = _mm256_sub_ps(p1, p1);
    __m512 v512f_4 = g_v512f[11];
    __m512 v512f_5 = _mm512_mul_ps(v512f_4, v512f_4);
    __m128 v128f_6 = g_v128f[3];
    __m128 v128f_7 = _mm_shuffle_ps(v128f_6, v128f_6, 0x1B);
    __m512i v512i_8 = _mm512_cvtepi8_epi32(v128i_2);
    __m128i v128i_9 = _mm512_cvtepi32_epi8(v512i_8);
    __m128 v128f_10 = _mm_sub_ps(v128f_6, v128f_6);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_div_pd(v512d_11, v512d_11);
    __mmask16 m16_13 = (__mmask16)(g_m16[2] ^ 204);
    __mmask16 m16_14 = _kxor_mask16(m16_13, m16_13);
    __m128 v128f_15 = _mm_fnmadd_ps(v128f_6, v128f_7, v128f_6);
    __m128 v128f_16 = _mm_shuffle_ps(v128f_15, v128f_15, 0x1B);
    __m256i v256i_17 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_18 = _mm256_add_epi16(v256i_17, v256i_17);
    __m512i v512i_19 = _mm512_permutexvar_epi32(v512i_8, v512i_8);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512 v512f_20 = _mm512_sub_ps(v512f_4, v512f_4);
    __m512 v512f_21 = _mm512_maskz_mul_ps(m16_13, v512f_4, v512f_4);
    __m128i v128i_22 = _mm_unpacklo_epi32(v128i_2, v128i_2);
    }
    __m512 v512f_23 = _mm512_mask_blend_ps(m16_14, v512f_4, v512f_4);
    __m256i v256i_24 = _mm256_and_si256(v256i_17, v256i_18);
    __mmask16 m16_25 = _mm512_fpclass_ps_mask(v512f_5, 0x3);
    __m512 v512f_26 = _mm512_div_ps(v512f_23, v512f_23);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_25) & 7); i_27++) {
    __m256i v256i_28 = _mm256_sub_epi16(v256i_18, v256i_17);
    __m128d v128d_29 = g_v128d[1];
    __m128d v128d_30 = _mm_sub_pd(v128d_29, v128d_29);
    }
    g_v512i[10] = v512i_1;
    g_v128f[9] = v128f_7;
    g_v512h[3] = p0;
    return p0;
}

__mmask32 torture_266(__m256h p0, __m256h p1, __m128 p2, __m128 p3, __m256i p4) {
    __m512 v512f_1 = _mm512_broadcast_f32x4(p2);
    __m512i v512i_2 = g_v512i[3];
    __m512i v512i_3 = _mm512_add_epi16(v512i_2, v512i_2);
    __m512 v512f_4 = _mm512_mul_ps(v512f_1, v512f_1);
    __m512i v512i_5 = _mm512_and_si512(v512i_3, v512i_3);
    __mmask8 m8_6 = (__mmask8)(g_m8[3] ^ 238);
    __m512i v512i_7 = _mm512_broadcastmb_epi64(m8_6);
    __m512i v512i_8 = _mm512_sub_epi16(v512i_2, v512i_3);
    __m256i v256i_9 = _mm256_unpacklo_epi32(p4, p4);
    __m128i v128i_10 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_11 = _mm_or_si128(v128i_10, v128i_10);
    __mmask16 m16_12 = _mm512_movepi32_mask(v512i_5);
    __m128i v128i_13 = _mm_slli_epi32(v128i_10, 3);
    __m512h v512h_14 = g_v512h[9];
    __m512h v512h_15 = _mm512_rcp_ph(v512h_14);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_12) & 7); i_16++) {
    __m512i v512i_17 = _mm512_cvtps_epi32(v512f_4);
    __m512 v512f_18 = _mm512_mask_blend_ps(m16_12, v512f_4, v512f_4);
    __m128i v128i_19 = _mm_add_epi64(v128i_11, v128i_13);
    __m128 v128f_20 = _mm_fmsub_ps(p2, p2, p3);
    __m512d v512d_21 = g_v512d[2];
    __m256d v256d_22 = _mm512_extractf64x4_pd(v512d_21, 1);
    }
    __m512i v512i_23 = _mm512_broadcast_i32x4(v128i_10);
    __m256d v256d_24 = g_v256d[4];
    __m256d v256d_25 = _mm256_fmadd_pd(v256d_24, v256d_24, v256d_24);
    __m512i v512i_26 = _mm512_sub_epi64(v512i_8, v512i_7);
    __mmask64 m64_27 = _mm512_cmp_epi8_mask(v512i_23, v512i_8, 1);
    __m512i v512i_28 = _mm512_popcnt_epi64(v512i_7);
    __mmask32 m32_29 = _mm512_cmp_ph_mask(v512h_15, v512h_15, 1);
    __m512 v512f_30 = _mm512_cvtepi32_ps(v512i_8);
    __m512 v512f_31 = _mm512_unpacklo_ps(v512f_1, v512f_4);
    __m128i v128i_32 = _mm_and_si128(v128i_10, v128i_13);
    __m128i v128i_33 = _mm_abs_epi32(v128i_13);
    __m256i v256i_34 = _mm256_set1_epi32(0x55);
    for (int i_35 = 0; i_35 < (int)(((unsigned long long)m16_12) & 7); i_35++) {
    __m512 v512f_36 = _mm512_set1_ps(1.0f);
    __m512 v512f_37 = _mm512_rsqrt14_ps(v512f_4);
    __m512i v512i_38 = _mm512_abs_epi32(v512i_28);
    __m128d v128d_39 = g_v128d[0];
    __m128d v128d_40 = _mm_unpackhi_pd(v128d_39, v128d_39);
    __m512d v512d_41 = _mm512_set1_pd(2.0);
    }
    g_v512i[10] = v512i_8;
    g_v256d[2] = v256d_25;
    g_v256i[7] = v256i_34;
    g_v128f[3] = p2;
    g_v128i[1] = v128i_11;
    g_v512h[3] = v512h_14;
    g_m64[4] = m64_27;
    return m32_29;
}

__m512i torture_267(__m512h p0, __m512 p1) {
    __m128i v128i_1 = g_v128i[10];
    __m128i v128i_2 = _mm_sub_epi32(v128i_1, v128i_1);
    __m512 v512f_3 = _mm512_rsqrt14_ps(p1);
    __mmask16 m16_4 = _mm512_cmp_ps_mask(p1, p1, _CMP_LT_OQ);
    __mmask32 m32_5 = _mm512_kunpackw(m16_4, m16_4);
    __m512i v512i_6 = g_v512i[4];
    __m512i v512i_7 = _mm512_sub_epi16(v512i_6, v512i_6);
    __m512d v512d_8 = g_v512d[10];
    __mmask8 m8_9 = _mm512_cmp_pd_mask(v512d_8, v512d_8, _CMP_GE_OQ);
    __m128i v128i_10 = _mm_sub_epi32(v128i_2, v128i_2);
    __m512i v512i_11 = _mm512_slli_epi32(v512i_7, 3);
    __m512 v512f_12 = _mm512_unpacklo_ps(p1, v512f_3);
    __mmask32 m32_13 = _mm512_cmp_ph_mask(p0, p0, 1);
    __m512 v512f_14 = _mm512_cvtepi32_ps(v512i_6);
    __m512h v512h_15 = _mm512_getexp_ph(p0);
    __mmask16 m16_16 = _kshiftli_mask16(m16_4, 2);
    __m128 v128f_17 = _mm_loadu_ps(fbuf);
    __m128 v128f_18 = _mm_fnmadd_ps(v128f_17, v128f_17, v128f_17);
    __m128 v128f_19 = _mm_sqrt_ps(v128f_17);
    __m256 v256f_20 = _mm256_loadu_ps(fbuf);
    __m256 v256f_21 = _mm256_fmsub_ps(v256f_20, v256f_20, v256f_20);
    __mmask16 m16_22 = _mm512_cmp_ps_mask(v512f_12, v512f_12, _CMP_LT_OQ);
    __mmask64 m64_23 = _mm512_cmp_epi8_mask(v512i_6, v512i_6, 1);
    __m512 v512f_24 = _mm512_min_ps(p1, p1);
    __m256d v256d_25 = g_v256d[5];
    __m256d v256d_26 = _mm256_div_pd(v256d_25, v256d_25);
    __m512i v512i_27 = _mm512_madd52lo_epu64(v512i_11, v512i_7, v512i_7);
    __m512i v512i_28 = _mm512_cvtepi8_epi32(v128i_1);
    __m512d v512d_29 = _mm512_unpackhi_pd(v512d_8, v512d_8);
    __m256i v256i_30 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_31 = _mm256_mullo_epi32(v256i_30, v256i_30);
    g_v512f[4] = v512f_24;
    g_v512d[8] = v512d_29;
    g_v128f[7] = v128f_17;
    g_v128i[9] = v128i_2;
    g_m8[7] = m8_9;
    g_m64[7] = m64_23;
    return v512i_7;
}

__m256d torture_268(__m256i p0, __m128h p1) {
    __m128 v128f_1 = g_v128f[3];
    __m128 v128f_2 = _mm_sqrt_ps(v128f_1);
    __m512i v512i_3 = g_v512i[3];
    __m512i v512i_4 = _mm512_add_epi32(v512i_3, v512i_3);
    __m128d v128d_5 = g_v128d[11];
    __m128d v128d_6 = _mm_sub_pd(v128d_5, v128d_5);
    __m128d v128d_7 = _mm_mul_pd(v128d_5, v128d_5);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_9 = _mm_unpacklo_epi32(v128i_8, v128i_8);
    __m256 v256f_10 = g_v256f[10];
    __m256 v256f_11 = _mm256_sub_ps(v256f_10, v256f_10);
    __mmask16 m16_12 = (__mmask16)(g_m16[7] ^ 28);
    __mmask16 m16_13 = _kshiftli_mask16(m16_12, 2);
    __m512i v512i_14 = _mm512_shuffle_epi8(v512i_4, v512i_3);
    __m256d v256d_15 = g_v256d[8];
    __m256d v256d_16 = _mm256_div_pd(v256d_15, v256d_15);
    __m512i v512i_17 = _mm512_rol_epi32(v512i_4, 5);
    __m256d v256d_18 = _mm256_min_pd(v256d_16, v256d_16);
    __m512 v512f_19 = _mm512_loadu_ps(fbuf);
    __m128 v128f_20 = _mm512_extractf32x4_ps(v512f_19, 2);
    __m128i v128i_21 = _mm_mullo_epi32(v128i_8, v128i_9);
    __m128i v128i_22 = _mm_slli_epi32(v128i_8, 3);
    if (((unsigned long long)m16_13) & 1ULL) {
    __mmask16 m16_23 = _kand_mask16(m16_12, m16_13);
    __m512h v512h_24 = g_v512h[10];
    __m512h v512h_25 = _mm512_sqrt_ph(v512h_24);
    __m256 v256f_26 = _mm256_sub_ps(v256f_11, v256f_11);
    __m512i v512i_27 = _mm512_permutexvar_epi32(v512i_14, v512i_4);
    __mmask64 m64_28 = _mm512_movepi8_mask(v512i_4);
    __m512d v512d_29 = _mm512_loadu_pd(dbuf);
    __m256 v256f_30 = _mm512_cvtpd_ps(v512d_29);
    }
    __m512 v512f_31 = _mm512_sub_ps(v512f_19, v512f_19);
    __m512i v512i_32 = _mm512_xor_si512(v512i_14, v512i_14);
    __m256i v256i_33 = _mm256_add_epi32(p0, p0);
    __m512i v512i_34 = _mm512_slli_epi32(v512i_14, 3);
    __m128 v128f_35 = _mm_div_ps(v128f_1, v128f_1);
    __m512i v512i_36 = _mm512_dpwssd_epi32(v512i_3, v512i_14, v512i_14);
    __m512i v512i_37 = _mm512_add_epi8(v512i_36, v512i_34);
    for (int i_38 = 0; i_38 < (int)(((unsigned long long)m16_13) & 7); i_38++) {
    __m512d v512d_39 = _mm512_loadu_pd(dbuf);
    __m512d v512d_40 = _mm512_shuffle_f64x2(v512d_39, v512d_39, 0xEE);
    __m128i v128i_41 = _mm_add_epi64(v128i_8, v128i_9);
    }
    g_v512f[1] = v512f_19;
    g_v256f[7] = v256f_10;
    g_v256i[10] = v256i_33;
    g_v128f[4] = v128f_20;
    return v256d_18;
}

__m128h torture_269(__m128i p0, __m512d p1) {
    __m512i v512i_1 = _mm512_cvtepi8_epi32(p0);
    __m128i v128i_2 = _mm_set1_epi32(0x55);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_sub_ps(v128f_3, v128f_3);
    __m256 v256f_5 = _mm256_loadu_ps(fbuf);
    __m256 v256f_6 = _mm256_sub_ps(v256f_5, v256f_5);
    __m128d v128d_7 = g_v128d[9];
    __m128d v128d_8 = _mm_div_pd(v128d_7, v128d_7);
    __mmask16 m16_9 = _mm512_test_epi32_mask(v512i_1, v512i_1);
    __m256 v256f_10 = _mm256_shuffle_ps(v256f_5, v256f_5, 0x1B);
    __m256i v256i_11 = g_v256i[1];
    __m256i v256i_12 = _mm256_add_epi64(v256i_11, v256i_11);
    __m512i v512i_13 = _mm512_add_epi8(v512i_1, v512i_1);
    __m128i v128i_14 = _mm_add_epi8(v128i_2, v128i_2);
    __m512 v512f_15 = _mm512_set1_ps(1.0f);
    __m512 v512f_16 = _mm512_max_ps(v512f_15, v512f_15);
    __m128 v128f_17 = _mm_fnmadd_ps(v128f_4, v128f_4, v128f_3);
    __m256 v256f_18 = _mm256_fnmadd_ps(v256f_6, v256f_5, v256f_6);
    for (int i_19 = 0; i_19 < (int)(((unsigned long long)m16_9) & 7); i_19++) {
    __m128i v128i_20 = _mm_sub_epi16(p0, v128i_2);
    __m128i v128i_21 = _mm_sub_epi16(v128i_20, v128i_20);
    __m512 v512f_22 = _mm512_shuffle_f32x4(v512f_16, v512f_15, 0x4E);
    __mmask64 m64_23 = (__mmask64)(g_m64[8] ^ 68);
    __mmask64 m64_24 = _kor_mask64(m64_23, m64_23);
    }
    __m512i v512i_25 = _mm512_mullo_epi32(v512i_1, v512i_13);
    __m512i v512i_26 = _mm512_dpwssd_epi32(v512i_25, v512i_25, v512i_1);
    __m256h v256h_27 = g_v256h[9];
    __m512 v512f_28 = _mm512_cvtxph_ps(v256h_27);
    __m256i v256i_29 = _mm256_sub_epi64(v256i_11, v256i_12);
    __mmask16 m16_30 = _kshiftli_mask16(m16_9, 2);
    if (((unsigned long long)m16_30) & 1ULL) {
    __m512i v512i_31 = _mm512_rolv_epi32(v512i_25, v512i_25);
    __m512h v512h_32 = g_v512h[7];
    __m512i v512i_33 = _mm512_cvtph_epi16(v512h_32);
    __m128d v128d_34 = _mm_add_pd(v128d_8, v128d_8);
    __mmask8 m8_35 = (__mmask8)(g_m8[8] ^ 230);
    __m512d v512d_36 = _mm512_mask_blend_pd(m8_35, p1, p1);
    __m512i v512i_37 = _mm512_sub_epi64(v512i_1, v512i_33);
    __m128h v128h_38 = g_v128h[6];
    __m512d v512d_39 = _mm512_cvtph_pd(v128h_38);
    }
    g_v512d[2] = p1;
    g_v512i[5] = v512i_1;
    g_v256i[9] = v256i_29;
    g_v128f[4] = v128f_4;
    g_v256h[5] = v256h_27;
    g_m16[0] = m16_9;
    __m128h v128h_40 = g_v128h[7];
    return v128h_40;
}

__m256 torture_270(__m512h p0, __m128h p1, __m256d p2, __m128d p3) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_add_epi32(v256i_1, v256i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_4 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    __mmask8 m8_5 = (__mmask8)(g_m8[3] ^ 105);
    __m512i v512i_6 = _mm512_broadcastmb_epi64(m8_5);
    __mmask32 m32_7 = (__mmask32)(g_m32[5] ^ 105);
    __mmask32 m32_8 = _kxor_mask32(m32_7, m32_7);
    __mmask16 m16_9 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    __m128d v128d_10 = _mm_sqrt_pd(p3);
    __m128i v128i_11 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_12 = _mm_sllv_epi32(v128i_11, v128i_11);
    __m128i v128i_13 = _mm_add_epi8(v128i_12, v128i_11);
    __m128 v128f_14 = _mm_loadu_ps(fbuf);
    __m128 v128f_15 = _mm_and_ps(v128f_14, v128f_14);
    __m512i v512i_16 = _mm512_add_epi64(v512i_6, v512i_6);
    __m256i v256i_17 = _mm256_sub_epi32(v256i_1, v256i_1);
    __m256i v256i_18 = _mm256_add_epi8(v256i_17, v256i_2);
    __m512 v512f_19 = _mm512_maskz_mul_ps(m16_9, v512f_3, v512f_3);
    __m512d v512d_20 = _mm512_set1_pd(2.0);
    __m128i v128i_21 = _mm_sub_epi64(v128i_12, v128i_13);
    __m512 v512f_22 = _mm512_getexp_ps(v512f_3);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_9) & 7); i_23++) {
    __m256d v256d_24 = _mm256_add_pd(p2, p2);
    __m128d v128d_25 = _mm_set1_pd(2.0);
    __m128i v128i_26 = _mm_add_epi8(v128i_12, v128i_11);
    __m512 v512f_27 = _mm512_maskz_mul_ps(m16_9, v512f_22, v512f_19);
    __m512d v512d_28 = _mm512_min_pd(v512d_20, v512d_20);
    }
    __m128d v128d_29 = _mm_min_pd(v128d_10, v128d_10);
    __m512 v512f_30 = _mm512_max_ps(v512f_19, v512f_3);
    __m512h v512h_31 = _mm512_add_ph(p0, p0);
    __m128i v128i_32 = _mm_sub_epi8(v128i_13, v128i_21);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m512d v512d_33 = _mm512_cvtph_pd(p1);
    __m512 v512f_34 = _mm512_maskz_mul_ps(m16_9, v512f_3, v512f_30);
    }
    g_v256i[11] = v256i_1;
    g_m32[4] = m32_8;
    __m256 v256f_35 = _mm256_loadu_ps(fbuf);
    return v256f_35;
}

__m512h torture_271(__m256 p0, __m256 p1) {
    __m512i v512i_1 = g_v512i[4];
    __m512i v512i_2 = _mm512_add_epi64(v512i_1, v512i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_4 = (__mmask16)(g_m16[6] ^ 205);
    __m512 v512f_5 = _mm512_mask_add_ps(v512f_3, m16_4, v512f_3, v512f_3);
    __m256i v256i_6 = g_v256i[9];
    __m256i v256i_7 = _mm256_add_epi64(v256i_6, v256i_6);
    __m256d v256d_8 = g_v256d[9];
    __m256d v256d_9 = _mm256_mul_pd(v256d_8, v256d_8);
    __mmask32 m32_10 = _mm512_kunpackw(m16_4, m16_4);
    __m512h v512h_11 = g_v512h[5];
    __mmask32 m32_12 = _mm512_cmp_ph_mask(v512h_11, v512h_11, 1);
    __m256i v256i_13 = _mm256_abs_epi32(v256i_7);
    __m512i v512i_14 = _mm512_conflict_epi32(v512i_1);
    __m256 v256f_15 = _mm256_sqrt_ps(p0);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m128i v128i_16 = g_v128i[6];
    __m128i v128i_17 = _mm_shuffle_epi8(v128i_16, v128i_16);
    __m512 v512f_18 = _mm512_set1_ps(1.0f);
    __m512d v512d_19 = g_v512d[2];
    __m512d v512d_20 = _mm512_fmadd_pd(v512d_19, v512d_19, v512d_19);
    __m512i v512i_21 = _mm512_mullo_epi32(v512i_1, v512i_2);
    __m256 v256f_22 = _mm256_shuffle_ps(p0, p0, 0x1B);
    __m128i v128i_23 = _mm_add_epi8(v128i_17, v128i_16);
    }
    __mmask16 m16_24 = _mm512_movepi32_mask(v512i_2);
    __mmask16 m16_25 = _mm512_cmp_epu32_mask(v512i_2, v512i_1, 4);
    __m256i v256i_26 = _mm256_add_epi8(v256i_7, v256i_7);
    __mmask16 m16_27 = _mm512_movepi32_mask(v512i_2);
    __m256i v256i_28 = _mm256_and_si256(v256i_7, v256i_26);
    __m128i v128i_29 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_30 = _mm_abs_epi32(v128i_29);
    __m256d v256d_31 = _mm256_fmadd_pd(v256d_9, v256d_9, v256d_9);
    for (int i_32 = 0; i_32 < (int)(((unsigned long long)m16_25) & 7); i_32++) {
    unsigned u32_33 = 1;
    __mmask16 m16_34 = _cvtu32_mask16(u32_33);
    __m128i v128i_35 = _mm512_cvtepi32_epi8(v512i_2);
    __m256d v256d_36 = _mm256_set1_pd(2.0);
    __m512i v512i_37 = _mm512_xor_si512(v512i_1, v512i_2);
    }
    g_v512h[4] = v512h_11;
    g_m16[1] = m16_27;
    return v512h_11;
}

__m256d torture_272(__m512i p0, __m256d p1, __m512i p2, __m512d p3) {
    __mmask64 m64_1 = (__mmask64)(g_m64[7] ^ 204);
    __mmask64 m64_2 = _kor_mask64(m64_1, m64_1);
    __m512 v512f_3 = g_v512f[9];
    __m512 v512f_4 = _mm512_fnmadd_ps(v512f_3, v512f_3, v512f_3);
    __m512 v512f_5 = _mm512_roundscale_ps(v512f_4, 0x10);
    __m512i v512i_6 = _mm512_abs_epi32(p2);
    __m128d v128d_7 = g_v128d[11];
    __m128d v128d_8 = _mm_unpackhi_pd(v128d_7, v128d_7);
    __mmask16 m16_9 = _mm512_testn_epi32_mask(v512i_6, p0);
    __m256i v256i_10 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_11 = _mm256_slli_epi32(v256i_10, 3);
    __m256h v256h_12 = g_v256h[11];
    __m512 v512f_13 = _mm512_cvtxph_ps(v256h_12);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m256i v256i_14 = _mm256_slli_epi32(v256i_10, 3);
    __mmask32 m32_15 = (__mmask32)(g_m32[8] ^ 140);
    __mmask32 m32_16 = _kxor_mask32(m32_15, m32_15);
    __m256i v256i_17 = _mm256_add_epi8(v256i_11, v256i_14);
    }
    __m128 v128f_18 = g_v128f[1];
    __m128 v128f_19 = _mm_mul_ps(v128f_18, v128f_18);
    __m256 v256f_20 = _mm256_loadu_ps(fbuf);
    __m256 v256f_21 = _mm256_sub_ps(v256f_20, v256f_20);
    __m512d v512d_22 = _mm512_insertf64x2(p3, v128d_8, 3);
    __m128i v128i_23 = g_v128i[0];
    __m128i v128i_24 = _mm_sub_epi32(v128i_23, v128i_23);
    __m128i v128i_25 = _mm_add_epi64(v128i_24, v128i_24);
    __m256 v256f_26 = _mm256_unpacklo_ps(v256f_20, v256f_21);
    __m128h v128h_27 = _mm512_cvtpd_ph(p3);
    __m512 v512f_28 = _mm512_mask_add_ps(v512f_5, m16_9, v512f_5, v512f_3);
    __m512 v512f_29 = _mm512_getexp_ps(v512f_4);
    __m256d v256d_30 = _mm256_add_pd(p1, p1);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m256 v256f_31 = _mm256_xor_ps(v256f_21, v256f_21);
    __m256 v256f_32 = _mm256_max_ps(v256f_21, v256f_20);
    __m128 v128f_33 = _mm_and_ps(v128f_19, v128f_19);
    }
    g_v512d[3] = p3;
    g_v128i[10] = v128i_25;
    g_v128h[7] = v128h_27;
    return p1;
}

__mmask16 torture_273(__m256d p0, __m128d p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_or_si512(v512i_1, v512i_1);
    __m256i v256i_3 = g_v256i[9];
    __m256i v256i_4 = _mm256_sub_epi64(v256i_3, v256i_3);
    __m256i v256i_5 = _mm256_sub_epi16(v256i_3, v256i_3);
    __m512 v512f_6 = g_v512f[6];
    __m512i v512i_7 = _mm512_cvtps_epi32(v512f_6);
    __m512 v512f_8 = _mm512_fmsub_ps(v512f_6, v512f_6, v512f_6);
    __m512 v512f_9 = _mm512_fmsub_ps(v512f_8, v512f_8, v512f_6);
    __m512 v512f_10 = _mm512_scalef_ps(v512f_9, v512f_6);
    __m256i v256i_11 = _mm256_sllv_epi32(v256i_3, v256i_3);
    __m512 v512f_12 = _mm512_max_ps(v512f_10, v512f_10);
    __m512i v512i_13 = _mm512_xor_si512(v512i_2, v512i_7);
    int s32_14 = 7;
    if ((s32_14) & 1ULL) {
    __m512d v512d_15 = _mm512_loadu_pd(dbuf);
    __m256 v256f_16 = _mm512_cvtpd_ps(v512d_15);
    __mmask16 m16_17 = (__mmask16)(g_m16[7] ^ 41);
    unsigned u32_18 = _cvtmask16_u32(m16_17);
    }
    __m128i v128i_19 = g_v128i[11];
    __m128i v128i_20 = _mm_unpacklo_epi32(v128i_19, v128i_19);
    __m512 v512f_21 = _mm512_getexp_ps(v512f_12);
    __m256d v256d_22 = _mm256_unpackhi_pd(p0, p0);
    __m512i v512i_23 = _mm512_xor_si512(v512i_1, v512i_13);
    __m512i v512i_24 = _mm512_add_epi32(v512i_2, v512i_1);
    __mmask64 m64_25 = _mm512_cmp_epi8_mask(v512i_13, v512i_13, 1);
    __m256h v256h_26 = _mm512_cvtxps_ph(v512f_21);
    __m512d v512d_27 = _mm512_loadu_pd(dbuf);
    __m512d v512d_28 = _mm512_min_pd(v512d_27, v512d_27);
    g_v256d[2] = v256d_22;
    __mmask16 m16_29 = (__mmask16)(g_m16[5] ^ 216);
    return m16_29;
}

__m256i torture_274(__m256h p0, __m128i p1) {
    __m256d v256d_1 = g_v256d[10];
    __m256d v256d_2 = _mm256_add_pd(v256d_1, v256d_1);
    __m256i v256i_3 = g_v256i[11];
    __m256i v256i_4 = _mm256_and_si256(v256i_3, v256i_3);
    __m512h v512h_5 = g_v512h[3];
    __m512i v512i_6 = _mm512_cvtph_epi16(v512h_5);
    __m256d v256d_7 = _mm256_mul_pd(v256d_1, v256d_2);
    __m128 v128f_8 = _mm_loadu_ps(fbuf);
    __m128 v128f_9 = _mm_unpacklo_ps(v128f_8, v128f_8);
    __m512 v512f_10 = _mm512_broadcast_f32x4(v128f_9);
    __mmask8 m8_11 = (__mmask8)(g_m8[1] ^ 60);
    __m512i v512i_12 = _mm512_broadcastmb_epi64(m8_11);
    __m512i v512i_13 = _mm512_set1_epi32(0x55);
    __m512i v512i_14 = _mm512_conflict_epi32(v512i_13);
    __m128i v128i_15 = _mm_or_si128(p1, p1);
    __m128i v128i_16 = _mm_or_si128(v128i_15, v128i_15);
    __m512i v512i_17 = _mm512_madd52lo_epu64(v512i_12, v512i_6, v512i_6);
    __m128 v128f_18 = _mm_sqrt_ps(v128f_8);
    __mmask64 m64_19 = (__mmask64)(g_m64[7] ^ 221);
    __mmask64 m64_20 = _kor_mask64(m64_19, m64_19);
    __m128d v128d_21 = g_v128d[8];
    __m128d v128d_22 = _mm_add_pd(v128d_21, v128d_21);
    __mmask16 m16_23 = (__mmask16)(g_m16[8] ^ 230);
    __mmask16 m16_24 = _kshiftli_mask16(m16_23, 2);
    __m512 v512f_25 = _mm512_fmadd_ps(v512f_10, v512f_10, v512f_10);
    if (((unsigned long long)m16_23) & 1ULL) {
    __m128 v128f_26 = _mm_max_ps(v128f_18, v128f_18);
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m256 v256f_28 = _mm256_max_ps(v256f_27, v256f_27);
    __m512i v512i_29 = _mm512_and_si512(v512i_12, v512i_6);
    __m512i v512i_30 = _mm512_sllv_epi32(v512i_6, v512i_29);
    }
    __m512d v512d_31 = _mm512_loadu_pd(dbuf);
    __m512d v512d_32 = _mm512_mask_blend_pd(m8_11, v512d_31, v512d_31);
    __m128 v128f_33 = _mm_sub_ps(v128f_18, v128f_18);
    __m256 v256f_34 = g_v256f[0];
    __m256 v256f_35 = _mm256_min_ps(v256f_34, v256f_34);
    __m512 v512f_36 = _mm512_fmadd_ps(v512f_25, v512f_10, v512f_25);
    __m512i v512i_37 = _mm512_permutex2var_epi32(v512i_14, v512i_13, v512i_17);
    __m512i v512i_38 = _mm512_sub_epi32(v512i_13, v512i_37);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_23) & 7); i_39++) {
    __m512i v512i_40 = _mm512_sllv_epi32(v512i_12, v512i_17);
    __m256 v256f_41 = _mm256_sqrt_ps(v256f_35);
    __m128 v128f_42 = _mm_fmadd_ps(v128f_18, v128f_33, v128f_33);
    __m512i v512i_43 = _mm512_unpacklo_epi32(v512i_6, v512i_6);
    }
    g_v512f[7] = v512f_10;
    g_v512d[4] = v512d_31;
    g_v512i[2] = v512i_12;
    g_v256f[11] = v256f_34;
    g_v256d[1] = v256d_7;
    g_v128f[2] = v128f_33;
    g_v128i[3] = p1;
    g_v256h[10] = p0;
    return v256i_3;
}

__m128i torture_275(__m512d p0, __m128 p1) {
    __m256i v256i_1 = _mm256_set1_epi32(0x55);
    __mmask16 m16_2 = (__mmask16)(g_m16[11] ^ 133);
    __mmask16 m16_3 = _kand_mask16(m16_2, m16_2);
    __m512h v512h_4 = g_v512h[10];
    __m512i v512i_5 = _mm512_cvtph_epi16(v512h_4);
    __m256d v256d_6 = g_v256d[7];
    __m256d v256d_7 = _mm256_max_pd(v256d_6, v256d_6);
    __m512d v512d_8 = _mm512_max_pd(p0, p0);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m512 v512f_10 = _mm512_add_ps(v512f_9, v512f_9);
    __m512h v512h_11 = _mm512_cvtepi16_ph(v512i_5);
    __m256i v256i_12 = _mm256_sub_epi16(v256i_1, v256i_1);
    __m128i v128i_13 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_14 = _mm512_broadcast_i32x4(v128i_13);
    __m256i v256i_15 = _mm256_sub_epi32(v256i_1, v256i_1);
    __m128i v128i_16 = _mm_set1_epi32(0x55);
    __m512d v512d_17 = _mm512_fmadd_pd(v512d_8, p0, v512d_8);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m512i v512i_18 = _mm512_shuffle_epi8(v512i_14, v512i_5);
    __m512i v512i_19 = _mm512_conflict_epi32(v512i_18);
    __m512i v512i_20 = _mm512_sllv_epi32(v512i_19, v512i_14);
    }
    __m512 v512f_21 = _mm512_xor_ps(v512f_10, v512f_10);
    __mmask8 m8_22 = (__mmask8)(g_m8[8] ^ 69);
    __m512i v512i_23 = _mm512_broadcastmb_epi64(m8_22);
    __m256d v256d_24 = _mm256_sqrt_pd(v256d_7);
    __m128 v128f_25 = _mm_fmadd_ps(p1, p1, p1);
    __m256d v256d_26 = _mm256_unpackhi_pd(v256d_7, v256d_7);
    __m256i v256i_27 = _mm256_sub_epi16(v256i_15, v256i_15);
    __m256i v256i_28 = _mm512_cvtepi64_epi32(v512i_5);
    __m256i v256i_29 = _mm256_add_epi8(v256i_12, v256i_15);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_3) & 7); i_30++) {
    __m256 v256f_31 = _mm256_loadu_ps(fbuf);
    __m256 v256f_32 = _mm256_sub_ps(v256f_31, v256f_31);
    __m256h v256h_33 = _mm512_cvtxps_ph(v512f_9);
    __m512i v512i_34 = _mm512_sub_epi16(v512i_14, v512i_5);
    __m512d v512d_35 = _mm512_div_pd(v512d_17, v512d_17);
    }
    g_v512d[8] = p0;
    g_v256i[11] = v256i_29;
    g_v128f[5] = p1;
    g_m8[7] = m8_22;
    g_m16[11] = m16_2;
    return v128i_16;
}

__mmask64 torture_276(__m128i p0, __m128h p1, __m512d p2, __m512d p3) {
    __m128d v128d_1 = g_v128d[10];
    __m128d v128d_2 = _mm_fmadd_pd(v128d_1, v128d_1, v128d_1);
    __m512 v512f_3 = g_v512f[2];
    __mmask16 m16_4 = _mm512_cmp_ps_mask(v512f_3, v512f_3, _CMP_LT_OQ);
    __m512i v512i_5 = _mm512_broadcast_i32x4(p0);
    __m512i v512i_6 = _mm512_lzcnt_epi32(v512i_5);
    __m512i v512i_7 = _mm512_set1_epi32(0x55);
    __m128i v128i_8 = _mm_sub_epi32(p0, p0);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_sub_epi64(v256i_9, v256i_9);
    __m512d v512d_11 = _mm512_cvtph_pd(p1);
    __m512i v512i_12 = _mm512_sllv_epi32(v512i_5, v512i_5);
    __m512h v512h_13 = g_v512h[6];
    __m512h v512h_14 = _mm512_sqrt_ph(v512h_13);
    __m128d v128d_15 = _mm_sqrt_pd(v128d_1);
    __mmask64 m64_16 = _mm512_movepi8_mask(v512i_7);
    __m256 v256f_17 = _mm256_loadu_ps(fbuf);
    __m256 v256f_18 = _mm256_fmadd_ps(v256f_17, v256f_17, v256f_17);
    __m256d v256d_19 = g_v256d[2];
    __m256d v256d_20 = _mm256_mul_pd(v256d_19, v256d_19);
    __m512d v512d_21 = _mm512_div_pd(p2, v512d_11);
    __m512i v512i_22 = _mm512_mask_blend_epi32(m16_4, v512i_7, v512i_7);
    __mmask32 m32_23 = _mm512_kunpackw(m16_4, m16_4);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_4) & 7); i_24++) {
    __m512i v512i_25 = _mm512_dpwssd_epi32(v512i_22, v512i_5, v512i_22);
    __mmask16 m16_26 = _mm512_fpclass_ps_mask(v512f_3, 0x3);
    __m512d v512d_27 = _mm512_sqrt_pd(v512d_11);
    __m128h v128h_28 = _mm512_cvtpd_ph(p3);
    __m128 v128f_29 = g_v128f[6];
    __m128 v128f_30 = _mm_div_ps(v128f_29, v128f_29);
    }
    __m256 v256f_31 = _mm256_add_ps(v256f_17, v256f_18);
    __m128i v128i_32 = _mm_set1_epi32(0x55);
    __m512 v512f_33 = _mm512_fmsub_ps(v512f_3, v512f_3, v512f_3);
    __m128d v128d_34 = _mm_sqrt_pd(v128d_15);
    __mmask16 m16_35 = _kand_mask16(m16_4, m16_4);
    g_v512f[11] = v512f_3;
    g_v512d[9] = v512d_11;
    g_v512i[9] = v512i_5;
    g_v256f[11] = v256f_17;
    g_v256i[11] = v256i_10;
    g_v128h[2] = p1;
    return m64_16;
}

__m512i torture_277(__m512h p0, __m512i p1, __m512 p2, __m128d p3) {
    __m512h v512h_1 = _mm512_cvtepi16_ph(p1);
    __m512i v512i_2 = _mm512_xor_si512(p1, p1);
    __m128 v128f_3 = g_v128f[6];
    __m128 v128f_4 = _mm_div_ps(v128f_3, v128f_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[9] ^ 227);
    __mmask32 m32_6 = _mm512_kunpackw(m16_5, m16_5);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_add_ps(v256f_7, v256f_7);
    __m256i v256i_9 = _mm256_set1_epi32(0x55);
    __m256d v256d_10 = g_v256d[8];
    __m256d v256d_11 = _mm256_sqrt_pd(v256d_10);
    __m512 v512f_12 = _mm512_min_ps(p2, p2);
    __m256d v256d_13 = _mm256_set1_pd(2.0);
    __m256i v256i_14 = _mm256_sub_epi16(v256i_9, v256i_9);
    __mmask32 m32_15 = _mm512_kunpackw(m16_5, m16_5);
    __m256h v256h_16 = _mm512_cvtxps_ph(v512f_12);
    __mmask16 m16_17 = _kshiftli_mask16(m16_5, 2);
    __m256 v256f_18 = _mm256_min_ps(v256f_7, v256f_7);
    for (int i_19 = 0; i_19 < (int)(((unsigned long long)m16_17) & 7); i_19++) {
    __m512i v512i_20 = _mm512_popcnt_epi64(p1);
    __m256i v256i_21 = _mm256_unpacklo_epi32(v256i_9, v256i_9);
    }
    __m128 v128f_22 = _mm_mul_ps(v128f_4, v128f_3);
    __m512 v512f_23 = _mm512_fmadd_ps(v512f_12, p2, p2);
    __m512 v512f_24 = _mm512_div_ps(v512f_12, v512f_12);
    __m128d v128d_25 = _mm_add_pd(p3, p3);
    __m128 v128f_26 = _mm_mul_ps(v128f_3, v128f_4);
    __m512i v512i_27 = _mm512_mask_blend_epi32(m16_17, p1, v512i_2);
    __m256d v256d_28 = _mm256_add_pd(v256d_10, v256d_11);
    __m256i v256i_29 = _mm256_add_epi32(v256i_14, v256i_14);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_17) & 7); i_30++) {
    __m256i v256i_31 = _mm256_sub_epi8(v256i_29, v256i_14);
    __m512i v512i_32 = _mm512_abs_epi32(v512i_2);
    __m128i v128i_33 = _mm512_cvtepi32_epi8(v512i_27);
    __m128d v128d_34 = _mm_fmadd_pd(p3, v128d_25, v128d_25);
    }
    g_v256f[1] = v256f_18;
    g_v256d[4] = v256d_11;
    g_v256i[9] = v256i_29;
    return v512i_2;
}

__m512 torture_278(__m128d p0, __m128i p1, __m512 p2, __m128d p3, __m128h p4) {
    __m512i v512i_1 = g_v512i[11];
    __m512i v512i_2 = _mm512_unpacklo_epi32(v512i_1, v512i_1);
    __m256d v256d_3 = g_v256d[4];
    __m256d v256d_4 = _mm256_mul_pd(v256d_3, v256d_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[7] ^ 98);
    __m512 v512f_6 = _mm512_mask_add_ps(p2, m16_5, p2, p2);
    __m128i v128i_7 = _mm_and_si128(p1, p1);
    __m512i v512i_8 = _mm512_permutexvar_epi32(v512i_1, v512i_2);
    __m512 v512f_9 = _mm512_sqrt_ps(v512f_6);
    __m128i v128i_10 = _mm_shuffle_epi8(p1, p1);
    __m512i v512i_11 = _mm512_cvtps_epi32(p2);
    __m512i v512i_12 = _mm512_sllv_epi32(v512i_2, v512i_1);
    __m512d v512d_13 = g_v512d[2];
    __m128h v128h_14 = _mm512_cvtpd_ph(v512d_13);
    __m512i v512i_15 = _mm512_and_si512(v512i_12, v512i_12);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_div_ps(v128f_16, v128f_16);
    __m512d v512d_18 = _mm512_cvtph_pd(v128h_14);
    __m128d v128d_19 = _mm_sqrt_pd(p3);
    __m128i v128i_20 = _mm_or_si128(v128i_7, v128i_10);
    __mmask16 m16_21 = _mm512_fpclass_ps_mask(v512f_9, 0x3);
    __m128 v128f_22 = _mm_mul_ps(v128f_16, v128f_16);
    __m512i v512i_23 = _mm512_sllv_epi32(v512i_12, v512i_1);
    __mmask32 m32_24 = (__mmask32)(g_m32[8] ^ 199);
    __mmask32 m32_25 = _kxor_mask32(m32_24, m32_24);
    unsigned u32_26 = _cvtmask16_u32(m16_5);
    __m128d v128d_27 = _mm_unpackhi_pd(p3, v128d_19);
    __m256 v256f_28 = _mm512_cvtpd_ps(v512d_18);
    if (((unsigned long long)m16_21) & 1ULL) {
    __m256 v256f_29 = _mm256_shuffle_ps(v256f_28, v256f_28, 0x1B);
    __m256 v256f_30 = _mm256_fmadd_ps(v256f_28, v256f_29, v256f_29);
    __m128i v128i_31 = _mm_xor_si128(v128i_10, v128i_7);
    __m128 v128f_32 = _mm_fnmadd_ps(v128f_17, v128f_17, v128f_16);
    }
    g_v512d[8] = v512d_18;
    g_v256d[8] = v256d_3;
    g_v128d[9] = v128d_27;
    g_v128i[0] = v128i_20;
    g_v128h[0] = v128h_14;
    g_m16[1] = m16_5;
    g_m32[0] = m32_24;
    return v512f_9;
}

__mmask32 torture_279(__m128 p0, __m512 p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_dpbusd_epi32(v512i_1, v512i_1, v512i_1);
    __m512h v512h_3 = g_v512h[9];
    __m512h v512h_4 = _mm512_mul_ph(v512h_3, v512h_3);
    __m128i v128i_5 = _mm512_cvtepi32_epi8(v512i_2);
    __m256 v256f_6 = g_v256f[10];
    __m256 v256f_7 = _mm256_mul_ps(v256f_6, v256f_6);
    __mmask8 m8_8 = (__mmask8)(g_m8[4] ^ 159);
    __mmask8 m8_9 = _kand_mask8(m8_8, m8_8);
    __mmask16 m16_10 = (__mmask16)(g_m16[7] ^ 216);
    __mmask16 m16_11 = _kshiftli_mask16(m16_10, 2);
    __m128 v128f_12 = _mm_shuffle_ps(p0, p0, 0x1B);
    __m128i v128i_13 = _mm_unpacklo_epi32(v128i_5, v128i_5);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_11) & 7); i_14++) {
    __m512h v512h_15 = _mm512_getexp_ph(v512h_4);
    __m512i v512i_16 = _mm512_cvtph_epi16(v512h_4);
    __m512i v512i_17 = _mm512_mask_add_epi32(v512i_16, m16_10, v512i_2, v512i_1);
    __m512 v512f_18 = _mm512_xor_ps(p1, p1);
    }
    __m256h v256h_19 = _mm512_cvtxps_ph(p1);
    __m128d v128d_20 = g_v128d[3];
    __m128d v128d_21 = _mm_sub_pd(v128d_20, v128d_20);
    __m512d v512d_22 = _mm512_loadu_pd(dbuf);
    __m512d v512d_23 = _mm512_unpackhi_pd(v512d_22, v512d_22);
    __m128 v128f_24 = _mm_xor_ps(v128f_12, p0);
    __m512i v512i_25 = _mm512_unpacklo_epi32(v512i_1, v512i_2);
    __m512i v512i_26 = _mm512_slli_epi32(v512i_1, 3);
    __m512i v512i_27 = _mm512_cvtph_epi16(v512h_4);
    __m512i v512i_28 = _mm512_sllv_epi32(v512i_25, v512i_26);
    __m256 v256f_29 = _mm256_div_ps(v256f_6, v256f_7);
    __m256d v256d_30 = g_v256d[10];
    __m256d v256d_31 = _mm256_sub_pd(v256d_30, v256d_30);
    g_v128f[6] = v128f_12;
    g_v128d[3] = v128d_21;
    g_v512h[9] = v512h_4;
    g_v256h[3] = v256h_19;
    __mmask32 m32_32 = (__mmask32)(g_m32[1] ^ 21);
    return m32_32;
}

__m512i torture_280(__m256h p0, __m512 p1) {
    __m256 v256f_1 = _mm256_set1_ps(1.0f);
    __m512h v512h_2 = g_v512h[11];
    __m512h v512h_3 = _mm512_getexp_ph(v512h_2);
    __m128i v128i_4 = g_v128i[3];
    __m128i v128i_5 = _mm_add_epi32(v128i_4, v128i_4);
    __m512d v512d_6 = g_v512d[9];
    __m256d v256d_7 = _mm512_extractf64x4_pd(v512d_6, 1);
    __m128 v128f_8 = _mm_loadu_ps(fbuf);
    __m128 v128f_9 = _mm_sqrt_ps(v128f_8);
    __m512i v512i_10 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_11 = _mm512_cmp_epu32_mask(v512i_10, v512i_10, 4);
    __m512 v512f_12 = _mm512_roundscale_ps(p1, 0x10);
    __m256i v256i_13 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_14 = _mm256_xor_si256(v256i_13, v256i_13);
    if (((unsigned long long)m16_11) & 1ULL) {
    __m512 v512f_15 = _mm512_max_ps(v512f_12, p1);
    __m512i v512i_16 = _mm512_slli_epi32(v512i_10, 3);
    __m128 v128f_17 = _mm_set1_ps(1.0f);
    __mmask32 m32_18 = (__mmask32)(g_m32[9] ^ 139);
    __mmask32 m32_19 = _kxor_mask32(m32_18, m32_18);
    __m512d v512d_20 = _mm512_shuffle_f64x2(v512d_6, v512d_6, 0xEE);
    }
    __m256i v256i_21 = _mm256_xor_si256(v256i_13, v256i_14);
    __m128 v128f_22 = _mm_fnmadd_ps(v128f_9, v128f_9, v128f_8);
    __m512i v512i_23 = _mm512_sub_epi8(v512i_10, v512i_10);
    __m256 v256f_24 = _mm256_shuffle_ps(v256f_1, v256f_1, 0x1B);
    __m256d v256d_25 = _mm256_set1_pd(2.0);
    __m512 v512f_26 = _mm512_rsqrt14_ps(v512f_12);
    __m512i v512i_27 = _mm512_shuffle_epi8(v512i_10, v512i_10);
    __m512 v512f_28 = _mm512_unpacklo_ps(p1, p1);
    __m512 v512f_29 = _mm512_set1_ps(1.0f);
    g_v512d[5] = v512d_6;
    g_v256f[4] = v256f_24;
    g_v256d[8] = v256d_25;
    return v512i_10;
}

__m128h torture_281(__m512i p0, __m128i p1, __m128 p2) {
    __mmask8 m8_1 = (__mmask8)(g_m8[11] ^ 131);
    __m512d v512d_2 = g_v512d[11];
    __m512d v512d_3 = _mm512_mask_blend_pd(m8_1, v512d_2, v512d_2);
    __m128i v128i_4 = _mm_slli_epi32(p1, 3);
    __m128i v128i_5 = _mm_abs_epi32(v128i_4);
    __m256d v256d_6 = _mm256_set1_pd(2.0);
    __mmask16 m16_7 = _mm512_movepi32_mask(p0);
    __m256i v256i_8 = g_v256i[5];
    __m256i v256i_9 = _mm256_add_epi16(v256i_8, v256i_8);
    __m512i v512i_10 = _mm512_mask_add_epi32(p0, m16_7, p0, p0);
    __m128i v128i_11 = _mm_or_si128(v128i_5, v128i_5);
    __mmask16 m16_12 = _kand_mask16(m16_7, m16_7);
    __m512d v512d_13 = _mm512_sub_pd(v512d_3, v512d_2);
    __m512d v512d_14 = _mm512_unpackhi_pd(v512d_13, v512d_2);
    __m256 v256f_15 = _mm256_loadu_ps(fbuf);
    __m256 v256f_16 = _mm256_fmadd_ps(v256f_15, v256f_15, v256f_15);
    __m256 v256f_17 = _mm256_add_ps(v256f_16, v256f_16);
    __m512i v512i_18 = _mm512_popcnt_epi64(v512i_10);
    __m256 v256f_19 = _mm256_add_ps(v256f_16, v256f_17);
    __m256 v256f_20 = _mm256_xor_ps(v256f_16, v256f_15);
    __m128 v128f_21 = _mm_unpacklo_ps(p2, p2);
    __mmask16 m16_22 = _kshiftli_mask16(m16_12, 2);
    __m128d v128d_23 = g_v128d[6];
    __m128d v128d_24 = _mm_add_pd(v128d_23, v128d_23);
    __m512d v512d_25 = _mm512_insertf64x2(v512d_3, v128d_24, 3);
    __m512 v512f_26 = _mm512_loadu_ps(fbuf);
    __m256h v256h_27 = _mm512_cvtxps_ph(v512f_26);
    g_v256f[9] = v256f_20;
    g_v128f[2] = p2;
    g_v128i[7] = v128i_5;
    __m128h v128h_28 = g_v128h[7];
    return v128h_28;
}

__m256i torture_282(__m256i p0, __m256d p1, __m512 p2) {
    __m512i v512i_1 = g_v512i[6];
    __m512i v512i_2 = _mm512_dpwssd_epi32(v512i_1, v512i_1, v512i_1);
    __m256i v256i_3 = _mm256_sub_epi64(p0, p0);
    __m512d v512d_4 = _mm512_loadu_pd(dbuf);
    __m512d v512d_5 = _mm512_add_pd(v512d_4, v512d_4);
    __m256i v256i_6 = _mm256_sub_epi32(v256i_3, p0);
    __m256i v256i_7 = _mm256_abs_epi32(v256i_3);
    __m512i v512i_8 = _mm512_xor_si512(v512i_1, v512i_1);
    __m512i v512i_9 = _mm512_sub_epi8(v512i_2, v512i_1);
    __m512i v512i_10 = _mm512_rolv_epi32(v512i_9, v512i_1);
    __m512 v512f_11 = _mm512_rcp14_ps(p2);
    int s32_12 = 39;
    for (int i_13 = 0; i_13 < (int)((s32_12) & 7); i_13++) {
    __m512i v512i_14 = _mm512_lzcnt_epi32(v512i_2);
    __m128i v128i_15 = g_v128i[5];
    __m128i v128i_16 = _mm_sub_epi32(v128i_15, v128i_15);
    __m256i v256i_17 = _mm256_set1_epi32(0x55);
    __m128d v128d_18 = _mm_set1_pd(2.0);
    __m512 v512f_19 = _mm512_div_ps(v512f_11, v512f_11);
    }
    __m512i v512i_20 = _mm512_unpacklo_epi32(v512i_9, v512i_2);
    __m128 v128f_21 = g_v128f[4];
    __m128 v128f_22 = _mm_shuffle_ps(v128f_21, v128f_21, 0x1B);
    __m256i v256i_23 = _mm256_add_epi16(v256i_7, v256i_7);
    __m512d v512d_24 = _mm512_sub_pd(v512d_5, v512d_4);
    __mmask32 m32_25 = (__mmask32)(g_m32[5] ^ 196);
    __mmask32 m32_26 = _kxor_mask32(m32_25, m32_25);
    __m128 v128f_27 = _mm_xor_ps(v128f_22, v128f_21);
    __mmask8 m8_28 = (__mmask8)(g_m8[8] ^ 197);
    __mmask8 m8_29 = _kand_mask8(m8_28, m8_28);
    if (((unsigned long long)m8_29) & 1ULL) {
    __m128i v128i_30 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_31 = _mm_slli_epi32(v128i_30, 3);
    __m256d v256d_32 = _mm256_fmadd_pd(p1, p1, p1);
    __m128 v128f_33 = _mm512_extractf32x4_ps(p2, 2);
    __m256 v256f_34 = _mm256_loadu_ps(fbuf);
    __m256 v256f_35 = _mm256_mul_ps(v256f_34, v256f_34);
    __m512 v512f_36 = _mm512_insertf32x8(v512f_11, v256f_35, 1);
    }
    g_v512f[10] = p2;
    g_v512d[0] = v512d_24;
    g_v128f[0] = v128f_21;
    g_m32[6] = m32_26;
    return v256i_23;
}

__mmask8 torture_283(__m128i p0, __m128h p1, __m128i p2, __m512 p3, __m256d p4) {
    __m512i v512i_1 = g_v512i[4];
    __m512i v512i_2 = _mm512_rolv_epi32(v512i_1, v512i_1);
    __m512 v512f_3 = _mm512_xor_ps(p3, p3);
    __m256 v256f_4 = g_v256f[8];
    __m256 v256f_5 = _mm256_min_ps(v256f_4, v256f_4);
    __m256i v256i_6 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_7 = _mm256_abs_epi32(v256i_6);
    __mmask16 m16_8 = (__mmask16)(g_m16[4] ^ 203);
    __m512i v512i_9 = _mm512_broadcastmw_epi32(m16_8);
    __m256d v256d_10 = _mm256_div_pd(p4, p4);
    __m128d v128d_11 = g_v128d[4];
    __m128d v128d_12 = _mm_sub_pd(v128d_11, v128d_11);
    __m128d v128d_13 = _mm_set1_pd(2.0);
    __mmask16 m16_14 = _kshiftri_mask16(m16_8, 1);
    __m256i v256i_15 = _mm256_xor_si256(v256i_6, v256i_6);
    __m256i v256i_16 = _mm256_add_epi8(v256i_15, v256i_15);
    __m256i v256i_17 = _mm256_add_epi8(v256i_15, v256i_7);
    __m128d v128d_18 = _mm_unpackhi_pd(v128d_12, v128d_13);
    __m512 v512f_19 = _mm512_shuffle_ps(v512f_3, p3, 0x1B);
    __mmask8 m8_20 = (__mmask8)(g_m8[0] ^ 115);
    __mmask8 m8_21 = _kand_mask8(m8_20, m8_20);
    __m256d v256d_22 = _mm256_fmadd_pd(v256d_10, p4, p4);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512 v512f_23 = _mm512_mask_add_ps(p3, m16_14, p3, v512f_19);
    unsigned u32_24 = 46;
    __mmask16 m16_25 = _cvtu32_mask16(u32_24);
    __m256i v256i_26 = _mm256_set1_epi32(0x55);
    }
    __m512i v512i_27 = _mm512_and_si512(v512i_1, v512i_2);
    unsigned u32_28 = 61;
    __mmask16 m16_29 = _cvtu32_mask16(u32_28);
    __m128 v128f_30 = g_v128f[6];
    __m512 v512f_31 = _mm512_broadcast_f32x4(v128f_30);
    __mmask16 m16_32 = _kor_mask16(m16_8, m16_29);
    __m512i v512i_33 = _mm512_dpbusd_epi32(v512i_1, v512i_27, v512i_27);
    __mmask16 m16_34 = _kshiftli_mask16(m16_14, 2);
    __m512 v512f_35 = _mm512_add_ps(v512f_31, v512f_19);
    __m512i v512i_36 = _mm512_unpacklo_epi32(v512i_2, v512i_1);
    __m256i v256i_37 = _mm256_add_epi8(v256i_7, v256i_6);
    g_v512f[6] = v512f_35;
    g_v512i[4] = v512i_1;
    g_v256d[5] = p4;
    g_v128d[6] = v128d_12;
    g_v128i[6] = p0;
    g_m8[8] = m8_20;
    return m8_21;
}

__m256h torture_284(__m256 p0, __m256 p1, __m256 p2) {
    __m512d v512d_1 = g_v512d[2];
    __m512i v512i_2 = _mm512_cvtpd_epi64(v512d_1);
    __m512 v512f_3 = g_v512f[3];
    __m512 v512f_4 = _mm512_insertf32x8(v512f_3, p2, 1);
    __m256 v256f_5 = _mm256_xor_ps(p2, p2);
    __m512 v512f_6 = _mm512_and_ps(v512f_4, v512f_3);
    __m512 v512f_7 = _mm512_set1_ps(1.0f);
    __m512i v512i_8 = _mm512_dpwssd_epi32(v512i_2, v512i_2, v512i_2);
    __m512 v512f_9 = _mm512_and_ps(v512f_4, v512f_7);
    __m512i v512i_10 = _mm512_madd52lo_epu64(v512i_2, v512i_2, v512i_8);
    __m256 v256f_11 = _mm512_cvtpd_ps(v512d_1);
    __mmask16 m16_12 = _mm512_movepi32_mask(v512i_2);
    __m256i v256i_13 = _mm256_set1_epi32(0x55);
    __m256i v256i_14 = _mm256_add_epi8(v256i_13, v256i_13);
    __m128 v128f_15 = _mm_loadu_ps(fbuf);
    __m512 v512f_16 = _mm512_broadcast_f32x4(v128f_15);
    __m512i v512i_17 = _mm512_mask_blend_epi32(m16_12, v512i_8, v512i_2);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m128i v128i_18 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_19 = _mm512_broadcast_i32x4(v128i_18);
    __mmask8 m8_20 = (__mmask8)(g_m8[5] ^ 76);
    __m512d v512d_21 = _mm512_mask_blend_pd(m8_20, v512d_1, v512d_1);
    __m256 v256f_22 = _mm256_and_ps(v256f_5, p1);
    __m512 v512f_23 = _mm512_fnmadd_ps(v512f_7, v512f_6, v512f_3);
    __m512h v512h_24 = g_v512h[2];
    __m512h v512h_25 = _mm512_fmadd_ph(v512h_24, v512h_24, v512h_24);
    }
    __m512 v512f_26 = _mm512_set1_ps(1.0f);
    __m512d v512d_27 = _mm512_set1_pd(2.0);
    __m512h v512h_28 = g_v512h[9];
    __m512h v512h_29 = _mm512_rcp_ph(v512h_28);
    __m256 v256f_30 = _mm256_fmsub_ps(v256f_5, v256f_5, v256f_5);
    __m512i v512i_31 = _mm512_shuffle_i32x4(v512i_17, v512i_10, 0x1B);
    __m512i v512i_32 = _mm512_add_epi32(v512i_10, v512i_17);
    g_v256f[7] = v256f_5;
    g_v128f[4] = v128f_15;
    g_v512h[4] = v512h_28;
    g_m16[5] = m16_12;
    __m256h v256h_33 = g_v256h[5];
    return v256h_33;
}

__mmask16 torture_285(__m256h p0, __m512i p1, __m128 p2, __m512i p3, __m128 p4) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_fmadd_ps(v256f_1, v256f_1, v256f_1);
    __m256 v256f_3 = _mm256_mul_ps(v256f_2, v256f_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[6] ^ 76);
    __mmask16 m16_5 = _kor_mask16(m16_4, m16_4);
    __m512i v512i_6 = _mm512_mask_add_epi32(p3, m16_4, p3, p3);
    __m512d v512d_7 = g_v512d[11];
    __m512d v512d_8 = _mm512_fmadd_pd(v512d_7, v512d_7, v512d_7);
    __m512d v512d_9 = _mm512_set1_pd(2.0);
    __m256 v256f_10 = _mm512_cvtpd_ps(v512d_9);
    __m512 v512f_11 = g_v512f[11];
    __m512 v512f_12 = _mm512_unpacklo_ps(v512f_11, v512f_11);
    __m128 v128f_13 = _mm_and_ps(p2, p2);
    __m512i v512i_14 = _mm512_sub_epi8(p1, v512i_6);
    __m256d v256d_15 = _mm512_extractf64x4_pd(v512d_9, 1);
    __m256i v256i_16 = g_v256i[6];
    __m256i v256i_17 = _mm256_add_epi32(v256i_16, v256i_16);
    __mmask64 m64_18 = _mm512_cmp_epi8_mask(v512i_14, v512i_6, 1);
    __m128i v128i_19 = g_v128i[2];
    __m128i v128i_20 = _mm_mullo_epi32(v128i_19, v128i_19);
    __m256i v256i_21 = _mm256_add_epi64(v256i_17, v256i_17);
    __m256 v256f_22 = _mm256_max_ps(v256f_3, v256f_1);
    __m128 v128f_23 = _mm_fmsub_ps(p4, p4, p4);
    __mmask16 m16_24 = _mm512_movepi32_mask(v512i_14);
    if (((unsigned long long)m16_4) & 1ULL) {
    unsigned u32_25 = 97;
    __mmask16 m16_26 = _cvtu32_mask16(u32_25);
    __mmask32 m32_27 = _mm512_kunpackw(m16_24, m16_4);
    }
    __m512i v512i_28 = _mm512_and_si512(v512i_14, v512i_6);
    __m512d v512d_29 = _mm512_max_pd(v512d_9, v512d_9);
    __mmask64 m64_30 = _mm512_movepi8_mask(p3);
    __m512i v512i_31 = _mm512_unpacklo_epi32(p1, p3);
    __mmask16 m16_32 = _mm512_cmp_ps_mask(v512f_12, v512f_12, _CMP_LT_OQ);
    __m512 v512f_33 = _mm512_maskz_mul_ps(m16_32, v512f_11, v512f_11);
    __m512 v512f_34 = _mm512_getexp_ps(v512f_12);
    __m512i v512i_35 = _mm512_mask_blend_epi32(m16_32, v512i_14, v512i_14);
    __mmask16 m16_36 = _mm512_cmp_epi32_mask(v512i_6, v512i_14, 2);
    __m512i v512i_37 = _mm512_mask_blend_epi32(m16_32, p3, v512i_6);
    for (int i_38 = 0; i_38 < (int)(((unsigned long long)m16_32) & 7); i_38++) {
    __m512 v512f_39 = _mm512_unpacklo_ps(v512f_12, v512f_12);
    __m512i v512i_40 = _mm512_permutexvar_epi32(v512i_35, v512i_14);
    }
    g_v512i[6] = p3;
    g_v256f[11] = v256f_3;
    g_v128f[9] = p2;
    g_v128i[1] = v128i_20;
    g_v256h[9] = p0;
    g_m16[2] = m16_4;
    return m16_36;
}

__m512i torture_286(__m256 p0, __m512d p1, __m128h p2, __m256i p3, __m128 p4) {
    __m256 v256f_1 = _mm256_set1_ps(1.0f);
    __m512i v512i_2 = g_v512i[9];
    __m512i v512i_3 = _mm512_sllv_epi32(v512i_2, v512i_2);
    __m512 v512f_4 = g_v512f[2];
    __m512 v512f_5 = _mm512_min_ps(v512f_4, v512f_4);
    __m256h v256h_6 = _mm512_cvtxps_ph(v512f_5);
    __m512i v512i_7 = _mm512_slli_epi32(v512i_3, 3);
    __m512i v512i_8 = _mm512_rol_epi32(v512i_3, 5);
    __mmask8 m8_9 = (__mmask8)(g_m8[7] ^ 231);
    __mmask16 m16_10 = _mm512_kunpackb(m8_9, m8_9);
    __m128d v128d_11 = g_v128d[10];
    __m128d v128d_12 = _mm_add_pd(v128d_11, v128d_11);
    __mmask16 m16_13 = _mm512_cmp_epu32_mask(v512i_7, v512i_2, 4);
    __m512d v512d_14 = _mm512_mul_pd(p1, p1);
    __m256 v256f_15 = _mm256_add_ps(p0, v256f_1);
    __m256i v256i_16 = _mm256_sub_epi64(p3, p3);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_13) & 7); i_17++) {
    __m512h v512h_18 = g_v512h[9];
    __m512h v512h_19 = _mm512_rcp_ph(v512h_18);
    __m128 v128f_20 = _mm_min_ps(p4, p4);
    }
    __m512 v512f_21 = _mm512_div_ps(v512f_5, v512f_4);
    __mmask16 m16_22 = _mm512_cmp_epu32_mask(v512i_8, v512i_7, 4);
    __m256d v256d_23 = g_v256d[3];
    __m256d v256d_24 = _mm256_div_pd(v256d_23, v256d_23);
    __mmask8 m8_25 = _kand_mask8(m8_9, m8_9);
    __m128d v128d_26 = _mm_add_pd(v128d_11, v128d_12);
    __m128i v128i_27 = _mm_set1_epi32(0x55);
    __m256i v256i_28 = _mm256_abs_epi32(p3);
    __m128i v128i_29 = _mm_sub_epi16(v128i_27, v128i_27);
    __m128 v128f_30 = _mm_and_ps(p4, p4);
    __m128i v128i_31 = _mm_sub_epi64(v128i_29, v128i_27);
    __m512 v512f_32 = _mm512_insertf32x8(v512f_5, v256f_15, 1);
    if (((unsigned long long)m16_10) & 1ULL) {
    __mmask64 m64_33 = _mm512_movepi8_mask(v512i_8);
    __m512i v512i_34 = _mm512_lzcnt_epi32(v512i_3);
    __m512 v512f_35 = _mm512_sub_ps(v512f_5, v512f_4);
    __m128i v128i_36 = _mm_add_epi64(v128i_29, v128i_27);
    __mmask16 m16_37 = _mm512_cmp_ps_mask(v512f_32, v512f_32, _CMP_LT_OQ);
    __m256i v256i_38 = _mm256_set1_epi32(0x55);
    }
    g_v512f[6] = v512f_21;
    g_v256d[1] = v256d_24;
    g_v256i[5] = p3;
    g_v128f[11] = p4;
    g_v128i[7] = v128i_29;
    g_v256h[1] = v256h_6;
    g_v128h[4] = p2;
    g_m8[0] = m8_25;
    return v512i_8;
}

__mmask16 torture_287(__m512h p0, __m128i p1) {
    __m256 v256f_1 = g_v256f[11];
    __m256 v256f_2 = _mm256_sub_ps(v256f_1, v256f_1);
    __m256i v256i_3 = g_v256i[0];
    __m256i v256i_4 = _mm256_add_epi16(v256i_3, v256i_3);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_6 = _mm512_abs_epi32(v512i_5);
    __m128 v128f_7 = _mm_loadu_ps(fbuf);
    __m512 v512f_8 = _mm512_broadcast_f32x4(v128f_7);
    __m512i v512i_9 = _mm512_unpacklo_epi32(v512i_6, v512i_6);
    __m128i v128i_10 = _mm_sub_epi64(p1, p1);
    __m512i v512i_11 = _mm512_rol_epi32(v512i_6, 5);
    __m512i v512i_12 = _mm512_popcnt_epi64(v512i_6);
    __m512i v512i_13 = _mm512_sub_epi8(v512i_6, v512i_11);
    __m512i v512i_14 = _mm512_popcnt_epi64(v512i_9);
    __m128i v128i_15 = _mm_or_si128(p1, v128i_10);
    __mmask16 m16_16 = _mm512_fpclass_ps_mask(v512f_8, 0x3);
    __m256i v256i_17 = _mm256_add_epi8(v256i_3, v256i_3);
    __m512i v512i_18 = _mm512_permutex2var_epi32(v512i_12, v512i_5, v512i_12);
    __m512d v512d_19 = _mm512_set1_pd(2.0);
    __m256i v256i_20 = _mm256_add_epi8(v256i_4, v256i_3);
    __m512h v512h_21 = _mm512_fmsub_ph(p0, p0, p0);
    __m512 v512f_22 = _mm512_shuffle_f32x4(v512f_8, v512f_8, 0x4E);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_16) & 7); i_23++) {
    __mmask16 m16_24 = _mm512_testn_epi32_mask(v512i_18, v512i_11);
    __m512d v512d_25 = _mm512_min_pd(v512d_19, v512d_19);
    }
    __m512i v512i_26 = _mm512_sub_epi8(v512i_5, v512i_11);
    __m512i v512i_27 = _mm512_rol_epi32(v512i_18, 5);
    __m512h v512h_28 = _mm512_cvtepi16_ph(v512i_6);
    __m512i v512i_29 = _mm512_sub_epi16(v512i_11, v512i_18);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_16) & 7); i_30++) {
    __m128i v128i_31 = _mm_add_epi64(v128i_15, v128i_10);
    __m512i v512i_32 = _mm512_mask_add_epi32(v512i_11, m16_16, v512i_6, v512i_12);
    __m512d v512d_33 = _mm512_set1_pd(2.0);
    __m512 v512f_34 = _mm512_mask_add_ps(v512f_22, m16_16, v512f_22, v512f_8);
    }
    g_v128i[11] = v128i_10;
    return m16_16;
}

__m256i torture_288(__m256i p0, __m512d p1) {
    __m128h v128h_1 = _mm512_cvtpd_ph(p1);
    __mmask8 m8_2 = (__mmask8)(g_m8[9] ^ 102);
    __m512d v512d_3 = _mm512_mask_blend_pd(m8_2, p1, p1);
    __m512d v512d_4 = _mm512_add_pd(v512d_3, v512d_3);
    __m512 v512f_5 = g_v512f[5];
    __m512 v512f_6 = _mm512_add_ps(v512f_5, v512f_5);
    __m512i v512i_7 = g_v512i[3];
    __m512i v512i_8 = _mm512_sub_epi32(v512i_7, v512i_7);
    __m256d v256d_9 = _mm256_set1_pd(2.0);
    __m512d v512d_10 = _mm512_add_pd(v512d_3, p1);
    __m256i v256i_11 = _mm256_sllv_epi32(p0, p0);
    __m512d v512d_12 = _mm512_mask_blend_pd(m8_2, v512d_10, v512d_3);
    __m128i v128i_13 = g_v128i[10];
    __m128i v128i_14 = _mm_unpacklo_epi32(v128i_13, v128i_13);
    __m512d v512d_15 = _mm512_div_pd(p1, p1);
    __m512d v512d_16 = _mm512_add_pd(v512d_10, v512d_10);
    __m512d v512d_17 = _mm512_cvtph_pd(v128h_1);
    __mmask16 m16_18 = (__mmask16)(g_m16[0] ^ 207);
    __mmask32 m32_19 = _mm512_kunpackw(m16_18, m16_18);
    __m512d v512d_20 = _mm512_mul_pd(v512d_12, p1);
    __m512h v512h_21 = g_v512h[2];
    __m512h v512h_22 = _mm512_getexp_ph(v512h_21);
    __m512 v512f_23 = _mm512_xor_ps(v512f_6, v512f_5);
    __m512i v512i_24 = _mm512_and_si512(v512i_7, v512i_7);
    __m256 v256f_25 = _mm256_loadu_ps(fbuf);
    __m256 v256f_26 = _mm256_and_ps(v256f_25, v256f_25);
    __m512 v512f_27 = _mm512_unpacklo_ps(v512f_23, v512f_5);
    __m512h v512h_28 = _mm512_sqrt_ph(v512h_21);
    __m128 v128f_29 = _mm_loadu_ps(fbuf);
    __m128 v128f_30 = _mm_xor_ps(v128f_29, v128f_29);
    if (((unsigned long long)m16_18) & 1ULL) {
    __m512i v512i_31 = _mm512_mask_add_epi32(v512i_7, m16_18, v512i_7, v512i_24);
    __m512 v512f_32 = _mm512_getexp_ps(v512f_5);
    __m256 v256f_33 = _mm256_fmadd_ps(v256f_25, v256f_25, v256f_26);
    __m128 v128f_34 = _mm_div_ps(v128f_30, v128f_30);
    __m256 v256f_35 = _mm256_add_ps(v256f_33, v256f_26);
    }
    g_v256f[10] = v256f_25;
    g_v512h[4] = v512h_28;
    g_v128h[10] = v128h_1;
    g_m32[11] = m32_19;
    return p0;
}

__mmask16 torture_289(__m512h p0, __m128i p1) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_fmsub_ps(v512f_1, v512f_1, v512f_1);
    __m128i v128i_3 = _mm_unpacklo_epi32(p1, p1);
    __m512 v512f_4 = _mm512_div_ps(v512f_1, v512f_1);
    __mmask16 m16_5 = (__mmask16)(g_m16[4] ^ 73);
    __mmask16 m16_6 = _kxor_mask16(m16_5, m16_5);
    __m512 v512f_7 = _mm512_unpacklo_ps(v512f_1, v512f_1);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_9 = _mm512_shuffle_i32x4(v512i_8, v512i_8, 0x1B);
    __m128i v128i_10 = _mm_or_si128(v128i_3, v128i_3);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_max_pd(v512d_11, v512d_11);
    __m512i v512i_13 = _mm512_dpbusd_epi32(v512i_9, v512i_9, v512i_9);
    __mmask64 m64_14 = _mm512_cmp_epi8_mask(v512i_9, v512i_8, 1);
    __m256d v256d_15 = g_v256d[1];
    __m256d v256d_16 = _mm256_div_pd(v256d_15, v256d_15);
    __m256 v256f_17 = _mm256_loadu_ps(fbuf);
    __m256 v256f_18 = _mm256_add_ps(v256f_17, v256f_17);
    __m128i v128i_19 = _mm_sub_epi16(v128i_3, v128i_3);
    __mmask16 m16_20 = _kor_mask16(m16_5, m16_5);
    __m512d v512d_21 = _mm512_max_pd(v512d_12, v512d_11);
    __m128i v128i_22 = _mm_mullo_epi32(v128i_19, p1);
    __m128 v128f_23 = g_v128f[4];
    __m128 v128f_24 = _mm_div_ps(v128f_23, v128f_23);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_6) & 7); i_25++) {
    __m512 v512f_26 = _mm512_roundscale_ps(v512f_4, 0x10);
    __m128 v128f_27 = _mm512_extractf32x4_ps(v512f_2, 2);
    __mmask16 m16_28 = _knot_mask16(m16_20);
    }
    __m128i v128i_29 = _mm_sllv_epi32(v128i_19, v128i_19);
    __m512i v512i_30 = _mm512_slli_epi32(v512i_8, 3);
    __m128i v128i_31 = _mm_sllv_epi32(v128i_19, p1);
    __m512i v512i_32 = _mm512_shuffle_i32x4(v512i_30, v512i_9, 0x1B);
    __m512d v512d_33 = _mm512_div_pd(v512d_21, v512d_21);
    __m128 v128f_34 = _mm_div_ps(v128f_23, v128f_24);
    __m128i v128i_35 = _mm_and_si128(v128i_3, v128i_31);
    __m256i v256i_36 = g_v256i[5];
    __m256i v256i_37 = _mm256_and_si256(v256i_36, v256i_36);
    __m512h v512h_38 = _mm512_mul_ph(p0, p0);
    __m512h v512h_39 = _mm512_getexp_ph(v512h_38);
    __m256i v256i_40 = _mm256_slli_epi32(v256i_37, 3);
    __m256i v256i_41 = _mm256_and_si256(v256i_36, v256i_40);
    g_v512h[0] = v512h_39;
    g_m16[7] = m16_20;
    g_m64[1] = m64_14;
    return m16_20;
}

__m128h torture_290(__m256i p0, __m512d p1) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_sub_epi32(v128i_1, v128i_1);
    __m512 v512f_3 = g_v512f[10];
    __mmask16 m16_4 = _mm512_cmp_ps_mask(v512f_3, v512f_3, _CMP_LT_OQ);
    __m256i v256i_5 = _mm256_sllv_epi32(p0, p0);
    __m128d v128d_6 = g_v128d[2];
    __m128d v128d_7 = _mm_sqrt_pd(v128d_6);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __mmask64 m64_9 = _mm512_cmp_epi8_mask(v512i_8, v512i_8, 1);
    __m512d v512d_10 = _mm512_mul_pd(p1, p1);
    __mmask8 m8_11 = (__mmask8)(g_m8[0] ^ 163);
    __mmask8 m8_12 = _kand_mask8(m8_11, m8_11);
    __m256h v256h_13 = g_v256h[1];
    __m512 v512f_14 = _mm512_cvtxph_ps(v256h_13);
    __m256i v256i_15 = _mm256_set1_epi32(0x55);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_4) & 7); i_16++) {
    __m256 v256f_17 = g_v256f[3];
    __m256 v256f_18 = _mm256_sqrt_ps(v256f_17);
    __m128d v128d_19 = _mm_sqrt_pd(v128d_6);
    }
    __m256d v256d_20 = g_v256d[7];
    __m256d v256d_21 = _mm256_sub_pd(v256d_20, v256d_20);
    __m256 v256f_22 = g_v256f[10];
    __m512d v512d_23 = _mm512_cvtps_pd(v256f_22);
    __m128i v128i_24 = _mm_sub_epi32(v128i_1, v128i_2);
    __m512h v512h_25 = g_v512h[4];
    __m512h v512h_26 = _mm512_add_ph(v512h_25, v512h_25);
    __m512 v512f_27 = _mm512_max_ps(v512f_14, v512f_3);
    __m256 v256f_28 = _mm256_fmadd_ps(v256f_22, v256f_22, v256f_22);
    __m512i v512i_29 = _mm512_broadcastmb_epi64(m8_11);
    __m512i v512i_30 = _mm512_dpbusd_epi32(v512i_29, v512i_8, v512i_8);
    __m256i v256i_31 = _mm256_sllv_epi32(p0, v256i_5);
    __m512 v512f_32 = _mm512_add_ps(v512f_3, v512f_14);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_4) & 7); i_33++) {
    __m128i v128i_34 = _mm_slli_epi32(v128i_1, 3);
    __mmask16 m16_35 = _kor_mask16(m16_4, m16_4);
    }
    g_v512f[3] = v512f_14;
    g_v512d[6] = p1;
    g_v256f[7] = v256f_22;
    g_v128i[1] = v128i_1;
    g_v512h[2] = v512h_26;
    g_m16[9] = m16_4;
    __m128h v128h_36 = g_v128h[5];
    return v128h_36;
}

__m128d torture_291(__m128 p0, __m512i p1) {
    __m256d v256d_1 = g_v256d[6];
    __m256d v256d_2 = _mm256_div_pd(v256d_1, v256d_1);
    __m256d v256d_3 = _mm256_div_pd(v256d_2, v256d_1);
    __m512 v512f_4 = g_v512f[5];
    __m512 v512f_5 = _mm512_xor_ps(v512f_4, v512f_4);
    __m128i v128i_6 = g_v128i[6];
    __m128i v128i_7 = _mm_abs_epi32(v128i_6);
    __mmask16 m16_8 = (__mmask16)(g_m16[9] ^ 217);
    __m512 v512f_9 = _mm512_mask_blend_ps(m16_8, v512f_5, v512f_4);
    __m128i v128i_10 = _mm_sub_epi32(v128i_6, v128i_6);
    __m256 v256f_11 = g_v256f[3];
    __m256 v256f_12 = _mm256_fmadd_ps(v256f_11, v256f_11, v256f_11);
    __m512 v512f_13 = _mm512_unpacklo_ps(v512f_4, v512f_4);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m512i v512i_14 = _mm512_permutex2var_epi32(p1, p1, p1);
    __m128d v128d_15 = _mm_set1_pd(2.0);
    __m512 v512f_16 = _mm512_getexp_ps(v512f_5);
    __m128i v128i_17 = _mm_add_epi16(v128i_6, v128i_6);
    __m512i v512i_18 = _mm512_cvtepi8_epi32(v128i_6);
    unsigned u32_19 = 58;
    __mmask16 m16_20 = _cvtu32_mask16(u32_19);
    }
    __m128d v128d_21 = g_v128d[11];
    __m128d v128d_22 = _mm_sqrt_pd(v128d_21);
    __m512i v512i_23 = _mm512_dpbusd_epi32(p1, p1, p1);
    __m256 v256f_24 = _mm256_sub_ps(v256f_12, v256f_11);
    __m512i v512i_25 = _mm512_lzcnt_epi32(v512i_23);
    __m512i v512i_26 = _mm512_xor_si512(v512i_23, v512i_23);
    __m512 v512f_27 = _mm512_sqrt_ps(v512f_13);
    __mmask8 m8_28 = (__mmask8)(g_m8[7] ^ 197);
    __mmask16 m16_29 = _mm512_kunpackb(m8_28, m8_28);
    __m128 v128f_30 = _mm_max_ps(p0, p0);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_29) & 7); i_31++) {
    __m256i v256i_32 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_33 = _mm256_sllv_epi32(v256i_32, v256i_32);
    __m256 v256f_34 = _mm256_and_ps(v256f_24, v256f_11);
    }
    g_v512f[7] = v512f_27;
    g_v512i[11] = v512i_23;
    g_v128i[4] = v128i_7;
    g_m8[1] = m8_28;
    return v128d_22;
}

__m128 torture_292(__m128h p0, __m256i p1) {
    __mmask8 m8_1 = (__mmask8)(g_m8[3] ^ 175);
    __mmask16 m16_2 = _mm512_kunpackb(m8_1, m8_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m256d v256d_4 = _mm512_extractf64x4_pd(v512d_3, 1);
    __m256i v256i_5 = _mm256_mullo_epi32(p1, p1);
    __m512h v512h_6 = g_v512h[11];
    __m512h v512h_7 = _mm512_mul_ph(v512h_6, v512h_6);
    __m512 v512f_8 = g_v512f[2];
    __m512 v512f_9 = _mm512_and_ps(v512f_8, v512f_8);
    __mmask16 m16_10 = _mm512_kunpackb(m8_1, m8_1);
    __m128i v128i_11 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_12 = _mm_or_si128(v128i_11, v128i_11);
    __m256 v256f_13 = g_v256f[5];
    __m256 v256f_14 = _mm256_and_ps(v256f_13, v256f_13);
    __m512 v512f_15 = _mm512_div_ps(v512f_9, v512f_8);
    __m512i v512i_16 = g_v512i[8];
    __m512i v512i_17 = _mm512_mask_add_epi32(v512i_16, m16_10, v512i_16, v512i_16);
    __m128i v128i_18 = _mm_sub_epi64(v128i_11, v128i_11);
    __mmask16 m16_19 = _knot_mask16(m16_10);
    __mmask16 m16_20 = _kshiftri_mask16(m16_19, 1);
    __m512h v512h_21 = _mm512_cvtepi16_ph(v512i_17);
    __m128d v128d_22 = g_v128d[8];
    __m128d v128d_23 = _mm_min_pd(v128d_22, v128d_22);
    __m256d v256d_24 = _mm256_sqrt_pd(v256d_4);
    __m256 v256f_25 = _mm256_sub_ps(v256f_13, v256f_14);
    __m256 v256f_26 = _mm256_fmadd_ps(v256f_25, v256f_13, v256f_14);
    __m512 v512f_27 = _mm512_unpacklo_ps(v512f_9, v512f_15);
    __mmask16 m16_28 = _mm512_kunpackb(m8_1, m8_1);
    __mmask16 m16_29 = _mm512_testn_epi32_mask(v512i_16, v512i_16);
    __m128i v128i_30 = _mm_sub_epi16(v128i_11, v128i_11);
    __m512 v512f_31 = _mm512_min_ps(v512f_27, v512f_8);
    __m256d v256d_32 = _mm256_set1_pd(2.0);
    __m128i v128i_33 = _mm_add_epi16(v128i_18, v128i_11);
    g_v512i[1] = v512i_16;
    g_v128d[5] = v128d_23;
    g_v128i[9] = v128i_30;
    g_v512h[1] = v512h_21;
    g_v128h[5] = p0;
    g_m8[1] = m8_1;
    __m128 v128f_34 = g_v128f[11];
    return v128f_34;
}

__mmask32 torture_293(__m128h p0, __m128h p1, __m512d p2, __m512d p3, __m256i p4) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_rcp14_ps(v512f_1);
    __m512h v512h_3 = g_v512h[7];
    __m512h v512h_4 = _mm512_rcp_ph(v512h_3);
    __m256i v256i_5 = _mm256_sub_epi32(p4, p4);
    __m256i v256i_6 = _mm256_add_epi8(v256i_5, v256i_5);
    __m256i v256i_7 = _mm256_add_epi8(p4, v256i_5);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_9 = _mm_add_epi8(v128i_8, v128i_8);
    __m512i v512i_10 = g_v512i[4];
    __m512i v512i_11 = _mm512_lzcnt_epi32(v512i_10);
    __m512i v512i_12 = _mm512_set1_epi32(0x55);
    __m128i v128i_13 = _mm_or_si128(v128i_8, v128i_9);
    __m512 v512f_14 = _mm512_cvtepi32_ps(v512i_11);
    __m512i v512i_15 = _mm512_xor_si512(v512i_10, v512i_12);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_shuffle_ps(v128f_16, v128f_16, 0x1B);
    __m512i v512i_18 = _mm512_broadcast_i32x4(v128i_8);
    __m512i v512i_19 = _mm512_abs_epi32(v512i_11);
    __m512i v512i_20 = _mm512_rol_epi32(v512i_18, 5);
    __m512i v512i_21 = _mm512_dpwssd_epi32(v512i_20, v512i_20, v512i_15);
    __m256i v256i_22 = _mm256_add_epi64(v256i_6, p4);
    __m128 v128f_23 = _mm_fmadd_ps(v128f_16, v128f_16, v128f_16);
    int s32_24 = 71;
    if ((s32_24) & 1ULL) {
    __m512d v512d_25 = _mm512_min_pd(p3, p3);
    __m512i v512i_26 = _mm512_add_epi8(v512i_18, v512i_10);
    }
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m256 v256f_28 = _mm256_unpacklo_ps(v256f_27, v256f_27);
    __m512 v512f_29 = _mm512_shuffle_f32x4(v512f_14, v512f_2, 0x4E);
    __m512h v512h_30 = _mm512_getexp_ph(v512h_3);
    __m128i v128i_31 = _mm_or_si128(v128i_9, v128i_9);
    __m512i v512i_32 = _mm512_add_epi16(v512i_20, v512i_20);
    __m512 v512f_33 = _mm512_and_ps(v512f_29, v512f_1);
    __m128d v128d_34 = g_v128d[4];
    __m128d v128d_35 = _mm_min_pd(v128d_34, v128d_34);
    __m128 v128f_36 = _mm_div_ps(v128f_17, v128f_16);
    __mmask16 m16_37 = _mm512_movepi32_mask(v512i_19);
    __m256 v256f_38 = _mm256_sqrt_ps(v256f_27);
    __m512d v512d_39 = _mm512_sqrt_pd(p3);
    for (int i_40 = 0; i_40 < (int)(((unsigned long long)m16_37) & 7); i_40++) {
    __m128d v128d_41 = _mm_div_pd(v128d_35, v128d_35);
    __m128i v128i_42 = _mm_or_si128(v128i_13, v128i_13);
    }
    g_v512f[1] = v512f_1;
    g_v256f[4] = v256f_28;
    g_v256i[10] = v256i_22;
    g_v128i[1] = v128i_13;
    __mmask32 m32_43 = (__mmask32)(g_m32[1] ^ 87);
    return m32_43;
}

__mmask32 torture_294(__m128d p0, __m256d p1, __m128 p2, __m128h p3, __m256d p4) {
    __m256d v256d_1 = _mm256_sub_pd(p1, p4);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_mul_ps(v512f_2, v512f_2);
    __m256d v256d_4 = _mm256_max_pd(p4, p4);
    __mmask64 m64_5 = (__mmask64)(g_m64[6] ^ 176);
    __mmask64 m64_6 = _kor_mask64(m64_5, m64_5);
    __m128 v128f_7 = _mm_max_ps(p2, p2);
    __m512 v512f_8 = _mm512_sub_ps(v512f_2, v512f_2);
    __m256 v256f_9 = _mm256_loadu_ps(fbuf);
    __m256 v256f_10 = _mm256_unpacklo_ps(v256f_9, v256f_9);
    __m512 v512f_11 = _mm512_xor_ps(v512f_3, v512f_8);
    __m256 v256f_12 = _mm256_div_ps(v256f_10, v256f_9);
    __m256d v256d_13 = _mm256_add_pd(v256d_1, p1);
    __m512 v512f_14 = _mm512_shuffle_ps(v512f_11, v512f_8, 0x1B);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m64_5) & 7); i_15++) {
    __m512h v512h_16 = g_v512h[4];
    __m512h v512h_17 = _mm512_rcp_ph(v512h_16);
    __m128 v128f_18 = _mm512_extractf32x4_ps(v512f_11, 2);
    __m512d v512d_19 = g_v512d[0];
    __m512d v512d_20 = _mm512_mul_pd(v512d_19, v512d_19);
    }
    __m512h v512h_21 = g_v512h[3];
    __m512h v512h_22 = _mm512_fmsub_ph(v512h_21, v512h_21, v512h_21);
    __mmask16 m16_23 = (__mmask16)(g_m16[3] ^ 148);
    unsigned u32_24 = _cvtmask16_u32(m16_23);
    __mmask16 m16_25 = _kor_mask16(m16_23, m16_23);
    __m512i v512i_26 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_27 = _mm512_conflict_epi32(v512i_26);
    __m256i v256i_28 = g_v256i[11];
    __m256i v256i_29 = _mm256_add_epi8(v256i_28, v256i_28);
    __mmask32 m32_30 = (__mmask32)(g_m32[7] ^ 206);
    __mmask32 m32_31 = _kxor_mask32(m32_30, m32_30);
    __m512h v512h_32 = _mm512_cvtepi16_ph(v512i_27);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_23) & 7); i_33++) {
    __m128 v128f_34 = _mm_sqrt_ps(v128f_7);
    __m512d v512d_35 = _mm512_cvtps_pd(v256f_9);
    __mmask16 m16_36 = _kor_mask16(m16_25, m16_23);
    __m128 v128f_37 = _mm_fmsub_ps(v128f_34, p2, v128f_34);
    }
    g_v512f[0] = v512f_14;
    g_v512i[7] = v512i_26;
    g_v128f[2] = v128f_7;
    g_v512h[11] = v512h_32;
    g_m16[11] = m16_25;
    g_m64[11] = m64_5;
    return m32_31;
}

__m512i torture_295(__m128h p0, __m256 p1) {
    __m512i v512i_1 = g_v512i[0];
    __m512i v512i_2 = _mm512_popcnt_epi64(v512i_1);
    __mmask8 m8_3 = (__mmask8)(g_m8[11] ^ 7);
    __mmask16 m16_4 = _mm512_kunpackb(m8_3, m8_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_add_epi64(v128i_5, v128i_5);
    __m256d v256d_7 = g_v256d[7];
    __m256d v256d_8 = _mm256_min_pd(v256d_7, v256d_7);
    __m512d v512d_9 = _mm512_set1_pd(2.0);
    __m512 v512f_10 = g_v512f[2];
    __m512 v512f_11 = _mm512_div_ps(v512f_10, v512f_10);
    __m128d v128d_12 = _mm_set1_pd(2.0);
    __m512d v512d_13 = _mm512_unpackhi_pd(v512d_9, v512d_9);
    __m128i v128i_14 = _mm_sub_epi16(v128i_6, v128i_5);
    __m512h v512h_15 = g_v512h[3];
    __m512h v512h_16 = _mm512_mul_ph(v512h_15, v512h_15);
    __m256i v256i_17 = g_v256i[1];
    __m256i v256i_18 = _mm256_add_epi8(v256i_17, v256i_17);
    __m512i v512i_19 = _mm512_dpwssd_epi32(v512i_2, v512i_1, v512i_2);
    __m128i v128i_20 = _mm_add_epi32(v128i_14, v128i_5);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_4) & 7); i_21++) {
    __m256i v256i_22 = _mm256_shuffle_epi8(v256i_17, v256i_17);
    __m256i v256i_23 = _mm256_set1_epi32(0x55);
    __m512 v512f_24 = _mm512_fmadd_ps(v512f_11, v512f_11, v512f_11);
    __m512i v512i_25 = _mm512_dpbusd_epi32(v512i_1, v512i_2, v512i_1);
    __m128i v128i_26 = _mm_and_si128(v128i_6, v128i_6);
    }
    __m256i v256i_27 = _mm256_add_epi32(v256i_17, v256i_17);
    __m256i v256i_28 = _mm256_or_si256(v256i_18, v256i_27);
    __m512 v512f_29 = _mm512_fmsub_ps(v512f_10, v512f_11, v512f_10);
    __m512 v512f_30 = _mm512_min_ps(v512f_11, v512f_11);
    __m512d v512d_31 = _mm512_min_pd(v512d_9, v512d_9);
    __m512 v512f_32 = _mm512_maskz_mul_ps(m16_4, v512f_30, v512f_10);
    __m512 v512f_33 = _mm512_cvtepi32_ps(v512i_1);
    __m128 v128f_34 = g_v128f[2];
    __m128 v128f_35 = _mm_max_ps(v128f_34, v128f_34);
    __m256i v256i_36 = _mm256_slli_epi32(v256i_27, 3);
    __mmask64 m64_37 = _mm512_movepi8_mask(v512i_19);
    __mmask16 m16_38 = _kshiftri_mask16(m16_4, 1);
    __m512h v512h_39 = _mm512_sqrt_ph(v512h_16);
    for (int i_40 = 0; i_40 < (int)(((unsigned long long)m16_38) & 7); i_40++) {
    __m128i v128i_41 = _mm_slli_epi32(v128i_5, 3);
    __m512i v512i_42 = _mm512_add_epi16(v512i_19, v512i_1);
    __m512d v512d_43 = _mm512_fmadd_pd(v512d_9, v512d_9, v512d_31);
    __m512i v512i_44 = _mm512_dpbusd_epi32(v512i_19, v512i_19, v512i_42);
    }
    g_v512d[7] = v512d_31;
    g_v512i[2] = v512i_1;
    g_v256i[6] = v256i_28;
    g_m16[3] = m16_4;
    g_m64[2] = m64_37;
    return v512i_2;
}

__m512d torture_296(__m512h p0, __m256h p1, __m128d p2) {
    __m256 v256f_1 = g_v256f[7];
    __m256 v256f_2 = _mm256_add_ps(v256f_1, v256f_1);
    __m512d v512d_3 = g_v512d[6];
    __m512d v512d_4 = _mm512_sub_pd(v512d_3, v512d_3);
    __m512 v512f_5 = g_v512f[6];
    __m512 v512f_6 = _mm512_unpacklo_ps(v512f_5, v512f_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512h v512h_8 = _mm512_cvtepi16_ph(v512i_7);
    __m512i v512i_9 = _mm512_mullo_epi32(v512i_7, v512i_7);
    __m128d v128d_10 = _mm_max_pd(p2, p2);
    __m128 v128f_11 = g_v128f[7];
    __m128 v128f_12 = _mm_div_ps(v128f_11, v128f_11);
    __m128i v128i_13 = g_v128i[0];
    __m128i v128i_14 = _mm_xor_si128(v128i_13, v128i_13);
    __m256 v256f_15 = _mm256_div_ps(v256f_1, v256f_1);
    __m128 v128f_16 = _mm512_extractf32x4_ps(v512f_6, 2);
    __m128 v128f_17 = _mm_add_ps(v128f_12, v128f_16);
    __m256i v256i_18 = g_v256i[2];
    __m256i v256i_19 = _mm256_sub_epi16(v256i_18, v256i_18);
    __m512 v512f_20 = _mm512_xor_ps(v512f_6, v512f_6);
    __m256i v256i_21 = _mm256_shuffle_epi8(v256i_19, v256i_19);
    int s32_22 = 16;
    for (int i_23 = 0; i_23 < (int)((s32_22) & 7); i_23++) {
    __m512d v512d_24 = _mm512_div_pd(v512d_3, v512d_3);
    __m256d v256d_25 = g_v256d[10];
    __m256d v256d_26 = _mm256_min_pd(v256d_25, v256d_25);
    __mmask16 m16_27 = (__mmask16)(g_m16[10] ^ 9);
    __mmask16 m16_28 = _knot_mask16(m16_27);
    __m128d v128d_29 = _mm_sqrt_pd(p2);
    __m512i v512i_30 = _mm512_dpwssd_epi32(v512i_9, v512i_7, v512i_7);
    }
    __mmask16 m16_31 = (__mmask16)(g_m16[8] ^ 245);
    __mmask32 m32_32 = _mm512_kunpackw(m16_31, m16_31);
    __m512d v512d_33 = _mm512_unpackhi_pd(v512d_3, v512d_3);
    __mmask16 m16_34 = _mm512_fpclass_ps_mask(v512f_6, 0x3);
    __m256 v256f_35 = _mm512_cvtpd_ps(v512d_33);
    __m512i v512i_36 = _mm512_sub_epi8(v512i_9, v512i_9);
    __m512i v512i_37 = _mm512_madd52lo_epu64(v512i_36, v512i_36, v512i_36);
    __m128h v128h_38 = _mm512_cvtpd_ph(v512d_3);
    __m128h v128h_39 = _mm512_cvtpd_ph(v512d_4);
    __m512 v512f_40 = _mm512_mask_add_ps(v512f_20, m16_34, v512f_20, v512f_5);
    __m256d v256d_41 = g_v256d[10];
    __m256d v256d_42 = _mm256_unpackhi_pd(v256d_41, v256d_41);
    __m128 v128f_43 = _mm_div_ps(v128f_11, v128f_17);
    __m512i v512i_44 = _mm512_permutex2var_epi32(v512i_9, v512i_9, v512i_36);
    for (int i_45 = 0; i_45 < (int)(((unsigned long long)m16_31) & 7); i_45++) {
    __m512 v512f_46 = _mm512_shuffle_f32x4(v512f_6, v512f_40, 0x4E);
    __mmask16 m16_47 = _mm512_test_epi32_mask(v512i_37, v512i_44);
    __mmask64 m64_48 = _mm512_movepi8_mask(v512i_7);
    __m256 v256f_49 = _mm256_xor_ps(v256f_35, v256f_2);
    __m512 v512f_50 = _mm512_xor_ps(v512f_6, v512f_20);
    }
    g_v256d[7] = v256d_42;
    g_v256i[2] = v256i_19;
    g_v128i[10] = v128i_14;
    g_v512h[11] = v512h_8;
    g_m32[11] = m32_32;
    return v512d_3;
}

__m256d torture_297(__m512h p0, __m512d p1) {
    __m512i v512i_1 = _mm512_set1_epi32(0x55);
    __m256i v256i_2 = g_v256i[5];
    __m256i v256i_3 = _mm256_mullo_epi32(v256i_2, v256i_2);
    __mmask16 m16_4 = (__mmask16)(g_m16[3] ^ 72);
    __m512i v512i_5 = _mm512_mask_blend_epi32(m16_4, v512i_1, v512i_1);
    __m512 v512f_6 = g_v512f[3];
    __m512 v512f_7 = _mm512_shuffle_ps(v512f_6, v512f_6, 0x1B);
    unsigned u32_8 = _cvtmask16_u32(m16_4);
    __mmask64 m64_9 = _mm512_cmp_epi8_mask(v512i_5, v512i_5, 1);
    __m128i v128i_10 = g_v128i[11];
    __m512i v512i_11 = _mm512_broadcast_i32x4(v128i_10);
    __m128i v128i_12 = _mm_shuffle_epi8(v128i_10, v128i_10);
    __m256i v256i_13 = _mm256_add_epi8(v256i_3, v256i_3);
    __m256i v256i_14 = _mm256_add_epi32(v256i_2, v256i_13);
    __m512h v512h_15 = _mm512_getexp_ph(p0);
    __mmask16 m16_16 = _mm512_testn_epi32_mask(v512i_11, v512i_1);
    __mmask64 m64_17 = _mm512_movepi8_mask(v512i_11);
    __m512 v512f_18 = _mm512_fmsub_ps(v512f_6, v512f_7, v512f_7);
    __m128i v128i_19 = _mm_xor_si128(v128i_10, v128i_12);
    __mmask64 m64_20 = _mm512_movepi8_mask(v512i_11);
    __mmask8 m8_21 = (__mmask8)(g_m8[2] ^ 216);
    __m512i v512i_22 = _mm512_broadcastmb_epi64(m8_21);
    __m128d v128d_23 = g_v128d[2];
    __m128d v128d_24 = _mm_sub_pd(v128d_23, v128d_23);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_16) & 7); i_25++) {
    __m512 v512f_26 = _mm512_fmsub_ps(v512f_7, v512f_18, v512f_18);
    __m128d v128d_27 = _mm_set1_pd(2.0);
    }
    __m128i v128i_28 = _mm_mullo_epi32(v128i_12, v128i_12);
    __m128i v128i_29 = _mm_sllv_epi32(v128i_19, v128i_19);
    __m256i v256i_30 = _mm256_add_epi64(v256i_14, v256i_2);
    __m512 v512f_31 = _mm512_rsqrt14_ps(v512f_18);
    __m512i v512i_32 = _mm512_unpacklo_epi32(v512i_1, v512i_11);
    __m512 v512f_33 = _mm512_sub_ps(v512f_6, v512f_18);
    __m512i v512i_34 = _mm512_or_si512(v512i_22, v512i_32);
    __m256 v256f_35 = _mm256_loadu_ps(fbuf);
    __m256 v256f_36 = _mm256_shuffle_ps(v256f_35, v256f_35, 0x1B);
    __m512 v512f_37 = _mm512_div_ps(v512f_6, v512f_7);
    if (((unsigned long long)m16_4) & 1ULL) {
    __m512i v512i_38 = _mm512_sub_epi8(v512i_5, v512i_5);
    __m512i v512i_39 = _mm512_dpwssd_epi32(v512i_32, v512i_34, v512i_5);
    __m256i v256i_40 = _mm256_or_si256(v256i_30, v256i_13);
    __m256 v256f_41 = _mm256_fnmadd_ps(v256f_35, v256f_35, v256f_36);
    __m512 v512f_42 = _mm512_mul_ps(v512f_7, v512f_33);
    }
    g_v512f[3] = v512f_6;
    g_v512d[1] = p1;
    g_v256f[6] = v256f_35;
    g_v256i[8] = v256i_2;
    g_v128i[3] = v128i_29;
    g_m16[6] = m16_16;
    __m256d v256d_43 = g_v256d[8];
    return v256d_43;
}

__m128d torture_298(__m256 p0, __m128 p1, __m512 p2, __m256h p3) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_2 = _mm512_test_epi32_mask(v512i_1, v512i_1);
    __m128d v128d_3 = g_v128d[7];
    __m128d v128d_4 = _mm_add_pd(v128d_3, v128d_3);
    __m512d v512d_5 = g_v512d[2];
    __m512d v512d_6 = _mm512_insertf64x2(v512d_5, v128d_3, 3);
    __m512i v512i_7 = _mm512_sub_epi32(v512i_1, v512i_1);
    unsigned u32_8 = 59;
    __mmask16 m16_9 = _cvtu32_mask16(u32_8);
    __m256i v256i_10 = g_v256i[2];
    __m256i v256i_11 = _mm256_sub_epi64(v256i_10, v256i_10);
    __m512 v512f_12 = _mm512_mask_blend_ps(m16_9, p2, p2);
    __m512d v512d_13 = _mm512_cvtps_pd(p0);
    __m256i v256i_14 = _mm256_shuffle_epi8(v256i_10, v256i_10);
    __mmask16 m16_15 = _kand_mask16(m16_2, m16_9);
    __m512 v512f_16 = _mm512_min_ps(v512f_12, v512f_12);
    __m512i v512i_17 = _mm512_sllv_epi32(v512i_1, v512i_1);
    __m512h v512h_18 = g_v512h[7];
    __m512h v512h_19 = _mm512_rcp_ph(v512h_18);
    __mmask8 m8_20 = (__mmask8)(g_m8[1] ^ 219);
    __mmask8 m8_21 = _kand_mask8(m8_20, m8_20);
    __m256i v256i_22 = _mm256_xor_si256(v256i_10, v256i_11);
    __m512i v512i_23 = _mm512_or_si512(v512i_1, v512i_7);
    __mmask16 m16_24 = _mm512_cmp_epi32_mask(v512i_17, v512i_17, 2);
    __m256i v256i_25 = _mm256_abs_epi32(v256i_11);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_15) & 7); i_26++) {
    __m128 v128f_27 = _mm_set1_ps(1.0f);
    __m512i v512i_28 = _mm512_slli_epi32(v512i_7, 3);
    __mmask16 m16_29 = _mm512_cmp_epu32_mask(v512i_7, v512i_1, 4);
    __m256 v256f_30 = _mm256_fmsub_ps(p0, p0, p0);
    }
    __m256d v256d_31 = g_v256d[8];
    __m256d v256d_32 = _mm256_sqrt_pd(v256d_31);
    __m512 v512f_33 = _mm512_sub_ps(v512f_12, p2);
    __m512i v512i_34 = _mm512_add_epi64(v512i_23, v512i_1);
    __m128d v128d_35 = _mm_add_pd(v128d_3, v128d_3);
    __m512 v512f_36 = _mm512_cvtepi32_ps(v512i_1);
    __m128d v128d_37 = _mm_min_pd(v128d_3, v128d_3);
    g_v512f[7] = v512f_33;
    g_v512i[6] = v512i_17;
    g_v256i[4] = v256i_14;
    g_v256h[3] = p3;
    g_m8[0] = m8_20;
    return v128d_4;
}

__m512 torture_299(__m512i p0, __m512 p1) {
    __m512i v512i_1 = _mm512_mullo_epi32(p0, p0);
    __m128d v128d_2 = _mm_set1_pd(2.0);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_div_ps(v256f_3, v256f_3);
    __m128i v128i_5 = g_v128i[9];
    __m512i v512i_6 = _mm512_cvtepi8_epi32(v128i_5);
    __m256 v256f_7 = _mm256_mul_ps(v256f_4, v256f_3);
    __m256 v256f_8 = _mm256_fmsub_ps(v256f_7, v256f_7, v256f_7);
    __m128 v128f_9 = g_v128f[5];
    __m128 v128f_10 = _mm_sqrt_ps(v128f_9);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m512d v512d_12 = _mm512_max_pd(v512d_11, v512d_11);
    __m128d v128d_13 = _mm_sqrt_pd(v128d_2);
    __m256 v256f_14 = _mm256_fmsub_ps(v256f_7, v256f_8, v256f_4);
    __m512i v512i_15 = _mm512_or_si512(v512i_1, v512i_1);
    __m128i v128i_16 = _mm_sub_epi64(v128i_5, v128i_5);
    __m128 v128f_17 = _mm_unpacklo_ps(v128f_10, v128f_9);
    __m512i v512i_18 = _mm512_lzcnt_epi32(v512i_1);
    __m128d v128d_19 = _mm_mul_pd(v128d_2, v128d_13);
    __mmask16 m16_20 = (__mmask16)(g_m16[0] ^ 149);
    __mmask16 m16_21 = _kxor_mask16(m16_20, m16_20);
    __mmask16 m16_22 = _mm512_fpclass_ps_mask(p1, 0x3);
    __m128d v128d_23 = _mm_set1_pd(2.0);
    if (((unsigned long long)m16_21) & 1ULL) {
    __m512i v512i_24 = _mm512_mullo_epi32(v512i_15, v512i_6);
    __m512i v512i_25 = _mm512_sub_epi64(v512i_1, p0);
    __m256 v256f_26 = _mm256_fmsub_ps(v256f_14, v256f_4, v256f_7);
    }
    __m512i v512i_27 = _mm512_broadcastmw_epi32(m16_22);
    __m512i v512i_28 = _mm512_rolv_epi32(v512i_1, v512i_18);
    __m512 v512f_29 = _mm512_min_ps(p1, p1);
    __m256d v256d_30 = g_v256d[1];
    __m256d v256d_31 = _mm256_mul_pd(v256d_30, v256d_30);
    if (((unsigned long long)m16_21) & 1ULL) {
    __m256 v256f_32 = _mm512_cvtpd_ps(v512d_12);
    __m256 v256f_33 = _mm512_cvtpd_ps(v512d_11);
    __m128i v128i_34 = _mm_sub_epi8(v128i_5, v128i_16);
    __m512d v512d_35 = _mm512_sqrt_pd(v512d_12);
    __m512i v512i_36 = _mm512_cvtpd_epi64(v512d_35);
    __mmask16 m16_37 = _mm512_cmp_epi32_mask(v512i_1, v512i_1, 2);
    }
    g_v512i[1] = v512i_6;
    g_v256f[5] = v256f_7;
    g_v256d[9] = v256d_31;
    return v512f_29;
}

__m256d torture_300(__m256d p0, __m512i p1, __m256 p2, __m512i p3, __m256d p4) {
    __m512i v512i_1 = _mm512_add_epi64(p1, p1);
    __m512i v512i_2 = _mm512_sub_epi32(p3, p3);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_fmsub_ps(v512f_3, v512f_3, v512f_3);
    __m256 v256f_5 = _mm256_mul_ps(p2, p2);
    __m512i v512i_6 = _mm512_add_epi64(p3, p3);
    __m512i v512i_7 = _mm512_sllv_epi32(v512i_6, v512i_1);
    __m512d v512d_8 = _mm512_loadu_pd(dbuf);
    __m128h v128h_9 = _mm512_cvtpd_ph(v512d_8);
    __m512i v512i_10 = _mm512_add_epi32(v512i_6, v512i_6);
    __m256d v256d_11 = _mm256_set1_pd(2.0);
    __m512h v512h_12 = g_v512h[11];
    __m512h v512h_13 = _mm512_rcp_ph(v512h_12);
    __mmask16 m16_14 = _mm512_cmp_ps_mask(v512f_3, v512f_4, _CMP_LT_OQ);
    __m128 v128f_15 = g_v128f[0];
    __m128 v128f_16 = _mm_xor_ps(v128f_15, v128f_15);
    __m256i v256i_17 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_18 = _mm256_unpacklo_epi32(v256i_17, v256i_17);
    __m512i v512i_19 = _mm512_rol_epi32(v512i_7, 5);
    __m256d v256d_20 = _mm256_div_pd(p4, p0);
    __mmask16 m16_21 = _mm512_cmp_epi32_mask(v512i_7, v512i_10, 2);
    __mmask8 m8_22 = (__mmask8)(g_m8[1] ^ 49);
    __m512i v512i_23 = _mm512_broadcastmb_epi64(m8_22);
    __m128i v128i_24 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_25 = _mm_add_epi64(v128i_24, v128i_24);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_21) & 7); i_26++) {
    __m256 v256f_27 = _mm256_and_ps(v256f_5, v256f_5);
    __m512 v512f_28 = _mm512_insertf32x8(v512f_4, p2, 1);
    __m256i v256i_29 = _mm256_sub_epi8(v256i_17, v256i_17);
    __m512 v512f_30 = _mm512_insertf32x8(v512f_4, v256f_5, 1);
    __m256i v256i_31 = _mm256_sub_epi16(v256i_29, v256i_18);
    }
    g_v512f[10] = v512f_3;
    g_v512d[4] = v512d_8;
    g_v256i[3] = v256i_18;
    g_v128f[6] = v128f_16;
    g_v128h[5] = v128h_9;
    g_m8[10] = m8_22;
    return v256d_20;
}

__m256h torture_301(__m512 p0, __m256d p1) {
    __m256i v256i_1 = _mm256_set1_epi32(0x55);
    __m512 v512f_2 = _mm512_xor_ps(p0, p0);
    __m256 v256f_3 = g_v256f[10];
    __m256 v256f_4 = _mm256_and_ps(v256f_3, v256f_3);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_6 = _mm512_rolv_epi32(v512i_5, v512i_5);
    __m512h v512h_7 = g_v512h[2];
    __mmask32 m32_8 = _mm512_cmp_ph_mask(v512h_7, v512h_7, 1);
    __mmask16 m16_9 = _mm512_fpclass_ps_mask(v512f_2, 0x3);
    __m512i v512i_10 = _mm512_set1_epi32(0x55);
    __mmask64 m64_11 = _mm512_movepi8_mask(v512i_5);
    __mmask8 m8_12 = (__mmask8)(g_m8[1] ^ 24);
    __m512i v512i_13 = _mm512_broadcastmb_epi64(m8_12);
    __m512i v512i_14 = _mm512_sub_epi16(v512i_13, v512i_10);
    __m512d v512d_15 = g_v512d[1];
    __m512d v512d_16 = _mm512_fmadd_pd(v512d_15, v512d_15, v512d_15);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_9) & 7); i_17++) {
    __m512i v512i_18 = _mm512_mask_add_epi32(v512i_10, m16_9, v512i_14, v512i_13);
    __m128i v128i_19 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_20 = _mm_unpacklo_epi32(v128i_19, v128i_19);
    __m512i v512i_21 = _mm512_add_epi16(v512i_10, v512i_10);
    }
    __m256i v256i_22 = _mm256_add_epi32(v256i_1, v256i_1);
    __m512 v512f_23 = _mm512_rsqrt14_ps(p0);
    __m512i v512i_24 = _mm512_add_epi32(v512i_5, v512i_6);
    __m512i v512i_25 = _mm512_cvtps_epi32(p0);
    __m512 v512f_26 = _mm512_sqrt_ps(v512f_23);
    __m256i v256i_27 = _mm256_abs_epi32(v256i_1);
    __m512i v512i_28 = _mm512_and_si512(v512i_5, v512i_14);
    __m256 v256f_29 = _mm256_and_ps(v256f_3, v256f_4);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_9) & 7); i_30++) {
    __m256d v256d_31 = _mm512_extractf64x4_pd(v512d_15, 1);
    __m256d v256d_32 = _mm256_min_pd(p1, v256d_31);
    __mmask32 m32_33 = _mm512_kunpackw(m16_9, m16_9);
    __m256 v256f_34 = _mm256_fmsub_ps(v256f_3, v256f_29, v256f_29);
    }
    g_v512i[7] = v512i_13;
    g_v256d[3] = p1;
    g_v256i[9] = v256i_22;
    g_m8[11] = m8_12;
    g_m32[11] = m32_8;
    __m256h v256h_35 = g_v256h[5];
    return v256h_35;
}

__mmask8 torture_302(__m128h p0, __m512i p1, __m256i p2) {
    __m512i v512i_1 = _mm512_lzcnt_epi32(p1);
    __m128i v128i_2 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_3 = _mm_sub_epi8(v128i_2, v128i_2);
    __m128 v128f_4 = _mm_loadu_ps(fbuf);
    __m128 v128f_5 = _mm_max_ps(v128f_4, v128f_4);
    __mmask16 m16_6 = (__mmask16)(g_m16[8] ^ 214);
    __m512i v512i_7 = _mm512_mask_add_epi32(v512i_1, m16_6, v512i_1, p1);
    __m512i v512i_8 = _mm512_sub_epi64(v512i_1, v512i_1);
    __m128 v128f_9 = _mm_sub_ps(v128f_5, v128f_5);
    __m512d v512d_10 = g_v512d[9];
    __m512d v512d_11 = _mm512_div_pd(v512d_10, v512d_10);
    __m512 v512f_12 = g_v512f[1];
    __m512 v512f_13 = _mm512_min_ps(v512f_12, v512f_12);
    __m512i v512i_14 = _mm512_rol_epi32(v512i_8, 5);
    __m128 v128f_15 = _mm_fmsub_ps(v128f_5, v128f_4, v128f_5);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m256d v256d_16 = g_v256d[9];
    __m256d v256d_17 = _mm256_add_pd(v256d_16, v256d_16);
    __m128 v128f_18 = _mm_fmsub_ps(v128f_15, v128f_15, v128f_5);
    __m512 v512f_19 = _mm512_fmsub_ps(v512f_13, v512f_13, v512f_13);
    __m256h v256h_20 = g_v256h[9];
    __m512 v512f_21 = _mm512_cvtxph_ps(v256h_20);
    __m512 v512f_22 = _mm512_fmadd_ps(v512f_19, v512f_12, v512f_12);
    }
    __m256i v256i_23 = _mm256_sub_epi16(p2, p2);
    __m256i v256i_24 = _mm256_add_epi8(p2, p2);
    __m256d v256d_25 = g_v256d[11];
    __m256d v256d_26 = _mm256_unpackhi_pd(v256d_25, v256d_25);
    __m512i v512i_27 = _mm512_cvtepi8_epi32(v128i_2);
    __m512i v512i_28 = _mm512_slli_epi32(v512i_7, 3);
    __m512i v512i_29 = _mm512_broadcastmw_epi32(m16_6);
    if (((unsigned long long)m16_6) & 1ULL) {
    __mmask16 m16_30 = _mm512_cmp_epu32_mask(v512i_14, v512i_28, 4);
    __m256d v256d_31 = _mm512_extractf64x4_pd(v512d_11, 1);
    }
    g_v256i[5] = p2;
    g_v128f[3] = v128f_5;
    g_v128h[3] = p0;
    __mmask8 m8_32 = (__mmask8)(g_m8[3] ^ 8);
    return m8_32;
}

__m128d torture_303(__m128 p0, __m128i p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_unpacklo_epi32(v512i_1, v512i_1);
    __m512d v512d_3 = g_v512d[9];
    __m512d v512d_4 = _mm512_shuffle_f64x2(v512d_3, v512d_3, 0xEE);
    __m128 v128f_5 = _mm_unpacklo_ps(p0, p0);
    __m512i v512i_6 = _mm512_shuffle_i32x4(v512i_2, v512i_2, 0x1B);
    __m256 v256f_7 = _mm256_loadu_ps(fbuf);
    __m256 v256f_8 = _mm256_xor_ps(v256f_7, v256f_7);
    __m512i v512i_9 = _mm512_cvtpd_epi64(v512d_3);
    __mmask16 m16_10 = (__mmask16)(g_m16[6] ^ 117);
    __mmask16 m16_11 = _kxor_mask16(m16_10, m16_10);
    __mmask16 m16_12 = _kxor_mask16(m16_11, m16_10);
    __m512 v512f_13 = _mm512_loadu_ps(fbuf);
    __m512 v512f_14 = _mm512_unpacklo_ps(v512f_13, v512f_13);
    __m128 v128f_15 = _mm_add_ps(p0, p0);
    __m512 v512f_16 = _mm512_div_ps(v512f_14, v512f_14);
    __m256i v256i_17 = g_v256i[9];
    __m256i v256i_18 = _mm256_slli_epi32(v256i_17, 3);
    __m512 v512f_19 = _mm512_unpacklo_ps(v512f_14, v512f_13);
    __mmask8 m8_20 = (__mmask8)(g_m8[6] ^ 175);
    __m512i v512i_21 = _mm512_broadcastmb_epi64(m8_20);
    __m512d v512d_22 = _mm512_mask_blend_pd(m8_20, v512d_3, v512d_4);
    __m256i v256i_23 = _mm256_abs_epi32(v256i_17);
    __m256h v256h_24 = g_v256h[9];
    __m512 v512f_25 = _mm512_cvtxph_ps(v256h_24);
    __m256 v256f_26 = _mm256_set1_ps(1.0f);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_11) & 7); i_27++) {
    __m512i v512i_28 = _mm512_shuffle_epi8(v512i_1, v512i_6);
    __m256i v256i_29 = _mm256_xor_si256(v256i_18, v256i_18);
    }
    __m256 v256f_30 = _mm256_sqrt_ps(v256f_26);
    __m256i v256i_31 = _mm256_unpacklo_epi32(v256i_18, v256i_23);
    __m256d v256d_32 = g_v256d[2];
    __m256d v256d_33 = _mm256_add_pd(v256d_32, v256d_32);
    __m128i v128i_34 = _mm_sub_epi64(p1, p1);
    __m512i v512i_35 = _mm512_rolv_epi32(v512i_1, v512i_9);
    __m256i v256i_36 = _mm256_slli_epi32(v256i_23, 3);
    __m256i v256i_37 = _mm256_shuffle_epi8(v256i_17, v256i_17);
    __m128i v128i_38 = _mm_add_epi64(p1, v128i_34);
    __m512d v512d_39 = _mm512_max_pd(v512d_4, v512d_22);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m512h v512h_40 = _mm512_cvtepi16_ph(v512i_6);
    __m512i v512i_41 = _mm512_shuffle_i32x4(v512i_21, v512i_21, 0x1B);
    __m256d v256d_42 = _mm512_extractf64x4_pd(v512d_22, 1);
    __mmask16 m16_43 = _mm512_cmp_ps_mask(v512f_13, v512f_19, _CMP_LT_OQ);
    __m128i v128i_44 = _mm_mullo_epi32(p1, v128i_38);
    }
    g_v512d[10] = v512d_22;
    g_m8[10] = m8_20;
    __m128d v128d_45 = g_v128d[0];
    return v128d_45;
}

__m128i torture_304(__m512 p0, __m128d p1, __m256i p2, __m512h p3, __m256h p4) {
    __m512d v512d_1 = g_v512d[0];
    __m512d v512d_2 = _mm512_add_pd(v512d_1, v512d_1);
    __m128i v128i_3 = g_v128i[7];
    __m128i v128i_4 = _mm_xor_si128(v128i_3, v128i_3);
    __m128h v128h_5 = _mm512_cvtpd_ph(v512d_2);
    __m256d v256d_6 = _mm512_extractf64x4_pd(v512d_2, 1);
    __m512i v512i_7 = _mm512_cvtepi8_epi32(v128i_4);
    __m512i v512i_8 = _mm512_rol_epi32(v512i_7, 5);
    __mmask16 m16_9 = (__mmask16)(g_m16[11] ^ 28);
    __m512i v512i_10 = _mm512_broadcastmw_epi32(m16_9);
    __m256d v256d_11 = _mm512_extractf64x4_pd(v512d_2, 1);
    __m512h v512h_12 = _mm512_getexp_ph(p3);
    __m512 v512f_13 = _mm512_sub_ps(p0, p0);
    __m128 v128f_14 = g_v128f[1];
    __m128 v128f_15 = _mm_xor_ps(v128f_14, v128f_14);
    __m128i v128i_16 = _mm_mullo_epi32(v128i_3, v128i_3);
    __mmask8 m8_17 = (__mmask8)(g_m8[1] ^ 209);
    __m512i v512i_18 = _mm512_broadcastmb_epi64(m8_17);
    __m512 v512f_19 = _mm512_xor_ps(v512f_13, v512f_13);
    __m128 v128f_20 = _mm_xor_ps(v128f_15, v128f_14);
    __m256i v256i_21 = _mm256_or_si256(p2, p2);
    __m256i v256i_22 = _mm256_sub_epi64(p2, p2);
    __m256i v256i_23 = _mm256_mullo_epi32(v256i_22, p2);
    for (int i_24 = 0; i_24 < (int)(((unsigned long long)m16_9) & 7); i_24++) {
    __m256i v256i_25 = _mm256_and_si256(p2, v256i_21);
    __m512i v512i_26 = _mm512_madd52lo_epu64(v512i_7, v512i_18, v512i_7);
    }
    __m128 v128f_27 = _mm_sub_ps(v128f_20, v128f_14);
    __m128 v128f_28 = _mm_add_ps(v128f_27, v128f_15);
    __m512i v512i_29 = _mm512_mask_blend_epi32(m16_9, v512i_7, v512i_18);
    __m128 v128f_30 = _mm_shuffle_ps(v128f_14, v128f_15, 0x1B);
    __m128d v128d_31 = _mm_fmadd_pd(p1, p1, p1);
    __m256 v256f_32 = _mm256_loadu_ps(fbuf);
    __m256 v256f_33 = _mm256_add_ps(v256f_32, v256f_32);
    __m512i v512i_34 = _mm512_broadcast_i32x4(v128i_3);
    __m128i v128i_35 = _mm_set1_epi32(0x55);
    g_v512f[1] = p0;
    g_v512d[10] = v512d_1;
    g_v512i[2] = v512i_7;
    g_v256d[7] = v256d_6;
    g_v128f[0] = v128f_27;
    g_v256h[8] = p4;
    return v128i_3;
}

__mmask16 torture_305(__m128 p0, __m256d p1, __m512h p2, __m128 p3, __m256d p4) {
    __m512i v512i_1 = g_v512i[7];
    __m512i v512i_2 = _mm512_dpbusd_epi32(v512i_1, v512i_1, v512i_1);
    __m128i v128i_3 = g_v128i[10];
    __m128i v128i_4 = _mm_sub_epi8(v128i_3, v128i_3);
    __m128d v128d_5 = g_v128d[0];
    __m128d v128d_6 = _mm_sqrt_pd(v128d_5);
    __m128d v128d_7 = _mm_min_pd(v128d_6, v128d_6);
    __m128 v128f_8 = _mm_mul_ps(p3, p0);
    __m128 v128f_9 = _mm_mul_ps(p0, p3);
    __m128i v128i_10 = _mm_shuffle_epi8(v128i_3, v128i_4);
    __m128 v128f_11 = _mm_and_ps(v128f_8, v128f_8);
    __m512i v512i_12 = _mm512_ternarylogic_epi32(v512i_2, v512i_1, v512i_1, 0x96);
    __m512i v512i_13 = _mm512_sub_epi32(v512i_12, v512i_2);
    __m256 v256f_14 = g_v256f[7];
    __m256 v256f_15 = _mm256_max_ps(v256f_14, v256f_14);
    __m512h v512h_16 = _mm512_rcp_ph(p2);
    __m256 v256f_17 = _mm256_add_ps(v256f_15, v256f_15);
    __m512d v512d_18 = _mm512_cvtps_pd(v256f_15);
    __m512i v512i_19 = _mm512_sllv_epi32(v512i_13, v512i_1);
    __m256i v256i_20 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_21 = _mm256_sub_epi16(v256i_20, v256i_20);
    int s32_22 = 56;
    for (int i_23 = 0; i_23 < (int)((s32_22) & 7); i_23++) {
    __m256 v256f_24 = _mm256_sub_ps(v256f_17, v256f_15);
    __m512i v512i_25 = _mm512_rol_epi32(v512i_2, 5);
    }
    __m128 v128f_26 = _mm_max_ps(p3, v128f_9);
    __m256 v256f_27 = _mm256_fnmadd_ps(v256f_17, v256f_14, v256f_14);
    __m512i v512i_28 = _mm512_set1_epi32(0x55);
    __mmask16 m16_29 = (__mmask16)(g_m16[7] ^ 152);
    __mmask16 m16_30 = _kxor_mask16(m16_29, m16_29);
    __m512 v512f_31 = _mm512_loadu_ps(fbuf);
    __m512 v512f_32 = _mm512_rsqrt14_ps(v512f_31);
    __m256 v256f_33 = _mm256_unpacklo_ps(v256f_15, v256f_17);
    __m512d v512d_34 = _mm512_sub_pd(v512d_18, v512d_18);
    __m256h v256h_35 = _mm512_cvtxps_ph(v512f_31);
    __m256 v256f_36 = _mm256_and_ps(v256f_15, v256f_33);
    g_v512i[1] = v512i_2;
    g_v256d[0] = p4;
    g_v128d[3] = v128d_6;
    g_v512h[1] = v512h_16;
    return m16_29;
}

__m512d torture_306(__m128h p0, __m256d p1, __m256h p2, __m512i p3, __m128 p4) {
    __m512i v512i_1 = _mm512_sub_epi16(p3, p3);
    __m128i v128i_2 = g_v128i[2];
    __m128i v128i_3 = _mm_unpacklo_epi32(v128i_2, v128i_2);
    __m256 v256f_4 = _mm256_loadu_ps(fbuf);
    __m256 v256f_5 = _mm256_and_ps(v256f_4, v256f_4);
    __m512i v512i_6 = _mm512_or_si512(v512i_1, p3);
    __mmask16 m16_7 = (__mmask16)(g_m16[1] ^ 26);
    unsigned u32_8 = _cvtmask16_u32(m16_7);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m512d v512d_10 = _mm512_add_pd(v512d_9, v512d_9);
    __m128i v128i_11 = _mm_and_si128(v128i_2, v128i_3);
    __m256i v256i_12 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_13 = _mm256_add_epi16(v256i_12, v256i_12);
    __m512 v512f_14 = g_v512f[10];
    __m512 v512f_15 = _mm512_add_ps(v512f_14, v512f_14);
    __m256 v256f_16 = _mm256_unpacklo_ps(v256f_4, v256f_4);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_7) & 7); i_17++) {
    __m512 v512f_18 = _mm512_mask_add_ps(v512f_14, m16_7, v512f_15, v512f_14);
    __m512 v512f_19 = _mm512_insertf32x8(v512f_15, v256f_16, 1);
    __mmask64 m64_20 = (__mmask64)(g_m64[7] ^ 175);
    __mmask64 m64_21 = _kor_mask64(m64_20, m64_20);
    }
    __m256 v256f_22 = _mm256_add_ps(v256f_4, v256f_16);
    __m128d v128d_23 = g_v128d[11];
    __m128d v128d_24 = _mm_div_pd(v128d_23, v128d_23);
    __m128 v128f_25 = _mm_shuffle_ps(p4, p4, 0x1B);
    __mmask32 m32_26 = (__mmask32)(g_m32[11] ^ 217);
    __mmask32 m32_27 = _kxor_mask32(m32_26, m32_26);
    __m512i v512i_28 = _mm512_add_epi64(v512i_6, v512i_6);
    __m256i v256i_29 = _mm256_sllv_epi32(v256i_13, v256i_12);
    for (int i_30 = 0; i_30 < (int)(((unsigned long long)m16_7) & 7); i_30++) {
    __m256d v256d_31 = _mm256_sqrt_pd(p1);
    __m512 v512f_32 = _mm512_mul_ps(v512f_14, v512f_14);
    }
    g_v512d[0] = v512d_10;
    g_v256i[5] = v256i_13;
    g_v128d[7] = v128d_24;
    g_v128h[9] = p0;
    return v512d_9;
}

__m512d torture_307(__m128h p0, __m256h p1, __m512h p2, __m512 p3) {
    __m256 v256f_1 = g_v256f[0];
    __m256 v256f_2 = _mm256_mul_ps(v256f_1, v256f_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_abs_epi32(v128i_3);
    __m512d v512d_5 = _mm512_loadu_pd(dbuf);
    __m512d v512d_6 = _mm512_mul_pd(v512d_5, v512d_5);
    __m256i v256i_7 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_8 = _mm256_unpacklo_epi32(v256i_7, v256i_7);
    __m512 v512f_9 = _mm512_shuffle_f32x4(p3, p3, 0x4E);
    __m128i v128i_10 = _mm_add_epi64(v128i_4, v128i_4);
    __m512i v512i_11 = g_v512i[10];
    __m512i v512i_12 = _mm512_abs_epi32(v512i_11);
    __m512 v512f_13 = _mm512_fnmadd_ps(v512f_9, p3, v512f_9);
    __m512d v512d_14 = _mm512_sub_pd(v512d_5, v512d_5);
    __m128d v128d_15 = g_v128d[1];
    __m128d v128d_16 = _mm_sqrt_pd(v128d_15);
    __m512 v512f_17 = _mm512_fmsub_ps(p3, v512f_13, p3);
    __m512i v512i_18 = _mm512_cvtps_epi32(p3);
    int s32_19 = 17;
    if ((s32_19) & 1ULL) {
    __m512i v512i_20 = _mm512_sub_epi32(v512i_11, v512i_18);
    __mmask16 m16_21 = (__mmask16)(g_m16[11] ^ 133);
    __mmask32 m32_22 = _mm512_kunpackw(m16_21, m16_21);
    __m128 v128f_23 = g_v128f[9];
    __m128 v128f_24 = _mm_fmadd_ps(v128f_23, v128f_23, v128f_23);
    __m128i v128i_25 = _mm_add_epi8(v128i_3, v128i_3);
    unsigned u32_26 = _cvtmask16_u32(m16_21);
    }
    __m128 v128f_27 = g_v128f[2];
    __m128 v128f_28 = _mm_fmsub_ps(v128f_27, v128f_27, v128f_27);
    __m512 v512f_29 = _mm512_roundscale_ps(v512f_9, 0x10);
    __m128d v128d_30 = _mm_sub_pd(v128d_16, v128d_16);
    __m128i v128i_31 = _mm_unpacklo_epi32(v128i_10, v128i_3);
    __m256d v256d_32 = g_v256d[7];
    __m256d v256d_33 = _mm256_sub_pd(v256d_32, v256d_32);
    __m256i v256i_34 = _mm256_shuffle_epi8(v256i_8, v256i_7);
    if ((s32_19) & 1ULL) {
    __mmask16 m16_35 = (__mmask16)(g_m16[11] ^ 224);
    __m512i v512i_36 = _mm512_mask_blend_epi32(m16_35, v512i_11, v512i_11);
    __m512 v512f_37 = _mm512_rcp14_ps(p3);
    __m512d v512d_38 = _mm512_set1_pd(2.0);
    __mmask16 m16_39 = _mm512_cmp_ps_mask(v512f_29, v512f_17, _CMP_LT_OQ);
    __m512 v512f_40 = _mm512_rcp14_ps(v512f_37);
    }
    g_v256d[0] = v256d_32;
    g_v256i[10] = v256i_34;
    g_v128f[9] = v128f_27;
    g_v128d[7] = v128d_15;
    g_v128i[10] = v128i_10;
    return v512d_5;
}

__m512h torture_308(__m256i p0, __m256 p1, __m256h p2, __m128h p3, __m256 p4) {
    __mmask64 m64_1 = (__mmask64)(g_m64[4] ^ 40);
    __mmask64 m64_2 = _kor_mask64(m64_1, m64_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[1] ^ 26);
    __mmask16 m16_4 = _kshiftri_mask16(m16_3, 1);
    __m256 v256f_5 = _mm256_max_ps(p1, p4);
    __m512 v512f_6 = g_v512f[3];
    __m512 v512f_7 = _mm512_maskz_mul_ps(m16_4, v512f_6, v512f_6);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_9 = _mm512_movepi32_mask(v512i_8);
    __mmask8 m8_10 = (__mmask8)(g_m8[10] ^ 142);
    __m512i v512i_11 = _mm512_broadcastmb_epi64(m8_10);
    __m256i v256i_12 = _mm256_slli_epi32(p0, 3);
    __mmask64 m64_13 = _kor_mask64(m64_1, m64_1);
    __m128d v128d_14 = g_v128d[11];
    __m128d v128d_15 = _mm_max_pd(v128d_14, v128d_14);
    __m512i v512i_16 = _mm512_add_epi16(v512i_8, v512i_11);
    __mmask16 m16_17 = _mm512_cmp_ps_mask(v512f_6, v512f_7, _CMP_LT_OQ);
    __m512 v512f_18 = _mm512_insertf32x8(v512f_7, p4, 1);
    __m128i v128i_19 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_20 = _mm_abs_epi32(v128i_19);
    __m256d v256d_21 = g_v256d[10];
    __m256d v256d_22 = _mm256_sub_pd(v256d_21, v256d_21);
    __m512i v512i_23 = _mm512_cvtepi8_epi32(v128i_19);
    __m512i v512i_24 = _mm512_popcnt_epi64(v512i_8);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_9) & 7); i_25++) {
    __m128i v128i_26 = _mm_or_si128(v128i_20, v128i_19);
    __m512i v512i_27 = _mm512_abs_epi32(v512i_24);
    __m512h v512h_28 = g_v512h[6];
    __m512h v512h_29 = _mm512_getexp_ph(v512h_28);
    __m512i v512i_30 = _mm512_sub_epi64(v512i_23, v512i_16);
    }
    __m256h v256h_31 = _mm512_cvtxps_ph(v512f_6);
    __m128 v128f_32 = g_v128f[2];
    __m128 v128f_33 = _mm_sub_ps(v128f_32, v128f_32);
    __m512i v512i_34 = _mm512_rol_epi32(v512i_23, 5);
    __m128i v128i_35 = _mm_sub_epi8(v128i_20, v128i_20);
    __m128i v128i_36 = _mm_add_epi64(v128i_35, v128i_20);
    __m512 v512f_37 = _mm512_set1_ps(1.0f);
    __m256 v256f_38 = _mm256_fmadd_ps(p1, p4, p4);
    __m512d v512d_39 = _mm512_loadu_pd(dbuf);
    __m128h v128h_40 = _mm512_cvtpd_ph(v512d_39);
    __m256i v256i_41 = _mm256_xor_si256(v256i_12, p0);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m128i v128i_42 = _mm_xor_si128(v128i_35, v128i_19);
    __m128h v128h_43 = _mm512_cvtpd_ph(v512d_39);
    __mmask16 m16_44 = _kshiftli_mask16(m16_3, 2);
    }
    g_v512f[9] = v512f_18;
    g_v512d[7] = v512d_39;
    g_v512i[10] = v512i_8;
    g_v256f[0] = p1;
    g_v256d[11] = v256d_21;
    g_v128f[3] = v128f_32;
    g_m8[6] = m8_10;
    g_m16[8] = m16_3;
    __m512h v512h_45 = g_v512h[7];
    return v512h_45;
}

__m512d torture_309(__m512h p0, __m256 p1, __m128d p2) {
    __m128d v128d_1 = _mm_div_pd(p2, p2);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m256h v256h_3 = _mm512_cvtxps_ph(v512f_2);
    __m128d v128d_4 = _mm_add_pd(p2, p2);
    __m256 v256f_5 = _mm256_add_ps(p1, p1);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_lzcnt_epi32(v512i_6);
    __mmask64 m64_8 = _mm512_cmp_epi8_mask(v512i_7, v512i_6, 1);
    __m512i v512i_9 = _mm512_popcnt_epi64(v512i_7);
    __m512d v512d_10 = _mm512_loadu_pd(dbuf);
    __m512d v512d_11 = _mm512_fmadd_pd(v512d_10, v512d_10, v512d_10);
    __m128 v128f_12 = g_v128f[6];
    __m128 v128f_13 = _mm_add_ps(v128f_12, v128f_12);
    __m128d v128d_14 = _mm_unpackhi_pd(p2, v128d_4);
    __m512i v512i_15 = _mm512_xor_si512(v512i_7, v512i_9);
    __m256i v256i_16 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_17 = _mm256_sub_epi8(v256i_16, v256i_16);
    if (((unsigned long long)m64_8) & 1ULL) {
    __m512 v512f_18 = _mm512_mul_ps(v512f_2, v512f_2);
    __m128 v128f_19 = _mm_sub_ps(v128f_12, v128f_13);
    }
    __m512d v512d_20 = _mm512_min_pd(v512d_10, v512d_10);
    __m128i v128i_21 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_22 = _mm512_broadcast_i32x4(v128i_21);
    __m512 v512f_23 = _mm512_getexp_ps(v512f_2);
    __m512d v512d_24 = _mm512_max_pd(v512d_11, v512d_20);
    __m512i v512i_25 = _mm512_add_epi64(v512i_15, v512i_6);
    __mmask16 m16_26 = (__mmask16)(g_m16[0] ^ 171);
    __m512 v512f_27 = _mm512_mask3_fmadd_ps(v512f_2, v512f_23, v512f_23, m16_26);
    __m512 v512f_28 = _mm512_cvtepi32_ps(v512i_25);
    __m512 v512f_29 = _mm512_and_ps(v512f_28, v512f_28);
    __m256i v256i_30 = _mm256_add_epi32(v256i_16, v256i_16);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_26) & 7); i_31++) {
    __m512h v512h_32 = _mm512_fmsub_ph(p0, p0, p0);
    __mmask16 m16_33 = _mm512_testn_epi32_mask(v512i_15, v512i_6);
    __m512 v512f_34 = _mm512_and_ps(v512f_23, v512f_27);
    __m512 v512f_35 = _mm512_xor_ps(v512f_23, v512f_27);
    __m256i v256i_36 = _mm256_xor_si256(v256i_17, v256i_30);
    }
    g_v512f[9] = v512f_27;
    g_v256i[9] = v256i_16;
    g_v128d[4] = p2;
    g_v128i[0] = v128i_21;
    g_v256h[8] = v256h_3;
    g_m16[4] = m16_26;
    return v512d_20;
}

__m128 torture_310(__m512i p0, __m128i p1, __m512d p2, __m256d p3) {
    __m256d v256d_1 = _mm256_mul_pd(p3, p3);
    __m512i v512i_2 = _mm512_add_epi64(p0, p0);
    __m512d v512d_3 = _mm512_max_pd(p2, p2);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_sub_ps(v512f_4, v512f_4);
    __mmask16 m16_6 = (__mmask16)(g_m16[0] ^ 178);
    __m512i v512i_7 = _mm512_broadcastmw_epi32(m16_6);
    __m512 v512f_8 = _mm512_fmsub_ps(v512f_4, v512f_5, v512f_5);
    __m512d v512d_9 = _mm512_div_pd(v512d_3, v512d_3);
    __m256 v256f_10 = _mm256_loadu_ps(fbuf);
    __m256 v256f_11 = _mm256_add_ps(v256f_10, v256f_10);
    __m128d v128d_12 = _mm_set1_pd(2.0);
    __m512i v512i_13 = _mm512_slli_epi32(v512i_7, 3);
    __mmask8 m8_14 = (__mmask8)(g_m8[1] ^ 60);
    __m512i v512i_15 = _mm512_broadcastmb_epi64(m8_14);
    __m128i v128i_16 = _mm_and_si128(p1, p1);
    __m128i v128i_17 = _mm_sub_epi16(v128i_16, p1);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_6) & 7); i_18++) {
    __m128d v128d_19 = _mm_set1_pd(2.0);
    __m512 v512f_20 = _mm512_unpacklo_ps(v512f_4, v512f_5);
    __m512d v512d_21 = _mm512_unpackhi_pd(v512d_9, v512d_9);
    }
    __mmask32 m32_22 = (__mmask32)(g_m32[9] ^ 231);
    __mmask32 m32_23 = _kxor_mask32(m32_22, m32_22);
    __m512h v512h_24 = g_v512h[0];
    __m512h v512h_25 = _mm512_rcp_ph(v512h_24);
    __m512i v512i_26 = _mm512_broadcast_i32x4(v128i_16);
    __m512i v512i_27 = _mm512_cvtph_epi16(v512h_25);
    __mmask32 m32_28 = _kxor_mask32(m32_23, m32_22);
    __m256i v256i_29 = g_v256i[1];
    __m256i v256i_30 = _mm256_sub_epi16(v256i_29, v256i_29);
    __m512d v512d_31 = _mm512_mask_blend_pd(m8_14, v512d_3, v512d_3);
    __m512d v512d_32 = _mm512_mul_pd(v512d_3, p2);
    __m256 v256f_33 = _mm256_shuffle_ps(v256f_10, v256f_11, 0x1B);
    __m128i v128i_34 = _mm_add_epi8(v128i_17, p1);
    __m256d v256d_35 = _mm256_div_pd(v256d_1, v256d_1);
    __m512i v512i_36 = _mm512_set1_epi32(0x55);
    g_v512f[6] = v512f_5;
    g_v256d[7] = p3;
    g_v256i[1] = v256i_30;
    __m128 v128f_37 = g_v128f[11];
    return v128f_37;
}

__m256h torture_311(__m128d p0, __m512i p1, __m512 p2, __m512i p3) {
    __m512 v512f_1 = _mm512_mul_ps(p2, p2);
    __m128d v128d_2 = _mm_unpackhi_pd(p0, p0);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_fmadd_ps(v256f_3, v256f_3, v256f_3);
    __m256i v256i_5 = g_v256i[4];
    __m256i v256i_6 = _mm256_abs_epi32(v256i_5);
    __m512h v512h_7 = g_v512h[6];
    __m512h v512h_8 = _mm512_getexp_ph(v512h_7);
    __m128d v128d_9 = _mm_max_pd(v128d_2, v128d_2);
    __m256 v256f_10 = _mm256_and_ps(v256f_4, v256f_3);
    __m256i v256i_11 = _mm256_sub_epi16(v256i_6, v256i_5);
    __mmask16 m16_12 = (__mmask16)(g_m16[8] ^ 137);
    __mmask16 m16_13 = _kshiftri_mask16(m16_12, 1);
    __m512i v512i_14 = _mm512_add_epi8(p1, p1);
    __m512i v512i_15 = _mm512_madd52lo_epu64(p3, p3, v512i_14);
    if (((unsigned long long)m16_13) & 1ULL) {
    __mmask8 m8_16 = (__mmask8)(g_m8[10] ^ 3);
    __m512i v512i_17 = _mm512_broadcastmb_epi64(m8_16);
    __m128i v128i_18 = g_v128i[4];
    __m128i v128i_19 = _mm_add_epi16(v128i_18, v128i_18);
    __mmask64 m64_20 = _mm512_movepi8_mask(v512i_14);
    }
    __m128i v128i_21 = g_v128i[0];
    __m128i v128i_22 = _mm_sub_epi16(v128i_21, v128i_21);
    __m128d v128d_23 = _mm_min_pd(v128d_9, p0);
    __m512i v512i_24 = _mm512_xor_si512(p1, v512i_14);
    __m512d v512d_25 = _mm512_loadu_pd(dbuf);
    __m512d v512d_26 = _mm512_min_pd(v512d_25, v512d_25);
    __m128i v128i_27 = _mm_sub_epi16(v128i_21, v128i_22);
    __m128i v128i_28 = _mm_mullo_epi32(v128i_27, v128i_27);
    __m512 v512f_29 = _mm512_mask_add_ps(v512f_1, m16_12, v512f_1, p2);
    __m512 v512f_30 = _mm512_mask_add_ps(v512f_29, m16_13, v512f_29, p2);
    __m128 v128f_31 = _mm_loadu_ps(fbuf);
    __m512 v512f_32 = _mm512_broadcast_f32x4(v128f_31);
    __m256d v256d_33 = g_v256d[9];
    __m256d v256d_34 = _mm256_sub_pd(v256d_33, v256d_33);
    __m128i v128i_35 = _mm_mullo_epi32(v128i_21, v128i_27);
    __m128 v128f_36 = _mm_shuffle_ps(v128f_31, v128f_31, 0x1B);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m128 v128f_37 = _mm_fmadd_ps(v128f_36, v128f_31, v128f_31);
    __m256 v256f_38 = _mm256_shuffle_ps(v256f_10, v256f_3, 0x1B);
    __m128d v128d_39 = _mm_min_pd(v128d_2, v128d_23);
    }
    g_v512f[4] = v512f_1;
    g_v512d[5] = v512d_26;
    g_v256d[7] = v256d_34;
    g_v256i[5] = v256i_5;
    __m256h v256h_40 = g_v256h[11];
    return v256h_40;
}

__m128 torture_312(__m128h p0, __m256i p1, __m512h p2) {
    __m512h v512h_1 = _mm512_add_ph(p2, p2);
    __m128i v128i_2 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_3 = _mm_sllv_epi32(v128i_2, v128i_2);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_max_ps(v512f_4, v512f_4);
    __m512d v512d_6 = _mm512_loadu_pd(dbuf);
    __m512i v512i_7 = _mm512_cvtpd_epi64(v512d_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_and_ps(v256f_8, v256f_8);
    __mmask16 m16_10 = _mm512_testn_epi32_mask(v512i_7, v512i_7);
    __m512d v512d_11 = _mm512_add_pd(v512d_6, v512d_6);
    __mmask16 m16_12 = _kor_mask16(m16_10, m16_10);
    __m128d v128d_13 = g_v128d[5];
    __m128d v128d_14 = _mm_max_pd(v128d_13, v128d_13);
    __m512d v512d_15 = _mm512_max_pd(v512d_6, v512d_6);
    __m128i v128i_16 = _mm_sub_epi8(v128i_3, v128i_2);
    __m128 v128f_17 = g_v128f[1];
    __m128 v128f_18 = _mm_fmadd_ps(v128f_17, v128f_17, v128f_17);
    __m256 v256f_19 = _mm256_set1_ps(1.0f);
    __m128d v128d_20 = _mm_sqrt_pd(v128d_13);
    __m512 v512f_21 = _mm512_add_ps(v512f_4, v512f_4);
    __m512 v512f_22 = _mm512_broadcast_f32x4(v128f_17);
    __m128i v128i_23 = _mm_set1_epi32(0x55);
    __m128i v128i_24 = _mm_or_si128(v128i_16, v128i_23);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m128 v128f_25 = _mm_div_ps(v128f_18, v128f_18);
    __m128d v128d_26 = _mm_fmadd_pd(v128d_20, v128d_13, v128d_14);
    __m256i v256i_27 = _mm256_sub_epi64(p1, p1);
    __m512h v512h_28 = _mm512_cvtepi16_ph(v512i_7);
    __m512 v512f_29 = _mm512_add_ps(v512f_4, v512f_22);
    }
    __mmask16 m16_30 = _kor_mask16(m16_10, m16_12);
    __mmask16 m16_31 = _mm512_cmp_ps_mask(v512f_21, v512f_21, _CMP_LT_OQ);
    __m512d v512d_32 = _mm512_cvtph_pd(p0);
    __mmask16 m16_33 = _kshiftri_mask16(m16_30, 1);
    __mmask16 m16_34 = _kand_mask16(m16_33, m16_33);
    g_v512f[3] = v512f_4;
    g_v512d[6] = v512d_11;
    g_v512i[3] = v512i_7;
    g_v256i[9] = p1;
    g_v128d[9] = v128d_13;
    g_v128h[0] = p0;
    return v128f_18;
}

__mmask64 torture_313(__m256 p0, __m512h p1, __m256h p2, __m256i p3, __m128 p4) {
    __mmask16 m16_1 = (__mmask16)(g_m16[6] ^ 98);
    __mmask16 m16_2 = _kshiftli_mask16(m16_1, 2);
    __mmask8 m8_3 = (__mmask8)(g_m8[11] ^ 84);
    __mmask16 m16_4 = _mm512_kunpackb(m8_3, m8_3);
    __m128 v128f_5 = _mm_shuffle_ps(p4, p4, 0x1B);
    __m128 v128f_6 = _mm_and_ps(p4, v128f_5);
    __m512h v512h_7 = _mm512_getexp_ph(p1);
    __m512i v512i_8 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_9 = _mm512_and_si512(v512i_8, v512i_8);
    __m128 v128f_10 = _mm_xor_ps(p4, v128f_5);
    unsigned u32_11 = _cvtmask16_u32(m16_2);
    __m512i v512i_12 = _mm512_add_epi64(v512i_9, v512i_9);
    __m512i v512i_13 = _mm512_shuffle_epi8(v512i_8, v512i_8);
    __m512i v512i_14 = _mm512_mullo_epi32(v512i_12, v512i_13);
    __m128 v128f_15 = _mm_min_ps(v128f_10, v128f_5);
    __m256d v256d_16 = g_v256d[6];
    __m256d v256d_17 = _mm256_min_pd(v256d_16, v256d_16);
    __m512h v512h_18 = _mm512_rcp_ph(p1);
    __m512 v512f_19 = _mm512_cvtepi32_ps(v512i_14);
    __m128i v128i_20 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_21 = _mm512_cvtepi8_epi32(v128i_20);
    __m256d v256d_22 = _mm256_mul_pd(v256d_17, v256d_17);
    __m256i v256i_23 = _mm256_sub_epi32(p3, p3);
    __m256 v256f_24 = _mm256_xor_ps(p0, p0);
    __m256 v256f_25 = _mm256_fmadd_ps(p0, v256f_24, v256f_24);
    __m256i v256i_26 = _mm256_and_si256(p3, p3);
    __m256i v256i_27 = _mm256_add_epi64(v256i_26, p3);
    __m512 v512f_28 = _mm512_rsqrt14_ps(v512f_19);
    __m512d v512d_29 = _mm512_loadu_pd(dbuf);
    __m256 v256f_30 = _mm512_cvtpd_ps(v512d_29);
    __m512 v512f_31 = _mm512_sqrt_ps(v512f_28);
    __m128d v128d_32 = g_v128d[1];
    __m512d v512d_33 = _mm512_insertf64x2(v512d_29, v128d_32, 3);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_2) & 7); i_34++) {
    __m512h v512h_35 = _mm512_add_ph(p1, v512h_18);
    __m128 v128f_36 = _mm_fmsub_ps(v128f_10, v128f_10, v128f_10);
    }
    g_v512d[2] = v512d_29;
    g_v512i[7] = v512i_9;
    g_v128f[8] = v128f_6;
    g_v128d[10] = v128d_32;
    g_v512h[4] = v512h_18;
    g_m8[2] = m8_3;
    __mmask64 m64_37 = (__mmask64)(g_m64[2] ^ 68);
    return m64_37;
}

__m512i torture_314(__m256d p0, __m256d p1, __m256 p2) {
    __m256i v256i_1 = g_v256i[2];
    __m256i v256i_2 = _mm256_sub_epi32(v256i_1, v256i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_max_ps(v512f_3, v512f_3);
    __m256i v256i_5 = _mm256_sub_epi64(v256i_2, v256i_2);
    __m512i v512i_6 = g_v512i[3];
    __m512i v512i_7 = _mm512_add_epi8(v512i_6, v512i_6);
    __m512d v512d_8 = _mm512_loadu_pd(dbuf);
    __m512d v512d_9 = _mm512_sqrt_pd(v512d_8);
    __m512 v512f_10 = _mm512_scalef_ps(v512f_4, v512f_4);
    __m512i v512i_11 = _mm512_add_epi32(v512i_7, v512i_7);
    __m512i v512i_12 = _mm512_shuffle_epi8(v512i_7, v512i_7);
    __m128d v128d_13 = g_v128d[7];
    __m128d v128d_14 = _mm_max_pd(v128d_13, v128d_13);
    __m256d v256d_15 = _mm256_min_pd(p1, p1);
    __m256i v256i_16 = _mm256_xor_si256(v256i_5, v256i_1);
    __mmask16 m16_17 = (__mmask16)(g_m16[5] ^ 207);
    __m512i v512i_18 = _mm512_mask_blend_epi32(m16_17, v512i_11, v512i_6);
    __m128i v128i_19 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_20 = _mm512_broadcast_i32x4(v128i_19);
    __m256i v256i_21 = _mm256_sub_epi16(v256i_1, v256i_2);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_17) & 7); i_22++) {
    __m128i v128i_23 = _mm_or_si128(v128i_19, v128i_19);
    __m128d v128d_24 = _mm_unpackhi_pd(v128d_14, v128d_14);
    __m512 v512f_25 = _mm512_max_ps(v512f_4, v512f_3);
    __m128i v128i_26 = _mm_sub_epi64(v128i_19, v128i_19);
    __m256d v256d_27 = _mm256_sqrt_pd(p1);
    }
    __mmask16 m16_28 = _mm512_cmp_ps_mask(v512f_3, v512f_3, _CMP_LT_OQ);
    __m256 v256f_29 = _mm256_div_ps(p2, p2);
    __m256h v256h_30 = _mm512_cvtxps_ph(v512f_10);
    __m128d v128d_31 = _mm_max_pd(v128d_13, v128d_13);
    if (((unsigned long long)m16_17) & 1ULL) {
    __m128i v128i_32 = _mm_abs_epi32(v128i_19);
    __mmask64 m64_33 = (__mmask64)(g_m64[7] ^ 73);
    __mmask64 m64_34 = _kor_mask64(m64_33, m64_33);
    __m512 v512f_35 = _mm512_sqrt_ps(v512f_4);
    __m256d v256d_36 = _mm256_sqrt_pd(v256d_15);
    }
    g_v256f[0] = v256f_29;
    return v512i_7;
}

__m256 torture_315(__m256d p0, __m128i p1, __m512 p2, __m128d p3, __m128i p4) {
    __m256i v256i_1 = g_v256i[3];
    __m256i v256i_2 = _mm256_sllv_epi32(v256i_1, v256i_1);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_fnmadd_ps(v256f_3, v256f_3, v256f_3);
    __m512h v512h_5 = g_v512h[5];
    __m512h v512h_6 = _mm512_rcp_ph(v512h_5);
    __m128 v128f_7 = g_v128f[9];
    __m128 v128f_8 = _mm_fmadd_ps(v128f_7, v128f_7, v128f_7);
    __m128d v128d_9 = _mm_sub_pd(p3, p3);
    __m512 v512f_10 = _mm512_mul_ps(p2, p2);
    __m256d v256d_11 = _mm256_sqrt_pd(p0);
    __mmask16 m16_12 = (__mmask16)(g_m16[2] ^ 90);
    __mmask16 m16_13 = _kshiftli_mask16(m16_12, 2);
    __m512i v512i_14 = g_v512i[10];
    __m512i v512i_15 = _mm512_sub_epi32(v512i_14, v512i_14);
    if (((unsigned long long)m16_13) & 1ULL) {
    unsigned u32_16 = 62;
    __mmask16 m16_17 = _cvtu32_mask16(u32_16);
    __m512d v512d_18 = _mm512_loadu_pd(dbuf);
    __m512d v512d_19 = _mm512_sub_pd(v512d_18, v512d_18);
    __m128d v128d_20 = _mm_mul_pd(p3, v128d_9);
    __m128 v128f_21 = _mm_fnmadd_ps(v128f_7, v128f_7, v128f_8);
    }
    __m128 v128f_22 = _mm_xor_ps(v128f_7, v128f_8);
    __m256 v256f_23 = _mm256_max_ps(v256f_3, v256f_4);
    __m128d v128d_24 = _mm_div_pd(v128d_9, p3);
    __m256d v256d_25 = _mm256_set1_pd(2.0);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_13) & 7); i_26++) {
    __m128i v128i_27 = _mm_unpacklo_epi32(p1, p1);
    __m256 v256f_28 = _mm256_and_ps(v256f_3, v256f_3);
    }
    g_v512f[1] = p2;
    g_v128i[9] = p1;
    g_m16[9] = m16_12;
    return v256f_3;
}

__m512d torture_316(__m512h p0, __m256d p1) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_abs_epi32(v256i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_sub_epi16(v512i_3, v512i_3);
    __m512h v512h_5 = _mm512_add_ph(p0, p0);
    __m128i v128i_6 = g_v128i[11];
    __m512i v512i_7 = _mm512_cvtepi8_epi32(v128i_6);
    __m256d v256d_8 = _mm256_sqrt_pd(p1);
    __m128i v128i_9 = _mm_or_si128(v128i_6, v128i_6);
    __m512 v512f_10 = _mm512_loadu_ps(fbuf);
    __m512 v512f_11 = _mm512_shuffle_ps(v512f_10, v512f_10, 0x1B);
    __m128d v128d_12 = g_v128d[4];
    __m128d v128d_13 = _mm_unpackhi_pd(v128d_12, v128d_12);
    __m256 v256f_14 = _mm256_set1_ps(1.0f);
    __m128d v128d_15 = _mm_sub_pd(v128d_13, v128d_13);
    __m512i v512i_16 = _mm512_cvtps_epi32(v512f_11);
    __m512i v512i_17 = _mm512_unpacklo_epi32(v512i_4, v512i_4);
    __m512 v512f_18 = _mm512_rsqrt14_ps(v512f_11);
    __m256d v256d_19 = _mm256_fmadd_pd(p1, v256d_8, v256d_8);
    int s32_20 = 64;
    for (int i_21 = 0; i_21 < (int)((s32_20) & 7); i_21++) {
    __m256 v256f_22 = _mm256_fmadd_ps(v256f_14, v256f_14, v256f_14);
    __m128i v128i_23 = _mm_add_epi8(v128i_9, v128i_6);
    __m512 v512f_24 = _mm512_shuffle_ps(v512f_11, v512f_10, 0x1B);
    __m512 v512f_25 = _mm512_xor_ps(v512f_24, v512f_18);
    }
    __m128i v128i_26 = _mm_abs_epi32(v128i_6);
    __m128 v128f_27 = g_v128f[1];
    __m128 v128f_28 = _mm_fmsub_ps(v128f_27, v128f_27, v128f_27);
    __m128i v128i_29 = _mm_add_epi8(v128i_26, v128i_6);
    __m512 v512f_30 = _mm512_cvtepi32_ps(v512i_3);
    __m128 v128f_31 = _mm_and_ps(v128f_27, v128f_27);
    __m128i v128i_32 = _mm_and_si128(v128i_26, v128i_9);
    __m512d v512d_33 = _mm512_set1_pd(2.0);
    __m512 v512f_34 = _mm512_xor_ps(v512f_18, v512f_11);
    __mmask16 m16_35 = (__mmask16)(g_m16[2] ^ 29);
    __m512 v512f_36 = _mm512_mask3_fmadd_ps(v512f_34, v512f_10, v512f_10, m16_35);
    __m512 v512f_37 = _mm512_fnmadd_ps(v512f_36, v512f_10, v512f_36);
    __m256i v256i_38 = _mm512_cvtepi64_epi32(v512i_4);
    __m128i v128i_39 = _mm_add_epi64(v128i_9, v128i_32);
    g_v512f[4] = v512f_30;
    g_v512d[1] = v512d_33;
    g_v256d[3] = v256d_19;
    return v512d_33;
}

__m256h torture_317(__m256 p0, __m256i p1, __m256i p2, __m128h p3) {
    __m512h v512h_1 = g_v512h[11];
    __m512h v512h_2 = _mm512_fmadd_ph(v512h_1, v512h_1, v512h_1);
    __m512 v512f_3 = g_v512f[6];
    __m512 v512f_4 = _mm512_scalef_ps(v512f_3, v512f_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[4] ^ 193);
    __mmask16 m16_6 = _kshiftri_mask16(m16_5, 1);
    __m128 v128f_7 = _mm_loadu_ps(fbuf);
    __m128 v128f_8 = _mm_fnmadd_ps(v128f_7, v128f_7, v128f_7);
    __m512i v512i_9 = g_v512i[7];
    __m512i v512i_10 = _mm512_add_epi8(v512i_9, v512i_9);
    __m512i v512i_11 = _mm512_ternarylogic_epi32(v512i_9, v512i_9, v512i_9, 0x96);
    __m256i v256i_12 = _mm256_abs_epi32(p2);
    __m512i v512i_13 = _mm512_rolv_epi32(v512i_10, v512i_9);
    __m256 v256f_14 = _mm256_min_ps(p0, p0);
    __m256d v256d_15 = g_v256d[0];
    __m256d v256d_16 = _mm256_sqrt_pd(v256d_15);
    __m256i v256i_17 = _mm256_or_si256(p2, p1);
    __m512d v512d_18 = g_v512d[1];
    __m128h v128h_19 = _mm512_cvtpd_ph(v512d_18);
    __m128i v128i_20 = g_v128i[6];
    __m128i v128i_21 = _mm_add_epi32(v128i_20, v128i_20);
    __m256d v256d_22 = _mm256_max_pd(v256d_16, v256d_15);
    __m256i v256i_23 = _mm256_abs_epi32(v256i_17);
    __m128d v128d_24 = g_v128d[9];
    __m128d v128d_25 = _mm_mul_pd(v128d_24, v128d_24);
    __m512 v512f_26 = _mm512_getexp_ps(v512f_3);
    __m128i v128i_27 = _mm_or_si128(v128i_20, v128i_21);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m512i v512i_28 = _mm512_rol_epi32(v512i_10, 5);
    __m128 v128f_29 = _mm_fmadd_ps(v128f_7, v128f_7, v128f_7);
    }
    __mmask16 m16_30 = _mm512_cmp_epu32_mask(v512i_11, v512i_9, 4);
    __m512d v512d_31 = _mm512_div_pd(v512d_18, v512d_18);
    __m256 v256f_32 = _mm256_div_ps(v256f_14, p0);
    __m256d v256d_33 = _mm256_fmadd_pd(v256d_16, v256d_22, v256d_15);
    __m128 v128f_34 = _mm_set1_ps(1.0f);
    __m512i v512i_35 = _mm512_dpwssd_epi32(v512i_11, v512i_10, v512i_11);
    __m256i v256i_36 = _mm256_add_epi8(p1, v256i_17);
    __m512i v512i_37 = _mm512_rol_epi32(v512i_10, 5);
    __mmask8 m8_38 = (__mmask8)(g_m8[7] ^ 135);
    __mmask8 m8_39 = _kand_mask8(m8_38, m8_38);
    __m256i v256i_40 = _mm256_add_epi8(v256i_17, p1);
    __m256 v256f_41 = _mm256_sqrt_ps(p0);
    __m256i v256i_42 = _mm256_sub_epi64(p1, v256i_12);
    g_v512f[3] = v512f_4;
    g_v256f[5] = v256f_41;
    g_v128f[1] = v128f_34;
    __m256h v256h_43 = g_v256h[7];
    return v256h_43;
}

__mmask16 torture_318(__m128i p0, __m512i p1, __m256i p2, __m512 p3, __m128d p4) {
    __m256i v256i_1 = _mm256_unpacklo_epi32(p2, p2);
    __m128 v128f_2 = g_v128f[11];
    __m128 v128f_3 = _mm_min_ps(v128f_2, v128f_2);
    __m512i v512i_4 = _mm512_rol_epi32(p1, 5);
    __m128 v128f_5 = _mm_max_ps(v128f_3, v128f_3);
    __m512h v512h_6 = g_v512h[8];
    __m512h v512h_7 = _mm512_sqrt_ph(v512h_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_sub_ps(v256f_8, v256f_8);
    __m128 v128f_10 = _mm_min_ps(v128f_5, v128f_5);
    __mmask64 m64_11 = _mm512_movepi8_mask(p1);
    __mmask16 m16_12 = (__mmask16)(g_m16[11] ^ 204);
    __m512i v512i_13 = _mm512_mask_add_epi32(p1, m16_12, p1, p1);
    __m128 v128f_14 = _mm_add_ps(v128f_2, v128f_10);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_12) & 7); i_15++) {
    __m512 v512f_16 = _mm512_and_ps(p3, p3);
    __m512i v512i_17 = _mm512_shuffle_epi8(v512i_13, v512i_4);
    __m128d v128d_18 = _mm_min_pd(p4, p4);
    unsigned u32_19 = _cvtmask16_u32(m16_12);
    }
    __m512i v512i_20 = _mm512_mask_add_epi32(p1, m16_12, v512i_4, p1);
    __m128i v128i_21 = _mm512_cvtepi32_epi8(v512i_13);
    __m256i v256i_22 = _mm256_sllv_epi32(v256i_1, v256i_1);
    __mmask16 m16_23 = _mm512_cmp_epu32_mask(v512i_20, v512i_20, 4);
    __m512h v512h_24 = _mm512_rcp_ph(v512h_6);
    __m512h v512h_25 = _mm512_mul_ph(v512h_7, v512h_24);
    __m512h v512h_26 = _mm512_mul_ph(v512h_6, v512h_7);
    g_v128i[7] = p0;
    g_m16[3] = m16_23;
    return m16_23;
}

__m256i torture_319(__m128 p0, __m512i p1, __m256 p2, __m256d p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[8] ^ 135);
    __m512 v512f_2 = _mm512_loadu_ps(fbuf);
    __m512 v512f_3 = _mm512_maskz_mul_ps(m16_1, v512f_2, v512f_2);
    __m256i v256i_4 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_5 = _mm256_add_epi32(v256i_4, v256i_4);
    __m256i v256i_6 = _mm256_unpacklo_epi32(v256i_4, v256i_5);
    __m256h v256h_7 = g_v256h[6];
    __m512 v512f_8 = _mm512_cvtxph_ps(v256h_7);
    __m256d v256d_9 = _mm256_fmadd_pd(p3, p3, p3);
    __m512h v512h_10 = g_v512h[11];
    __mmask32 m32_11 = _mm512_cmp_ph_mask(v512h_10, v512h_10, 1);
    __mmask16 m16_12 = _kand_mask16(m16_1, m16_1);
    __m256 v256f_13 = _mm256_xor_ps(p2, p2);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m128i v128i_14 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_15 = _mm_shuffle_epi8(v128i_14, v128i_14);
    __m512 v512f_16 = _mm512_and_ps(v512f_8, v512f_2);
    __m128 v128f_17 = _mm_and_ps(p0, p0);
    __m512 v512f_18 = _mm512_unpacklo_ps(v512f_8, v512f_3);
    }
    __m512i v512i_19 = _mm512_abs_epi32(p1);
    __mmask8 m8_20 = (__mmask8)(g_m8[5] ^ 30);
    __mmask8 m8_21 = _kand_mask8(m8_20, m8_20);
    __m128d v128d_22 = g_v128d[5];
    __m128d v128d_23 = _mm_add_pd(v128d_22, v128d_22);
    __m128i v128i_24 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_25 = _mm_shuffle_epi8(v128i_24, v128i_24);
    __m128i v128i_26 = _mm_add_epi32(v128i_25, v128i_25);
    __m512 v512f_27 = _mm512_fmsub_ps(v512f_3, v512f_3, v512f_3);
    __m512d v512d_28 = g_v512d[3];
    __m512d v512d_29 = _mm512_unpackhi_pd(v512d_28, v512d_28);
    __m256 v256f_30 = _mm256_unpacklo_ps(p2, v256f_13);
    __m512h v512h_31 = _mm512_sqrt_ph(v512h_10);
    __m512i v512i_32 = _mm512_sub_epi64(v512i_19, v512i_19);
    __m128 v128f_33 = _mm_fmsub_ps(p0, p0, p0);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_1) & 7); i_34++) {
    __m512 v512f_35 = _mm512_getexp_ps(v512f_27);
    __m128d v128d_36 = _mm_max_pd(v128d_22, v128d_22);
    }
    g_v512d[10] = v512d_28;
    g_v512i[8] = p1;
    g_v512h[11] = v512h_10;
    g_m8[0] = m8_20;
    g_m16[9] = m16_12;
    return v256i_5;
}

__m512i torture_320(__m128 p0, __m512h p1, __m256i p2, __m512 p3, __m512 p4) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m256 v256f_2 = _mm256_mul_ps(v256f_1, v256f_1);
    __m256d v256d_3 = g_v256d[2];
    __m256d v256d_4 = _mm256_min_pd(v256d_3, v256d_3);
    __m128i v128i_5 = g_v128i[5];
    __m128i v128i_6 = _mm_sub_epi64(v128i_5, v128i_5);
    __m256 v256f_7 = _mm256_add_ps(v256f_2, v256f_2);
    __m256 v256f_8 = _mm256_fmsub_ps(v256f_1, v256f_7, v256f_1);
    __mmask32 m32_9 = (__mmask32)(g_m32[6] ^ 37);
    __mmask32 m32_10 = _kxor_mask32(m32_9, m32_9);
    __m256i v256i_11 = _mm256_sub_epi64(p2, p2);
    __m128d v128d_12 = g_v128d[9];
    __m128d v128d_13 = _mm_add_pd(v128d_12, v128d_12);
    __m128 v128f_14 = _mm_xor_ps(p0, p0);
    __m256 v256f_15 = _mm256_xor_ps(v256f_2, v256f_8);
    __m256 v256f_16 = _mm256_set1_ps(1.0f);
    if (((unsigned long long)m32_10) & 1ULL) {
    __m512i v512i_17 = _mm512_cvtph_epi16(p1);
    __m256i v256i_18 = _mm256_unpacklo_epi32(v256i_11, v256i_11);
    }
    __m256 v256f_19 = _mm256_add_ps(v256f_2, v256f_15);
    __m128d v128d_20 = _mm_add_pd(v128d_12, v128d_12);
    __m256i v256i_21 = _mm256_mullo_epi32(p2, p2);
    __m512h v512h_22 = _mm512_fmadd_ph(p1, p1, p1);
    __m256 v256f_23 = _mm256_shuffle_ps(v256f_2, v256f_2, 0x1B);
    __m512i v512i_24 = g_v512i[4];
    __m512i v512i_25 = _mm512_sub_epi32(v512i_24, v512i_24);
    __m512i v512i_26 = _mm512_add_epi32(v512i_25, v512i_24);
    __m512d v512d_27 = g_v512d[4];
    __mmask8 m8_28 = _mm512_cmp_pd_mask(v512d_27, v512d_27, _CMP_GE_OQ);
    g_v256f[1] = v256f_2;
    g_v256d[7] = v256d_4;
    g_v128f[1] = v128f_14;
    g_v128d[6] = v128d_13;
    g_m8[7] = m8_28;
    g_m32[9] = m32_10;
    return v512i_26;
}

__mmask16 torture_321(__m256d p0, __m256d p1, __m256i p2) {
    __m128d v128d_1 = g_v128d[11];
    __m128d v128d_2 = _mm_div_pd(v128d_1, v128d_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m256 v256f_4 = g_v256f[0];
    __m512 v512f_5 = _mm512_insertf32x8(v512f_3, v256f_4, 1);
    __m256 v256f_6 = _mm256_xor_ps(v256f_4, v256f_4);
    __m256 v256f_7 = _mm256_mul_ps(v256f_4, v256f_6);
    __m512i v512i_8 = g_v512i[3];
    __m512i v512i_9 = _mm512_rol_epi32(v512i_8, 5);
    __m512 v512f_10 = _mm512_rsqrt14_ps(v512f_3);
    __m256i v256i_11 = _mm256_sub_epi64(p2, p2);
    __m128i v128i_12 = g_v128i[10];
    __m128i v128i_13 = _mm_abs_epi32(v128i_12);
    __m512 v512f_14 = _mm512_insertf32x8(v512f_5, v256f_6, 1);
    __m512 v512f_15 = _mm512_fnmadd_ps(v512f_14, v512f_14, v512f_5);
    __mmask8 m8_16 = (__mmask8)(g_m8[8] ^ 84);
    __mmask8 m8_17 = _kand_mask8(m8_16, m8_16);
    __m512h v512h_18 = g_v512h[4];
    __m512h v512h_19 = _mm512_fmadd_ph(v512h_18, v512h_18, v512h_18);
    __m256d v256d_20 = _mm256_fmadd_pd(p1, p1, p0);
    __m512i v512i_21 = _mm512_sub_epi8(v512i_8, v512i_9);
    if (((unsigned long long)m8_17) & 1ULL) {
    __m256 v256f_22 = _mm256_unpacklo_ps(v256f_4, v256f_4);
    __mmask16 m16_23 = (__mmask16)(g_m16[0] ^ 115);
    __mmask16 m16_24 = _kor_mask16(m16_23, m16_23);
    __m128d v128d_25 = _mm_unpackhi_pd(v128d_2, v128d_2);
    unsigned u32_26 = _cvtmask16_u32(m16_24);
    __m256i v256i_27 = _mm256_sub_epi8(v256i_11, p2);
    }
    __m512 v512f_28 = _mm512_set1_ps(1.0f);
    __mmask16 m16_29 = (__mmask16)(g_m16[7] ^ 51);
    __m512 v512f_30 = _mm512_maskz_mul_ps(m16_29, v512f_28, v512f_3);
    __m256i v256i_31 = _mm256_add_epi16(v256i_11, p2);
    __m512 v512f_32 = _mm512_insertf32x8(v512f_28, v256f_7, 1);
    __m512d v512d_33 = g_v512d[1];
    __m512d v512d_34 = _mm512_fmadd_pd(v512d_33, v512d_33, v512d_33);
    __m128i v128i_35 = _mm_xor_si128(v128i_13, v128i_12);
    __m512i v512i_36 = _mm512_add_epi8(v512i_9, v512i_21);
    __m256d v256d_37 = _mm512_extractf64x4_pd(v512d_33, 1);
    __m128i v128i_38 = _mm_shuffle_epi8(v128i_13, v128i_12);
    if (((unsigned long long)m16_29) & 1ULL) {
    __m256h v256h_39 = g_v256h[7];
    __m512 v512f_40 = _mm512_cvtxph_ps(v256h_39);
    __m128 v128f_41 = g_v128f[0];
    __m128 v128f_42 = _mm_sqrt_ps(v128f_41);
    __m256 v256f_43 = _mm256_min_ps(v256f_7, v256f_7);
    __m128d v128d_44 = _mm_fmadd_pd(v128d_1, v128d_1, v128d_2);
    }
    g_v512f[1] = v512f_14;
    g_v512d[7] = v512d_33;
    g_v256f[5] = v256f_6;
    g_m8[7] = m8_17;
    return m16_29;
}

__m512h torture_322(__m128d p0, __m512 p1, __m256h p2, __m128h p3) {
    __m512i v512i_1 = g_v512i[0];
    __mmask16 m16_2 = _mm512_cmp_epu32_mask(v512i_1, v512i_1, 4);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_sqrt_ps(v128f_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_6 = _mm512_cvtepi8_epi32(v128i_5);
    __mmask8 m8_7 = (__mmask8)(g_m8[8] ^ 115);
    __m512i v512i_8 = _mm512_broadcastmb_epi64(m8_7);
    __m512d v512d_9 = g_v512d[0];
    __m512i v512i_10 = _mm512_cvtpd_epi64(v512d_9);
    __m512i v512i_11 = _mm512_sub_epi16(v512i_10, v512i_10);
    __m512i v512i_12 = _mm512_slli_epi32(v512i_6, 3);
    __mmask16 m16_13 = _mm512_movepi32_mask(v512i_10);
    __m512 v512f_14 = _mm512_set1_ps(1.0f);
    __m256i v256i_15 = g_v256i[7];
    __m256i v256i_16 = _mm256_xor_si256(v256i_15, v256i_15);
    __m512i v512i_17 = _mm512_popcnt_epi64(v512i_8);
    __m128 v128f_18 = _mm_add_ps(v128f_4, v128f_4);
    __m512d v512d_19 = _mm512_mul_pd(v512d_9, v512d_9);
    __mmask16 m16_20 = _kor_mask16(m16_13, m16_13);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_2) & 7); i_21++) {
    __mmask16 m16_22 = _mm512_fpclass_ps_mask(p1, 0x3);
    __m256d v256d_23 = g_v256d[7];
    __m256d v256d_24 = _mm256_sub_pd(v256d_23, v256d_23);
    __m256i v256i_25 = _mm256_sub_epi64(v256i_15, v256i_16);
    __m512 v512f_26 = _mm512_maskz_mul_ps(m16_22, p1, v512f_14);
    }
    __m512 v512f_27 = _mm512_div_ps(v512f_14, p1);
    __m512h v512h_28 = g_v512h[4];
    __m512h v512h_29 = _mm512_getexp_ph(v512h_28);
    __m512i v512i_30 = _mm512_sllv_epi32(v512i_6, v512i_10);
    __m512i v512i_31 = _mm512_mask_blend_epi32(m16_13, v512i_17, v512i_12);
    __m128d v128d_32 = _mm_sub_pd(p0, p0);
    __m512d v512d_33 = _mm512_mul_pd(v512d_19, v512d_9);
    for (int i_34 = 0; i_34 < (int)(((unsigned long long)m16_20) & 7); i_34++) {
    __m256 v256f_35 = _mm256_loadu_ps(fbuf);
    __m256 v256f_36 = _mm256_fmsub_ps(v256f_35, v256f_35, v256f_35);
    __m128d v128d_37 = _mm_set1_pd(2.0);
    __m256i v256i_38 = _mm256_add_epi8(v256i_16, v256i_15);
    __m256 v256f_39 = _mm256_shuffle_ps(v256f_35, v256f_36, 0x1B);
    }
    g_v512f[0] = v512f_14;
    g_v512i[7] = v512i_12;
    g_v128f[8] = v128f_18;
    g_v128d[7] = v128d_32;
    g_m16[6] = m16_2;
    return v512h_29;
}

__m512h torture_323(__m128d p0, __m128d p1, __m128d p2, __m512d p3, __m128i p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_mullo_epi32(v512i_1, v512i_1);
    __m128i v128i_3 = _mm_add_epi8(p4, p4);
    __m128h v128h_4 = _mm512_cvtpd_ph(p3);
    __m128 v128f_5 = _mm_loadu_ps(fbuf);
    __m128 v128f_6 = _mm_fmadd_ps(v128f_5, v128f_5, v128f_5);
    __m512d v512d_7 = _mm512_add_pd(p3, p3);
    __m512i v512i_8 = _mm512_shuffle_i32x4(v512i_2, v512i_2, 0x1B);
    __m128i v128i_9 = _mm_slli_epi32(p4, 3);
    __m512h v512h_10 = g_v512h[2];
    __m512h v512h_11 = _mm512_sqrt_ph(v512h_10);
    __m512d v512d_12 = _mm512_set1_pd(2.0);
    __m512h v512h_13 = _mm512_fmsub_ph(v512h_10, v512h_10, v512h_10);
    __m512h v512h_14 = _mm512_add_ph(v512h_11, v512h_11);
    __m256 v256f_15 = _mm256_loadu_ps(fbuf);
    __m256 v256f_16 = _mm256_and_ps(v256f_15, v256f_15);
    __m512h v512h_17 = _mm512_getexp_ph(v512h_14);
    __m512i v512i_18 = _mm512_ternarylogic_epi32(v512i_2, v512i_2, v512i_2, 0x96);
    __m128i v128i_19 = _mm_sub_epi64(p4, v128i_3);
    int s32_20 = 76;
    for (int i_21 = 0; i_21 < (int)((s32_20) & 7); i_21++) {
    __m256 v256f_22 = _mm256_xor_ps(v256f_15, v256f_15);
    __m512 v512f_23 = _mm512_loadu_ps(fbuf);
    __m512 v512f_24 = _mm512_roundscale_ps(v512f_23, 0x10);
    __m128d v128d_25 = _mm_set1_pd(2.0);
    }
    g_v256f[1] = v256f_16;
    g_v128f[3] = v128f_5;
    g_v128h[9] = v128h_4;
    return v512h_17;
}

__m128h torture_324(__m512 p0, __m256h p1, __m512h p2, __m512d p3) {
    __mmask8 m8_1 = (__mmask8)(g_m8[10] ^ 183);
    __m512d v512d_2 = _mm512_mask_blend_pd(m8_1, p3, p3);
    __m256d v256d_3 = g_v256d[8];
    __m256d v256d_4 = _mm256_div_pd(v256d_3, v256d_3);
    __m512 v512f_5 = _mm512_fnmadd_ps(p0, p0, p0);
    __m128 v128f_6 = _mm_loadu_ps(fbuf);
    __m128 v128f_7 = _mm_and_ps(v128f_6, v128f_6);
    __m128 v128f_8 = _mm_fnmadd_ps(v128f_6, v128f_7, v128f_7);
    __m512 v512f_9 = _mm512_unpacklo_ps(p0, v512f_5);
    __m256d v256d_10 = _mm256_div_pd(v256d_3, v256d_3);
    __m128 v128f_11 = _mm_set1_ps(1.0f);
    __m512i v512i_12 = g_v512i[4];
    __m512i v512i_13 = _mm512_sub_epi8(v512i_12, v512i_12);
    __m128 v128f_14 = _mm_mul_ps(v128f_6, v128f_11);
    __m512i v512i_15 = _mm512_dpbusd_epi32(v512i_13, v512i_13, v512i_12);
    __mmask16 m16_16 = (__mmask16)(g_m16[1] ^ 55);
    __m512i v512i_17 = _mm512_mask_blend_epi32(m16_16, v512i_13, v512i_12);
    __m512i v512i_18 = _mm512_or_si512(v512i_17, v512i_17);
    __m512i v512i_19 = _mm512_and_si512(v512i_18, v512i_15);
    __m128d v128d_20 = g_v128d[3];
    __m128d v128d_21 = _mm_unpackhi_pd(v128d_20, v128d_20);
    __m128d v128d_22 = _mm_mul_pd(v128d_21, v128d_20);
    __m128 v128f_23 = _mm_and_ps(v128f_7, v128f_14);
    __m512h v512h_24 = _mm512_fmadd_ph(p2, p2, p2);
    __mmask32 m32_25 = _mm512_cmp_ph_mask(v512h_24, v512h_24, 1);
    __m128 v128f_26 = _mm_fnmadd_ps(v128f_14, v128f_11, v128f_23);
    __m128i v128i_27 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_28 = _mm_unpacklo_epi32(v128i_27, v128i_27);
    __m512i v512i_29 = _mm512_add_epi64(v512i_19, v512i_12);
    __m512h v512h_30 = _mm512_mul_ph(p2, p2);
    __m512i v512i_31 = _mm512_add_epi64(v512i_13, v512i_18);
    __m512i v512i_32 = _mm512_add_epi16(v512i_17, v512i_15);
    __m256h v256h_33 = _mm512_cvtxps_ph(v512f_9);
    __m512h v512h_34 = _mm512_cvtepi16_ph(v512i_15);
    __m128d v128d_35 = _mm_max_pd(v128d_22, v128d_21);
    g_v512f[6] = v512f_9;
    g_v128d[1] = v128d_21;
    g_v512h[9] = p2;
    g_v256h[3] = p1;
    g_m8[1] = m8_1;
    g_m16[11] = m16_16;
    g_m32[6] = m32_25;
    __m128h v128h_36 = g_v128h[10];
    return v128h_36;
}

__mmask8 torture_325(__m256h p0, __m128i p1, __m256h p2, __m128d p3) {
    __m128i v128i_1 = _mm_abs_epi32(p1);
    __m128i v128i_2 = _mm_slli_epi32(v128i_1, 3);
    __m128i v128i_3 = _mm_abs_epi32(v128i_1);
    unsigned u32_4 = 86;
    __mmask16 m16_5 = _cvtu32_mask16(u32_4);
    __m128 v128f_6 = _mm_loadu_ps(fbuf);
    __m128 v128f_7 = _mm_fmadd_ps(v128f_6, v128f_6, v128f_6);
    __m512d v512d_8 = g_v512d[2];
    __m512d v512d_9 = _mm512_fmadd_pd(v512d_8, v512d_8, v512d_8);
    __m256i v256i_10 = g_v256i[8];
    __m256i v256i_11 = _mm256_sub_epi8(v256i_10, v256i_10);
    __m256i v256i_12 = _mm256_sub_epi64(v256i_11, v256i_11);
    __m256 v256f_13 = _mm256_loadu_ps(fbuf);
    __m256 v256f_14 = _mm256_min_ps(v256f_13, v256f_13);
    __m128 v128f_15 = _mm_unpacklo_ps(v128f_6, v128f_6);
    __m128i v128i_16 = _mm_set1_epi32(0x55);
    __m128h v128h_17 = g_v128h[2];
    __m512d v512d_18 = _mm512_cvtph_pd(v128h_17);
    __mmask8 m8_19 = (__mmask8)(g_m8[10] ^ 235);
    __mmask8 m8_20 = _kand_mask8(m8_19, m8_19);
    __m256d v256d_21 = g_v256d[8];
    __m256d v256d_22 = _mm256_div_pd(v256d_21, v256d_21);
    __m128 v128f_23 = _mm_div_ps(v128f_7, v128f_15);
    __m512 v512f_24 = _mm512_loadu_ps(fbuf);
    __m512 v512f_25 = _mm512_maskz_mul_ps(m16_5, v512f_24, v512f_24);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m16_5) & 7); i_26++) {
    __m128i v128i_27 = _mm_add_epi8(v128i_3, v128i_1);
    __m512i v512i_28 = g_v512i[5];
    __m512i v512i_29 = _mm512_mask_add_epi32(v512i_28, m16_5, v512i_28, v512i_28);
    __mmask16 m16_30 = _mm512_fpclass_ps_mask(v512f_24, 0x3);
    }
    g_v512d[2] = v512d_9;
    g_v256d[6] = v256d_22;
    g_v256i[5] = v256i_10;
    g_v256h[6] = p0;
    return m8_20;
}

__m128 torture_326(__m128 p0, __m512h p1, __m128d p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_fmsub_ps(v512f_1, v512f_1, v512f_1);
    __m512d v512d_3 = g_v512d[8];
    __m512d v512d_4 = _mm512_add_pd(v512d_3, v512d_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_sub_epi64(v128i_5, v128i_5);
    __m512h v512h_7 = _mm512_fmul_pch(p1, p1);
    __m512i v512i_8 = g_v512i[1];
    __m512i v512i_9 = _mm512_sub_epi16(v512i_8, v512i_8);
    __mmask16 m16_10 = (__mmask16)(g_m16[8] ^ 148);
    __m512 v512f_11 = _mm512_mask_blend_ps(m16_10, v512f_2, v512f_2);
    __m128 v128f_12 = _mm_xor_ps(p0, p0);
    __m512i v512i_13 = _mm512_xor_si512(v512i_9, v512i_8);
    __m256d v256d_14 = g_v256d[1];
    __m256d v256d_15 = _mm256_unpackhi_pd(v256d_14, v256d_14);
    __mmask32 m32_16 = _mm512_cmp_ph_mask(p1, p1, 1);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m512 v512f_17 = _mm512_cvtepi32_ps(v512i_9);
    __m512i v512i_18 = _mm512_ternarylogic_epi32(v512i_8, v512i_13, v512i_13, 0x96);
    __m512i v512i_19 = _mm512_sub_epi16(v512i_8, v512i_18);
    }
    __m512i v512i_20 = _mm512_lzcnt_epi32(v512i_13);
    __m512i v512i_21 = _mm512_xor_si512(v512i_9, v512i_20);
    __m256i v256i_22 = _mm256_set1_epi32(0x55);
    __m256i v256i_23 = _mm256_shuffle_epi8(v256i_22, v256i_22);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m128i v128i_24 = _mm_mullo_epi32(v128i_6, v128i_6);
    __m512i v512i_25 = _mm512_dpbusd_epi32(v512i_8, v512i_21, v512i_20);
    __m512 v512f_26 = _mm512_unpacklo_ps(v512f_1, v512f_1);
    __m128d v128d_27 = _mm_set1_pd(2.0);
    unsigned u32_28 = _cvtmask16_u32(m16_10);
    __m512 v512f_29 = _mm512_sqrt_ps(v512f_1);
    }
    g_v256d[1] = v256d_14;
    g_v512h[3] = p1;
    g_m16[4] = m16_10;
    g_m32[3] = m32_16;
    return v128f_12;
}

__mmask32 torture_327(__m256 p0, __m512d p1, __m256d p2, __m512h p3) {
    __m128h v128h_1 = _mm512_cvtpd_ph(p1);
    __m512d v512d_2 = _mm512_min_pd(p1, p1);
    __m512d v512d_3 = _mm512_sqrt_pd(p1);
    __mmask16 m16_4 = (__mmask16)(g_m16[10] ^ 77);
    __mmask16 m16_5 = _knot_mask16(m16_4);
    __m512d v512d_6 = _mm512_shuffle_f64x2(v512d_2, v512d_3, 0xEE);
    __m512 v512f_7 = _mm512_loadu_ps(fbuf);
    __m512 v512f_8 = _mm512_fmadd_ps(v512f_7, v512f_7, v512f_7);
    __m512i v512i_9 = g_v512i[10];
    __m512i v512i_10 = _mm512_ternarylogic_epi32(v512i_9, v512i_9, v512i_9, 0x96);
    __mmask8 m8_11 = (__mmask8)(g_m8[3] ^ 197);
    __mmask16 m16_12 = _mm512_kunpackb(m8_11, m8_11);
    __m256i v256i_13 = g_v256i[1];
    __m256i v256i_14 = _mm256_sub_epi32(v256i_13, v256i_13);
    __m128i v128i_15 = g_v128i[11];
    __m128i v128i_16 = _mm_xor_si128(v128i_15, v128i_15);
    __m256i v256i_17 = _mm256_sub_epi16(v256i_14, v256i_13);
    __m128i v128i_18 = _mm_xor_si128(v128i_16, v128i_15);
    __m256d v256d_19 = _mm256_min_pd(p2, p2);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_5) & 7); i_20++) {
    __m256i v256i_21 = _mm512_cvtepi64_epi32(v512i_10);
    __m256d v256d_22 = _mm256_add_pd(p2, p2);
    __mmask32 m32_23 = _mm512_kunpackw(m16_5, m16_4);
    __m256h v256h_24 = g_v256h[6];
    __m512 v512f_25 = _mm512_cvtxph_ps(v256h_24);
    __m256 v256f_26 = _mm256_shuffle_ps(p0, p0, 0x1B);
    }
    g_v512f[11] = v512f_8;
    g_v512d[10] = v512d_6;
    g_v256i[1] = v256i_17;
    g_v128i[8] = v128i_16;
    g_v128h[5] = v128h_1;
    g_m16[6] = m16_12;
    __mmask32 m32_27 = (__mmask32)(g_m32[11] ^ 209);
    return m32_27;
}

__m128i torture_328(__m256h p0, __m256 p1) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_rol_epi32(v512i_1, 5);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_fmadd_ps(v512f_3, v512f_3, v512f_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_mullo_epi32(v128i_5, v128i_5);
    __m128i v128i_7 = _mm_xor_si128(v128i_5, v128i_6);
    __m128i v128i_8 = _mm_sub_epi16(v128i_6, v128i_6);
    __m512i v512i_9 = _mm512_or_si512(v512i_1, v512i_1);
    __m512i v512i_10 = _mm512_rolv_epi32(v512i_2, v512i_2);
    __m128 v128f_11 = g_v128f[1];
    __m128 v128f_12 = _mm_fmsub_ps(v128f_11, v128f_11, v128f_11);
    __m512i v512i_13 = _mm512_set1_epi32(0x55);
    __m512d v512d_14 = _mm512_loadu_pd(dbuf);
    __m512d v512d_15 = _mm512_mul_pd(v512d_14, v512d_14);
    __mmask8 m8_16 = (__mmask8)(g_m8[11] ^ 188);
    __m512i v512i_17 = _mm512_broadcastmb_epi64(m8_16);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m8_16) & 7); i_18++) {
    __mmask64 m64_19 = (__mmask64)(g_m64[2] ^ 29);
    __mmask64 m64_20 = _kor_mask64(m64_19, m64_19);
    __m256 v256f_21 = _mm256_mul_ps(p1, p1);
    }
    __m512d v512d_22 = _mm512_cvtps_pd(p1);
    __m128d v128d_23 = _mm_set1_pd(2.0);
    __m512i v512i_24 = _mm512_sub_epi16(v512i_2, v512i_10);
    __mmask16 m16_25 = (__mmask16)(g_m16[1] ^ 46);
    __m512i v512i_26 = _mm512_mask_add_epi32(v512i_17, m16_25, v512i_17, v512i_17);
    __m512d v512d_27 = _mm512_mul_pd(v512d_15, v512d_14);
    __m512 v512f_28 = _mm512_unpacklo_ps(v512f_4, v512f_3);
    __m256i v256i_29 = g_v256i[0];
    __m256i v256i_30 = _mm256_add_epi16(v256i_29, v256i_29);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_25) & 7); i_31++) {
    __m256i v256i_32 = _mm256_sub_epi16(v256i_30, v256i_29);
    __m512i v512i_33 = _mm512_permutexvar_epi32(v512i_17, v512i_17);
    __m256i v256i_34 = _mm256_sub_epi32(v256i_29, v256i_32);
    __m512i v512i_35 = _mm512_broadcast_i32x4(v128i_8);
    __m256i v256i_36 = _mm256_and_si256(v256i_32, v256i_29);
    }
    g_v256f[5] = p1;
    g_v256i[0] = v256i_30;
    g_v128f[8] = v128f_11;
    g_v128i[9] = v128i_6;
    g_v256h[5] = p0;
    g_m16[4] = m16_25;
    return v128i_5;
}

__m128h torture_329(__m256h p0, __m128h p1, __m256h p2, __m256i p3, __m128d p4) {
    __m128d v128d_1 = _mm_set1_pd(2.0);
    __m256i v256i_2 = _mm256_add_epi32(p3, p3);
    __m256i v256i_3 = _mm256_sub_epi16(v256i_2, p3);
    __m128 v128f_4 = g_v128f[7];
    __m128 v128f_5 = _mm_div_ps(v128f_4, v128f_4);
    __m256i v256i_6 = _mm256_sub_epi16(p3, v256i_3);
    unsigned u32_7 = 63;
    __mmask16 m16_8 = _cvtu32_mask16(u32_7);
    __m512 v512f_9 = g_v512f[7];
    __mmask16 m16_10 = _mm512_fpclass_ps_mask(v512f_9, 0x3);
    __mmask16 m16_11 = _kshiftri_mask16(m16_10, 1);
    if (((unsigned long long)m16_8) & 1ULL) {
    __m256i v256i_12 = _mm256_unpacklo_epi32(v256i_2, p3);
    __mmask16 m16_13 = _cvtu32_mask16(u32_7);
    __m512i v512i_14 = g_v512i[11];
    __mmask16 m16_15 = _mm512_movepi32_mask(v512i_14);
    __mmask16 m16_16 = _kand_mask16(m16_13, m16_8);
    }
    __m512d v512d_17 = _mm512_loadu_pd(dbuf);
    __m512d v512d_18 = _mm512_add_pd(v512d_17, v512d_17);
    __m512h v512h_19 = g_v512h[4];
    __m512h v512h_20 = _mm512_add_ph(v512h_19, v512h_19);
    __mmask8 m8_21 = _mm512_cmp_pd_mask(v512d_17, v512d_17, _CMP_GE_OQ);
    __m128 v128f_22 = _mm_add_ps(v128f_4, v128f_5);
    __m512 v512f_23 = _mm512_mask_blend_ps(m16_8, v512f_9, v512f_9);
    __m512 v512f_24 = _mm512_mask_add_ps(v512f_9, m16_11, v512f_23, v512f_9);
    __m128 v128f_25 = _mm_shuffle_ps(v128f_4, v128f_4, 0x1B);
    __m512d v512d_26 = _mm512_cvtph_pd(p1);
    g_v512f[11] = v512f_9;
    g_v256i[10] = v256i_6;
    g_v128f[5] = v128f_4;
    return p1;
}

__m128h torture_330(__m128d p0, __m512 p1, __m128d p2, __m128i p3, __m128 p4) {
    __m512i v512i_1 = _mm512_broadcast_i32x4(p3);
    __mmask8 m8_2 = (__mmask8)(g_m8[2] ^ 212);
    __m512i v512i_3 = _mm512_broadcastmb_epi64(m8_2);
    __m128 v128f_4 = _mm_min_ps(p4, p4);
    __mmask16 m16_5 = (__mmask16)(g_m16[5] ^ 4);
    __m512i v512i_6 = _mm512_mask_blend_epi32(m16_5, v512i_1, v512i_1);
    __m512i v512i_7 = _mm512_mullo_epi32(v512i_6, v512i_6);
    __m128i v128i_8 = _mm_add_epi16(p3, p3);
    __m128i v128i_9 = _mm_xor_si128(v128i_8, v128i_8);
    __m128 v128f_10 = _mm_set1_ps(1.0f);
    __m256i v256i_11 = g_v256i[1];
    __m256i v256i_12 = _mm256_add_epi16(v256i_11, v256i_11);
    if (((unsigned long long)m16_5) & 1ULL) {
    __mmask64 m64_13 = _mm512_movepi8_mask(v512i_7);
    __m128i v128i_14 = _mm_unpacklo_epi32(v128i_8, p3);
    __m128 v128f_15 = _mm_sqrt_ps(p4);
    }
    __m512h v512h_16 = g_v512h[6];
    __m512h v512h_17 = _mm512_mul_ph(v512h_16, v512h_16);
    __m512 v512f_18 = _mm512_max_ps(p1, p1);
    __m256d v256d_19 = g_v256d[0];
    __m256d v256d_20 = _mm256_fmadd_pd(v256d_19, v256d_19, v256d_19);
    __m512d v512d_21 = g_v512d[9];
    __m512d v512d_22 = _mm512_fmadd_pd(v512d_21, v512d_21, v512d_21);
    __m256h v256h_23 = _mm512_cvtxps_ph(p1);
    __m128 v128f_24 = _mm_sub_ps(v128f_4, v128f_10);
    __m256d v256d_25 = _mm256_min_pd(v256d_20, v256d_19);
    __m256d v256d_26 = _mm256_set1_pd(2.0);
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m512d v512d_28 = _mm512_cvtps_pd(v256f_27);
    __mmask16 m16_29 = _mm512_cmp_epi32_mask(v512i_1, v512i_3, 2);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m128i v128i_30 = _mm512_cvtepi32_epi8(v512i_1);
    __m128d v128d_31 = _mm_sqrt_pd(p2);
    }
    g_v256d[7] = v256d_20;
    g_v256i[7] = v256i_12;
    g_v128i[8] = v128i_8;
    g_v512h[7] = v512h_16;
    g_m16[10] = m16_5;
    __m128h v128h_32 = g_v128h[2];
    return v128h_32;
}

__m128d torture_331(__m256h p0, __m128i p1) {
    __m512i v512i_1 = g_v512i[0];
    __mmask16 m16_2 = _mm512_movepi32_mask(v512i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_fmsub_ps(v512f_3, v512f_3, v512f_3);
    __m256 v256f_5 = _mm256_loadu_ps(fbuf);
    __m256 v256f_6 = _mm256_and_ps(v256f_5, v256f_5);
    __m256i v256i_7 = g_v256i[2];
    __m256i v256i_8 = _mm256_xor_si256(v256i_7, v256i_7);
    __m128i v128i_9 = _mm_and_si128(p1, p1);
    __m128d v128d_10 = g_v128d[7];
    __m128d v128d_11 = _mm_div_pd(v128d_10, v128d_10);
    __m256i v256i_12 = _mm256_add_epi64(v256i_7, v256i_8);
    __m256i v256i_13 = _mm256_or_si256(v256i_7, v256i_7);
    __m128i v128i_14 = _mm_or_si128(v128i_9, p1);
    __m512h v512h_15 = g_v512h[8];
    __m512h v512h_16 = _mm512_fmul_pch(v512h_15, v512h_15);
    if (((unsigned long long)m16_2) & 1ULL) {
    __m256i v256i_17 = _mm512_cvtepi64_epi32(v512i_1);
    __m512h v512h_18 = _mm512_add_ph(v512h_15, v512h_16);
    __m512 v512f_19 = _mm512_maskz_mul_ps(m16_2, v512f_3, v512f_3);
    }
    __m512 v512f_20 = _mm512_and_ps(v512f_3, v512f_3);
    __m512i v512i_21 = _mm512_ternarylogic_epi32(v512i_1, v512i_1, v512i_1, 0x96);
    __m512i v512i_22 = _mm512_lzcnt_epi32(v512i_1);
    __m256i v256i_23 = _mm256_set1_epi32(0x55);
    __m512d v512d_24 = g_v512d[9];
    __m512d v512d_25 = _mm512_mul_pd(v512d_24, v512d_24);
    __m512 v512f_26 = _mm512_rsqrt14_ps(v512f_20);
    __m256i v256i_27 = _mm256_add_epi8(v256i_23, v256i_8);
    __m128i v128i_28 = _mm_set1_epi32(0x55);
    __m512 v512f_29 = _mm512_fmsub_ps(v512f_3, v512f_20, v512f_26);
    __m512i v512i_30 = _mm512_broadcast_i32x4(v128i_28);
    __mmask16 m16_31 = _kand_mask16(m16_2, m16_2);
    __m256 v256f_32 = _mm256_div_ps(v256f_5, v256f_6);
    if (((unsigned long long)m16_31) & 1ULL) {
    __m256d v256d_33 = g_v256d[4];
    __m256d v256d_34 = _mm256_sqrt_pd(v256d_33);
    __m512 v512f_35 = _mm512_div_ps(v512f_20, v512f_4);
    __m512i v512i_36 = _mm512_conflict_epi32(v512i_22);
    __m512 v512f_37 = _mm512_fmadd_ps(v512f_4, v512f_26, v512f_26);
    __m512h v512h_38 = _mm512_add_ph(v512h_16, v512h_16);
    __m512 v512f_39 = _mm512_mask3_fmadd_ps(v512f_4, v512f_35, v512f_29, m16_31);
    }
    g_v512f[0] = v512f_29;
    g_v512d[10] = v512d_25;
    g_v512i[11] = v512i_30;
    g_v256i[6] = v256i_8;
    g_v512h[4] = v512h_16;
    g_m16[10] = m16_31;
    return v128d_11;
}

__m256 torture_332(__m128d p0, __m128d p1) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m256d v256d_2 = _mm512_extractf64x4_pd(v512d_1, 1);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_and_ps(v256f_3, v256f_3);
    __m128i v128i_5 = g_v128i[11];
    __m128i v128i_6 = _mm_shuffle_epi8(v128i_5, v128i_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_8 = _mm512_cmp_epu32_mask(v512i_7, v512i_7, 4);
    __m512h v512h_9 = g_v512h[5];
    __m512i v512i_10 = _mm512_cvtph_epi16(v512h_9);
    __m128i v128i_11 = _mm_set1_epi32(0x55);
    __m512i v512i_12 = _mm512_permutexvar_epi32(v512i_10, v512i_10);
    __m512i v512i_13 = _mm512_cvtepi8_epi32(v128i_6);
    __m256 v256f_14 = _mm256_max_ps(v256f_3, v256f_3);
    __m512i v512i_15 = _mm512_or_si512(v512i_10, v512i_13);
    __m128 v128f_16 = g_v128f[10];
    __m128 v128f_17 = _mm_xor_ps(v128f_16, v128f_16);
    __m128i v128i_18 = _mm_xor_si128(v128i_5, v128i_5);
    __m256i v256i_19 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_20 = _mm256_mullo_epi32(v256i_19, v256i_19);
    __mmask16 m16_21 = _kshiftli_mask16(m16_8, 2);
    __m128d v128d_22 = _mm_sqrt_pd(p0);
    __m512i v512i_23 = _mm512_mullo_epi32(v512i_7, v512i_12);
    __m128i v128i_24 = _mm_sllv_epi32(v128i_6, v128i_11);
    __mmask32 m32_25 = (__mmask32)(g_m32[6] ^ 137);
    __mmask32 m32_26 = _kxor_mask32(m32_25, m32_25);
    __m128d v128d_27 = _mm_mul_pd(v128d_22, p1);
    __m128 v128f_28 = _mm_max_ps(v128f_16, v128f_17);
    __m256d v256d_29 = _mm512_extractf64x4_pd(v512d_1, 1);
    __m512 v512f_30 = g_v512f[11];
    __m512 v512f_31 = _mm512_fmsub_ps(v512f_30, v512f_30, v512f_30);
    g_v512f[0] = v512f_31;
    g_v512d[7] = v512d_1;
    g_v256i[8] = v256i_20;
    g_v128f[3] = v128f_17;
    g_v128d[7] = p1;
    g_v128i[5] = v128i_5;
    g_m32[4] = m32_25;
    return v256f_14;
}

__m512i torture_333(__m128d p0, __m512d p1) {
    __m512 v512f_1 = g_v512f[7];
    __m512 v512f_2 = _mm512_max_ps(v512f_1, v512f_1);
    __m512d v512d_3 = _mm512_sub_pd(p1, p1);
    __m128d v128d_4 = _mm_min_pd(p0, p0);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_xor_si128(v128i_5, v128i_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[1] ^ 246);
    unsigned u32_8 = _cvtmask16_u32(m16_7);
    __m256d v256d_9 = _mm256_set1_pd(2.0);
    __m512i v512i_10 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_11 = _mm512_madd52lo_epu64(v512i_10, v512i_10, v512i_10);
    __m256 v256f_12 = g_v256f[11];
    __m512 v512f_13 = _mm512_insertf32x8(v512f_1, v256f_12, 1);
    __m128i v128i_14 = _mm_abs_epi32(v128i_5);
    __m128 v128f_15 = g_v128f[1];
    __m128 v128f_16 = _mm_min_ps(v128f_15, v128f_15);
    __m256 v256f_17 = _mm256_shuffle_ps(v256f_12, v256f_12, 0x1B);
    __m128i v128i_18 = _mm512_cvtepi32_epi8(v512i_10);
    __m128 v128f_19 = _mm_fnmadd_ps(v128f_15, v128f_15, v128f_16);
    __mmask8 m8_20 = (__mmask8)(g_m8[6] ^ 105);
    __m512d v512d_21 = _mm512_mask_blend_pd(m8_20, p1, p1);
    __m512i v512i_22 = _mm512_permutex2var_epi32(v512i_10, v512i_11, v512i_11);
    __m512 v512f_23 = _mm512_min_ps(v512f_13, v512f_1);
    __m128i v128i_24 = _mm_sub_epi64(v128i_6, v128i_18);
    __m128i v128i_25 = _mm_sub_epi32(v128i_5, v128i_18);
    __m128i v128i_26 = _mm_add_epi8(v128i_24, v128i_18);
    __m512d v512d_27 = _mm512_insertf64x2(v512d_21, p0, 3);
    __m512i v512i_28 = _mm512_mullo_epi32(v512i_11, v512i_10);
    unsigned u32_29 = _cvtmask16_u32(m16_7);
    __m256 v256f_30 = _mm256_sub_ps(v256f_12, v256f_12);
    g_v128f[1] = v128f_15;
    g_v128d[7] = v128d_4;
    g_v128i[3] = v128i_25;
    g_m8[6] = m8_20;
    g_m16[1] = m16_7;
    return v512i_10;
}

__m256h torture_334(__m512i p0, __m128 p1, __m512h p2, __m512i p3, __m256d p4) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_add_epi32(v128i_1, v128i_1);
    __m128i v128i_3 = _mm_add_epi8(v128i_2, v128i_1);
    __m128d v128d_4 = g_v128d[9];
    __m128d v128d_5 = _mm_sub_pd(v128d_4, v128d_4);
    __m256d v256d_6 = _mm256_sqrt_pd(p4);
    __m512h v512h_7 = _mm512_mul_ph(p2, p2);
    __m512i v512i_8 = _mm512_sub_epi64(p0, p3);
    __m128i v128i_9 = _mm_xor_si128(v128i_3, v128i_2);
    __m512i v512i_10 = _mm512_and_si512(p3, v512i_8);
    __m512d v512d_11 = _mm512_loadu_pd(dbuf);
    __m256d v256d_12 = _mm512_extractf64x4_pd(v512d_11, 1);
    __m128d v128d_13 = _mm_sqrt_pd(v128d_4);
    __m512 v512f_14 = g_v512f[3];
    __m512 v512f_15 = _mm512_unpacklo_ps(v512f_14, v512f_14);
    __m128i v128i_16 = _mm_mullo_epi32(v128i_1, v128i_1);
    __m128i v128i_17 = _mm_add_epi32(v128i_2, v128i_1);
    __m512i v512i_18 = _mm512_sub_epi16(p3, v512i_10);
    __mmask32 m32_19 = _mm512_cmp_ph_mask(v512h_7, v512h_7, 1);
    __m256h v256h_20 = _mm512_cvtxps_ph(v512f_15);
    __m256d v256d_21 = _mm256_max_pd(p4, p4);
    if (((unsigned long long)m32_19) & 1ULL) {
    __m512i v512i_22 = _mm512_sub_epi8(v512i_18, v512i_18);
    __m512 v512f_23 = _mm512_rcp14_ps(v512f_15);
    __m128i v128i_24 = _mm_sub_epi8(v128i_3, v128i_9);
    __m512d v512d_25 = _mm512_mul_pd(v512d_11, v512d_11);
    }
    __m128i v128i_26 = _mm_sub_epi16(v128i_1, v128i_17);
    __m256d v256d_27 = _mm256_unpackhi_pd(v256d_12, v256d_21);
    __m256i v256i_28 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_29 = _mm256_add_epi8(v256i_28, v256i_28);
    __m512d v512d_30 = _mm512_div_pd(v512d_11, v512d_11);
    __m512 v512f_31 = _mm512_rcp14_ps(v512f_15);
    __mmask16 m16_32 = (__mmask16)(g_m16[4] ^ 29);
    __mmask16 m16_33 = _kxor_mask16(m16_32, m16_32);
    __m256 v256f_34 = g_v256f[10];
    __m256 v256f_35 = _mm256_fnmadd_ps(v256f_34, v256f_34, v256f_34);
    __m128 v128f_36 = _mm_mul_ps(p1, p1);
    if (((unsigned long long)m16_32) & 1ULL) {
    __mmask16 m16_37 = _mm512_test_epi32_mask(v512i_10, v512i_18);
    __m256d v256d_38 = _mm256_unpackhi_pd(v256d_27, p4);
    __m512 v512f_39 = _mm512_getexp_ps(v512f_31);
    __m128i v128i_40 = _mm_xor_si128(v128i_3, v128i_16);
    __m128i v128i_41 = _mm_shuffle_epi8(v128i_9, v128i_3);
    __m256i v256i_42 = _mm256_set1_epi32(0x55);
    }
    g_v512d[11] = v512d_30;
    g_v128i[7] = v128i_1;
    return v256h_20;
}

__m128 torture_335(__m128h p0, __m128i p1, __m256i p2) {
    __m512i v512i_1 = g_v512i[4];
    __m512 v512f_2 = _mm512_cvtepi32_ps(v512i_1);
    __m512i v512i_3 = _mm512_broadcast_i32x4(p1);
    __m512i v512i_4 = _mm512_madd52lo_epu64(v512i_3, v512i_3, v512i_1);
    __mmask16 m16_5 = _mm512_movepi32_mask(v512i_4);
    __m256i v256i_6 = _mm256_set1_epi32(0x55);
    __mmask16 m16_7 = _mm512_fpclass_ps_mask(v512f_2, 0x3);
    __m512d v512d_8 = g_v512d[11];
    __mmask8 m8_9 = _mm512_cmp_pd_mask(v512d_8, v512d_8, _CMP_GE_OQ);
    __mmask8 m8_10 = _mm512_cmp_pd_mask(v512d_8, v512d_8, _CMP_GE_OQ);
    __mmask32 m32_11 = (__mmask32)(g_m32[7] ^ 225);
    __mmask32 m32_12 = _kxor_mask32(m32_11, m32_11);
    __m256 v256f_13 = _mm512_cvtpd_ps(v512d_8);
    __m256i v256i_14 = _mm256_and_si256(p2, p2);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_5) & 7); i_15++) {
    __m256i v256i_16 = _mm256_sub_epi8(p2, v256i_14);
    __m512i v512i_17 = _mm512_conflict_epi32(v512i_1);
    __m128i v128i_18 = _mm_abs_epi32(p1);
    __m128i v128i_19 = _mm_add_epi8(p1, v128i_18);
    __m128i v128i_20 = _mm_sub_epi32(p1, p1);
    }
    __m256i v256i_21 = _mm256_shuffle_epi8(v256i_14, p2);
    __m512i v512i_22 = _mm512_dpwssd_epi32(v512i_1, v512i_3, v512i_1);
    __mmask16 m16_23 = _kor_mask16(m16_5, m16_7);
    __m512i v512i_24 = _mm512_unpacklo_epi32(v512i_1, v512i_22);
    __m512i v512i_25 = _mm512_unpacklo_epi32(v512i_1, v512i_24);
    __m512i v512i_26 = _mm512_cvtepi8_epi32(p1);
    __m512d v512d_27 = _mm512_cvtph_pd(p0);
    __m512h v512h_28 = g_v512h[9];
    __m512h v512h_29 = _mm512_add_ph(v512h_28, v512h_28);
    __m128d v128d_30 = g_v128d[10];
    __m128d v128d_31 = _mm_div_pd(v128d_30, v128d_30);
    unsigned u32_32 = _cvtmask16_u32(m16_7);
    if (((unsigned long long)m16_7) & 1ULL) {
    __m256 v256f_33 = _mm256_set1_ps(1.0f);
    __m512 v512f_34 = _mm512_mask_blend_ps(m16_23, v512f_2, v512f_2);
    __m128d v128d_35 = _mm_set1_pd(2.0);
    __mmask16 m16_36 = _kand_mask16(m16_5, m16_5);
    __m128d v128d_37 = _mm_min_pd(v128d_30, v128d_30);
    __m512i v512i_38 = _mm512_set1_epi32(0x55);
    }
    g_v512d[3] = v512d_8;
    g_v512i[8] = v512i_24;
    g_v256f[0] = v256f_13;
    g_v512h[5] = v512h_29;
    g_m16[2] = m16_23;
    g_m32[6] = m32_11;
    __m128 v128f_39 = g_v128f[2];
    return v128f_39;
}

__m512 torture_336(__m256h p0, __m512d p1, __m128i p2, __m256h p3) {
    __m512i v512i_1 = g_v512i[5];
    __m512i v512i_2 = _mm512_sub_epi32(v512i_1, v512i_1);
    __m256 v256f_3 = g_v256f[11];
    __m256 v256f_4 = _mm256_xor_ps(v256f_3, v256f_3);
    __m128 v128f_5 = g_v128f[1];
    __m128 v128f_6 = _mm_max_ps(v128f_5, v128f_5);
    __m512d v512d_7 = _mm512_mul_pd(p1, p1);
    __m128i v128i_8 = _mm_sllv_epi32(p2, p2);
    __m256 v256f_9 = _mm256_fmadd_ps(v256f_3, v256f_3, v256f_4);
    __mmask16 m16_10 = (__mmask16)(g_m16[0] ^ 15);
    __mmask32 m32_11 = _mm512_kunpackw(m16_10, m16_10);
    __m256d v256d_12 = g_v256d[4];
    __m256d v256d_13 = _mm256_unpackhi_pd(v256d_12, v256d_12);
    __m256 v256f_14 = _mm256_add_ps(v256f_4, v256f_4);
    unsigned u32_15 = 60;
    __mmask16 m16_16 = _cvtu32_mask16(u32_15);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_10) & 7); i_17++) {
    __m512 v512f_18 = _mm512_loadu_ps(fbuf);
    __m512 v512f_19 = _mm512_rsqrt14_ps(v512f_18);
    __m512i v512i_20 = _mm512_popcnt_epi64(v512i_2);
    __m512 v512f_21 = _mm512_sqrt_ps(v512f_18);
    }
    __m512h v512h_22 = g_v512h[2];
    __m512h v512h_23 = _mm512_rcp_ph(v512h_22);
    __mmask32 m32_24 = _kxor_mask32(m32_11, m32_11);
    __mmask32 m32_25 = _mm512_kunpackw(m16_10, m16_10);
    __m256d v256d_26 = _mm256_mul_pd(v256d_13, v256d_12);
    __m256i v256i_27 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_28 = _mm256_unpacklo_epi32(v256i_27, v256i_27);
    __m512i v512i_29 = _mm512_unpacklo_epi32(v512i_1, v512i_1);
    __m512i v512i_30 = _mm512_set1_epi32(0x55);
    for (int i_31 = 0; i_31 < (int)(((unsigned long long)m16_10) & 7); i_31++) {
    __m512i v512i_32 = _mm512_madd52lo_epu64(v512i_30, v512i_30, v512i_30);
    __m512i v512i_33 = _mm512_shuffle_epi8(v512i_29, v512i_29);
    __m256 v256f_34 = _mm256_sub_ps(v256f_3, v256f_9);
    __m256i v256i_35 = _mm256_add_epi64(v256i_27, v256i_28);
    __m128d v128d_36 = g_v128d[4];
    __m128d v128d_37 = _mm_mul_pd(v128d_36, v128d_36);
    }
    g_v128i[11] = p2;
    g_v256h[9] = p0;
    g_m32[8] = m32_25;
    __m512 v512f_38 = g_v512f[2];
    return v512f_38;
}

__m256i torture_337(__m128h p0, __m256 p1) {
    __m128d v128d_1 = g_v128d[11];
    __m128d v128d_2 = _mm_sqrt_pd(v128d_1);
    __m128i v128i_3 = g_v128i[10];
    __m128i v128i_4 = _mm_or_si128(v128i_3, v128i_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __mmask16 m16_6 = _mm512_cmp_ps_mask(v512f_5, v512f_5, _CMP_LT_OQ);
    __m128d v128d_7 = _mm_fmadd_pd(v128d_2, v128d_2, v128d_1);
    __m128d v128d_8 = _mm_div_pd(v128d_2, v128d_7);
    __m512d v512d_9 = g_v512d[9];
    __m512d v512d_10 = _mm512_max_pd(v512d_9, v512d_9);
    __m128i v128i_11 = _mm_add_epi8(v128i_4, v128i_4);
    __m512 v512f_12 = _mm512_mask3_fmadd_ps(v512f_5, v512f_5, v512f_5, m16_6);
    __m512d v512d_13 = _mm512_fmadd_pd(v512d_9, v512d_9, v512d_10);
    __m512 v512f_14 = _mm512_rcp14_ps(v512f_5);
    __m256 v256f_15 = _mm256_div_ps(p1, p1);
    for (int i_16 = 0; i_16 < (int)(((unsigned long long)m16_6) & 7); i_16++) {
    __mmask8 m8_17 = _mm512_cmp_pd_mask(v512d_9, v512d_10, _CMP_GE_OQ);
    __m512d v512d_18 = _mm512_sqrt_pd(v512d_9);
    __m256i v256i_19 = g_v256i[5];
    __m256i v256i_20 = _mm256_sub_epi32(v256i_19, v256i_19);
    }
    __m128 v128f_21 = _mm_loadu_ps(fbuf);
    __m128 v128f_22 = _mm_and_ps(v128f_21, v128f_21);
    __m512i v512i_23 = g_v512i[8];
    __m512i v512i_24 = _mm512_permutex2var_epi32(v512i_23, v512i_23, v512i_23);
    __m256d v256d_25 = g_v256d[0];
    __m256d v256d_26 = _mm256_mul_pd(v256d_25, v256d_25);
    __m128i v128i_27 = _mm_add_epi8(v128i_11, v128i_4);
    __m512i v512i_28 = _mm512_sllv_epi32(v512i_24, v512i_23);
    __mmask64 m64_29 = _mm512_cmp_epi8_mask(v512i_24, v512i_23, 1);
    __m512 v512f_30 = _mm512_mask_blend_ps(m16_6, v512f_5, v512f_5);
    __m256 v256f_31 = _mm256_min_ps(v256f_15, v256f_15);
    for (int i_32 = 0; i_32 < (int)(((unsigned long long)m16_6) & 7); i_32++) {
    __m256i v256i_33 = g_v256i[8];
    __m256i v256i_34 = _mm256_unpacklo_epi32(v256i_33, v256i_33);
    __m128d v128d_35 = _mm_div_pd(v128d_8, v128d_1);
    __m128 v128f_36 = _mm_fnmadd_ps(v128f_21, v128f_22, v128f_21);
    }
    g_v512f[0] = v512f_12;
    g_v256f[4] = v256f_15;
    g_v128d[11] = v128d_8;
    g_v128i[1] = v128i_11;
    g_m16[11] = m16_6;
    __m256i v256i_37 = g_v256i[9];
    return v256i_37;
}

__m512h torture_338(__m128d p0, __m512d p1, __m256h p2, __m512 p3, __m512h p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_permutexvar_epi32(v512i_1, v512i_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_abs_epi32(v128i_3);
    __m512 v512f_5 = _mm512_sqrt_ps(p3);
    __m512i v512i_6 = _mm512_popcnt_epi64(v512i_1);
    __m128d v128d_7 = _mm_unpackhi_pd(p0, p0);
    __m256i v256i_8 = g_v256i[10];
    __m256i v256i_9 = _mm256_sub_epi32(v256i_8, v256i_8);
    __m128 v128f_10 = g_v128f[2];
    __m128 v128f_11 = _mm_add_ps(v128f_10, v128f_10);
    __m512 v512f_12 = _mm512_rsqrt14_ps(v512f_5);
    __m512i v512i_13 = _mm512_cvtepi8_epi32(v128i_4);
    __m512h v512h_14 = _mm512_sqrt_ph(p4);
    __m128i v128i_15 = _mm_abs_epi32(v128i_4);
    __mmask8 m8_16 = (__mmask8)(g_m8[6] ^ 134);
    __mmask8 m8_17 = _kand_mask8(m8_16, m8_16);
    __m512i v512i_18 = _mm512_sub_epi16(v512i_6, v512i_6);
    __m512i v512i_19 = _mm512_and_si512(v512i_13, v512i_2);
    __m256i v256i_20 = _mm256_sllv_epi32(v256i_9, v256i_9);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m8_16) & 7); i_21++) {
    __m256 v256f_22 = _mm512_cvtpd_ps(p1);
    __m128 v128f_23 = _mm_and_ps(v128f_10, v128f_11);
    __m512 v512f_24 = _mm512_and_ps(p3, p3);
    }
    __m512i v512i_25 = _mm512_sub_epi64(v512i_13, v512i_13);
    __m512i v512i_26 = _mm512_lzcnt_epi32(v512i_2);
    __m512 v512f_27 = _mm512_xor_ps(v512f_5, v512f_5);
    __mmask16 m16_28 = (__mmask16)(g_m16[8] ^ 91);
    __mmask16 m16_29 = _kxor_mask16(m16_28, m16_28);
    __m512 v512f_30 = _mm512_mask_blend_ps(m16_28, v512f_27, v512f_27);
    __m128i v128i_31 = _mm_shuffle_epi8(v128i_15, v128i_3);
    __m512 v512f_32 = _mm512_sub_ps(p3, v512f_12);
    __m512i v512i_33 = _mm512_conflict_epi32(v512i_18);
    __m512i v512i_34 = _mm512_sub_epi8(v512i_33, v512i_25);
    __mmask32 m32_35 = _mm512_kunpackw(m16_28, m16_28);
    __m256 v256f_36 = _mm256_loadu_ps(fbuf);
    __m256 v256f_37 = _mm256_fnmadd_ps(v256f_36, v256f_36, v256f_36);
    __mmask16 m16_38 = _mm512_cmp_ps_mask(v512f_32, v512f_27, _CMP_LT_OQ);
    g_v512d[6] = p1;
    g_v256h[8] = p2;
    return p4;
}

__mmask32 torture_339(__m128i p0, __m512 p1, __m512h p2, __m128h p3, __m512 p4) {
    __m512d v512d_1 = _mm512_set1_pd(2.0);
    __m256 v256f_2 = _mm256_loadu_ps(fbuf);
    __m256 v256f_3 = _mm256_div_ps(v256f_2, v256f_2);
    __m256d v256d_4 = g_v256d[5];
    __m256d v256d_5 = _mm256_div_pd(v256d_4, v256d_4);
    __m256 v256f_6 = _mm256_sqrt_ps(v256f_2);
    __m256i v256i_7 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_8 = _mm256_xor_si256(v256i_7, v256i_7);
    __m128d v128d_9 = g_v128d[5];
    __m128d v128d_10 = _mm_fmadd_pd(v128d_9, v128d_9, v128d_9);
    __mmask16 m16_11 = (__mmask16)(g_m16[10] ^ 141);
    __mmask16 m16_12 = _kxor_mask16(m16_11, m16_11);
    __m512 v512f_13 = _mm512_fnmadd_ps(p4, p1, p1);
    __m512 v512f_14 = _mm512_sub_ps(v512f_13, p4);
    __m512 v512f_15 = _mm512_fnmadd_ps(v512f_14, p4, p4);
    __m128i v128i_16 = _mm_sub_epi8(p0, p0);
    __m256i v256i_17 = _mm256_add_epi16(v256i_7, v256i_7);
    __mmask16 m16_18 = _kand_mask16(m16_12, m16_11);
    __m512 v512f_19 = _mm512_fmadd_ps(v512f_14, v512f_14, v512f_14);
    __m512i v512i_20 = g_v512i[10];
    __m512i v512i_21 = _mm512_mask_blend_epi32(m16_11, v512i_20, v512i_20);
    __m512i v512i_22 = _mm512_cvtph_epi16(p2);
    __m512d v512d_23 = _mm512_fmadd_pd(v512d_1, v512d_1, v512d_1);
    __mmask64 m64_24 = _mm512_cmp_epi8_mask(v512i_22, v512i_22, 1);
    if (((unsigned long long)m16_12) & 1ULL) {
    __m256 v256f_25 = _mm256_sub_ps(v256f_2, v256f_6);
    __m128 v128f_26 = _mm_loadu_ps(fbuf);
    __m128 v128f_27 = _mm_fnmadd_ps(v128f_26, v128f_26, v128f_26);
    }
    g_v256f[3] = v256f_2;
    g_v256d[4] = v256d_5;
    g_v128d[2] = v128d_10;
    g_v512h[6] = p2;
    g_m16[10] = m16_11;
    g_m64[11] = m64_24;
    __mmask32 m32_28 = (__mmask32)(g_m32[11] ^ 204);
    return m32_28;
}

__m128 torture_340(__m512i p0, __m128h p1, __m512d p2) {
    __m512i v512i_1 = _mm512_permutexvar_epi32(p0, p0);
    __m256d v256d_2 = g_v256d[10];
    __m256d v256d_3 = _mm256_sqrt_pd(v256d_2);
    __mmask8 m8_4 = (__mmask8)(g_m8[8] ^ 234);
    __m512d v512d_5 = _mm512_mask_blend_pd(m8_4, p2, p2);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_getexp_ps(v512f_6);
    __mmask16 m16_8 = (__mmask16)(g_m16[7] ^ 196);
    __mmask16 m16_9 = _knot_mask16(m16_8);
    __m512 v512f_10 = _mm512_min_ps(v512f_7, v512f_6);
    __m256 v256f_11 = _mm256_loadu_ps(fbuf);
    __m256 v256f_12 = _mm256_fmadd_ps(v256f_11, v256f_11, v256f_11);
    __m128 v128f_13 = _mm_loadu_ps(fbuf);
    __m128 v128f_14 = _mm_and_ps(v128f_13, v128f_13);
    __m512h v512h_15 = g_v512h[4];
    __m512i v512i_16 = _mm512_cvtph_epi16(v512h_15);
    __m512i v512i_17 = _mm512_conflict_epi32(p0);
    __m512i v512i_18 = _mm512_sub_epi8(v512i_16, v512i_16);
    __m512 v512f_19 = _mm512_div_ps(v512f_7, v512f_6);
    __mmask16 m16_20 = _mm512_test_epi32_mask(v512i_1, p0);
    if (((unsigned long long)m16_20) & 1ULL) {
    __m512d v512d_21 = _mm512_fmadd_pd(p2, p2, p2);
    __m128i v128i_22 = g_v128i[4];
    __m128i v128i_23 = _mm_abs_epi32(v128i_22);
    __m512 v512f_24 = _mm512_roundscale_ps(v512f_6, 0x10);
    __m128i v128i_25 = _mm_mullo_epi32(v128i_23, v128i_23);
    __m512i v512i_26 = _mm512_dpwssd_epi32(p0, p0, v512i_16);
    __m512 v512f_27 = _mm512_add_ps(v512f_7, v512f_6);
    }
    __m256i v256i_28 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_29 = _mm256_shuffle_epi8(v256i_28, v256i_28);
    __m512 v512f_30 = _mm512_insertf32x8(v512f_6, v256f_11, 1);
    __m128i v128i_31 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_32 = _mm512_broadcast_i32x4(v128i_31);
    __m512 v512f_33 = _mm512_min_ps(v512f_7, v512f_10);
    __m512 v512f_34 = _mm512_mask_blend_ps(m16_8, v512f_10, v512f_33);
    __m512 v512f_35 = _mm512_insertf32x8(v512f_33, v256f_12, 1);
    if (((unsigned long long)m16_20) & 1ULL) {
    __m512 v512f_36 = _mm512_sqrt_ps(v512f_6);
    __m256i v256i_37 = _mm256_unpacklo_epi32(v256i_28, v256i_28);
    }
    g_v512d[4] = p2;
    g_v512i[0] = p0;
    g_v256i[3] = v256i_29;
    g_v128f[10] = v128f_14;
    g_v128i[1] = v128i_31;
    g_v128h[10] = p1;
    g_m8[0] = m8_4;
    g_m16[9] = m16_8;
    return v128f_13;
}

__m256i torture_341(__m256d p0, __m128 p1, __m128d p2, __m256d p3) {
    __m512i v512i_1 = g_v512i[6];
    __m512i v512i_2 = _mm512_madd52lo_epu64(v512i_1, v512i_1, v512i_1);
    __m512h v512h_3 = g_v512h[4];
    __m512h v512h_4 = _mm512_fmsub_ph(v512h_3, v512h_3, v512h_3);
    __m256i v256i_5 = g_v256i[3];
    __m256i v256i_6 = _mm256_sub_epi16(v256i_5, v256i_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[3] ^ 163);
    __mmask16 m16_8 = _kshiftli_mask16(m16_7, 2);
    __m128i v128i_9 = g_v128i[9];
    __m128i v128i_10 = _mm_sub_epi8(v128i_9, v128i_9);
    __mmask32 m32_11 = _mm512_cmp_ph_mask(v512h_3, v512h_4, 1);
    __m256 v256f_12 = _mm256_loadu_ps(fbuf);
    __m256 v256f_13 = _mm256_unpacklo_ps(v256f_12, v256f_12);
    __m256 v256f_14 = _mm256_div_ps(v256f_13, v256f_12);
    __mmask32 m32_15 = _mm512_cmp_ph_mask(v512h_3, v512h_4, 1);
    __m128 v128f_16 = _mm_xor_ps(p1, p1);
    __mmask32 m32_17 = _kxor_mask32(m32_15, m32_11);
    __mmask64 m64_18 = _mm512_cmp_epi8_mask(v512i_1, v512i_2, 1);
    __m512i v512i_19 = _mm512_sub_epi16(v512i_1, v512i_2);
    __m128i v128i_20 = _mm512_cvtepi32_epi8(v512i_2);
    __m512h v512h_21 = _mm512_mul_ph(v512h_3, v512h_4);
    unsigned u32_22 = 56;
    __mmask16 m16_23 = _cvtu32_mask16(u32_22);
    __m512 v512f_24 = _mm512_set1_ps(1.0f);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_8) & 7); i_25++) {
    __m512h v512h_26 = _mm512_add_ph(v512h_4, v512h_21);
    __mmask16 m16_27 = _mm512_fpclass_ps_mask(v512f_24, 0x3);
    }
    __m128 v128f_28 = _mm_fmadd_ps(p1, v128f_16, p1);
    __mmask16 m16_29 = _kxor_mask16(m16_23, m16_7);
    __m512d v512d_30 = g_v512d[4];
    __m512d v512d_31 = _mm512_mul_pd(v512d_30, v512d_30);
    __mmask16 m16_32 = _kxor_mask16(m16_23, m16_29);
    g_v512d[10] = v512d_30;
    g_v256d[5] = p3;
    g_v128f[5] = v128f_16;
    g_v512h[6] = v512h_21;
    g_m64[6] = m64_18;
    return v256i_5;
}

__mmask8 torture_342(__m512i p0, __m512h p1, __m128d p2, __m256 p3, __m512 p4) {
    __m256 v256f_1 = _mm256_min_ps(p3, p3);
    __m512i v512i_2 = _mm512_permutex2var_epi32(p0, p0, p0);
    __mmask32 m32_3 = (__mmask32)(g_m32[4] ^ 76);
    __mmask32 m32_4 = _kxor_mask32(m32_3, m32_3);
    __m256 v256f_5 = _mm256_set1_ps(1.0f);
    __m512h v512h_6 = _mm512_cvtepi16_ph(v512i_2);
    __m128h v128h_7 = g_v128h[5];
    __m512d v512d_8 = _mm512_cvtph_pd(v128h_7);
    __mmask16 m16_9 = _mm512_testn_epi32_mask(v512i_2, v512i_2);
    __m512 v512f_10 = _mm512_sub_ps(p4, p4);
    __m512 v512f_11 = _mm512_roundscale_ps(v512f_10, 0x10);
    __m256 v256f_12 = _mm512_cvtpd_ps(v512d_8);
    __m256 v256f_13 = _mm512_cvtpd_ps(v512d_8);
    __m256i v256i_14 = _mm256_set1_epi32(0x55);
    __m512d v512d_15 = _mm512_insertf64x2(v512d_8, p2, 3);
    __m512 v512f_16 = _mm512_mask_blend_ps(m16_9, v512f_11, v512f_11);
    __m512 v512f_17 = _mm512_rcp14_ps(v512f_11);
    __m256 v256f_18 = _mm256_xor_ps(v256f_12, v256f_1);
    __m512 v512f_19 = _mm512_sub_ps(v512f_11, v512f_16);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m128i v128i_20 = g_v128i[10];
    __m128i v128i_21 = _mm_slli_epi32(v128i_20, 3);
    __m512 v512f_22 = _mm512_add_ps(v512f_16, v512f_19);
    __m512 v512f_23 = _mm512_fmadd_ps(v512f_16, p4, v512f_17);
    }
    __m512d v512d_24 = _mm512_cvtph_pd(v128h_7);
    __mmask8 m8_25 = (__mmask8)(g_m8[9] ^ 165);
    __m512i v512i_26 = _mm512_broadcastmb_epi64(m8_25);
    __m128i v128i_27 = g_v128i[6];
    __m128i v128i_28 = _mm_abs_epi32(v128i_27);
    __m512d v512d_29 = _mm512_min_pd(v512d_24, v512d_8);
    __m512d v512d_30 = _mm512_sqrt_pd(v512d_24);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m512h v512h_31 = _mm512_sqrt_ph(v512h_6);
    __mmask32 m32_32 = _kxor_mask32(m32_3, m32_4);
    __m512d v512d_33 = _mm512_div_pd(v512d_24, v512d_30);
    }
    g_v256i[11] = v256i_14;
    g_v128i[6] = v128i_27;
    g_m16[3] = m16_9;
    g_m32[6] = m32_3;
    return m8_25;
}

__m512h torture_343(__m256d p0, __m512d p1) {
    __m512h v512h_1 = g_v512h[11];
    __m512h v512h_2 = _mm512_fmul_pch(v512h_1, v512h_1);
    __m512i v512i_3 = g_v512i[2];
    __m512i v512i_4 = _mm512_rol_epi32(v512i_3, 5);
    __mmask16 m16_5 = _mm512_test_epi32_mask(v512i_3, v512i_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m128 v128f_7 = _mm512_extractf32x4_ps(v512f_6, 2);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_9 = _mm_sub_epi8(v128i_8, v128i_8);
    __m128i v128i_10 = _mm_mullo_epi32(v128i_8, v128i_8);
    __m512i v512i_11 = _mm512_abs_epi32(v512i_3);
    __mmask16 m16_12 = _mm512_cmp_ps_mask(v512f_6, v512f_6, _CMP_LT_OQ);
    __m256d v256d_13 = _mm256_div_pd(p0, p0);
    __m512i v512i_14 = _mm512_cvtps_epi32(v512f_6);
    __m512d v512d_15 = _mm512_set1_pd(2.0);
    __m512 v512f_16 = _mm512_cvtepi32_ps(v512i_14);
    __m256 v256f_17 = _mm256_loadu_ps(fbuf);
    __m256 v256f_18 = _mm256_xor_ps(v256f_17, v256f_17);
    __mmask32 m32_19 = _mm512_cmp_ph_mask(v512h_2, v512h_1, 1);
    __mmask16 m16_20 = _kor_mask16(m16_5, m16_5);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_20) & 7); i_21++) {
    __mmask8 m8_22 = (__mmask8)(g_m8[6] ^ 209);
    __mmask16 m16_23 = _mm512_kunpackb(m8_22, m8_22);
    __m512d v512d_24 = _mm512_shuffle_f64x2(p1, p1, 0xEE);
    __m512i v512i_25 = _mm512_mullo_epi32(v512i_14, v512i_14);
    __mmask16 m16_26 = _mm512_test_epi32_mask(v512i_11, v512i_25);
    __m512 v512f_27 = _mm512_cvtepi32_ps(v512i_25);
    }
    __m128i v128i_28 = _mm_sub_epi64(v128i_9, v128i_8);
    __m512h v512h_29 = _mm512_sqrt_ph(v512h_2);
    __m256d v256d_30 = _mm256_set1_pd(2.0);
    __m128i v128i_31 = _mm_sub_epi16(v128i_28, v128i_9);
    __m256 v256f_32 = _mm256_fnmadd_ps(v256f_17, v256f_17, v256f_17);
    __m256d v256d_33 = _mm256_div_pd(p0, v256d_30);
    __mmask16 m16_34 = _mm512_cmp_epu32_mask(v512i_14, v512i_4, 4);
    __m512 v512f_35 = _mm512_unpacklo_ps(v512f_16, v512f_6);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_5) & 7); i_36++) {
    __m512i v512i_37 = _mm512_set1_epi32(0x55);
    __m512 v512f_38 = _mm512_maskz_mul_ps(m16_12, v512f_6, v512f_6);
    __m256i v256i_39 = g_v256i[1];
    __m256i v256i_40 = _mm256_sub_epi32(v256i_39, v256i_39);
    __mmask32 m32_41 = _kxor_mask32(m32_19, m32_19);
    }
    g_v512f[8] = v512f_35;
    g_v512i[11] = v512i_14;
    g_v256f[7] = v256f_17;
    g_v512h[3] = v512h_2;
    g_m16[11] = m16_34;
    g_m32[5] = m32_19;
    return v512h_1;
}

__m512h torture_344(__m128 p0, __m128 p1, __m128h p2) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_max_ps(v512f_1, v512f_1);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_slli_epi32(v128i_3, 3);
    __m512h v512h_5 = g_v512h[8];
    __m512h v512h_6 = _mm512_fmadd_ph(v512h_5, v512h_5, v512h_5);
    __m128d v128d_7 = g_v128d[0];
    __m128d v128d_8 = _mm_div_pd(v128d_7, v128d_7);
    __m128i v128i_9 = _mm_mullo_epi32(v128i_3, v128i_3);
    __m512 v512f_10 = _mm512_rsqrt14_ps(v512f_2);
    __m512i v512i_11 = g_v512i[11];
    __m512i v512i_12 = _mm512_dpbusd_epi32(v512i_11, v512i_11, v512i_11);
    __m128i v128i_13 = _mm_xor_si128(v128i_4, v128i_9);
    __m128 v128f_14 = _mm_fmadd_ps(p1, p1, p1);
    __mmask16 m16_15 = (__mmask16)(g_m16[3] ^ 205);
    __mmask16 m16_16 = _kand_mask16(m16_15, m16_15);
    __m512i v512i_17 = _mm512_cvtps_epi32(v512f_1);
    __m256 v256f_18 = g_v256f[5];
    __m256 v256f_19 = _mm256_xor_ps(v256f_18, v256f_18);
    __m256d v256d_20 = g_v256d[7];
    __m256d v256d_21 = _mm256_add_pd(v256d_20, v256d_20);
    __m256i v256i_22 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_23 = _mm256_sllv_epi32(v256i_22, v256i_22);
    __mmask8 m8_24 = (__mmask8)(g_m8[1] ^ 158);
    __m512i v512i_25 = _mm512_broadcastmb_epi64(m8_24);
    __m128 v128f_26 = _mm_sqrt_ps(p1);
    __m128d v128d_27 = _mm_sub_pd(v128d_7, v128d_7);
    __m256i v256i_28 = _mm256_sub_epi32(v256i_23, v256i_23);
    __m256 v256f_29 = _mm256_div_ps(v256f_18, v256f_19);
    __m256d v256d_30 = _mm256_mul_pd(v256d_20, v256d_20);
    __m256i v256i_31 = _mm256_add_epi16(v256i_28, v256i_22);
    __mmask16 m16_32 = _knot_mask16(m16_15);
    __m512i v512i_33 = _mm512_and_si512(v512i_25, v512i_12);
    __m256i v256i_34 = _mm256_abs_epi32(v256i_28);
    __m128d v128d_35 = _mm_sub_pd(v128d_7, v128d_8);
    __m128d v128d_36 = _mm_min_pd(v128d_8, v128d_8);
    __m256 v256f_37 = _mm256_fmadd_ps(v256f_29, v256f_29, v256f_19);
    g_v256d[0] = v256d_20;
    g_v128f[9] = p1;
    g_v128d[7] = v128d_35;
    g_v512h[9] = v512h_5;
    g_v128h[7] = p2;
    g_m8[10] = m8_24;
    return v512h_6;
}

__mmask8 torture_345(__m512 p0, __m128i p1) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_insertf32x8(p0, v256f_1, 1);
    __m256 v256f_3 = _mm256_unpacklo_ps(v256f_1, v256f_1);
    __m512i v512i_4 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_5 = (__mmask16)(g_m16[8] ^ 199);
    __m512i v512i_6 = _mm512_mask_add_epi32(v512i_4, m16_5, v512i_4, v512i_4);
    __m512i v512i_7 = _mm512_ternarylogic_epi32(v512i_4, v512i_6, v512i_4, 0x96);
    __m512d v512d_8 = g_v512d[7];
    __m512d v512d_9 = _mm512_min_pd(v512d_8, v512d_8);
    __m256i v256i_10 = g_v256i[0];
    __m256i v256i_11 = _mm256_mullo_epi32(v256i_10, v256i_10);
    __m128 v128f_12 = _mm_loadu_ps(fbuf);
    __m128 v128f_13 = _mm_add_ps(v128f_12, v128f_12);
    __m512 v512f_14 = _mm512_mask_add_ps(v512f_2, m16_5, v512f_2, p0);
    __m256 v256f_15 = _mm256_max_ps(v256f_3, v256f_1);
    __m512i v512i_16 = _mm512_add_epi8(v512i_4, v512i_7);
    __m512d v512d_17 = _mm512_mul_pd(v512d_8, v512d_9);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_5) & 7); i_18++) {
    __m128i v128i_19 = _mm_sub_epi32(p1, p1);
    __m512i v512i_20 = _mm512_conflict_epi32(v512i_4);
    __m128i v128i_21 = _mm_add_epi64(v128i_19, v128i_19);
    __m512 v512f_22 = _mm512_scalef_ps(v512f_2, v512f_2);
    }
    __m512i v512i_23 = _mm512_rol_epi32(v512i_4, 5);
    __m512h v512h_24 = g_v512h[1];
    __m512h v512h_25 = _mm512_rcp_ph(v512h_24);
    __m128 v128f_26 = _mm_fnmadd_ps(v128f_12, v128f_12, v128f_13);
    __m256d v256d_27 = g_v256d[6];
    __m256d v256d_28 = _mm256_fmadd_pd(v256d_27, v256d_27, v256d_27);
    __m512d v512d_29 = _mm512_sub_pd(v512d_8, v512d_17);
    if (((unsigned long long)m16_5) & 1ULL) {
    __m512i v512i_30 = _mm512_sub_epi64(v512i_7, v512i_23);
    __m512d v512d_31 = _mm512_max_pd(v512d_8, v512d_9);
    __m256i v256i_32 = _mm256_add_epi64(v256i_11, v256i_10);
    __m512i v512i_33 = _mm512_mask_add_epi32(v512i_4, m16_5, v512i_4, v512i_23);
    }
    g_v512d[11] = v512d_29;
    g_v512i[4] = v512i_7;
    g_v256f[7] = v256f_3;
    g_v512h[1] = v512h_24;
    g_m16[7] = m16_5;
    __mmask8 m8_34 = (__mmask8)(g_m8[2] ^ 55);
    return m8_34;
}

__mmask8 torture_346(__m512h p0, __m256i p1, __m512i p2, __m128d p3, __m256h p4) {
    __m512d v512d_1 = g_v512d[3];
    __m512d v512d_2 = _mm512_max_pd(v512d_1, v512d_1);
    __m256d v256d_3 = g_v256d[10];
    __m256d v256d_4 = _mm256_fmadd_pd(v256d_3, v256d_3, v256d_3);
    __m256d v256d_5 = _mm256_sub_pd(v256d_4, v256d_3);
    __m512i v512i_6 = _mm512_permutex2var_epi32(p2, p2, p2);
    __mmask16 m16_7 = _mm512_movepi32_mask(p2);
    __m512i v512i_8 = _mm512_lzcnt_epi32(p2);
    __mmask16 m16_9 = _mm512_cmp_epi32_mask(p2, v512i_8, 2);
    __m256 v256f_10 = _mm256_loadu_ps(fbuf);
    __m256 v256f_11 = _mm256_fmsub_ps(v256f_10, v256f_10, v256f_10);
    __m256d v256d_12 = _mm512_extractf64x4_pd(v512d_1, 1);
    __m512d v512d_13 = _mm512_max_pd(v512d_2, v512d_1);
    __m512 v512f_14 = g_v512f[5];
    __m512 v512f_15 = _mm512_mask3_fmadd_ps(v512f_14, v512f_14, v512f_14, m16_9);
    __m512h v512h_16 = _mm512_getexp_ph(p0);
    g_v512d[10] = v512d_13;
    g_v256f[3] = v256f_11;
    g_v128d[11] = p3;
    g_m16[1] = m16_9;
    __mmask8 m8_17 = (__mmask8)(g_m8[8] ^ 137);
    return m8_17;
}

__m512d torture_347(__m128h p0, __m128d p1, __m512 p2, __m256i p3) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_add_epi64(v512i_1, v512i_1);
    __m256 v256f_3 = _mm256_loadu_ps(fbuf);
    __m256 v256f_4 = _mm256_mul_ps(v256f_3, v256f_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_sub_epi16(v128i_5, v128i_5);
    __m512 v512f_7 = _mm512_fnmadd_ps(p2, p2, p2);
    __m512i v512i_8 = _mm512_set1_epi32(0x55);
    __m512 v512f_9 = _mm512_scalef_ps(p2, p2);
    __m128i v128i_10 = _mm_shuffle_epi8(v128i_5, v128i_6);
    __m512h v512h_11 = g_v512h[9];
    __m512h v512h_12 = _mm512_fmadd_ph(v512h_11, v512h_11, v512h_11);
    __m256d v256d_13 = g_v256d[3];
    __m256d v256d_14 = _mm256_mul_pd(v256d_13, v256d_13);
    __m512i v512i_15 = _mm512_add_epi32(v512i_2, v512i_1);
    __m512i v512i_16 = _mm512_popcnt_epi64(v512i_2);
    int s32_17 = 3;
    if ((s32_17) & 1ULL) {
    __m512i v512i_18 = _mm512_shuffle_i32x4(v512i_8, v512i_16, 0x1B);
    __m512i v512i_19 = _mm512_add_epi16(v512i_15, v512i_16);
    __m512i v512i_20 = _mm512_permutexvar_epi32(v512i_16, v512i_8);
    __m512i v512i_21 = _mm512_broadcast_i32x4(v128i_6);
    }
    __mmask32 m32_22 = _mm512_cmp_ph_mask(v512h_12, v512h_12, 1);
    __m256 v256f_23 = _mm256_add_ps(v256f_3, v256f_4);
    __m512d v512d_24 = _mm512_loadu_pd(dbuf);
    __m512d v512d_25 = _mm512_sqrt_pd(v512d_24);
    __m128d v128d_26 = _mm_sub_pd(p1, p1);
    __m256i v256i_27 = _mm512_cvtepi64_epi32(v512i_1);
    if (((unsigned long long)m32_22) & 1ULL) {
    __m256i v256i_28 = _mm512_cvtepi64_epi32(v512i_16);
    __m512 v512f_29 = _mm512_sub_ps(v512f_7, p2);
    __m256i v256i_30 = _mm256_add_epi32(v256i_27, v256i_27);
    __m512i v512i_31 = _mm512_sub_epi16(v512i_16, v512i_16);
    __m256d v256d_32 = _mm256_max_pd(v256d_13, v256d_13);
    }
    g_v512f[10] = v512f_9;
    g_v512d[1] = v512d_25;
    g_v256d[7] = v256d_14;
    g_v256i[4] = p3;
    g_v128d[1] = v128d_26;
    return v512d_24;
}

__m512 torture_348(__m256 p0, __m512d p1, __m256d p2, __m256h p3) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_2 = _mm512_cvtepi8_epi32(v128i_1);
    __m512i v512i_3 = _mm512_abs_epi32(v512i_2);
    __m512d v512d_4 = _mm512_mul_pd(p1, p1);
    __mmask16 m16_5 = (__mmask16)(g_m16[2] ^ 30);
    __m512i v512i_6 = _mm512_mask_add_epi32(v512i_3, m16_5, v512i_3, v512i_3);
    __m512i v512i_7 = _mm512_slli_epi32(v512i_2, 3);
    __m256 v256f_8 = _mm256_max_ps(p0, p0);
    __m512d v512d_9 = _mm512_sqrt_pd(p1);
    __m128d v128d_10 = g_v128d[1];
    __m128d v128d_11 = _mm_min_pd(v128d_10, v128d_10);
    __m256d v256d_12 = _mm256_sqrt_pd(p2);
    __m512i v512i_13 = _mm512_sllv_epi32(v512i_6, v512i_3);
    __m256i v256i_14 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_15 = _mm256_add_epi32(v256i_14, v256i_14);
    __mmask16 m16_16 = _mm512_cmp_epi32_mask(v512i_13, v512i_3, 2);
    __m512h v512h_17 = g_v512h[10];
    __m512h v512h_18 = _mm512_sqrt_ph(v512h_17);
    __m512i v512i_19 = _mm512_broadcast_i32x4(v128i_1);
    __m512i v512i_20 = _mm512_sllv_epi32(v512i_13, v512i_3);
    __m128d v128d_21 = _mm_max_pd(v128d_11, v128d_10);
    __m128 v128f_22 = _mm_loadu_ps(fbuf);
    __m128 v128f_23 = _mm_fmadd_ps(v128f_22, v128f_22, v128f_22);
    __m512 v512f_24 = g_v512f[6];
    __m256h v256h_25 = _mm512_cvtxps_ph(v512f_24);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m256d v256d_26 = _mm256_add_pd(v256d_12, p2);
    __m512 v512f_27 = _mm512_fmadd_ps(v512f_24, v512f_24, v512f_24);
    __mmask16 m16_28 = _kand_mask16(m16_16, m16_5);
    __m512 v512f_29 = _mm512_shuffle_f32x4(v512f_24, v512f_27, 0x4E);
    __m512 v512f_30 = _mm512_fnmadd_ps(v512f_27, v512f_27, v512f_29);
    }
    __m256i v256i_31 = _mm256_slli_epi32(v256i_14, 3);
    __m512 v512f_32 = _mm512_mask_add_ps(v512f_24, m16_5, v512f_24, v512f_24);
    __m256d v256d_33 = _mm256_set1_pd(2.0);
    __m512i v512i_34 = _mm512_slli_epi32(v512i_2, 3);
    __m512d v512d_35 = _mm512_set1_pd(2.0);
    __m256i v256i_36 = _mm256_xor_si256(v256i_15, v256i_15);
    __m512 v512f_37 = _mm512_shuffle_f32x4(v512f_24, v512f_32, 0x4E);
    __m128 v128f_38 = _mm_mul_ps(v128f_22, v128f_23);
    __m512i v512i_39 = _mm512_permutex2var_epi32(v512i_3, v512i_20, v512i_13);
    g_v256i[10] = v256i_36;
    g_v128d[0] = v128d_11;
    g_v256h[9] = p3;
    g_m16[2] = m16_5;
    return v512f_24;
}

__m256 torture_349(__m256d p0, __m128 p1, __m512i p2) {
    __m128 v128f_1 = _mm_mul_ps(p1, p1);
    __mmask16 m16_2 = (__mmask16)(g_m16[1] ^ 191);
    __m512i v512i_3 = _mm512_mask_add_epi32(p2, m16_2, p2, p2);
    __m512i v512i_4 = _mm512_madd52lo_epu64(v512i_3, p2, p2);
    __m512i v512i_5 = _mm512_rol_epi32(v512i_3, 5);
    __m128i v128i_6 = g_v128i[7];
    __m128i v128i_7 = _mm_add_epi64(v128i_6, v128i_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m256 v256f_9 = _mm256_sqrt_ps(v256f_8);
    __m256i v256i_10 = g_v256i[9];
    __m256i v256i_11 = _mm256_sub_epi32(v256i_10, v256i_10);
    __m128d v128d_12 = g_v128d[7];
    __m128d v128d_13 = _mm_div_pd(v128d_12, v128d_12);
    __m512i v512i_14 = _mm512_conflict_epi32(v512i_3);
    __m512d v512d_15 = g_v512d[2];
    __m512d v512d_16 = _mm512_sqrt_pd(v512d_15);
    __mmask8 m8_17 = (__mmask8)(g_m8[3] ^ 47);
    __m512i v512i_18 = _mm512_broadcastmb_epi64(m8_17);
    __m512i v512i_19 = _mm512_and_si512(v512i_4, v512i_14);
    __m128i v128i_20 = _mm_sub_epi8(v128i_7, v128i_7);
    __m256i v256i_21 = _mm256_sub_epi8(v256i_10, v256i_10);
    __m512i v512i_22 = _mm512_dpbusd_epi32(v512i_18, v512i_19, v512i_18);
    __m256 v256f_23 = _mm256_mul_ps(v256f_8, v256f_8);
    __mmask16 m16_24 = _knot_mask16(m16_2);
    __mmask16 m16_25 = _kxor_mask16(m16_24, m16_24);
    __m256i v256i_26 = _mm256_sub_epi32(v256i_10, v256i_10);
    __m512i v512i_27 = _mm512_conflict_epi32(v512i_4);
    __m512h v512h_28 = g_v512h[4];
    __m512h v512h_29 = _mm512_fmsub_ph(v512h_28, v512h_28, v512h_28);
    __m128i v128i_30 = _mm_sub_epi16(v128i_20, v128i_7);
    __m512 v512f_31 = _mm512_loadu_ps(fbuf);
    __m512 v512f_32 = _mm512_fmsub_ps(v512f_31, v512f_31, v512f_31);
    __m512 v512f_33 = _mm512_fmadd_ps(v512f_31, v512f_31, v512f_32);
    __m128i v128i_34 = _mm_set1_epi32(0x55);
    __mmask16 m16_35 = _mm512_kunpackb(m8_17, m8_17);
    __m512 v512f_36 = _mm512_insertf32x8(v512f_31, v256f_9, 1);
    __m512i v512i_37 = _mm512_dpbusd_epi32(v512i_14, v512i_22, v512i_22);
    __m256 v256f_38 = _mm256_shuffle_ps(v256f_23, v256f_8, 0x1B);
    if (((unsigned long long)m16_25) & 1ULL) {
    __m128h v128h_39 = _mm512_cvtpd_ph(v512d_16);
    __m256 v256f_40 = _mm256_xor_ps(v256f_9, v256f_38);
    __m128i v128i_41 = _mm_set1_epi32(0x55);
    __m128 v128f_42 = _mm_mul_ps(v128f_1, v128f_1);
    __m512h v512h_43 = _mm512_mul_ph(v512h_28, v512h_29);
    }
    g_v512d[0] = v512d_15;
    g_v128d[0] = v128d_12;
    return v256f_23;
}

__mmask16 torture_350(__m256 p0, __m512h p1, __m512 p2, __m128i p3, __m256h p4) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_2 = _mm512_add_epi16(v512i_1, v512i_1);
    __m256h v256h_3 = _mm512_cvtxps_ph(p2);
    __m256 v256f_4 = _mm256_shuffle_ps(p0, p0, 0x1B);
    __m512i v512i_5 = _mm512_broadcast_i32x4(p3);
    __m512h v512h_6 = _mm512_sqrt_ph(p1);
    __m128 v128f_7 = g_v128f[3];
    __m128 v128f_8 = _mm_fmsub_ps(v128f_7, v128f_7, v128f_7);
    __m512h v512h_9 = _mm512_fmsub_ph(p1, v512h_6, p1);
    __m512i v512i_10 = _mm512_add_epi16(v512i_1, v512i_1);
    __m512i v512i_11 = _mm512_slli_epi32(v512i_5, 3);
    __m128 v128f_12 = _mm_mul_ps(v128f_8, v128f_7);
    int s32_13 = 87;
    for (int i_14 = 0; i_14 < (int)((s32_13) & 7); i_14++) {
    __m512 v512f_15 = _mm512_broadcast_f32x4(v128f_8);
    __m512h v512h_16 = _mm512_mul_ph(v512h_9, v512h_9);
    __m512d v512d_17 = g_v512d[10];
    __m256d v256d_18 = _mm512_extractf64x4_pd(v512d_17, 1);
    }
    __m256d v256d_19 = g_v256d[8];
    __m256d v256d_20 = _mm256_fmadd_pd(v256d_19, v256d_19, v256d_19);
    __mmask16 m16_21 = (__mmask16)(g_m16[9] ^ 122);
    __mmask16 m16_22 = _kxor_mask16(m16_21, m16_21);
    __m256i v256i_23 = g_v256i[8];
    __m256i v256i_24 = _mm256_sub_epi8(v256i_23, v256i_23);
    __mmask8 m8_25 = (__mmask8)(g_m8[2] ^ 77);
    __m512d v512d_26 = _mm512_loadu_pd(dbuf);
    __m512d v512d_27 = _mm512_mask_blend_pd(m8_25, v512d_26, v512d_26);
    __m512 v512f_28 = _mm512_cvtepi32_ps(v512i_5);
    __m256d v256d_29 = _mm256_min_pd(v256d_19, v256d_20);
    __m128 v128f_30 = _mm_fnmadd_ps(v128f_8, v128f_8, v128f_12);
    if (((unsigned long long)m16_21) & 1ULL) {
    __m512h v512h_31 = _mm512_fmadd_ph(v512h_6, v512h_6, v512h_9);
    __m128i v128i_32 = _mm_and_si128(p3, p3);
    __m256 v256f_33 = _mm256_sub_ps(p0, p0);
    }
    g_v512f[11] = p2;
    g_v512d[10] = v512d_27;
    g_v512i[9] = v512i_2;
    g_v128i[3] = p3;
    return m16_22;
}

__m256i torture_351(__m512i p0, __m128 p1) {
    __m256d v256d_1 = g_v256d[4];
    __m256d v256d_2 = _mm256_min_pd(v256d_1, v256d_1);
    __m256d v256d_3 = _mm256_div_pd(v256d_2, v256d_1);
    __m512 v512f_4 = g_v512f[6];
    __m512 v512f_5 = _mm512_mul_ps(v512f_4, v512f_4);
    __m128 v128f_6 = _mm_mul_ps(p1, p1);
    __m128d v128d_7 = g_v128d[6];
    __m128d v128d_8 = _mm_sub_pd(v128d_7, v128d_7);
    __m512d v512d_9 = g_v512d[5];
    __m512d v512d_10 = _mm512_sqrt_pd(v512d_9);
    __m128d v128d_11 = _mm_unpackhi_pd(v128d_8, v128d_8);
    __m256 v256f_12 = _mm256_loadu_ps(fbuf);
    __m256 v256f_13 = _mm256_max_ps(v256f_12, v256f_12);
    __m512i v512i_14 = _mm512_shuffle_epi8(p0, p0);
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_16 = _mm512_broadcast_i32x4(v128i_15);
    __m512d v512d_17 = _mm512_max_pd(v512d_9, v512d_10);
    __m512 v512f_18 = _mm512_broadcast_f32x4(p1);
    __m256i v256i_19 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_20 = _mm256_xor_si256(v256i_19, v256i_19);
    __m128 v128f_21 = _mm_xor_ps(v128f_6, v128f_6);
    __m128d v128d_22 = _mm_mul_pd(v128d_8, v128d_8);
    g_v512f[1] = v512f_18;
    g_v512d[7] = v512d_10;
    g_v256f[8] = v256f_13;
    g_v128i[11] = v128i_15;
    return v256i_20;
}

__m128i torture_352(__m128d p0, __m512i p1, __m512 p2, __m512i p3, __m512d p4) {
    __m512i v512i_1 = _mm512_rol_epi32(p3, 5);
    __mmask16 m16_2 = _mm512_cmp_epu32_mask(v512i_1, p1, 4);
    __m512i v512i_3 = _mm512_sub_epi32(p1, p3);
    __m128d v128d_4 = _mm_div_pd(p0, p0);
    __m256 v256f_5 = g_v256f[11];
    __m256 v256f_6 = _mm256_min_ps(v256f_5, v256f_5);
    __m256h v256h_7 = g_v256h[7];
    __m512 v512f_8 = _mm512_cvtxph_ps(v256h_7);
    __m512d v512d_9 = _mm512_sub_pd(p4, p4);
    __m128 v128f_10 = _mm512_extractf32x4_ps(p2, 2);
    __m128d v128d_11 = _mm_min_pd(p0, p0);
    __m256d v256d_12 = g_v256d[10];
    __m256d v256d_13 = _mm256_sqrt_pd(v256d_12);
    __m128i v128i_14 = _mm512_cvtepi32_epi8(v512i_1);
    __m128i v128i_15 = _mm_set1_epi32(0x55);
    __m256 v256f_16 = _mm256_add_ps(v256f_5, v256f_6);
    __m512i v512i_17 = _mm512_mask_blend_epi32(m16_2, p3, v512i_3);
    __mmask8 m8_18 = (__mmask8)(g_m8[0] ^ 199);
    __mmask8 m8_19 = _kand_mask8(m8_18, m8_18);
    __m128i v128i_20 = _mm_and_si128(v128i_15, v128i_15);
    __m128i v128i_21 = _mm_or_si128(v128i_14, v128i_20);
    __m128 v128f_22 = _mm_fmadd_ps(v128f_10, v128f_10, v128f_10);
    g_v512d[6] = p4;
    g_v128f[0] = v128f_10;
    g_v128i[11] = v128i_20;
    return v128i_15;
}

__m512h torture_353(__m128d p0, __m256h p1, __m512h p2, __m128 p3, __m256 p4) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m512d v512d_2 = _mm512_shuffle_f64x2(v512d_1, v512d_1, 0xEE);
    __m128i v128i_3 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_4 = _mm_add_epi64(v128i_3, v128i_3);
    __m128i v128i_5 = _mm_set1_epi32(0x55);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_7 = _mm512_movepi32_mask(v512i_6);
    __m256 v256f_8 = _mm256_fmadd_ps(p4, p4, p4);
    __m256 v256f_9 = _mm256_max_ps(v256f_8, p4);
    __mmask16 m16_10 = _kshiftli_mask16(m16_7, 2);
    __m128i v128i_11 = _mm_mullo_epi32(v128i_4, v128i_5);
    __m512i v512i_12 = _mm512_rolv_epi32(v512i_6, v512i_6);
    __m512d v512d_13 = _mm512_shuffle_f64x2(v512d_2, v512d_2, 0xEE);
    __m512d v512d_14 = _mm512_fmadd_pd(v512d_2, v512d_2, v512d_13);
    __m512h v512h_15 = _mm512_getexp_ph(p2);
    __mmask64 m64_16 = (__mmask64)(g_m64[2] ^ 170);
    __mmask64 m64_17 = _kor_mask64(m64_16, m64_16);
    __m128i v128i_18 = _mm_sub_epi32(v128i_11, v128i_11);
    __m256 v256f_19 = _mm256_sub_ps(p4, p4);
    __m512 v512f_20 = _mm512_set1_ps(1.0f);
    __mmask8 m8_21 = _mm512_cmp_pd_mask(v512d_13, v512d_2, _CMP_GE_OQ);
    __m128i v128i_22 = _mm_sub_epi16(v128i_5, v128i_18);
    if (((unsigned long long)m16_10) & 1ULL) {
    __mmask16 m16_23 = _mm512_cmp_ps_mask(v512f_20, v512f_20, _CMP_LT_OQ);
    __m128i v128i_24 = _mm_add_epi32(v128i_5, v128i_22);
    }
    __m512 v512f_25 = _mm512_insertf32x8(v512f_20, p4, 1);
    __m512i v512i_26 = _mm512_unpacklo_epi32(v512i_6, v512i_6);
    __m512i v512i_27 = _mm512_shuffle_epi8(v512i_6, v512i_26);
    __m128 v128f_28 = _mm_shuffle_ps(p3, p3, 0x1B);
    __m256d v256d_29 = g_v256d[10];
    __m256d v256d_30 = _mm256_sub_pd(v256d_29, v256d_29);
    __m128d v128d_31 = _mm_add_pd(p0, p0);
    __m256i v256i_32 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_33 = _mm256_xor_si256(v256i_32, v256i_32);
    __m512i v512i_34 = _mm512_sub_epi8(v512i_27, v512i_26);
    __m512i v512i_35 = _mm512_sllv_epi32(v512i_12, v512i_12);
    __m512d v512d_36 = _mm512_unpackhi_pd(v512d_2, v512d_1);
    __m512 v512f_37 = _mm512_fnmadd_ps(v512f_20, v512f_25, v512f_25);
    __m512d v512d_38 = _mm512_mul_pd(v512d_1, v512d_1);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_10) & 7); i_39++) {
    __m256 v256f_40 = _mm256_div_ps(v256f_19, v256f_9);
    __m512i v512i_41 = _mm512_permutex2var_epi32(v512i_26, v512i_34, v512i_34);
    __m256i v256i_42 = _mm256_unpacklo_epi32(v256i_33, v256i_33);
    __mmask16 m16_43 = _mm512_cmp_epi32_mask(v512i_35, v512i_12, 2);
    __m512i v512i_44 = _mm512_dpwssd_epi32(v512i_34, v512i_34, v512i_12);
    }
    g_v512d[6] = v512d_36;
    g_v128d[3] = v128d_31;
    g_m8[9] = m8_21;
    g_m64[9] = m64_16;
    return v512h_15;
}

__m512d torture_354(__m256 p0, __m128 p1, __m256 p2, __m128d p3, __m256h p4) {
    __m128i v128i_1 = g_v128i[8];
    __m512i v512i_2 = _mm512_cvtepi8_epi32(v128i_1);
    __m128i v128i_3 = _mm_slli_epi32(v128i_1, 3);
    __m128 v128f_4 = _mm_xor_ps(p1, p1);
    __m512i v512i_5 = _mm512_lzcnt_epi32(v512i_2);
    __m512i v512i_6 = _mm512_sub_epi8(v512i_2, v512i_2);
    __m256d v256d_7 = g_v256d[9];
    __m256d v256d_8 = _mm256_sqrt_pd(v256d_7);
    __m256i v256i_9 = g_v256i[8];
    __m256i v256i_10 = _mm256_sub_epi8(v256i_9, v256i_9);
    __m128d v128d_11 = _mm_max_pd(p3, p3);
    __m256 v256f_12 = _mm256_xor_ps(p0, p0);
    __m256d v256d_13 = _mm256_mul_pd(v256d_7, v256d_7);
    __m256 v256f_14 = _mm256_xor_ps(v256f_12, p2);
    __mmask16 m16_15 = (__mmask16)(g_m16[0] ^ 225);
    __m512i v512i_16 = _mm512_broadcastmw_epi32(m16_15);
    __m512 v512f_17 = g_v512f[10];
    __m512 v512f_18 = _mm512_fmadd_ps(v512f_17, v512f_17, v512f_17);
    __m128i v128i_19 = _mm_xor_si128(v128i_3, v128i_1);
    __m512i v512i_20 = _mm512_and_si512(v512i_6, v512i_16);
    __m256d v256d_21 = _mm256_set1_pd(2.0);
    __m128i v128i_22 = _mm_mullo_epi32(v128i_3, v128i_3);
    __m256d v256d_23 = _mm256_min_pd(v256d_7, v256d_8);
    __m128i v128i_24 = _mm_shuffle_epi8(v128i_1, v128i_22);
    __mmask64 m64_25 = _mm512_cmp_epi8_mask(v512i_2, v512i_16, 1);
    __mmask16 m16_26 = _mm512_cmp_epi32_mask(v512i_2, v512i_6, 2);
    __m512i v512i_27 = _mm512_mask_blend_epi32(m16_15, v512i_2, v512i_20);
    for (int i_28 = 0; i_28 < (int)(((unsigned long long)m16_26) & 7); i_28++) {
    __m512 v512f_29 = _mm512_shuffle_f32x4(v512f_18, v512f_17, 0x4E);
    __m256 v256f_30 = _mm256_fmadd_ps(p2, p0, v256f_12);
    __m512 v512f_31 = _mm512_sqrt_ps(v512f_29);
    __m128d v128d_32 = _mm_div_pd(p3, v128d_11);
    }
    g_v512f[0] = v512f_18;
    g_v512i[7] = v512i_2;
    g_v256i[2] = v256i_10;
    g_v128i[6] = v128i_22;
    g_m64[8] = m64_25;
    __m512d v512d_33 = _mm512_loadu_pd(dbuf);
    return v512d_33;
}

__m256d torture_355(__m256 p0, __m512i p1) {
    __mmask16 m16_1 = (__mmask16)(g_m16[8] ^ 116);
    __m512i v512i_2 = _mm512_mask_add_epi32(p1, m16_1, p1, p1);
    __m128i v128i_3 = g_v128i[5];
    __m128i v128i_4 = _mm_sub_epi32(v128i_3, v128i_3);
    __m512d v512d_5 = g_v512d[6];
    __m512i v512i_6 = _mm512_cvtpd_epi64(v512d_5);
    __m512i v512i_7 = _mm512_add_epi64(v512i_6, v512i_6);
    __m256i v256i_8 = _mm512_cvtepi64_epi32(p1);
    __m512i v512i_9 = _mm512_rol_epi32(v512i_2, 5);
    __m512h v512h_10 = g_v512h[6];
    __m512h v512h_11 = _mm512_fmul_pch(v512h_10, v512h_10);
    __mmask32 m32_12 = _mm512_kunpackw(m16_1, m16_1);
    __m256i v256i_13 = _mm256_add_epi8(v256i_8, v256i_8);
    __mmask8 m8_14 = _mm512_cmp_pd_mask(v512d_5, v512d_5, _CMP_GE_OQ);
    __m128 v128f_15 = _mm_loadu_ps(fbuf);
    __m128 v128f_16 = _mm_sqrt_ps(v128f_15);
    __m512 v512f_17 = _mm512_cvtepi32_ps(v512i_9);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_1) & 7); i_18++) {
    __m512i v512i_19 = _mm512_shuffle_epi8(p1, v512i_9);
    __m128 v128f_20 = _mm_sqrt_ps(v128f_16);
    __m256 v256f_21 = _mm256_unpacklo_ps(p0, p0);
    __mmask16 m16_22 = _mm512_testn_epi32_mask(v512i_7, v512i_2);
    __m512i v512i_23 = _mm512_popcnt_epi64(v512i_7);
    }
    __m512h v512h_24 = _mm512_fmadd_ph(v512h_11, v512h_10, v512h_11);
    __m256 v256f_25 = _mm256_xor_ps(p0, p0);
    __m256i v256i_26 = _mm256_shuffle_epi8(v256i_13, v256i_8);
    __m512i v512i_27 = _mm512_sub_epi32(p1, v512i_2);
    __mmask16 m16_28 = _kshiftri_mask16(m16_1, 1);
    __mmask16 m16_29 = _mm512_cmp_ps_mask(v512f_17, v512f_17, _CMP_LT_OQ);
    __m128i v128i_30 = _mm512_cvtepi32_epi8(v512i_7);
    __m512 v512f_31 = _mm512_fmadd_ps(v512f_17, v512f_17, v512f_17);
    __m512i v512i_32 = _mm512_cvtps_epi32(v512f_17);
    __m512i v512i_33 = _mm512_broadcastmw_epi32(m16_29);
    __m128d v128d_34 = g_v128d[3];
    __m128d v128d_35 = _mm_div_pd(v128d_34, v128d_34);
    __m512d v512d_36 = _mm512_set1_pd(2.0);
    g_v512f[1] = v512f_31;
    g_v128i[3] = v128i_4;
    g_m32[6] = m32_12;
    __m256d v256d_37 = g_v256d[2];
    return v256d_37;
}

__m512d torture_356(__m512d p0, __m256d p1, __m256h p2, __m256 p3, __m128 p4) {
    __m256d v256d_1 = _mm256_max_pd(p1, p1);
    __m128h v128h_2 = g_v128h[11];
    __m512d v512d_3 = _mm512_cvtph_pd(v128h_2);
    __m512h v512h_4 = g_v512h[2];
    __m512h v512h_5 = _mm512_fmul_pch(v512h_4, v512h_4);
    __m256i v256i_6 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_7 = _mm256_mullo_epi32(v256i_6, v256i_6);
    __m512i v512i_8 = g_v512i[8];
    __m512i v512i_9 = _mm512_conflict_epi32(v512i_8);
    __mmask8 m8_10 = (__mmask8)(g_m8[7] ^ 68);
    __mmask16 m16_11 = _mm512_kunpackb(m8_10, m8_10);
    __m512 v512f_12 = g_v512f[3];
    __m512 v512f_13 = _mm512_max_ps(v512f_12, v512f_12);
    __m256 v256f_14 = _mm256_div_ps(p3, p3);
    __m128i v128i_15 = g_v128i[9];
    __m128i v128i_16 = _mm_xor_si128(v128i_15, v128i_15);
    __m512 v512f_17 = _mm512_set1_ps(1.0f);
    __m128d v128d_18 = g_v128d[10];
    __m128d v128d_19 = _mm_sqrt_pd(v128d_18);
    __mmask64 m64_20 = _mm512_cmp_epi8_mask(v512i_8, v512i_8, 1);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_11) & 7); i_21++) {
    __m256i v256i_22 = _mm256_sub_epi64(v256i_6, v256i_6);
    __m256i v256i_23 = _mm256_slli_epi32(v256i_7, 3);
    }
    g_v512f[10] = v512f_13;
    g_v256i[0] = v256i_7;
    g_v128d[0] = v128d_19;
    g_v512h[4] = v512h_4;
    g_m8[0] = m8_10;
    return v512d_3;
}

__m512d torture_357(__m256i p0, __m256i p1, __m128i p2) {
    __m128i v128i_1 = _mm_xor_si128(p2, p2);
    __m128 v128f_2 = _mm_loadu_ps(fbuf);
    __m128 v128f_3 = _mm_max_ps(v128f_2, v128f_2);
    __m128i v128i_4 = _mm_unpacklo_epi32(v128i_1, p2);
    __m128d v128d_5 = g_v128d[10];
    __m128d v128d_6 = _mm_sqrt_pd(v128d_5);
    __m512 v512f_7 = _mm512_loadu_ps(fbuf);
    __m512 v512f_8 = _mm512_rsqrt14_ps(v512f_7);
    __m256h v256h_9 = _mm512_cvtxps_ph(v512f_8);
    __mmask16 m16_10 = (__mmask16)(g_m16[11] ^ 125);
    __mmask16 m16_11 = _kxor_mask16(m16_10, m16_10);
    __mmask8 m8_12 = (__mmask8)(g_m8[9] ^ 232);
    __mmask8 m8_13 = _kand_mask8(m8_12, m8_12);
    unsigned u32_14 = _cvtmask16_u32(m16_11);
    __m128i v128i_15 = _mm_sub_epi64(v128i_4, v128i_4);
    __m512 v512f_16 = _mm512_add_ps(v512f_8, v512f_8);
    __mmask16 m16_17 = _kshiftli_mask16(m16_10, 2);
    __m512i v512i_18 = _mm512_broadcastmw_epi32(m16_11);
    __m256 v256f_19 = g_v256f[6];
    __m512 v512f_20 = _mm512_insertf32x8(v512f_16, v256f_19, 1);
    __m128i v128i_21 = _mm_add_epi64(p2, v128i_15);
    __m256 v256f_22 = _mm256_unpacklo_ps(v256f_19, v256f_19);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m256i v256i_23 = _mm256_sub_epi16(p0, p1);
    __m256d v256d_24 = g_v256d[0];
    __m256d v256d_25 = _mm256_div_pd(v256d_24, v256d_24);
    __m512 v512f_26 = _mm512_broadcast_f32x4(v128f_3);
    __mmask16 m16_27 = _mm512_cmp_epu32_mask(v512i_18, v512i_18, 4);
    }
    __m128 v128f_28 = _mm_set1_ps(1.0f);
    __m128i v128i_29 = _mm_add_epi16(v128i_21, v128i_1);
    __m512i v512i_30 = _mm512_sub_epi32(v512i_18, v512i_18);
    __m128i v128i_31 = _mm_sub_epi64(v128i_15, p2);
    __m512 v512f_32 = _mm512_fmadd_ps(v512f_20, v512f_16, v512f_20);
    __m512i v512i_33 = _mm512_sub_epi64(v512i_18, v512i_30);
    __m128 v128f_34 = _mm_div_ps(v128f_2, v128f_3);
    __m128 v128f_35 = _mm_xor_ps(v128f_34, v128f_3);
    __m128d v128d_36 = _mm_min_pd(v128d_6, v128d_6);
    g_v128i[4] = v128i_1;
    g_v256h[7] = v256h_9;
    __m512d v512d_37 = _mm512_loadu_pd(dbuf);
    return v512d_37;
}

__m256h torture_358(__m128d p0, __m128 p1, __m256 p2) {
    __mmask16 m16_1 = (__mmask16)(g_m16[8] ^ 127);
    __mmask16 m16_2 = _kshiftri_mask16(m16_1, 1);
    __m512i v512i_3 = g_v512i[1];
    __m512i v512i_4 = _mm512_mask_blend_epi32(m16_1, v512i_3, v512i_3);
    __m128i v128i_5 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_6 = _mm_and_si128(v128i_5, v128i_5);
    __m512 v512f_7 = _mm512_cvtepi32_ps(v512i_3);
    __m256i v256i_8 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_9 = _mm256_add_epi32(v256i_8, v256i_8);
    __m512i v512i_10 = _mm512_shuffle_i32x4(v512i_3, v512i_3, 0x1B);
    __m128i v128i_11 = _mm512_cvtepi32_epi8(v512i_4);
    __m512i v512i_12 = _mm512_popcnt_epi64(v512i_4);
    __m512h v512h_13 = g_v512h[8];
    __m512i v512i_14 = _mm512_cvtph_epi16(v512h_13);
    __m512i v512i_15 = _mm512_conflict_epi32(v512i_12);
    __m256h v256h_16 = g_v256h[7];
    __m512 v512f_17 = _mm512_cvtxph_ps(v256h_16);
    __m512 v512f_18 = _mm512_cvtxph_ps(v256h_16);
    __m256 v256f_19 = _mm256_fmsub_ps(p2, p2, p2);
    __mmask16 m16_20 = _knot_mask16(m16_2);
    __m128 v128f_21 = _mm_sqrt_ps(p1);
    __m512h v512h_22 = _mm512_add_ph(v512h_13, v512h_13);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_20) & 7); i_23++) {
    __mmask16 m16_24 = _kor_mask16(m16_1, m16_2);
    __m256d v256d_25 = _mm256_set1_pd(2.0);
    __m128i v128i_26 = _mm_add_epi8(v128i_5, v128i_11);
    __m512 v512f_27 = _mm512_getexp_ps(v512f_18);
    }
    g_v512i[6] = v512i_14;
    g_v256f[0] = v256f_19;
    g_v128i[3] = v128i_11;
    g_v512h[9] = v512h_13;
    g_m16[1] = m16_1;
    return v256h_16;
}

__m512h torture_359(__m128i p0, __m128h p1, __m512h p2) {
    __m256 v256f_1 = g_v256f[1];
    __m256 v256f_2 = _mm256_and_ps(v256f_1, v256f_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_permutexvar_epi32(v512i_3, v512i_3);
    __m512h v512h_5 = _mm512_sqrt_ph(p2);
    __m256 v256f_6 = _mm256_and_ps(v256f_1, v256f_2);
    __m128i v128i_7 = _mm_or_si128(p0, p0);
    __m128d v128d_8 = g_v128d[3];
    __m128d v128d_9 = _mm_fmadd_pd(v128d_8, v128d_8, v128d_8);
    __m512i v512i_10 = _mm512_ternarylogic_epi32(v512i_4, v512i_3, v512i_4, 0x96);
    __m512 v512f_11 = g_v512f[2];
    __m512 v512f_12 = _mm512_fnmadd_ps(v512f_11, v512f_11, v512f_11);
    __m256 v256f_13 = _mm256_sub_ps(v256f_2, v256f_1);
    int s32_14 = 29;
    for (int i_15 = 0; i_15 < (int)((s32_14) & 7); i_15++) {
    __m512i v512i_16 = _mm512_permutex2var_epi32(v512i_10, v512i_4, v512i_4);
    __m256 v256f_17 = _mm256_max_ps(v256f_1, v256f_2);
    __m512i v512i_18 = _mm512_rolv_epi32(v512i_3, v512i_3);
    __m512d v512d_19 = _mm512_set1_pd(2.0);
    }
    __m512i v512i_20 = _mm512_sub_epi32(v512i_3, v512i_10);
    __m256 v256f_21 = _mm256_div_ps(v256f_13, v256f_6);
    __m512 v512f_22 = _mm512_cvtepi32_ps(v512i_10);
    __m512 v512f_23 = _mm512_rcp14_ps(v512f_22);
    __m128i v128i_24 = _mm_mullo_epi32(v128i_7, p0);
    __m512 v512f_25 = _mm512_div_ps(v512f_22, v512f_23);
    __m128i v128i_26 = _mm_add_epi64(v128i_24, p0);
    __m128d v128d_27 = _mm_fmadd_pd(v128d_8, v128d_9, v128d_9);
    __m512 v512f_28 = _mm512_and_ps(v512f_12, v512f_25);
    if ((s32_14) & 1ULL) {
    __m512 v512f_29 = _mm512_set1_ps(1.0f);
    __m256i v256i_30 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_31 = _mm256_sub_epi8(v256i_30, v256i_30);
    __m512 v512f_32 = _mm512_set1_ps(1.0f);
    __m128i v128i_33 = _mm_sub_epi64(v128i_24, v128i_24);
    __m128i v128i_34 = _mm_or_si128(v128i_7, p0);
    }
    g_v512f[3] = v512f_28;
    g_v256f[5] = v256f_13;
    g_v128d[9] = v128d_8;
    g_v128i[8] = p0;
    g_v128h[4] = p1;
    return v512h_5;
}

__m512d torture_360(__m512 p0, __m256h p1, __m128i p2, __m128 p3) {
    __m512i v512i_1 = g_v512i[10];
    __m512i v512i_2 = _mm512_madd52lo_epu64(v512i_1, v512i_1, v512i_1);
    __m128 v128f_3 = _mm_fnmadd_ps(p3, p3, p3);
    __m512i v512i_4 = _mm512_sub_epi64(v512i_2, v512i_1);
    __m256i v256i_5 = g_v256i[3];
    __m256i v256i_6 = _mm256_add_epi16(v256i_5, v256i_5);
    __mmask16 m16_7 = _mm512_test_epi32_mask(v512i_2, v512i_1);
    __m256i v256i_8 = _mm256_add_epi8(v256i_5, v256i_6);
    __m512 v512f_9 = _mm512_fmsub_ps(p0, p0, p0);
    __m512h v512h_10 = g_v512h[7];
    __m512h v512h_11 = _mm512_fmsub_ph(v512h_10, v512h_10, v512h_10);
    __mmask32 m32_12 = _mm512_kunpackw(m16_7, m16_7);
    __m512d v512d_13 = g_v512d[5];
    __m512d v512d_14 = _mm512_shuffle_f64x2(v512d_13, v512d_13, 0xEE);
    __m512i v512i_15 = _mm512_sub_epi64(v512i_1, v512i_2);
    __m512 v512f_16 = _mm512_mask3_fmadd_ps(v512f_9, v512f_9, v512f_9, m16_7);
    __m256d v256d_17 = g_v256d[10];
    __m256d v256d_18 = _mm256_fmadd_pd(v256d_17, v256d_17, v256d_17);
    __m512i v512i_19 = _mm512_add_epi8(v512i_15, v512i_15);
    __m128i v128i_20 = _mm_and_si128(p2, p2);
    __m256 v256f_21 = _mm256_loadu_ps(fbuf);
    __m256 v256f_22 = _mm256_div_ps(v256f_21, v256f_21);
    for (int i_23 = 0; i_23 < (int)(((unsigned long long)m16_7) & 7); i_23++) {
    __m512i v512i_24 = _mm512_cvtpd_epi64(v512d_13);
    __m512d v512d_25 = _mm512_cvtps_pd(v256f_21);
    __m512i v512i_26 = _mm512_slli_epi32(v512i_19, 3);
    }
    __m512 v512f_27 = _mm512_shuffle_ps(p0, p0, 0x1B);
    __m512i v512i_28 = _mm512_rolv_epi32(v512i_2, v512i_4);
    __mmask8 m8_29 = (__mmask8)(g_m8[1] ^ 190);
    __mmask8 m8_30 = _kand_mask8(m8_29, m8_29);
    __m256i v256i_31 = _mm256_unpacklo_epi32(v256i_8, v256i_6);
    __m512i v512i_32 = _mm512_sub_epi32(v512i_4, v512i_1);
    __m256 v256f_33 = _mm512_cvtpd_ps(v512d_13);
    __m512i v512i_34 = _mm512_shuffle_i32x4(v512i_1, v512i_19, 0x1B);
    __m512 v512f_35 = _mm512_sub_ps(p0, v512f_27);
    __m256i v256i_36 = _mm256_add_epi16(v256i_6, v256i_5);
    __m512d v512d_37 = _mm512_unpackhi_pd(v512d_14, v512d_13);
    __m512 v512f_38 = _mm512_rcp14_ps(p0);
    __m256d v256d_39 = _mm256_max_pd(v256d_17, v256d_17);
    g_v512d[0] = v512d_13;
    g_v256f[8] = v256f_33;
    g_v256d[6] = v256d_39;
    g_v128f[0] = v128f_3;
    g_v256h[6] = p1;
    g_m16[0] = m16_7;
    g_m32[9] = m32_12;
    return v512d_14;
}

__m128h torture_361(__m512 p0, __m128d p1) {
    __m256i v256i_1 = g_v256i[0];
    __m256i v256i_2 = _mm256_add_epi16(v256i_1, v256i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_madd52lo_epu64(v512i_3, v512i_3, v512i_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[3] ^ 30);
    __mmask16 m16_6 = _kor_mask16(m16_5, m16_5);
    __m256d v256d_7 = _mm256_set1_pd(2.0);
    __m256i v256i_8 = _mm256_sllv_epi32(v256i_1, v256i_2);
    __m512 v512f_9 = _mm512_shuffle_f32x4(p0, p0, 0x4E);
    __m128i v128i_10 = g_v128i[2];
    __m128i v128i_11 = _mm_sllv_epi32(v128i_10, v128i_10);
    __m512 v512f_12 = _mm512_sub_ps(p0, p0);
    __m128i v128i_13 = _mm_set1_epi32(0x55);
    __m256 v256f_14 = _mm256_loadu_ps(fbuf);
    __m256 v256f_15 = _mm256_fmsub_ps(v256f_14, v256f_14, v256f_14);
    __m512 v512f_16 = _mm512_mul_ps(v512f_9, v512f_12);
    __m256d v256d_17 = _mm256_fmadd_pd(v256d_7, v256d_7, v256d_7);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_5) & 7); i_18++) {
    __m512i v512i_19 = _mm512_rol_epi32(v512i_3, 5);
    __m256 v256f_20 = _mm256_xor_ps(v256f_14, v256f_14);
    __m256i v256i_21 = _mm256_add_epi16(v256i_2, v256i_1);
    __m512i v512i_22 = _mm512_xor_si512(v512i_19, v512i_3);
    }
    __m128i v128i_23 = _mm_xor_si128(v128i_11, v128i_13);
    __m512i v512i_24 = _mm512_set1_epi32(0x55);
    __m256 v256f_25 = _mm256_sub_ps(v256f_14, v256f_14);
    __m512i v512i_26 = _mm512_ternarylogic_epi32(v512i_24, v512i_24, v512i_24, 0x96);
    __m512d v512d_27 = g_v512d[8];
    __m512d v512d_28 = _mm512_min_pd(v512d_27, v512d_27);
    __m128i v128i_29 = _mm_sllv_epi32(v128i_10, v128i_13);
    __m512h v512h_30 = g_v512h[3];
    __m512h v512h_31 = _mm512_add_ph(v512h_30, v512h_30);
    __m512 v512f_32 = _mm512_insertf32x8(p0, v256f_14, 1);
    __mmask16 m16_33 = _mm512_cmp_epu32_mask(v512i_4, v512i_24, 4);
    __m512d v512d_34 = _mm512_insertf64x2(v512d_28, p1, 3);
    for (int i_35 = 0; i_35 < (int)(((unsigned long long)m16_5) & 7); i_35++) {
    __m512 v512f_36 = _mm512_rcp14_ps(v512f_32);
    __m128 v128f_37 = g_v128f[3];
    __m128 v128f_38 = _mm_sub_ps(v128f_37, v128f_37);
    __m512d v512d_39 = _mm512_cvtps_pd(v256f_14);
    }
    g_v512d[3] = v512d_27;
    g_v256d[8] = v256d_7;
    g_v512h[10] = v512h_30;
    __m128h v128h_40 = g_v128h[11];
    return v128h_40;
}

__mmask64 torture_362(__m128i p0, __m512h p1, __m128 p2, __m512h p3, __m256h p4) {
    __m128h v128h_1 = g_v128h[5];
    __m512d v512d_2 = _mm512_cvtph_pd(v128h_1);
    __m256i v256i_3 = g_v256i[11];
    __m256i v256i_4 = _mm256_or_si256(v256i_3, v256i_3);
    __m512h v512h_5 = _mm512_getexp_ph(p1);
    __m512 v512f_6 = _mm512_set1_ps(1.0f);
    __m512 v512f_7 = _mm512_broadcast_f32x4(p2);
    __m512 v512f_8 = _mm512_fmsub_ps(v512f_6, v512f_7, v512f_7);
    __m256h v256h_9 = _mm512_cvtxps_ph(v512f_7);
    __m256 v256f_10 = _mm256_loadu_ps(fbuf);
    __m256 v256f_11 = _mm256_fmadd_ps(v256f_10, v256f_10, v256f_10);
    __m128 v128f_12 = _mm_max_ps(p2, p2);
    unsigned u32_13 = 40;
    __mmask16 m16_14 = _cvtu32_mask16(u32_13);
    __m512i v512i_15 = g_v512i[2];
    __m512i v512i_16 = _mm512_and_si512(v512i_15, v512i_15);
    __m256 v256f_17 = _mm512_cvtpd_ps(v512d_2);
    __m256 v256f_18 = _mm256_xor_ps(v256f_10, v256f_17);
    __mmask8 m8_19 = (__mmask8)(g_m8[6] ^ 6);
    __mmask8 m8_20 = _kand_mask8(m8_19, m8_19);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_14) & 7); i_21++) {
    __m512 v512f_22 = _mm512_sub_ps(v512f_7, v512f_7);
    __m512d v512d_23 = _mm512_set1_pd(2.0);
    __m256 v256f_24 = _mm256_max_ps(v256f_11, v256f_11);
    }
    __mmask16 m16_25 = _mm512_kunpackb(m8_20, m8_19);
    __m512i v512i_26 = _mm512_sllv_epi32(v512i_16, v512i_15);
    __m512 v512f_27 = _mm512_insertf32x8(v512f_8, v256f_17, 1);
    __m512i v512i_28 = _mm512_and_si512(v512i_16, v512i_16);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512i v512i_29 = _mm512_xor_si512(v512i_16, v512i_26);
    __m512 v512f_30 = _mm512_max_ps(v512f_27, v512f_7);
    __m512h v512h_31 = _mm512_mul_ph(p1, p1);
    __m256i v256i_32 = _mm256_slli_epi32(v256i_3, 3);
    }
    g_v512f[3] = v512f_8;
    g_v512d[7] = v512d_2;
    g_v512i[2] = v512i_16;
    g_v256i[3] = v256i_3;
    g_v128f[2] = v128f_12;
    g_v128h[11] = v128h_1;
    g_m8[11] = m8_20;
    __mmask64 m64_33 = (__mmask64)(g_m64[2] ^ 90);
    return m64_33;
}

__m128 torture_363(__m256d p0, __m512i p1, __m256d p2, __m128h p3, __m256 p4) {
    __m128i v128i_1 = g_v128i[1];
    __m128i v128i_2 = _mm_add_epi8(v128i_1, v128i_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[8] ^ 116);
    __m512i v512i_4 = _mm512_broadcastmw_epi32(m16_3);
    __m512i v512i_5 = _mm512_mask_add_epi32(p1, m16_3, v512i_4, v512i_4);
    __m512i v512i_6 = _mm512_permutex2var_epi32(v512i_5, v512i_4, v512i_4);
    __m512i v512i_7 = _mm512_add_epi8(p1, v512i_4);
    __m512 v512f_8 = g_v512f[10];
    __m512 v512f_9 = _mm512_min_ps(v512f_8, v512f_8);
    __mmask32 m32_10 = (__mmask32)(g_m32[5] ^ 76);
    __mmask32 m32_11 = _kxor_mask32(m32_10, m32_10);
    __m512d v512d_12 = _mm512_loadu_pd(dbuf);
    __m512d v512d_13 = _mm512_sub_pd(v512d_12, v512d_12);
    __m512 v512f_14 = _mm512_mask_add_ps(v512f_9, m16_3, v512f_8, v512f_8);
    __m256i v256i_15 = g_v256i[5];
    __m256i v256i_16 = _mm256_and_si256(v256i_15, v256i_15);
    __m512 v512f_17 = _mm512_mul_ps(v512f_9, v512f_9);
    __m128 v128f_18 = _mm512_extractf32x4_ps(v512f_17, 2);
    __m512d v512d_19 = _mm512_set1_pd(2.0);
    for (int i_20 = 0; i_20 < (int)(((unsigned long long)m16_3) & 7); i_20++) {
    __m512i v512i_21 = _mm512_sub_epi32(v512i_7, p1);
    __m256i v256i_22 = _mm256_sub_epi64(v256i_15, v256i_15);
    __mmask16 m16_23 = _kshiftri_mask16(m16_3, 1);
    }
    __mmask8 m8_24 = (__mmask8)(g_m8[7] ^ 225);
    __m512d v512d_25 = _mm512_mask_blend_pd(m8_24, v512d_19, v512d_13);
    __mmask8 m8_26 = _mm512_cmp_pd_mask(v512d_25, v512d_25, _CMP_GE_OQ);
    __m256 v256f_27 = _mm256_xor_ps(p4, p4);
    __mmask16 m16_28 = _mm512_cmp_ps_mask(v512f_8, v512f_9, _CMP_LT_OQ);
    __m128i v128i_29 = _mm_add_epi64(v128i_2, v128i_1);
    __m256i v256i_30 = _mm256_sub_epi64(v256i_16, v256i_15);
    __m256 v256f_31 = _mm256_sqrt_ps(v256f_27);
    __m256i v256i_32 = _mm256_sub_epi16(v256i_16, v256i_16);
    __m512 v512f_33 = _mm512_roundscale_ps(v512f_9, 0x10);
    __m512d v512d_34 = _mm512_cvtph_pd(p3);
    __m512i v512i_35 = _mm512_ternarylogic_epi32(v512i_4, v512i_4, v512i_6, 0x96);
    g_v512f[1] = v512f_17;
    return v128f_18;
}

__m128i torture_364(__m512 p0, __m512i p1, __m128 p2, __m256d p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[2] ^ 195);
    __mmask16 m16_2 = _kxor_mask16(m16_1, m16_1);
    __m512d v512d_3 = _mm512_loadu_pd(dbuf);
    __m512d v512d_4 = _mm512_add_pd(v512d_3, v512d_3);
    __m512i v512i_5 = _mm512_add_epi64(p1, p1);
    __m128d v128d_6 = g_v128d[6];
    __m128d v128d_7 = _mm_unpackhi_pd(v128d_6, v128d_6);
    __m512 v512f_8 = _mm512_sub_ps(p0, p0);
    __m512 v512f_9 = _mm512_rsqrt14_ps(p0);
    __mmask64 m64_10 = _mm512_cmp_epi8_mask(p1, v512i_5, 1);
    __m128h v128h_11 = g_v128h[2];
    __m512d v512d_12 = _mm512_cvtph_pd(v128h_11);
    __m512 v512f_13 = _mm512_scalef_ps(v512f_9, v512f_8);
    __m512 v512f_14 = _mm512_fnmadd_ps(v512f_9, v512f_9, v512f_13);
    __m256d v256d_15 = _mm256_mul_pd(p3, p3);
    __m256i v256i_16 = g_v256i[2];
    __m256i v256i_17 = _mm256_unpacklo_epi32(v256i_16, v256i_16);
    __m128 v128f_18 = _mm_add_ps(p2, p2);
    __m128i v128i_19 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_20 = _mm_slli_epi32(v128i_19, 3);
    __m512 v512f_21 = _mm512_rsqrt14_ps(v512f_13);
    __m512 v512f_22 = _mm512_sub_ps(v512f_9, v512f_9);
    __m512 v512f_23 = _mm512_rcp14_ps(v512f_9);
    __m512i v512i_24 = _mm512_sllv_epi32(p1, p1);
    __m512i v512i_25 = _mm512_and_si512(v512i_24, v512i_24);
    __m512i v512i_26 = _mm512_rolv_epi32(v512i_25, v512i_25);
    __m128 v128f_27 = _mm_min_ps(v128f_18, v128f_18);
    __m128 v128f_28 = _mm_fmsub_ps(v128f_27, v128f_27, p2);
    __m512i v512i_29 = _mm512_abs_epi32(v512i_26);
    __m512i v512i_30 = _mm512_broadcastmw_epi32(m16_2);
    __m128 v128f_31 = _mm_fmadd_ps(v128f_28, v128f_27, v128f_18);
    __m512i v512i_32 = _mm512_ternarylogic_epi32(v512i_24, v512i_30, p1, 0x96);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_1) & 7); i_33++) {
    __m128h v128h_34 = _mm512_cvtpd_ph(v512d_3);
    __m128i v128i_35 = _mm_slli_epi32(v128i_20, 3);
    }
    g_v512d[5] = v512d_3;
    g_v256d[1] = v256d_15;
    g_v128i[0] = v128i_19;
    g_m16[11] = m16_2;
    return v128i_19;
}

__mmask64 torture_365(__m128d p0, __m128 p1, __m256d p2, __m512h p3, __m256 p4) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_shuffle_epi8(v128i_1, v128i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_abs_epi32(v512i_3);
    __m512i v512i_5 = _mm512_mullo_epi32(v512i_4, v512i_3);
    __m128d v128d_6 = _mm_add_pd(p0, p0);
    __m128h v128h_7 = g_v128h[1];
    __m512d v512d_8 = _mm512_cvtph_pd(v128h_7);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_sub_epi16(v256i_9, v256i_9);
    __m256i v256i_11 = _mm256_or_si256(v256i_9, v256i_10);
    __m512 v512f_12 = g_v512f[7];
    __m512 v512f_13 = _mm512_insertf32x8(v512f_12, p4, 1);
    __m512 v512f_14 = _mm512_cvtepi32_ps(v512i_3);
    __m256i v256i_15 = _mm512_cvtepi64_epi32(v512i_4);
    __m256 v256f_16 = _mm512_cvtpd_ps(v512d_8);
    __m128i v128i_17 = _mm_and_si128(v128i_2, v128i_2);
    __m256d v256d_18 = _mm256_min_pd(p2, p2);
    __m512i v512i_19 = _mm512_rol_epi32(v512i_3, 5);
    __m512i v512i_20 = _mm512_sub_epi8(v512i_19, v512i_3);
    __m512h v512h_21 = _mm512_sqrt_ph(p3);
    __m128 v128f_22 = _mm_min_ps(p1, p1);
    __m128 v128f_23 = _mm_mul_ps(p1, v128f_22);
    __m512 v512f_24 = _mm512_roundscale_ps(v512f_14, 0x10);
    __m256d v256d_25 = _mm256_unpackhi_pd(p2, p2);
    __m512d v512d_26 = _mm512_sub_pd(v512d_8, v512d_8);
    __m512 v512f_27 = _mm512_rcp14_ps(v512f_14);
    __m512 v512f_28 = _mm512_xor_ps(v512f_24, v512f_27);
    __m512i v512i_29 = _mm512_slli_epi32(v512i_20, 3);
    __m256i v256i_30 = _mm256_add_epi8(v256i_9, v256i_15);
    __m128i v128i_31 = _mm_sub_epi8(v128i_17, v128i_1);
    int s32_32 = 53;
    if ((s32_32) & 1ULL) {
    __m256 v256f_33 = _mm256_add_ps(v256f_16, p4);
    __m256h v256h_34 = g_v256h[7];
    __m512 v512f_35 = _mm512_cvtxph_ps(v256h_34);
    __m512i v512i_36 = _mm512_cvtph_epi16(v512h_21);
    __mmask16 m16_37 = (__mmask16)(g_m16[10] ^ 46);
    __m512 v512f_38 = _mm512_mask_add_ps(v512f_24, m16_37, v512f_24, v512f_24);
    __mmask8 m8_39 = (__mmask8)(g_m8[2] ^ 91);
    __mmask16 m16_40 = _mm512_kunpackb(m8_39, m8_39);
    __m256 v256f_41 = _mm256_sub_ps(v256f_33, p4);
    }
    g_v512i[5] = v512i_20;
    g_v256f[7] = p4;
    g_v128i[2] = v128i_31;
    g_v512h[7] = p3;
    __mmask64 m64_42 = (__mmask64)(g_m64[0] ^ 212);
    return m64_42;
}

__m128h torture_366(__m128i p0, __m128i p1, __m512 p2) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m128h v128h_2 = _mm512_cvtpd_ph(v512d_1);
    __m512i v512i_3 = g_v512i[2];
    __m512i v512i_4 = _mm512_rolv_epi32(v512i_3, v512i_3);
    __m256h v256h_5 = g_v256h[2];
    __m512 v512f_6 = _mm512_cvtxph_ps(v256h_5);
    __m512i v512i_7 = _mm512_set1_epi32(0x55);
    __m128 v128f_8 = g_v128f[10];
    __m128 v128f_9 = _mm_shuffle_ps(v128f_8, v128f_8, 0x1B);
    __m512 v512f_10 = _mm512_getexp_ps(v512f_6);
    __mmask8 m8_11 = (__mmask8)(g_m8[1] ^ 20);
    __mmask8 m8_12 = _kand_mask8(m8_11, m8_11);
    __m512i v512i_13 = _mm512_conflict_epi32(v512i_7);
    __m512 v512f_14 = _mm512_sqrt_ps(v512f_6);
    __m256d v256d_15 = g_v256d[10];
    __m256d v256d_16 = _mm256_min_pd(v256d_15, v256d_15);
    __m128d v128d_17 = g_v128d[6];
    __m128d v128d_18 = _mm_min_pd(v128d_17, v128d_17);
    __m512h v512h_19 = g_v512h[11];
    __m512i v512i_20 = _mm512_cvtph_epi16(v512h_19);
    __m512i v512i_21 = _mm512_slli_epi32(v512i_13, 3);
    __m256i v256i_22 = g_v256i[2];
    __m256i v256i_23 = _mm256_unpacklo_epi32(v256i_22, v256i_22);
    __m256 v256f_24 = _mm256_loadu_ps(fbuf);
    __m256 v256f_25 = _mm256_max_ps(v256f_24, v256f_24);
    for (int i_26 = 0; i_26 < (int)(((unsigned long long)m8_11) & 7); i_26++) {
    __m512i v512i_27 = _mm512_dpbusd_epi32(v512i_21, v512i_13, v512i_3);
    __m256i v256i_28 = _mm256_add_epi64(v256i_23, v256i_22);
    __m128i v128i_29 = _mm_add_epi64(p1, p1);
    __m128 v128f_30 = _mm_shuffle_ps(v128f_9, v128f_8, 0x1B);
    __mmask16 m16_31 = _mm512_fpclass_ps_mask(p2, 0x3);
    }
    __m512i v512i_32 = _mm512_lzcnt_epi32(v512i_4);
    __mmask64 m64_33 = _mm512_cmp_epi8_mask(v512i_13, v512i_20, 1);
    __m128i v128i_34 = _mm_slli_epi32(p1, 3);
    __m256i v256i_35 = _mm256_set1_epi32(0x55);
    __m512h v512h_36 = _mm512_cvtepi16_ph(v512i_13);
    __m512i v512i_37 = _mm512_broadcastmb_epi64(m8_12);
    __m512d v512d_38 = _mm512_mask_blend_pd(m8_11, v512d_1, v512d_1);
    g_v512f[6] = v512f_14;
    g_v512d[3] = v512d_1;
    g_v256f[6] = v256f_25;
    g_v256d[6] = v256d_16;
    g_v256i[2] = v256i_22;
    g_v128i[8] = p0;
    g_v256h[3] = v256h_5;
    g_m8[10] = m8_11;
    g_m64[7] = m64_33;
    return v128h_2;
}

__m512 torture_367(__m128d p0, __m512d p1) {
    __m256i v256i_1 = _mm256_set1_epi32(0x55);
    __m512h v512h_2 = g_v512h[8];
    __m512h v512h_3 = _mm512_mul_ph(v512h_2, v512h_2);
    __m128d v128d_4 = _mm_fmadd_pd(p0, p0, p0);
    __m512i v512i_5 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_6 = _mm512_test_epi32_mask(v512i_5, v512i_5);
    __m128d v128d_7 = _mm_sub_pd(p0, v128d_4);
    __m512i v512i_8 = _mm512_dpwssd_epi32(v512i_5, v512i_5, v512i_5);
    __m128i v128i_9 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_10 = _mm_shuffle_epi8(v128i_9, v128i_9);
    __m512h v512h_11 = _mm512_sqrt_ph(v512h_2);
    __m256d v256d_12 = g_v256d[7];
    __m256d v256d_13 = _mm256_mul_pd(v256d_12, v256d_12);
    __m512i v512i_14 = _mm512_ternarylogic_epi32(v512i_5, v512i_8, v512i_5, 0x96);
    __m512 v512f_15 = _mm512_loadu_ps(fbuf);
    __m512 v512f_16 = _mm512_scalef_ps(v512f_15, v512f_15);
    __m128d v128d_17 = _mm_div_pd(p0, p0);
    __m512d v512d_18 = _mm512_div_pd(p1, p1);
    __m512 v512f_19 = _mm512_div_ps(v512f_15, v512f_16);
    __m256i v256i_20 = _mm256_slli_epi32(v256i_1, 3);
    __m128 v128f_21 = g_v128f[6];
    __m128 v128f_22 = _mm_fmsub_ps(v128f_21, v128f_21, v128f_21);
    __m256i v256i_23 = _mm256_sub_epi16(v256i_1, v256i_20);
    __m512i v512i_24 = _mm512_madd52lo_epu64(v512i_5, v512i_8, v512i_5);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_6) & 7); i_25++) {
    __m512d v512d_26 = _mm512_max_pd(p1, v512d_18);
    __m256i v256i_27 = _mm256_slli_epi32(v256i_1, 3);
    __m512 v512f_28 = _mm512_maskz_mul_ps(m16_6, v512f_15, v512f_19);
    }
    unsigned u32_29 = _cvtmask16_u32(m16_6);
    __m256i v256i_30 = _mm512_cvtepi64_epi32(v512i_14);
    __m512i v512i_31 = _mm512_cvtps_epi32(v512f_19);
    __m512i v512i_32 = _mm512_mask_add_epi32(v512i_31, m16_6, v512i_8, v512i_5);
    __m256i v256i_33 = _mm256_sub_epi64(v256i_20, v256i_30);
    __m512 v512f_34 = _mm512_mul_ps(v512f_19, v512f_15);
    __m256 v256f_35 = g_v256f[11];
    __m512 v512f_36 = _mm512_insertf32x8(v512f_34, v256f_35, 1);
    __m512d v512d_37 = _mm512_cvtps_pd(v256f_35);
    __m512h v512h_38 = _mm512_getexp_ph(v512h_2);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_6) & 7); i_39++) {
    __m128d v128d_40 = _mm_div_pd(v128d_17, v128d_17);
    __mmask16 m16_41 = _kxor_mask16(m16_6, m16_6);
    __m512 v512f_42 = _mm512_add_ps(v512f_34, v512f_15);
    __m512d v512d_43 = _mm512_mul_pd(v512d_37, v512d_37);
    }
    g_v512i[6] = v512i_24;
    g_v256f[1] = v256f_35;
    g_v128f[10] = v128f_21;
    g_v128d[0] = v128d_17;
    g_v128i[2] = v128i_9;
    g_m16[4] = m16_6;
    return v512f_19;
}

__m512 torture_368(__m256h p0, __m512 p1) {
    __m128i v128i_1 = g_v128i[5];
    __m128i v128i_2 = _mm_and_si128(v128i_1, v128i_1);
    __m256i v256i_3 = g_v256i[8];
    __m256i v256i_4 = _mm256_add_epi16(v256i_3, v256i_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[2] ^ 20);
    __m512 v512f_6 = _mm512_mask3_fmadd_ps(p1, p1, p1, m16_5);
    __m512d v512d_7 = g_v512d[0];
    __m256 v256f_8 = _mm512_cvtpd_ps(v512d_7);
    __m256 v256f_9 = _mm256_shuffle_ps(v256f_8, v256f_8, 0x1B);
    __m512i v512i_10 = _mm512_set1_epi32(0x55);
    __m512i v512i_11 = _mm512_abs_epi32(v512i_10);
    __m128i v128i_12 = _mm_add_epi8(v128i_1, v128i_1);
    __m256d v256d_13 = g_v256d[7];
    __m256d v256d_14 = _mm256_sub_pd(v256d_13, v256d_13);
    __m512d v512d_15 = _mm512_max_pd(v512d_7, v512d_7);
    __m256d v256d_16 = _mm256_sqrt_pd(v256d_14);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_5) & 7); i_17++) {
    __m128d v128d_18 = g_v128d[0];
    __m128d v128d_19 = _mm_mul_pd(v128d_18, v128d_18);
    __m128i v128i_20 = _mm_sub_epi8(v128i_2, v128i_12);
    __m128i v128i_21 = _mm_set1_epi32(0x55);
    __m512h v512h_22 = g_v512h[5];
    __m512h v512h_23 = _mm512_fmadd_ph(v512h_22, v512h_22, v512h_22);
    __m256i v256i_24 = _mm256_or_si256(v256i_3, v256i_4);
    }
    __m512h v512h_25 = g_v512h[1];
    __m512h v512h_26 = _mm512_sqrt_ph(v512h_25);
    __m512d v512d_27 = _mm512_cvtps_pd(v256f_8);
    __m256i v256i_28 = _mm256_sub_epi32(v256i_3, v256i_4);
    __m128d v128d_29 = g_v128d[0];
    __m512d v512d_30 = _mm512_insertf64x2(v512d_15, v128d_29, 3);
    __m256 v256f_31 = _mm256_and_ps(v256f_9, v256f_9);
    __m256i v256i_32 = _mm512_cvtepi64_epi32(v512i_10);
    for (int i_33 = 0; i_33 < (int)(((unsigned long long)m16_5) & 7); i_33++) {
    __m512 v512f_34 = _mm512_and_ps(v512f_6, v512f_6);
    __m512 v512f_35 = _mm512_rsqrt14_ps(v512f_34);
    __m512 v512f_36 = _mm512_maskz_mul_ps(m16_5, v512f_35, v512f_35);
    }
    g_v512d[4] = v512d_27;
    g_v256f[0] = v256f_9;
    g_v256i[5] = v256i_3;
    g_v512h[6] = v512h_26;
    return v512f_6;
}

__mmask32 torture_369(__m128d p0, __m512 p1, __m128d p2) {
    __m512 v512f_1 = _mm512_getexp_ps(p1);
    __m512 v512f_2 = _mm512_sqrt_ps(v512f_1);
    __m128 v128f_3 = _mm_loadu_ps(fbuf);
    __m128 v128f_4 = _mm_sub_ps(v128f_3, v128f_3);
    __m128 v128f_5 = _mm_fmsub_ps(v128f_4, v128f_4, v128f_4);
    __m512i v512i_6 = g_v512i[7];
    __m512i v512i_7 = _mm512_madd52lo_epu64(v512i_6, v512i_6, v512i_6);
    __mmask16 m16_8 = _mm512_cmp_ps_mask(v512f_2, p1, _CMP_LT_OQ);
    __m128i v128i_9 = g_v128i[5];
    __m128i v128i_10 = _mm_abs_epi32(v128i_9);
    __m128 v128f_11 = _mm_xor_ps(v128f_4, v128f_4);
    __m256 v256f_12 = g_v256f[3];
    __m256 v256f_13 = _mm256_fnmadd_ps(v256f_12, v256f_12, v256f_12);
    __m512h v512h_14 = g_v512h[0];
    __m512h v512h_15 = _mm512_rcp_ph(v512h_14);
    __m512d v512d_16 = g_v512d[9];
    __m256d v256d_17 = _mm512_extractf64x4_pd(v512d_16, 1);
    __m256 v256f_18 = _mm256_fmadd_ps(v256f_12, v256f_12, v256f_12);
    __m512d v512d_19 = _mm512_sqrt_pd(v512d_16);
    __m128 v128f_20 = _mm_and_ps(v128f_3, v128f_11);
    __m512d v512d_21 = _mm512_sqrt_pd(v512d_16);
    __m256d v256d_22 = _mm256_set1_pd(2.0);
    __m256d v256d_23 = _mm256_sub_pd(v256d_22, v256d_22);
    __m512i v512i_24 = _mm512_slli_epi32(v512i_7, 3);
    __m512i v512i_25 = _mm512_cvtph_epi16(v512h_15);
    __m128i v128i_26 = _mm_sub_epi64(v128i_9, v128i_9);
    __m512 v512f_27 = _mm512_xor_ps(p1, v512f_2);
    __mmask16 m16_28 = _mm512_cmp_epu32_mask(v512i_7, v512i_25, 4);
    __m512 v512f_29 = _mm512_rsqrt14_ps(p1);
    __mmask16 m16_30 = _mm512_cmp_epi32_mask(v512i_25, v512i_7, 2);
    __m512i v512i_31 = _mm512_unpacklo_epi32(v512i_24, v512i_25);
    __m128 v128f_32 = _mm_fmsub_ps(v128f_20, v128f_20, v128f_5);
    __m512h v512h_33 = _mm512_sqrt_ph(v512h_15);
    g_v256f[3] = v256f_18;
    g_v128f[2] = v128f_11;
    g_v128i[0] = v128i_10;
    g_v512h[11] = v512h_14;
    g_m16[2] = m16_28;
    __mmask32 m32_34 = (__mmask32)(g_m32[8] ^ 249);
    return m32_34;
}

__m512h torture_370(__m128 p0, __m256d p1, __m128i p2) {
    __m128d v128d_1 = g_v128d[2];
    __m128d v128d_2 = _mm_mul_pd(v128d_1, v128d_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[1] ^ 138);
    __m512 v512f_4 = g_v512f[10];
    __m512 v512f_5 = _mm512_mask_blend_ps(m16_3, v512f_4, v512f_4);
    __m512i v512i_6 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_7 = _mm512_sub_epi32(v512i_6, v512i_6);
    __m256i v256i_8 = _mm256_set1_epi32(0x55);
    __mmask16 m16_9 = _kor_mask16(m16_3, m16_3);
    __m512i v512i_10 = _mm512_dpwssd_epi32(v512i_6, v512i_7, v512i_7);
    __m512i v512i_11 = _mm512_cvtps_epi32(v512f_4);
    __m512 v512f_12 = _mm512_mask_add_ps(v512f_5, m16_9, v512f_4, v512f_4);
    __m512 v512f_13 = _mm512_div_ps(v512f_4, v512f_5);
    __m512i v512i_14 = _mm512_mask_blend_epi32(m16_9, v512i_10, v512i_6);
    __m256d v256d_15 = _mm256_sqrt_pd(p1);
    __m512d v512d_16 = g_v512d[10];
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_16, v512d_16, 0xEE);
    __m512i v512i_18 = _mm512_ternarylogic_epi32(v512i_10, v512i_6, v512i_7, 0x96);
    __m512 v512f_19 = _mm512_xor_ps(v512f_13, v512f_5);
    __m128d v128d_20 = _mm_set1_pd(2.0);
    __m512h v512h_21 = g_v512h[8];
    __m512h v512h_22 = _mm512_fmul_pch(v512h_21, v512h_21);
    __m128 v128f_23 = _mm_sub_ps(p0, p0);
    unsigned u32_24 = _cvtmask16_u32(m16_9);
    for (int i_25 = 0; i_25 < (int)(((unsigned long long)m16_9) & 7); i_25++) {
    __m256i v256i_26 = _mm256_xor_si256(v256i_8, v256i_8);
    __mmask64 m64_27 = (__mmask64)(g_m64[6] ^ 232);
    __mmask64 m64_28 = _kor_mask64(m64_27, m64_27);
    __m512i v512i_29 = _mm512_conflict_epi32(v512i_10);
    }
    __mmask16 m16_30 = _mm512_movepi32_mask(v512i_14);
    __mmask8 m8_31 = (__mmask8)(g_m8[6] ^ 160);
    __m512i v512i_32 = _mm512_broadcastmb_epi64(m8_31);
    __m256 v256f_33 = g_v256f[10];
    __m512 v512f_34 = _mm512_insertf32x8(v512f_13, v256f_33, 1);
    __m512i v512i_35 = _mm512_sub_epi64(v512i_7, v512i_6);
    __m512d v512d_36 = _mm512_fmadd_pd(v512d_17, v512d_17, v512d_16);
    __m512 v512f_37 = _mm512_roundscale_ps(v512f_13, 0x10);
    __m128 v128f_38 = _mm_fmadd_ps(v128f_23, v128f_23, v128f_23);
    __m512d v512d_39 = _mm512_fmadd_pd(v512d_17, v512d_17, v512d_16);
    __m128d v128d_40 = _mm_sub_pd(v128d_20, v128d_1);
    __m512 v512f_41 = _mm512_fnmadd_ps(v512f_37, v512f_19, v512f_13);
    __m512i v512i_42 = _mm512_shuffle_i32x4(v512i_35, v512i_35, 0x1B);
    __m512i v512i_43 = _mm512_ternarylogic_epi32(v512i_14, v512i_6, v512i_35, 0x96);
    for (int i_44 = 0; i_44 < (int)(((unsigned long long)m16_3) & 7); i_44++) {
    __m512i v512i_45 = _mm512_sub_epi16(v512i_6, v512i_14);
    __m256i v256i_46 = _mm256_abs_epi32(v256i_8);
    }
    g_v256d[9] = p1;
    g_v128f[8] = v128f_23;
    g_v128i[6] = p2;
    g_m8[7] = m8_31;
    g_m16[10] = m16_9;
    return v512h_21;
}

__m128 torture_371(__m512d p0, __m512h p1, __m256i p2, __m256h p3) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_add_epi8(v128i_1, v128i_1);
    __mmask16 m16_3 = (__mmask16)(g_m16[8] ^ 17);
    __mmask16 m16_4 = _kshiftri_mask16(m16_3, 1);
    __m512d v512d_5 = _mm512_shuffle_f64x2(p0, p0, 0xEE);
    __mmask32 m32_6 = _mm512_kunpackw(m16_4, m16_3);
    __m256 v256f_7 = g_v256f[6];
    __m256 v256f_8 = _mm256_add_ps(v256f_7, v256f_7);
    __m512 v512f_9 = _mm512_loadu_ps(fbuf);
    __m512 v512f_10 = _mm512_div_ps(v512f_9, v512f_9);
    __m512i v512i_11 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_12 = _mm512_add_epi32(v512i_11, v512i_11);
    __m128 v128f_13 = _mm_loadu_ps(fbuf);
    __m128 v128f_14 = _mm_sqrt_ps(v128f_13);
    __m256 v256f_15 = _mm256_sqrt_ps(v256f_8);
    __m512 v512f_16 = _mm512_fnmadd_ps(v512f_10, v512f_10, v512f_9);
    __m128i v128i_17 = _mm_add_epi32(v128i_1, v128i_2);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m512i v512i_18 = _mm512_mask_blend_epi32(m16_4, v512i_11, v512i_12);
    __m512 v512f_19 = _mm512_mask_blend_ps(m16_4, v512f_16, v512f_10);
    __m256 v256f_20 = _mm256_set1_ps(1.0f);
    __m256d v256d_21 = g_v256d[8];
    __m256d v256d_22 = _mm256_sqrt_pd(v256d_21);
    __m256 v256f_23 = _mm256_div_ps(v256f_7, v256f_15);
    __m128i v128i_24 = _mm_slli_epi32(v128i_2, 3);
    }
    __m512d v512d_25 = _mm512_mul_pd(p0, p0);
    __mmask16 m16_26 = _mm512_test_epi32_mask(v512i_12, v512i_12);
    __m512d v512d_27 = _mm512_set1_pd(2.0);
    __m256i v256i_28 = _mm256_sub_epi32(p2, p2);
    __m512i v512i_29 = _mm512_and_si512(v512i_12, v512i_11);
    __m512d v512d_30 = _mm512_add_pd(v512d_5, v512d_27);
    __m512 v512f_31 = _mm512_cvtepi32_ps(v512i_11);
    __m256 v256f_32 = _mm256_sqrt_ps(v256f_7);
    __mmask64 m64_33 = _mm512_cmp_epi8_mask(v512i_12, v512i_11, 1);
    if (((unsigned long long)m16_3) & 1ULL) {
    __m512h v512h_34 = _mm512_fmul_pch(p1, p1);
    __m512d v512d_35 = _mm512_add_pd(v512d_25, v512d_5);
    __m512i v512i_36 = _mm512_popcnt_epi64(v512i_12);
    __m128i v128i_37 = _mm_xor_si128(v128i_2, v128i_2);
    __mmask16 m16_38 = _mm512_fpclass_ps_mask(v512f_10, 0x3);
    }
    g_v512d[7] = v512d_25;
    g_v256f[4] = v256f_7;
    g_v256i[6] = v256i_28;
    g_v128f[7] = v128f_13;
    g_m64[7] = m64_33;
    return v128f_13;
}

__m512h torture_372(__m128d p0, __m256 p1) {
    __m256i v256i_1 = g_v256i[8];
    __m256i v256i_2 = _mm256_sub_epi16(v256i_1, v256i_1);
    __m512h v512h_3 = g_v512h[0];
    __mmask32 m32_4 = _mm512_cmp_ph_mask(v512h_3, v512h_3, 1);
    __m256 v256f_5 = _mm256_fnmadd_ps(p1, p1, p1);
    __m128 v128f_6 = _mm_set1_ps(1.0f);
    __m256 v256f_7 = _mm256_fmsub_ps(v256f_5, p1, p1);
    __m512i v512i_8 = g_v512i[0];
    __mmask16 m16_9 = _mm512_movepi32_mask(v512i_8);
    __m512d v512d_10 = g_v512d[10];
    __m512d v512d_11 = _mm512_div_pd(v512d_10, v512d_10);
    __m256 v256f_12 = _mm256_div_ps(v256f_5, p1);
    __m512 v512f_13 = g_v512f[0];
    __m512 v512f_14 = _mm512_unpacklo_ps(v512f_13, v512f_13);
    __m128i v128i_15 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_16 = _mm_shuffle_epi8(v128i_15, v128i_15);
    __m256 v256f_17 = _mm512_cvtpd_ps(v512d_11);
    __m512 v512f_18 = _mm512_mask_add_ps(v512f_14, m16_9, v512f_13, v512f_14);
    __m128 v128f_19 = _mm_max_ps(v128f_6, v128f_6);
    __mmask16 m16_20 = _mm512_testn_epi32_mask(v512i_8, v512i_8);
    __m512i v512i_21 = _mm512_add_epi64(v512i_8, v512i_8);
    __m256i v256i_22 = _mm256_xor_si256(v256i_1, v256i_1);
    __m256d v256d_23 = _mm512_extractf64x4_pd(v512d_10, 1);
    __m512 v512f_24 = _mm512_min_ps(v512f_13, v512f_13);
    __m512 v512f_25 = _mm512_xor_ps(v512f_13, v512f_18);
    g_v128f[5] = v128f_6;
    g_v128i[9] = v128i_16;
    g_m32[0] = m32_4;
    return v512h_3;
}

__m128i torture_373(__m256 p0, __m128h p1, __m256i p2, __m256i p3) {
    __m256 v256f_1 = _mm256_fmsub_ps(p0, p0, p0);
    __m128i v128i_2 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_3 = _mm_add_epi64(v128i_2, v128i_2);
    __m128i v128i_4 = _mm_sllv_epi32(v128i_2, v128i_3);
    __m256 v256f_5 = _mm256_sub_ps(p0, p0);
    __m256 v256f_6 = _mm256_fmsub_ps(v256f_1, v256f_1, v256f_1);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_add_epi16(v512i_7, v512i_7);
    __m256i v256i_9 = _mm256_sub_epi32(p2, p3);
    __m512i v512i_10 = _mm512_sllv_epi32(v512i_7, v512i_8);
    __m512i v512i_11 = _mm512_unpacklo_epi32(v512i_8, v512i_8);
    __m512d v512d_12 = g_v512d[7];
    __m256d v256d_13 = _mm512_extractf64x4_pd(v512d_12, 1);
    __m256i v256i_14 = _mm256_add_epi8(v256i_9, p2);
    __m512 v512f_15 = _mm512_loadu_ps(fbuf);
    __m512 v512f_16 = _mm512_div_ps(v512f_15, v512f_15);
    __m512i v512i_17 = _mm512_add_epi8(v512i_11, v512i_11);
    int s32_18 = 82;
    if ((s32_18) & 1ULL) {
    __m128i v128i_19 = _mm_add_epi16(v128i_3, v128i_4);
    __m512i v512i_20 = _mm512_permutexvar_epi32(v512i_10, v512i_10);
    __m512 v512f_21 = _mm512_scalef_ps(v512f_15, v512f_15);
    }
    __m128 v128f_22 = _mm512_extractf32x4_ps(v512f_15, 2);
    __mmask64 m64_23 = (__mmask64)(g_m64[10] ^ 87);
    __mmask64 m64_24 = _kor_mask64(m64_23, m64_23);
    __m512i v512i_25 = _mm512_ternarylogic_epi32(v512i_7, v512i_17, v512i_11, 0x96);
    __m256d v256d_26 = _mm256_max_pd(v256d_13, v256d_13);
    __m512i v512i_27 = _mm512_dpbusd_epi32(v512i_8, v512i_8, v512i_25);
    __m256d v256d_28 = _mm256_set1_pd(2.0);
    __mmask64 m64_29 = _mm512_movepi8_mask(v512i_11);
    __m256d v256d_30 = _mm256_div_pd(v256d_26, v256d_28);
    __m512d v512d_31 = _mm512_div_pd(v512d_12, v512d_12);
    __m256i v256i_32 = _mm256_mullo_epi32(p2, v256i_14);
    __m512 v512f_33 = _mm512_broadcast_f32x4(v128f_22);
    __mmask8 m8_34 = (__mmask8)(g_m8[1] ^ 159);
    __mmask16 m16_35 = _mm512_kunpackb(m8_34, m8_34);
    g_v512f[3] = v512f_15;
    g_v512i[0] = v512i_7;
    g_m8[6] = m8_34;
    g_m16[3] = m16_35;
    g_m64[7] = m64_23;
    return v128i_2;
}

__m512d torture_374(__m256i p0, __m128h p1, __m512d p2) {
    __m512i v512i_1 = g_v512i[0];
    __m512i v512i_2 = _mm512_add_epi8(v512i_1, v512i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_fmadd_ps(v512f_3, v512f_3, v512f_3);
    __m512 v512f_5 = _mm512_rcp14_ps(v512f_4);
    __m512h v512h_6 = g_v512h[5];
    __m512h v512h_7 = _mm512_fmul_pch(v512h_6, v512h_6);
    __m512i v512i_8 = _mm512_add_epi16(v512i_1, v512i_2);
    __m512i v512i_9 = _mm512_ternarylogic_epi32(v512i_1, v512i_8, v512i_2, 0x96);
    __m256 v256f_10 = g_v256f[9];
    __m256 v256f_11 = _mm256_sub_ps(v256f_10, v256f_10);
    __m512h v512h_12 = _mm512_fmul_pch(v512h_6, v512h_7);
    __m512i v512i_13 = _mm512_add_epi32(v512i_9, v512i_1);
    __m128i v128i_14 = g_v128i[0];
    __m512i v512i_15 = _mm512_cvtepi8_epi32(v128i_14);
    __m512i v512i_16 = _mm512_sub_epi32(v512i_15, v512i_1);
    __m128 v128f_17 = _mm_loadu_ps(fbuf);
    __m128 v128f_18 = _mm_min_ps(v128f_17, v128f_17);
    __m256d v256d_19 = _mm256_set1_pd(2.0);
    __m512 v512f_20 = _mm512_mul_ps(v512f_4, v512f_3);
    int s32_21 = 19;
    for (int i_22 = 0; i_22 < (int)((s32_21) & 7); i_22++) {
    __m512 v512f_23 = _mm512_and_ps(v512f_4, v512f_3);
    __mmask16 m16_24 = (__mmask16)(g_m16[0] ^ 194);
    __mmask32 m32_25 = _mm512_kunpackw(m16_24, m16_24);
    __m512d v512d_26 = _mm512_set1_pd(2.0);
    __m512 v512f_27 = _mm512_sub_ps(v512f_5, v512f_3);
    __m512 v512f_28 = _mm512_add_ps(v512f_27, v512f_20);
    }
    __mmask8 m8_29 = _mm512_cmp_pd_mask(p2, p2, _CMP_GE_OQ);
    __mmask16 m16_30 = (__mmask16)(g_m16[4] ^ 42);
    __m512 v512f_31 = _mm512_mask_blend_ps(m16_30, v512f_3, v512f_3);
    __mmask64 m64_32 = _mm512_movepi8_mask(v512i_9);
    __m256i v256i_33 = _mm256_add_epi8(p0, p0);
    __m512 v512f_34 = _mm512_sqrt_ps(v512f_5);
    __m512i v512i_35 = _mm512_slli_epi32(v512i_15, 3);
    __m128 v128f_36 = _mm_mul_ps(v128f_17, v128f_18);
    g_v512h[1] = v512h_7;
    g_m16[7] = m16_30;
    g_m64[3] = m64_32;
    return p2;
}

__mmask32 torture_375(__m512 p0, __m256i p1) {
    __mmask16 m16_1 = (__mmask16)(g_m16[0] ^ 78);
    __m512i v512i_2 = g_v512i[6];
    __m512i v512i_3 = _mm512_mask_blend_epi32(m16_1, v512i_2, v512i_2);
    __m512i v512i_4 = _mm512_abs_epi32(v512i_2);
    __m256 v256f_5 = g_v256f[11];
    __m512 v512f_6 = _mm512_insertf32x8(p0, v256f_5, 1);
    __m512i v512i_7 = _mm512_popcnt_epi64(v512i_3);
    __m512i v512i_8 = _mm512_conflict_epi32(v512i_2);
    __m128 v128f_9 = _mm_loadu_ps(fbuf);
    __m128 v128f_10 = _mm_sqrt_ps(v128f_9);
    __m512 v512f_11 = _mm512_fmsub_ps(p0, v512f_6, v512f_6);
    __m512i v512i_12 = _mm512_dpwssd_epi32(v512i_3, v512i_7, v512i_7);
    for (int i_13 = 0; i_13 < (int)(((unsigned long long)m16_1) & 7); i_13++) {
    __m256i v256i_14 = _mm256_add_epi64(p1, p1);
    __m128d v128d_15 = g_v128d[8];
    __m128d v128d_16 = _mm_min_pd(v128d_15, v128d_15);
    }
    __m256d v256d_17 = g_v256d[5];
    __m256d v256d_18 = _mm256_add_pd(v256d_17, v256d_17);
    __m512h v512h_19 = g_v512h[7];
    __m512h v512h_20 = _mm512_fmul_pch(v512h_19, v512h_19);
    __m512d v512d_21 = g_v512d[6];
    __m512d v512d_22 = _mm512_mul_pd(v512d_21, v512d_21);
    __m256d v256d_23 = _mm256_max_pd(v256d_17, v256d_18);
    __m512 v512f_24 = _mm512_rcp14_ps(v512f_6);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m128i v128i_25 = g_v128i[10];
    __m128i v128i_26 = _mm_abs_epi32(v128i_25);
    __m512d v512d_27 = _mm512_unpackhi_pd(v512d_22, v512d_21);
    __m256i v256i_28 = _mm256_slli_epi32(p1, 3);
    }
    g_v512f[7] = v512f_11;
    g_v256d[4] = v256d_17;
    __mmask32 m32_29 = (__mmask32)(g_m32[7] ^ 212);
    return m32_29;
}

__m512h torture_376(__m256h p0, __m512h p1, __m512i p2, __m512i p3) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_add_epi64(v128i_1, v128i_1);
    __m512 v512f_3 = g_v512f[7];
    __m256 v256f_4 = g_v256f[1];
    __m512 v512f_5 = _mm512_insertf32x8(v512f_3, v256f_4, 1);
    __m128d v128d_6 = g_v128d[4];
    __m128d v128d_7 = _mm_unpackhi_pd(v128d_6, v128d_6);
    __m256i v256i_8 = g_v256i[5];
    __m256i v256i_9 = _mm256_sub_epi16(v256i_8, v256i_8);
    __mmask16 m16_10 = (__mmask16)(g_m16[4] ^ 141);
    unsigned u32_11 = _cvtmask16_u32(m16_10);
    __m512i v512i_12 = _mm512_dpwssd_epi32(p2, p3, p3);
    __m128 v128f_13 = g_v128f[8];
    __m128 v128f_14 = _mm_min_ps(v128f_13, v128f_13);
    __mmask64 m64_15 = _mm512_movepi8_mask(v512i_12);
    __m512i v512i_16 = _mm512_shuffle_i32x4(p3, p3, 0x1B);
    __m512i v512i_17 = _mm512_xor_si512(p3, p2);
    __m256d v256d_18 = g_v256d[9];
    __m256d v256d_19 = _mm256_min_pd(v256d_18, v256d_18);
    __m256i v256i_20 = _mm256_shuffle_epi8(v256i_9, v256i_8);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m256 v256f_21 = _mm256_fmsub_ps(v256f_4, v256f_4, v256f_4);
    __m128i v128i_22 = _mm_set1_epi32(0x55);
    __m128i v128i_23 = _mm_shuffle_epi8(v128i_2, v128i_1);
    __m512d v512d_24 = g_v512d[0];
    __m256 v256f_25 = _mm512_cvtpd_ps(v512d_24);
    __m128 v128f_26 = _mm_add_ps(v128f_14, v128f_14);
    }
    __m512 v512f_27 = _mm512_unpacklo_ps(v512f_3, v512f_5);
    __m128d v128d_28 = _mm_unpackhi_pd(v128d_7, v128d_6);
    __m512d v512d_29 = g_v512d[5];
    __m512d v512d_30 = _mm512_add_pd(v512d_29, v512d_29);
    __m512i v512i_31 = _mm512_unpacklo_epi32(p3, v512i_17);
    __mmask16 m16_32 = _kshiftli_mask16(m16_10, 2);
    __m512i v512i_33 = _mm512_shuffle_epi8(v512i_31, v512i_16);
    __m512d v512d_34 = _mm512_div_pd(v512d_29, v512d_29);
    __m128 v128f_35 = _mm_fmsub_ps(v128f_14, v128f_14, v128f_14);
    if (((unsigned long long)m16_10) & 1ULL) {
    __m128i v128i_36 = _mm_sub_epi16(v128i_1, v128i_2);
    __m512i v512i_37 = _mm512_sllv_epi32(v512i_12, v512i_33);
    __m128i v128i_38 = _mm_set1_epi32(0x55);
    __m128 v128f_39 = _mm_max_ps(v128f_35, v128f_35);
    }
    g_v512i[1] = v512i_16;
    g_v128i[7] = v128i_2;
    g_m16[11] = m16_10;
    g_m64[8] = m64_15;
    return p1;
}

__mmask16 torture_377(__m128 p0, __m256 p1, __m512 p2, __m512d p3) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sub_epi16(v256i_1, v256i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __mmask16 m16_4 = _mm512_movepi32_mask(v512i_3);
    __m512i v512i_5 = _mm512_unpacklo_epi32(v512i_3, v512i_3);
    __m256d v256d_6 = g_v256d[8];
    __m256d v256d_7 = _mm256_sqrt_pd(v256d_6);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_9 = _mm512_cvtepi8_epi32(v128i_8);
    __m512 v512f_10 = _mm512_roundscale_ps(p2, 0x10);
    __m512i v512i_11 = _mm512_rol_epi32(v512i_9, 5);
    __m256 v256f_12 = _mm256_and_ps(p1, p1);
    __mmask8 m8_13 = (__mmask8)(g_m8[11] ^ 235);
    __m512d v512d_14 = _mm512_mask_blend_pd(m8_13, p3, p3);
    __m512h v512h_15 = g_v512h[8];
    __m512h v512h_16 = _mm512_fmadd_ph(v512h_15, v512h_15, v512h_15);
    for (int i_17 = 0; i_17 < (int)(((unsigned long long)m16_4) & 7); i_17++) {
    __m128i v128i_18 = _mm_xor_si128(v128i_8, v128i_8);
    __m256d v256d_19 = _mm256_min_pd(v256d_7, v256d_7);
    __m512h v512h_20 = _mm512_fmul_pch(v512h_16, v512h_16);
    }
    __m128 v128f_21 = _mm_add_ps(p0, p0);
    __mmask16 m16_22 = _kxor_mask16(m16_4, m16_4);
    __mmask64 m64_23 = _mm512_movepi8_mask(v512i_9);
    __m512 v512f_24 = _mm512_div_ps(p2, p2);
    __mmask16 m16_25 = _mm512_cmp_epu32_mask(v512i_11, v512i_5, 4);
    __m256i v256i_26 = _mm256_sub_epi64(v256i_1, v256i_2);
    __m128d v128d_27 = g_v128d[9];
    __m128d v128d_28 = _mm_min_pd(v128d_27, v128d_27);
    for (int i_29 = 0; i_29 < (int)(((unsigned long long)m16_22) & 7); i_29++) {
    __m128d v128d_30 = _mm_add_pd(v128d_28, v128d_27);
    __m512 v512f_31 = _mm512_broadcast_f32x4(v128f_21);
    __m512i v512i_32 = _mm512_shuffle_epi8(v512i_9, v512i_11);
    __m256h v256h_33 = g_v256h[6];
    __m512 v512f_34 = _mm512_cvtxph_ps(v256h_33);
    __m512i v512i_35 = _mm512_dpwssd_epi32(v512i_32, v512i_11, v512i_32);
    }
    g_v512d[8] = v512d_14;
    g_v512i[2] = v512i_3;
    g_v128f[4] = v128f_21;
    g_v128d[0] = v128d_27;
    g_v128i[9] = v128i_8;
    g_m64[5] = m64_23;
    return m16_4;
}

__mmask8 torture_378(__m512i p0, __m512h p1, __m128h p2) {
    __mmask16 m16_1 = (__mmask16)(g_m16[2] ^ 26);
    __m512i v512i_2 = _mm512_mask_add_epi32(p0, m16_1, p0, p0);
    __m512i v512i_3 = _mm512_sub_epi32(v512i_2, v512i_2);
    unsigned u32_4 = 84;
    __mmask16 m16_5 = _cvtu32_mask16(u32_4);
    __m512 v512f_6 = _mm512_loadu_ps(fbuf);
    __m512 v512f_7 = _mm512_rcp14_ps(v512f_6);
    __m512d v512d_8 = g_v512d[0];
    __m256d v256d_9 = _mm512_extractf64x4_pd(v512d_8, 1);
    __m512d v512d_10 = _mm512_unpackhi_pd(v512d_8, v512d_8);
    __m512 v512f_11 = _mm512_fmadd_ps(v512f_7, v512f_7, v512f_7);
    __m128 v128f_12 = g_v128f[4];
    __m128 v128f_13 = _mm_fnmadd_ps(v128f_12, v128f_12, v128f_12);
    __m512 v512f_14 = _mm512_fnmadd_ps(v512f_11, v512f_11, v512f_6);
    __m256i v256i_15 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_16 = _mm256_abs_epi32(v256i_15);
    __m512i v512i_17 = _mm512_abs_epi32(v512i_2);
    __m256d v256d_18 = _mm256_min_pd(v256d_9, v256d_9);
    __mmask16 m16_19 = _mm512_cmp_epi32_mask(v512i_17, v512i_17, 2);
    __m256 v256f_20 = _mm256_loadu_ps(fbuf);
    __m256 v256f_21 = _mm256_fnmadd_ps(v256f_20, v256f_20, v256f_20);
    for (int i_22 = 0; i_22 < (int)(((unsigned long long)m16_19) & 7); i_22++) {
    __m256d v256d_23 = _mm256_sqrt_pd(v256d_18);
    __m512 v512f_24 = _mm512_and_ps(v512f_6, v512f_14);
    __mmask64 m64_25 = _mm512_cmp_epi8_mask(v512i_2, v512i_3, 1);
    __m512 v512f_26 = _mm512_fnmadd_ps(v512f_6, v512f_24, v512f_6);
    __mmask16 m16_27 = _mm512_cmp_ps_mask(v512f_24, v512f_24, _CMP_LT_OQ);
    }
    __mmask32 m32_28 = _mm512_cmp_ph_mask(p1, p1, 1);
    __m256d v256d_29 = _mm256_add_pd(v256d_9, v256d_18);
    __m512h v512h_30 = _mm512_getexp_ph(p1);
    __m512 v512f_31 = _mm512_unpacklo_ps(v512f_7, v512f_11);
    __m512 v512f_32 = _mm512_maskz_mul_ps(m16_5, v512f_14, v512f_31);
    __m512i v512i_33 = _mm512_cvtph_epi16(v512h_30);
    __m512i v512i_34 = _mm512_popcnt_epi64(v512i_2);
    __m256d v256d_35 = _mm512_extractf64x4_pd(v512d_10, 1);
    __m512h v512h_36 = _mm512_fmul_pch(p1, p1);
    __m256 v256f_37 = _mm256_set1_ps(1.0f);
    __m512 v512f_38 = _mm512_sub_ps(v512f_32, v512f_14);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m512i v512i_39 = _mm512_add_epi16(v512i_34, v512i_2);
    __m512i v512i_40 = _mm512_conflict_epi32(p0);
    __m256d v256d_41 = _mm256_max_pd(v256d_9, v256d_29);
    __m512i v512i_42 = _mm512_add_epi32(v512i_33, v512i_3);
    __m128i v128i_43 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_44 = _mm_sub_epi64(v128i_43, v128i_43);
    __m128 v128f_45 = _mm_add_ps(v128f_13, v128f_13);
    }
    g_v512d[6] = v512d_8;
    g_v512i[3] = v512i_34;
    g_v256d[0] = v256d_35;
    __mmask8 m8_46 = (__mmask8)(g_m8[1] ^ 127);
    return m8_46;
}

__m128 torture_379(__m128d p0, __m512i p1, __m128 p2) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_2 = _mm512_broadcast_i32x4(v128i_1);
    __m256d v256d_3 = g_v256d[7];
    __m256d v256d_4 = _mm256_unpackhi_pd(v256d_3, v256d_3);
    __m256i v256i_5 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_6 = _mm256_add_epi64(v256i_5, v256i_5);
    __m512 v512f_7 = g_v512f[2];
    __m512 v512f_8 = _mm512_add_ps(v512f_7, v512f_7);
    __m512i v512i_9 = _mm512_add_epi8(p1, v512i_2);
    __m256h v256h_10 = g_v256h[8];
    __m512 v512f_11 = _mm512_cvtxph_ps(v256h_10);
    __m128i v128i_12 = _mm_add_epi32(v128i_1, v128i_1);
    __m128d v128d_13 = _mm_add_pd(p0, p0);
    __m512 v512f_14 = _mm512_unpacklo_ps(v512f_11, v512f_11);
    __mmask16 m16_15 = (__mmask16)(g_m16[6] ^ 103);
    __m512i v512i_16 = _mm512_mask_blend_epi32(m16_15, p1, p1);
    __mmask64 m64_17 = _mm512_cmp_epi8_mask(v512i_9, v512i_2, 1);
    __m256d v256d_18 = _mm256_mul_pd(v256d_3, v256d_3);
    __m512h v512h_19 = g_v512h[10];
    __mmask32 m32_20 = _mm512_cmp_ph_mask(v512h_19, v512h_19, 1);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_15) & 7); i_21++) {
    __m256i v256i_22 = _mm256_sub_epi8(v256i_5, v256i_6);
    __m256d v256d_23 = _mm256_div_pd(v256d_3, v256d_18);
    __m512 v512f_24 = _mm512_and_ps(v512f_11, v512f_14);
    __m128i v128i_25 = _mm_sllv_epi32(v128i_1, v128i_1);
    __m128i v128i_26 = _mm_sub_epi64(v128i_12, v128i_12);
    }
    __m512i v512i_27 = _mm512_madd52lo_epu64(v512i_16, p1, v512i_16);
    __m512 v512f_28 = _mm512_broadcast_f32x4(p2);
    __m512 v512f_29 = _mm512_add_ps(v512f_11, v512f_8);
    __m512i v512i_30 = _mm512_sub_epi32(p1, v512i_16);
    __m512i v512i_31 = _mm512_lzcnt_epi32(v512i_2);
    if (((unsigned long long)m16_15) & 1ULL) {
    __mmask16 m16_32 = _knot_mask16(m16_15);
    __m128 v128f_33 = _mm_max_ps(p2, p2);
    __m256d v256d_34 = _mm256_fmadd_pd(v256d_18, v256d_4, v256d_4);
    __m512i v512i_35 = _mm512_ternarylogic_epi32(v512i_31, v512i_31, v512i_27, 0x96);
    __m256d v256d_36 = _mm256_div_pd(v256d_4, v256d_34);
    }
    g_v512f[9] = v512f_28;
    g_v256i[1] = v256i_5;
    g_v128d[3] = p0;
    g_v128i[10] = v128i_1;
    g_m16[5] = m16_15;
    g_m64[6] = m64_17;
    return p2;
}

__m512 torture_380(__m512i p0, __m512i p1, __m256 p2, __m128h p3) {
    __m128i v128i_1 = g_v128i[2];
    __m128i v128i_2 = _mm_mullo_epi32(v128i_1, v128i_1);
    __m512 v512f_3 = g_v512f[7];
    __m512 v512f_4 = _mm512_div_ps(v512f_3, v512f_3);
    __mmask16 m16_5 = (__mmask16)(g_m16[1] ^ 95);
    __m512 v512f_6 = _mm512_mask_blend_ps(m16_5, v512f_3, v512f_3);
    __m512d v512d_7 = g_v512d[4];
    __m128d v128d_8 = g_v128d[7];
    __m512d v512d_9 = _mm512_insertf64x2(v512d_7, v128d_8, 3);
    __m512d v512d_10 = _mm512_cvtph_pd(p3);
    __m256 v256f_11 = _mm256_fmsub_ps(p2, p2, p2);
    __m512i v512i_12 = _mm512_add_epi64(p1, p1);
    __m256 v256f_13 = _mm512_cvtpd_ps(v512d_10);
    for (int i_14 = 0; i_14 < (int)(((unsigned long long)m16_5) & 7); i_14++) {
    __m128d v128d_15 = _mm_mul_pd(v128d_8, v128d_8);
    __m512 v512f_16 = _mm512_set1_ps(1.0f);
    }
    __m128d v128d_17 = _mm_min_pd(v128d_8, v128d_8);
    __m128i v128i_18 = _mm_and_si128(v128i_1, v128i_1);
    __mmask16 m16_19 = _mm512_cmp_ps_mask(v512f_4, v512f_4, _CMP_LT_OQ);
    __m128i v128i_20 = _mm_unpacklo_epi32(v128i_2, v128i_1);
    __m256i v256i_21 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_22 = _mm256_or_si256(v256i_21, v256i_21);
    __mmask16 m16_23 = _kand_mask16(m16_5, m16_19);
    __m128i v128i_24 = _mm_shuffle_epi8(v128i_1, v128i_1);
    if (((unsigned long long)m16_23) & 1ULL) {
    __m512d v512d_25 = _mm512_max_pd(v512d_9, v512d_7);
    __m512 v512f_26 = _mm512_scalef_ps(v512f_4, v512f_3);
    __m512i v512i_27 = _mm512_abs_epi32(p1);
    __m512 v512f_28 = _mm512_fnmadd_ps(v512f_4, v512f_3, v512f_26);
    __m256d v256d_29 = g_v256d[10];
    __m256d v256d_30 = _mm256_min_pd(v256d_29, v256d_29);
    __m512i v512i_31 = _mm512_set1_epi32(0x55);
    }
    g_v512f[11] = v512f_3;
    g_v256i[8] = v256i_21;
    g_v128i[0] = v128i_24;
    g_m16[0] = m16_5;
    return v512f_6;
}

__m512 torture_381(__m128 p0, __m512h p1, __m128d p2) {
    __m128 v128f_1 = _mm_and_ps(p0, p0);
    __m128i v128i_2 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_3 = _mm_sub_epi32(v128i_2, v128i_2);
    __m128d v128d_4 = _mm_min_pd(p2, p2);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512 v512f_6 = _mm512_div_ps(v512f_5, v512f_5);
    __m512i v512i_7 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_8 = _mm512_and_si512(v512i_7, v512i_7);
    __m512i v512i_9 = _mm512_abs_epi32(v512i_7);
    __mmask64 m64_10 = _mm512_movepi8_mask(v512i_9);
    __m512h v512h_11 = _mm512_cvtepi16_ph(v512i_9);
    __m256 v256f_12 = g_v256f[6];
    __m512d v512d_13 = _mm512_cvtps_pd(v256f_12);
    __m128i v128i_14 = _mm_set1_epi32(0x55);
    __m512 v512f_15 = _mm512_and_ps(v512f_5, v512f_5);
    __m256 v256f_16 = _mm256_fmsub_ps(v256f_12, v256f_12, v256f_12);
    __m128i v128i_17 = _mm_or_si128(v128i_14, v128i_2);
    __m128d v128d_18 = _mm_min_pd(p2, v128d_4);
    __m512d v512d_19 = _mm512_cvtps_pd(v256f_12);
    __m512d v512d_20 = _mm512_shuffle_f64x2(v512d_13, v512d_13, 0xEE);
    __m512h v512h_21 = _mm512_mul_ph(v512h_11, p1);
    __m128i v128i_22 = _mm_set1_epi32(0x55);
    if (((unsigned long long)m64_10) & 1ULL) {
    __m512i v512i_23 = _mm512_dpbusd_epi32(v512i_9, v512i_9, v512i_7);
    __m512 v512f_24 = _mm512_roundscale_ps(v512f_15, 0x10);
    __m128d v128d_25 = _mm_unpackhi_pd(v128d_4, v128d_4);
    __m128 v128f_26 = _mm_fmsub_ps(v128f_1, p0, v128f_1);
    __m128 v128f_27 = _mm_set1_ps(1.0f);
    __m128 v128f_28 = _mm_max_ps(v128f_1, v128f_1);
    }
    __m512i v512i_29 = _mm512_slli_epi32(v512i_8, 3);
    __m512i v512i_30 = _mm512_dpbusd_epi32(v512i_7, v512i_29, v512i_8);
    __m512 v512f_31 = _mm512_min_ps(v512f_15, v512f_6);
    __m512 v512f_32 = _mm512_sqrt_ps(v512f_15);
    __m128d v128d_33 = _mm_min_pd(v128d_18, v128d_18);
    __m512 v512f_34 = _mm512_set1_ps(1.0f);
    __mmask16 m16_35 = _mm512_cmp_epi32_mask(v512i_9, v512i_29, 2);
    __mmask32 m32_36 = _mm512_kunpackw(m16_35, m16_35);
    __m512i v512i_37 = _mm512_cvtepi8_epi32(v128i_22);
    if (((unsigned long long)m16_35) & 1ULL) {
    __m512 v512f_38 = _mm512_shuffle_f32x4(v512f_31, v512f_15, 0x4E);
    __m512h v512h_39 = _mm512_fmadd_ph(v512h_11, v512h_11, p1);
    __m512i v512i_40 = _mm512_cvtph_epi16(v512h_21);
    }
    g_v512f[0] = v512f_6;
    g_v512i[7] = v512i_8;
    g_v128f[5] = p0;
    g_v128d[1] = p2;
    g_v512h[6] = p1;
    return v512f_6;
}

__m512i torture_382(__m128h p0, __m128d p1) {
    __m512d v512d_1 = _mm512_loadu_pd(dbuf);
    __m512d v512d_2 = _mm512_sqrt_pd(v512d_1);
    __m512i v512i_3 = g_v512i[10];
    __m512i v512i_4 = _mm512_and_si512(v512i_3, v512i_3);
    __m256 v256f_5 = _mm512_cvtpd_ps(v512d_1);
    __m256i v256i_6 = g_v256i[11];
    __m256i v256i_7 = _mm256_sub_epi64(v256i_6, v256i_6);
    __m256 v256f_8 = _mm256_fnmadd_ps(v256f_5, v256f_5, v256f_5);
    __m512h v512h_9 = g_v512h[6];
    __m512h v512h_10 = _mm512_getexp_ph(v512h_9);
    __m256i v256i_11 = _mm256_sub_epi64(v256i_6, v256i_7);
    __m128d v128d_12 = _mm_fmadd_pd(p1, p1, p1);
    __m128i v128i_13 = g_v128i[3];
    __m128i v128i_14 = _mm_add_epi16(v128i_13, v128i_13);
    __m512i v512i_15 = _mm512_abs_epi32(v512i_3);
    __m512i v512i_16 = _mm512_unpacklo_epi32(v512i_3, v512i_3);
    __m512i v512i_17 = _mm512_permutex2var_epi32(v512i_4, v512i_3, v512i_3);
    __mmask16 m16_18 = (__mmask16)(g_m16[11] ^ 74);
    __m512 v512f_19 = g_v512f[8];
    __m512 v512f_20 = _mm512_mask_blend_ps(m16_18, v512f_19, v512f_19);
    __m512 v512f_21 = _mm512_roundscale_ps(v512f_19, 0x10);
    unsigned u32_22 = _cvtmask16_u32(m16_18);
    __mmask8 m8_23 = (__mmask8)(g_m8[0] ^ 146);
    __m512i v512i_24 = _mm512_broadcastmb_epi64(m8_23);
    __m512i v512i_25 = _mm512_cvtepi8_epi32(v128i_13);
    __m128 v128f_26 = _mm_loadu_ps(fbuf);
    __m512 v512f_27 = _mm512_broadcast_f32x4(v128f_26);
    __m256i v256i_28 = _mm256_and_si256(v256i_6, v256i_11);
    __m512 v512f_29 = _mm512_fmsub_ps(v512f_19, v512f_27, v512f_20);
    __m512i v512i_30 = _mm512_popcnt_epi64(v512i_3);
    g_v512f[5] = v512f_20;
    g_v512d[8] = v512d_1;
    g_v128f[9] = v128f_26;
    g_v128d[7] = v128d_12;
    g_m8[11] = m8_23;
    g_m16[10] = m16_18;
    return v512i_25;
}

__m512i torture_383(__m256d p0, __m128h p1, __m256h p2, __m128i p3, __m256h p4) {
    __m512 v512f_1 = _mm512_loadu_ps(fbuf);
    __m512 v512f_2 = _mm512_max_ps(v512f_1, v512f_1);
    __m128i v128i_3 = _mm_add_epi32(p3, p3);
    __m256i v256i_4 = g_v256i[5];
    __m256i v256i_5 = _mm256_add_epi8(v256i_4, v256i_4);
    __m512 v512f_6 = _mm512_max_ps(v512f_2, v512f_2);
    __m512d v512d_7 = g_v512d[10];
    __m256 v256f_8 = _mm512_cvtpd_ps(v512d_7);
    __m512h v512h_9 = g_v512h[6];
    __m512h v512h_10 = _mm512_fmul_pch(v512h_9, v512h_9);
    __m256 v256f_11 = _mm256_sqrt_ps(v256f_8);
    __m512d v512d_12 = _mm512_set1_pd(2.0);
    __m512i v512i_13 = _mm512_loadu_si512((const void*)ibuf);
    __m128i v128i_14 = _mm512_cvtepi32_epi8(v512i_13);
    __m256d v256d_15 = _mm256_fmadd_pd(p0, p0, p0);
    __m512 v512f_16 = _mm512_cvtxph_ps(p2);
    __m512i v512i_17 = _mm512_shuffle_i32x4(v512i_13, v512i_13, 0x1B);
    int s32_18 = 52;
    for (int i_19 = 0; i_19 < (int)((s32_18) & 7); i_19++) {
    __m512i v512i_20 = _mm512_add_epi64(v512i_17, v512i_13);
    __m512i v512i_21 = _mm512_madd52lo_epu64(v512i_13, v512i_17, v512i_20);
    }
    __m512i v512i_22 = _mm512_add_epi8(v512i_17, v512i_17);
    __mmask8 m8_23 = (__mmask8)(g_m8[0] ^ 126);
    __m512d v512d_24 = _mm512_mask_blend_pd(m8_23, v512d_7, v512d_12);
    __m512i v512i_25 = _mm512_sub_epi8(v512i_13, v512i_22);
    __m128d v128d_26 = g_v128d[5];
    __m128d v128d_27 = _mm_unpackhi_pd(v128d_26, v128d_26);
    g_v512i[11] = v512i_13;
    g_v256d[2] = p0;
    g_v128d[10] = v128d_27;
    g_v128i[11] = p3;
    g_v256h[0] = p2;
    g_v128h[10] = p1;
    return v512i_22;
}

__m256h torture_384(__m256d p0, __m256 p1, __m128h p2, __m256d p3) {
    __m512 v512f_1 = g_v512f[9];
    __m512 v512f_2 = _mm512_mul_ps(v512f_1, v512f_1);
    __m512d v512d_3 = g_v512d[5];
    __m512d v512d_4 = _mm512_sqrt_pd(v512d_3);
    __m512h v512h_5 = g_v512h[0];
    __m512h v512h_6 = _mm512_fmsub_ph(v512h_5, v512h_5, v512h_5);
    __m256 v256f_7 = _mm256_add_ps(p1, p1);
    __m512i v512i_8 = g_v512i[7];
    __mmask64 m64_9 = _mm512_movepi8_mask(v512i_8);
    __m512i v512i_10 = _mm512_add_epi8(v512i_8, v512i_8);
    __m512 v512f_11 = _mm512_xor_ps(v512f_2, v512f_1);
    __m128 v128f_12 = g_v128f[8];
    __m128 v128f_13 = _mm_sqrt_ps(v128f_12);
    __m128i v128i_14 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_15 = _mm_sub_epi8(v128i_14, v128i_14);
    __mmask8 m8_16 = _mm512_cmp_pd_mask(v512d_3, v512d_4, _CMP_GE_OQ);
    __m512 v512f_17 = _mm512_set1_ps(1.0f);
    __m256 v256f_18 = _mm256_sub_ps(v256f_7, p1);
    __mmask16 m16_19 = _mm512_testn_epi32_mask(v512i_8, v512i_8);
    __m256i v256i_20 = _mm512_cvtepi64_epi32(v512i_10);
    __m128i v128i_21 = _mm_mullo_epi32(v128i_15, v128i_15);
    __m256i v256i_22 = _mm256_abs_epi32(v256i_20);
    __m256i v256i_23 = _mm512_cvtepi64_epi32(v512i_8);
    __m512d v512d_24 = _mm512_unpackhi_pd(v512d_4, v512d_3);
    __mmask16 m16_25 = _kshiftri_mask16(m16_19, 1);
    __m256 v256f_26 = _mm256_sub_ps(p1, p1);
    __m512 v512f_27 = _mm512_scalef_ps(v512f_11, v512f_17);
    __m128i v128i_28 = _mm_sub_epi16(v128i_15, v128i_15);
    __m512h v512h_29 = _mm512_fmsub_ph(v512h_5, v512h_5, v512h_6);
    g_v512f[7] = v512f_17;
    g_v256d[2] = p0;
    g_v256i[8] = v256i_22;
    g_v128i[3] = v128i_28;
    g_v512h[1] = v512h_5;
    __m256h v256h_30 = g_v256h[4];
    return v256h_30;
}

__mmask64 torture_385(__m256h p0, __m256 p1, __m256d p2, __m512d p3, __m128 p4) {
    __m128 v128f_1 = _mm_unpacklo_ps(p4, p4);
    __m512 v512f_2 = _mm512_cvtxph_ps(p0);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_popcnt_epi64(v512i_3);
    __m512d v512d_5 = _mm512_add_pd(p3, p3);
    __m128i v128i_6 = g_v128i[5];
    __m128i v128i_7 = _mm_or_si128(v128i_6, v128i_6);
    __m512d v512d_8 = _mm512_mul_pd(p3, p3);
    __m256 v256f_9 = _mm256_add_ps(p1, p1);
    __m256i v256i_10 = g_v256i[10];
    __m256i v256i_11 = _mm256_sub_epi32(v256i_10, v256i_10);
    int s32_12 = 32;
    if ((s32_12) & 1ULL) {
    __m128d v128d_13 = g_v128d[0];
    __m128d v128d_14 = _mm_max_pd(v128d_13, v128d_13);
    __m128i v128i_15 = _mm_set1_epi32(0x55);
    }
    __m512 v512f_16 = _mm512_mul_ps(v512f_2, v512f_2);
    __m512d v512d_17 = _mm512_shuffle_f64x2(v512d_5, v512d_8, 0xEE);
    __m256i v256i_18 = _mm256_add_epi32(v256i_10, v256i_10);
    __m512 v512f_19 = _mm512_max_ps(v512f_16, v512f_16);
    for (int i_20 = 0; i_20 < (int)((s32_12) & 7); i_20++) {
    __m128i v128i_21 = _mm_xor_si128(v128i_7, v128i_6);
    __m256 v256f_22 = _mm256_sub_ps(v256f_9, v256f_9);
    __m512 v512f_23 = _mm512_shuffle_f32x4(v512f_16, v512f_16, 0x4E);
    }
    g_v512f[6] = v512f_19;
    g_v512d[0] = v512d_17;
    g_v256i[0] = v256i_11;
    g_v128i[7] = v128i_6;
    __mmask64 m64_24 = (__mmask64)(g_m64[0] ^ 233);
    return m64_24;
}

__m256h torture_386(__m256d p0, __m128 p1) {
    __m256 v256f_1 = _mm256_loadu_ps(fbuf);
    __m512d v512d_2 = _mm512_cvtps_pd(v256f_1);
    __m256 v256f_3 = _mm256_sqrt_ps(v256f_1);
    __m512i v512i_4 = g_v512i[9];
    __m512i v512i_5 = _mm512_add_epi64(v512i_4, v512i_4);
    __m256d v256d_6 = _mm512_extractf64x4_pd(v512d_2, 1);
    __m512h v512h_7 = _mm512_cvtepi16_ph(v512i_4);
    __mmask8 m8_8 = (__mmask8)(g_m8[1] ^ 250);
    __m512i v512i_9 = _mm512_broadcastmb_epi64(m8_8);
    __m128i v128i_10 = _mm512_cvtepi32_epi8(v512i_4);
    __m512h v512h_11 = _mm512_fmsub_ph(v512h_7, v512h_7, v512h_7);
    __m128i v128i_12 = _mm_add_epi32(v128i_10, v128i_10);
    __m512i v512i_13 = _mm512_cvtepi8_epi32(v128i_12);
    __mmask16 m16_14 = _mm512_cmp_epi32_mask(v512i_4, v512i_4, 2);
    __m256i v256i_15 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_16 = _mm256_mullo_epi32(v256i_15, v256i_15);
    __m128d v128d_17 = g_v128d[11];
    __m128d v128d_18 = _mm_sqrt_pd(v128d_17);
    __m256i v256i_19 = _mm256_sllv_epi32(v256i_15, v256i_15);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m256i v256i_20 = _mm256_add_epi32(v256i_16, v256i_19);
    __m128i v128i_21 = _mm_sllv_epi32(v128i_12, v128i_10);
    }
    __m512 v512f_22 = _mm512_set1_ps(1.0f);
    __m512h v512h_23 = _mm512_fmsub_ph(v512h_7, v512h_7, v512h_11);
    __m512d v512d_24 = _mm512_mul_pd(v512d_2, v512d_2);
    __mmask64 m64_25 = _mm512_cmp_epi8_mask(v512i_9, v512i_4, 1);
    __m256i v256i_26 = _mm256_add_epi8(v256i_16, v256i_16);
    __m512d v512d_27 = _mm512_sqrt_pd(v512d_2);
    __m512i v512i_28 = _mm512_add_epi64(v512i_9, v512i_13);
    __m512i v512i_29 = _mm512_permutex2var_epi32(v512i_4, v512i_5, v512i_13);
    __m512i v512i_30 = _mm512_and_si512(v512i_13, v512i_29);
    g_v256f[3] = v256f_1;
    g_v128f[8] = p1;
    g_m8[2] = m8_8;
    g_m16[1] = m16_14;
    __m256h v256h_31 = g_v256h[2];
    return v256h_31;
}

__m256h torture_387(__m256i p0, __m256h p1, __m128 p2) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_mullo_epi32(v128i_1, v128i_1);
    __m512i v512i_3 = g_v512i[7];
    __m512i v512i_4 = _mm512_shuffle_i32x4(v512i_3, v512i_3, 0x1B);
    __m128 v128f_5 = _mm_fmadd_ps(p2, p2, p2);
    __m256d v256d_6 = g_v256d[8];
    __m256d v256d_7 = _mm256_sub_pd(v256d_6, v256d_6);
    __m512d v512d_8 = g_v512d[7];
    __m512d v512d_9 = _mm512_add_pd(v512d_8, v512d_8);
    __m128 v128f_10 = _mm_sqrt_ps(p2);
    __m256 v256f_11 = g_v256f[11];
    __m256 v256f_12 = _mm256_shuffle_ps(v256f_11, v256f_11, 0x1B);
    __m128i v128i_13 = _mm_slli_epi32(v128i_1, 3);
    __m512 v512f_14 = g_v512f[8];
    __m512 v512f_15 = _mm512_xor_ps(v512f_14, v512f_14);
    __mmask16 m16_16 = (__mmask16)(g_m16[7] ^ 76);
    __m512 v512f_17 = _mm512_mask3_fmadd_ps(v512f_14, v512f_14, v512f_15, m16_16);
    __m512i v512i_18 = _mm512_sllv_epi32(v512i_3, v512i_4);
    __m256i v256i_19 = _mm256_sub_epi32(p0, p0);
    __m512d v512d_20 = _mm512_add_pd(v512d_8, v512d_8);
    __m512 v512f_21 = _mm512_min_ps(v512f_17, v512f_14);
    __m256i v256i_22 = _mm256_shuffle_epi8(v256i_19, v256i_19);
    __m512i v512i_23 = _mm512_dpwssd_epi32(v512i_18, v512i_4, v512i_18);
    __m512h v512h_24 = g_v512h[7];
    __mmask32 m32_25 = _mm512_cmp_ph_mask(v512h_24, v512h_24, 1);
    __m256 v256f_26 = _mm256_set1_ps(1.0f);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m512 v512f_27 = _mm512_cvtxph_ps(p1);
    __m128 v128f_28 = _mm_xor_ps(v128f_5, v128f_10);
    __m256i v256i_29 = _mm256_sllv_epi32(v256i_22, v256i_22);
    }
    __m512d v512d_30 = _mm512_max_pd(v512d_9, v512d_8);
    __m256 v256f_31 = _mm256_and_ps(v256f_11, v256f_12);
    __m128i v128i_32 = _mm512_cvtepi32_epi8(v512i_23);
    __m128d v128d_33 = g_v128d[10];
    __m128d v128d_34 = _mm_mul_pd(v128d_33, v128d_33);
    __m128i v128i_35 = _mm_add_epi16(v128i_13, v128i_2);
    __m512 v512f_36 = _mm512_set1_ps(1.0f);
    __m512 v512f_37 = _mm512_fnmadd_ps(v512f_17, v512f_14, v512f_17);
    if (((unsigned long long)m16_16) & 1ULL) {
    __m256 v256f_38 = _mm256_mul_ps(v256f_26, v256f_12);
    __m256d v256d_39 = _mm256_div_pd(v256d_6, v256d_6);
    }
    g_v512d[9] = v512d_30;
    g_v256f[0] = v256f_12;
    g_v128i[0] = v128i_35;
    return p1;
}

__mmask16 torture_388(__m256 p0, __m128i p1, __m128h p2, __m256h p3, __m128 p4) {
    __m256d v256d_1 = g_v256d[0];
    __m256d v256d_2 = _mm256_fmadd_pd(v256d_1, v256d_1, v256d_1);
    __m128 v128f_3 = _mm_xor_ps(p4, p4);
    __m512i v512i_4 = g_v512i[7];
    __mmask16 m16_5 = _mm512_test_epi32_mask(v512i_4, v512i_4);
    __m128d v128d_6 = g_v128d[11];
    __m128d v128d_7 = _mm_fmadd_pd(v128d_6, v128d_6, v128d_6);
    __m512i v512i_8 = _mm512_add_epi16(v512i_4, v512i_4);
    __m256i v256i_9 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_10 = _mm256_sub_epi8(v256i_9, v256i_9);
    __m512i v512i_11 = _mm512_add_epi64(v512i_4, v512i_4);
    __m128 v128f_12 = _mm_fmsub_ps(p4, p4, p4);
    __m128i v128i_13 = _mm_xor_si128(p1, p1);
    __m512d v512d_14 = _mm512_cvtph_pd(p2);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_5) & 7); i_15++) {
    __mmask16 m16_16 = _mm512_testn_epi32_mask(v512i_4, v512i_11);
    __m512 v512f_17 = _mm512_loadu_ps(fbuf);
    __m512 v512f_18 = _mm512_sub_ps(v512f_17, v512f_17);
    __m256i v256i_19 = _mm256_slli_epi32(v256i_10, 3);
    __m256d v256d_20 = _mm256_min_pd(v256d_1, v256d_1);
    __m512i v512i_21 = _mm512_sub_epi16(v512i_4, v512i_8);
    }
    __m512d v512d_22 = _mm512_shuffle_f64x2(v512d_14, v512d_14, 0xEE);
    __m512 v512f_23 = _mm512_loadu_ps(fbuf);
    __m512 v512f_24 = _mm512_fmsub_ps(v512f_23, v512f_23, v512f_23);
    __m512d v512d_25 = _mm512_cvtps_pd(p0);
    __m256d v256d_26 = _mm256_unpackhi_pd(v256d_2, v256d_1);
    for (int i_27 = 0; i_27 < (int)(((unsigned long long)m16_5) & 7); i_27++) {
    __m512h v512h_28 = g_v512h[2];
    __m512i v512i_29 = _mm512_cvtph_epi16(v512h_28);
    __m128 v128f_30 = _mm_fmsub_ps(v128f_12, v128f_3, v128f_12);
    }
    g_v512d[0] = v512d_25;
    g_v256f[3] = p0;
    g_v128d[9] = v128d_7;
    g_v128i[9] = p1;
    g_m16[8] = m16_5;
    return m16_5;
}

__mmask16 torture_389(__m128 p0, __m512 p1) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sub_epi32(v256i_1, v256i_1);
    __m256i v256i_3 = _mm256_sllv_epi32(v256i_2, v256i_2);
    __m128d v128d_4 = g_v128d[6];
    __m128d v128d_5 = _mm_sqrt_pd(v128d_4);
    __m128i v128i_6 = g_v128i[2];
    __m128i v128i_7 = _mm_unpacklo_epi32(v128i_6, v128i_6);
    __m256 v256f_8 = _mm256_loadu_ps(fbuf);
    __m512d v512d_9 = _mm512_cvtps_pd(v256f_8);
    __m256 v256f_10 = _mm256_xor_ps(v256f_8, v256f_8);
    __mmask16 m16_11 = (__mmask16)(g_m16[1] ^ 176);
    __mmask16 m16_12 = _kxor_mask16(m16_11, m16_11);
    __m256d v256d_13 = g_v256d[0];
    __m256d v256d_14 = _mm256_fmadd_pd(v256d_13, v256d_13, v256d_13);
    __m512d v512d_15 = _mm512_shuffle_f64x2(v512d_9, v512d_9, 0xEE);
    __m128i v128i_16 = _mm_add_epi64(v128i_7, v128i_7);
    __m512 v512f_17 = _mm512_sub_ps(p1, p1);
    __m512i v512i_18 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_19 = _mm512_shuffle_i32x4(v512i_18, v512i_18, 0x1B);
    __m512h v512h_20 = g_v512h[3];
    __m512h v512h_21 = _mm512_getexp_ph(v512h_20);
    __m512 v512f_22 = _mm512_getexp_ps(v512f_17);
    __m512i v512i_23 = _mm512_slli_epi32(v512i_18, 3);
    __mmask16 m16_24 = _mm512_test_epi32_mask(v512i_18, v512i_23);
    __mmask16 m16_25 = _kshiftli_mask16(m16_24, 2);
    __m128 v128f_26 = _mm_fmsub_ps(p0, p0, p0);
    __mmask16 m16_27 = _kshiftri_mask16(m16_25, 1);
    __m512i v512i_28 = _mm512_sub_epi64(v512i_18, v512i_23);
    __m128 v128f_29 = _mm_min_ps(v128f_26, v128f_26);
    __m256d v256d_30 = _mm512_extractf64x4_pd(v512d_9, 1);
    __m128i v128i_31 = _mm_slli_epi32(v128i_6, 3);
    __m512i v512i_32 = _mm512_mullo_epi32(v512i_28, v512i_23);
    if (((unsigned long long)m16_24) & 1ULL) {
    __m512i v512i_33 = _mm512_add_epi32(v512i_28, v512i_32);
    __m512i v512i_34 = _mm512_cvtepi8_epi32(v128i_7);
    }
    g_v512f[3] = p1;
    g_v512i[8] = v512i_19;
    g_v256f[3] = v256f_10;
    g_v128f[9] = v128f_29;
    g_v512h[0] = v512h_21;
    g_m16[5] = m16_11;
    return m16_25;
}

__m512h torture_390(__m128d p0, __m128h p1) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_or_si256(v256i_1, v256i_1);
    __m512i v512i_3 = _mm512_loadu_si512((const void*)ibuf);
    __m512i v512i_4 = _mm512_and_si512(v512i_3, v512i_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512 v512f_6 = _mm512_roundscale_ps(v512f_5, 0x10);
    __mmask64 m64_7 = (__mmask64)(g_m64[10] ^ 241);
    __mmask64 m64_8 = _kor_mask64(m64_7, m64_7);
    __m256 v256f_9 = _mm256_loadu_ps(fbuf);
    __m256 v256f_10 = _mm256_fmadd_ps(v256f_9, v256f_9, v256f_9);
    __m256d v256d_11 = g_v256d[1];
    __m256d v256d_12 = _mm256_div_pd(v256d_11, v256d_11);
    __m256 v256f_13 = _mm256_max_ps(v256f_9, v256f_10);
    __mmask16 m16_14 = (__mmask16)(g_m16[0] ^ 81);
    __m512 v512f_15 = _mm512_mask3_fmadd_ps(v512f_5, v512f_5, v512f_6, m16_14);
    __m128 v128f_16 = _mm_loadu_ps(fbuf);
    __m128 v128f_17 = _mm_add_ps(v128f_16, v128f_16);
    __m128i v128i_18 = g_v128i[6];
    __m128i v128i_19 = _mm_add_epi8(v128i_18, v128i_18);
    __m512i v512i_20 = _mm512_broadcastmw_epi32(m16_14);
    __m512i v512i_21 = _mm512_slli_epi32(v512i_4, 3);
    __m128 v128f_22 = _mm_add_ps(v128f_16, v128f_16);
    __m128i v128i_23 = _mm_sub_epi32(v128i_18, v128i_19);
    __m512 v512f_24 = _mm512_fmadd_ps(v512f_15, v512f_5, v512f_15);
    __mmask8 m8_25 = (__mmask8)(g_m8[3] ^ 233);
    __mmask16 m16_26 = _mm512_kunpackb(m8_25, m8_25);
    __m256i v256i_27 = _mm256_add_epi64(v256i_1, v256i_2);
    unsigned u32_28 = 57;
    __mmask16 m16_29 = _cvtu32_mask16(u32_28);
    __m128d v128d_30 = _mm_unpackhi_pd(p0, p0);
    __m512i v512i_31 = _mm512_abs_epi32(v512i_3);
    __mmask16 m16_32 = _kshiftli_mask16(m16_29, 2);
    __m512i v512i_33 = _mm512_sub_epi32(v512i_31, v512i_4);
    __m512h v512h_34 = g_v512h[8];
    __m512h v512h_35 = _mm512_rcp_ph(v512h_34);
    __m256i v256i_36 = _mm256_add_epi64(v256i_27, v256i_1);
    if (((unsigned long long)m16_14) & 1ULL) {
    __m512i v512i_37 = _mm512_add_epi32(v512i_20, v512i_33);
    __m512h v512h_38 = _mm512_getexp_ph(v512h_35);
    }
    g_v512f[11] = v512f_6;
    g_v512i[2] = v512i_20;
    g_v256d[10] = v256d_12;
    g_v128f[7] = v128f_17;
    g_m16[2] = m16_14;
    g_m64[0] = m64_8;
    return v512h_35;
}

__m512d torture_391(__m128 p0, __m512 p1, __m512h p2) {
    __m512i v512i_1 = _mm512_loadu_si512((const void*)ibuf);
    __mmask64 m64_2 = _mm512_movepi8_mask(v512i_1);
    __m128 v128f_3 = _mm_sqrt_ps(p0);
    __m512d v512d_4 = _mm512_loadu_pd(dbuf);
    __m512d v512d_5 = _mm512_fmadd_pd(v512d_4, v512d_4, v512d_4);
    __mmask16 m16_6 = (__mmask16)(g_m16[4] ^ 235);
    __m512 v512f_7 = _mm512_mask3_fmadd_ps(p1, p1, p1, m16_6);
    __m128i v128i_8 = g_v128i[9];
    __m128i v128i_9 = _mm_add_epi64(v128i_8, v128i_8);
    __m512 v512f_10 = _mm512_fnmadd_ps(v512f_7, p1, v512f_7);
    __m512h v512h_11 = _mm512_rcp_ph(p2);
    __m128i v128i_12 = _mm_sub_epi32(v128i_9, v128i_9);
    __m256i v256i_13 = g_v256i[7];
    __m256i v256i_14 = _mm256_sub_epi64(v256i_13, v256i_13);
    __m256 v256f_15 = _mm256_loadu_ps(fbuf);
    __m512d v512d_16 = _mm512_cvtps_pd(v256f_15);
    __mmask16 m16_17 = _mm512_cmp_epi32_mask(v512i_1, v512i_1, 2);
    __mmask32 m32_18 = _mm512_kunpackw(m16_17, m16_17);
    __m256 v256f_19 = _mm256_min_ps(v256f_15, v256f_15);
    __m128 v128f_20 = _mm_fmsub_ps(p0, v128f_3, v128f_3);
    for (int i_21 = 0; i_21 < (int)(((unsigned long long)m16_6) & 7); i_21++) {
    __mmask32 m32_22 = _mm512_cmp_ph_mask(v512h_11, v512h_11, 1);
    __m512i v512i_23 = _mm512_and_si512(v512i_1, v512i_1);
    }
    __m256i v256i_24 = _mm256_and_si256(v256i_14, v256i_13);
    __m256i v256i_25 = _mm256_set1_epi32(0x55);
    __m128h v128h_26 = g_v128h[10];
    __m512d v512d_27 = _mm512_cvtph_pd(v128h_26);
    __m512i v512i_28 = _mm512_ternarylogic_epi32(v512i_1, v512i_1, v512i_1, 0x96);
    __m512i v512i_29 = _mm512_shuffle_i32x4(v512i_1, v512i_28, 0x1B);
    __mmask64 m64_30 = _mm512_cmp_epi8_mask(v512i_28, v512i_29, 1);
    __mmask16 m16_31 = _mm512_movepi32_mask(v512i_29);
    __m512 v512f_32 = _mm512_sqrt_ps(v512f_10);
    __m128d v128d_33 = g_v128d[4];
    __m128d v128d_34 = _mm_unpackhi_pd(v128d_33, v128d_33);
    __m512 v512f_35 = _mm512_insertf32x8(v512f_7, v256f_15, 1);
    __m256d v256d_36 = g_v256d[11];
    __m256d v256d_37 = _mm256_mul_pd(v256d_36, v256d_36);
    for (int i_38 = 0; i_38 < (int)(((unsigned long long)m16_31) & 7); i_38++) {
    __m512d v512d_39 = _mm512_mul_pd(v512d_27, v512d_5);
    __m512i v512i_40 = _mm512_and_si512(v512i_1, v512i_1);
    __m256i v256i_41 = _mm256_set1_epi32(0x55);
    }
    g_v512i[2] = v512i_29;
    g_v256d[4] = v256d_36;
    g_v128i[5] = v128i_8;
    return v512d_27;
}

__m256i torture_392(__m256d p0, __m512h p1, __m128d p2, __m128 p3) {
    __mmask16 m16_1 = (__mmask16)(g_m16[6] ^ 4);
    __mmask16 m16_2 = _kshiftli_mask16(m16_1, 2);
    __mmask8 m8_3 = (__mmask8)(g_m8[2] ^ 246);
    __m512d v512d_4 = _mm512_loadu_pd(dbuf);
    __m512d v512d_5 = _mm512_mask_blend_pd(m8_3, v512d_4, v512d_4);
    __m256 v256f_6 = _mm256_loadu_ps(fbuf);
    __m256 v256f_7 = _mm256_and_ps(v256f_6, v256f_6);
    __m128i v128i_8 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_9 = _mm_unpacklo_epi32(v128i_8, v128i_8);
    __m256 v256f_10 = _mm256_min_ps(v256f_7, v256f_7);
    __m512i v512i_11 = g_v512i[5];
    __m512i v512i_12 = _mm512_xor_si512(v512i_11, v512i_11);
    __m128i v128i_13 = _mm_set1_epi32(0x55);
    __mmask16 m16_14 = _kshiftri_mask16(m16_2, 1);
    __m512d v512d_15 = _mm512_mask_blend_pd(m8_3, v512d_5, v512d_5);
    __m512i v512i_16 = _mm512_abs_epi32(v512i_11);
    __m128h v128h_17 = g_v128h[3];
    __m512d v512d_18 = _mm512_cvtph_pd(v128h_17);
    __m512i v512i_19 = _mm512_mullo_epi32(v512i_11, v512i_16);
    __mmask16 m16_20 = _mm512_testn_epi32_mask(v512i_12, v512i_11);
    if (((unsigned long long)m16_1) & 1ULL) {
    __m256i v256i_21 = g_v256i[7];
    __m256i v256i_22 = _mm256_sub_epi8(v256i_21, v256i_21);
    __m512i v512i_23 = _mm512_sub_epi8(v512i_12, v512i_11);
    __m256 v256f_24 = _mm256_shuffle_ps(v256f_6, v256f_10, 0x1B);
    __m512i v512i_25 = _mm512_dpwssd_epi32(v512i_11, v512i_11, v512i_19);
    }
    __m512i v512i_26 = _mm512_or_si512(v512i_16, v512i_12);
    __m128 v128f_27 = _mm_fmsub_ps(p3, p3, p3);
    __m512i v512i_28 = _mm512_sub_epi16(v512i_16, v512i_11);
    __m512 v512f_29 = g_v512f[4];
    __m512 v512f_30 = _mm512_add_ps(v512f_29, v512f_29);
    __mmask32 m32_31 = _mm512_cmp_ph_mask(p1, p1, 1);
    __m256i v256i_32 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_33 = _mm256_sub_epi64(v256i_32, v256i_32);
    __m512i v512i_34 = _mm512_mask_blend_epi32(m16_14, v512i_11, v512i_28);
    __m512d v512d_35 = _mm512_mul_pd(v512d_5, v512d_15);
    for (int i_36 = 0; i_36 < (int)(((unsigned long long)m16_20) & 7); i_36++) {
    __m512 v512f_37 = _mm512_roundscale_ps(v512f_29, 0x10);
    __m128i v128i_38 = _mm_add_epi16(v128i_9, v128i_13);
    __m256 v256f_39 = _mm256_fnmadd_ps(v256f_7, v256f_10, v256f_10);
    __m256 v256f_40 = _mm256_min_ps(v256f_7, v256f_39);
    }
    g_v512f[11] = v512f_29;
    g_v256f[3] = v256f_7;
    g_v256i[2] = v256i_33;
    g_v128i[5] = v128i_13;
    g_v512h[3] = p1;
    return v256i_33;
}

__m128d torture_393(__m512i p0, __m512d p1) {
    __m256i v256i_1 = g_v256i[11];
    __m256i v256i_2 = _mm256_xor_si256(v256i_1, v256i_1);
    __m512 v512f_3 = _mm512_loadu_ps(fbuf);
    __m512 v512f_4 = _mm512_shuffle_ps(v512f_3, v512f_3, 0x1B);
    __m512d v512d_5 = _mm512_max_pd(p1, p1);
    __m512i v512i_6 = _mm512_dpbusd_epi32(p0, p0, p0);
    __m128d v128d_7 = g_v128d[10];
    __m128d v128d_8 = _mm_max_pd(v128d_7, v128d_7);
    __m512d v512d_9 = _mm512_min_pd(p1, v512d_5);
    __m512h v512h_10 = _mm512_cvtepi16_ph(p0);
    __m256d v256d_11 = g_v256d[8];
    __m256d v256d_12 = _mm256_add_pd(v256d_11, v256d_11);
    __mmask16 m16_13 = (__mmask16)(g_m16[6] ^ 222);
    __mmask16 m16_14 = _kshiftri_mask16(m16_13, 1);
    for (int i_15 = 0; i_15 < (int)(((unsigned long long)m16_13) & 7); i_15++) {
    __m512i v512i_16 = _mm512_shuffle_epi8(v512i_6, v512i_6);
    __m512h v512h_17 = _mm512_fmul_pch(v512h_10, v512h_10);
    __mmask16 m16_18 = _mm512_cmp_epu32_mask(v512i_6, p0, 4);
    __m512i v512i_19 = _mm512_unpacklo_epi32(v512i_6, v512i_6);
    }
    __m512i v512i_20 = _mm512_add_epi16(p0, p0);
    __m256i v256i_21 = _mm256_sub_epi16(v256i_2, v256i_1);
    __m512i v512i_22 = _mm512_add_epi8(p0, v512i_6);
    __m256i v256i_23 = _mm256_or_si256(v256i_2, v256i_21);
    __m128d v128d_24 = _mm_fmadd_pd(v128d_8, v128d_7, v128d_7);
    __m256i v256i_25 = _mm256_sub_epi16(v256i_2, v256i_1);
    __m512 v512f_26 = _mm512_unpacklo_ps(v512f_3, v512f_3);
    __m256 v256f_27 = _mm256_loadu_ps(fbuf);
    __m256 v256f_28 = _mm256_xor_ps(v256f_27, v256f_27);
    __m256d v256d_29 = _mm256_min_pd(v256d_12, v256d_12);
    g_v512i[2] = p0;
    g_v256f[0] = v256f_28;
    g_v128d[5] = v128d_24;
    g_v512h[8] = v512h_10;
    g_m16[6] = m16_13;
    return v128d_7;
}

__m512d torture_394(__m128 p0, __m128d p1, __m128h p2) {
    __m128 v128f_1 = _mm_min_ps(p0, p0);
    __m512i v512i_2 = g_v512i[7];
    __m512i v512i_3 = _mm512_sub_epi8(v512i_2, v512i_2);
    __m512 v512f_4 = _mm512_loadu_ps(fbuf);
    __m512 v512f_5 = _mm512_roundscale_ps(v512f_4, 0x10);
    __mmask16 m16_6 = _mm512_testn_epi32_mask(v512i_2, v512i_3);
    __mmask16 m16_7 = _kshiftri_mask16(m16_6, 1);
    __m512d v512d_8 = g_v512d[10];
    __m512d v512d_9 = _mm512_sqrt_pd(v512d_8);
    __m256i v256i_10 = g_v256i[2];
    __m256i v256i_11 = _mm256_and_si256(v256i_10, v256i_10);
    __m128d v128d_12 = _mm_sqrt_pd(p1);
    if (((unsigned long long)m16_6) & 1ULL) {
    __m128i v128i_13 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_14 = _mm_add_epi64(v128i_13, v128i_13);
    __mmask16 m16_15 = _kshiftri_mask16(m16_7, 1);
    __m256i v256i_16 = _mm256_sub_epi32(v256i_10, v256i_10);
    }
    __m256i v256i_17 = _mm256_shuffle_epi8(v256i_11, v256i_11);
    __m128i v128i_18 = _mm_loadu_si128((const void*)ibuf);
    __m512i v512i_19 = _mm512_cvtepi8_epi32(v128i_18);
    __m256d v256d_20 = _mm512_extractf64x4_pd(v512d_8, 1);
    __m512i v512i_21 = _mm512_rolv_epi32(v512i_19, v512i_19);
    __m256 v256f_22 = g_v256f[5];
    __m256 v256f_23 = _mm256_add_ps(v256f_22, v256f_22);
    __m256 v256f_24 = _mm256_add_ps(v256f_23, v256f_23);
    g_v256f[9] = v256f_22;
    g_v128i[3] = v128i_18;
    g_v128h[10] = p2;
    return v512d_8;
}

__mmask64 torture_395(__m256h p0, __m128d p1, __m512d p2, __m128i p3, __m256d p4) {
    __m512 v512f_1 = g_v512f[7];
    __m512 v512f_2 = _mm512_fmadd_ps(v512f_1, v512f_1, v512f_1);
    __m128i v128i_3 = _mm_sub_epi64(p3, p3);
    __m512i v512i_4 = g_v512i[7];
    __m512h v512h_5 = _mm512_cvtepi16_ph(v512i_4);
    __m128i v128i_6 = _mm_slli_epi32(p3, 3);
    __mmask8 m8_7 = (__mmask8)(g_m8[11] ^ 175);
    __m512d v512d_8 = _mm512_mask_blend_pd(m8_7, p2, p2);
    __m128i v128i_9 = _mm_slli_epi32(v128i_3, 3);
    __m128i v128i_10 = _mm_add_epi64(v128i_9, p3);
    __m256i v256i_11 = g_v256i[1];
    __m256i v256i_12 = _mm256_shuffle_epi8(v256i_11, v256i_11);
    __m256 v256f_13 = _mm256_loadu_ps(fbuf);
    __m256 v256f_14 = _mm256_min_ps(v256f_13, v256f_13);
    __m128 v128f_15 = g_v128f[0];
    __m128 v128f_16 = _mm_and_ps(v128f_15, v128f_15);
    __m512i v512i_17 = _mm512_cvtph_epi16(v512h_5);
    __m512i v512i_18 = _mm512_sub_epi64(v512i_17, v512i_4);
    __m512d v512d_19 = _mm512_sub_pd(v512d_8, p2);
    __m512i v512i_20 = _mm512_xor_si512(v512i_4, v512i_17);
    __m512i v512i_21 = _mm512_ternarylogic_epi32(v512i_20, v512i_17, v512i_17, 0x96);
    if (((unsigned long long)m8_7) & 1ULL) {
    __m256i v256i_22 = _mm256_mullo_epi32(v256i_11, v256i_12);
    __m128 v128f_23 = _mm_xor_ps(v128f_16, v128f_15);
    }
    __m128 v128f_24 = _mm_fmsub_ps(v128f_16, v128f_16, v128f_15);
    __m512i v512i_25 = _mm512_sllv_epi32(v512i_21, v512i_20);
    __m512 v512f_26 = _mm512_unpacklo_ps(v512f_1, v512f_2);
    __m512 v512f_27 = _mm512_set1_ps(1.0f);
    __m512 v512f_28 = _mm512_div_ps(v512f_2, v512f_2);
    __m512 v512f_29 = _mm512_cvtepi32_ps(v512i_21);
    __mmask16 m16_30 = (__mmask16)(g_m16[7] ^ 67);
    __mmask16 m16_31 = _kxor_mask16(m16_30, m16_30);
    g_v512d[0] = v512d_8;
    g_v256f[0] = v256f_13;
    g_v256i[10] = v256i_11;
    g_v128f[3] = v128f_24;
    __mmask64 m64_32 = (__mmask64)(g_m64[7] ^ 100);
    return m64_32;
}

__m512h torture_396(__m512 p0, __m128h p1, __m256d p2, __m256i p3) {
    __m256i v256i_1 = _mm256_sllv_epi32(p3, p3);
    __m512i v512i_2 = g_v512i[5];
    __m512i v512i_3 = _mm512_ternarylogic_epi32(v512i_2, v512i_2, v512i_2, 0x96);
    __mmask16 m16_4 = (__mmask16)(g_m16[7] ^ 27);
    __mmask16 m16_5 = _kor_mask16(m16_4, m16_4);
    __m128 v128f_6 = g_v128f[3];
    __m128 v128f_7 = _mm_sub_ps(v128f_6, v128f_6);
    __m512i v512i_8 = _mm512_set1_epi32(0x55);
    __m256 v256f_9 = g_v256f[10];
    __m256 v256f_10 = _mm256_fnmadd_ps(v256f_9, v256f_9, v256f_9);
    __mmask16 m16_11 = _kxor_mask16(m16_5, m16_4);
    __m512i v512i_12 = _mm512_sllv_epi32(v512i_3, v512i_3);
    __m512i v512i_13 = _mm512_sllv_epi32(v512i_8, v512i_12);
    __m512i v512i_14 = _mm512_conflict_epi32(v512i_13);
    __m512i v512i_15 = _mm512_slli_epi32(v512i_3, 3);
    __m512i v512i_16 = _mm512_ternarylogic_epi32(v512i_15, v512i_15, v512i_13, 0x96);
    __m512i v512i_17 = _mm512_xor_si512(v512i_3, v512i_13);
    for (int i_18 = 0; i_18 < (int)(((unsigned long long)m16_5) & 7); i_18++) {
    __mmask64 m64_19 = _mm512_movepi8_mask(v512i_12);
    __m256i v256i_20 = _mm256_sub_epi8(p3, v256i_1);
    }
    __m128 v128f_21 = _mm_mul_ps(v128f_6, v128f_7);
    __m128i v128i_22 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_23 = _mm_unpacklo_epi32(v128i_22, v128i_22);
    __m512i v512i_24 = _mm512_rolv_epi32(v512i_8, v512i_15);
    __m512i v512i_25 = _mm512_madd52lo_epu64(v512i_16, v512i_12, v512i_24);
    unsigned u32_26 = _cvtmask16_u32(m16_11);
    __m256d v256d_27 = _mm256_min_pd(p2, p2);
    __mmask16 m16_28 = _kshiftri_mask16(m16_11, 1);
    __m128i v128i_29 = _mm_unpacklo_epi32(v128i_23, v128i_23);
    __m256d v256d_30 = _mm256_sqrt_pd(p2);
    __m512d v512d_31 = _mm512_loadu_pd(dbuf);
    __m256d v256d_32 = _mm512_extractf64x4_pd(v512d_31, 1);
    __m512 v512f_33 = _mm512_min_ps(p0, p0);
    g_v512i[0] = v512i_3;
    g_v256f[2] = v256f_9;
    g_v128f[2] = v128f_7;
    __m512h v512h_34 = g_v512h[5];
    return v512h_34;
}

__m256h torture_397(__m512i p0, __m128h p1, __m512i p2, __m512 p3) {
    __m256i v256i_1 = _mm256_loadu_si256((const void*)ibuf);
    __m256i v256i_2 = _mm256_sub_epi32(v256i_1, v256i_1);
    __m512h v512h_3 = g_v512h[7];
    __m512h v512h_4 = _mm512_rcp_ph(v512h_3);
    __m128d v128d_5 = g_v128d[8];
    __m128d v128d_6 = _mm_min_pd(v128d_5, v128d_5);
    __m512i v512i_7 = _mm512_xor_si512(p0, p2);
    __m512i v512i_8 = _mm512_add_epi16(p0, p0);
    __m512d v512d_9 = _mm512_loadu_pd(dbuf);
    __m512d v512d_10 = _mm512_unpackhi_pd(v512d_9, v512d_9);
    __m512h v512h_11 = _mm512_rcp_ph(v512h_3);
    __m512i v512i_12 = _mm512_mullo_epi32(v512i_8, p2);
    __m128 v128f_13 = _mm_loadu_ps(fbuf);
    __m128 v128f_14 = _mm_sub_ps(v128f_13, v128f_13);
    __m512h v512h_15 = _mm512_mul_ph(v512h_4, v512h_4);
    __m128i v128i_16 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_17 = _mm_or_si128(v128i_16, v128i_16);
    __mmask16 m16_18 = (__mmask16)(g_m16[3] ^ 1);
    __mmask16 m16_19 = _kor_mask16(m16_18, m16_18);
    __m128i v128i_20 = _mm_sub_epi32(v128i_16, v128i_16);
    __m512i v512i_21 = _mm512_permutex2var_epi32(v512i_7, p2, v512i_12);
    if (((unsigned long long)m16_19) & 1ULL) {
    __m128 v128f_22 = _mm_xor_ps(v128f_14, v128f_13);
    __m512d v512d_23 = _mm512_div_pd(v512d_10, v512d_9);
    __m128i v128i_24 = _mm_abs_epi32(v128i_17);
    __mmask8 m8_25 = _mm512_cmp_pd_mask(v512d_10, v512d_10, _CMP_GE_OQ);
    __m256 v256f_26 = _mm256_loadu_ps(fbuf);
    __m512d v512d_27 = _mm512_cvtps_pd(v256f_26);
    }
    __m128 v128f_28 = _mm_fnmadd_ps(v128f_14, v128f_14, v128f_14);
    __m256d v256d_29 = g_v256d[1];
    __m256d v256d_30 = _mm256_sqrt_pd(v256d_29);
    __m512h v512h_31 = _mm512_getexp_ph(v512h_15);
    __m512i v512i_32 = _mm512_broadcastmw_epi32(m16_18);
    unsigned u32_33 = 32;
    __mmask16 m16_34 = _cvtu32_mask16(u32_33);
    __m256i v256i_35 = _mm256_sub_epi32(v256i_1, v256i_2);
    __m512 v512f_36 = _mm512_rcp14_ps(p3);
    __m128i v128i_37 = _mm_sllv_epi32(v128i_20, v128i_16);
    __m256d v256d_38 = _mm256_div_pd(v256d_29, v256d_30);
    for (int i_39 = 0; i_39 < (int)(((unsigned long long)m16_19) & 7); i_39++) {
    __m256d v256d_40 = _mm256_div_pd(v256d_38, v256d_38);
    __m256 v256f_41 = _mm256_loadu_ps(fbuf);
    __m256 v256f_42 = _mm256_fmsub_ps(v256f_41, v256f_41, v256f_41);
    }
    g_v128f[0] = v128f_28;
    g_v128i[9] = v128i_37;
    g_m16[8] = m16_34;
    __m256h v256h_43 = g_v256h[5];
    return v256h_43;
}

__m256h torture_398(__m128 p0, __m256h p1, __m256h p2) {
    __m128i v128i_1 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_2 = _mm_sub_epi8(v128i_1, v128i_1);
    __m512h v512h_3 = g_v512h[1];
    __m512i v512i_4 = _mm512_cvtph_epi16(v512h_3);
    __m512 v512f_5 = _mm512_loadu_ps(fbuf);
    __m512 v512f_6 = _mm512_rcp14_ps(v512f_5);
    __mmask16 m16_7 = (__mmask16)(g_m16[0] ^ 180);
    __m512 v512f_8 = _mm512_mask_add_ps(v512f_5, m16_7, v512f_5, v512f_5);
    __m128i v128i_9 = _mm_add_epi8(v128i_1, v128i_1);
    __m512i v512i_10 = _mm512_dpbusd_epi32(v512i_4, v512i_4, v512i_4);
    __m512 v512f_11 = _mm512_div_ps(v512f_6, v512f_6);
    __m512i v512i_12 = _mm512_mullo_epi32(v512i_4, v512i_4);
    __m512i v512i_13 = _mm512_or_si512(v512i_4, v512i_4);
    __m512i v512i_14 = _mm512_unpacklo_epi32(v512i_13, v512i_13);
    __m128d v128d_15 = g_v128d[7];
    __m128d v128d_16 = _mm_sqrt_pd(v128d_15);
    __m512d v512d_17 = _mm512_loadu_pd(dbuf);
    __m128h v128h_18 = _mm512_cvtpd_ph(v512d_17);
    __m512d v512d_19 = _mm512_sqrt_pd(v512d_17);
    __m512 v512f_20 = _mm512_roundscale_ps(v512f_6, 0x10);
    __m256d v256d_21 = g_v256d[11];
    __m256d v256d_22 = _mm256_add_pd(v256d_21, v256d_21);
    __m512i v512i_23 = _mm512_sub_epi32(v512i_12, v512i_14);
    __m128 v128f_24 = _mm_set1_ps(1.0f);
    __m256d v256d_25 = _mm256_unpackhi_pd(v256d_21, v256d_21);
    __m512 v512f_26 = _mm512_broadcast_f32x4(p0);
    __m512i v512i_27 = _mm512_add_epi16(v512i_12, v512i_4);
    __m256 v256f_28 = _mm256_loadu_ps(fbuf);
    __m256 v256f_29 = _mm256_fmsub_ps(v256f_28, v256f_28, v256f_28);
    __m512 v512f_30 = _mm512_rsqrt14_ps(v512f_8);
    __m128 v128f_31 = _mm_shuffle_ps(p0, v128f_24, 0x1B);
    __mmask16 m16_32 = _kshiftri_mask16(m16_7, 1);
    g_v512f[5] = v512f_20;
    g_v256d[8] = v256d_21;
    g_v128f[9] = v128f_24;
    g_v128d[8] = v128d_15;
    g_v128i[11] = v128i_1;
    g_v512h[0] = v512h_3;
    g_v256h[8] = p1;
    g_v128h[2] = v128h_18;
    return p1;
}

__m128d torture_399(__m128 p0, __m128 p1, __m128d p2) {
    __m256d v256d_1 = g_v256d[1];
    __m256d v256d_2 = _mm256_unpackhi_pd(v256d_1, v256d_1);
    __m512h v512h_3 = g_v512h[11];
    __m512h v512h_4 = _mm512_sqrt_ph(v512h_3);
    __m256d v256d_5 = _mm256_sub_pd(v256d_1, v256d_2);
    __m128d v128d_6 = _mm_div_pd(p2, p2);
    __m256d v256d_7 = _mm256_div_pd(v256d_1, v256d_2);
    __m128 v128f_8 = _mm_fmsub_ps(p0, p0, p0);
    __mmask16 m16_9 = (__mmask16)(g_m16[3] ^ 3);
    unsigned u32_10 = _cvtmask16_u32(m16_9);
    __m512 v512f_11 = _mm512_loadu_ps(fbuf);
    __m512 v512f_12 = _mm512_fmadd_ps(v512f_11, v512f_11, v512f_11);
    __m128i v128i_13 = _mm_loadu_si128((const void*)ibuf);
    __m128i v128i_14 = _mm_sub_epi16(v128i_13, v128i_13);
    __m512 v512f_15 = _mm512_fnmadd_ps(v512f_12, v512f_11, v512f_11);
    __m512d v512d_16 = _mm512_loadu_pd(dbuf);
    __m256d v256d_17 = _mm512_extractf64x4_pd(v512d_16, 1);
    __m128d v128d_18 = _mm_set1_pd(2.0);
    __mmask32 m32_19 = _mm512_kunpackw(m16_9, m16_9);
    __mmask32 m32_20 = _mm512_cmp_ph_mask(v512h_4, v512h_3, 1);
    __m128i v128i_21 = _mm_add_epi8(v128i_13, v128i_14);
    __m128i v128i_22 = _mm_shuffle_epi8(v128i_14, v128i_14);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m128 v128f_23 = _mm_add_ps(p0, p0);
    __m256 v256f_24 = _mm256_loadu_ps(fbuf);
    __m256 v256f_25 = _mm256_mul_ps(v256f_24, v256f_24);
    __m512i v512i_26 = g_v512i[4];
    __mmask16 m16_27 = _mm512_cmp_epu32_mask(v512i_26, v512i_26, 4);
    }
    __m128 v128f_28 = _mm_fmadd_ps(v128f_8, v128f_8, p1);
    __m256d v256d_29 = _mm256_div_pd(v256d_2, v256d_1);
    __m256i v256i_30 = g_v256i[6];
    __m256i v256i_31 = _mm256_sub_epi8(v256i_30, v256i_30);
    __m256d v256d_32 = _mm256_unpackhi_pd(v256d_1, v256d_7);
    __m256 v256f_33 = _mm256_loadu_ps(fbuf);
    __m256 v256f_34 = _mm256_div_ps(v256f_33, v256f_33);
    __m512h v512h_35 = _mm512_sqrt_ph(v512h_4);
    __m512i v512i_36 = g_v512i[4];
    __m512i v512i_37 = _mm512_add_epi16(v512i_36, v512i_36);
    if (((unsigned long long)m16_9) & 1ULL) {
    __m512 v512f_38 = _mm512_add_ps(v512f_15, v512f_12);
    __m512i v512i_39 = _mm512_popcnt_epi64(v512i_37);
    __m512i v512i_40 = _mm512_sub_epi32(v512i_37, v512i_37);
    }
    g_v512f[4] = v512f_11;
    g_v256d[10] = v256d_5;
    g_v128i[6] = v128i_21;
    return v128d_6;
}
