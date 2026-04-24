/*
 * AVX AI Math Utilities
 * AVX helpers for tactical AI calculations.
 */

#ifndef SHOOTER_AVX_AI_MATH_H
#define SHOOTER_AVX_AI_MATH_H

#include "avx_math.h"

#ifndef ALIGN32
#define ALIGN32 __attribute__((aligned(32)))
#endif

#ifndef AI_MEMORY_SLOTS
#define AI_MEMORY_SLOTS 8
#endif

typedef struct ALIGN32 {
    float x[AI_MEMORY_SLOTS];
    float y[AI_MEMORY_SLOTS];
    float time[AI_MEMORY_SLOTS];
    float confidence[AI_MEMORY_SLOTS];
    float velocity_x[AI_MEMORY_SLOTS];
    float velocity_y[AI_MEMORY_SLOTS];
} AIMemory;

#if defined(__x86_64__) || defined(__i386__)

static inline int avx_memory_find_slot(const AIMemory* memory) {
    __m256 conf = _mm256_loadu_ps(memory->confidence);
    ALIGN32 float conf_vals[8];
    _mm256_store_ps(conf_vals, conf);

    int slot = 0;
    float min_val = conf_vals[0];
    for (int i = 1; i < AI_MEMORY_SLOTS; i++) {
        if (conf_vals[i] < min_val) {
            min_val = conf_vals[i];
            slot = i;
        }
    }

    return slot;
}

static inline void avx_memory_decay(AIMemory* memory, float decay_rate) {
    __m256 conf = _mm256_loadu_ps(memory->confidence);
    __m256 decay = _mm256_set1_ps(decay_rate);
    conf = _mm256_mul_ps(conf, decay);
    _mm256_storeu_ps(memory->confidence, conf);
}

static inline void avx_steering_separation_single(
    float self_x,
    float self_y,
    __m256 nx,
    __m256 ny,
    __m256 active,
    float desired_sep,
    float* out_x,
    float* out_y
) {
    __m256 sx = _mm256_set1_ps(self_x);
    __m256 sy = _mm256_set1_ps(self_y);
    __m256 dx = _mm256_sub_ps(sx, nx);
    __m256 dy = _mm256_sub_ps(sy, ny);
    __m256 dist_sq = fmadd_ps(dy, dy, _mm256_mul_ps(dx, dx));
    __m256 dist = _mm256_sqrt_ps(dist_sq);
    __m256 inv_dist = _mm256_div_ps(_mm256_set1_ps(1.0f),
        _mm256_add_ps(dist, _mm256_set1_ps(0.001f)));

    __m256 active_mask = _mm256_cmp_ps(active, _mm256_setzero_ps(), _CMP_GT_OQ);
    __m256 sep_mask = _mm256_cmp_ps(dist, _mm256_set1_ps(desired_sep), _CMP_LT_OQ);
    __m256 mask = _mm256_and_ps(active_mask, sep_mask);
    __m256 steer_x = _mm256_and_ps(_mm256_mul_ps(dx, inv_dist), mask);
    __m256 steer_y = _mm256_and_ps(_mm256_mul_ps(dy, inv_dist), mask);

    *out_x = hsum256_ps(steer_x);
    *out_y = hsum256_ps(steer_y);
}

static inline void avx_steering_cohesion_single(
    float self_x,
    float self_y,
    __m256 nx,
    __m256 ny,
    __m256 active,
    float* out_x,
    float* out_y
) {
    float count = hsum256_ps(active);
    if (count <= 0.0f) {
        *out_x = 0.0f;
        *out_y = 0.0f;
        return;
    }

    float avg_x = hsum256_ps(_mm256_mul_ps(nx, active)) / count;
    float avg_y = hsum256_ps(_mm256_mul_ps(ny, active)) / count;
    *out_x = avg_x - self_x;
    *out_y = avg_y - self_y;
}

static inline void avx_steering_alignment_single(
    __m256 vx,
    __m256 vy,
    __m256 active,
    float* out_x,
    float* out_y
) {
    float count = hsum256_ps(active);
    if (count <= 0.0f) {
        *out_x = 0.0f;
        *out_y = 0.0f;
        return;
    }

    *out_x = hsum256_ps(_mm256_mul_ps(vx, active)) / count;
    *out_y = hsum256_ps(_mm256_mul_ps(vy, active)) / count;
}

static inline __m256 avx_score_attack_8(
    __m256 has_ammo,
    __m256 target_visible,
    __m256 target_in_range,
    __m256 health_ratio,
    __m256 aggression,
    __m256 threat_count
) {
    __m256 score = _mm256_mul_ps(has_ammo, target_visible);
    score = _mm256_mul_ps(score, target_in_range);
    score = _mm256_mul_ps(score, _mm256_add_ps(_mm256_set1_ps(0.5f), aggression));
    score = _mm256_mul_ps(score, _mm256_max_ps(health_ratio, _mm256_set1_ps(0.2f)));
    score = _mm256_sub_ps(score, _mm256_mul_ps(threat_count, _mm256_set1_ps(0.1f)));
    return score;
}

#else

static inline int avx_memory_find_slot(const AIMemory* memory) {
    (void)memory;
    return 0;
}

static inline void avx_memory_decay(AIMemory* memory, float decay_rate) {
    (void)memory;
    (void)decay_rate;
}

static inline void avx_steering_separation_single(
    float self_x,
    float self_y,
    __m256 nx,
    __m256 ny,
    __m256 active,
    float desired_sep,
    float* out_x,
    float* out_y
) {
    (void)self_x;
    (void)self_y;
    (void)nx;
    (void)ny;
    (void)active;
    (void)desired_sep;
    *out_x = 0.0f;
    *out_y = 0.0f;
}

static inline void avx_steering_cohesion_single(
    float self_x,
    float self_y,
    __m256 nx,
    __m256 ny,
    __m256 active,
    float* out_x,
    float* out_y
) {
    (void)self_x;
    (void)self_y;
    (void)nx;
    (void)ny;
    (void)active;
    *out_x = 0.0f;
    *out_y = 0.0f;
}

static inline void avx_steering_alignment_single(
    __m256 vx,
    __m256 vy,
    __m256 active,
    float* out_x,
    float* out_y
) {
    (void)vx;
    (void)vy;
    (void)active;
    *out_x = 0.0f;
    *out_y = 0.0f;
}

static inline __m256 avx_score_attack_8(
    __m256 has_ammo,
    __m256 target_visible,
    __m256 target_in_range,
    __m256 health_ratio,
    __m256 aggression,
    __m256 threat_count
) {
    (void)has_ammo;
    (void)target_visible;
    (void)target_in_range;
    (void)health_ratio;
    (void)aggression;
    (void)threat_count;
    __m256 zero = {0};
    return zero;
}

#endif

#endif /* SHOOTER_AVX_AI_MATH_H */
