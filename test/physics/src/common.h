#ifndef COMMON_H
#define COMMON_H

#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Constants
#define G_CONST 6.67430e-11f
#define C_LIGHT 2.99792e8f
#define PI 3.14159265359f

// Visualization Constants
#define FPS 30
#define SIM_DURATION_SEC 10

// Alignment macros
#define ALIGN32 __attribute__((aligned(32)))

// Vector types
typedef struct ALIGN32 {
    float x, y, z, w;
} Vec4;

typedef struct ALIGN32 {
    float x, y, z;
    float r; // Radius or padding
} Sphere;

// Ray structure
typedef struct ALIGN32 {
    Vec4 origin;
    Vec4 dir;
} Ray;

// Planet structure
typedef struct {
    float mass;
    float radius;
    float orbit_radius;
    float orbit_speed; // rad/s
    float current_angle;
    Vec4 pos;
} Planet;

// Star structure
typedef struct {
    float mass;
    float radius;
    Vec4 pos;
} Star;

// Fluid Grid
#define FLUID_W 64
#define FLUID_H 32
#define FLUID_SIZE (FLUID_W * FLUID_H)
#define IX(x, y) ((x) + (y) * FLUID_W)

typedef struct ALIGN32 {
    float density[FLUID_SIZE];
    float vx[FLUID_SIZE];
    float vy[FLUID_SIZE];
    float density_prev[FLUID_SIZE];
    float vx_prev[FLUID_SIZE];
    float vy_prev[FLUID_SIZE];
} FluidGrid;

// Viewport structure with double buffering
typedef struct {
    int width;
    int height;
    char** buffer;      // Current frame
    char** prev_buffer; // Previous frame for diff rendering
} Viewport;

// Visualization Helpers (implemented in vis.c)
Viewport* create_viewport();
void free_viewport(Viewport* vp);
void clear_buffer(Viewport* vp);
void draw_pixel(Viewport* vp, int x, int y, const char* utf8);
void draw_pixel_char(Viewport* vp, int x, int y, char c);
void draw_line(Viewport* vp, int x0, int y0, int x1, int y1, const char* utf8);
void draw_circle(Viewport* vp, int cx, int cy, int r, const char* utf8);
void draw_filled_circle(Viewport* vp, int cx, int cy, int r, const char* utf8);
void draw_string(Viewport* vp, int x, int y, const char* str);
void draw_box(Viewport* vp, int x1, int y1, int x2, int y2, const char* style);
void render_buffer(Viewport* vp);
void sleep_ms(int ms);

// Simulation Runners
// max_frames: -1 for unlimited, positive value for frame limit
void run_bullet_sim(int max_frames);
void run_interplanetary_sim();
void run_asteroid_sim();
void run_fluid_sim();

// Core Physics Functions (Original)
int trace_bullet_avx(const Ray* ray, const Sphere* spheres, int count, float* out_t);
void calc_interplanetary_avx(Planet* p1, Planet* p2, Star* star, float t_start, float dt, float* out_dists, int* out_occluded);
void update_asteroid_sse(Vec4* asteroid_pos, Vec4* asteroid_vel, const Star* star, const Planet planets[3], float dt, float* out_time_dilation);
void fluid_diffuse_avx(float* x, const float* x0, float diff, float dt, int iter);
void fluid_advect(float* d, const float* d0, const float* u, const float* v, float dt);

#endif // COMMON_H
