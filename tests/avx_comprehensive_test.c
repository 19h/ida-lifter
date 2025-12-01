/*
 * Comprehensive AVX/AVX2/AVX-512 Instruction Test Suite
 * 
 * This program exercises an extremely diverse set of vector instructions
 * across all three major extensions, covering:
 * - Arithmetic (packed/scalar, float/double/integer)
 * - FMA variants (132/213/231, add/sub/addsub)
 * - Logical operations (and/or/xor/andnot/ternlog)
 * - Comparisons (all predicates, packed/scalar)
 * - Shuffle/Permute/Blend operations
 * - Broadcast operations
 * - Gather/Scatter operations
 * - Masked operations (merge/zero masking)
 * - Conversion operations
 * - Horizontal operations
 * - Shift operations (immediate/variable)
 * - Insert/Extract operations
 * - Special functions (sqrt, rsqrt, rcp, round)
 * - AVX-512 specific (ternlog, conflict, compress, expand, etc.)
 *
 * Compile with:
 *   gcc -O2 -mavx -mavx2 -mfma -mavx512f -mavx512bw -mavx512dq -mavx512vl \
 *       -mavx512cd -mavx512vbmi -mavx512ifma -o avx_test avx_comprehensive_test.c -lm
 *
 * For clang:
 *   clang -O2 -mavx -mavx2 -mfma -mavx512f -mavx512bw -mavx512dq -mavx512vl \
 *         -mavx512cd -mavx512vbmi -mavx512ifma -o avx_test avx_comprehensive_test.c -lm
 */

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* Alignment macros */
#define ALIGN32 __attribute__((aligned(32)))
#define ALIGN64 __attribute__((aligned(64)))

/* Prevent optimization of results */
#define FORCE_USE(x) __asm__ volatile("" :: "r,m"(x) : "memory")
#define FORCE_USE_PTR(x) __asm__ volatile("" :: "r"(x) : "memory")

/* Test result accumulator to prevent dead code elimination */
static volatile double g_accumulator = 0.0;
static volatile int64_t g_int_accumulator = 0;

/*============================================================================
 * SECTION 1: AVX ARITHMETIC OPERATIONS (256-bit, float/double)
 *===========================================================================*/

__attribute__((noinline))
void test_avx_arithmetic_ps(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 a = _mm256_loadu_ps(&src1[i]);
        __m256 b = _mm256_loadu_ps(&src2[i]);
        
        /* Basic arithmetic */
        __m256 sum = _mm256_add_ps(a, b);
        __m256 diff = _mm256_sub_ps(a, b);
        __m256 prod = _mm256_mul_ps(a, b);
        __m256 quot = _mm256_div_ps(a, b);
        
        /* Min/Max */
        __m256 vmin = _mm256_min_ps(a, b);
        __m256 vmax = _mm256_max_ps(a, b);
        
        /* Special functions */
        __m256 vsqrt = _mm256_sqrt_ps(_mm256_max_ps(a, _mm256_setzero_ps()));
        __m256 vrsqrt = _mm256_rsqrt_ps(_mm256_max_ps(b, _mm256_set1_ps(0.0001f)));
        __m256 vrcp = _mm256_rcp_ps(_mm256_max_ps(b, _mm256_set1_ps(0.0001f)));
        
        /* Rounding */
        __m256 vround = _mm256_round_ps(sum, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        __m256 vfloor = _mm256_floor_ps(diff);
        __m256 vceil = _mm256_ceil_ps(prod);
        
        /* Combine all results */
        __m256 r1 = _mm256_add_ps(sum, diff);
        __m256 r2 = _mm256_add_ps(prod, quot);
        __m256 r3 = _mm256_add_ps(vmin, vmax);
        __m256 r4 = _mm256_add_ps(vsqrt, vrsqrt);
        __m256 r5 = _mm256_add_ps(vrcp, vround);
        __m256 r6 = _mm256_add_ps(vfloor, vceil);
        
        __m256 result = _mm256_add_ps(_mm256_add_ps(r1, r2), 
                                       _mm256_add_ps(_mm256_add_ps(r3, r4), 
                                                     _mm256_add_ps(r5, r6)));
        _mm256_storeu_ps(&dst[i], result);
    }
}

__attribute__((noinline))
void test_avx_arithmetic_pd(double *src1, double *src2, double *dst, int n) {
    for (int i = 0; i < n; i += 4) {
        __m256d a = _mm256_loadu_pd(&src1[i]);
        __m256d b = _mm256_loadu_pd(&src2[i]);
        
        /* Basic arithmetic */
        __m256d sum = _mm256_add_pd(a, b);
        __m256d diff = _mm256_sub_pd(a, b);
        __m256d prod = _mm256_mul_pd(a, b);
        __m256d quot = _mm256_div_pd(a, b);
        
        /* Min/Max */
        __m256d vmin = _mm256_min_pd(a, b);
        __m256d vmax = _mm256_max_pd(a, b);
        
        /* Special functions */
        __m256d vsqrt = _mm256_sqrt_pd(_mm256_max_pd(a, _mm256_setzero_pd()));
        
        /* Rounding */
        __m256d vround = _mm256_round_pd(sum, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
        __m256d vfloor = _mm256_floor_pd(diff);
        __m256d vceil = _mm256_ceil_pd(prod);
        
        /* Horizontal add/sub (AVX) */
        __m256d hadd = _mm256_hadd_pd(a, b);
        __m256d hsub = _mm256_hsub_pd(a, b);
        
        /* Addsub */
        __m256d vaddsub = _mm256_addsub_pd(a, b);
        
        /* Combine */
        __m256d r1 = _mm256_add_pd(sum, diff);
        __m256d r2 = _mm256_add_pd(prod, quot);
        __m256d r3 = _mm256_add_pd(vmin, vmax);
        __m256d r4 = _mm256_add_pd(vsqrt, vround);
        __m256d r5 = _mm256_add_pd(vfloor, vceil);
        __m256d r6 = _mm256_add_pd(hadd, hsub);
        __m256d r7 = vaddsub;
        
        __m256d result = _mm256_add_pd(_mm256_add_pd(r1, r2),
                                        _mm256_add_pd(_mm256_add_pd(r3, r4),
                                                      _mm256_add_pd(r5, _mm256_add_pd(r6, r7))));
        _mm256_storeu_pd(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 2: AVX SCALAR OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
float test_avx_scalar_ss(float a, float b) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 zero = _mm_setzero_ps();
    
    /* Scalar arithmetic */
    __m128 sum = _mm_add_ss(va, vb);
    __m128 diff = _mm_sub_ss(va, vb);
    __m128 prod = _mm_mul_ss(va, vb);
    __m128 quot = _mm_div_ss(va, _mm_max_ss(vb, _mm_set_ss(0.0001f)));
    
    /* Scalar min/max */
    __m128 vmin = _mm_min_ss(va, vb);
    __m128 vmax = _mm_max_ss(va, vb);
    
    /* Scalar sqrt/rsqrt/rcp */
    __m128 vsqrt = _mm_sqrt_ss(_mm_max_ss(va, zero));
    __m128 vrsqrt = _mm_rsqrt_ss(_mm_max_ss(vb, _mm_set_ss(0.0001f)));
    __m128 vrcp = _mm_rcp_ss(_mm_max_ss(vb, _mm_set_ss(0.0001f)));
    
    /* Scalar rounding */
    __m128 vround = _mm_round_ss(zero, sum, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    
    /* Combine all */
    __m128 r1 = _mm_add_ss(sum, diff);
    __m128 r2 = _mm_add_ss(prod, quot);
    __m128 r3 = _mm_add_ss(vmin, vmax);
    __m128 r4 = _mm_add_ss(vsqrt, vrsqrt);
    __m128 r5 = _mm_add_ss(vrcp, vround);
    
    __m128 result = _mm_add_ss(_mm_add_ss(r1, r2), _mm_add_ss(_mm_add_ss(r3, r4), r5));
    return _mm_cvtss_f32(result);
}

__attribute__((noinline))
double test_avx_scalar_sd(double a, double b) {
    __m128d va = _mm_set_sd(a);
    __m128d vb = _mm_set_sd(b);
    __m128d zero = _mm_setzero_pd();
    
    /* Scalar arithmetic */
    __m128d sum = _mm_add_sd(va, vb);
    __m128d diff = _mm_sub_sd(va, vb);
    __m128d prod = _mm_mul_sd(va, vb);
    __m128d quot = _mm_div_sd(va, _mm_max_sd(vb, _mm_set_sd(0.0001)));
    
    /* Scalar min/max */
    __m128d vmin = _mm_min_sd(va, vb);
    __m128d vmax = _mm_max_sd(va, vb);
    
    /* Scalar sqrt */
    __m128d vsqrt = _mm_sqrt_sd(zero, _mm_max_sd(va, zero));
    
    /* Scalar rounding */
    __m128d vround = _mm_round_sd(zero, sum, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
    
    /* Combine all */
    __m128d r1 = _mm_add_sd(sum, diff);
    __m128d r2 = _mm_add_sd(prod, quot);
    __m128d r3 = _mm_add_sd(vmin, vmax);
    __m128d r4 = _mm_add_sd(vsqrt, vround);
    
    __m128d result = _mm_add_sd(_mm_add_sd(r1, r2), _mm_add_sd(r3, r4));
    return _mm_cvtsd_f64(result);
}

/*============================================================================
 * SECTION 3: FMA OPERATIONS (All 132/213/231 variants)
 *===========================================================================*/

__attribute__((noinline))
void test_fma_ps(float *src1, float *src2, float *src3, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 a = _mm256_loadu_ps(&src1[i]);
        __m256 b = _mm256_loadu_ps(&src2[i]);
        __m256 c = _mm256_loadu_ps(&src3[i]);
        
        /* FMADD variants: a*b+c with different operand orderings */
        __m256 fmadd132 = _mm256_fmadd_ps(a, c, b);     /* a*c + b */
        __m256 fmadd213 = _mm256_fmadd_ps(b, a, c);     /* b*a + c */
        __m256 fmadd231 = _mm256_fmadd_ps(c, b, a);     /* c*b + a */
        
        /* FMSUB variants: a*b-c */
        __m256 fmsub132 = _mm256_fmsub_ps(a, c, b);     /* a*c - b */
        __m256 fmsub213 = _mm256_fmsub_ps(b, a, c);     /* b*a - c */
        __m256 fmsub231 = _mm256_fmsub_ps(c, b, a);     /* c*b - a */
        
        /* FNMADD variants: -(a*b)+c = c - a*b */
        __m256 fnmadd132 = _mm256_fnmadd_ps(a, c, b);   /* -(a*c) + b */
        __m256 fnmadd213 = _mm256_fnmadd_ps(b, a, c);   /* -(b*a) + c */
        __m256 fnmadd231 = _mm256_fnmadd_ps(c, b, a);   /* -(c*b) + a */
        
        /* FNMSUB variants: -(a*b)-c */
        __m256 fnmsub132 = _mm256_fnmsub_ps(a, c, b);   /* -(a*c) - b */
        __m256 fnmsub213 = _mm256_fnmsub_ps(b, a, c);   /* -(b*a) - c */
        __m256 fnmsub231 = _mm256_fnmsub_ps(c, b, a);   /* -(c*b) - a */
        
        /* FMADDSUB/FMSUBADD: alternating add/sub */
        __m256 fmaddsub = _mm256_fmaddsub_ps(a, b, c);  /* odd: a*b+c, even: a*b-c */
        __m256 fmsubadd = _mm256_fmsubadd_ps(a, b, c);  /* odd: a*b-c, even: a*b+c */
        
        /* Combine all FMA results */
        __m256 r1 = _mm256_add_ps(fmadd132, fmadd213);
        __m256 r2 = _mm256_add_ps(fmadd231, fmsub132);
        __m256 r3 = _mm256_add_ps(fmsub213, fmsub231);
        __m256 r4 = _mm256_add_ps(fnmadd132, fnmadd213);
        __m256 r5 = _mm256_add_ps(fnmadd231, fnmsub132);
        __m256 r6 = _mm256_add_ps(fnmsub213, fnmsub231);
        __m256 r7 = _mm256_add_ps(fmaddsub, fmsubadd);
        
        __m256 result = _mm256_add_ps(_mm256_add_ps(_mm256_add_ps(r1, r2), 
                                                     _mm256_add_ps(r3, r4)),
                                       _mm256_add_ps(_mm256_add_ps(r5, r6), r7));
        _mm256_storeu_ps(&dst[i], result);
    }
}

__attribute__((noinline))
void test_fma_pd(double *src1, double *src2, double *src3, double *dst, int n) {
    for (int i = 0; i < n; i += 4) {
        __m256d a = _mm256_loadu_pd(&src1[i]);
        __m256d b = _mm256_loadu_pd(&src2[i]);
        __m256d c = _mm256_loadu_pd(&src3[i]);
        
        /* All FMA variants for double */
        __m256d fmadd = _mm256_fmadd_pd(a, b, c);
        __m256d fmsub = _mm256_fmsub_pd(a, b, c);
        __m256d fnmadd = _mm256_fnmadd_pd(a, b, c);
        __m256d fnmsub = _mm256_fnmsub_pd(a, b, c);
        __m256d fmaddsub = _mm256_fmaddsub_pd(a, b, c);
        __m256d fmsubadd = _mm256_fmsubadd_pd(a, b, c);
        
        __m256d result = _mm256_add_pd(_mm256_add_pd(fmadd, fmsub),
                                        _mm256_add_pd(_mm256_add_pd(fnmadd, fnmsub),
                                                      _mm256_add_pd(fmaddsub, fmsubadd)));
        _mm256_storeu_pd(&dst[i], result);
    }
}

/* Scalar FMA */
__attribute__((noinline))
float test_fma_scalar_ss(float a, float b, float c) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_set_ss(c);
    
    __m128 fmadd = _mm_fmadd_ss(va, vb, vc);
    __m128 fmsub = _mm_fmsub_ss(va, vb, vc);
    __m128 fnmadd = _mm_fnmadd_ss(va, vb, vc);
    __m128 fnmsub = _mm_fnmsub_ss(va, vb, vc);
    
    __m128 result = _mm_add_ss(_mm_add_ss(fmadd, fmsub), _mm_add_ss(fnmadd, fnmsub));
    return _mm_cvtss_f32(result);
}

/*============================================================================
 * SECTION 4: AVX2 INTEGER ARITHMETIC
 *===========================================================================*/

__attribute__((noinline))
void test_avx2_integer_arithmetic(int32_t *src1, int32_t *src2, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* 32-bit integer arithmetic */
        __m256i add32 = _mm256_add_epi32(a, b);
        __m256i sub32 = _mm256_sub_epi32(a, b);
        __m256i mul32lo = _mm256_mullo_epi32(a, b);     /* Low 32 bits of 32x32 */
        __m256i mul32hi = _mm256_mulhi_epi16(_mm256_packs_epi32(a, a), 
                                              _mm256_packs_epi32(b, b)); /* Saturating pack then mulhi */
        
        /* Min/Max signed/unsigned */
        __m256i min32s = _mm256_min_epi32(a, b);
        __m256i max32s = _mm256_max_epi32(a, b);
        __m256i min32u = _mm256_min_epu32(a, b);
        __m256i max32u = _mm256_max_epu32(a, b);
        
        /* Absolute value and sign */
        __m256i abs32 = _mm256_abs_epi32(a);
        __m256i sign32 = _mm256_sign_epi32(a, b);
        
        /* Horizontal add */
        __m256i hadd32 = _mm256_hadd_epi32(a, b);
        __m256i hsub32 = _mm256_hsub_epi32(a, b);
        
        /* Combine results */
        __m256i r1 = _mm256_add_epi32(add32, sub32);
        __m256i r2 = _mm256_add_epi32(mul32lo, mul32hi);
        __m256i r3 = _mm256_add_epi32(min32s, max32s);
        __m256i r4 = _mm256_add_epi32(min32u, max32u);
        __m256i r5 = _mm256_add_epi32(abs32, sign32);
        __m256i r6 = _mm256_add_epi32(hadd32, hsub32);
        
        __m256i result = _mm256_add_epi32(_mm256_add_epi32(r1, r2),
                                          _mm256_add_epi32(_mm256_add_epi32(r3, r4),
                                                           _mm256_add_epi32(r5, r6)));
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

__attribute__((noinline))
void test_avx2_integer_8_16(int16_t *src1, int16_t *src2, int16_t *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* 16-bit operations */
        __m256i add16 = _mm256_add_epi16(a, b);
        __m256i sub16 = _mm256_sub_epi16(a, b);
        __m256i mullo16 = _mm256_mullo_epi16(a, b);
        __m256i mulhi16 = _mm256_mulhi_epi16(a, b);
        __m256i mulhiu16 = _mm256_mulhi_epu16(a, b);
        
        /* Saturating arithmetic */
        __m256i adds16 = _mm256_adds_epi16(a, b);
        __m256i subs16 = _mm256_subs_epi16(a, b);
        __m256i addsu16 = _mm256_adds_epu16(a, b);
        __m256i subsu16 = _mm256_subs_epu16(a, b);
        
        /* Average */
        __m256i avg16 = _mm256_avg_epu16(a, b);
        
        /* Min/Max */
        __m256i min16s = _mm256_min_epi16(a, b);
        __m256i max16s = _mm256_max_epi16(a, b);
        __m256i min16u = _mm256_min_epu16(a, b);
        __m256i max16u = _mm256_max_epu16(a, b);
        
        /* Horizontal add with saturation */
        __m256i hadds16 = _mm256_hadds_epi16(a, b);
        __m256i hsubs16 = _mm256_hsubs_epi16(a, b);
        
        /* MADD: multiply and add adjacent pairs */
        __m256i madd = _mm256_madd_epi16(a, b);
        
        /* MADDUBS: multiply unsigned byte by signed byte and add */
        __m256i maddubs = _mm256_maddubs_epi16(a, b);
        
        /* Combine */
        __m256i r1 = _mm256_add_epi16(add16, sub16);
        __m256i r2 = _mm256_add_epi16(mullo16, mulhi16);
        __m256i r3 = _mm256_add_epi16(adds16, subs16);
        __m256i r4 = _mm256_add_epi16(avg16, min16s);
        __m256i r5 = _mm256_add_epi16(max16s, hadds16);
        __m256i r6 = _mm256_add_epi16(hsubs16, _mm256_packs_epi32(madd, maddubs));
        
        __m256i result = _mm256_add_epi16(_mm256_add_epi16(r1, r2),
                                          _mm256_add_epi16(_mm256_add_epi16(r3, r4),
                                                           _mm256_add_epi16(r5, r6)));
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

__attribute__((noinline))
void test_avx2_integer_64(int64_t *src1, int64_t *src2, int64_t *dst, int n) {
    for (int i = 0; i < n; i += 4) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* 64-bit operations (limited in AVX2) */
        __m256i add64 = _mm256_add_epi64(a, b);
        __m256i sub64 = _mm256_sub_epi64(a, b);
        
        /* 64-bit compare (GT only in AVX2) */
        __m256i gt64 = _mm256_cmpgt_epi64(a, b);
        
        /* Use comparison as mask for blend */
        __m256i blended = _mm256_blendv_epi8(a, b, gt64);
        
        /* Combine */
        __m256i result = _mm256_add_epi64(_mm256_add_epi64(add64, sub64), blended);
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

/*============================================================================
 * SECTION 5: LOGICAL OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_logical_ops_ps(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 a = _mm256_loadu_ps(&src1[i]);
        __m256 b = _mm256_loadu_ps(&src2[i]);
        
        /* Floating-point logical (operate on bit patterns) */
        __m256 vand = _mm256_and_ps(a, b);
        __m256 vor = _mm256_or_ps(a, b);
        __m256 vxor = _mm256_xor_ps(a, b);
        __m256 vandn = _mm256_andnot_ps(a, b);  /* ~a & b */
        
        /* Use XOR to flip signs, AND/OR for masking */
        __m256 sign_mask = _mm256_set1_ps(-0.0f);
        __m256 abs_a = _mm256_andnot_ps(sign_mask, a);  /* Clear sign bit */
        __m256 neg_b = _mm256_xor_ps(b, sign_mask);     /* Flip sign bit */
        
        /* Combine */
        __m256 result = _mm256_add_ps(_mm256_add_ps(vand, vor),
                                       _mm256_add_ps(_mm256_add_ps(vxor, vandn),
                                                     _mm256_add_ps(abs_a, neg_b)));
        _mm256_storeu_ps(&dst[i], result);
    }
}

__attribute__((noinline))
void test_logical_ops_int(int32_t *src1, int32_t *src2, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* Integer logical */
        __m256i vand = _mm256_and_si256(a, b);
        __m256i vor = _mm256_or_si256(a, b);
        __m256i vxor = _mm256_xor_si256(a, b);
        __m256i vandn = _mm256_andnot_si256(a, b);
        
        /* Test operations (returns mask in integer) */
        int testz = _mm256_testz_si256(a, b);      /* ZF = (a & b) == 0 */
        int testc = _mm256_testc_si256(a, b);      /* CF = (~a & b) == 0 */
        int testnzc = _mm256_testnzc_si256(a, b);  /* Neither all zeros nor all ones */
        
        /* Use test results to modify behavior */
        __m256i modifier = _mm256_set1_epi32(testz + testc * 2 + testnzc * 4);
        
        /* Combine */
        __m256i result = _mm256_add_epi32(_mm256_add_epi32(vand, vor),
                                          _mm256_add_epi32(_mm256_add_epi32(vxor, vandn), modifier));
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

/*============================================================================
 * SECTION 6: COMPARISON OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_compare_ps(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 a = _mm256_loadu_ps(&src1[i]);
        __m256 b = _mm256_loadu_ps(&src2[i]);
        
        /* All comparison predicates */
        __m256 cmp_eq = _mm256_cmp_ps(a, b, _CMP_EQ_OQ);      /* Equal, ordered, quiet */
        __m256 cmp_lt = _mm256_cmp_ps(a, b, _CMP_LT_OQ);      /* Less than */
        __m256 cmp_le = _mm256_cmp_ps(a, b, _CMP_LE_OQ);      /* Less or equal */
        __m256 cmp_gt = _mm256_cmp_ps(a, b, _CMP_GT_OQ);      /* Greater than (reversed) */
        __m256 cmp_ge = _mm256_cmp_ps(a, b, _CMP_GE_OQ);      /* Greater or equal */
        __m256 cmp_neq = _mm256_cmp_ps(a, b, _CMP_NEQ_OQ);    /* Not equal */
        __m256 cmp_ord = _mm256_cmp_ps(a, b, _CMP_ORD_Q);     /* Ordered (neither is NaN) */
        __m256 cmp_unord = _mm256_cmp_ps(a, b, _CMP_UNORD_Q); /* Unordered (at least one NaN) */
        
        /* Signaling variants */
        __m256 cmp_eq_s = _mm256_cmp_ps(a, b, _CMP_EQ_OS);    /* Equal, ordered, signaling */
        __m256 cmp_lt_s = _mm256_cmp_ps(a, b, _CMP_LT_OS);
        
        /* Use comparisons to blend */
        __m256 selected1 = _mm256_blendv_ps(a, b, cmp_lt);
        __m256 selected2 = _mm256_blendv_ps(b, a, cmp_gt);
        
        /* Movemask to extract comparison results */
        int mask_eq = _mm256_movemask_ps(cmp_eq);
        int mask_lt = _mm256_movemask_ps(cmp_lt);
        
        /* Combine (sum all mask bits as scalar modifier) */
        __m256 modifier = _mm256_set1_ps((float)(mask_eq + mask_lt));
        
        __m256 r1 = _mm256_and_ps(cmp_eq, a);
        __m256 r2 = _mm256_and_ps(cmp_neq, b);
        __m256 r3 = _mm256_add_ps(selected1, selected2);
        
        __m256 result = _mm256_add_ps(_mm256_add_ps(r1, r2), _mm256_add_ps(r3, modifier));
        _mm256_storeu_ps(&dst[i], result);
    }
}

__attribute__((noinline))
void test_compare_int(int32_t *src1, int32_t *src2, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* Integer comparisons */
        __m256i cmpeq8 = _mm256_cmpeq_epi8(a, b);
        __m256i cmpeq16 = _mm256_cmpeq_epi16(a, b);
        __m256i cmpeq32 = _mm256_cmpeq_epi32(a, b);
        __m256i cmpeq64 = _mm256_cmpeq_epi64(a, b);
        
        __m256i cmpgt8 = _mm256_cmpgt_epi8(a, b);
        __m256i cmpgt16 = _mm256_cmpgt_epi16(a, b);
        __m256i cmpgt32 = _mm256_cmpgt_epi32(a, b);
        __m256i cmpgt64 = _mm256_cmpgt_epi64(a, b);
        
        /* Extract masks */
        int mask8 = _mm256_movemask_epi8(cmpeq8);
        
        /* Use comparisons for conditional operations */
        __m256i max_via_cmp = _mm256_blendv_epi8(b, a, cmpgt8);
        __m256i min_via_cmp = _mm256_blendv_epi8(a, b, cmpgt8);
        
        /* Combine */
        __m256i r1 = _mm256_and_si256(cmpeq32, a);
        __m256i r2 = _mm256_and_si256(cmpgt32, b);
        __m256i r3 = _mm256_add_epi32(max_via_cmp, min_via_cmp);
        __m256i modifier = _mm256_set1_epi32(mask8);
        
        __m256i result = _mm256_add_epi32(_mm256_add_epi32(r1, r2), 
                                          _mm256_add_epi32(r3, modifier));
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

/*============================================================================
 * SECTION 7: SHUFFLE/PERMUTE/BLEND OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_shuffle_ps(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 a = _mm256_loadu_ps(&src1[i]);
        __m256 b = _mm256_loadu_ps(&src2[i]);
        
        /* Shuffle within 128-bit lanes */
        __m256 shuf1 = _mm256_shuffle_ps(a, b, _MM_SHUFFLE(3, 1, 2, 0));
        __m256 shuf2 = _mm256_shuffle_ps(a, b, _MM_SHUFFLE(0, 2, 1, 3));
        
        /* Permute within lanes */
        __m256 perm1 = _mm256_permute_ps(a, _MM_SHUFFLE(2, 3, 0, 1));
        __m256 perm2 = _mm256_permute_ps(b, _MM_SHUFFLE(1, 0, 3, 2));
        
        /* Permute across lanes (AVX2) */
        __m256 perm2f = _mm256_permutevar8x32_ps(a, _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0));
        
        /* Blend with immediate mask */
        __m256 blend1 = _mm256_blend_ps(a, b, 0b10101010);  /* Alternating */
        __m256 blend2 = _mm256_blend_ps(a, b, 0b11110000);  /* Upper half from b */
        
        /* Blend with variable mask */
        __m256 mask = _mm256_cmp_ps(a, b, _CMP_GT_OQ);
        __m256 blendv = _mm256_blendv_ps(a, b, mask);
        
        /* Unpack */
        __m256 unpackhi = _mm256_unpackhi_ps(a, b);
        __m256 unpacklo = _mm256_unpacklo_ps(a, b);
        
        /* Move high/low duplicate */
        __m256 movehdup = _mm256_movehdup_ps(a);
        __m256 moveldup = _mm256_moveldup_ps(b);
        
        /* Permute 128-bit lanes */
        __m256 perm128 = _mm256_permute2f128_ps(a, b, 0x21);  /* Swap lanes */
        
        /* Combine all */
        __m256 r1 = _mm256_add_ps(shuf1, shuf2);
        __m256 r2 = _mm256_add_ps(perm1, perm2);
        __m256 r3 = _mm256_add_ps(perm2f, blend1);
        __m256 r4 = _mm256_add_ps(blend2, blendv);
        __m256 r5 = _mm256_add_ps(unpackhi, unpacklo);
        __m256 r6 = _mm256_add_ps(movehdup, moveldup);
        __m256 r7 = perm128;
        
        __m256 result = _mm256_add_ps(_mm256_add_ps(_mm256_add_ps(r1, r2), 
                                                     _mm256_add_ps(r3, r4)),
                                       _mm256_add_ps(_mm256_add_ps(r5, r6), r7));
        _mm256_storeu_ps(&dst[i], result);
    }
}

__attribute__((noinline))
void test_shuffle_int(int32_t *src1, int32_t *src2, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* Shuffle dwords */
        __m256i pshufd1 = _mm256_shuffle_epi32(a, _MM_SHUFFLE(0, 1, 2, 3));
        __m256i pshufd2 = _mm256_shuffle_epi32(b, _MM_SHUFFLE(2, 3, 0, 1));
        
        /* Shuffle bytes (AVX2) */
        __m256i shuf_ctrl = _mm256_setr_epi8(
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
        );
        __m256i pshufb = _mm256_shuffle_epi8(a, shuf_ctrl);
        
        /* Shuffle high/low words */
        __m256i pshufhw = _mm256_shufflehi_epi16(a, _MM_SHUFFLE(0, 1, 2, 3));
        __m256i pshuflw = _mm256_shufflelo_epi16(b, _MM_SHUFFLE(3, 2, 1, 0));
        
        /* Permute qwords */
        __m256i permq = _mm256_permute4x64_epi64(a, _MM_SHUFFLE(0, 1, 2, 3));
        
        /* Permute dwords across lanes */
        __m256i permd = _mm256_permutevar8x32_epi32(a, _mm256_setr_epi32(7, 6, 5, 4, 3, 2, 1, 0));
        
        /* Blend with immediate */
        __m256i blendd = _mm256_blend_epi32(a, b, 0b10101010);
        __m256i blendw = _mm256_blend_epi16(a, b, 0b10101010);
        
        /* Blend with variable mask */
        __m256i mask = _mm256_cmpgt_epi32(a, b);
        __m256i blendv = _mm256_blendv_epi8(a, b, mask);
        
        /* Unpack */
        __m256i unpackhi32 = _mm256_unpackhi_epi32(a, b);
        __m256i unpacklo32 = _mm256_unpacklo_epi32(a, b);
        __m256i unpackhi64 = _mm256_unpackhi_epi64(a, b);
        __m256i unpacklo64 = _mm256_unpacklo_epi64(a, b);
        
        /* Permute 128-bit lanes */
        __m256i perm128 = _mm256_permute2x128_si256(a, b, 0x21);
        
        /* Combine */
        __m256i r1 = _mm256_add_epi32(pshufd1, pshufd2);
        __m256i r2 = _mm256_add_epi32(pshufb, pshufhw);
        __m256i r3 = _mm256_add_epi32(pshuflw, permq);
        __m256i r4 = _mm256_add_epi32(permd, blendd);
        __m256i r5 = _mm256_add_epi32(blendv, unpackhi32);
        __m256i r6 = _mm256_add_epi32(unpacklo32, unpackhi64);
        __m256i r7 = _mm256_add_epi32(unpacklo64, perm128);
        
        __m256i result = _mm256_add_epi32(_mm256_add_epi32(_mm256_add_epi32(r1, r2),
                                                           _mm256_add_epi32(r3, r4)),
                                          _mm256_add_epi32(_mm256_add_epi32(r5, r6), r7));
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

/*============================================================================
 * SECTION 8: BROADCAST OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_broadcast(float *src, double *srcd, int32_t *srci, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        /* Broadcast single float */
        __m256 bcast_ss = _mm256_broadcast_ss(&src[i]);
        
        /* Broadcast 128-bit to both lanes */
        __m128 xmm = _mm_loadu_ps(&src[i]);
        __m256 bcast_128 = _mm256_broadcast_ps(&xmm);
        
        /* Broadcast double */
        __m256d bcast_sd = _mm256_broadcast_sd(&srcd[i >> 1]);
        
        /* Set/broadcast using set1 */
        __m256 set1_ps = _mm256_set1_ps(src[i]);
        __m256d set1_pd = _mm256_set1_pd(srcd[i >> 1]);
        __m256i set1_32 = _mm256_set1_epi32(srci[i]);
        __m256i set1_64 = _mm256_set1_epi64x(srci[i]);
        __m256i set1_16 = _mm256_set1_epi16((int16_t)srci[i]);
        __m256i set1_8 = _mm256_set1_epi8((int8_t)srci[i]);
        
        /* AVX2: Broadcast from integer memory */
        __m256i bcast_d = _mm256_broadcastd_epi32(_mm_loadu_si128((__m128i*)&srci[i]));
        __m256i bcast_q = _mm256_broadcastq_epi64(_mm_loadu_si128((__m128i*)&srci[i]));
        __m256i bcast_w = _mm256_broadcastw_epi16(_mm_loadu_si128((__m128i*)&srci[i]));
        __m256i bcast_b = _mm256_broadcastb_epi8(_mm_loadu_si128((__m128i*)&srci[i]));
        
        /* Broadcast 128-bit integer lane */
        __m256i bcast_i128 = _mm256_broadcastsi128_si256(_mm_loadu_si128((__m128i*)&srci[i]));
        
        /* Combine float results */
        __m256 r1 = _mm256_add_ps(bcast_ss, bcast_128);
        __m256 r2 = _mm256_add_ps(set1_ps, _mm256_castpd_ps(bcast_sd));
        __m256 r3 = _mm256_castsi256_ps(_mm256_add_epi32(set1_32, bcast_d));
        
        __m256 result = _mm256_add_ps(r1, _mm256_add_ps(r2, r3));
        _mm256_storeu_ps(&dst[i], result);
        
        /* Force use of other broadcasts */
        g_int_accumulator += _mm256_extract_epi32(bcast_q, 0);
        g_int_accumulator += _mm256_extract_epi16(bcast_w, 0);
        g_int_accumulator += _mm256_extract_epi8(bcast_b, 0);
    }
}

/*============================================================================
 * SECTION 9: GATHER OPERATIONS (AVX2)
 *===========================================================================*/

__attribute__((noinline))
void test_gather(float *base_ps, double *base_pd, int32_t *base_i32, 
                 int32_t *indices, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256i idx = _mm256_loadu_si256((__m256i*)&indices[i]);
        __m128i idx128 = _mm_loadu_si128((__m128i*)&indices[i]);
        
        /* Gather floats with 32-bit indices */
        __m256 gather_ps = _mm256_i32gather_ps(base_ps, idx, 4);
        
        /* Gather doubles with 32-bit indices (4 doubles) */
        __m256d gather_pd = _mm256_i32gather_pd(base_pd, idx128, 8);
        
        /* Gather 32-bit integers */
        __m256i gather_d = _mm256_i32gather_epi32(base_i32, idx, 4);
        
        /* Gather 64-bit integers with 32-bit indices */
        __m256i gather_q = _mm256_i32gather_epi64((const long long*)base_i32, idx128, 4);
        
        /* Masked gather */
        __m256 mask_ps = _mm256_cmp_ps(_mm256_loadu_ps(&base_ps[i]), 
                                        _mm256_setzero_ps(), _CMP_GT_OQ);
        __m256 src_ps = _mm256_loadu_ps(&base_ps[i]);
        __m256 gather_mask = _mm256_mask_i32gather_ps(src_ps, base_ps, idx, mask_ps, 4);
        
        /* Combine float results */
        __m256 r1 = _mm256_add_ps(gather_ps, _mm256_castpd_ps(gather_pd));
        __m256 r2 = _mm256_add_ps(_mm256_castsi256_ps(gather_d), gather_mask);
        
        __m256 result = _mm256_add_ps(r1, r2);
        _mm256_storeu_ps(&dst[i], result);
        
        /* Force use of 64-bit gather */
        g_int_accumulator += _mm256_extract_epi64(gather_q, 0);
    }
}

/*============================================================================
 * SECTION 10: SHIFT OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_shifts(int32_t *src1, int32_t *src2, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        __m128i shift_amt = _mm_set1_epi64x(4);  /* Shift by 4 bits */
        
        /* Immediate shifts - 16-bit */
        __m256i slli16 = _mm256_slli_epi16(a, 3);
        __m256i srli16 = _mm256_srli_epi16(a, 3);
        __m256i srai16 = _mm256_srai_epi16(a, 3);
        
        /* Immediate shifts - 32-bit */
        __m256i slli32 = _mm256_slli_epi32(a, 5);
        __m256i srli32 = _mm256_srli_epi32(a, 5);
        __m256i srai32 = _mm256_srai_epi32(a, 5);
        
        /* Immediate shifts - 64-bit */
        __m256i slli64 = _mm256_slli_epi64(a, 7);
        __m256i srli64 = _mm256_srli_epi64(a, 7);
        /* Note: No srai_epi64 in AVX2 */
        
        /* Variable shifts (same shift for all elements) */
        __m256i sll16 = _mm256_sll_epi16(a, shift_amt);
        __m256i srl16 = _mm256_srl_epi16(a, shift_amt);
        __m256i sra16 = _mm256_sra_epi16(a, shift_amt);
        
        __m256i sll32 = _mm256_sll_epi32(a, shift_amt);
        __m256i srl32 = _mm256_srl_epi32(a, shift_amt);
        __m256i sra32 = _mm256_sra_epi32(a, shift_amt);
        
        __m256i sll64 = _mm256_sll_epi64(a, shift_amt);
        __m256i srl64 = _mm256_srl_epi64(a, shift_amt);
        
        /* Variable shifts (per-element, AVX2) */
        __m256i sllv32 = _mm256_sllv_epi32(a, _mm256_and_si256(b, _mm256_set1_epi32(31)));
        __m256i srlv32 = _mm256_srlv_epi32(a, _mm256_and_si256(b, _mm256_set1_epi32(31)));
        __m256i srav32 = _mm256_srav_epi32(a, _mm256_and_si256(b, _mm256_set1_epi32(31)));
        
        __m256i sllv64 = _mm256_sllv_epi64(a, _mm256_and_si256(b, _mm256_set1_epi64x(63)));
        __m256i srlv64 = _mm256_srlv_epi64(a, _mm256_and_si256(b, _mm256_set1_epi64x(63)));
        
        /* Byte shifts (shift entire 128-bit lane) */
        __m256i bslli = _mm256_bslli_epi128(a, 4);  /* Shift left by 4 bytes */
        __m256i bsrli = _mm256_bsrli_epi128(a, 4);  /* Shift right by 4 bytes */
        
        /* Combine all results */
        __m256i r1 = _mm256_add_epi32(slli16, srli16);
        __m256i r2 = _mm256_add_epi32(srai16, slli32);
        __m256i r3 = _mm256_add_epi32(srli32, srai32);
        __m256i r4 = _mm256_add_epi32(slli64, srli64);
        __m256i r5 = _mm256_add_epi32(sllv32, srlv32);
        __m256i r6 = _mm256_add_epi32(srav32, sllv64);
        __m256i r7 = _mm256_add_epi32(srlv64, bslli);
        __m256i r8 = bsrli;
        
        __m256i result = _mm256_add_epi32(
            _mm256_add_epi32(_mm256_add_epi32(r1, r2), _mm256_add_epi32(r3, r4)),
            _mm256_add_epi32(_mm256_add_epi32(r5, r6), _mm256_add_epi32(r7, r8)));
        
        _mm256_storeu_si256((__m256i*)&dst[i], result);
    }
}

/*============================================================================
 * SECTION 11: INSERT/EXTRACT OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_insert_extract(float *src_ps, int32_t *src_i, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 a = _mm256_loadu_ps(&src_ps[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src_i[i]);
        __m128 xmm = _mm_loadu_ps(&src_ps[i]);
        __m128i xmmi = _mm_loadu_si128((__m128i*)&src_i[i]);
        
        /* Extract 128-bit lane */
        __m128 lo_ps = _mm256_extractf128_ps(a, 0);
        __m128 hi_ps = _mm256_extractf128_ps(a, 1);
        __m128i lo_i = _mm256_extracti128_si256(b, 0);
        __m128i hi_i = _mm256_extracti128_si256(b, 1);
        
        /* Insert 128-bit lane */
        __m256 inserted_ps = _mm256_insertf128_ps(a, hi_ps, 0);  /* Put high in low */
        __m256i inserted_i = _mm256_inserti128_si256(b, hi_i, 0);
        
        /* Extract individual elements (AVX2) */
        int32_t e0 = _mm256_extract_epi32(b, 0);
        int32_t e3 = _mm256_extract_epi32(b, 3);
        int32_t e7 = _mm256_extract_epi32(b, 7);
        int64_t q0 = _mm256_extract_epi64(b, 0);
        int16_t w5 = _mm256_extract_epi16(b, 5);
        int8_t b10 = _mm256_extract_epi8(b, 10);
        
        /* Insert individual elements */
        __m256i inserted_d = _mm256_insert_epi32(b, 0x12345678, 4);
        __m256i inserted_q = _mm256_insert_epi64(b, 0xDEADBEEFCAFEBABELL, 2);
        __m256i inserted_w = _mm256_insert_epi16(b, 0xABCD, 7);
        __m256i inserted_b = _mm256_insert_epi8(b, 0x42, 15);
        
        /* Combine extracted lanes */
        __m128 sum_lanes = _mm_add_ps(lo_ps, hi_ps);
        __m256 expanded = _mm256_castps128_ps256(sum_lanes);
        expanded = _mm256_insertf128_ps(expanded, sum_lanes, 1);
        
        /* Combine */
        __m256 r1 = _mm256_add_ps(inserted_ps, expanded);
        __m256 r2 = _mm256_castsi256_ps(_mm256_add_epi32(inserted_i, inserted_d));
        __m256 modifier = _mm256_set1_ps((float)(e0 + e3 + e7));
        
        __m256 result = _mm256_add_ps(r1, _mm256_add_ps(r2, modifier));
        _mm256_storeu_ps(&dst[i], result);
        
        /* Force use of other extracts/inserts */
        g_int_accumulator += q0 + w5 + b10;
        g_int_accumulator += _mm256_extract_epi32(inserted_q, 0);
        g_int_accumulator += _mm256_extract_epi16(inserted_w, 0);
        g_int_accumulator += _mm256_extract_epi8(inserted_b, 0);
    }
}

/*============================================================================
 * SECTION 12: CONVERSION OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_conversions(float *src_ps, double *src_pd, int32_t *src_i, 
                      float *dst_ps, double *dst_pd, int32_t *dst_i, int n) {
    for (int i = 0; i < n; i += 8) {
        __m256 ps = _mm256_loadu_ps(&src_ps[i]);
        __m256d pd = _mm256_loadu_pd(&src_pd[i >> 1]);
        __m256i i32 = _mm256_loadu_si256((__m256i*)&src_i[i]);
        __m128 ps128 = _mm_loadu_ps(&src_ps[i]);
        __m128i i32_128 = _mm_loadu_si128((__m128i*)&src_i[i]);
        
        /* Float <-> Double */
        __m128 pd_to_ps = _mm256_cvtpd_ps(pd);              /* 4 double -> 4 float */
        __m256d ps_to_pd = _mm256_cvtps_pd(ps128);          /* 4 float -> 4 double */
        
        /* Float <-> Int32 */
        __m256i ps_to_i32 = _mm256_cvtps_epi32(ps);         /* 8 float -> 8 int32 (round) */
        __m256i ps_to_i32_t = _mm256_cvttps_epi32(ps);      /* 8 float -> 8 int32 (truncate) */
        __m256 i32_to_ps = _mm256_cvtepi32_ps(i32);         /* 8 int32 -> 8 float */
        
        /* Double <-> Int32 */
        __m128i pd_to_i32 = _mm256_cvtpd_epi32(pd);         /* 4 double -> 4 int32 (round) */
        __m128i pd_to_i32_t = _mm256_cvttpd_epi32(pd);      /* 4 double -> 4 int32 (truncate) */
        __m256d i32_to_pd = _mm256_cvtepi32_pd(i32_128);    /* 4 int32 -> 4 double */
        
        /* Sign/zero extension (AVX2) */
        __m256i ext_8_32 = _mm256_cvtepi8_epi32(_mm_loadu_si128((__m128i*)src_i));
        __m256i ext_8_64 = _mm256_cvtepi8_epi64(_mm_loadu_si128((__m128i*)src_i));
        __m256i ext_16_32 = _mm256_cvtepi16_epi32(_mm_loadu_si128((__m128i*)src_i));
        __m256i ext_16_64 = _mm256_cvtepi16_epi64(_mm_loadu_si128((__m128i*)src_i));
        __m256i ext_32_64 = _mm256_cvtepi32_epi64(_mm_loadu_si128((__m128i*)src_i));
        
        /* Unsigned extension */
        __m256i extu_8_32 = _mm256_cvtepu8_epi32(_mm_loadu_si128((__m128i*)src_i));
        __m256i extu_8_64 = _mm256_cvtepu8_epi64(_mm_loadu_si128((__m128i*)src_i));
        __m256i extu_16_32 = _mm256_cvtepu16_epi32(_mm_loadu_si128((__m128i*)src_i));
        __m256i extu_16_64 = _mm256_cvtepu16_epi64(_mm_loadu_si128((__m128i*)src_i));
        __m256i extu_32_64 = _mm256_cvtepu32_epi64(_mm_loadu_si128((__m128i*)src_i));
        
        /* Pack operations */
        __m256i pack_i32_i16 = _mm256_packs_epi32(i32, i32);     /* Signed saturate 32->16 */
        __m256i pack_i16_i8 = _mm256_packs_epi16(i32, i32);      /* Signed saturate 16->8 */
        __m256i packu_i32_i16 = _mm256_packus_epi32(i32, i32);   /* Unsigned saturate 32->16 */
        __m256i packu_i16_i8 = _mm256_packus_epi16(i32, i32);    /* Unsigned saturate 16->8 */
        
        /* Store conversions */
        _mm256_storeu_ps(&dst_ps[i], i32_to_ps);
        _mm256_storeu_pd(&dst_pd[i >> 1], i32_to_pd);
        _mm256_storeu_si256((__m256i*)&dst_i[i], ps_to_i32);
        
        /* Accumulate for side effects */
        g_int_accumulator += _mm256_extract_epi32(ext_8_32, 0);
        g_int_accumulator += _mm256_extract_epi64(ext_8_64, 0);
        g_int_accumulator += _mm256_extract_epi32(extu_8_32, 0);
        g_int_accumulator += _mm256_extract_epi32(pack_i32_i16, 0);
    }
}

/*============================================================================
 * SECTION 13: HORIZONTAL OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
float test_horizontal_sum_ps(__m256 v) {
    /* Horizontal sum of 8 floats */
    __m256 hadd1 = _mm256_hadd_ps(v, v);        /* [a0+a1, a2+a3, a0+a1, a2+a3, a4+a5, a6+a7, a4+a5, a6+a7] */
    __m256 hadd2 = _mm256_hadd_ps(hadd1, hadd1);
    __m128 lo = _mm256_extractf128_ps(hadd2, 0);
    __m128 hi = _mm256_extractf128_ps(hadd2, 1);
    __m128 sum = _mm_add_ps(lo, hi);
    return _mm_cvtss_f32(sum);
}

__attribute__((noinline))
double test_horizontal_sum_pd(__m256d v) {
    /* Horizontal sum of 4 doubles */
    __m256d hadd = _mm256_hadd_pd(v, v);        /* [a0+a1, a0+a1, a2+a3, a2+a3] */
    __m128d lo = _mm256_extractf128_pd(hadd, 0);
    __m128d hi = _mm256_extractf128_pd(hadd, 1);
    __m128d sum = _mm_add_pd(lo, hi);
    return _mm_cvtsd_f64(sum);
}

__attribute__((noinline))
int32_t test_horizontal_sum_epi32(__m256i v) {
    /* Horizontal sum of 8 int32s */
    __m256i hadd1 = _mm256_hadd_epi32(v, v);
    __m256i hadd2 = _mm256_hadd_epi32(hadd1, hadd1);
    __m128i lo = _mm256_extracti128_si256(hadd2, 0);
    __m128i hi = _mm256_extracti128_si256(hadd2, 1);
    __m128i sum = _mm_add_epi32(lo, hi);
    return _mm_cvtsi128_si32(sum);
}

/*============================================================================
 * SECTION 14: SAD (Sum of Absolute Differences)
 *===========================================================================*/

__attribute__((noinline))
void test_sad(uint8_t *src1, uint8_t *src2, int64_t *dst, int n) {
    for (int i = 0; i < n; i += 32) {
        __m256i a = _mm256_loadu_si256((__m256i*)&src1[i]);
        __m256i b = _mm256_loadu_si256((__m256i*)&src2[i]);
        
        /* SAD: sum of absolute differences of unsigned bytes, accumulated into 64-bit */
        __m256i sad = _mm256_sad_epu8(a, b);
        
        /* MPSADBW: multiple packed sums of absolute differences */
        __m256i mpsad = _mm256_mpsadbw_epu8(a, b, 0x00);
        
        /* Store SAD result */
        _mm256_storeu_si256((__m256i*)&dst[i >> 3], sad);
        
        /* Force use of mpsadbw */
        g_int_accumulator += _mm256_extract_epi16(mpsad, 0);
    }
}

/*============================================================================
 * SECTION 15: MOVEMASK AND ZEROUPPER
 *===========================================================================*/

__attribute__((noinline))
int test_movemask(float *src_ps, double *src_pd, int8_t *src_i8, int n) {
    int result = 0;
    
    for (int i = 0; i < n; i += 8) {
        __m256 ps = _mm256_loadu_ps(&src_ps[i]);
        __m256d pd = _mm256_loadu_pd(&src_pd[i >> 1]);
        __m256i i8 = _mm256_loadu_si256((__m256i*)&src_i8[i * 4]);
        
        /* Extract sign bits */
        int mask_ps = _mm256_movemask_ps(ps);   /* 8 bits from 8 floats */
        int mask_pd = _mm256_movemask_pd(pd);   /* 4 bits from 4 doubles */
        int mask_epi8 = _mm256_movemask_epi8(i8); /* 32 bits from 32 bytes */
        
        result += mask_ps + mask_pd + mask_epi8;
    }
    
    /* vzeroupper: Clear upper 128 bits of all YMM registers */
    /* Important for AVX-SSE transition penalty avoidance */
    _mm256_zeroupper();
    
    return result;
}

/*============================================================================
 * SECTION 16: SPECIAL MOVE OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_special_moves(float *src, float *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        /* Aligned loads/stores */
        __m256 aligned_load = _mm256_load_ps(&src[i]);  /* Requires 32-byte alignment */
        
        /* Streaming stores (non-temporal, bypass cache) */
        _mm256_stream_ps(&dst[i], aligned_load);
        
        /* Masked load/store (AVX) */
        __m256i mask = _mm256_setr_epi32(-1, 0, -1, 0, -1, 0, -1, 0);
        __m256 masked_load = _mm256_maskload_ps(&src[i], mask);
        _mm256_maskstore_ps(&dst[i], mask, masked_load);
        
        /* Move duplicates */
        __m256 movehdup = _mm256_movehdup_ps(aligned_load);
        __m256 moveldup = _mm256_moveldup_ps(aligned_load);
        
        /* Move with zeroing upper */
        __m128 lo = _mm256_castps256_ps128(aligned_load);
        __m256 zero_extended = _mm256_castps128_ps256(lo);
        
        /* Store with fence */
        _mm_sfence();
    }
}

/*============================================================================
 * SECTION 17: AVX-512 ARITHMETIC (512-bit, float/double)
 *===========================================================================*/

#ifdef __AVX512F__

__attribute__((noinline))
void test_avx512_arithmetic_ps(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 a = _mm512_loadu_ps(&src1[i]);
        __m512 b = _mm512_loadu_ps(&src2[i]);
        
        /* Basic arithmetic */
        __m512 sum = _mm512_add_ps(a, b);
        __m512 diff = _mm512_sub_ps(a, b);
        __m512 prod = _mm512_mul_ps(a, b);
        __m512 quot = _mm512_div_ps(a, _mm512_max_ps(b, _mm512_set1_ps(0.0001f)));
        
        /* Min/Max */
        __m512 vmin = _mm512_min_ps(a, b);
        __m512 vmax = _mm512_max_ps(a, b);
        
        /* Special functions */
        __m512 vsqrt = _mm512_sqrt_ps(_mm512_max_ps(a, _mm512_setzero_ps()));
        __m512 vrsqrt = _mm512_rsqrt14_ps(_mm512_max_ps(b, _mm512_set1_ps(0.0001f)));
        __m512 vrcp = _mm512_rcp14_ps(_mm512_max_ps(b, _mm512_set1_ps(0.0001f)));
        
        /* Rounding */
        __m512 vround = _mm512_roundscale_ps(sum, _MM_FROUND_TO_NEAREST_INT);
        __m512 vfloor = _mm512_floor_ps(diff);
        __m512 vceil = _mm512_ceil_ps(prod);
        
        /* FMA */
        __m512 vfmadd = _mm512_fmadd_ps(a, b, sum);
        __m512 vfmsub = _mm512_fmsub_ps(a, b, diff);
        __m512 vfnmadd = _mm512_fnmadd_ps(a, b, prod);
        __m512 vfnmsub = _mm512_fnmsub_ps(a, b, quot);
        
        /* Absolute value */
        __m512 vabs = _mm512_abs_ps(diff);
        
        /* Combine all results */
        __m512 r1 = _mm512_add_ps(sum, diff);
        __m512 r2 = _mm512_add_ps(prod, quot);
        __m512 r3 = _mm512_add_ps(vmin, vmax);
        __m512 r4 = _mm512_add_ps(vsqrt, vrsqrt);
        __m512 r5 = _mm512_add_ps(vrcp, vround);
        __m512 r6 = _mm512_add_ps(vfloor, vceil);
        __m512 r7 = _mm512_add_ps(vfmadd, vfmsub);
        __m512 r8 = _mm512_add_ps(vfnmadd, vfnmsub);
        __m512 r9 = vabs;
        
        __m512 result = _mm512_add_ps(
            _mm512_add_ps(_mm512_add_ps(r1, r2), _mm512_add_ps(r3, r4)),
            _mm512_add_ps(_mm512_add_ps(r5, r6), _mm512_add_ps(_mm512_add_ps(r7, r8), r9)));
        _mm512_storeu_ps(&dst[i], result);
    }
}

__attribute__((noinline))
void test_avx512_arithmetic_pd(double *src1, double *src2, double *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m512d a = _mm512_loadu_pd(&src1[i]);
        __m512d b = _mm512_loadu_pd(&src2[i]);
        
        /* Basic arithmetic */
        __m512d sum = _mm512_add_pd(a, b);
        __m512d diff = _mm512_sub_pd(a, b);
        __m512d prod = _mm512_mul_pd(a, b);
        __m512d quot = _mm512_div_pd(a, _mm512_max_pd(b, _mm512_set1_pd(0.0001)));
        
        /* Min/Max */
        __m512d vmin = _mm512_min_pd(a, b);
        __m512d vmax = _mm512_max_pd(a, b);
        
        /* Special functions */
        __m512d vsqrt = _mm512_sqrt_pd(_mm512_max_pd(a, _mm512_setzero_pd()));
        
        /* Rounding */
        __m512d vround = _mm512_roundscale_pd(sum, _MM_FROUND_TO_NEAREST_INT);
        __m512d vfloor = _mm512_floor_pd(diff);
        __m512d vceil = _mm512_ceil_pd(prod);
        
        /* FMA */
        __m512d vfmadd = _mm512_fmadd_pd(a, b, sum);
        
        /* Combine */
        __m512d r1 = _mm512_add_pd(sum, diff);
        __m512d r2 = _mm512_add_pd(prod, quot);
        __m512d r3 = _mm512_add_pd(vmin, vmax);
        __m512d r4 = _mm512_add_pd(vsqrt, vround);
        __m512d r5 = _mm512_add_pd(vfloor, vceil);
        __m512d r6 = vfmadd;
        
        __m512d result = _mm512_add_pd(
            _mm512_add_pd(r1, r2),
            _mm512_add_pd(_mm512_add_pd(r3, r4), _mm512_add_pd(r5, r6)));
        _mm512_storeu_pd(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 18: AVX-512 MASKED OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_masked_ops(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 a = _mm512_loadu_ps(&src1[i]);
        __m512 b = _mm512_loadu_ps(&src2[i]);
        __m512 c = _mm512_loadu_ps(&dst[i]);  /* For merge masking */
        
        /* Create various masks */
        __mmask16 mask_cmp = _mm512_cmp_ps_mask(a, b, _CMP_GT_OQ);
        __mmask16 mask_alt = 0b1010101010101010;  /* Alternating */
        __mmask16 mask_upper = 0xFF00;             /* Upper 8 elements */
        __mmask16 mask_lower = 0x00FF;             /* Lower 8 elements */
        
        /* Merge masking: elements where mask=0 come from 'c' */
        __m512 add_merge = _mm512_mask_add_ps(c, mask_cmp, a, b);
        __m512 sub_merge = _mm512_mask_sub_ps(c, mask_alt, a, b);
        __m512 mul_merge = _mm512_mask_mul_ps(c, mask_upper, a, b);
        __m512 div_merge = _mm512_mask_div_ps(c, mask_lower, a, _mm512_max_ps(b, _mm512_set1_ps(0.0001f)));
        
        /* Zero masking: elements where mask=0 become 0 */
        __m512 add_zero = _mm512_maskz_add_ps(mask_cmp, a, b);
        __m512 sub_zero = _mm512_maskz_sub_ps(mask_alt, a, b);
        __m512 mul_zero = _mm512_maskz_mul_ps(mask_upper, a, b);
        __m512 div_zero = _mm512_maskz_div_ps(mask_lower, a, _mm512_max_ps(b, _mm512_set1_ps(0.0001f)));
        
        /* Masked min/max */
        __m512 min_merge = _mm512_mask_min_ps(c, mask_cmp, a, b);
        __m512 max_zero = _mm512_maskz_max_ps(mask_alt, a, b);
        
        /* Masked sqrt */
        __m512 sqrt_merge = _mm512_mask_sqrt_ps(c, mask_upper, _mm512_max_ps(a, _mm512_setzero_ps()));
        
        /* Masked blend */
        __m512 blend = _mm512_mask_blend_ps(mask_cmp, a, b);
        
        /* Masked move */
        __m512 mov = _mm512_mask_mov_ps(c, mask_alt, a);
        
        /* Masked FMA */
        __m512 fma_merge = _mm512_mask_fmadd_ps(a, mask_cmp, b, c);
        __m512 fma_zero = _mm512_maskz_fmadd_ps(mask_alt, a, b, c);
        
        /* Combine all results */
        __m512 r1 = _mm512_add_ps(add_merge, sub_merge);
        __m512 r2 = _mm512_add_ps(mul_merge, div_merge);
        __m512 r3 = _mm512_add_ps(add_zero, sub_zero);
        __m512 r4 = _mm512_add_ps(mul_zero, div_zero);
        __m512 r5 = _mm512_add_ps(min_merge, max_zero);
        __m512 r6 = _mm512_add_ps(sqrt_merge, blend);
        __m512 r7 = _mm512_add_ps(mov, fma_merge);
        __m512 r8 = fma_zero;
        
        __m512 result = _mm512_add_ps(
            _mm512_add_ps(_mm512_add_ps(r1, r2), _mm512_add_ps(r3, r4)),
            _mm512_add_ps(_mm512_add_ps(r5, r6), _mm512_add_ps(r7, r8)));
        _mm512_storeu_ps(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 19: AVX-512 INTEGER OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_integer(int32_t *src1, int32_t *src2, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512i a = _mm512_loadu_si512((__m512i*)&src1[i]);
        __m512i b = _mm512_loadu_si512((__m512i*)&src2[i]);
        
        /* Basic arithmetic */
        __m512i add32 = _mm512_add_epi32(a, b);
        __m512i sub32 = _mm512_sub_epi32(a, b);
        __m512i mul32lo = _mm512_mullo_epi32(a, b);
        
        /* 64-bit arithmetic */
        __m512i add64 = _mm512_add_epi64(a, b);
        __m512i sub64 = _mm512_sub_epi64(a, b);
        __m512i mul64lo = _mm512_mullo_epi64(a, b);
        
        /* Min/Max */
        __m512i min32s = _mm512_min_epi32(a, b);
        __m512i max32s = _mm512_max_epi32(a, b);
        __m512i min32u = _mm512_min_epu32(a, b);
        __m512i max32u = _mm512_max_epu32(a, b);
        
        /* Absolute value */
        __m512i abs32 = _mm512_abs_epi32(a);
        __m512i abs64 = _mm512_abs_epi64(a);
        
        /* Logical */
        __m512i vand = _mm512_and_si512(a, b);
        __m512i vor = _mm512_or_si512(a, b);
        __m512i vxor = _mm512_xor_si512(a, b);
        __m512i vandn = _mm512_andnot_si512(a, b);
        
        /* Ternary logic (unique to AVX-512) */
        /* imm8 encodes truth table for (a,b,c) -> result */
        __m512i tern_and = _mm512_ternarylogic_epi32(a, b, add32, 0x80);   /* a & b & c */
        __m512i tern_or = _mm512_ternarylogic_epi32(a, b, add32, 0xFE);    /* a | b | c */
        __m512i tern_xor = _mm512_ternarylogic_epi32(a, b, add32, 0x96);   /* a ^ b ^ c */
        __m512i tern_maj = _mm512_ternarylogic_epi32(a, b, add32, 0xE8);   /* majority(a,b,c) */
        
        /* Shifts */
        __m512i sll32 = _mm512_slli_epi32(a, 4);
        __m512i srl32 = _mm512_srli_epi32(a, 4);
        __m512i sra32 = _mm512_srai_epi32(a, 4);
        __m512i sra64 = _mm512_srai_epi64(a, 4);  /* AVX-512 has 64-bit arithmetic shift! */
        
        /* Variable shifts */
        __m512i sllv32 = _mm512_sllv_epi32(a, _mm512_and_si512(b, _mm512_set1_epi32(31)));
        __m512i srlv32 = _mm512_srlv_epi32(a, _mm512_and_si512(b, _mm512_set1_epi32(31)));
        __m512i srav32 = _mm512_srav_epi32(a, _mm512_and_si512(b, _mm512_set1_epi32(31)));
        __m512i srav64 = _mm512_srav_epi64(a, _mm512_and_si512(b, _mm512_set1_epi64(63)));
        
        /* Rotate */
        __m512i rol32 = _mm512_rol_epi32(a, 7);
        __m512i ror32 = _mm512_ror_epi32(a, 7);
        __m512i rol64 = _mm512_rol_epi64(a, 13);
        __m512i ror64 = _mm512_ror_epi64(a, 13);
        
        /* Combine results */
        __m512i r1 = _mm512_add_epi32(add32, sub32);
        __m512i r2 = _mm512_add_epi32(mul32lo, min32s);
        __m512i r3 = _mm512_add_epi32(max32s, abs32);
        __m512i r4 = _mm512_add_epi32(vand, vor);
        __m512i r5 = _mm512_add_epi32(vxor, tern_and);
        __m512i r6 = _mm512_add_epi32(tern_or, tern_xor);
        __m512i r7 = _mm512_add_epi32(sll32, srl32);
        __m512i r8 = _mm512_add_epi32(sllv32, srlv32);
        __m512i r9 = _mm512_add_epi32(rol32, ror32);
        
        __m512i result = _mm512_add_epi32(
            _mm512_add_epi32(_mm512_add_epi32(r1, r2), _mm512_add_epi32(r3, r4)),
            _mm512_add_epi32(_mm512_add_epi32(r5, r6), _mm512_add_epi32(_mm512_add_epi32(r7, r8), r9)));
        _mm512_storeu_si512((__m512i*)&dst[i], result);
    }
}

/*============================================================================
 * SECTION 20: AVX-512 COMPARISON AND MASK OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_compare(float *src1, float *src2, int32_t *src_i, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 a = _mm512_loadu_ps(&src1[i]);
        __m512 b = _mm512_loadu_ps(&src2[i]);
        __m512i ia = _mm512_loadu_si512((__m512i*)&src_i[i]);
        __m512i ib = _mm512_loadu_si512((__m512i*)&src_i[i + 16 < n ? i + 16 : 0]);
        
        /* Float comparisons return __mmask16 */
        __mmask16 cmp_eq = _mm512_cmp_ps_mask(a, b, _CMP_EQ_OQ);
        __mmask16 cmp_lt = _mm512_cmp_ps_mask(a, b, _CMP_LT_OQ);
        __mmask16 cmp_le = _mm512_cmp_ps_mask(a, b, _CMP_LE_OQ);
        __mmask16 cmp_gt = _mm512_cmp_ps_mask(a, b, _CMP_GT_OQ);
        __mmask16 cmp_ge = _mm512_cmp_ps_mask(a, b, _CMP_GE_OQ);
        __mmask16 cmp_neq = _mm512_cmp_ps_mask(a, b, _CMP_NEQ_OQ);
        __mmask16 cmp_ord = _mm512_cmp_ps_mask(a, b, _CMP_ORD_Q);
        __mmask16 cmp_unord = _mm512_cmp_ps_mask(a, b, _CMP_UNORD_Q);
        
        /* Integer comparisons */
        __mmask16 icmp_eq = _mm512_cmpeq_epi32_mask(ia, ib);
        __mmask16 icmp_gt = _mm512_cmpgt_epi32_mask(ia, ib);
        __mmask16 icmp_lt = _mm512_cmplt_epi32_mask(ia, ib);
        __mmask16 icmp_neq = _mm512_cmpneq_epi32_mask(ia, ib);
        __mmask16 icmp_ge = _mm512_cmpge_epi32_mask(ia, ib);
        __mmask16 icmp_le = _mm512_cmple_epi32_mask(ia, ib);
        
        /* Mask operations */
        __mmask16 mask_and = _kand_mask16(cmp_lt, cmp_gt);
        __mmask16 mask_or = _kor_mask16(cmp_lt, cmp_gt);
        __mmask16 mask_xor = _kxor_mask16(cmp_eq, cmp_neq);
        __mmask16 mask_not = _knot_mask16(cmp_eq);
        __mmask16 mask_andn = _kandn_mask16(cmp_lt, cmp_gt);
        
        /* Test operations */
        __mmask16 test_mask = _mm512_test_epi32_mask(ia, ib);  /* (a & b) != 0 */
        __mmask16 testn_mask = _mm512_testn_epi32_mask(ia, ib); /* (a & b) == 0 */
        
        /* Mask to integer and back */
        int mask_int = (int)cmp_lt;
        __mmask16 from_int = (__mmask16)mask_int;
        
        /* Kortest - test if any/all bits set */
        int any_lt = !_kortestz_mask16_u8(cmp_lt, cmp_lt);  /* Any bit set? */
        int all_lt = _kortestc_mask16_u8(cmp_lt, cmp_lt);   /* All bits set? */
        
        /* Use masks for blending */
        __m512 blend1 = _mm512_mask_blend_ps(cmp_lt, a, b);
        __m512 blend2 = _mm512_mask_blend_ps(icmp_gt, a, b);
        __m512 blend3 = _mm512_mask_blend_ps(mask_or, a, b);
        
        /* Masked move based on comparison */
        __m512 mov1 = _mm512_mask_mov_ps(a, cmp_gt, b);
        __m512 mov2 = _mm512_maskz_mov_ps(cmp_lt, a);
        
        /* Combine */
        __m512 modifier = _mm512_set1_ps((float)(any_lt + all_lt * 2));
        __m512 result = _mm512_add_ps(_mm512_add_ps(blend1, blend2), 
                                       _mm512_add_ps(_mm512_add_ps(blend3, mov1), 
                                                     _mm512_add_ps(mov2, modifier)));
        _mm512_storeu_ps(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 21: AVX-512 SHUFFLE/PERMUTE
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_shuffle(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 a = _mm512_loadu_ps(&src1[i]);
        __m512 b = _mm512_loadu_ps(&src2[i]);
        __m512i idx = _mm512_setr_epi32(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
        
        /* Shuffle within 128-bit lanes */
        __m512 shuf = _mm512_shuffle_ps(a, b, _MM_SHUFFLE(3, 1, 2, 0));
        
        /* Permute single source */
        __m512 perm = _mm512_permute_ps(a, _MM_SHUFFLE(2, 3, 0, 1));
        
        /* Permute across all lanes (AVX-512) */
        __m512 permx = _mm512_permutexvar_ps(idx, a);
        
        /* Permute two sources (powerful 2-input permute) */
        __m512 perm2 = _mm512_permutex2var_ps(a, idx, b);
        
        /* Shuffle 128-bit lanes */
        __m512 shuf128 = _mm512_shuffle_f32x4(a, b, _MM_SHUFFLE(3, 1, 2, 0));
        
        /* Shuffle 64-bit doubles across 128-bit lanes */
        __m512d ad = _mm512_castps_pd(a);
        __m512d bd = _mm512_castps_pd(b);
        __m512d shuf64 = _mm512_shuffle_pd(ad, bd, 0b10101010);
        
        /* Unpack */
        __m512 unpackhi = _mm512_unpackhi_ps(a, b);
        __m512 unpacklo = _mm512_unpacklo_ps(a, b);
        
        /* Masked shuffle/permute */
        __mmask16 mask = 0b1010101010101010;
        __m512 perm_mask = _mm512_mask_permutexvar_ps(a, mask, idx, b);
        __m512 perm2_mask = _mm512_mask_permutex2var_ps(a, mask, idx, b);
        
        /* Combine */
        __m512 r1 = _mm512_add_ps(shuf, perm);
        __m512 r2 = _mm512_add_ps(permx, perm2);
        __m512 r3 = _mm512_add_ps(shuf128, _mm512_castpd_ps(shuf64));
        __m512 r4 = _mm512_add_ps(unpackhi, unpacklo);
        __m512 r5 = _mm512_add_ps(perm_mask, perm2_mask);
        
        __m512 result = _mm512_add_ps(_mm512_add_ps(r1, r2), _mm512_add_ps(_mm512_add_ps(r3, r4), r5));
        _mm512_storeu_ps(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 22: AVX-512 BROADCAST
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_broadcast(float *src_ps, double *src_pd, int32_t *src_i, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        /* Broadcast scalar */
        __m512 bcast_ss = _mm512_set1_ps(src_ps[i]);
        __m512d bcast_sd = _mm512_set1_pd(src_pd[i >> 1]);
        __m512i bcast_d = _mm512_set1_epi32(src_i[i]);
        __m512i bcast_q = _mm512_set1_epi64(src_i[i]);
        
        /* Broadcast from memory */
        __m512 bcast_mem_ps = _mm512_broadcastss_ps(_mm_load_ss(&src_ps[i]));
        __m512d bcast_mem_pd = _mm512_broadcastsd_pd(_mm_load_sd(&src_pd[i >> 1]));
        
        /* Broadcast 128-bit to all lanes */
        __m128 xmm = _mm_loadu_ps(&src_ps[i]);
        __m512 bcast_128 = _mm512_broadcast_f32x4(xmm);
        
        /* Broadcast 256-bit to both halves */
        __m256 ymm = _mm256_loadu_ps(&src_ps[i]);
        __m512 bcast_256 = _mm512_broadcast_f32x8(ymm);
        
        /* Integer broadcasts */
        __m128i xmmi = _mm_loadu_si128((__m128i*)&src_i[i]);
        __m512i bcast_i128 = _mm512_broadcast_i32x4(xmmi);
        
        __m256i ymmi = _mm256_loadu_si256((__m256i*)&src_i[i]);
        __m512i bcast_i256 = _mm512_broadcast_i32x8(ymmi);
        
        /* Combine */
        __m512 r1 = _mm512_add_ps(bcast_ss, bcast_mem_ps);
        __m512 r2 = _mm512_add_ps(bcast_128, bcast_256);
        __m512 r3 = _mm512_castsi512_ps(_mm512_add_epi32(bcast_d, bcast_i128));
        
        __m512 result = _mm512_add_ps(r1, _mm512_add_ps(r2, r3));
        _mm512_storeu_ps(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 23: AVX-512 GATHER/SCATTER
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_gather_scatter(float *base, int32_t *indices, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512i idx = _mm512_loadu_si512((__m512i*)&indices[i]);
        
        /* Gather 16 floats */
        __m512 gathered = _mm512_i32gather_ps(idx, base, 4);
        
        /* Masked gather */
        __mmask16 mask = 0b1111111100000000;  /* Upper 8 only */
        __m512 src = _mm512_loadu_ps(&dst[i]);
        __m512 gathered_mask = _mm512_mask_i32gather_ps(src, mask, idx, base, 4);
        
        /* Gather doubles (8 doubles with 8 indices) */
        __m256i idx256 = _mm256_loadu_si256((__m256i*)&indices[i]);
        __m512d gathered_pd = _mm512_i32gather_pd(idx256, (double*)base, 8);
        
        /* Gather 32-bit integers */
        __m512i gathered_d = _mm512_i32gather_epi32(idx, (int*)base, 4);
        
        /* Gather 64-bit integers */
        __m512i gathered_q = _mm512_i32gather_epi64(idx256, (long long*)base, 8);
        
        /* Process gathered data */
        __m512 processed = _mm512_add_ps(gathered, gathered_mask);
        
        /* Scatter back */
        __m512i scatter_idx = _mm512_add_epi32(idx, _mm512_set1_epi32(n));
        _mm512_i32scatter_ps(dst, scatter_idx, processed, 4);
        
        /* Masked scatter */
        __mmask16 scatter_mask = 0b0101010101010101;
        _mm512_mask_i32scatter_ps(dst, scatter_mask, scatter_idx, gathered, 4);
        
        /* Store result */
        _mm512_storeu_ps(&dst[i], processed);
    }
}

/*============================================================================
 * SECTION 24: AVX-512 COMPRESS/EXPAND
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_compress_expand(float *src, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 a = _mm512_loadu_ps(&src[i]);
        
        /* Create mask based on comparison */
        __mmask16 mask = _mm512_cmp_ps_mask(a, _mm512_setzero_ps(), _CMP_GT_OQ);
        
        /* Compress: pack elements where mask=1 to low positions */
        __m512 compressed = _mm512_maskz_compress_ps(mask, a);
        
        /* Expand: scatter low elements to positions where mask=1 */
        __m512 expanded = _mm512_maskz_expand_ps(mask, a);
        
        /* Compress store: write only selected elements contiguously to memory */
        _mm512_mask_compressstoreu_ps(&dst[i], mask, a);
        
        /* Expand load: read contiguous elements into selected positions */
        __m512 expand_loaded = _mm512_maskz_expandloadu_ps(mask, &src[i]);
        
        /* Integer versions */
        __m512i ai = _mm512_castps_si512(a);
        __m512i compressed_i = _mm512_maskz_compress_epi32(mask, ai);
        __m512i expanded_i = _mm512_maskz_expand_epi32(mask, ai);
        
        /* Combine */
        __m512 result = _mm512_add_ps(_mm512_add_ps(compressed, expanded), expand_loaded);
        _mm512_storeu_ps(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 25: AVX-512 CONFLICT DETECTION (AVX512CD)
 *===========================================================================*/

#ifdef __AVX512CD__

__attribute__((noinline))
void test_avx512_conflict(int32_t *indices, int32_t *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512i idx = _mm512_loadu_si512((__m512i*)&indices[i]);
        
        /* Conflict detection: for each element, which earlier elements have same value */
        __m512i conflicts = _mm512_conflict_epi32(idx);
        
        /* Leading zero count */
        __m512i lzcnt = _mm512_lzcnt_epi32(idx);
        
        /* 64-bit versions */
        __m512i conflicts64 = _mm512_conflict_epi64(idx);
        __m512i lzcnt64 = _mm512_lzcnt_epi64(idx);
        
        /* Use conflict detection for histogram-like operations */
        /* If conflicts[i] == 0, element i has no conflicts with earlier elements */
        __mmask16 no_conflict = _mm512_cmpeq_epi32_mask(conflicts, _mm512_setzero_si512());
        
        /* Combine */
        __m512i result = _mm512_add_epi32(_mm512_add_epi32(conflicts, lzcnt), conflicts64);
        _mm512_storeu_si512((__m512i*)&dst[i], result);
    }
}

#endif /* __AVX512CD__ */

/*============================================================================
 * SECTION 26: AVX-512 SPECIAL OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_special(float *src1, float *src2, float *dst, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 a = _mm512_loadu_ps(&src1[i]);
        __m512 b = _mm512_loadu_ps(&src2[i]);
        
        /* Getexp: extract exponent as float */
        __m512 vexp = _mm512_getexp_ps(a);
        
        /* Getmant: extract mantissa with normalization */
        __m512 vmant = _mm512_getmant_ps(a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src);
        
        /* Scalef: scale by power of 2 (a * 2^floor(b)) */
        __m512 scaled = _mm512_scalef_ps(a, b);
        
        /* Fixupimm: fixup special values based on table */
        __m512i fixup_table = _mm512_set1_epi32(0x88888888);
        __m512 fixed = _mm512_fixupimm_ps(a, b, fixup_table, 0);
        
        /* Range: per-element min/max with sign control */
        __m512 vrange = _mm512_range_ps(a, b, 0x00);  /* abs min */
        
        /* Reduce: reduce argument (for trig functions) */
        __m512 reduced = _mm512_reduce_ps(a, 0);
        
        /* FP classification */
        __mmask16 is_nan = _mm512_fpclass_ps_mask(a, 0x01);      /* QNaN */
        __mmask16 is_neg_inf = _mm512_fpclass_ps_mask(a, 0x04);  /* -Inf */
        __mmask16 is_pos_inf = _mm512_fpclass_ps_mask(a, 0x08);  /* +Inf */
        __mmask16 is_zero = _mm512_fpclass_ps_mask(a, 0x02 | 0x10);  /* +0 or -0 */
        __mmask16 is_denorm = _mm512_fpclass_ps_mask(a, 0x20 | 0x40);  /* denormals */
        
        /* Use classification for cleanup */
        __m512 cleaned = _mm512_mask_mov_ps(a, is_nan, _mm512_setzero_ps());
        
        /* Rounding with scale */
        __m512 round_scale = _mm512_roundscale_ps(a, _MM_FROUND_TO_ZERO | (4 << 4));
        
        /* Combine */
        __m512 r1 = _mm512_add_ps(vexp, vmant);
        __m512 r2 = _mm512_add_ps(scaled, fixed);
        __m512 r3 = _mm512_add_ps(vrange, reduced);
        __m512 r4 = _mm512_add_ps(cleaned, round_scale);
        
        __m512 result = _mm512_add_ps(_mm512_add_ps(r1, r2), _mm512_add_ps(r3, r4));
        _mm512_storeu_ps(&dst[i], result);
    }
}

/*============================================================================
 * SECTION 27: AVX-512 CONVERSION OPERATIONS
 *===========================================================================*/

__attribute__((noinline))
void test_avx512_conversions(float *src_ps, double *src_pd, int32_t *src_i, 
                              float *dst_ps, double *dst_pd, int32_t *dst_i, int n) {
    for (int i = 0; i < n; i += 16) {
        __m512 ps = _mm512_loadu_ps(&src_ps[i]);
        __m512d pd = _mm512_loadu_pd(&src_pd[i >> 1]);
        __m512i i32 = _mm512_loadu_si512((__m512i*)&src_i[i]);
        __m256 ps256 = _mm256_loadu_ps(&src_ps[i]);
        __m256i i32_256 = _mm256_loadu_si256((__m256i*)&src_i[i]);
        
        /* Float <-> Double */
        __m256 pd_to_ps = _mm512_cvtpd_ps(pd);              /* 8 double -> 8 float */
        __m512d ps_to_pd = _mm512_cvtps_pd(ps256);          /* 8 float -> 8 double */
        
        /* Float <-> Int32 */
        __m512i ps_to_i32 = _mm512_cvtps_epi32(ps);         /* 16 float -> 16 int32 (round) */
        __m512i ps_to_i32_t = _mm512_cvttps_epi32(ps);      /* 16 float -> 16 int32 (truncate) */
        __m512 i32_to_ps = _mm512_cvtepi32_ps(i32);         /* 16 int32 -> 16 float */
        
        /* Float <-> Unsigned Int32 */
        __m512i ps_to_u32 = _mm512_cvtps_epu32(_mm512_max_ps(ps, _mm512_setzero_ps()));
        __m512 u32_to_ps = _mm512_cvtepu32_ps(i32);
        
        /* Double <-> Int32 */
        __m256i pd_to_i32 = _mm512_cvtpd_epi32(pd);         /* 8 double -> 8 int32 (round) */
        __m256i pd_to_i32_t = _mm512_cvttpd_epi32(pd);      /* 8 double -> 8 int32 (truncate) */
        __m512d i32_to_pd = _mm512_cvtepi32_pd(i32_256);    /* 8 int32 -> 8 double */
        
        /* Double <-> Int64 (AVX-512DQ) */
        #ifdef __AVX512DQ__
        __m512i pd_to_i64 = _mm512_cvtpd_epi64(pd);         /* 8 double -> 8 int64 */
        __m512d i64_to_pd = _mm512_cvtepi64_pd(i32);        /* 8 int64 -> 8 double */
        __m512i ps_to_i64 = _mm512_cvtps_epi64(ps256);      /* 8 float -> 8 int64 */
        __m256 i64_to_ps = _mm512_cvtepi64_ps(i32);         /* 8 int64 -> 8 float (returns __m256!) */
        #endif
        
        /* Sign/zero extension */
        __m512i ext_8_32 = _mm512_cvtepi8_epi32(_mm_loadu_si128((__m128i*)src_i));
        __m512i ext_8_64 = _mm512_cvtepi8_epi64(_mm_loadu_si128((__m128i*)src_i));
        __m512i ext_16_32 = _mm512_cvtepi16_epi32(_mm256_loadu_si256((__m256i*)src_i));
        __m512i ext_16_64 = _mm512_cvtepi16_epi64(_mm_loadu_si128((__m128i*)src_i));
        __m512i ext_32_64 = _mm512_cvtepi32_epi64(_mm256_loadu_si256((__m256i*)src_i));
        
        __m512i extu_8_32 = _mm512_cvtepu8_epi32(_mm_loadu_si128((__m128i*)src_i));
        __m512i extu_16_32 = _mm512_cvtepu16_epi32(_mm256_loadu_si256((__m256i*)src_i));
        __m512i extu_32_64 = _mm512_cvtepu32_epi64(_mm256_loadu_si256((__m256i*)src_i));
        
        /* Truncation pack */
        __m256i trunc_32_16 = _mm512_cvtepi32_epi16(i32);   /* 16 int32 -> 16 int16 (truncate) */
        __m128i trunc_32_8 = _mm512_cvtepi32_epi8(i32);     /* 16 int32 -> 16 int8 (truncate) */
        __m256i trunc_64_32 = _mm512_cvtepi64_epi32(i32);   /* 8 int64 -> 8 int32 (truncate) */
        
        /* Saturating pack */
        __m256i sat_32_16 = _mm512_cvtsepi32_epi16(i32);    /* 16 int32 -> 16 int16 (saturate) */
        __m128i sat_32_8 = _mm512_cvtsepi32_epi8(i32);      /* 16 int32 -> 16 int8 (saturate) */
        __m256i satu_32_16 = _mm512_cvtusepi32_epi16(i32);  /* Unsigned saturate */
        
        /* Store results */
        _mm512_storeu_ps(&dst_ps[i], i32_to_ps);
        _mm512_storeu_pd(&dst_pd[i >> 1], ps_to_pd);
        _mm512_storeu_si512((__m512i*)&dst_i[i], ps_to_i32);
    }
}

/*============================================================================
 * SECTION 28: AVX-512BW BYTE/WORD OPERATIONS
 *===========================================================================*/

#ifdef __AVX512BW__

__attribute__((noinline))
void test_avx512bw(int16_t *src1, int16_t *src2, int16_t *dst, int n) {
    for (int i = 0; i < n; i += 32) {
        __m512i a = _mm512_loadu_si512((__m512i*)&src1[i]);
        __m512i b = _mm512_loadu_si512((__m512i*)&src2[i]);
        
        /* 16-bit operations */
        __m512i add16 = _mm512_add_epi16(a, b);
        __m512i sub16 = _mm512_sub_epi16(a, b);
        __m512i mullo16 = _mm512_mullo_epi16(a, b);
        __m512i mulhi16 = _mm512_mulhi_epi16(a, b);
        __m512i mulhiu16 = _mm512_mulhi_epu16(a, b);
        
        /* Saturating arithmetic */
        __m512i adds16 = _mm512_adds_epi16(a, b);
        __m512i subs16 = _mm512_subs_epi16(a, b);
        __m512i addsu16 = _mm512_adds_epu16(a, b);
        __m512i subsu16 = _mm512_subs_epu16(a, b);
        
        /* 8-bit operations */
        __m512i add8 = _mm512_add_epi8(a, b);
        __m512i sub8 = _mm512_sub_epi8(a, b);
        __m512i adds8 = _mm512_adds_epi8(a, b);
        __m512i subs8 = _mm512_subs_epi8(a, b);
        
        /* Min/Max */
        __m512i min16s = _mm512_min_epi16(a, b);
        __m512i max16s = _mm512_max_epi16(a, b);
        __m512i min8s = _mm512_min_epi8(a, b);
        __m512i max8s = _mm512_max_epi8(a, b);
        
        /* Absolute value */
        __m512i abs16 = _mm512_abs_epi16(a);
        __m512i abs8 = _mm512_abs_epi8(a);
        
        /* Average */
        __m512i avg16 = _mm512_avg_epu16(a, b);
        __m512i avg8 = _mm512_avg_epu8(a, b);
        
        /* MADD */
        __m512i madd = _mm512_madd_epi16(a, b);
        __m512i maddubs = _mm512_maddubs_epi16(a, b);
        
        /* Shuffle bytes */
        __m512i shufb = _mm512_shuffle_epi8(a, b);
        
        /* Pack */
        __m512i packs = _mm512_packs_epi16(a, b);
        __m512i packus = _mm512_packus_epi16(a, b);
        
        /* 64-bit masks for byte/word operations */
        __mmask64 mask8 = _mm512_cmpgt_epi8_mask(a, b);
        __mmask32 mask16 = _mm512_cmpgt_epi16_mask(a, b);
        
        /* Masked operations */
        __m512i add16_mask = _mm512_mask_add_epi16(a, mask16, a, b);
        __m512i add8_mask = _mm512_mask_add_epi8(a, mask8, a, b);
        
        /* Combine */
        __m512i r1 = _mm512_add_epi16(add16, sub16);
        __m512i r2 = _mm512_add_epi16(mullo16, adds16);
        __m512i r3 = _mm512_add_epi16(min16s, max16s);
        __m512i r4 = _mm512_add_epi16(abs16, avg16);
        __m512i r5 = _mm512_add_epi16(add16_mask, add8_mask);
        
        __m512i result = _mm512_add_epi16(_mm512_add_epi16(r1, r2), 
                                          _mm512_add_epi16(_mm512_add_epi16(r3, r4), r5));
        _mm512_storeu_si512((__m512i*)&dst[i], result);
    }
}

#endif /* __AVX512BW__ */

/*============================================================================
 * SECTION 29: AVX-512DQ DOUBLE/QUAD OPERATIONS
 *===========================================================================*/

#ifdef __AVX512DQ__

__attribute__((noinline))
void test_avx512dq(double *src1, double *src2, int64_t *src_i, double *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m512d a = _mm512_loadu_pd(&src1[i]);
        __m512d b = _mm512_loadu_pd(&src2[i]);
        __m512i ia = _mm512_loadu_si512((__m512i*)&src_i[i]);
        
        /* 64-bit integer multiply */
        __m512i mul64 = _mm512_mullo_epi64(ia, ia);
        
        /* Float/double <-> int64 conversions */
        __m512i pd_to_i64 = _mm512_cvtpd_epi64(a);
        __m512d i64_to_pd = _mm512_cvtepi64_pd(ia);
        __m512i pd_to_u64 = _mm512_cvtpd_epu64(_mm512_max_pd(a, _mm512_setzero_pd()));
        __m512d u64_to_pd = _mm512_cvtepu64_pd(ia);
        
        /* Range operations */
        __m512d vrange = _mm512_range_pd(a, b, 0x00);
        
        /* Reduce */
        __m512d reduced = _mm512_reduce_pd(a, 0);
        
        /* And/or floating point */
        __m512d vand = _mm512_and_pd(a, b);
        __m512d vor = _mm512_or_pd(a, b);
        __m512d vxor = _mm512_xor_pd(a, b);
        __m512d vandn = _mm512_andnot_pd(a, b);
        
        /* Broadcast 64-bit to 512-bit */
        __m512d bcast_f64 = _mm512_broadcastsd_pd(_mm_load_sd(&src1[i]));
        __m512i bcast_i64 = _mm512_broadcastq_epi64(_mm_loadu_si128((__m128i*)&src_i[i]));
        
        /* Extract/insert 256-bit lanes */
        __m256d lo = _mm512_extractf64x4_pd(a, 0);
        __m256d hi = _mm512_extractf64x4_pd(a, 1);
        __m512d inserted = _mm512_insertf64x4(a, hi, 0);
        
        /* FP classification */
        __mmask8 is_nan = _mm512_fpclass_pd_mask(a, 0x01);
        
        /* Combine */
        __m512d r1 = _mm512_add_pd(i64_to_pd, u64_to_pd);
        __m512d r2 = _mm512_add_pd(vrange, reduced);
        __m512d r3 = _mm512_add_pd(vand, vor);
        __m512d r4 = _mm512_add_pd(vxor, bcast_f64);
        __m512d r5 = inserted;
        
        __m512d result = _mm512_add_pd(_mm512_add_pd(r1, r2), _mm512_add_pd(_mm512_add_pd(r3, r4), r5));
        _mm512_storeu_pd(&dst[i], result);
    }
}

#endif /* __AVX512DQ__ */

/*============================================================================
 * SECTION 30: AVX-512 VBMI (Vector Byte Manipulation Instructions)
 *===========================================================================*/

#ifdef __AVX512VBMI__

__attribute__((noinline))
void test_avx512vbmi(int8_t *src1, int8_t *src2, int8_t *dst, int n) {
    for (int i = 0; i < n; i += 64) {
        __m512i a = _mm512_loadu_si512((__m512i*)&src1[i]);
        __m512i b = _mm512_loadu_si512((__m512i*)&src2[i]);
        __m512i idx = _mm512_loadu_si512((__m512i*)&src2[i]);
        
        /* Permute bytes (single source) */
        __m512i perm1 = _mm512_permutexvar_epi8(idx, a);
        
        /* Permute bytes (two sources) */
        __m512i perm2 = _mm512_permutex2var_epi8(a, idx, b);
        
        /* Multi-shift (shift each qword by per-byte amounts, extract bytes) */
        __m512i mshift = _mm512_multishift_epi64_epi8(idx, a);
        
        /* Combine */
        __m512i result = _mm512_add_epi8(_mm512_add_epi8(perm1, perm2), mshift);
        _mm512_storeu_si512((__m512i*)&dst[i], result);
    }
}

#endif /* __AVX512VBMI__ */

/*============================================================================
 * SECTION 31: AVX-512 IFMA (Integer Fused Multiply-Add)
 *===========================================================================*/

#ifdef __AVX512IFMA__

__attribute__((noinline))
void test_avx512ifma(int64_t *src1, int64_t *src2, int64_t *src3, int64_t *dst, int n) {
    for (int i = 0; i < n; i += 8) {
        __m512i a = _mm512_loadu_si512((__m512i*)&src1[i]);
        __m512i b = _mm512_loadu_si512((__m512i*)&src2[i]);
        __m512i c = _mm512_loadu_si512((__m512i*)&src3[i]);
        
        /* 52-bit integer FMA - useful for big integer / crypto */
        /* madd52lo: (a + b*c) low 52 bits */
        __m512i madd52lo = _mm512_madd52lo_epu64(a, b, c);
        
        /* madd52hi: (a + b*c) high 52 bits (shifted) */
        __m512i madd52hi = _mm512_madd52hi_epu64(a, b, c);
        
        /* Combine */
        __m512i result = _mm512_add_epi64(madd52lo, madd52hi);
        _mm512_storeu_si512((__m512i*)&dst[i], result);
    }
}

#endif /* __AVX512IFMA__ */

#endif /* __AVX512F__ */

/*============================================================================
 * SECTION 32: UTILITY FUNCTIONS AND TEST HARNESS
 *===========================================================================*/

static void init_float_array(float *arr, int n, float base) {
    for (int i = 0; i < n; i++) {
        arr[i] = base + (float)(i % 100) * 0.01f + (float)((i * 7) % 13) * 0.001f;
    }
}

static void init_double_array(double *arr, int n, double base) {
    for (int i = 0; i < n; i++) {
        arr[i] = base + (double)(i % 100) * 0.01 + (double)((i * 7) % 13) * 0.001;
    }
}

static void init_int32_array(int32_t *arr, int n, int32_t base) {
    for (int i = 0; i < n; i++) {
        arr[i] = base + (i % 256) - 128 + ((i * 17) % 31);
    }
}

static void init_int16_array(int16_t *arr, int n, int16_t base) {
    for (int i = 0; i < n; i++) {
        arr[i] = base + (i % 128) - 64 + ((i * 13) % 23);
    }
}

static void init_int8_array(int8_t *arr, int n, int8_t base) {
    for (int i = 0; i < n; i++) {
        arr[i] = base + (i % 64) - 32 + ((i * 11) % 17);
    }
}

static void init_int64_array(int64_t *arr, int n, int64_t base) {
    for (int i = 0; i < n; i++) {
        arr[i] = base + (i % 1000) - 500 + ((int64_t)(i * 23) % 97);
    }
}

static void init_index_array(int32_t *arr, int n, int max_idx) {
    for (int i = 0; i < n; i++) {
        arr[i] = (i * 7 + 3) % max_idx;
    }
}

int main(int argc, char *argv[]) {
    const int N = 1024;  /* Array size */
    
    /* Allocate aligned arrays */
    float *f1 ALIGN64 = aligned_alloc(64, N * sizeof(float));
    float *f2 ALIGN64 = aligned_alloc(64, N * sizeof(float));
    float *f3 ALIGN64 = aligned_alloc(64, N * sizeof(float));
    float *fd ALIGN64 = aligned_alloc(64, N * sizeof(float));
    
    double *d1 ALIGN64 = aligned_alloc(64, N * sizeof(double));
    double *d2 ALIGN64 = aligned_alloc(64, N * sizeof(double));
    double *d3 ALIGN64 = aligned_alloc(64, N * sizeof(double));
    double *dd ALIGN64 = aligned_alloc(64, N * sizeof(double));
    
    int32_t *i1 ALIGN64 = aligned_alloc(64, N * sizeof(int32_t));
    int32_t *i2 ALIGN64 = aligned_alloc(64, N * sizeof(int32_t));
    int32_t *id ALIGN64 = aligned_alloc(64, N * sizeof(int32_t));
    int32_t *indices ALIGN64 = aligned_alloc(64, N * sizeof(int32_t));
    
    int16_t *s1 ALIGN64 = aligned_alloc(64, N * sizeof(int16_t));
    int16_t *s2 ALIGN64 = aligned_alloc(64, N * sizeof(int16_t));
    int16_t *sd ALIGN64 = aligned_alloc(64, N * sizeof(int16_t));
    
    int8_t *b1 ALIGN64 = aligned_alloc(64, N * sizeof(int8_t));
    int8_t *b2 ALIGN64 = aligned_alloc(64, N * sizeof(int8_t));
    int8_t *bd ALIGN64 = aligned_alloc(64, N * sizeof(int8_t));
    
    int64_t *q1 ALIGN64 = aligned_alloc(64, N * sizeof(int64_t));
    int64_t *q2 ALIGN64 = aligned_alloc(64, N * sizeof(int64_t));
    int64_t *q3 ALIGN64 = aligned_alloc(64, N * sizeof(int64_t));
    int64_t *qd ALIGN64 = aligned_alloc(64, N * sizeof(int64_t));
    
    uint8_t *u1 ALIGN64 = aligned_alloc(64, N * sizeof(uint8_t));
    uint8_t *u2 ALIGN64 = aligned_alloc(64, N * sizeof(uint8_t));
    
    /* Initialize arrays */
    init_float_array(f1, N, 1.0f);
    init_float_array(f2, N, 2.0f);
    init_float_array(f3, N, 0.5f);
    init_double_array(d1, N, 1.0);
    init_double_array(d2, N, 2.0);
    init_double_array(d3, N, 0.5);
    init_int32_array(i1, N, 100);
    init_int32_array(i2, N, 50);
    init_int16_array(s1, N, 100);
    init_int16_array(s2, N, 50);
    init_int8_array(b1, N, 10);
    init_int8_array(b2, N, 5);
    init_int64_array(q1, N, 1000);
    init_int64_array(q2, N, 500);
    init_int64_array(q3, N, 250);
    init_index_array(indices, N, N);
    memcpy(u1, b1, N);
    memcpy(u2, b2, N);
    
    printf("=== AVX/AVX2/AVX-512 Comprehensive Test Suite ===\n\n");
    
    /* AVX Tests */
    printf("--- AVX Arithmetic (float) ---\n");
    test_avx_arithmetic_ps(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX Arithmetic (double) ---\n");
    test_avx_arithmetic_pd(d1, d2, dd, N);
    g_accumulator += dd[0];
    
    printf("--- AVX Scalar (float) ---\n");
    g_accumulator += test_avx_scalar_ss(3.14f, 2.71f);
    
    printf("--- AVX Scalar (double) ---\n");
    g_accumulator += test_avx_scalar_sd(3.14159, 2.71828);
    
    /* FMA Tests */
    printf("--- FMA (float) ---\n");
    test_fma_ps(f1, f2, f3, fd, N);
    g_accumulator += fd[0];
    
    printf("--- FMA (double) ---\n");
    test_fma_pd(d1, d2, d3, dd, N);
    g_accumulator += dd[0];
    
    printf("--- FMA Scalar ---\n");
    g_accumulator += test_fma_scalar_ss(1.5f, 2.5f, 3.5f);
    
    /* AVX2 Integer Tests */
    printf("--- AVX2 Integer (32-bit) ---\n");
    test_avx2_integer_arithmetic(i1, i2, id, N);
    g_int_accumulator += id[0];
    
    printf("--- AVX2 Integer (16-bit) ---\n");
    test_avx2_integer_8_16(s1, s2, sd, N);
    g_int_accumulator += sd[0];
    
    printf("--- AVX2 Integer (64-bit) ---\n");
    test_avx2_integer_64(q1, q2, qd, N);
    g_int_accumulator += qd[0];
    
    /* Logical Tests */
    printf("--- Logical (float) ---\n");
    test_logical_ops_ps(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- Logical (int) ---\n");
    test_logical_ops_int(i1, i2, id, N);
    g_int_accumulator += id[0];
    
    /* Comparison Tests */
    printf("--- Compare (float) ---\n");
    test_compare_ps(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- Compare (int) ---\n");
    test_compare_int(i1, i2, id, N);
    g_int_accumulator += id[0];
    
    /* Shuffle/Permute Tests */
    printf("--- Shuffle (float) ---\n");
    test_shuffle_ps(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- Shuffle (int) ---\n");
    test_shuffle_int(i1, i2, id, N);
    g_int_accumulator += id[0];
    
    /* Broadcast Tests */
    printf("--- Broadcast ---\n");
    test_broadcast(f1, d1, i1, fd, N);
    g_accumulator += fd[0];
    
    /* Gather Tests */
    printf("--- Gather ---\n");
    test_gather(f1, d1, i1, indices, fd, N);
    g_accumulator += fd[0];
    
    /* Shift Tests */
    printf("--- Shifts ---\n");
    test_shifts(i1, i2, id, N);
    g_int_accumulator += id[0];
    
    /* Insert/Extract Tests */
    printf("--- Insert/Extract ---\n");
    test_insert_extract(f1, i1, fd, N);
    g_accumulator += fd[0];
    
    /* Conversion Tests */
    printf("--- Conversions ---\n");
    test_conversions(f1, d1, i1, fd, dd, id, N);
    g_accumulator += fd[0] + dd[0];
    
    /* Horizontal Tests */
    printf("--- Horizontal Sum (float) ---\n");
    g_accumulator += test_horizontal_sum_ps(_mm256_loadu_ps(f1));
    
    printf("--- Horizontal Sum (double) ---\n");
    g_accumulator += test_horizontal_sum_pd(_mm256_loadu_pd(d1));
    
    printf("--- Horizontal Sum (int32) ---\n");
    g_int_accumulator += test_horizontal_sum_epi32(_mm256_loadu_si256((__m256i*)i1));
    
    /* SAD Tests */
    printf("--- SAD ---\n");
    test_sad(u1, u2, qd, N);
    g_int_accumulator += qd[0];
    
    /* Movemask Tests */
    printf("--- Movemask ---\n");
    g_int_accumulator += test_movemask(f1, d1, b1, N);
    
    /* Special Moves */
    printf("--- Special Moves ---\n");
    test_special_moves(f1, fd, N);
    g_accumulator += fd[0];
    
#ifdef __AVX512F__
    printf("\n=== AVX-512 Tests ===\n\n");
    
    printf("--- AVX-512 Arithmetic (float) ---\n");
    test_avx512_arithmetic_ps(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Arithmetic (double) ---\n");
    test_avx512_arithmetic_pd(d1, d2, dd, N);
    g_accumulator += dd[0];
    
    printf("--- AVX-512 Masked Ops ---\n");
    test_avx512_masked_ops(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Integer ---\n");
    test_avx512_integer(i1, i2, id, N);
    g_int_accumulator += id[0];
    
    printf("--- AVX-512 Compare ---\n");
    test_avx512_compare(f1, f2, i1, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Shuffle ---\n");
    test_avx512_shuffle(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Broadcast ---\n");
    test_avx512_broadcast(f1, d1, i1, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Gather/Scatter ---\n");
    test_avx512_gather_scatter(f1, indices, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Compress/Expand ---\n");
    test_avx512_compress_expand(f1, fd, N);
    g_accumulator += fd[0];
    
#ifdef __AVX512CD__
    printf("--- AVX-512 Conflict Detection ---\n");
    test_avx512_conflict(i1, id, N);
    g_int_accumulator += id[0];
#endif
    
    printf("--- AVX-512 Special ---\n");
    test_avx512_special(f1, f2, fd, N);
    g_accumulator += fd[0];
    
    printf("--- AVX-512 Conversions ---\n");
    test_avx512_conversions(f1, d1, i1, fd, dd, id, N);
    g_accumulator += fd[0] + dd[0];
    
#ifdef __AVX512BW__
    printf("--- AVX-512BW ---\n");
    test_avx512bw(s1, s2, sd, N);
    g_int_accumulator += sd[0];
#endif
    
#ifdef __AVX512DQ__
    printf("--- AVX-512DQ ---\n");
    test_avx512dq(d1, d2, q1, dd, N);
    g_accumulator += dd[0];
#endif
    
#ifdef __AVX512VBMI__
    printf("--- AVX-512VBMI ---\n");
    test_avx512vbmi(b1, b2, bd, N);
    g_int_accumulator += bd[0];
#endif
    
#ifdef __AVX512IFMA__
    printf("--- AVX-512IFMA ---\n");
    test_avx512ifma(q1, q2, q3, qd, N);
    g_int_accumulator += qd[0];
#endif
    
#else
    printf("\n(AVX-512 not available on this system)\n");
#endif
    
    printf("\n=== Test Complete ===\n");
    printf("Float accumulator: %g\n", g_accumulator);
    printf("Int accumulator: %lld\n", (long long)g_int_accumulator);
    
    /* Cleanup */
    free(f1); free(f2); free(f3); free(fd);
    free(d1); free(d2); free(d3); free(dd);
    free(i1); free(i2); free(id); free(indices);
    free(s1); free(s2); free(sd);
    free(b1); free(b2); free(bd);
    free(q1); free(q2); free(q3); free(qd);
    free(u1); free(u2);
    
    return 0;
}
