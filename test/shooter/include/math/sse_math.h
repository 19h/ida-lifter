/*
 * SSE Math Utilities
 * SIMD math functions using SSE instructions for scalar and small batch operations.
 */

#ifndef SHOOTER_SSE_MATH_H
#define SHOOTER_SSE_MATH_H

#include <immintrin.h>
#include "../config.h"

/* ==========================================================================
 * FMA OPERATIONS
 * ========================================================================== */

#ifdef __FMA__
static inline __m128 fmadd_sse(__m128 a, __m128 b, __m128 c) {
    return _mm_fmadd_ps(a, b, c);
}
#else
static inline __m128 fmadd_sse(__m128 a, __m128 b, __m128 c) {
    return _mm_add_ps(_mm_mul_ps(a, b), c);
}
#endif

/* ==========================================================================
 * HORIZONTAL OPERATIONS
 * ========================================================================== */

/* Horizontal add for SSE __m128 */
static inline float hsum128_ps(__m128 v) {
    __m128 shuf = _mm_movehdup_ps(v);
    __m128 sums = _mm_add_ps(v, shuf);
    shuf = _mm_movehl_ps(shuf, sums);
    sums = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

/* ==========================================================================
 * SQUARE ROOT OPERATIONS
 * ========================================================================== */

/* SSE sqrt with Newton-Raphson refinement */
static inline __m128 sqrt_nr_sse(__m128 x) {
    __m128 rsqrt = _mm_rsqrt_ps(x);
    __m128 half = _mm_set1_ps(0.5f);
    __m128 three_half = _mm_set1_ps(1.5f);
    __m128 rsqrt2 = _mm_mul_ps(rsqrt, rsqrt);
    __m128 refined = _mm_mul_ps(rsqrt, _mm_sub_ps(three_half,
                     _mm_mul_ps(half, _mm_mul_ps(x, rsqrt2))));
    return _mm_mul_ps(x, refined);
}

/* ==========================================================================
 * SCALAR DISTANCE OPERATIONS
 * ========================================================================== */

/* Single scalar distance using SSE (faster than scalar for single calc) */
static inline float sse_distance(float x1, float y1, float x2, float y2) {
    __m128 v1 = _mm_set_ps(0, 0, y1, x1);
    __m128 v2 = _mm_set_ps(0, 0, y2, x2);
    __m128 diff = _mm_sub_ps(v2, v1);
    __m128 sq = _mm_mul_ps(diff, diff);
    /* Horizontal add: sq.x + sq.y */
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    __m128 dist = _mm_sqrt_ss(sum);
    return _mm_cvtss_f32(dist);
}

/* SSE distance squared (faster when we don't need the actual distance) */
static inline float sse_distance_squared(float x1, float y1, float x2, float y2) {
    __m128 v1 = _mm_set_ps(0, 0, y1, x1);
    __m128 v2 = _mm_set_ps(0, 0, y2, x2);
    __m128 diff = _mm_sub_ps(v2, v1);
    __m128 sq = _mm_mul_ps(diff, diff);
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    return _mm_cvtss_f32(sum);
}

/* ==========================================================================
 * SCALAR MATH OPERATIONS
 * ========================================================================== */

/* Fast scalar inverse sqrt using SSE */
__attribute__((unused))
static inline float sse_rsqrt(float x) {
    __m128 v = _mm_set_ss(x);
    __m128 r = _mm_rsqrt_ss(v);
    /* One Newton-Raphson iteration */
    __m128 half = _mm_set_ss(0.5f);
    __m128 three = _mm_set_ss(3.0f);
    __m128 r2 = _mm_mul_ss(r, r);
    __m128 muls = _mm_mul_ss(_mm_mul_ss(v, r2), r);
    r = _mm_mul_ss(_mm_mul_ss(half, r), _mm_sub_ss(three, muls));
    return _mm_cvtss_f32(r);
}

/* Fast scalar sqrt using SSE */
__attribute__((unused))
static inline float sse_sqrt(float x) {
    __m128 v = _mm_set_ss(x);
    __m128 r = _mm_sqrt_ss(v);
    return _mm_cvtss_f32(r);
}

/* ==========================================================================
 * VECTOR OPERATIONS
 * ========================================================================== */

/* SSE normalize a 2D vector in-place */
static inline void sse_normalize(float* x, float* y) {
    __m128 v = _mm_set_ps(0, 0, *y, *x);
    __m128 sq = _mm_mul_ps(v, v);
    __m128 sum = _mm_add_ss(sq, _mm_shuffle_ps(sq, sq, 1));
    __m128 len = _mm_sqrt_ss(sum);

    /* Check for zero length */
    if (_mm_cvtss_f32(len) < 0.0001f) {
        *x = 0;
        *y = 0;
        return;
    }

    __m128 inv_len = _mm_rcp_ss(len);
    /* Newton-Raphson iteration for precision */
    __m128 two = _mm_set_ss(2.0f);
    inv_len = _mm_mul_ss(inv_len, _mm_sub_ss(two, _mm_mul_ss(len, inv_len)));

    __m128 inv_broadcast = _mm_shuffle_ps(inv_len, inv_len, 0);
    __m128 result = _mm_mul_ps(v, inv_broadcast);

    ALIGN32 float out[4];
    _mm_store_ps(out, result);
    *x = out[0];
    *y = out[1];
}

/* SSE dot product of two 2D vectors */
static inline float sse_dot2(float x1, float y1, float x2, float y2) {
    __m128 v1 = _mm_set_ps(0, 0, y1, x1);
    __m128 v2 = _mm_set_ps(0, 0, y2, x2);
    __m128 mul = _mm_mul_ps(v1, v2);
    __m128 sum = _mm_add_ss(mul, _mm_shuffle_ps(mul, mul, 1));
    return _mm_cvtss_f32(sum);
}

/* ==========================================================================
 * SCALAR ARITHMETIC
 * ========================================================================== */

/* SSE multiply-add for scalars: a * b + c */
static inline float sse_fmadd(float a, float b, float c) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_set_ss(c);
#ifdef __FMA__
    __m128 result = _mm_fmadd_ss(va, vb, vc);
#else
    __m128 result = _mm_add_ss(_mm_mul_ss(va, vb), vc);
#endif
    return _mm_cvtss_f32(result);
}

/* SSE linear interpolation: a + t * (b - a) */
static inline float sse_lerp(float a, float b, float t) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vt = _mm_set_ss(t);
    __m128 diff = _mm_sub_ss(vb, va);
#ifdef __FMA__
    __m128 result = _mm_fmadd_ss(vt, diff, va);
#else
    __m128 result = _mm_add_ss(_mm_mul_ss(vt, diff), va);
#endif
    return _mm_cvtss_f32(result);
}

/* SSE clamp a value between min and max */
static inline float sse_clamp(float val, float min_val, float max_val) {
    __m128 v = _mm_set_ss(val);
    __m128 vmin = _mm_set_ss(min_val);
    __m128 vmax = _mm_set_ss(max_val);
    v = _mm_max_ss(vmin, _mm_min_ss(vmax, v));
    return _mm_cvtss_f32(v);
}

/* ==========================================================================
 * DOT PRODUCT & NORMALIZATION (3D)
 * ========================================================================== */

/* Dot product of two vectors (x,y,z) stored in __m128 */
static inline __m128 dot_product_sse(__m128 a, __m128 b) {
    /* mask 0x71 = 0111 0001 (calc x,y,z; store x) */
    return _mm_dp_ps(a, b, 0x71);
}

/* Normalize __m128 vector */
static inline __m128 normalize_sse(__m128 v) {
    __m128 dot = dot_product_sse(v, v);
    __m128 inv_sqrt = _mm_rsqrt_ps(dot);
    return _mm_mul_ps(v, inv_sqrt);
}

#endif /* SHOOTER_SSE_MATH_H */
