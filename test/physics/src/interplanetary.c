#include "common.h"
#include "math_avx.h"
#include <string.h>

#define MAX_ORBIT_TRAIL 500

typedef struct {
    float x1, y1, x2, y2;
    int age;
} OrbitLine;

// Taylor series approximation for sin(x) on AVX [ -PI, PI ]
__m256 sin_ps_avx(__m256 x) {
    __m256 x2 = _mm256_mul_ps(x, x);
    __m256 x3 = _mm256_mul_ps(x2, x);
    __m256 x5 = _mm256_mul_ps(x3, x2);
    __m256 x7 = _mm256_mul_ps(x5, x2);

    __m256 c1 = _mm256_set1_ps(-1.0f / 6.0f);
    __m256 c2 = _mm256_set1_ps(1.0f / 120.0f);
    __m256 c3 = _mm256_set1_ps(-1.0f / 5040.0f);

    __m256 result = fmadd_ps(x3, c1, x);
    result = fmadd_ps(x5, c2, result);
    result = fmadd_ps(x7, c3, result);
    return result;
}

__m256 cos_ps_avx(__m256 x) {
    __m256 x2 = _mm256_mul_ps(x, x);
    __m256 x4 = _mm256_mul_ps(x2, x2);
    __m256 x6 = _mm256_mul_ps(x4, x2);

    __m256 one = _mm256_set1_ps(1.0f);
    __m256 c1 = _mm256_set1_ps(-0.5f);
    __m256 c2 = _mm256_set1_ps(1.0f / 24.0f);
    __m256 c3 = _mm256_set1_ps(-1.0f / 720.0f);

    __m256 result = fmadd_ps(x2, c1, one);
    result = fmadd_ps(x4, c2, result);
    result = fmadd_ps(x6, c3, result);
    return result;
}

void calc_planet_pos_avx(Planet* p, __m256 time, __m256* out_x, __m256* out_y) {
    __m256 w = _mm256_set1_ps(p->orbit_speed);
    __m256 r = _mm256_set1_ps(p->orbit_radius);
    __m256 theta = _mm256_mul_ps(w, time);
    *out_x = _mm256_mul_ps(r, cos_ps_avx(theta));
    *out_y = _mm256_mul_ps(r, sin_ps_avx(theta));
}

void calc_interplanetary_avx(
    Planet* p1, Planet* p2, Star* star,
    float t_start, float dt,
    float* out_dists, int* out_occluded
) {
    __m256 t_base = _mm256_set1_ps(t_start);
    __m256 t_offsets = _mm256_set_ps(7*dt, 6*dt, 5*dt, 4*dt, 3*dt, 2*dt, dt, 0);
    __m256 time = _mm256_add_ps(t_base, t_offsets);

    __m256 p1_x, p1_y, p2_x, p2_y;
    calc_planet_pos_avx(p1, time, &p1_x, &p1_y);
    calc_planet_pos_avx(p2, time, &p2_x, &p2_y);

    __m256 p1_z = _mm256_setzero_ps();
    __m256 p2_z = _mm256_setzero_ps();

    __m256 dx = _mm256_sub_ps(p2_x, p1_x);
    __m256 dy = _mm256_sub_ps(p2_y, p1_y);
    __m256 dz = _mm256_sub_ps(p2_z, p1_z);

    __m256 dist_sq = _mm256_mul_ps(dx, dx);
    dist_sq = fmadd_ps(dy, dy, dist_sq);
    dist_sq = fmadd_ps(dz, dz, dist_sq);
    __m256 dist = sqrt_nr_ps(dist_sq);

    _mm256_storeu_ps(out_dists, dist);

    __m256 neg_p1_x = _mm256_sub_ps(_mm256_setzero_ps(), p1_x);
    __m256 neg_p1_y = _mm256_sub_ps(_mm256_setzero_ps(), p1_y);
    __m256 neg_p1_z = _mm256_sub_ps(_mm256_setzero_ps(), p1_z);

    __m256 dot_val = _mm256_mul_ps(neg_p1_x, dx);
    dot_val = fmadd_ps(neg_p1_y, dy, dot_val);
    dot_val = fmadd_ps(neg_p1_z, dz, dot_val);

    __m256 t_proj = _mm256_div_ps(dot_val, dist_sq);

    __m256 zero = _mm256_setzero_ps();
    __m256 one = _mm256_set1_ps(1.0f);
    t_proj = _mm256_max_ps(zero, _mm256_min_ps(one, t_proj));

    __m256 close_x = fmadd_ps(dx, t_proj, p1_x);
    __m256 close_y = fmadd_ps(dy, t_proj, p1_y);
    __m256 close_z = fmadd_ps(dz, t_proj, p1_z);

    __m256 close_dist2 = _mm256_mul_ps(close_x, close_x);
    close_dist2 = fmadd_ps(close_y, close_y, close_dist2);
    close_dist2 = fmadd_ps(close_z, close_z, close_dist2);

    __m256 star_r2 = _mm256_set1_ps(star->radius * star->radius);

    __m256 mask_occ = _mm256_cmp_ps(close_dist2, star_r2, _CMP_LT_OQ);

    int mask = _mm256_movemask_ps(mask_occ);
    for(int i=0; i<8; i++) {
        out_occluded[i] = (mask >> i) & 1;
    }
}

void run_interplanetary_sim() {
    Viewport* vp = create_viewport();

    Star star = {1.989e30f, 696340000.0f, {0,0,0,0}};
    (void)star;  // Used by calc_interplanetary_avx; visualization draws star at center
    Planet p1 = {5.972e24f, 6371000.0f, 1.496e11f, 1.99e-7f, 0, {0}};
    Planet p2 = {6.39e23f, 3389500.0f, 2.279e11f, 1.05e-7f, 0, {0}};

    float t = 0.0f;
    float dt = 604800.0f; // 1 week per frame

    int cx = vp->width / 2;
    int cy = vp->height / 2;
    float scale = (vp->width < vp->height ? vp->width : vp->height) * 0.0000000004f;

    OrbitLine trail1[MAX_ORBIT_TRAIL];
    OrbitLine trail2[MAX_ORBIT_TRAIL];
    int trail_idx = 0;
    
    for (int i = 0; i < MAX_ORBIT_TRAIL; i++) {
        trail1[i].age = 0;
        trail2[i].age = 0;
    }

    float prev_x1 = 0, prev_y1 = 0;
    float prev_x2 = 0, prev_y2 = 0;
    int first = 1;

    int frames = SIM_DURATION_SEC * FPS;
    for (int f = 0; f < frames; f++) {
        clear_buffer(vp);
        
        draw_box(vp, 0, 0, vp->width - 1, vp->height - 1, "single");
        
        char title[128];
        snprintf(title, 128, "═══ ☀ ORBITAL MECHANICS SIMULATOR ☀ ═══");
        draw_string(vp, (vp->width - strlen(title)) / 2, 1, title);

        __m256 vt = _mm256_set1_ps(t);
        __m256 p1x, p1y, p2x, p2y;
        calc_planet_pos_avx(&p1, vt, &p1x, &p1y);
        calc_planet_pos_avx(&p2, vt, &p2x, &p2y);

        float x1 = _mm_cvtss_f32(_mm256_castps256_ps128(p1x));
        float y1 = _mm_cvtss_f32(_mm256_castps256_ps128(p1y));
        float x2 = _mm_cvtss_f32(_mm256_castps256_ps128(p2x));
        float y2 = _mm_cvtss_f32(_mm256_castps256_ps128(p2y));

        // Age trails
        for (int i = 0; i < MAX_ORBIT_TRAIL; i++) {
            if (trail1[i].age > 0) trail1[i].age--;
            if (trail2[i].age > 0) trail2[i].age--;
        }

        // Add new trail segments
        if (!first) {
            trail1[trail_idx].x1 = cx + (int)(prev_x1 * scale);
            trail1[trail_idx].y1 = cy + (int)(prev_y1 * scale * 0.5f);
            trail1[trail_idx].x2 = cx + (int)(x1 * scale);
            trail1[trail_idx].y2 = cy + (int)(y1 * scale * 0.5f);
            trail1[trail_idx].age = 200;

            trail2[trail_idx].x1 = cx + (int)(prev_x2 * scale);
            trail2[trail_idx].y1 = cy + (int)(prev_y2 * scale * 0.5f);
            trail2[trail_idx].x2 = cx + (int)(x2 * scale);
            trail2[trail_idx].y2 = cy + (int)(y2 * scale * 0.5f);
            trail2[trail_idx].age = 200;

            trail_idx = (trail_idx + 1) % MAX_ORBIT_TRAIL;
        }
        first = 0;
        prev_x1 = x1; prev_y1 = y1;
        prev_x2 = x2; prev_y2 = y2;

        // Draw orbit trails
        for (int i = 0; i < MAX_ORBIT_TRAIL; i++) {
            if (trail1[i].age > 0) {
                const char* fade = (trail1[i].age > 100) ? "─" : (trail1[i].age > 50) ? "╌" : "┄";
                draw_line(vp, trail1[i].x1, trail1[i].y1, trail1[i].x2, trail1[i].y2, fade);
            }
            if (trail2[i].age > 0) {
                const char* fade = (trail2[i].age > 100) ? "─" : (trail2[i].age > 50) ? "╌" : "┄";
                draw_line(vp, trail2[i].x1, trail2[i].y1, trail2[i].x2, trail2[i].y2, fade);
            }
        }

        // Draw Star with corona effect
        draw_filled_circle(vp, cx, cy, 3, "░");
        draw_filled_circle(vp, cx, cy, 2, "▒");
        draw_filled_circle(vp, cx, cy, 1, "█");
        draw_pixel(vp, cx, cy, "☀");
        
        // Star rays
        const char* rays[] = {"╲", "│", "╱", "─", "╲", "│", "╱", "─"};
        for (int i = 0; i < 8; i++) {
            float ang = i * PI / 4.0f + t * 0.000001f;
            int rx = cx + (int)(cosf(ang) * 5);
            int ry = cy + (int)(sinf(ang) * 5 * 0.5f);
            draw_pixel(vp, rx, ry, rays[i]);
        }

        // Draw Planets
        int sx1 = cx + (int)(x1 * scale);
        int sy1 = cy + (int)(y1 * scale * 0.5f);
        draw_filled_circle(vp, sx1, sy1, 2, "▓");
        draw_pixel(vp, sx1, sy1, "⊕");
        draw_string(vp, sx1 + 3, sy1, "Earth");

        int sx2 = cx + (int)(x2 * scale);
        int sy2 = cy + (int)(y2 * scale * 0.5f);
        draw_filled_circle(vp, sx2, sy2, 1, "▒");
        draw_pixel(vp, sx2, sy2, "♂");
        draw_string(vp, sx2 + 2, sy2, "Mars");

        // Communication line
        if (f % 20 < 10) {
            draw_line(vp, sx1, sy1, sx2, sy2, "⋯");
        }

        // Stats
        float dist = sqrtf((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
        char stats[256];
        snprintf(stats, 256, "Time: %.1f days | Distance: %.2e km | Period E: %.0f d, M: %.0f d",
                 t / 86400.0f, dist / 1000.0f, 
                 2 * PI / p1.orbit_speed / 86400.0f,
                 2 * PI / p2.orbit_speed / 86400.0f);
        draw_string(vp, 3, vp->height - 2, stats);

        render_buffer(vp);
        t += dt;
        sleep_ms(1000 / FPS);
    }
    
    free_viewport(vp);
}
