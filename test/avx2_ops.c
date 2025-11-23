#include "avx_test.h"

void run_avx2_ops(TestData *data) {
    __m256i a = _mm256_load_si256((__m256i*)data->i32);
    __m256i b = _mm256_load_si256((__m256i*)(data->i32 + 8));

    // --- Integer Arithmetic (8, 16, 32, 64 bit) ---
    // 8-bit
    sink_m256i(_mm256_add_epi8(a, b));
    sink_m256i(_mm256_sub_epi8(a, b));
    sink_m256i(_mm256_adds_epi8(a, b)); // Saturated
    sink_m256i(_mm256_subs_epi8(a, b));
    sink_m256i(_mm256_avg_epu8(a, b));
    sink_m256i(_mm256_max_epu8(a, b));
    sink_m256i(_mm256_min_epu8(a, b));
    sink_m256i(_mm256_sign_epi8(a, b));
    sink_m256i(_mm256_abs_epi8(a));

    // 16-bit
    sink_m256i(_mm256_add_epi16(a, b));
    sink_m256i(_mm256_sub_epi16(a, b));
    sink_m256i(_mm256_adds_epi16(a, b));
    sink_m256i(_mm256_subs_epi16(a, b));
    sink_m256i(_mm256_mullo_epi16(a, b));
    sink_m256i(_mm256_mulhi_epi16(a, b));
    sink_m256i(_mm256_avg_epu16(a, b));
    sink_m256i(_mm256_max_epi16(a, b));
    sink_m256i(_mm256_min_epi16(a, b));
    sink_m256i(_mm256_sign_epi16(a, b));
    sink_m256i(_mm256_abs_epi16(a));
    sink_m256i(_mm256_hadd_epi16(a, b));
    sink_m256i(_mm256_hadds_epi16(a, b));

    // 32-bit
    sink_m256i(_mm256_add_epi32(a, b));
    sink_m256i(_mm256_sub_epi32(a, b));
    sink_m256i(_mm256_mullo_epi32(a, b));
    sink_m256i(_mm256_max_epi32(a, b));
    sink_m256i(_mm256_min_epi32(a, b));
    sink_m256i(_mm256_abs_epi32(a));
    sink_m256i(_mm256_hadd_epi32(a, b));
    sink_m256i(_mm256_hsub_epi32(a, b));

    // 64-bit
    sink_m256i(_mm256_add_epi64(a, b));
    sink_m256i(_mm256_sub_epi64(a, b));
    sink_m256i(_mm256_mul_epu32(a, b));

    // --- Logical ---
    sink_m256i(_mm256_and_si256(a, b));
    sink_m256i(_mm256_or_si256(a, b));
    sink_m256i(_mm256_xor_si256(a, b));
    sink_m256i(_mm256_andnot_si256(a, b));

    // --- Shifts ---
    sink_m256i(_mm256_slli_epi16(a, 2));
    sink_m256i(_mm256_srai_epi16(a, 2));
    sink_m256i(_mm256_srli_epi16(a, 2));
    sink_m256i(_mm256_slli_epi32(a, 2));
    sink_m256i(_mm256_srai_epi32(a, 2));
    sink_m256i(_mm256_srli_epi32(a, 2));
    sink_m256i(_mm256_slli_epi64(a, 2));
    sink_m256i(_mm256_srli_epi64(a, 2));

    // Variable shifts
    sink_m256i(_mm256_sllv_epi32(a, b));
    sink_m256i(_mm256_srav_epi32(a, b));
    sink_m256i(_mm256_srlv_epi32(a, b));
    sink_m256i(_mm256_sllv_epi64(a, b));
    sink_m256i(_mm256_srlv_epi64(a, b));

    // --- Compare ---
    sink_m256i(_mm256_cmpeq_epi8(a, b));
    sink_m256i(_mm256_cmpgt_epi8(a, b));
    sink_m256i(_mm256_cmpeq_epi16(a, b));
    sink_m256i(_mm256_cmpgt_epi16(a, b));
    sink_m256i(_mm256_cmpeq_epi32(a, b));
    sink_m256i(_mm256_cmpgt_epi32(a, b));
    sink_m256i(_mm256_cmpeq_epi64(a, b));
    sink_m256i(_mm256_cmpgt_epi64(a, b));

    // --- Permute / Shuffle / Broadcast ---
    sink_m256i(_mm256_shuffle_epi8(a, b));
    sink_m256i(_mm256_shuffle_epi32(a, 0x1B));
    sink_m256i(_mm256_shufflehi_epi16(a, 0x1B));
    sink_m256i(_mm256_shufflelo_epi16(a, 0x1B));
    sink_m256i(_mm256_permute2x128_si256(a, b, 0x01));
    sink_m256i(_mm256_permute4x64_epi64(a, 0x1B));
    sink_m256i(_mm256_permutevar8x32_epi32(a, b));

    sink_m256i(_mm256_broadcastb_epi8(_mm_set1_epi8(1)));
    sink_m256i(_mm256_broadcastw_epi16(_mm_set1_epi16(1)));
    sink_m256i(_mm256_broadcastd_epi32(_mm_set1_epi32(1)));
    sink_m256i(_mm256_broadcastq_epi64(_mm_set1_epi64x(1)));

    // --- Align ---
    sink_m256i(_mm256_alignr_epi8(a, b, 4));

    // --- Gather ---
    __m256i idx = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
    __m256 gathered_ps = _mm256_i32gather_ps(data->f32, idx, 4);
    volatile __m256 sink_ps = gathered_ps; (void)sink_ps;

    __m256d gathered_pd = _mm256_i32gather_pd(data->f64, _mm256_castsi256_si128(idx), 8);
    volatile __m256d sink_pd = gathered_pd; (void)sink_pd;

    __m256i gathered_epi32 = _mm256_i32gather_epi32(data->i32, idx, 4);
    sink_m256i(gathered_epi32);

    __m256i gathered_epi64 = _mm256_i32gather_epi64(data->i64, _mm256_castsi256_si128(idx), 8);
    sink_m256i(gathered_epi64);
}
