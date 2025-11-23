#include "avx_test.h"

void run_avx_ops(TestData *data) {
    // Load data
    __m256 a = _mm256_load_ps(data->f32);
    __m256 b = _mm256_load_ps(data->f32 + 8);
    __m256d da = _mm256_load_pd(data->f64);
    __m256d db = _mm256_load_pd(data->f64 + 4);

    // --- Arithmetic (Single Precision) ---
    sink_m256(_mm256_add_ps(a, b));
    sink_m256(_mm256_sub_ps(a, b));
    sink_m256(_mm256_mul_ps(a, b));
    sink_m256(_mm256_div_ps(a, b));
    sink_m256(_mm256_max_ps(a, b));
    sink_m256(_mm256_min_ps(a, b));
    sink_m256(_mm256_sqrt_ps(a));
    sink_m256(_mm256_rcp_ps(a));
    sink_m256(_mm256_rsqrt_ps(a));
    sink_m256(_mm256_dp_ps(a, b, 0xFF));
    sink_m256(_mm256_hadd_ps(a, b));
    sink_m256(_mm256_hsub_ps(a, b));
    sink_m256(_mm256_floor_ps(a));
    sink_m256(_mm256_ceil_ps(a));
    sink_m256(_mm256_round_ps(a, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));

    // --- Arithmetic (Double Precision) ---
    sink_m256d(_mm256_add_pd(da, db));
    sink_m256d(_mm256_sub_pd(da, db));
    sink_m256d(_mm256_mul_pd(da, db));
    sink_m256d(_mm256_div_pd(da, db));
    sink_m256d(_mm256_max_pd(da, db));
    sink_m256d(_mm256_min_pd(da, db));
    sink_m256d(_mm256_sqrt_pd(da));
    sink_m256d(_mm256_hadd_pd(da, db));
    sink_m256d(_mm256_hsub_pd(da, db));
    sink_m256d(_mm256_floor_pd(da));
    sink_m256d(_mm256_ceil_pd(da));
    sink_m256d(_mm256_round_pd(da, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC));

    // --- Logic (Single Precision) ---
    sink_m256(_mm256_and_ps(a, b));
    sink_m256(_mm256_or_ps(a, b));
    sink_m256(_mm256_xor_ps(a, b));
    sink_m256(_mm256_andnot_ps(a, b));

    // --- Logic (Double Precision) ---
    sink_m256d(_mm256_and_pd(da, db));
    sink_m256d(_mm256_or_pd(da, db));
    sink_m256d(_mm256_xor_pd(da, db));
    sink_m256d(_mm256_andnot_pd(da, db));

    // --- Compare ---
    sink_m256(_mm256_cmp_ps(a, b, _CMP_EQ_OQ));
    sink_m256d(_mm256_cmp_pd(da, db, _CMP_EQ_OQ));

    // --- Permute / Shuffle ---
    sink_m256(_mm256_permute_ps(a, 0x1B));
    sink_m256(_mm256_permute2f128_ps(a, b, 0x01));
    sink_m256(_mm256_shuffle_ps(a, b, 0x1B));
    sink_m256d(_mm256_permute_pd(da, 0x5));
    sink_m256d(_mm256_permute2f128_pd(da, db, 0x01));
    sink_m256d(_mm256_shuffle_pd(da, db, 0x5));

    // --- Blend ---
    sink_m256(_mm256_blend_ps(a, b, 0xAA));
    sink_m256(_mm256_blendv_ps(a, b, a)); // Using a as mask
    sink_m256d(_mm256_blend_pd(da, db, 0xA));
    sink_m256d(_mm256_blendv_pd(da, db, da));

    // --- Conversion ---
    // Convert 8 floats to 8 ints (256b -> 256b)
    __m256i i_res = _mm256_cvtps_epi32(a);
    sink_m256i(i_res);

    // Convert 8 ints to 8 floats (256b -> 256b)
    sink_m256(_mm256_cvtepi32_ps(i_res));

    // Convert 4 floats to 4 doubles (128b -> 256b)
    // castps256_ps128 is a type cast, returns __m128
    sink_m256d(_mm256_cvtps_pd(_mm256_castps256_ps128(a)));

    // Convert 4 doubles to 4 floats (256b -> 128b)
    // This was the source of the error: returns __m128, not __m256
    sink_m128(_mm256_cvtpd_ps(da));

    // --- Masked Load/Store ---
    __m256i mask = _mm256_set1_epi32(-1); // All ones
    __m256 loaded = _mm256_maskload_ps(data->f32, mask);
    _mm256_maskstore_ps(data->f32, mask, loaded);

    __m256i mask_d = _mm256_set1_epi64x(-1);
    __m256d loaded_d = _mm256_maskload_pd(data->f64, mask_d);
    _mm256_maskstore_pd(data->f64, mask_d, loaded_d);

    // --- State Management ---
    // Clear upper 128 bits of YMM registers to avoid SSE transition penalties
    _mm256_zeroupper();
}
