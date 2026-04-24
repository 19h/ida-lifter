/*
 * AVX Math Utilities
 * SIMD math helpers for __m256 operations.
 */

#ifndef SHOOTER_AVX_MATH_H
#define SHOOTER_AVX_MATH_H

#if defined(__x86_64__) || defined(__i386__)
#include <immintrin.h>
#include "../config.h"
#else
typedef struct { float v[8]; } __m256;
#endif

/* ==========================================================================
 * FMA OPERATIONS
 * ========================================================================== */

static inline __m256 fmadd_ps(__m256 a, __m256 b, __m256 c) {
#if defined(__x86_64__) || defined(__i386__)
    return _mm256_fmadd_ps(a, b, c);
#else
    (void)a;
    (void)b;
    (void)c;
    __m256 zero = {0};
    return zero;
#endif
}

/* ==========================================================================
 * HORIZONTAL OPERATIONS
 * ========================================================================== */

/* Horizontal add for AVX __m256 */
static inline float hsum256_ps(__m256 v) {
#if defined(__x86_64__) || defined(__i386__)
    __m128 lo = _mm256_castps256_ps128(v);
    __m128 hi = _mm256_extractf128_ps(v, 1);
    __m128 sum = _mm_add_ps(lo, hi);
    __m128 shuf = _mm_movehdup_ps(sum);
    sum = _mm_add_ps(sum, shuf);
    shuf = _mm_movehl_ps(shuf, sum);
    sum = _mm_add_ss(sum, shuf);
    return _mm_cvtss_f32(sum);
#else
    (void)v;
    return 0.0f;
#endif
}

/* ==========================================================================
 * DISTANCE / MASK HELPERS
 * ========================================================================== */

static inline __m256 avx_sound_detection_8(
    __m256 ex,
    __m256 ey,
    float x,
    float y,
    float radius
) {
#if defined(__x86_64__) || defined(__i386__)
    __m256 sx = _mm256_set1_ps(x);
    __m256 sy = _mm256_set1_ps(y);
    __m256 dx = _mm256_sub_ps(ex, sx);
    __m256 dy = _mm256_sub_ps(ey, sy);
    __m256 dist_sq = fmadd_ps(dy, dy, _mm256_mul_ps(dx, dx));
    __m256 radius_sq = _mm256_set1_ps(radius * radius);
    __m256 mask = _mm256_cmp_ps(dist_sq, radius_sq, _CMP_LE_OQ);
    return _mm256_and_ps(mask, _mm256_set1_ps(1.0f));
#else
    (void)ex;
    (void)ey;
    (void)x;
    (void)y;
    (void)radius;
    __m256 zero = {0};
    return zero;
#endif
}

static inline void avx_update_bullets_8(
    float* bx,
    float* by,
    const float* bvx,
    const float* bvy,
    float dt
) {
#if defined(__x86_64__) || defined(__i386__)
    __m256 vx = _mm256_load_ps(bvx);
    __m256 vy = _mm256_load_ps(bvy);
    __m256 x = _mm256_load_ps(bx);
    __m256 y = _mm256_load_ps(by);
    __m256 dtv = _mm256_set1_ps(dt);

    x = fmadd_ps(vx, dtv, x);
    y = fmadd_ps(vy, dtv, y);

    _mm256_store_ps(bx, x);
    _mm256_store_ps(by, y);
#else
    (void)bx;
    (void)by;
    (void)bvx;
    (void)bvy;
    (void)dt;
#endif
}

#endif /* SHOOTER_AVX_MATH_H */
