/*
 * AVX/AVX2 Instruction Lifter Test Suite
 * Replicates instruction patterns from IDA Pro assembly dump
 * Compile: gcc -mavx2 -mavx -mfma -O0 -g -o avx_lifter_test avx_lifter_test.c -lm
 *
 * NOTE: -O0 is critical to prevent compiler optimization from altering instruction sequences
 */

#define _GNU_SOURCE
#include <immintrin.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Prevent compiler from optimizing away operations */
#define FORCE_USE(x) __asm__ volatile("" : : "r,m"(x) : "memory")
#define FORCE_USE_XMM(x) __asm__ volatile("" : : "x"(x) : "memory")
#define FORCE_USE_YMM(x) __asm__ volatile("" : : "x"(x) : "memory")
#define COMPILER_BARRIER() __asm__ volatile("" ::: "memory")

/* Aligned buffer for YMM operations */
typedef struct __attribute__((aligned(32))) {
    uint8_t data[32];
} YmmBuffer;

typedef struct __attribute__((aligned(16))) {
    uint8_t data[16];
} XmmBuffer;

/* Global storage locations (simulating cs:unk_* addresses)
 * These mirror the original binary's global YMM storage targets */
static __attribute__((aligned(32), unused)) YmmBuffer g_unk_149E0F8D0;
static __attribute__((aligned(32), unused)) YmmBuffer g_unk_149E0F990;
static __attribute__((aligned(32), unused)) YmmBuffer g_unk_149FDED90;
static __attribute__((aligned(32), unused)) YmmBuffer g_unk_149FDEE00;
static __attribute__((aligned(32), unused)) YmmBuffer g_qword_149FE3E20;
static __attribute__((aligned(32))) YmmBuffer g_qword_149E00118;

/* Constants matching cs: references */
static const float g_xmmword_1485CFD10 = -0.0f;  /* Sign bit mask */
static const float g_xmmword_1485CFCF0 = 0.0f;
static const __attribute__((aligned(16))) float g_xmmword_1485D4980[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static const double g_qword_1485DD0A8 = 1e-10;
static const double g_qword_1485D4930 = 1.0;
static const __attribute__((aligned(16))) double g_xmmword_1485DA4E0[2] = {0.0, 1.0};
static const float g_flt_1485DD128 = 0.159154943f;  /* 1/(2*PI) */
static const float g_dword_1485DD0C8 = 360.0f;
static const float g_dword_1485CF940 = 0.0f;
static const float g_dword_1485CFB3C = 1.0f;
static const float g_dword_1485DD14C = 0.159154943f;
static const float g_dword_1485DD098 = 360.0f;
static const float g_dword_148608B08 = 1.0f;
static const float g_dword_148608B04 = 0.00392157f;  /* 1/255 */
static const float g_dword_148608AF4 = 0.0f;
static const float g_dword_1485DD140 = 255.0f;
static const float g_dword_1485E72B8 = 255.0f;
static const float g_Y = 1.0f;

/* ============================================================================
 * sub_1402C6260 - Sign-preserving round function
 * Uses: vmovaps, vmovss, vandnps, vroundps, vsubss, vcmpeqss, vandps, vaddss, vorps
 * ============================================================================ */
__attribute__((noinline))
float sub_1402C6260(float input) {
    __m128 xmm0, xmm1, xmm2, xmm3;

    /* vmovaps xmm1, xmm0 */
    xmm1 = _mm_set_ss(input);

    /* vmovss xmm0, dword ptr cs:xmmword_1485CFD10  (sign bit mask: -0.0f) */
    xmm0 = _mm_set_ss(g_xmmword_1485CFD10);

    /* vmovaps xmm3, xmm0 */
    xmm3 = xmm0;

    /* vandnps xmm0, xmm0, xmm1 - clear sign bit (absolute value) */
    xmm0 = _mm_andnot_ps(xmm0, xmm1);

    /* vroundps xmm2, xmm0, 0 - round to nearest */
    xmm2 = _mm_round_ps(xmm0, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);

    /* vsubss xmm0, xmm0, xmm2 - fractional part */
    xmm0 = _mm_sub_ss(xmm0, xmm2);

    /* vcmpeqss xmm0, xmm0, dword ptr cs:xmmword_1485CFCF0 - compare with 0.0 */
    xmm0 = _mm_cmpeq_ss(xmm0, _mm_set_ss(g_xmmword_1485CFCF0));

    /* vandps xmm0, xmm0, cs:xmmword_1485D4980 - mask with 1.0 if equal */
    xmm0 = _mm_and_ps(xmm0, _mm_load_ps(g_xmmword_1485D4980));

    /* vaddss xmm0, xmm0, xmm2 */
    xmm0 = _mm_add_ss(xmm0, xmm2);

    /* vandps xmm3, xmm3, xmm1 - extract original sign */
    xmm3 = _mm_and_ps(xmm3, xmm1);

    /* vorps xmm0, xmm0, xmm3 - restore sign */
    xmm0 = _mm_or_ps(xmm0, xmm3);

    float result;
    _mm_store_ss(&result, xmm0);
    return result;
}

/* ============================================================================
 * sub_140322E70 - 3D vector angle calculation
 * Uses: vmovsd, vsubsd, vmulsd, vaddsd, vcomisd, vmovddup, vsqrtpd, vunpckhpd,
 *       vdivsd, vcvtsd2ss, vcvtpd2ps, vmulss, vaddss, vroundps, vcmpeqss, etc.
 * ============================================================================ */
typedef struct __attribute__((aligned(16))) {
    double x, y, z;  /* Position at offsets 0, 8, 16 */
} Vec3d;

typedef struct __attribute__((aligned(16))) {
    double data[12];  /* Camera/transform matrix simulation */
} CameraData;

static CameraData* g_camera_ptr = NULL;

__attribute__((noinline))
float sub_140322E70(const Vec3d* pos) {
    __m128d xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6_d;
    __m128 xmm0_s, xmm2_s, xmm3_s;

    if (!g_camera_ptr) {
        static CameraData dummy_camera = {
            {1.0, 0.0, 0.0, 100.0, 0.0, 1.0, 0.0, 200.0, 0.0, 0.0, 1.0, 300.0}
        };
        g_camera_ptr = &dummy_camera;
    }
    CameraData* cam = g_camera_ptr;

    /* vmovsd xmm0, qword ptr [rbx+8] ; pos->y */
    xmm0 = _mm_load_sd(&pos->y);

    /* vmovsd xmm1, qword ptr [rbx] ; pos->x */
    xmm1 = _mm_load_sd(&pos->x);

    /* vsubsd xmm4, xmm0, qword ptr [rax+38h] ; cam[7] */
    xmm4 = _mm_sub_sd(xmm0, _mm_load_sd(&cam->data[7]));

    /* vsubsd xmm6, xmm1, qword ptr [rax+18h] ; cam[3] */
    xmm6_d = _mm_sub_sd(xmm1, _mm_load_sd(&cam->data[3]));

    /* vmovsd xmm0, qword ptr [rbx+10h] ; pos->z */
    xmm0 = _mm_load_sd(&pos->z);

    /* vsubsd xmm5, xmm0, qword ptr [rax+58h] ; cam[11] */
    xmm5 = _mm_sub_sd(xmm0, _mm_load_sd(&cam->data[11]));

    /* vmulsd xmm1, xmm4, xmm4 */
    xmm1 = _mm_mul_sd(xmm4, xmm4);

    /* vmulsd xmm2, xmm6, xmm6 */
    xmm2 = _mm_mul_sd(xmm6_d, xmm6_d);

    /* vmulsd xmm0, xmm5, xmm5 */
    xmm0 = _mm_mul_sd(xmm5, xmm5);

    /* vaddsd xmm3, xmm2, xmm1 */
    xmm3 = _mm_add_sd(xmm2, xmm1);

    /* vaddsd xmm1, xmm3, xmm0 - squared distance */
    xmm1 = _mm_add_sd(xmm3, xmm0);

    double dist_sq;
    _mm_store_sd(&dist_sq, xmm1);

    double dx, dy, dz;
    _mm_store_sd(&dx, xmm6_d);
    _mm_store_sd(&dy, xmm4);
    _mm_store_sd(&dz, xmm5);

    /* vcomisd xmm1, cs:qword_1485DD0A8 */
    if (dist_sq > g_qword_1485DD0A8) {
        /* vmovddup xmm0, xmm1 */
        xmm0 = _mm_movedup_pd(xmm1);

        /* vmovsd xmm1, cs:qword_1485D4930 (1.0) */
        xmm1 = _mm_set_sd(g_qword_1485D4930);

        /* vsqrtpd xmm2, xmm0 */
        xmm2 = _mm_sqrt_pd(xmm0);

        /* vunpckhpd xmm2, xmm2, xmm2 */
        xmm2 = _mm_unpackhi_pd(xmm2, xmm2);

        /* vdivsd xmm0, xmm1, xmm2 - inverse distance */
        xmm0 = _mm_div_sd(xmm1, xmm2);

        double inv_dist;
        _mm_store_sd(&inv_dist, xmm0);

        /* Normalize direction */
        dx *= inv_dist;
        dy *= inv_dist;
        dz *= inv_dist;
    } else {
        /* Default direction from xmmword_1485DA4E0 */
        dx = g_xmmword_1485DA4E0[0];
        dz = g_xmmword_1485DA4E0[1];
        dy = 0.0;
    }

    /* Dot product with camera forward vector */
    /* vcvtsd2ss, vcvtpd2ps, vmulss, vaddss sequence */
    float fx = (float)dx;
    float fy = (float)dy;
    float fz = (float)dz;

    float cam_x = (float)cam->data[5];  /* cam[1] -> offset 8 -> index 1, but pattern suggests 5 */
    float cam_y = (float)cam->data[1];
    float cam_z = (float)cam->data[9];

    float dot = fx * cam_x + fy * cam_y + fz * cam_z;

    /* vmulss xmm1, xmm2, cs:flt_1485DD128 - scale to [0,1) range */
    float scaled = dot * g_flt_1485DD128;

    /* Sign-preserving round sequence (same pattern as sub_1402C6260) */
    xmm3_s = _mm_set_ss(g_xmmword_1485CFD10);
    xmm2_s = xmm3_s;

    /* vandnps xmm3, xmm3, xmm1 */
    xmm3_s = _mm_andnot_ps(xmm3_s, _mm_set_ss(scaled));

    /* vroundps xmm0, xmm3, 0 */
    xmm0_s = _mm_round_ps(xmm3_s, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);

    /* vsubss xmm3, xmm3, xmm0 */
    xmm3_s = _mm_sub_ss(xmm3_s, xmm0_s);

    /* vcmpeqss xmm3, xmm3, dword ptr cs:xmmword_1485CFCF0 */
    xmm3_s = _mm_cmpeq_ss(xmm3_s, _mm_set_ss(g_xmmword_1485CFCF0));

    /* vandps xmm3, xmm3, cs:xmmword_1485D4980 */
    xmm3_s = _mm_and_ps(xmm3_s, _mm_load_ps(g_xmmword_1485D4980));

    /* vaddss xmm3, xmm3, xmm0 */
    xmm3_s = _mm_add_ss(xmm3_s, xmm0_s);

    /* vandps xmm2, xmm2, xmm1 */
    xmm2_s = _mm_and_ps(xmm2_s, _mm_set_ss(scaled));

    /* vorps xmm3, xmm3, xmm2 */
    xmm3_s = _mm_or_ps(xmm3_s, xmm2_s);

    /* vmulss xmm0, xmm3, cs:dword_1485DD0C8 - convert to degrees */
    xmm0_s = _mm_mul_ss(xmm3_s, _mm_set_ss(g_dword_1485DD0C8));

    float result;
    _mm_store_ss(&result, xmm0_s);
    return result;
}

/* ============================================================================
 * sub_14032C890 - Bearing/heading calculation with blend operations
 * Uses: vmovss, vpcmpeqd, vblendvps, vsubsd, vcvtsd2ss, vcvtpd2ps, vmulss,
 *       vaddss, vcomiss, vroundps, vcmpeqss, vandps, vorps
 * ============================================================================ */
typedef struct __attribute__((aligned(16))) {
    double x, y, z;
} Position3d;

typedef struct {
    double data[32];
    int zone_count_a;  /* offset 0xF4 */
    int zone_count_b;  /* offset 0xF8 */
} CameraSystem;

static CameraSystem* g_camera_system = NULL;

__attribute__((noinline))
float sub_14032C890(const Position3d* target_pos, int mode) {
    if (!g_camera_system) {
        static CameraSystem dummy = {
            .data = {0.0, 1.0, 0.0, 100.0, 0.0, 0.5, 0.0, 200.0, 0.0, 0.0, 1.0, 300.0},
            .zone_count_a = 8,
            .zone_count_b = 12
        };
        g_camera_system = &dummy;
    }

    CameraSystem* cam = g_camera_system;
    int zone_count;

    /* Branch based on mode (esi/r8d) */
    if (mode == 0) {
        zone_count = cam->zone_count_a;
    } else {
        zone_count = cam->zone_count_b;
    }

    if (zone_count <= 0) {
        return g_dword_1485CF940;  /* Return 0.0 */
    }

    /* Simulated call to sub_1404144B0 - returns bearing data */
    float bearing_data[2] = {45.0f, 90.0f};

    /* vpcmpeqd xmm3, xmm0, xmm1 - compare mode with 0 */
    __m128i mode_vec = _mm_set1_epi32(mode);
    __m128i zero_vec = _mm_setzero_si128();
    __m128i cmp_result = _mm_cmpeq_epi32(mode_vec, zero_vec);

    /* vblendvps xmm3, xmm1, xmm2, xmm3 - select bearing based on mode */
    __m128 bearing0 = _mm_set_ss(bearing_data[0]);
    __m128 bearing1 = _mm_set_ss(bearing_data[1]);
    __m128 selected = _mm_blendv_ps(bearing1, bearing0, _mm_castsi128_ps(cmp_result));

    /* vcvtsi2ss xmm0, xmm0, edi - convert zone_count to float */
    __m128 zone_f = _mm_cvtsi32_ss(_mm_setzero_ps(), zone_count);

    /* vdivss xmm6, xmm3, xmm0 - divide bearing by zone count */
    __m128 xmm6 = _mm_div_ss(selected, zone_f);

    /* Vector subtraction and dot product */
    double dx = target_pos->x - cam->data[3];
    double dy = target_pos->y - cam->data[7];
    double dz = target_pos->z - cam->data[11];

    /* vcvtsd2ss conversions */
    float fdx = (float)dx;
    float fdy = (float)dy;
    float fdz = (float)dz;

    /* vcvtpd2ps and vmulss - dot product with camera axes */
    float cam_x = (float)cam->data[5];
    float cam_y = (float)cam->data[1];
    float cam_z = (float)cam->data[9];

    float dot = fdx * cam_y + fdy * cam_x + fdz * cam_z;

    /* vcomiss xmm4, xmm0 - check if dot < 0 */
    if (dot < 0.0f) {
        /* Flip bearing */
        float bearing_val;
        _mm_store_ss(&bearing_val, xmm6);
        bearing_val = g_dword_1485CFB3C - (bearing_val - g_dword_1485CFB3C);
        xmm6 = _mm_set_ss(bearing_val);
    }

    /* vmulss xmm0, xmm6, cs:dword_1485DD14C */
    __m128 xmm0 = _mm_mul_ss(xmm6, _mm_set_ss(g_dword_1485DD14C));

    /* Sign-preserving round pattern */
    __m128 sign_mask = _mm_set_ss(g_xmmword_1485CFD10);
    __m128 xmm2 = sign_mask;
    __m128 xmm3 = _mm_andnot_ps(sign_mask, xmm0);
    __m128 xmm1 = _mm_round_ps(xmm3, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    xmm3 = _mm_sub_ss(xmm3, xmm1);
    xmm3 = _mm_cmpeq_ss(xmm3, _mm_set_ss(g_xmmword_1485CFCF0));
    xmm3 = _mm_and_ps(xmm3, _mm_load_ps(g_xmmword_1485D4980));
    xmm2 = _mm_and_ps(xmm2, xmm0);
    xmm3 = _mm_add_ss(xmm3, xmm1);
    xmm3 = _mm_or_ps(xmm3, xmm2);

    /* vmulss xmm0, xmm3, cs:dword_1485DD098 */
    xmm0 = _mm_mul_ss(xmm3, _mm_set_ss(g_dword_1485DD098));

    float result;
    _mm_store_ss(&result, xmm0);
    return result;
}

/* ============================================================================
 * sub_140331470 - Velocity accumulator with conditional blend
 * Uses: vmovsd, vmovss, vcmpless, vxorps, vblendvps, vmulss, vshufps, vaddss
 * ============================================================================ */
typedef struct __attribute__((aligned(16))) {
    float current[3];    /* offsets 0, 4, 8 */
    float max_val;       /* offset 0xC */
    float accumulated[3]; /* offsets 0x10, 0x14, 0x18 - second set */
} VelocityAccum;

__attribute__((noinline))
void sub_140331470(VelocityAccum* accum, const double* velocity) {
    __m128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;

    /* vmovsd xmm3, qword ptr [rdx] - load velocity x,y as double */
    __m128d vel_xy = _mm_load_sd(velocity);
    xmm3 = _mm_castpd_ps(vel_xy);

    /* vmovss xmm1, cs:Y (1.0) */
    xmm1 = _mm_set_ss(g_Y);

    /* vmovss xmm7, dword ptr [rcx] - current[0] */
    xmm7 = _mm_load_ss(&accum->current[0]);

    /* vcmpless xmm2, xmm7, dword ptr [rcx+0Ch] - current[0] <= max_val */
    xmm2 = _mm_cmple_ss(xmm7, _mm_load_ss(&accum->max_val));

    /* vxorps xmm0, xmm0, xmm0 */
    xmm0 = _mm_setzero_ps();

    /* vblendvps xmm4, xmm0, xmm1, xmm2 - select 1.0 or 0.0 based on comparison */
    xmm4 = _mm_blendv_ps(xmm0, xmm1, xmm2);

    /* vmulss xmm6, xmm3, xmm4 - scale velocity.x */
    xmm6 = _mm_mul_ss(xmm3, xmm4);

    /* vshufps xmm0, xmm3, xmm3, 55h - extract velocity.y */
    xmm0 = _mm_shuffle_ps(xmm3, xmm3, 0x55);

    /* vmulss xmm3, xmm4, dword ptr [rdx+8] - scale velocity.z */
    float vel_z = (float)velocity[1];  /* Simplified - actual is [rdx+8] */
    xmm3 = _mm_mul_ss(xmm4, _mm_set_ss(vel_z));

    /* vmulss xmm5, xmm0, xmm4 - scale velocity.y */
    xmm5 = _mm_mul_ss(xmm0, xmm4);

    /* vaddss xmm0, xmm6, xmm7 - accumulate x */
    xmm0 = _mm_add_ss(xmm6, xmm7);
    _mm_store_ss(&accum->current[0], xmm0);

    /* vaddss xmm1, xmm5, dword ptr [rcx+4] - accumulate y */
    xmm1 = _mm_add_ss(xmm5, _mm_load_ss(&accum->current[1]));
    _mm_store_ss(&accum->current[1], xmm1);

    /* vaddss xmm0, xmm3, dword ptr [rcx+8] - accumulate z */
    xmm0 = _mm_add_ss(xmm3, _mm_load_ss(&accum->current[2]));
    _mm_store_ss(&accum->current[2], xmm0);

    /* Second accumulator set */
    /* vaddss xmm0, xmm6, dword ptr [rcx+0Ch] */
    xmm0 = _mm_add_ss(xmm6, _mm_load_ss(&accum->max_val));
    _mm_store_ss(&accum->max_val, xmm0);

    /* vaddss xmm1, xmm5, dword ptr [rcx+10h] */
    xmm1 = _mm_add_ss(xmm5, _mm_load_ss(&accum->accumulated[0]));
    _mm_store_ss(&accum->accumulated[0], xmm1);

    /* vaddss xmm0, xmm3, dword ptr [rcx+14h] */
    xmm0 = _mm_add_ss(xmm3, _mm_load_ss(&accum->accumulated[1]));
    _mm_store_ss(&accum->accumulated[1], xmm0);
}

/* ============================================================================
 * sub_14052B7D0 - RGBA color quantization with clamping and rounding
 * Uses: vmovss, vmulss, vaddss, vcomiss, vminss, vroundps, vcmpeqss,
 *       vandps, vandnps, vorps, vcvttss2si
 * ============================================================================ */
typedef struct {
    float r, g, b, a;  /* RGBA at offsets 0, 4, 8, 0xC */
} ColorRGBA;

__attribute__((noinline))
uint8_t* sub_14052B7D0(const ColorRGBA* color, uint8_t* out) {
    __m128 xmm0, xmm1, xmm2, xmm3, xmm4;
    __m128 xmm10, xmm11;

    /* Constants */
    float scale = g_dword_148608B08;
    float alpha_scale = g_dword_148608B04;
    float clamp_min = g_dword_1485CF940;
    float clamp_max = g_Y;
    float quant_scale = g_dword_1485E72B8;

    /* vmovss xmm2, cs:dword_148608B08 (1.0) */
    xmm2 = _mm_set_ss(scale);

    /* vmovss xmm0, dword ptr [rcx+0Ch] - alpha */
    xmm0 = _mm_load_ss(&color->a);

    /* vmulss xmm1, xmm0, cs:dword_148608B04 */
    xmm1 = _mm_mul_ss(xmm0, _mm_set_ss(alpha_scale));

    /* vmulss xmm3, xmm2, dword ptr [rcx] - r * scale */
    xmm3 = _mm_mul_ss(xmm2, _mm_load_ss(&color->r));

    /* vmulss xmm10, xmm2, dword ptr [rcx+4] - g * scale */
    xmm10 = _mm_mul_ss(xmm2, _mm_load_ss(&color->g));

    /* vmulss xmm11, xmm2, dword ptr [rcx+8] - b * scale */
    xmm11 = _mm_mul_ss(xmm2, _mm_load_ss(&color->b));

    /* vaddss xmm2, xmm1, cs:dword_148608AF4 */
    xmm2 = _mm_add_ss(xmm1, _mm_set_ss(g_dword_148608AF4));

    /* Clamp alpha to [0, 1] */
    xmm0 = _mm_setzero_ps();
    int alpha_negative = (_mm_comilt_ss(xmm2, xmm0) != 0);
    if (alpha_negative) {
        xmm0 = _mm_setzero_ps();
    } else {
        xmm0 = _mm_min_ss(xmm2, _mm_set_ss(clamp_max));
    }

    /* vmulss xmm0, xmm0, cs:dword_1485DD140 - scale to 255 */
    xmm0 = _mm_mul_ss(xmm0, _mm_set_ss(g_dword_1485DD140));

    /* vcvttss2si ecx, xmm0 - truncate to int (alpha) */
    int alpha_int = _mm_cvttss_si32(xmm0);

    /* Process R channel with full rounding sequence */
    xmm4 = _mm_set_ss(clamp_min);
    int r_negative = (_mm_comilt_ss(xmm3, xmm4) != 0);
    if (r_negative) {
        xmm0 = xmm4;
    } else {
        xmm0 = _mm_min_ss(xmm3, _mm_set_ss(clamp_max));
    }

    /* vmulss xmm0, xmm0, cs:dword_1485E72B8 */
    xmm0 = _mm_mul_ss(xmm0, _mm_set_ss(quant_scale));

    /* Sign-preserving round */
    __m128 sign_mask = _mm_set_ss(g_xmmword_1485CFD10);
    xmm2 = sign_mask;
    xmm3 = _mm_andnot_ps(sign_mask, xmm0);
    xmm1 = _mm_round_ps(xmm3, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    xmm3 = _mm_sub_ss(xmm3, xmm1);
    xmm3 = _mm_cmpeq_ss(xmm3, _mm_set_ss(g_xmmword_1485CFCF0));
    xmm3 = _mm_and_ps(xmm3, _mm_load_ps(g_xmmword_1485D4980));
    xmm3 = _mm_add_ss(xmm3, xmm1);
    xmm2 = _mm_and_ps(xmm2, xmm0);
    xmm3 = _mm_or_ps(xmm3, xmm2);
    int r_int = _mm_cvttss_si32(xmm3);

    /* Process G channel */
    int g_negative = (_mm_comilt_ss(xmm10, xmm4) != 0);
    if (g_negative) {
        xmm0 = xmm4;
    } else {
        xmm0 = _mm_min_ss(xmm10, _mm_set_ss(clamp_max));
    }
    xmm0 = _mm_mul_ss(xmm0, _mm_set_ss(quant_scale));

    sign_mask = _mm_set_ss(g_xmmword_1485CFD10);
    xmm2 = sign_mask;
    xmm3 = _mm_andnot_ps(sign_mask, xmm0);
    xmm1 = _mm_round_ps(xmm3, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    xmm3 = _mm_sub_ss(xmm3, xmm1);
    xmm3 = _mm_cmpeq_ss(xmm3, _mm_set_ss(g_xmmword_1485CFCF0));
    xmm3 = _mm_and_ps(xmm3, _mm_load_ps(g_xmmword_1485D4980));
    xmm3 = _mm_add_ss(xmm3, xmm1);
    xmm2 = _mm_and_ps(xmm2, xmm0);
    xmm3 = _mm_or_ps(xmm3, xmm2);
    int g_int = _mm_cvttss_si32(xmm3);

    /* Process B channel */
    int b_negative = (_mm_comilt_ss(xmm11, xmm4) != 0);
    if (b_negative) {
        xmm4 = _mm_setzero_ps();
    } else {
        xmm4 = _mm_min_ss(xmm11, _mm_set_ss(clamp_max));
    }
    xmm0 = _mm_mul_ss(xmm4, _mm_set_ss(quant_scale));

    sign_mask = _mm_set_ss(g_xmmword_1485CFD10);
    xmm2 = sign_mask;
    xmm3 = _mm_andnot_ps(sign_mask, xmm0);
    xmm1 = _mm_round_ps(xmm3, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    xmm3 = _mm_sub_ss(xmm3, xmm1);
    xmm3 = _mm_cmpeq_ss(xmm3, _mm_set_ss(g_xmmword_1485CFCF0));
    xmm3 = _mm_and_ps(xmm3, _mm_load_ps(g_xmmword_1485D4980));
    xmm3 = _mm_add_ss(xmm3, xmm1);
    xmm2 = _mm_and_ps(xmm2, xmm0);
    xmm3 = _mm_or_ps(xmm3, xmm2);
    int b_int = _mm_cvttss_si32(xmm3);

    /* Store RGBA bytes */
    out[0] = (uint8_t)r_int;
    out[1] = (uint8_t)g_int;
    out[2] = (uint8_t)b_int;
    out[3] = (uint8_t)alpha_int;

    return out;
}

/* ============================================================================
 * Large structure copy functions (sub_14054FFA0, sub_140550110, etc.)
 * Uses: vmovups ymm0/ymm1, vmovq, vpextrq, xor-based hash
 * ============================================================================ */
typedef struct __attribute__((aligned(32))) {
    uint8_t data[0xE0];  /* Large data block copied with YMM */
} LargeStruct;

typedef struct __attribute__((aligned(16))) {
    uint64_t field_38[2];  /* xmm1 source at offset 0x38 */
    uint64_t field_40;     /* XOR operand */
    void*    vtable_88;    /* Pointer at offset 0x88 */
} ObjectHeader;

/* Hash computation pattern from sub_14054FFA0 */
__attribute__((noinline))
uint64_t compute_struct_hash(const ObjectHeader* obj, const LargeStruct* data) {
    __m128i xmm0, xmm1;
    uint64_t hash;

    /* vmovups xmm1, xmmword ptr [rcx+38h] */
    xmm1 = _mm_loadu_si128((__m128i*)obj->field_38);

    /* vmovups xmm0, xmmword ptr [r11-80h] - from copied data */
    xmm0 = _mm_loadu_si128((__m128i*)&data->data[0x80]);

    /* mov ecx, [r11-70h] */
    uint32_t ecx_val = *(uint32_t*)&data->data[0x90];
    hash = ecx_val;

    /* vmovq rcx, xmm0 */
    uint64_t xmm0_lo = _mm_cvtsi128_si64(xmm0);
    hash ^= xmm0_lo;

    /* vpextrq rcx, xmm0, 1 */
    uint64_t xmm0_hi = _mm_extract_epi64(xmm0, 1);
    hash ^= xmm0_hi;

    /* xor rdx, [rbx+40h] */
    hash ^= obj->field_40;

    /* vmovq rcx, xmm1 */
    uint64_t xmm1_lo = _mm_cvtsi128_si64(xmm1);
    hash ^= xmm1_lo;

    /* xor rdx, rax (first qword of data) */
    hash ^= *(uint64_t*)data->data;

    /* mov rax, 1234567898765432h */
    /* xor rdx, rax */
    hash ^= 0x1234567898765432ULL;

    return hash;
}

/* Full structure copy with YMM registers */
__attribute__((noinline))
void copy_large_struct(LargeStruct* dst, const LargeStruct* src) {
    __m256i ymm0, ymm1;

    /* vmovups ymm0, ymmword ptr [rdx] */
    ymm0 = _mm256_loadu_si256((__m256i*)&src->data[0]);

    /* vmovups ymm1, ymmword ptr [rdx+80h] */
    ymm1 = _mm256_loadu_si256((__m256i*)&src->data[0x80]);

    /* vmovups ymmword ptr [rax], ymm0 */
    _mm256_storeu_si256((__m256i*)&dst->data[0], ymm0);

    /* Continue with remaining blocks */
    ymm0 = _mm256_loadu_si256((__m256i*)&src->data[0x20]);
    _mm256_storeu_si256((__m256i*)&dst->data[0x20], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&src->data[0x40]);
    _mm256_storeu_si256((__m256i*)&dst->data[0x40], ymm0);

    /* XMM operations for 16-byte blocks */
    __m128i xmm0 = _mm_loadu_si128((__m128i*)&src->data[0x60]);
    _mm_storeu_si128((__m128i*)&dst->data[0x60], xmm0);

    xmm0 = _mm_loadu_si128((__m128i*)&src->data[0x70]);
    _mm_storeu_si128((__m128i*)&dst->data[0x70], xmm0);

    /* Second YMM block from offset 0x80 */
    _mm256_storeu_si256((__m256i*)&dst->data[0x80], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&src->data[0xA0]);
    _mm256_storeu_si256((__m256i*)&dst->data[0xA0], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&src->data[0xC0]);
    _mm256_storeu_si256((__m256i*)&dst->data[0xC0], ymm1);

    /* vzeroupper */
    _mm256_zeroupper();
}

/* ============================================================================
 * __InitializeDynamicThreadSafeBitSetAllocator__ pattern
 * Uses: lock cmpxchg, vmovups ymm0, vmovq
 * ============================================================================ */
static volatile int32_t g_allocator_lock = 0;
static int32_t g_thread_id_cache = -1;
static int32_t g_ref_count = 0;

typedef struct {
    uint64_t base_ptr;
    uint64_t size;
    uint64_t metadata[2];
} AllocatorState;

__attribute__((noinline))
void init_bitset_allocator(
    uint64_t* out_base,
    uint64_t* out_limit,
    void** out_alloc_func,
    void** out_free_func,
    void** out_realloc_func
) {
    int32_t thread_id = 12345;  /* Simulated thread ID */

    /* Check cached thread ID */
    if (g_thread_id_cache == thread_id) {
        __atomic_fetch_add(&g_ref_count, 1, __ATOMIC_SEQ_CST);
        goto setup_pointers;
    }

    /* lock cmpxchg cs:dword_149E00138, ecx */
    int32_t expected = 0;
    int32_t desired = 1;

    if (!__atomic_compare_exchange_n(&g_allocator_lock, &expected, desired,
                                      0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) {
        /* Lock contention - spin wait pattern */
        (void)expected;  /* Original value before failed CAS */
        while (g_allocator_lock != 0) {
            __asm__ volatile("pause" ::: "memory");
        }
    }

    g_thread_id_cache = thread_id;

setup_pointers:;
    /* Check if allocator already initialized */
    static AllocatorState allocator_state = {0};

    if (allocator_state.base_ptr == 0) {
        /* Initialize allocator state with vmovups pattern */
        __m256i ymm0;

        allocator_state.base_ptr = 0x100000000ULL;
        allocator_state.size = 0x10000000ULL;
        allocator_state.metadata[0] = 0;
        allocator_state.metadata[1] = 0;

        /* vmovups ymm0, ymmword ptr [rax] */
        ymm0 = _mm256_loadu_si256((__m256i*)&allocator_state);

        /* vmovups ymmword ptr cs:qword_149E00118, ymm0 */
        _mm256_storeu_si256((__m256i*)&g_qword_149E00118, ymm0);

        /* vmovq rax, xmm0 */
        __m128i xmm0 = _mm256_castsi256_si128(ymm0);
        uint64_t base = _mm_cvtsi128_si64(xmm0);

        *out_base = base;
    } else {
        *out_base = allocator_state.base_ptr;
    }

    /* Set up function pointers */
    *out_limit = allocator_state.base_ptr + 0x10000000ULL;
    *out_alloc_func = (void*)0xDEADBEEF;
    *out_free_func = (void*)0xCAFEBABE;
    *out_realloc_func = (void*)0x12345678;

    /* Decrement ref count and potentially release lock */
    int32_t count = __atomic_sub_fetch(&g_ref_count, 1, __ATOMIC_SEQ_CST);
    if (count == 0) {
        g_thread_id_cache = -1;
        __atomic_store_n(&g_allocator_lock, 0, __ATOMIC_SEQ_CST);
    }

    _mm256_zeroupper();
}

/* ============================================================================
 * sub_14054FFA0 / sub_140550110 / sub_140550FF0 / sub_140552980 patterns
 * Full implementation of the request dispatch with YMM copy and hash
 * ============================================================================ */
typedef struct __attribute__((aligned(32))) {
    uint8_t request_data[0xE0];
} RequestData;

typedef struct __attribute__((aligned(16))) {
    uint64_t vtable_ptr;
    uint8_t padding[0x30];
    uint64_t session_key[2];  /* offset 0x38 */
    uint64_t context_id;      /* offset 0x40 (via [rbx+40h]) */
    uint8_t padding2[0x40];
    void* handler_table;      /* offset 0x88 */
} RequestContext;

/* Simulated vtable */
typedef struct {
    uint8_t padding[0x40];
    void* (*dispatch_func)(void* ctx, void* req);
    uint8_t padding2[0x40];
    void (*process_func)(void* ctx, void* data, void* result);
    void (*process_alt_func)(void* ctx, void* data);
} HandlerVTable;

__attribute__((noinline))
void* sub_14054FFA0_impl(RequestContext* ctx, const RequestData* request, void* result_buf) {
    __attribute__((aligned(32))) uint8_t local_copy[0x188];
    __m256i ymm0, ymm1;
    __m128i xmm0, xmm1;

    /* Copy request data using YMM registers */
    /* vmovups ymm0, ymmword ptr [rdx] */
    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0]);

    /* vmovups ymm1, ymmword ptr [rdx+80h] */
    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0x80]);

    /* Store first block */
    _mm256_storeu_si256((__m256i*)&local_copy[0], ymm0);

    /* Continue copying remaining 256-bit blocks */
    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x20]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x20], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x40]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x40], ymm0);

    /* 128-bit blocks */
    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x60]);
    _mm_storeu_si128((__m128i*)&local_copy[0x60], xmm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x70]);
    _mm_storeu_si128((__m128i*)&local_copy[0x70], xmm0);

    /* Store second YMM block */
    _mm256_storeu_si256((__m256i*)&local_copy[0x80], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xA0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xA0], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xC0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xC0], ymm1);

    /* Hash computation using XOR chain with vmovq and vpextrq */
    /* vmovups xmm1, xmmword ptr [rcx+38h] */
    xmm1 = _mm_loadu_si128((__m128i*)ctx->session_key);

    /* vmovups xmm0, xmmword ptr [r11-80h] */
    xmm0 = _mm_loadu_si128((__m128i*)&local_copy[0x108]);

    /* Build hash */
    uint64_t first_qword = *(uint64_t*)&local_copy[0];
    uint32_t ecx_val = *(uint32_t*)&local_copy[0x118];
    (void)*(uint64_t*)&local_copy[0x38];  /* r8 in original at 0x38 */

    uint64_t hash = ecx_val;

    /* vmovq rcx, xmm0 */
    hash ^= _mm_cvtsi128_si64(xmm0);

    /* vpextrq rcx, xmm0, 1 */
    hash ^= _mm_extract_epi64(xmm0, 1);

    /* xor rdx, [rbx+40h] */
    hash ^= ctx->context_id;

    /* vmovq rcx, xmm1 */
    hash ^= _mm_cvtsi128_si64(xmm1);

    /* xor rdx, rax (first qword) */
    hash ^= first_qword;

    /* xor rdx, 1234567898765432h */
    hash ^= 0x1234567898765432ULL;

    /* Prepare dispatch structure */
    __attribute__((aligned(32))) uint8_t dispatch_data[0x60];

    /* vmovups xmmword ptr [rsp+var_168+8], xmm1 */
    _mm_storeu_si128((__m128i*)&dispatch_data[0x8], xmm1);

    /* vmovups xmmword ptr [rsp+var_168+18h], xmm0 */
    _mm_storeu_si128((__m128i*)&dispatch_data[0x18], xmm0);

    /* Store first qword */
    *(uint64_t*)&dispatch_data[0] = first_qword;

    /* vmovups ymm0, [rsp+var_168] */
    ymm0 = _mm256_loadu_si256((__m256i*)dispatch_data);

    /* vmovups xmm1, xmmword ptr [rsp+40h] */
    xmm1 = _mm_loadu_si128((__m128i*)&local_copy[0x40]);

    /* Store hash */
    *(uint64_t*)&dispatch_data[0x28] = hash;

    /* vmovups [rsp+var_128], ymm0 */
    _mm256_storeu_si256((__m256i*)&dispatch_data[0x28], ymm0);

    /* vmovsd xmm0, [rsp+var_138] */
    __m128d hash_vec = _mm_load_sd((double*)&hash);

    /* vmovsd [rsp+var_F8], xmm0 */
    _mm_store_sd((double*)&dispatch_data[0x50], hash_vec);

    /* vmovups [rsp+var_108], xmm1 */
    _mm_storeu_si128((__m128i*)&dispatch_data[0x40], xmm1);

    /* vzeroupper before call */
    _mm256_zeroupper();

    /* Simulate dispatch call */
    void* dispatch_result = (void*)0xABCDEF12;

    if (dispatch_result != NULL) {
        /* Simulate vtable call at [rax+90h] */
        FORCE_USE(local_copy);
        FORCE_USE(result_buf);
    }

    return dispatch_result;
}

/* ============================================================================
 * sub_140550260 - Request dispatch with string formatting
 * Includes additional vmovups patterns and string operations
 * ============================================================================ */
typedef struct {
    uint64_t length;
    uint64_t capacity;
    char* buffer;
} DynamicString;

__attribute__((noinline))
void sub_140550260_impl(RequestContext* ctx, const RequestData* request, DynamicString* error_out) {
    __attribute__((aligned(32))) uint8_t local_copy[0x300];
    __m256i ymm0, ymm1;
    __m128i xmm0, xmm1;

    /* Full YMM copy sequence (same as sub_14054FFA0) */
    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0]);
    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0x80]);

    _mm256_storeu_si256((__m256i*)&local_copy[0], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x20]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x20], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x40]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x40], ymm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x60]);
    _mm_storeu_si128((__m128i*)&local_copy[0x60], xmm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x70]);
    _mm_storeu_si128((__m128i*)&local_copy[0x70], xmm0);

    _mm256_storeu_si256((__m256i*)&local_copy[0x80], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xA0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xA0], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xC0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xC0], ymm1);

    /* Hash computation */
    xmm1 = _mm_loadu_si128((__m128i*)ctx->session_key);
    xmm0 = _mm_loadu_si128((__m128i*)&local_copy[0x8]);

    uint64_t first_qword = *(uint64_t*)&local_copy[0];
    uint32_t ecx_val = *(uint32_t*)&local_copy[0x18];

    uint64_t hash = ecx_val;
    hash ^= _mm_cvtsi128_si64(xmm0);
    hash ^= _mm_extract_epi64(xmm0, 1);
    hash ^= ctx->context_id;
    hash ^= _mm_cvtsi128_si64(xmm1);
    hash ^= first_qword;
    hash ^= 0x1234567898765432ULL;

    /* Prepare dispatch data */
    __attribute__((aligned(32))) uint8_t dispatch_data[0x60];
    *(uint64_t*)&dispatch_data[0] = first_qword;

    _mm_storeu_si128((__m128i*)&dispatch_data[0x8], xmm1);
    _mm_storeu_si128((__m128i*)&dispatch_data[0x18], xmm0);

    ymm0 = _mm256_loadu_si256((__m256i*)dispatch_data);
    xmm1 = _mm_loadu_si128((__m128i*)&local_copy[0x40]);

    *(uint64_t*)&dispatch_data[0x28] = hash;
    _mm256_storeu_si256((__m256i*)&dispatch_data[0x28], ymm0);

    __m128d hash_vec = _mm_load_sd((double*)&hash);
    _mm_store_sd((double*)&dispatch_data[0x50], hash_vec);
    _mm_storeu_si128((__m128i*)&dispatch_data[0x40], xmm1);

    _mm256_zeroupper();

    /* Simulate dispatch - returns NULL for error case */
    void* dispatch_result = NULL;

    if (dispatch_result == NULL) {
        /* Error path - format error string */
        static char error_buffer[512];
        const char* source_name = "Source";
        const char* error_desc = "Dispatch failed";

        snprintf(error_buffer, sizeof(error_buffer), "%s (%s)", error_desc, source_name);

        size_t len = strlen(error_buffer);

        if (error_out->capacity < len) {
            /* Reallocate (simulated) */
            error_out->buffer = error_buffer;
            error_out->capacity = sizeof(error_buffer);
        }

        memcpy(error_out->buffer, error_buffer, len);
        error_out->length = len;
        error_out->buffer[len] = '\0';
    }
}

/* ============================================================================
 * sub_140550FF0 - Task initialization dispatch
 * Similar YMM copy with task initialization call
 * ============================================================================ */
__attribute__((noinline))
void sub_140550FF0_impl(RequestContext* ctx, const RequestData* request, void* task_result) {
    __attribute__((aligned(32))) uint8_t local_copy[0x188];
    __m256i ymm0, ymm1;
    __m128i xmm0, xmm1;

    /* Full copy sequence */
    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0]);
    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0x80]);

    _mm256_storeu_si256((__m256i*)&local_copy[0], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x20]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x20], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x40]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x40], ymm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x60]);
    _mm_storeu_si128((__m128i*)&local_copy[0x60], xmm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x70]);
    _mm_storeu_si128((__m128i*)&local_copy[0x70], xmm0);

    _mm256_storeu_si256((__m256i*)&local_copy[0x80], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xA0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xA0], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xC0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xC0], ymm1);

    /* Hash computation */
    xmm1 = _mm_loadu_si128((__m128i*)ctx->session_key);
    xmm0 = _mm_loadu_si128((__m128i*)&local_copy[0x108]);

    uint64_t first_qword = *(uint64_t*)&local_copy[0];
    uint32_t ecx_val = *(uint32_t*)&local_copy[0x118];
    (void)*(uint64_t*)&local_copy[0x38];  /* r8 in original at 0x38 */

    uint64_t hash = ecx_val;
    hash ^= _mm_cvtsi128_si64(xmm0);
    hash ^= _mm_extract_epi64(xmm0, 1);
    hash ^= ctx->context_id;
    hash ^= _mm_cvtsi128_si64(xmm1);
    hash ^= first_qword;
    hash ^= 0x1234567898765432ULL;

    /* Build dispatch structure */
    __attribute__((aligned(32))) uint8_t dispatch_data[0x60];
    *(uint64_t*)&dispatch_data[0] = first_qword;

    _mm_storeu_si128((__m128i*)&dispatch_data[0x8], xmm1);
    _mm_storeu_si128((__m128i*)&dispatch_data[0x18], xmm0);

    ymm0 = _mm256_loadu_si256((__m256i*)dispatch_data);
    xmm1 = _mm_loadu_si128((__m128i*)&local_copy[0x40]);

    *(uint64_t*)&dispatch_data[0x28] = hash;
    _mm256_storeu_si256((__m256i*)&dispatch_data[0x28], ymm0);

    __m128d hash_vec = _mm_load_sd((double*)&hash);
    _mm_store_sd((double*)&dispatch_data[0x50], hash_vec);
    _mm_storeu_si128((__m128i*)&dispatch_data[0x40], xmm1);

    _mm256_zeroupper();

    /* Dispatch check */
    void* dispatch_result = (void*)0;  /* NULL triggers task init */

    if (dispatch_result == NULL) {
        /* call Subsumption__Task__InitializeTask */
        FORCE_USE(local_copy);
        FORCE_USE(dispatch_data);
    }

    /* Final vtable call at [rax+88h] */
    FORCE_USE(task_result);
}

/* ============================================================================
 * sub_140552980 - Two-phase dispatch with YMM rebuild
 * ============================================================================ */
__attribute__((noinline))
void sub_140552980_impl(RequestContext* ctx, const RequestData* request) {
    __attribute__((aligned(32))) uint8_t local_copy[0x180];
    __m256i ymm0, ymm1;
    __m128i xmm0, xmm1;
    __m128d xmm_d;

    /* Full copy sequence */
    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0]);
    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0x80]);

    _mm256_storeu_si256((__m256i*)&local_copy[0], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x20]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x20], ymm0);

    ymm0 = _mm256_loadu_si256((__m256i*)&request->request_data[0x40]);
    _mm256_storeu_si256((__m256i*)&local_copy[0x40], ymm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x60]);
    _mm_storeu_si128((__m128i*)&local_copy[0x60], xmm0);

    xmm0 = _mm_loadu_si128((__m128i*)&request->request_data[0x70]);
    _mm_storeu_si128((__m128i*)&local_copy[0x70], xmm0);

    _mm256_storeu_si256((__m256i*)&local_copy[0x80], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xA0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xA0], ymm1);

    ymm1 = _mm256_loadu_si256((__m256i*)&request->request_data[0xC0]);
    _mm256_storeu_si256((__m256i*)&local_copy[0xC0], ymm1);

    /* Hash computation - note different xmm order */
    xmm1 = _mm_loadu_si128((__m128i*)ctx->session_key);
    xmm0 = _mm_loadu_si128((__m128i*)&local_copy[0x8]);

    uint64_t first_qword = *(uint64_t*)&local_copy[0];
    uint32_t ecx_val = *(uint32_t*)&local_copy[0x18];
    (void)*(uint64_t*)&local_copy[0x40];  /* r8 in original at 0x40 */

    uint64_t hash = ecx_val;
    hash ^= _mm_cvtsi128_si64(xmm0);
    hash ^= _mm_extract_epi64(xmm0, 1);
    hash ^= ctx->context_id;
    hash ^= _mm_cvtsi128_si64(xmm1);
    hash ^= first_qword;
    hash ^= 0x1234567898765432ULL;

    /* Build dispatch - note xmm1/xmm0 order swapped vs other functions */
    __attribute__((aligned(32))) uint8_t dispatch_data[0x60];
    *(uint64_t*)&dispatch_data[0] = first_qword;

    /* vmovups xmmword ptr [rsp+var_160+8], xmm1 */
    _mm_storeu_si128((__m128i*)&dispatch_data[0x8], xmm1);

    /* vmovups xmmword ptr [rsp+var_160+18h], xmm0 */
    _mm_storeu_si128((__m128i*)&dispatch_data[0x18], xmm0);

    ymm0 = _mm256_loadu_si256((__m256i*)dispatch_data);

    *(uint64_t*)&dispatch_data[0x28] = hash;

    /* vmovsd xmm1, [rsp+var_130] - note: uses xmm1 for hash here */
    xmm_d = _mm_load_sd((double*)&hash);

    _mm256_storeu_si256((__m256i*)&dispatch_data[0x28], ymm0);

    xmm0 = _mm_loadu_si128((__m128i*)&local_copy[0x40]);

    _mm_store_sd((double*)&dispatch_data[0x50], xmm_d);
    _mm_storeu_si128((__m128i*)&dispatch_data[0x40], xmm0);

    _mm256_zeroupper();

    /* First dispatch */
    void* dispatch_result = (void*)0xDEAD;

    if (dispatch_result != NULL) {
        /* First vtable call at [rax+80h] */
        FORCE_USE(local_copy);

        /* Rebuild dispatch structure for second call */
        ymm0 = _mm256_loadu_si256((__m256i*)dispatch_data);

        _mm256_storeu_si256((__m256i*)&dispatch_data[0x28], ymm0);

        xmm0 = _mm_loadu_si128((__m128i*)&local_copy[0x40]);
        _mm_storeu_si128((__m128i*)&dispatch_data[0x40], xmm0);

        xmm_d = _mm_load_sd((double*)&hash);
        _mm_store_sd((double*)&dispatch_data[0x50], xmm_d);

        _mm256_zeroupper();

        /* Second vtable call at [rax+48h] */
        FORCE_USE(dispatch_data);
    }
}

/* ============================================================================
 * Test harness
 * ============================================================================ */
void print_xmm(__m128 v, const char* name) {
    float vals[4];
    _mm_storeu_ps(vals, v);
    printf("%s: [%.6f, %.6f, %.6f, %.6f]\n", name, vals[0], vals[1], vals[2], vals[3]);
}

void print_ymm(__m256 v, const char* name) {
    float vals[8];
    _mm256_storeu_ps(vals, v);
    printf("%s: [%.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f, %.6f]\n",
           name, vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], vals[6], vals[7]);
}

int main(int argc, char** argv) {
    printf("=== AVX/AVX2 Instruction Lifter Test Suite ===\n\n");

    /* Test sub_1402C6260 - rounding function */
    printf("--- Testing sub_1402C6260 (sign-preserving round) ---\n");
    float test_vals[] = {1.5f, -1.5f, 2.5f, -2.5f, 0.0f, 3.7f, -3.7f};
    for (int i = 0; i < sizeof(test_vals)/sizeof(test_vals[0]); i++) {
        float result = sub_1402C6260(test_vals[i]);
        printf("  round(%.2f) = %.2f\n", test_vals[i], result);
    }

    /* Test sub_140322E70 - vector angle calculation */
    printf("\n--- Testing sub_140322E70 (3D vector angle) ---\n");
    Vec3d positions[] = {
        {100.0, 200.0, 300.0},
        {0.0, 0.0, 0.0},
        {1000.0, 500.0, 250.0}
    };
    for (int i = 0; i < sizeof(positions)/sizeof(positions[0]); i++) {
        float angle = sub_140322E70(&positions[i]);
        printf("  pos(%.1f, %.1f, %.1f) -> angle = %.2f deg\n",
               positions[i].x, positions[i].y, positions[i].z, angle);
    }

    /* Test sub_14032C890 - bearing calculation */
    printf("\n--- Testing sub_14032C890 (bearing calculation) ---\n");
    Position3d targets[] = {
        {150.0, 250.0, 350.0},
        {50.0, 150.0, 250.0}
    };
    for (int mode = 0; mode <= 1; mode++) {
        for (int i = 0; i < sizeof(targets)/sizeof(targets[0]); i++) {
            float bearing = sub_14032C890(&targets[i], mode);
            printf("  target(%.1f, %.1f, %.1f) mode=%d -> bearing = %.2f\n",
                   targets[i].x, targets[i].y, targets[i].z, mode, bearing);
        }
    }

    /* Test sub_140331470 - velocity accumulator */
    printf("\n--- Testing sub_140331470 (velocity accumulator) ---\n");
    VelocityAccum accum = {
        .current = {0.0f, 0.0f, 0.0f},
        .max_val = 10.0f,
        .accumulated = {0.0f, 0.0f, 0.0f}
    };
    double velocity[] = {1.0, 2.0, 3.0};

    printf("  Before: current=[%.2f, %.2f, %.2f] max=%.2f\n",
           accum.current[0], accum.current[1], accum.current[2], accum.max_val);

    sub_140331470(&accum, velocity);

    printf("  After:  current=[%.2f, %.2f, %.2f] max=%.2f\n",
           accum.current[0], accum.current[1], accum.current[2], accum.max_val);

    /* Test sub_14052B7D0 - color quantization */
    printf("\n--- Testing sub_14052B7D0 (RGBA quantization) ---\n");
    ColorRGBA colors[] = {
        {1.0f, 0.5f, 0.25f, 0.75f},
        {0.0f, 1.0f, 0.0f, 1.0f},
        {-0.1f, 1.5f, 0.5f, 0.5f}  /* Test clamping */
    };
    for (int i = 0; i < sizeof(colors)/sizeof(colors[0]); i++) {
        uint8_t rgba[4];
        sub_14052B7D0(&colors[i], rgba);
        printf("  RGBA(%.2f, %.2f, %.2f, %.2f) -> [%d, %d, %d, %d]\n",
               colors[i].r, colors[i].g, colors[i].b, colors[i].a,
               rgba[0], rgba[1], rgba[2], rgba[3]);
    }

    /* Test large structure copy and hash */
    printf("\n--- Testing copy_large_struct (YMM copy) ---\n");
    LargeStruct src, dst;
    memset(&src, 0xAB, sizeof(src));
    memset(&dst, 0, sizeof(dst));

    copy_large_struct(&dst, &src);

    int match = (memcmp(&src, &dst, sizeof(LargeStruct)) == 0);
    printf("  Copy verification: %s\n", match ? "PASS" : "FAIL");

    /* Test hash computation */
    printf("\n--- Testing compute_struct_hash (XOR chain) ---\n");
    ObjectHeader obj = {
        .field_38 = {0x1111111111111111ULL, 0x2222222222222222ULL},
        .field_40 = 0x3333333333333333ULL
    };
    uint64_t hash = compute_struct_hash(&obj, &src);
    printf("  Hash result: 0x%016" PRIx64 "\n", hash);

    /* Test allocator initialization */
    printf("\n--- Testing init_bitset_allocator (lock cmpxchg + YMM) ---\n");
    uint64_t base, limit;
    void *alloc_fn, *free_fn, *realloc_fn;
    init_bitset_allocator(&base, &limit, &alloc_fn, &free_fn, &realloc_fn);
    printf("  Base: 0x%016" PRIx64 ", Limit: 0x%016" PRIx64 "\n", base, limit);

    /* Test request dispatch functions */
    printf("\n--- Testing request dispatch functions ---\n");
    RequestContext ctx = {
        .session_key = {0xAAAAAAAAAAAAAAAAULL, 0xBBBBBBBBBBBBBBBBULL},
        .context_id = 0xCCCCCCCCCCCCCCCCULL
    };
    RequestData req;
    memset(&req, 0x55, sizeof(req));

    uint8_t result_buf[256];
    void* result = sub_14054FFA0_impl(&ctx, &req, result_buf);
    printf("  sub_14054FFA0: result=%p\n", result);

    DynamicString error_str = {0, 512, NULL};
    error_str.buffer = (char*)malloc(512);
    sub_140550260_impl(&ctx, &req, &error_str);
    printf("  sub_140550260: error=\"%s\"\n", error_str.buffer ? error_str.buffer : "(null)");
    free(error_str.buffer);

    sub_140550FF0_impl(&ctx, &req, result_buf);
    printf("  sub_140550FF0: completed\n");

    sub_140552980_impl(&ctx, &req);
    printf("  sub_140552980: completed\n");

    printf("\n=== All tests completed ===\n");
    return 0;
}
