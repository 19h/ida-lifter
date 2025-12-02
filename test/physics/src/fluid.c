#include "common.h"
#include "math_avx.h"
#include <string.h>

#define IX(x, y) ((x) + (y) * FLUID_W)

void fluid_diffuse_avx(
    float* x, const float* x0,
    float diff, float dt,
    int iter
) {
    float a = dt * diff * (FLUID_W - 2) * (FLUID_H - 2);
    float c = 1 + 4 * a;
    float c_inv = 1.0f / c;

    __m256 v_a = _mm256_set1_ps(a);
    __m256 v_c_inv = _mm256_set1_ps(c_inv);

    for (int k = 0; k < iter; k++) {
        for (int j = 1; j < FLUID_H - 1; j++) {
            // AVX path - process 8 cells at a time
            int i;
            for (i = 1; i + 8 <= FLUID_W - 1; i += 8) {
                __m256 v_x0 = _mm256_loadu_ps(&x0[IX(i, j)]);

                __m256 v_left   = _mm256_loadu_ps(&x[IX(i - 1, j)]);
                __m256 v_right  = _mm256_loadu_ps(&x[IX(i + 1, j)]);
                __m256 v_up     = _mm256_loadu_ps(&x[IX(i, j - 1)]);
                __m256 v_down   = _mm256_loadu_ps(&x[IX(i, j + 1)]);

                __m256 sum = _mm256_add_ps(
                    _mm256_add_ps(v_left, v_right),
                    _mm256_add_ps(v_up, v_down)
                );

                __m256 res = _mm256_mul_ps(
                    _mm256_add_ps(v_x0, _mm256_mul_ps(v_a, sum)),
                    v_c_inv
                );

                _mm256_storeu_ps(&x[IX(i, j)], res);
            }

            // Scalar fallback for remaining cells
            for (; i < FLUID_W - 1; i++) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (
                    x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                    x[IX(i, j - 1)] + x[IX(i, j + 1)]
                )) * c_inv;
            }
        }
    }
}

void fluid_advect(
    float* d, const float* d0,
    const float* u, const float* v,
    float dt
) {
    float dt0 = dt * (FLUID_W - 2);
    __m256 v_dt0 = _mm256_set1_ps(dt0);
    __m256 v_min_xy = _mm256_set1_ps(0.5f);
    __m256 v_max_x = _mm256_set1_ps(FLUID_W - 1.5f);
    __m256 v_max_y = _mm256_set1_ps(FLUID_H - 1.5f);
    __m256 v_one = _mm256_set1_ps(1.0f);
    __m256i v_width = _mm256_set1_epi32(FLUID_W);

    for (int j = 1; j < FLUID_H - 1; j++) {
        __m256 v_j = _mm256_set1_ps((float)j);

        // AVX path - process 8 cells at a time
        int i;
        for (i = 1; i + 8 <= FLUID_W - 1; i += 8) {
            __m256 v_i = _mm256_set_ps(
                (float)(i+7), (float)(i+6), (float)(i+5), (float)(i+4),
                (float)(i+3), (float)(i+2), (float)(i+1), (float)i
            );

            __m256 v_u = _mm256_loadu_ps(&u[IX(i, j)]);
            __m256 v_v = _mm256_loadu_ps(&v[IX(i, j)]);

            __m256 x = _mm256_sub_ps(v_i, _mm256_mul_ps(v_dt0, v_u));
            __m256 y = _mm256_sub_ps(v_j, _mm256_mul_ps(v_dt0, v_v));

            x = clamp_ps(x, v_min_xy, v_max_x);
            y = clamp_ps(y, v_min_xy, v_max_y);

            __m256 x_floor = floor_ps(x);
            __m256 y_floor = floor_ps(y);
            __m256i i0 = _mm256_cvttps_epi32(x_floor);
            __m256i j0 = _mm256_cvttps_epi32(y_floor);
            __m256i i1 = _mm256_add_epi32(i0, _mm256_set1_epi32(1));
            __m256i j1 = _mm256_add_epi32(j0, _mm256_set1_epi32(1));

            __m256 s1 = _mm256_sub_ps(x, x_floor);
            __m256 s0 = _mm256_sub_ps(v_one, s1);
            __m256 t1 = _mm256_sub_ps(y, y_floor);
            __m256 t0 = _mm256_sub_ps(v_one, t1);

            __m256i idx00 = _mm256_add_epi32(i0, _mm256_mullo_epi32(j0, v_width));
            __m256i idx01 = _mm256_add_epi32(i0, _mm256_mullo_epi32(j1, v_width));
            __m256i idx10 = _mm256_add_epi32(i1, _mm256_mullo_epi32(j0, v_width));
            __m256i idx11 = _mm256_add_epi32(i1, _mm256_mullo_epi32(j1, v_width));

            __m256 d00 = _mm256_i32gather_ps(d0, idx00, 4);
            __m256 d01 = _mm256_i32gather_ps(d0, idx01, 4);
            __m256 d10 = _mm256_i32gather_ps(d0, idx10, 4);
            __m256 d11 = _mm256_i32gather_ps(d0, idx11, 4);

            __m256 interp0 = fmadd_ps(t1, d01, _mm256_mul_ps(t0, d00));
            __m256 interp1 = fmadd_ps(t1, d11, _mm256_mul_ps(t0, d10));
            __m256 result = fmadd_ps(s1, interp1, _mm256_mul_ps(s0, interp0));

            _mm256_storeu_ps(&d[IX(i, j)], result);
        }

        // Scalar fallback for remaining cells
        for (; i < FLUID_W - 1; i++) {
            float x = i - dt0 * u[IX(i, j)];
            float y = j - dt0 * v[IX(i, j)];

            if (x < 0.5f) x = 0.5f;
            if (x > FLUID_W - 1.5f) x = FLUID_W - 1.5f;
            if (y < 0.5f) y = 0.5f;
            if (y > FLUID_H - 1.5f) y = FLUID_H - 1.5f;

            int i0 = (int)x;
            int i1 = i0 + 1;
            int j0 = (int)y;
            int j1 = j0 + 1;

            float s1 = x - i0;
            float s0 = 1.0f - s1;
            float t1 = y - j0;
            float t0 = 1.0f - t1;

            d[IX(i, j)] =
                s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
}

void run_fluid_sim() {
    Viewport* vp = create_viewport();
    FluidGrid* grid = aligned_alloc(32, sizeof(FluidGrid));
    memset(grid, 0, sizeof(FluidGrid));

    // Initialize vortex velocity field
    for (int j = 0; j < FLUID_H; j++) {
        for (int i = 0; i < FLUID_W; i++) {
            float x = (i - FLUID_W / 2.0f) / (float)FLUID_W;
            float y = (j - FLUID_H / 2.0f) / (float)FLUID_H;
            grid->vx[IX(i, j)] = -y * 8.0f;
            grid->vy[IX(i, j)] = x * 8.0f;
        }
    }

    int frames = SIM_DURATION_SEC * FPS;
    float dt = 0.1f;
    float diff = 0.0001f;

    // Map viewport to fluid grid
    int scale_x = (vp->width - 4) / FLUID_W;
    int scale_y = (vp->height - 4) / FLUID_H;
    if (scale_x < 1) scale_x = 1;
    if (scale_y < 1) scale_y = 1;

    for (int f = 0; f < frames; f++) {
        clear_buffer(vp);
        draw_box(vp, 0, 0, vp->width - 1, vp->height - 1, "double");

        char title[128];
        snprintf(title, 128, "╔═══ 〰 NAVIER-STOKES FLUID VORTEX 〰 ═══╗");
        draw_string(vp, (vp->width - strlen(title)) / 2, 0, title);

        // Add pulsing sources
        int num_sources = 5;
        for (int s = 0; s < num_sources; s++) {
            float angle = (2.0f * PI * s / num_sources) + f * 0.05f;
            int sx = FLUID_W / 2 + (int)(cosf(angle) * FLUID_W * 0.3f);
            int sy = FLUID_H / 2 + (int)(sinf(angle) * FLUID_H * 0.3f);
            if (sx > 0 && sx < FLUID_W && sy > 0 && sy < FLUID_H) {
                grid->density[IX(sx, sy)] += 0.5f;
            }
        }

        // Center source
        grid->density[IX(FLUID_W/2, FLUID_H/2)] += 0.3f;

        // Advect
        fluid_advect(grid->density, grid->density_prev, grid->vx, grid->vy, dt);

        // Diffuse
        memcpy(grid->density_prev, grid->density, FLUID_SIZE * sizeof(float));
        fluid_diffuse_avx(grid->density, grid->density_prev, diff, dt, 4);

        // Render with fancy density ramp
        const char* ramp[] = {" ", "░", "▒", "▓", "█", "▓", "▒", "░"};
        const char* colors[] = {"∘", "·", "∙", "●", "◉", "⬤", "⬢", "◆"};
        
        for (int j = 0; j < FLUID_H; j++) {
            for (int i = 0; i < FLUID_W; i++) {
                float d = grid->density[IX(i, j)];
                if (d > 2.0f) d = 2.0f;
                if (d < 0.0f) d = 0.0f;
                
                int idx = (int)(d * 3.5f);
                if (idx > 7) idx = 7;
                
                int screen_x = 2 + i * scale_x / (scale_x > 1 ? scale_x : 1);
                int screen_y = 2 + j * scale_y / (scale_y > 1 ? scale_y : 1);
                
                if (d > 0.1f) {
                    draw_pixel(vp, screen_x, screen_y, d > 0.8f ? colors[idx] : ramp[idx]);
                }
            }
        }

        // Stats
        char stats[256];
        float total_density = 0.0f;
        for (int i = 0; i < FLUID_SIZE; i++) {
            total_density += grid->density[i];
        }
        snprintf(stats, 256, "Grid: %dx%d | Total Density: %.2f | Diffusion: %.5f | dt: %.3f",
                 FLUID_W, FLUID_H, total_density, diff, dt);
        draw_string(vp, 3, vp->height - 2, stats);

        render_buffer(vp);
        sleep_ms(1000 / FPS);
    }

    free(grid);
    free_viewport(vp);
}
