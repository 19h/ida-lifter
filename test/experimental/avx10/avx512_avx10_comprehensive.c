/*
 * Comprehensive AVX-512 / AVX10 / AVX10.2 Instruction Coverage
 * For binary analysis, lifter testing, and ISA coverage verification
 * 
 * Covers:
 *   AVX-512F, AVX-512CD, AVX-512BW, AVX-512DQ, AVX-512VL
 *   AVX-512IFMA, AVX-512VBMI, AVX-512VBMI2, AVX-512VNNI
 *   AVX-512BITALG, AVX-512VPOPCNTDQ, AVX-512VP2INTERSECT
 *   AVX-512FP16, AVX-512BF16, AVX-512GFNI, AVX-512VAES, AVX-512VPCLMULQDQ
 *   AVX10.1, AVX10.2 (new saturation, minmax, comparison, media instructions)
 */

#include <immintrin.h>
#include <x86intrin.h>  /* For _m_prefetch, _m_prefetchw, etc. */

/* Freestanding - no libc, define our own types */
typedef unsigned char      uint8_t;
typedef signed char        int8_t;
typedef unsigned short     uint16_t;
typedef signed short       int16_t;
typedef unsigned int       uint32_t;
typedef signed int         int32_t;
typedef unsigned long long uint64_t;
typedef signed long long   int64_t;

/* Prevent dead code elimination */
#define SINK_512(x) do { volatile __m512i _sink = (x); (void)_sink; } while(0)
#define SINK_512D(x) do { volatile __m512d _sink = (x); (void)_sink; } while(0)
#define SINK_512PS(x) do { volatile __m512 _sink = (x); (void)_sink; } while(0)
#define SINK_256(x) do { volatile __m256i _sink = (x); (void)_sink; } while(0)
#define SINK_256D(x) do { volatile __m256d _sink = (x); (void)_sink; } while(0)
#define SINK_256PS(x) do { volatile __m256 _sink = (x); (void)_sink; } while(0)
#define SINK_128(x) do { volatile __m128i _sink = (x); (void)_sink; } while(0)
#define SINK_128D(x) do { volatile __m128d _sink = (x); (void)_sink; } while(0)
#define SINK_128PS(x) do { volatile __m128 _sink = (x); (void)_sink; } while(0)
#define SINK_MASK8(x) do { volatile __mmask8 _sink = (x); (void)_sink; } while(0)
#define SINK_MASK16(x) do { volatile __mmask16 _sink = (x); (void)_sink; } while(0)
#define SINK_MASK32(x) do { volatile __mmask32 _sink = (x); (void)_sink; } while(0)
#define SINK_MASK64(x) do { volatile __mmask64 _sink = (x); (void)_sink; } while(0)
#define SINK_U64(x) do { volatile uint64_t _sink = (x); (void)_sink; } while(0)
#define SINK_U32(x) do { volatile uint32_t _sink = (x); (void)_sink; } while(0)

/* Aligned buffers for memory operations */
static __attribute__((aligned(64))) int8_t   buf_i8[128];
static __attribute__((aligned(64))) int16_t  buf_i16[64];
static __attribute__((aligned(64))) int32_t  buf_i32[32];
static __attribute__((aligned(64))) int64_t  buf_i64[16];
static __attribute__((aligned(64))) float    buf_f32[32];
static __attribute__((aligned(64))) double   buf_f64[16];
static __attribute__((aligned(64))) uint8_t  buf_u8[128];
static __attribute__((aligned(64))) uint16_t buf_u16[64];
static __attribute__((aligned(64))) uint32_t buf_u32[32];
static __attribute__((aligned(64))) uint64_t buf_u64[16];

/*===========================================================================
 * SHA + Cache Control (SHA, CLFLUSHOPT, CLWB)
 *===========================================================================*/
void test_sha_cache(void) {
    __m128i a = _mm_set1_epi32(0x01234567);
    __m128i b = _mm_set1_epi32(0x89ABCDEF);
    __m128i c = _mm_set1_epi32(0x0F0F0F0F);

    SINK_128(_mm_sha1msg1_epu32(a, b));
    SINK_128(_mm_sha1msg2_epu32(a, b));
    SINK_128(_mm_sha1nexte_epu32(a, b));
    SINK_128(_mm_sha1rnds4_epu32(a, b, 0));

    SINK_128(_mm_sha256msg1_epu32(a, b));
    SINK_128(_mm_sha256msg2_epu32(a, b));
    SINK_128(_mm_sha256rnds2_epu32(a, b, c));

    _mm_clflushopt(buf_u8);
    _mm_clwb(buf_u8);
}

/*===========================================================================
 * AVX-512F (Foundation) - 512-bit integer and floating-point operations
 *===========================================================================*/
void test_avx512f(void) {
    __m512i a = _mm512_set1_epi32(0x12345678);
    __m512i b = _mm512_set1_epi32(0x87654321);
    __m512i c = _mm512_setzero_si512();
    __m512d ad = _mm512_set1_pd(3.14159265358979);
    __m512d bd = _mm512_set1_pd(2.71828182845904);
    __m512 af = _mm512_set1_ps(1.41421356f);
    __m512 bf = _mm512_set1_ps(1.73205080f);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* Integer arithmetic */
    SINK_512(_mm512_add_epi32(a, b));
    SINK_512(_mm512_add_epi64(a, b));
    SINK_512(_mm512_sub_epi32(a, b));
    SINK_512(_mm512_sub_epi64(a, b));
    SINK_512(_mm512_mullo_epi32(a, b));
    SINK_512(_mm512_mullo_epi64(a, b));
    /* Note: _mm512_mullhi_epi32 and _mm512_mulhi_epu32 don't exist in AVX-512 */
    SINK_512(_mm512_mul_epi32(a, b));
    SINK_512(_mm512_mul_epu32(a, b));

    /* Integer logical */
    SINK_512(_mm512_and_si512(a, b));
    SINK_512(_mm512_andnot_si512(a, b));
    SINK_512(_mm512_or_si512(a, b));
    SINK_512(_mm512_xor_si512(a, b));

    /* Integer shifts */
    SINK_512(_mm512_slli_epi32(a, 5));
    SINK_512(_mm512_slli_epi64(a, 7));
    SINK_512(_mm512_srli_epi32(a, 5));
    SINK_512(_mm512_srli_epi64(a, 7));
    SINK_512(_mm512_srai_epi32(a, 5));
    SINK_512(_mm512_srai_epi64(a, 7));
    SINK_512(_mm512_sllv_epi32(a, b));
    SINK_512(_mm512_sllv_epi64(a, b));
    SINK_512(_mm512_srlv_epi32(a, b));
    SINK_512(_mm512_srlv_epi64(a, b));
    SINK_512(_mm512_srav_epi32(a, b));
    SINK_512(_mm512_srav_epi64(a, b));

    /* Integer min/max */
    SINK_512(_mm512_min_epi32(a, b));
    SINK_512(_mm512_min_epu32(a, b));
    SINK_512(_mm512_min_epi64(a, b));
    SINK_512(_mm512_min_epu64(a, b));
    SINK_512(_mm512_max_epi32(a, b));
    SINK_512(_mm512_max_epu32(a, b));
    SINK_512(_mm512_max_epi64(a, b));
    SINK_512(_mm512_max_epu64(a, b));

    /* Integer absolute value */
    SINK_512(_mm512_abs_epi32(a));
    SINK_512(_mm512_abs_epi64(a));

    /* Integer comparisons */
    SINK_MASK16(_mm512_cmpeq_epi32_mask(a, b));
    SINK_MASK16(_mm512_cmpgt_epi32_mask(a, b));
    SINK_MASK16(_mm512_cmplt_epi32_mask(a, b));
    SINK_MASK16(_mm512_cmpge_epi32_mask(a, b));
    SINK_MASK16(_mm512_cmple_epi32_mask(a, b));
    SINK_MASK16(_mm512_cmpneq_epi32_mask(a, b));
    SINK_MASK8(_mm512_cmpeq_epi64_mask(a, b));
    SINK_MASK8(_mm512_cmpgt_epi64_mask(a, b));

    /* Masked integer operations */
    SINK_512(_mm512_mask_add_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_add_epi32(k16, a, b));
    SINK_512(_mm512_mask_sub_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_sub_epi32(k16, a, b));
    SINK_512(_mm512_mask_mullo_epi32(c, k16, a, b));

    /* Broadcast */
    SINK_512(_mm512_broadcastd_epi32(_mm_set1_epi32(42)));
    SINK_512(_mm512_broadcastq_epi64(_mm_set1_epi64x(42)));
    SINK_512D(_mm512_broadcastsd_pd(_mm_set1_pd(3.14)));
    SINK_512PS(_mm512_broadcastss_ps(_mm_set1_ps(2.71f)));

    /* Permutations and shuffles */
    SINK_512(_mm512_permutex_epi64(a, 0x39));
    SINK_512(_mm512_permutexvar_epi32(b, a));
    SINK_512(_mm512_permutexvar_epi64(b, a));
    SINK_512(_mm512_shuffle_epi32(a, _MM_SHUFFLE(3,1,2,0)));
    SINK_512D(_mm512_shuffle_pd(ad, bd, 0xAA));
    SINK_512PS(_mm512_shuffle_ps(af, bf, _MM_SHUFFLE(2,0,2,0)));
    SINK_512(_mm512_unpackhi_epi32(a, b));
    SINK_512(_mm512_unpacklo_epi32(a, b));
    SINK_512(_mm512_unpackhi_epi64(a, b));
    SINK_512(_mm512_unpacklo_epi64(a, b));

    /* Blends */
    SINK_512(_mm512_mask_blend_epi32(k16, a, b));
    SINK_512(_mm512_mask_blend_epi64(k8, a, b));
    SINK_512D(_mm512_mask_blend_pd(k8, ad, bd));
    SINK_512PS(_mm512_mask_blend_ps(k16, af, bf));

    /* Compress/expand */
    SINK_512(_mm512_mask_compress_epi32(c, k16, a));
    SINK_512(_mm512_maskz_compress_epi32(k16, a));
    SINK_512(_mm512_mask_compress_epi64(c, k8, a));
    SINK_512(_mm512_mask_expand_epi32(c, k16, a));
    SINK_512(_mm512_maskz_expand_epi32(k16, a));
    SINK_512(_mm512_mask_expand_epi64(c, k8, a));

    /* Gather/scatter with indices */
    __m512i idx = _mm512_set_epi32(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
    SINK_512(_mm512_i32gather_epi32(idx, buf_i32, 4));
    SINK_512(_mm512_mask_i32gather_epi32(c, k16, idx, buf_i32, 4));
    _mm512_i32scatter_epi32(buf_i32, idx, a, 4);
    _mm512_mask_i32scatter_epi32(buf_i32, k16, idx, a, 4);
    
    __m512i idx64 = _mm512_set_epi64(7,6,5,4,3,2,1,0);
    SINK_512(_mm512_i64gather_epi64(idx64, buf_i64, 8));
    _mm512_i64scatter_epi64(buf_i64, idx64, a, 8);

    /* Conversions */
    SINK_512(_mm512_cvtepi8_epi32(_mm_loadu_si128((__m128i*)buf_i8)));
    SINK_512(_mm512_cvtepi8_epi64(_mm_loadl_epi64((__m128i*)buf_i8)));
    SINK_512(_mm512_cvtepi16_epi32(_mm256_loadu_si256((__m256i*)buf_i16)));
    SINK_512(_mm512_cvtepi16_epi64(_mm_loadu_si128((__m128i*)buf_i16)));
    SINK_512(_mm512_cvtepi32_epi64(_mm256_loadu_si256((__m256i*)buf_i32)));
    SINK_512(_mm512_cvtepu8_epi32(_mm_loadu_si128((__m128i*)buf_u8)));
    SINK_512(_mm512_cvtepu8_epi64(_mm_loadl_epi64((__m128i*)buf_u8)));
    SINK_512(_mm512_cvtepu16_epi32(_mm256_loadu_si256((__m256i*)buf_u16)));
    SINK_512(_mm512_cvtepu16_epi64(_mm_loadu_si128((__m128i*)buf_u16)));
    SINK_512(_mm512_cvtepu32_epi64(_mm256_loadu_si256((__m256i*)buf_u32)));

    /* Floating-point conversions */
    SINK_512(_mm512_cvtps_epi32(af));
    SINK_512(_mm512_cvttps_epi32(af));
    SINK_512(_mm512_cvtps_epu32(af));
    SINK_512(_mm512_cvttps_epu32(af));
    SINK_512PS(_mm512_cvtepi32_ps(a));
    SINK_512PS(_mm512_cvtepu32_ps(a));
    SINK_256(_mm512_cvtpd_epi32(ad));
    SINK_256(_mm512_cvttpd_epi32(ad));
    SINK_256(_mm512_cvtpd_epu32(ad));
    SINK_256(_mm512_cvttpd_epu32(ad));
    SINK_512D(_mm512_cvtepi32_pd(_mm256_loadu_si256((__m256i*)buf_i32)));
    SINK_512D(_mm512_cvtepu32_pd(_mm256_loadu_si256((__m256i*)buf_u32)));
    SINK_512D(_mm512_cvtps_pd(_mm256_loadu_ps(buf_f32)));
    SINK_256PS(_mm512_cvtpd_ps(ad));

    /* Floating-point arithmetic */
    SINK_512PS(_mm512_add_ps(af, bf));
    SINK_512D(_mm512_add_pd(ad, bd));
    SINK_512PS(_mm512_sub_ps(af, bf));
    SINK_512D(_mm512_sub_pd(ad, bd));
    SINK_512PS(_mm512_mul_ps(af, bf));
    SINK_512D(_mm512_mul_pd(ad, bd));
    SINK_512PS(_mm512_div_ps(af, bf));
    SINK_512D(_mm512_div_pd(ad, bd));
    SINK_512PS(_mm512_sqrt_ps(af));
    SINK_512D(_mm512_sqrt_pd(ad));
    SINK_512PS(_mm512_rsqrt14_ps(af));
    SINK_512D(_mm512_rsqrt14_pd(ad));
    SINK_512PS(_mm512_rcp14_ps(af));
    SINK_512D(_mm512_rcp14_pd(ad));

    /* FMA operations */
    SINK_512PS(_mm512_fmadd_ps(af, bf, af));
    SINK_512D(_mm512_fmadd_pd(ad, bd, ad));
    SINK_512PS(_mm512_fmsub_ps(af, bf, af));
    SINK_512D(_mm512_fmsub_pd(ad, bd, ad));
    SINK_512PS(_mm512_fnmadd_ps(af, bf, af));
    SINK_512D(_mm512_fnmadd_pd(ad, bd, ad));
    SINK_512PS(_mm512_fnmsub_ps(af, bf, af));
    SINK_512D(_mm512_fnmsub_pd(ad, bd, ad));
    SINK_512PS(_mm512_fmaddsub_ps(af, bf, af));
    SINK_512D(_mm512_fmaddsub_pd(ad, bd, ad));
    SINK_512PS(_mm512_fmsubadd_ps(af, bf, af));
    SINK_512D(_mm512_fmsubadd_pd(ad, bd, ad));

    /* Floating-point min/max */
    SINK_512PS(_mm512_min_ps(af, bf));
    SINK_512D(_mm512_min_pd(ad, bd));
    SINK_512PS(_mm512_max_ps(af, bf));
    SINK_512D(_mm512_max_pd(ad, bd));

    /* Floating-point comparisons */
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_EQ_OQ));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_LT_OS));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_LE_OS));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_UNORD_Q));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_NEQ_UQ));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_NLT_US));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_NLE_US));
    SINK_MASK16(_mm512_cmp_ps_mask(af, bf, _CMP_ORD_Q));
    SINK_MASK8(_mm512_cmp_pd_mask(ad, bd, _CMP_EQ_OQ));
    SINK_MASK8(_mm512_cmp_pd_mask(ad, bd, _CMP_LT_OS));

    /* Rounding */
    SINK_512PS(_mm512_roundscale_ps(af, _MM_FROUND_TO_NEAREST_INT));
    SINK_512D(_mm512_roundscale_pd(ad, _MM_FROUND_TO_NEAREST_INT));
    SINK_512PS(_mm512_floor_ps(af));
    SINK_512D(_mm512_floor_pd(ad));
    SINK_512PS(_mm512_ceil_ps(af));
    SINK_512D(_mm512_ceil_pd(ad));

    /* Reductions */
    SINK_U32(_mm512_reduce_add_epi32(a));
    SINK_U64(_mm512_reduce_add_epi64(a));
    SINK_U32(_mm512_reduce_mul_epi32(a));
    SINK_U32(_mm512_reduce_and_epi32(a));
    SINK_U32(_mm512_reduce_or_epi32(a));
    SINK_U32(_mm512_reduce_min_epi32(a));
    SINK_U32(_mm512_reduce_max_epi32(a));
    SINK_U32(_mm512_reduce_min_epu32(a));
    SINK_U32(_mm512_reduce_max_epu32(a));

    /* Memory operations */
    SINK_512(_mm512_load_si512(buf_i32));
    SINK_512(_mm512_loadu_si512(buf_i32));
    SINK_512(_mm512_stream_load_si512(buf_i32));
    _mm512_store_si512(buf_i32, a);
    _mm512_storeu_si512(buf_i32, a);
    _mm512_stream_si512((__m512i*)buf_i32, a);
    SINK_512D(_mm512_load_pd(buf_f64));
    SINK_512D(_mm512_loadu_pd(buf_f64));
    SINK_512PS(_mm512_load_ps(buf_f32));
    SINK_512PS(_mm512_loadu_ps(buf_f32));
    _mm512_store_pd(buf_f64, ad);
    _mm512_store_ps(buf_f32, af);

    /* Masked loads/stores */
    SINK_512(_mm512_mask_load_epi32(c, k16, buf_i32));
    SINK_512(_mm512_maskz_load_epi32(k16, buf_i32));
    _mm512_mask_store_epi32(buf_i32, k16, a);
    SINK_512(_mm512_mask_loadu_epi32(c, k16, buf_i32));
    SINK_512(_mm512_maskz_loadu_epi32(k16, buf_i32));
    _mm512_mask_storeu_epi32(buf_i32, k16, a);

    /* Ternary logic (VPTERNLOGD/Q) */
    SINK_512(_mm512_ternarylogic_epi32(a, b, c, 0x96)); /* XOR3 */
    SINK_512(_mm512_ternarylogic_epi32(a, b, c, 0xE8)); /* MAJ */
    SINK_512(_mm512_ternarylogic_epi32(a, b, c, 0xCA)); /* MUX */
    SINK_512(_mm512_ternarylogic_epi64(a, b, c, 0x96));
    SINK_512(_mm512_mask_ternarylogic_epi32(a, k16, b, c, 0x96));
    SINK_512(_mm512_maskz_ternarylogic_epi32(k16, a, b, c, 0x96));

    /* Rotate */
    SINK_512(_mm512_rol_epi32(a, 7));
    SINK_512(_mm512_rol_epi64(a, 13));
    SINK_512(_mm512_ror_epi32(a, 7));
    SINK_512(_mm512_ror_epi64(a, 13));
    SINK_512(_mm512_rolv_epi32(a, b));
    SINK_512(_mm512_rolv_epi64(a, b));
    SINK_512(_mm512_rorv_epi32(a, b));
    SINK_512(_mm512_rorv_epi64(a, b));

    /* Align */
    SINK_512(_mm512_alignr_epi32(a, b, 3));
    SINK_512(_mm512_alignr_epi64(a, b, 2));

    /* Extract/insert */
    SINK_256(_mm512_extracti32x8_epi32(a, 1));
    SINK_256(_mm512_extracti64x4_epi64(a, 1));
    SINK_128(_mm512_extracti32x4_epi32(a, 2));
    SINK_512(_mm512_inserti32x8(a, _mm256_setzero_si256(), 1));
    SINK_512(_mm512_inserti64x4(a, _mm256_setzero_si256(), 1));
    SINK_512(_mm512_inserti32x4(a, _mm_setzero_si128(), 2));

    /* Getexp/getmant */
    SINK_512PS(_mm512_getexp_ps(af));
    SINK_512D(_mm512_getexp_pd(ad));
    SINK_512PS(_mm512_getmant_ps(af, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));
    SINK_512D(_mm512_getmant_pd(ad, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));

    /* Fixup */
    SINK_512PS(_mm512_fixupimm_ps(af, bf, a, 0));
    SINK_512D(_mm512_fixupimm_pd(ad, bd, a, 0));

    /* Scale */
    SINK_512PS(_mm512_scalef_ps(af, bf));
    SINK_512D(_mm512_scalef_pd(ad, bd));

    /* Range */
    SINK_512PS(_mm512_range_ps(af, bf, 0));
    SINK_512D(_mm512_range_pd(ad, bd, 0));

    /* Reduce */
    SINK_512PS(_mm512_reduce_ps(af, 0));
    SINK_512D(_mm512_reduce_pd(ad, 0));

    /* Test/testn */
    SINK_MASK16(_mm512_test_epi32_mask(a, b));
    SINK_MASK8(_mm512_test_epi64_mask(a, b));
    SINK_MASK16(_mm512_testn_epi32_mask(a, b));
    SINK_MASK8(_mm512_testn_epi64_mask(a, b));

    /* KAND/KANDN/KOR/KXOR/KNOT mask operations */
    __mmask16 m1 = 0xF0F0, m2 = 0x0FF0;
    SINK_MASK16(_kand_mask16(m1, m2));
    SINK_MASK16(_kandn_mask16(m1, m2));
    SINK_MASK16(_kor_mask16(m1, m2));
    SINK_MASK16(_kxor_mask16(m1, m2));
    SINK_MASK16(_knot_mask16(m1));
    SINK_MASK16(_kxnor_mask16(m1, m2));
    SINK_U32(_cvtmask16_u32(m1));
    SINK_MASK16(_cvtu32_mask16(0xAAAA));
    SINK_MASK16(_kadd_mask16(m1, m2));
    SINK_U32(_kortestc_mask16_u8(m1, m2));
    SINK_U32(_kortestz_mask16_u8(m1, m2));

    /* Move mask to/from GP registers */
    SINK_U32(_mm512_movepi32_mask(a));
    SINK_512(_mm512_movm_epi32(k16));
    SINK_U32(_mm512_movepi64_mask(a));
    SINK_512(_mm512_movm_epi64(k8));
}

/*===========================================================================
 * AVX-512CD (Conflict Detection)
 *===========================================================================*/
void test_avx512cd(void) {
    __m512i a = _mm512_set_epi32(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
    __m512i b = _mm512_set_epi64(7,6,5,4,3,2,1,0);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* Conflict detection */
    SINK_512(_mm512_conflict_epi32(a));
    SINK_512(_mm512_conflict_epi64(b));
    SINK_512(_mm512_mask_conflict_epi32(a, k16, a));
    SINK_512(_mm512_maskz_conflict_epi32(k16, a));
    SINK_512(_mm512_mask_conflict_epi64(b, k8, b));
    SINK_512(_mm512_maskz_conflict_epi64(k8, b));

    /* Leading zero count */
    SINK_512(_mm512_lzcnt_epi32(a));
    SINK_512(_mm512_lzcnt_epi64(b));
    SINK_512(_mm512_mask_lzcnt_epi32(a, k16, a));
    SINK_512(_mm512_maskz_lzcnt_epi32(k16, a));
    SINK_512(_mm512_mask_lzcnt_epi64(b, k8, b));
    SINK_512(_mm512_maskz_lzcnt_epi64(k8, b));

    /* Broadcast mask to vector */
    SINK_512(_mm512_broadcastmb_epi64(k8));
    SINK_512(_mm512_broadcastmw_epi32(k16));
}

/*===========================================================================
 * AVX-512BW (Byte/Word operations)
 *===========================================================================*/
void test_avx512bw(void) {
    __m512i a = _mm512_set1_epi8(0x5A);
    __m512i b = _mm512_set1_epi8(0xA5);
    __m512i c = _mm512_setzero_si512();
    __m512i w = _mm512_set1_epi16(0x1234);
    __m512i x = _mm512_set1_epi16(0x5678);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;

    /* Byte arithmetic */
    SINK_512(_mm512_add_epi8(a, b));
    SINK_512(_mm512_adds_epi8(a, b));
    SINK_512(_mm512_adds_epu8(a, b));
    SINK_512(_mm512_sub_epi8(a, b));
    SINK_512(_mm512_subs_epi8(a, b));
    SINK_512(_mm512_subs_epu8(a, b));
    SINK_512(_mm512_avg_epu8(a, b));

    /* Word arithmetic */
    SINK_512(_mm512_add_epi16(w, x));
    SINK_512(_mm512_adds_epi16(w, x));
    SINK_512(_mm512_adds_epu16(w, x));
    SINK_512(_mm512_sub_epi16(w, x));
    SINK_512(_mm512_subs_epi16(w, x));
    SINK_512(_mm512_subs_epu16(w, x));
    SINK_512(_mm512_avg_epu16(w, x));
    SINK_512(_mm512_mullo_epi16(w, x));
    SINK_512(_mm512_mulhi_epi16(w, x));
    SINK_512(_mm512_mulhi_epu16(w, x));
    SINK_512(_mm512_mulhrs_epi16(w, x));
    SINK_512(_mm512_madd_epi16(w, x));
    SINK_512(_mm512_maddubs_epi16(a, b));

    /* Byte/word min/max */
    SINK_512(_mm512_min_epi8(a, b));
    SINK_512(_mm512_min_epu8(a, b));
    SINK_512(_mm512_max_epi8(a, b));
    SINK_512(_mm512_max_epu8(a, b));
    SINK_512(_mm512_min_epi16(w, x));
    SINK_512(_mm512_min_epu16(w, x));
    SINK_512(_mm512_max_epi16(w, x));
    SINK_512(_mm512_max_epu16(w, x));

    /* Byte/word abs */
    SINK_512(_mm512_abs_epi8(a));
    SINK_512(_mm512_abs_epi16(w));

    /* Byte/word comparisons */
    SINK_MASK64(_mm512_cmpeq_epi8_mask(a, b));
    SINK_MASK64(_mm512_cmpgt_epi8_mask(a, b));
    SINK_MASK64(_mm512_cmplt_epi8_mask(a, b));
    SINK_MASK64(_mm512_cmpge_epi8_mask(a, b));
    SINK_MASK64(_mm512_cmple_epi8_mask(a, b));
    SINK_MASK64(_mm512_cmpneq_epi8_mask(a, b));
    SINK_MASK32(_mm512_cmpeq_epi16_mask(w, x));
    SINK_MASK32(_mm512_cmpgt_epi16_mask(w, x));

    /* Word shifts */
    SINK_512(_mm512_slli_epi16(w, 3));
    SINK_512(_mm512_srli_epi16(w, 3));
    SINK_512(_mm512_srai_epi16(w, 3));
    SINK_512(_mm512_sllv_epi16(w, x));
    SINK_512(_mm512_srlv_epi16(w, x));
    SINK_512(_mm512_srav_epi16(w, x));

    /* Packs */
    SINK_512(_mm512_packs_epi16(w, x));
    SINK_512(_mm512_packus_epi16(w, x));
    SINK_512(_mm512_packs_epi32(w, x));
    SINK_512(_mm512_packus_epi32(w, x));

    /* Unpack */
    SINK_512(_mm512_unpackhi_epi8(a, b));
    SINK_512(_mm512_unpacklo_epi8(a, b));
    SINK_512(_mm512_unpackhi_epi16(w, x));
    SINK_512(_mm512_unpacklo_epi16(w, x));

    /* Shuffle bytes */
    SINK_512(_mm512_shuffle_epi8(a, b));

    /* Permutations */
    SINK_512(_mm512_permutexvar_epi16(x, w));
    SINK_512(_mm512_permutex2var_epi16(w, x, w));

    /* Blend */
    SINK_512(_mm512_mask_blend_epi8(k64, a, b));
    SINK_512(_mm512_mask_blend_epi16(k32, w, x));

    /* Masked operations */
    SINK_512(_mm512_mask_add_epi8(c, k64, a, b));
    SINK_512(_mm512_maskz_add_epi8(k64, a, b));
    SINK_512(_mm512_mask_add_epi16(c, k32, w, x));
    SINK_512(_mm512_maskz_add_epi16(k32, w, x));

    /* Conversions */
    SINK_512(_mm512_cvtepi8_epi16(_mm256_loadu_si256((__m256i*)buf_i8)));
    SINK_512(_mm512_cvtepu8_epi16(_mm256_loadu_si256((__m256i*)buf_u8)));
    SINK_256(_mm512_cvtepi16_epi8(w));
    SINK_256(_mm512_cvtsepi16_epi8(w));
    SINK_256(_mm512_cvtusepi16_epi8(w));

    /* Move mask */
    SINK_U64(_mm512_movepi8_mask(a));
    SINK_U32(_mm512_movepi16_mask(w));
    SINK_512(_mm512_movm_epi8(k64));
    SINK_512(_mm512_movm_epi16(k32));

    /* DBPSADBW */
    SINK_512(_mm512_dbsad_epu8(a, b, 0));

    /* Alignr for bytes */
    SINK_512(_mm512_alignr_epi8(a, b, 5));

    /* 64-bit mask operations */
    __mmask64 m1 = 0xF0F0F0F0F0F0F0F0ULL, m2 = 0x0FF00FF00FF00FF0ULL;
    SINK_MASK64(_kand_mask64(m1, m2));
    SINK_MASK64(_kandn_mask64(m1, m2));
    SINK_MASK64(_kor_mask64(m1, m2));
    SINK_MASK64(_kxor_mask64(m1, m2));
    SINK_MASK64(_knot_mask64(m1));
    SINK_U64(_cvtmask64_u64(m1));
    SINK_MASK64(_cvtu64_mask64(0xAAAAAAAAAAAAAAAAULL));

    /* 32-bit mask operations */
    __mmask32 n1 = 0xF0F0F0F0, n2 = 0x0FF00FF0;
    SINK_MASK32(_kand_mask32(n1, n2));
    SINK_MASK32(_kandn_mask32(n1, n2));
    SINK_MASK32(_kor_mask32(n1, n2));
    SINK_MASK32(_kxor_mask32(n1, n2));
    SINK_MASK32(_knot_mask32(n1));
}

/*===========================================================================
 * AVX-512DQ (Doubleword/Quadword operations)
 *===========================================================================*/
void test_avx512dq(void) {
    __m512i a = _mm512_set1_epi64(0x123456789ABCDEF0ULL);
    __m512i b = _mm512_set1_epi64(0x0FEDCBA987654321ULL);
    __m512d ad = _mm512_set1_pd(3.14159265358979);
    __m512d bd = _mm512_set1_pd(2.71828182845904);
    __m512 af = _mm512_set1_ps(1.41421356f);
    __m512 bf = _mm512_set1_ps(1.73205080f);
    __mmask8 k8 = 0x55;

    /* 64-bit multiply */
    SINK_512(_mm512_mullo_epi64(a, b));
    SINK_512(_mm512_mask_mullo_epi64(a, k8, a, b));
    SINK_512(_mm512_maskz_mullo_epi64(k8, a, b));

    /* Broadcast from memory */
    SINK_512D(_mm512_broadcast_f64x2(_mm_load_pd(buf_f64)));
    SINK_512PS(_mm512_broadcast_f32x2(_mm_load_ps(buf_f32)));
    SINK_512PS(_mm512_broadcast_f32x8(_mm256_load_ps(buf_f32)));
    SINK_512D(_mm512_broadcast_f64x4(_mm256_load_pd(buf_f64)));
    SINK_512(_mm512_broadcast_i64x2(_mm_load_si128((__m128i*)buf_i64)));
    SINK_512(_mm512_broadcast_i32x2(_mm_load_si128((__m128i*)buf_i32)));
    SINK_512(_mm512_broadcast_i32x8(_mm256_load_si256((__m256i*)buf_i32)));
    SINK_512(_mm512_broadcast_i64x4(_mm256_load_si256((__m256i*)buf_i64)));

    /* Extract/insert 256-bit */
    SINK_128D(_mm512_extractf64x2_pd(ad, 2));
    SINK_128PS(_mm512_extractf32x4_ps(af, 2));
    SINK_512D(_mm512_insertf64x2(ad, _mm_setzero_pd(), 2));
    SINK_512PS(_mm512_insertf32x4(af, _mm_setzero_ps(), 2));

    /* Convert with 64-bit integers */
    SINK_512D(_mm512_cvtepi64_pd(a));
    SINK_512D(_mm512_cvtepu64_pd(a));
    SINK_512(_mm512_cvtpd_epi64(ad));
    SINK_512(_mm512_cvtpd_epu64(ad));
    SINK_512(_mm512_cvttpd_epi64(ad));
    SINK_512(_mm512_cvttpd_epu64(ad));
    SINK_256PS(_mm512_cvtepi64_ps(a));
    SINK_256PS(_mm512_cvtepu64_ps(a));
    SINK_512(_mm512_cvtps_epi64(_mm256_load_ps(buf_f32)));
    SINK_512(_mm512_cvtps_epu64(_mm256_load_ps(buf_f32)));
    SINK_512(_mm512_cvttps_epi64(_mm256_load_ps(buf_f32)));
    SINK_512(_mm512_cvttps_epu64(_mm256_load_ps(buf_f32)));

    /* Floating-point AND/OR/XOR/ANDN */
    SINK_512PS(_mm512_and_ps(af, bf));
    SINK_512D(_mm512_and_pd(ad, bd));
    SINK_512PS(_mm512_andnot_ps(af, bf));
    SINK_512D(_mm512_andnot_pd(ad, bd));
    SINK_512PS(_mm512_or_ps(af, bf));
    SINK_512D(_mm512_or_pd(ad, bd));
    SINK_512PS(_mm512_xor_ps(af, bf));
    SINK_512D(_mm512_xor_pd(ad, bd));

    /* FPCLASS */
    SINK_MASK16(_mm512_fpclass_ps_mask(af, 0x18)); /* Check for +/- infinity */
    SINK_MASK8(_mm512_fpclass_pd_mask(ad, 0x18));
    SINK_MASK16(_mm512_mask_fpclass_ps_mask(0xFF, af, 0x18));
    SINK_MASK8(_mm512_mask_fpclass_pd_mask(0x0F, ad, 0x18));

    /* Range with saturation */
    SINK_512PS(_mm512_range_ps(af, bf, 0x02)); /* abs max */
    SINK_512D(_mm512_range_pd(ad, bd, 0x02));
    SINK_512PS(_mm512_mask_range_ps(af, (__mmask16)k8, af, bf, 0x02));
    SINK_512D(_mm512_mask_range_pd(ad, k8, ad, bd, 0x02));

    /* Reduce operations */
    SINK_512PS(_mm512_reduce_ps(af, 0x04));
    SINK_512D(_mm512_reduce_pd(ad, 0x04));

    /* CVTQQ2PS / CVTQQ2PD (aliases) */
    SINK_256PS(_mm512_cvtepi64_ps(a));
    SINK_512D(_mm512_cvtepi64_pd(a));
}

/*===========================================================================
 * AVX-512VL (Vector Length Extensions - 128/256-bit)
 *===========================================================================*/
void test_avx512vl(void) {
    __m256i a256 = _mm256_set1_epi32(0x12345678);
    __m256i b256 = _mm256_set1_epi32(0x87654321);
    __m256d ad256 = _mm256_set1_pd(3.14159265358979);
    __m256d bd256 = _mm256_set1_pd(2.71828182845904);
    __m256 af256 = _mm256_set1_ps(1.41421356f);
    __m256 bf256 = _mm256_set1_ps(1.73205080f);
    __m128i a128 = _mm_set1_epi32(0x12345678);
    __m128i b128 = _mm_set1_epi32(0x87654321);
    __m128d ad128 = _mm_set1_pd(3.14159265358979);
    __m128d bd128 = _mm_set1_pd(2.71828182845904);
    __m128 af128 = _mm_set1_ps(1.41421356f);
    __m128 bf128 = _mm_set1_ps(1.73205080f);
    __mmask8 k8 = 0x55;
    __mmask16 k16 = 0xAAAA;

    /* 256-bit masked operations */
    SINK_256(_mm256_mask_add_epi32(a256, k8, a256, b256));
    SINK_256(_mm256_maskz_add_epi32(k8, a256, b256));
    SINK_256(_mm256_mask_sub_epi32(a256, k8, a256, b256));
    SINK_256(_mm256_mask_mullo_epi32(a256, k8, a256, b256));
    SINK_256(_mm256_mask_and_epi32(a256, k8, a256, b256));
    SINK_256(_mm256_mask_or_epi32(a256, k8, a256, b256));
    SINK_256(_mm256_mask_xor_epi32(a256, k8, a256, b256));

    /* 256-bit comparisons with mask */
    SINK_MASK8(_mm256_cmpeq_epi32_mask(a256, b256));
    SINK_MASK8(_mm256_cmpgt_epi32_mask(a256, b256));
    SINK_MASK8(_mm256_mask_cmpeq_epi32_mask(k8, a256, b256));

    /* 256-bit floating-point masked */
    SINK_256PS(_mm256_mask_add_ps(af256, k8, af256, bf256));
    SINK_256PS(_mm256_maskz_add_ps(k8, af256, bf256));
    SINK_256D(_mm256_mask_add_pd(ad256, (__mmask8)(k8 & 0x0F), ad256, bd256));
    SINK_256D(_mm256_maskz_add_pd((__mmask8)(k8 & 0x0F), ad256, bd256));
    SINK_MASK8(_mm256_cmp_ps_mask(af256, bf256, _CMP_EQ_OQ));
    SINK_MASK8(_mm256_cmp_pd_mask(ad256, bd256, _CMP_EQ_OQ));

    /* 256-bit ternary logic */
    SINK_256(_mm256_ternarylogic_epi32(a256, b256, a256, 0x96));
    SINK_256(_mm256_mask_ternarylogic_epi32(a256, k8, b256, a256, 0x96));
    SINK_256(_mm256_maskz_ternarylogic_epi32(k8, a256, b256, a256, 0x96));

    /* 256-bit blend with mask */
    SINK_256(_mm256_mask_blend_epi32(k8, a256, b256));
    SINK_256PS(_mm256_mask_blend_ps(k8, af256, bf256));
    SINK_256D(_mm256_mask_blend_pd((__mmask8)(k8 & 0x0F), ad256, bd256));

    /* 256-bit permutations */
    SINK_256(_mm256_permutexvar_epi32(b256, a256));
    SINK_256(_mm256_permutex2var_epi32(a256, b256, a256));
    SINK_256(_mm256_mask_permutexvar_epi32(a256, k8, b256, a256));

    /* 256-bit compress/expand */
    SINK_256(_mm256_mask_compress_epi32(a256, k8, b256));
    SINK_256(_mm256_maskz_compress_epi32(k8, a256));
    SINK_256(_mm256_mask_expand_epi32(a256, k8, b256));
    SINK_256(_mm256_maskz_expand_epi32(k8, a256));

    /* 256-bit rotate */
    SINK_256(_mm256_rol_epi32(a256, 7));
    SINK_256(_mm256_ror_epi32(a256, 7));
    SINK_256(_mm256_rolv_epi32(a256, b256));
    SINK_256(_mm256_rorv_epi32(a256, b256));

    /* 256-bit conflict detection */
    SINK_256(_mm256_conflict_epi32(a256));
    SINK_256(_mm256_lzcnt_epi32(a256));

    /* 128-bit masked operations */
    SINK_128(_mm_mask_add_epi32(a128, (__mmask8)(k8 & 0x0F), a128, b128));
    SINK_128(_mm_maskz_add_epi32((__mmask8)(k8 & 0x0F), a128, b128));
    SINK_128(_mm_mask_sub_epi32(a128, (__mmask8)(k8 & 0x0F), a128, b128));
    SINK_128(_mm_mask_mullo_epi32(a128, (__mmask8)(k8 & 0x0F), a128, b128));

    /* 128-bit comparisons with mask */
    SINK_MASK8(_mm_cmpeq_epi32_mask(a128, b128));
    SINK_MASK8(_mm_cmpgt_epi32_mask(a128, b128));

    /* 128-bit floating-point masked */
    SINK_128PS(_mm_mask_add_ps(af128, (__mmask8)(k8 & 0x0F), af128, bf128));
    SINK_128PS(_mm_maskz_add_ps((__mmask8)(k8 & 0x0F), af128, bf128));
    SINK_128D(_mm_mask_add_pd(ad128, (__mmask8)(k8 & 0x03), ad128, bd128));
    SINK_MASK8(_mm_cmp_ps_mask(af128, bf128, _CMP_EQ_OQ));
    SINK_MASK8(_mm_cmp_pd_mask(ad128, bd128, _CMP_EQ_OQ));

    /* 128-bit ternary logic */
    SINK_128(_mm_ternarylogic_epi32(a128, b128, a128, 0x96));

    /* 128-bit blend with mask */
    SINK_128(_mm_mask_blend_epi32((__mmask8)(k8 & 0x0F), a128, b128));
    SINK_128PS(_mm_mask_blend_ps((__mmask8)(k8 & 0x0F), af128, bf128));
    SINK_128D(_mm_mask_blend_pd((__mmask8)(k8 & 0x03), ad128, bd128));

    /* 128-bit rotate */
    SINK_128(_mm_rol_epi32(a128, 7));
    SINK_128(_mm_ror_epi32(a128, 7));
    SINK_128(_mm_rolv_epi32(a128, b128));
    SINK_128(_mm_rorv_epi32(a128, b128));

    /* 128-bit conflict detection */
    SINK_128(_mm_conflict_epi32(a128));
    SINK_128(_mm_lzcnt_epi32(a128));

    /* Broadcast scalar to VL vectors */
    SINK_256(_mm256_mask_set1_epi32(a256, k8, 42));
    SINK_256(_mm256_maskz_set1_epi32(k8, 42));
    SINK_128(_mm_mask_set1_epi32(a128, (__mmask8)(k8 & 0x0F), 42));
}

/*===========================================================================
 * AVX-512IFMA (Integer Fused Multiply-Add) - 52-bit
 *===========================================================================*/
void test_avx512ifma(void) {
    __m512i a = _mm512_set1_epi64(0x000FFFFFFFFFFFFFULL);
    __m512i b = _mm512_set1_epi64(0x000FFFFFFFFFFFFFULL);
    __m512i c = _mm512_set1_epi64(0x0000000000000001ULL);
    __m256i a256 = _mm256_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m256i b256 = _mm256_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m256i c256 = _mm256_set1_epi64x(0x0000000000000001ULL);
    __m128i a128 = _mm_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m128i b128 = _mm_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m128i c128 = _mm_set1_epi64x(0x0000000000000001ULL);
    __mmask8 k8 = 0x55;

    /* 512-bit IFMA52 */
    SINK_512(_mm512_madd52lo_epu64(c, a, b));
    SINK_512(_mm512_madd52hi_epu64(c, a, b));
    SINK_512(_mm512_mask_madd52lo_epu64(c, k8, a, b));
    SINK_512(_mm512_maskz_madd52lo_epu64(k8, c, a, b));
    SINK_512(_mm512_mask_madd52hi_epu64(c, k8, a, b));
    SINK_512(_mm512_maskz_madd52hi_epu64(k8, c, a, b));

    /* 256-bit IFMA52 (VL) */
    SINK_256(_mm256_madd52lo_epu64(c256, a256, b256));
    SINK_256(_mm256_madd52hi_epu64(c256, a256, b256));
    SINK_256(_mm256_mask_madd52lo_epu64(c256, (__mmask8)(k8 & 0x0F), a256, b256));
    SINK_256(_mm256_maskz_madd52lo_epu64((__mmask8)(k8 & 0x0F), c256, a256, b256));

    /* 128-bit IFMA52 (VL) */
    SINK_128(_mm_madd52lo_epu64(c128, a128, b128));
    SINK_128(_mm_madd52hi_epu64(c128, a128, b128));
    SINK_128(_mm_mask_madd52lo_epu64(c128, (__mmask8)(k8 & 0x03), a128, b128));
    SINK_128(_mm_maskz_madd52lo_epu64((__mmask8)(k8 & 0x03), c128, a128, b128));
}

/*===========================================================================
 * AVX-512VBMI (Vector Byte Manipulation Instructions)
 *===========================================================================*/
void test_avx512vbmi(void) {
    __m512i a = _mm512_set1_epi8(0x5A);
    __m512i b = _mm512_set1_epi8(0xA5);
    __m512i idx = _mm512_set_epi8(
        63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,
        47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,
        31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,
        15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
    );
    __m256i a256 = _mm256_set1_epi8(0x5A);
    __m256i b256 = _mm256_set1_epi8(0xA5);
    __m256i idx256 = _mm256_set_epi8(
        31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,
        15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
    );
    __m128i a128 = _mm_set1_epi8(0x5A);
    __m128i b128 = _mm_set1_epi8(0xA5);
    __m128i idx128 = _mm_set_epi8(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;

    /* VPERMB - byte permute */
    SINK_512(_mm512_permutexvar_epi8(idx, a));
    SINK_512(_mm512_mask_permutexvar_epi8(a, k64, idx, b));
    SINK_512(_mm512_maskz_permutexvar_epi8(k64, idx, a));
    SINK_256(_mm256_permutexvar_epi8(idx256, a256));
    SINK_256(_mm256_mask_permutexvar_epi8(a256, k32, idx256, b256));
    SINK_128(_mm_permutexvar_epi8(idx128, a128));

    /* VPERMB2 - two-source byte permute */
    SINK_512(_mm512_permutex2var_epi8(a, idx, b));
    SINK_512(_mm512_mask_permutex2var_epi8(a, k64, idx, b));
    SINK_512(_mm512_maskz_permutex2var_epi8(k64, a, idx, b));
    SINK_512(_mm512_mask2_permutex2var_epi8(a, idx, k64, b));
    SINK_256(_mm256_permutex2var_epi8(a256, idx256, b256));
    SINK_128(_mm_permutex2var_epi8(a128, idx128, b128));

    /* VPERMI2B - indexed two-source byte permute */
    SINK_512(_mm512_mask2_permutex2var_epi8(a, idx, k64, b));

    /* VPMULTISHIFTQB - multishift */
    SINK_512(_mm512_multishift_epi64_epi8(a, b));
    SINK_512(_mm512_mask_multishift_epi64_epi8(a, k64, a, b));
    SINK_512(_mm512_maskz_multishift_epi64_epi8(k64, a, b));
    SINK_256(_mm256_multishift_epi64_epi8(a256, b256));
    SINK_128(_mm_multishift_epi64_epi8(a128, b128));
}

/*===========================================================================
 * AVX-512VBMI2 (Vector Byte Manipulation Instructions 2)
 *===========================================================================*/
void test_avx512vbmi2(void) {
    __m512i a = _mm512_set1_epi8(0x5A);
    __m512i b = _mm512_set1_epi8(0xA5);
    __m512i w = _mm512_set1_epi16(0x1234);
    __m512i x = _mm512_set1_epi16(0x5678);
    __m512i d = _mm512_set1_epi32(0x12345678);
    __m512i e = _mm512_set1_epi32(0x87654321);
    __m512i q = _mm512_set1_epi64(0x123456789ABCDEF0ULL);
    __m512i r = _mm512_set1_epi64(0x0FEDCBA987654321ULL);
    __m256i a256 = _mm256_set1_epi8(0x5A);
    __m256i w256 = _mm256_set1_epi16(0x1234);
    __m256i d256 = _mm256_set1_epi32(0x12345678);
    __m256i q256 = _mm256_set1_epi64x(0x123456789ABCDEF0ULL);
    __m128i a128 = _mm_set1_epi8(0x5A);
    __m128i w128 = _mm_set1_epi16(0x1234);
    __m128i d128 = _mm_set1_epi32(0x12345678);
    __m128i q128 = _mm_set1_epi64x(0x123456789ABCDEF0ULL);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VPCOMPRESSB/W - byte/word compress */
    SINK_512(_mm512_mask_compress_epi8(a, k64, b));
    SINK_512(_mm512_maskz_compress_epi8(k64, a));
    SINK_512(_mm512_mask_compress_epi16(w, k32, x));
    SINK_512(_mm512_maskz_compress_epi16(k32, w));
    SINK_256(_mm256_mask_compress_epi8(a256, k32, a256));
    SINK_256(_mm256_mask_compress_epi16(w256, k16, w256));
    SINK_128(_mm_mask_compress_epi8(a128, k16, a128));
    SINK_128(_mm_mask_compress_epi16(w128, k8, w128));

    /* VPEXPANDB/W - byte/word expand */
    SINK_512(_mm512_mask_expand_epi8(a, k64, b));
    SINK_512(_mm512_maskz_expand_epi8(k64, a));
    SINK_512(_mm512_mask_expand_epi16(w, k32, x));
    SINK_512(_mm512_maskz_expand_epi16(k32, w));
    SINK_256(_mm256_mask_expand_epi8(a256, k32, a256));
    SINK_256(_mm256_mask_expand_epi16(w256, k16, w256));
    SINK_128(_mm_mask_expand_epi8(a128, k16, a128));
    SINK_128(_mm_mask_expand_epi16(w128, k8, w128));

    /* VPSHLDV - variable shift left and merge */
    SINK_512(_mm512_shldv_epi16(w, x, w));
    SINK_512(_mm512_shldv_epi32(d, e, d));
    SINK_512(_mm512_shldv_epi64(q, r, q));
    SINK_512(_mm512_mask_shldv_epi16(w, k32, x, w));
    SINK_512(_mm512_maskz_shldv_epi16(k32, w, x, w));
    SINK_512(_mm512_mask_shldv_epi32(d, k16, e, d));
    SINK_512(_mm512_mask_shldv_epi64(q, k8, r, q));
    SINK_256(_mm256_shldv_epi16(w256, w256, w256));
    SINK_256(_mm256_shldv_epi32(d256, d256, d256));
    SINK_256(_mm256_shldv_epi64(q256, q256, q256));
    SINK_128(_mm_shldv_epi16(w128, w128, w128));
    SINK_128(_mm_shldv_epi32(d128, d128, d128));
    SINK_128(_mm_shldv_epi64(q128, q128, q128));

    /* VPSHRDV - variable shift right and merge */
    SINK_512(_mm512_shrdv_epi16(w, x, w));
    SINK_512(_mm512_shrdv_epi32(d, e, d));
    SINK_512(_mm512_shrdv_epi64(q, r, q));
    SINK_512(_mm512_mask_shrdv_epi16(w, k32, x, w));
    SINK_512(_mm512_maskz_shrdv_epi16(k32, w, x, w));
    SINK_256(_mm256_shrdv_epi16(w256, w256, w256));
    SINK_128(_mm_shrdv_epi16(w128, w128, w128));

    /* VPSHLDD/Q - immediate shift left double */
    SINK_512(_mm512_shldi_epi16(w, x, 5));
    SINK_512(_mm512_shldi_epi32(d, e, 7));
    SINK_512(_mm512_shldi_epi64(q, r, 11));
    SINK_512(_mm512_mask_shldi_epi16(w, k32, w, x, 5));
    SINK_512(_mm512_maskz_shldi_epi16(k32, w, x, 5));
    SINK_256(_mm256_shldi_epi16(w256, w256, 5));
    SINK_256(_mm256_shldi_epi32(d256, d256, 7));
    SINK_128(_mm_shldi_epi16(w128, w128, 5));
    SINK_128(_mm_shldi_epi32(d128, d128, 7));

    /* VPSHRDD/Q - immediate shift right double */
    SINK_512(_mm512_shrdi_epi16(w, x, 5));
    SINK_512(_mm512_shrdi_epi32(d, e, 7));
    SINK_512(_mm512_shrdi_epi64(q, r, 11));
    SINK_512(_mm512_mask_shrdi_epi16(w, k32, w, x, 5));
    SINK_512(_mm512_maskz_shrdi_epi16(k32, w, x, 5));
    SINK_256(_mm256_shrdi_epi16(w256, w256, 5));
    SINK_128(_mm_shrdi_epi16(w128, w128, 5));
}

/*===========================================================================
 * AVX-512VNNI (Vector Neural Network Instructions)
 *===========================================================================*/
void test_avx512vnni(void) {
    __m512i a = _mm512_set1_epi32(0x01010101);
    __m512i b = _mm512_set1_epi32(0x02020202);
    __m512i c = _mm512_set1_epi32(0x00000000);
    __m256i a256 = _mm256_set1_epi32(0x01010101);
    __m256i b256 = _mm256_set1_epi32(0x02020202);
    __m256i c256 = _mm256_set1_epi32(0x00000000);
    __m128i a128 = _mm_set1_epi32(0x01010101);
    __m128i b128 = _mm_set1_epi32(0x02020202);
    __m128i c128 = _mm_set1_epi32(0x00000000);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VPDPBUSD - multiply u8*i8, add pairs, accumulate to i32 */
    SINK_512(_mm512_dpbusd_epi32(c, a, b));
    SINK_512(_mm512_mask_dpbusd_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbusd_epi32(k16, c, a, b));
    SINK_256(_mm256_dpbusd_epi32(c256, a256, b256));
    SINK_256(_mm256_mask_dpbusd_epi32(c256, k8, a256, b256));
    SINK_128(_mm_dpbusd_epi32(c128, a128, b128));

    /* VPDPBUSDS - saturating version */
    SINK_512(_mm512_dpbusds_epi32(c, a, b));
    SINK_512(_mm512_mask_dpbusds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbusds_epi32(k16, c, a, b));
    SINK_256(_mm256_dpbusds_epi32(c256, a256, b256));
    SINK_128(_mm_dpbusds_epi32(c128, a128, b128));

    /* VPDPWSSD - multiply i16*i16, add pairs, accumulate to i32 */
    SINK_512(_mm512_dpwssd_epi32(c, a, b));
    SINK_512(_mm512_mask_dpwssd_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwssd_epi32(k16, c, a, b));
    SINK_256(_mm256_dpwssd_epi32(c256, a256, b256));
    SINK_128(_mm_dpwssd_epi32(c128, a128, b128));

    /* VPDPWSSDS - saturating version */
    SINK_512(_mm512_dpwssds_epi32(c, a, b));
    SINK_512(_mm512_mask_dpwssds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwssds_epi32(k16, c, a, b));
    SINK_256(_mm256_dpwssds_epi32(c256, a256, b256));
    SINK_128(_mm_dpwssds_epi32(c128, a128, b128));
}

/*===========================================================================
 * AVX-512BITALG (Bit Algorithms)
 *===========================================================================*/
void test_avx512bitalg(void) {
    __m512i a = _mm512_set1_epi8(0x5A);
    __m512i w = _mm512_set1_epi16(0x1234);
    __m256i a256 = _mm256_set1_epi8(0x5A);
    __m256i w256 = _mm256_set1_epi16(0x1234);
    __m128i a128 = _mm_set1_epi8(0x5A);
    __m128i w128 = _mm_set1_epi16(0x1234);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VPOPCNTB - byte popcount */
    SINK_512(_mm512_popcnt_epi8(a));
    SINK_512(_mm512_mask_popcnt_epi8(a, k64, a));
    SINK_512(_mm512_maskz_popcnt_epi8(k64, a));
    SINK_256(_mm256_popcnt_epi8(a256));
    SINK_256(_mm256_mask_popcnt_epi8(a256, k32, a256));
    SINK_128(_mm_popcnt_epi8(a128));

    /* VPOPCNTW - word popcount */
    SINK_512(_mm512_popcnt_epi16(w));
    SINK_512(_mm512_mask_popcnt_epi16(w, k32, w));
    SINK_512(_mm512_maskz_popcnt_epi16(k32, w));
    SINK_256(_mm256_popcnt_epi16(w256));
    SINK_128(_mm_popcnt_epi16(w128));

    /* VPSHUFBITQMB - shuffle bits and create mask */
    SINK_MASK64(_mm512_bitshuffle_epi64_mask(a, a));
    SINK_MASK64(_mm512_mask_bitshuffle_epi64_mask(k64, a, a));
    SINK_MASK32(_mm256_bitshuffle_epi64_mask(a256, a256));
    SINK_MASK16(_mm_bitshuffle_epi64_mask(a128, a128));
}

/*===========================================================================
 * AVX-512VPOPCNTDQ (Doubleword/Quadword Popcount)
 *===========================================================================*/
void test_avx512vpopcntdq(void) {
    __m512i a = _mm512_set1_epi32(0x12345678);
    __m512i b = _mm512_set1_epi64(0x123456789ABCDEF0ULL);
    __m256i a256 = _mm256_set1_epi32(0x12345678);
    __m256i b256 = _mm256_set1_epi64x(0x123456789ABCDEF0ULL);
    __m128i a128 = _mm_set1_epi32(0x12345678);
    __m128i b128 = _mm_set1_epi64x(0x123456789ABCDEF0ULL);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VPOPCNTD - dword popcount */
    SINK_512(_mm512_popcnt_epi32(a));
    SINK_512(_mm512_mask_popcnt_epi32(a, k16, a));
    SINK_512(_mm512_maskz_popcnt_epi32(k16, a));
    SINK_256(_mm256_popcnt_epi32(a256));
    SINK_256(_mm256_mask_popcnt_epi32(a256, k8, a256));
    SINK_128(_mm_popcnt_epi32(a128));

    /* VPOPCNTQ - qword popcount */
    SINK_512(_mm512_popcnt_epi64(b));
    SINK_512(_mm512_mask_popcnt_epi64(b, k8, b));
    SINK_512(_mm512_maskz_popcnt_epi64(k8, b));
    SINK_256(_mm256_popcnt_epi64(b256));
    SINK_128(_mm_popcnt_epi64(b128));
}

/*===========================================================================
 * AVX-512VP2INTERSECT (Vector Pair Intersection)
 *===========================================================================*/
void test_avx512vp2intersect(void) {
    __m512i a = _mm512_set_epi32(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
    __m512i b = _mm512_set_epi32(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
    __m512i c = _mm512_set_epi64(7,6,5,4,3,2,1,0);
    __m512i d = _mm512_set_epi64(0,1,2,3,4,5,6,7);
    __m256i a256 = _mm256_set_epi32(7,6,5,4,3,2,1,0);
    __m256i b256 = _mm256_set_epi32(0,1,2,3,4,5,6,7);
    __m128i a128 = _mm_set_epi32(3,2,1,0);
    __m128i b128 = _mm_set_epi32(0,1,2,3);
    __mmask16 k0_32, k1_32;
    __mmask8 k0_64, k1_64;

    /* VP2INTERSECTD - 32-bit intersection */
    _mm512_2intersect_epi32(a, b, &k0_32, &k1_32);
    SINK_MASK16(k0_32);
    SINK_MASK16(k1_32);

    /* VP2INTERSECTQ - 64-bit intersection */
    _mm512_2intersect_epi64(c, d, &k0_64, &k1_64);
    SINK_MASK8(k0_64);
    SINK_MASK8(k1_64);

    /* VL variants */
    _mm256_2intersect_epi32(a256, b256, &k0_64, &k1_64);
    SINK_MASK8(k0_64);
    SINK_MASK8(k1_64);
    
    __mmask8 k0_128_32, k1_128_32;
    _mm_2intersect_epi32(a128, b128, &k0_128_32, &k1_128_32);
    SINK_MASK8(k0_128_32);
    SINK_MASK8(k1_128_32);
}

/*===========================================================================
 * AVX-512FP16 (Half-Precision Floating-Point)
 *===========================================================================*/
#ifdef __AVX512FP16__
void test_avx512fp16(void) {
    __m512h a = _mm512_set1_ph((_Float16)1.5f);
    __m512h b = _mm512_set1_ph((_Float16)2.5f);
    __m512h c = _mm512_setzero_ph();
    __m256h a256 = _mm256_set1_ph((_Float16)1.5f);
    __m256h b256 = _mm256_set1_ph((_Float16)2.5f);
    __m128h a128 = _mm_set1_ph((_Float16)1.5f);
    __m128h b128 = _mm_set1_ph((_Float16)2.5f);
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* Basic FP16 arithmetic */
    SINK_512((__m512i)_mm512_add_ph(a, b));
    SINK_512((__m512i)_mm512_sub_ph(a, b));
    SINK_512((__m512i)_mm512_mul_ph(a, b));
    SINK_512((__m512i)_mm512_div_ph(a, b));
    SINK_512((__m512i)_mm512_sqrt_ph(a));
    SINK_512((__m512i)_mm512_rsqrt_ph(a));
    SINK_512((__m512i)_mm512_rcp_ph(a));

    /* Masked FP16 arithmetic */
    SINK_512((__m512i)_mm512_mask_add_ph(c, k32, a, b));
    SINK_512((__m512i)_mm512_maskz_add_ph(k32, a, b));
    SINK_512((__m512i)_mm512_mask_mul_ph(c, k32, a, b));

    /* FP16 FMA */
    SINK_512((__m512i)_mm512_fmadd_ph(a, b, c));
    SINK_512((__m512i)_mm512_fmsub_ph(a, b, c));
    SINK_512((__m512i)_mm512_fnmadd_ph(a, b, c));
    SINK_512((__m512i)_mm512_fnmsub_ph(a, b, c));
    SINK_512((__m512i)_mm512_fmaddsub_ph(a, b, c));
    SINK_512((__m512i)_mm512_fmsubadd_ph(a, b, c));

    /* FP16 min/max */
    SINK_512((__m512i)_mm512_min_ph(a, b));
    SINK_512((__m512i)_mm512_max_ph(a, b));

    /* FP16 comparisons */
    SINK_MASK32(_mm512_cmp_ph_mask(a, b, _CMP_EQ_OQ));
    SINK_MASK32(_mm512_cmp_ph_mask(a, b, _CMP_LT_OS));
    SINK_MASK32(_mm512_cmp_ph_mask(a, b, _CMP_LE_OS));
    SINK_MASK32(_mm512_mask_cmp_ph_mask(k32, a, b, _CMP_EQ_OQ));

    /* FP16 conversions */
    SINK_512((__m512i)_mm512_cvtepi16_ph(_mm512_set1_epi16(42)));
    SINK_512((__m512i)_mm512_cvtepu16_ph(_mm512_set1_epi16(42)));
    SINK_512(_mm512_cvtph_epi16(a));
    SINK_512(_mm512_cvtph_epu16(a));
    SINK_512(_mm512_cvttph_epi16(a));
    SINK_512(_mm512_cvttph_epu16(a));
    SINK_256((__m256i)_mm512_cvtxps_ph(_mm512_set1_ps(1.5f)));
    SINK_512PS(_mm512_cvtxph_ps(_mm256_set1_ph((_Float16)1.5f)));
    SINK_128((__m128i)_mm512_cvtpd_ph(_mm512_set1_pd(1.5)));
    SINK_512D(_mm512_cvtph_pd(_mm_set1_ph((_Float16)1.5f)));

    /* FP16 round/scale */
    SINK_512((__m512i)_mm512_roundscale_ph(a, 0));
    SINK_512((__m512i)_mm512_scalef_ph(a, b));
    SINK_512((__m512i)_mm512_getexp_ph(a));
    SINK_512((__m512i)_mm512_getmant_ph(a, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));

    /* FP16 reduce/range */
    SINK_512((__m512i)_mm512_reduce_ph(a, 0));
    /* Note: _mm512_range_ph does not exist in AVX-512FP16 */

    /* FP16 fpclass */
    SINK_MASK32(_mm512_fpclass_ph_mask(a, 0x18));

    /* VL variants - 256-bit */
    SINK_256((__m256i)_mm256_add_ph(a256, b256));
    SINK_256((__m256i)_mm256_sub_ph(a256, b256));
    SINK_256((__m256i)_mm256_mul_ph(a256, b256));
    SINK_256((__m256i)_mm256_div_ph(a256, b256));
    SINK_256((__m256i)_mm256_sqrt_ph(a256));
    SINK_256((__m256i)_mm256_fmadd_ph(a256, b256, a256));
    SINK_MASK16(_mm256_cmp_ph_mask(a256, b256, _CMP_EQ_OQ));

    /* VL variants - 128-bit */
    SINK_128((__m128i)_mm_add_ph(a128, b128));
    SINK_128((__m128i)_mm_sub_ph(a128, b128));
    SINK_128((__m128i)_mm_mul_ph(a128, b128));
    SINK_128((__m128i)_mm_div_ph(a128, b128));
    SINK_128((__m128i)_mm_sqrt_ph(a128));
    SINK_128((__m128i)_mm_fmadd_ph(a128, b128, a128));
    SINK_MASK8(_mm_cmp_ph_mask(a128, b128, _CMP_EQ_OQ));

    /* Scalar FP16 operations */
    __m128h sh_a = _mm_set_sh((_Float16)1.5f);
    __m128h sh_b = _mm_set_sh((_Float16)2.5f);
    SINK_128((__m128i)_mm_add_sh(sh_a, sh_b));
    SINK_128((__m128i)_mm_sub_sh(sh_a, sh_b));
    SINK_128((__m128i)_mm_mul_sh(sh_a, sh_b));
    SINK_128((__m128i)_mm_div_sh(sh_a, sh_b));
    SINK_128((__m128i)_mm_sqrt_sh(sh_a, sh_b));
    SINK_128((__m128i)_mm_fmadd_sh(sh_a, sh_b, sh_a));
    SINK_128((__m128i)_mm_min_sh(sh_a, sh_b));
    SINK_128((__m128i)_mm_max_sh(sh_a, sh_b));
    SINK_MASK8(_mm_cmp_sh_mask(sh_a, sh_b, _CMP_EQ_OQ));

    /* Complex FP16 - FCMUL/FCMADD */
    SINK_512((__m512i)_mm512_fcmul_pch(a, b));
    SINK_512((__m512i)_mm512_fcmadd_pch(a, b, c));
    SINK_512((__m512i)_mm512_fmul_pch(a, b));
    SINK_512((__m512i)_mm512_fmadd_pch(a, b, c));
    SINK_256((__m256i)_mm256_fcmul_pch(a256, b256));
    SINK_128((__m128i)_mm_fcmul_pch(a128, b128));
}
#else
void test_avx512fp16(void) { /* FP16 not available */ }
#endif

/*===========================================================================
 * AVX-512BF16 (Brain Float 16)
 *===========================================================================*/
void test_avx512bf16(void) {
    __m512 a = _mm512_set1_ps(1.5f);
    __m512 b = _mm512_set1_ps(2.5f);
    __m512bh abf = _mm512_cvtne2ps_pbh(a, b);
    __m256bh abf256 = _mm512_cvtneps_pbh(a);
    __m256 a256 = _mm256_set1_ps(1.5f);
    __m256 b256 = _mm256_set1_ps(2.5f);
    __m128 a128 = _mm_set1_ps(1.5f);
    __m128 b128 = _mm_set1_ps(2.5f);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* Convert FP32 to BF16 */
    SINK_512((__m512i)_mm512_cvtne2ps_pbh(a, b));
    SINK_256((__m256i)_mm512_cvtneps_pbh(a));
    SINK_512((__m512i)_mm512_mask_cvtne2ps_pbh(abf, (__mmask32)0xAAAAAAAA, a, b));
    SINK_512((__m512i)_mm512_maskz_cvtne2ps_pbh((__mmask32)0xAAAAAAAA, a, b));
    SINK_256((__m256i)_mm512_mask_cvtneps_pbh(abf256, k16, a));
    SINK_256((__m256i)_mm512_maskz_cvtneps_pbh(k16, a));

    /* VL variants */
    SINK_256((__m256i)_mm256_cvtne2ps_pbh(a256, b256));
    SINK_128((__m128i)_mm256_cvtneps_pbh(a256));
    SINK_128((__m128i)_mm_cvtne2ps_pbh(a128, b128));

    /* DPBF16PS - dot product BF16 to FP32 */
    __m512 acc = _mm512_setzero_ps();
    SINK_512PS(_mm512_dpbf16_ps(acc, abf, abf));
    SINK_512PS(_mm512_mask_dpbf16_ps(acc, k16, abf, abf));
    SINK_512PS(_mm512_maskz_dpbf16_ps(k16, acc, abf, abf));

    /* VL DPBF16 */
    __m256bh abf256_2 = _mm256_cvtne2ps_pbh(a256, b256);
    __m256 acc256 = _mm256_setzero_ps();
    SINK_256PS(_mm256_dpbf16_ps(acc256, abf256_2, abf256_2));
    SINK_256PS(_mm256_mask_dpbf16_ps(acc256, k8, abf256_2, abf256_2));

    __m128bh abf128 = _mm_cvtne2ps_pbh(a128, b128);
    __m128 acc128 = _mm_setzero_ps();
    SINK_128PS(_mm_dpbf16_ps(acc128, abf128, abf128));
}

/*===========================================================================
 * AVX-512GFNI (Galois Field New Instructions)
 *===========================================================================*/
void test_avx512gfni(void) {
    __m512i a = _mm512_set1_epi8(0x5A);
    __m512i b = _mm512_set1_epi64(0x0102040810204080ULL);
    __m256i a256 = _mm256_set1_epi8(0x5A);
    __m256i b256 = _mm256_set1_epi64x(0x0102040810204080ULL);
    __m128i a128 = _mm_set1_epi8(0x5A);
    __m128i b128 = _mm_set1_epi64x(0x0102040810204080ULL);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;

    /* GF2P8AFFINEQB - affine transformation */
    SINK_512(_mm512_gf2p8affine_epi64_epi8(a, b, 0));
    SINK_512(_mm512_mask_gf2p8affine_epi64_epi8(a, k64, a, b, 0));
    SINK_512(_mm512_maskz_gf2p8affine_epi64_epi8(k64, a, b, 0));
    SINK_256(_mm256_gf2p8affine_epi64_epi8(a256, b256, 0));
    SINK_256(_mm256_mask_gf2p8affine_epi64_epi8(a256, k32, a256, b256, 0));
    SINK_128(_mm_gf2p8affine_epi64_epi8(a128, b128, 0));
    SINK_128(_mm_mask_gf2p8affine_epi64_epi8(a128, k16, a128, b128, 0));

    /* GF2P8AFFINEINVQB - affine inverse transformation */
    SINK_512(_mm512_gf2p8affineinv_epi64_epi8(a, b, 0));
    SINK_512(_mm512_mask_gf2p8affineinv_epi64_epi8(a, k64, a, b, 0));
    SINK_512(_mm512_maskz_gf2p8affineinv_epi64_epi8(k64, a, b, 0));
    SINK_256(_mm256_gf2p8affineinv_epi64_epi8(a256, b256, 0));
    SINK_128(_mm_gf2p8affineinv_epi64_epi8(a128, b128, 0));

    /* GF2P8MULB - GF(2^8) multiplication */
    SINK_512(_mm512_gf2p8mul_epi8(a, a));
    SINK_512(_mm512_mask_gf2p8mul_epi8(a, k64, a, a));
    SINK_512(_mm512_maskz_gf2p8mul_epi8(k64, a, a));
    SINK_256(_mm256_gf2p8mul_epi8(a256, a256));
    SINK_128(_mm_gf2p8mul_epi8(a128, a128));
}

/*===========================================================================
 * AVX-512VAES (Vector AES)
 *===========================================================================*/
void test_avx512vaes(void) {
    __m512i key = _mm512_set1_epi32(0x12345678);
    __m512i data = _mm512_set1_epi32(0x87654321);
    __m256i key256 = _mm256_set1_epi32(0x12345678);
    __m256i data256 = _mm256_set1_epi32(0x87654321);

    /* VAESENC - AES encode round */
    SINK_512(_mm512_aesenc_epi128(data, key));
    SINK_256(_mm256_aesenc_epi128(data256, key256));

    /* VAESENCLAST - AES encode last round */
    SINK_512(_mm512_aesenclast_epi128(data, key));
    SINK_256(_mm256_aesenclast_epi128(data256, key256));

    /* VAESDEC - AES decode round */
    SINK_512(_mm512_aesdec_epi128(data, key));
    SINK_256(_mm256_aesdec_epi128(data256, key256));

    /* VAESDECLAST - AES decode last round */
    SINK_512(_mm512_aesdeclast_epi128(data, key));
    SINK_256(_mm256_aesdeclast_epi128(data256, key256));
}

/*===========================================================================
 * AVX-512VPCLMULQDQ (Vector Carryless Multiply)
 *===========================================================================*/
void test_avx512vpclmulqdq(void) {
    __m512i a = _mm512_set1_epi64(0x123456789ABCDEF0ULL);
    __m512i b = _mm512_set1_epi64(0x0FEDCBA987654321ULL);
    __m256i a256 = _mm256_set1_epi64x(0x123456789ABCDEF0ULL);
    __m256i b256 = _mm256_set1_epi64x(0x0FEDCBA987654321ULL);

    /* VPCLMULQDQ - carryless multiply */
    SINK_512(_mm512_clmulepi64_epi128(a, b, 0x00)); /* low * low */
    SINK_512(_mm512_clmulepi64_epi128(a, b, 0x01)); /* low * high */
    SINK_512(_mm512_clmulepi64_epi128(a, b, 0x10)); /* high * low */
    SINK_512(_mm512_clmulepi64_epi128(a, b, 0x11)); /* high * high */
    SINK_256(_mm256_clmulepi64_epi128(a256, b256, 0x00));
    SINK_256(_mm256_clmulepi64_epi128(a256, b256, 0x11));
}

/*===========================================================================
 * AVX10.2 - New Instructions (Saturating Conversions, MinMax, etc.)
 *===========================================================================*/
#ifdef __AVX10_2__

/*---------------------------------------------------------------------------
 * AVX10.2 Saturation Conversions (VCVTPS2IBS, VCVTPH2IBS, VCVTBF162IBS, etc.)
 *---------------------------------------------------------------------------*/
void test_avx10_2_saturation(void) {
    __m512 f32 = _mm512_set1_ps(1000000.0f);
    __m256 f32_256 = _mm256_set1_ps(1000000.0f);
    __m128 f32_128 = _mm_set1_ps(1000000.0f);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;
    __mmask32 k32 = 0xAAAAAAAA;

    /* 512-bit: FP32 to INT8 with saturation */
    SINK_512(_mm512_ipcvts_ps_epi8(f32));                              /* signed */
    SINK_512(_mm512_mask_ipcvts_ps_epi8(_mm512_setzero_si512(), k16, f32));
    SINK_512(_mm512_maskz_ipcvts_ps_epi8(k16, f32));
    SINK_512(_mm512_ipcvts_ps_epu8(f32));                              /* unsigned */
    SINK_512(_mm512_mask_ipcvts_ps_epu8(_mm512_setzero_si512(), k16, f32));
    SINK_512(_mm512_maskz_ipcvts_ps_epu8(k16, f32));
    SINK_512(_mm512_ipcvtts_ps_epi8(f32));                             /* truncating signed */
    SINK_512(_mm512_mask_ipcvtts_ps_epi8(_mm512_setzero_si512(), k16, f32));
    SINK_512(_mm512_maskz_ipcvtts_ps_epi8(k16, f32));
    SINK_512(_mm512_ipcvtts_ps_epu8(f32));                             /* truncating unsigned */
    SINK_512(_mm512_mask_ipcvtts_ps_epu8(_mm512_setzero_si512(), k16, f32));
    SINK_512(_mm512_maskz_ipcvtts_ps_epu8(k16, f32));

    /* 256-bit VL: FP32 to INT8 with saturation */
    SINK_256(_mm256_ipcvts_ps_epi8(f32_256));
    SINK_256(_mm256_mask_ipcvts_ps_epi8(_mm256_setzero_si256(), k8, f32_256));
    SINK_256(_mm256_maskz_ipcvts_ps_epi8(k8, f32_256));
    SINK_256(_mm256_ipcvts_ps_epu8(f32_256));
    SINK_256(_mm256_mask_ipcvts_ps_epu8(_mm256_setzero_si256(), k8, f32_256));
    SINK_256(_mm256_maskz_ipcvts_ps_epu8(k8, f32_256));
    SINK_256(_mm256_ipcvtts_ps_epi8(f32_256));
    SINK_256(_mm256_ipcvtts_ps_epu8(f32_256));

    /* 128-bit VL: FP32 to INT8 with saturation */
    SINK_128(_mm_ipcvts_ps_epi8(f32_128));
    SINK_128(_mm_mask_ipcvts_ps_epi8(_mm_setzero_si128(), (__mmask8)(k8 & 0x0F), f32_128));
    SINK_128(_mm_maskz_ipcvts_ps_epi8((__mmask8)(k8 & 0x0F), f32_128));
    SINK_128(_mm_ipcvts_ps_epu8(f32_128));
    SINK_128(_mm_ipcvtts_ps_epi8(f32_128));
    SINK_128(_mm_ipcvtts_ps_epu8(f32_128));

#ifdef __AVX512FP16__
    /* FP16 to INT8 with saturation */
    __m512h f16 = _mm512_set1_ph((_Float16)1000.0f);
    __m256h f16_256 = _mm256_set1_ph((_Float16)1000.0f);
    __m128h f16_128 = _mm_set1_ph((_Float16)1000.0f);

    SINK_512(_mm512_ipcvts_ph_epi8(f16));
    SINK_512(_mm512_mask_ipcvts_ph_epi8(_mm512_setzero_si512(), k32, f16));
    SINK_512(_mm512_maskz_ipcvts_ph_epi8(k32, f16));
    SINK_512(_mm512_ipcvts_ph_epu8(f16));
    SINK_512(_mm512_ipcvtts_ph_epi8(f16));
    SINK_512(_mm512_ipcvtts_ph_epu8(f16));

    SINK_256(_mm256_ipcvts_ph_epi8(f16_256));
    SINK_256(_mm256_ipcvts_ph_epu8(f16_256));
    SINK_256(_mm256_ipcvtts_ph_epi8(f16_256));
    SINK_256(_mm256_ipcvtts_ph_epu8(f16_256));

    SINK_128(_mm_ipcvts_ph_epi8(f16_128));
    SINK_128(_mm_ipcvts_ph_epu8(f16_128));
    SINK_128(_mm_ipcvtts_ph_epi8(f16_128));
    SINK_128(_mm_ipcvtts_ph_epu8(f16_128));
#endif

    /* BF16 to INT8 with saturation */
    __m512bh bf16 = _mm512_castps_pbh(_mm512_set1_ps(1.0f));
    __m256bh bf16_256 = _mm256_castps_pbh(_mm256_set1_ps(1.0f));
    __m128bh bf16_128 = _mm_castps_pbh(_mm_set1_ps(1.0f));

    SINK_512(_mm512_ipcvts_bf16_epi8(bf16));
    SINK_512(_mm512_mask_ipcvts_bf16_epi8(_mm512_setzero_si512(), k32, bf16));
    SINK_512(_mm512_maskz_ipcvts_bf16_epi8(k32, bf16));
    SINK_512(_mm512_ipcvts_bf16_epu8(bf16));
    SINK_512(_mm512_ipcvtts_bf16_epi8(bf16));
    SINK_512(_mm512_ipcvtts_bf16_epu8(bf16));

    SINK_256(_mm256_ipcvts_bf16_epi8(bf16_256));
    SINK_256(_mm256_ipcvts_bf16_epu8(bf16_256));
    SINK_256(_mm256_ipcvtts_bf16_epi8(bf16_256));
    SINK_256(_mm256_ipcvtts_bf16_epu8(bf16_256));

    SINK_128(_mm_ipcvts_bf16_epi8(bf16_128));
    SINK_128(_mm_ipcvts_bf16_epu8(bf16_128));
    SINK_128(_mm_ipcvtts_bf16_epi8(bf16_128));
    SINK_128(_mm_ipcvtts_bf16_epu8(bf16_128));

    /* Truncating saturation conversions for scalar FP to integer
       Note: rounding mode must be _MM_FROUND_CUR_DIRECTION for these intrinsics */
    SINK_U32(_mm_cvtts_roundss_i32(f32_128, _MM_FROUND_CUR_DIRECTION));
    SINK_U32(_mm_cvtts_roundss_u32(f32_128, _MM_FROUND_CUR_DIRECTION));
    SINK_U32(_mm_cvtts_roundsd_i32(_mm_set1_pd(1e9), _MM_FROUND_CUR_DIRECTION));
    SINK_U32(_mm_cvtts_roundsd_u32(_mm_set1_pd(1e9), _MM_FROUND_CUR_DIRECTION));
#ifdef __x86_64__
    SINK_U64(_mm_cvtts_roundss_i64(f32_128, _MM_FROUND_CUR_DIRECTION));
    SINK_U64(_mm_cvtts_roundss_u64(f32_128, _MM_FROUND_CUR_DIRECTION));
    SINK_U64(_mm_cvtts_roundsd_i64(_mm_set1_pd(1e15), _MM_FROUND_CUR_DIRECTION));
    SINK_U64(_mm_cvtts_roundsd_u64(_mm_set1_pd(1e15), _MM_FROUND_CUR_DIRECTION));
#endif

    /* Truncating saturation conversions for packed FP to integer */
    SINK_128(_mm_cvtts_ps_epi32(f32_128));
    SINK_128(_mm_cvtts_ps_epu32(f32_128));
    SINK_128(_mm_cvtts_ps_epi64(f32_128));
    SINK_128(_mm_cvtts_ps_epu64(f32_128));
}

/*---------------------------------------------------------------------------
 * AVX10.2 MinMax (VMINMAXPS, VMINMAXPD, VMINMAXSS, VMINMAXSD)
 *---------------------------------------------------------------------------*/
void test_avx10_2_minmax(void) {
    __m512 a = _mm512_set1_ps(1.5f);
    __m512 b = _mm512_set1_ps(2.5f);
    __m512d ad = _mm512_set1_pd(3.14);
    __m512d bd = _mm512_set1_pd(2.71);
    __m256 a256 = _mm256_set1_ps(1.5f);
    __m256 b256 = _mm256_set1_ps(2.5f);
    __m256d ad256 = _mm256_set1_pd(3.14);
    __m256d bd256 = _mm256_set1_pd(2.71);
    __m128 a128 = _mm_set1_ps(1.5f);
    __m128 b128 = _mm_set1_ps(2.5f);
    __m128d ad128 = _mm_set1_pd(3.14);
    __m128d bd128 = _mm_set1_pd(2.71);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* 512-bit VMINMAXPS - min/max with selection control (imm8[1:0]):
       0x00 = min(a,b), 0x01 = max(a,b), 0x02 = min_abs(a,b), 0x03 = max_abs(a,b)
       imm8[4:2] controls sign behavior */
    SINK_512PS(_mm512_minmax_ps(a, b, 0x00));
    SINK_512PS(_mm512_minmax_ps(a, b, 0x01));
    SINK_512PS(_mm512_minmax_ps(a, b, 0x02));
    SINK_512PS(_mm512_minmax_ps(a, b, 0x03));
    SINK_512PS(_mm512_minmax_ps(a, b, 0x10)); /* with sign src control */
    SINK_512PS(_mm512_mask_minmax_ps(a, k16, a, b, 0x00));
    SINK_512PS(_mm512_maskz_minmax_ps(k16, a, b, 0x00));
    SINK_512PS(_mm512_mask_minmax_ps(a, k16, a, b, 0x01));
    SINK_512PS(_mm512_maskz_minmax_ps(k16, a, b, 0x01));

    /* 512-bit VMINMAXPD - double precision */
    SINK_512D(_mm512_minmax_pd(ad, bd, 0x00));
    SINK_512D(_mm512_minmax_pd(ad, bd, 0x01));
    SINK_512D(_mm512_minmax_pd(ad, bd, 0x02));
    SINK_512D(_mm512_minmax_pd(ad, bd, 0x03));
    SINK_512D(_mm512_mask_minmax_pd(ad, k8, ad, bd, 0x00));
    SINK_512D(_mm512_maskz_minmax_pd(k8, ad, bd, 0x00));

    /* 256-bit VL variants */
    SINK_256PS(_mm256_minmax_ps(a256, b256, 0x00));
    SINK_256PS(_mm256_minmax_ps(a256, b256, 0x01));
    SINK_256PS(_mm256_minmax_ps(a256, b256, 0x02));
    SINK_256PS(_mm256_minmax_ps(a256, b256, 0x03));
    SINK_256PS(_mm256_mask_minmax_ps(a256, k8, a256, b256, 0x00));
    SINK_256PS(_mm256_maskz_minmax_ps(k8, a256, b256, 0x00));
    SINK_256D(_mm256_minmax_pd(ad256, bd256, 0x00));
    SINK_256D(_mm256_minmax_pd(ad256, bd256, 0x01));
    SINK_256D(_mm256_mask_minmax_pd(ad256, (__mmask8)(k8 & 0x0F), ad256, bd256, 0x00));

    /* 128-bit VL variants */
    SINK_128PS(_mm_minmax_ps(a128, b128, 0x00));
    SINK_128PS(_mm_minmax_ps(a128, b128, 0x01));
    SINK_128PS(_mm_minmax_ps(a128, b128, 0x02));
    SINK_128PS(_mm_minmax_ps(a128, b128, 0x03));
    SINK_128PS(_mm_mask_minmax_ps(a128, (__mmask8)(k8 & 0x0F), a128, b128, 0x00));
    SINK_128PS(_mm_maskz_minmax_ps((__mmask8)(k8 & 0x0F), a128, b128, 0x00));
    SINK_128D(_mm_minmax_pd(ad128, bd128, 0x00));
    SINK_128D(_mm_minmax_pd(ad128, bd128, 0x01));
    SINK_128D(_mm_mask_minmax_pd(ad128, (__mmask8)(k8 & 0x03), ad128, bd128, 0x00));

    /* Scalar VMINMAXSS / VMINMAXSD */
    SINK_128PS(_mm_minmax_ss(a128, b128, 0x00));
    SINK_128PS(_mm_minmax_ss(a128, b128, 0x01));
    SINK_128PS(_mm_minmax_ss(a128, b128, 0x02));
    SINK_128PS(_mm_minmax_ss(a128, b128, 0x03));
    SINK_128PS(_mm_mask_minmax_ss(a128, (__mmask8)0x01, a128, b128, 0x00));
    SINK_128PS(_mm_maskz_minmax_ss((__mmask8)0x01, a128, b128, 0x00));
    SINK_128D(_mm_minmax_sd(ad128, bd128, 0x00));
    SINK_128D(_mm_minmax_sd(ad128, bd128, 0x01));
    SINK_128D(_mm_mask_minmax_sd(ad128, (__mmask8)0x01, ad128, bd128, 0x00));
    SINK_128D(_mm_maskz_minmax_sd((__mmask8)0x01, ad128, bd128, 0x00));

#ifdef __AVX512FP16__
    /* FP16 VMINMAXPH / VMINMAXSH */
    __m512h h512a = _mm512_set1_ph((_Float16)1.5f);
    __m512h h512b = _mm512_set1_ph((_Float16)2.5f);
    __m256h h256a = _mm256_set1_ph((_Float16)1.5f);
    __m256h h256b = _mm256_set1_ph((_Float16)2.5f);
    __m128h h128a = _mm_set1_ph((_Float16)1.5f);
    __m128h h128b = _mm_set1_ph((_Float16)2.5f);
    __mmask32 k32 = 0xAAAAAAAA;

    SINK_512((__m512i)_mm512_minmax_ph(h512a, h512b, 0x00));
    SINK_512((__m512i)_mm512_minmax_ph(h512a, h512b, 0x01));
    SINK_512((__m512i)_mm512_mask_minmax_ph(h512a, k32, h512a, h512b, 0x00));
    SINK_512((__m512i)_mm512_maskz_minmax_ph(k32, h512a, h512b, 0x00));
    SINK_256((__m256i)_mm256_minmax_ph(h256a, h256b, 0x00));
    SINK_256((__m256i)_mm256_minmax_ph(h256a, h256b, 0x01));
    SINK_128((__m128i)_mm_minmax_ph(h128a, h128b, 0x00));
    SINK_128((__m128i)_mm_minmax_sh(h128a, h128b, 0x00));
    SINK_128((__m128i)_mm_minmax_sh(h128a, h128b, 0x01));
#endif
}

/*---------------------------------------------------------------------------
 * AVX10.2 BF16 Compare (VCOMSBF16)
 *---------------------------------------------------------------------------*/
void test_avx10_2_compare(void) {
    __m128bh bf_a = _mm_castps_pbh(_mm_set1_ps(1.0f));
    __m128bh bf_b = _mm_castps_pbh(_mm_set1_ps(2.0f));

    /* Scalar BF16 comparisons */
    SINK_U32(_mm_comieq_sbh(bf_a, bf_b));
    SINK_U32(_mm_comilt_sbh(bf_a, bf_b));
    SINK_U32(_mm_comile_sbh(bf_a, bf_b));
    SINK_U32(_mm_comigt_sbh(bf_a, bf_b));
    SINK_U32(_mm_comige_sbh(bf_a, bf_b));
    SINK_U32(_mm_comineq_sbh(bf_a, bf_b));
}

/*---------------------------------------------------------------------------
 * AVX10.2 Media (VMPSADBW, YMM/ZMM extensions)
 *---------------------------------------------------------------------------*/
void test_avx10_2_media(void) {
    __m512i a = _mm512_set1_epi8(0x5A);
    __m512i b = _mm512_set1_epi8(0xA5);
    __m256i a256 = _mm256_set1_epi8(0x5A);
    __m256i b256 = _mm256_set1_epi8(0xA5);
    __m128i a128 = _mm_set1_epi8(0x5A);
    __m128i b128 = _mm_set1_epi8(0xA5);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VMPSADBW - 512-bit (new in AVX10.2) */
    SINK_512(_mm512_mpsadbw_epu8(a, b, 0x00));
    SINK_512(_mm512_mpsadbw_epu8(a, b, 0x07));
    SINK_512(_mm512_mpsadbw_epu8(a, b, 0x3F));
    SINK_512(_mm512_mask_mpsadbw_epu8(a, k64, a, b, 0x00));
    SINK_512(_mm512_maskz_mpsadbw_epu8(k64, a, b, 0x00));

    /* VMPSADBW - 256-bit with masking (extended in AVX10.2) */
    SINK_256(_mm256_mask_mpsadbw_epu8(a256, k32, a256, b256, 0x00));
    SINK_256(_mm256_maskz_mpsadbw_epu8(k32, a256, b256, 0x00));

    /* VMPSADBW - 128-bit with masking (extended in AVX10.2) */
    SINK_128(_mm_mask_mpsadbw_epu8(a128, k16, a128, b128, 0x00));
    SINK_128(_mm_maskz_mpsadbw_epu8(k16, a128, b128, 0x00));

    /* Extended copy/move instructions */
    SINK_128(_mm_move_epi32(a128));
    SINK_128(_mm_move_epi16(a128));
}

/*---------------------------------------------------------------------------
 * AVX10.2 BF16 Arithmetic (VADDNEPBF16, VSUBNEPBF16, VMULNEPBF16, etc.)
 *---------------------------------------------------------------------------*/
void test_avx10_2_bf16_enhanced(void) {
    __m512bh a512 = _mm512_castps_pbh(_mm512_set1_ps(1.5f));
    __m512bh b512 = _mm512_castps_pbh(_mm512_set1_ps(2.5f));
    __m512bh c512 = _mm512_setzero_pbh();
    __m256bh a256 = _mm256_castps_pbh(_mm256_set1_ps(1.5f));
    __m256bh b256 = _mm256_castps_pbh(_mm256_set1_ps(2.5f));
    __m256bh c256 = _mm256_setzero_pbh();
    __m128bh a128 = _mm_castps_pbh(_mm_set1_ps(1.5f));
    __m128bh b128 = _mm_castps_pbh(_mm_set1_ps(2.5f));
    __m128bh c128 = _mm_setzero_pbh();
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* 512-bit BF16 arithmetic */
    SINK_512((__m512i)_mm512_add_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_add_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_add_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_sub_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_sub_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_sub_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_mul_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_mul_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_mul_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_div_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_div_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_div_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_sqrt_pbh(a512));
    SINK_512((__m512i)_mm512_mask_sqrt_pbh(c512, k32, a512));
    SINK_512((__m512i)_mm512_maskz_sqrt_pbh(k32, a512));

    /* 512-bit BF16 min/max/abs */
    SINK_512((__m512i)_mm512_min_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_min_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_min_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_max_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_max_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_max_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_abs_pbh(a512));

    /* 512-bit BF16 FMA */
    SINK_512((__m512i)_mm512_fmadd_pbh(a512, b512, c512));
    SINK_512((__m512i)_mm512_mask_fmadd_pbh(a512, k32, b512, c512));
    SINK_512((__m512i)_mm512_mask3_fmadd_pbh(a512, b512, c512, k32));
    SINK_512((__m512i)_mm512_maskz_fmadd_pbh(k32, a512, b512, c512));
    SINK_512((__m512i)_mm512_fmsub_pbh(a512, b512, c512));
    SINK_512((__m512i)_mm512_mask_fmsub_pbh(a512, k32, b512, c512));
    SINK_512((__m512i)_mm512_mask3_fmsub_pbh(a512, b512, c512, k32));
    SINK_512((__m512i)_mm512_maskz_fmsub_pbh(k32, a512, b512, c512));
    SINK_512((__m512i)_mm512_fnmadd_pbh(a512, b512, c512));
    SINK_512((__m512i)_mm512_mask_fnmadd_pbh(a512, k32, b512, c512));
    SINK_512((__m512i)_mm512_mask3_fnmadd_pbh(a512, b512, c512, k32));
    SINK_512((__m512i)_mm512_maskz_fnmadd_pbh(k32, a512, b512, c512));
    SINK_512((__m512i)_mm512_fnmsub_pbh(a512, b512, c512));
    SINK_512((__m512i)_mm512_mask_fnmsub_pbh(a512, k32, b512, c512));
    SINK_512((__m512i)_mm512_mask3_fnmsub_pbh(a512, b512, c512, k32));
    SINK_512((__m512i)_mm512_maskz_fnmsub_pbh(k32, a512, b512, c512));

    /* 512-bit BF16 special functions */
    SINK_512((__m512i)_mm512_rcp_pbh(a512));
    SINK_512((__m512i)_mm512_mask_rcp_pbh(c512, k32, a512));
    SINK_512((__m512i)_mm512_maskz_rcp_pbh(k32, a512));
    SINK_512((__m512i)_mm512_rsqrt_pbh(a512));
    SINK_512((__m512i)_mm512_mask_rsqrt_pbh(c512, k32, a512));
    SINK_512((__m512i)_mm512_maskz_rsqrt_pbh(k32, a512));
    SINK_512((__m512i)_mm512_getexp_pbh(a512));
    SINK_512((__m512i)_mm512_mask_getexp_pbh(c512, k32, a512));
    SINK_512((__m512i)_mm512_maskz_getexp_pbh(k32, a512));
    SINK_512((__m512i)_mm512_scalef_pbh(a512, b512));
    SINK_512((__m512i)_mm512_mask_scalef_pbh(c512, k32, a512, b512));
    SINK_512((__m512i)_mm512_maskz_scalef_pbh(k32, a512, b512));
    SINK_512((__m512i)_mm512_reduce_pbh(a512, 0));
    SINK_512((__m512i)_mm512_mask_reduce_pbh(c512, k32, a512, 0));
    SINK_512((__m512i)_mm512_maskz_reduce_pbh(k32, a512, 0));
    SINK_512((__m512i)_mm512_roundscale_pbh(a512, 0));
    SINK_512((__m512i)_mm512_mask_roundscale_pbh(c512, k32, a512, 0));
    SINK_512((__m512i)_mm512_maskz_roundscale_pbh(k32, a512, 0));
    SINK_512((__m512i)_mm512_getmant_pbh(a512, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));
    SINK_512((__m512i)_mm512_mask_getmant_pbh(c512, k32, a512, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));
    SINK_512((__m512i)_mm512_maskz_getmant_pbh(k32, a512, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));

    /* 512-bit BF16 compare and fpclass */
    SINK_MASK32(_mm512_cmp_pbh_mask(a512, b512, _CMP_EQ_OQ));
    SINK_MASK32(_mm512_cmp_pbh_mask(a512, b512, _CMP_LT_OS));
    SINK_MASK32(_mm512_cmp_pbh_mask(a512, b512, _CMP_LE_OS));
    SINK_MASK32(_mm512_cmp_pbh_mask(a512, b512, _CMP_NEQ_UQ));
    SINK_MASK32(_mm512_mask_cmp_pbh_mask(k32, a512, b512, _CMP_EQ_OQ));
    SINK_MASK32(_mm512_fpclass_pbh_mask(a512, 0x18));
    SINK_MASK32(_mm512_mask_fpclass_pbh_mask(k32, a512, 0x18));

    /* 256-bit BF16 arithmetic */
    SINK_256((__m256i)_mm256_add_pbh(a256, b256));
    SINK_256((__m256i)_mm256_mask_add_pbh(c256, k16, a256, b256));
    SINK_256((__m256i)_mm256_maskz_add_pbh(k16, a256, b256));
    SINK_256((__m256i)_mm256_sub_pbh(a256, b256));
    SINK_256((__m256i)_mm256_mul_pbh(a256, b256));
    SINK_256((__m256i)_mm256_div_pbh(a256, b256));
    SINK_256((__m256i)_mm256_sqrt_pbh(a256));
    SINK_256((__m256i)_mm256_min_pbh(a256, b256));
    SINK_256((__m256i)_mm256_max_pbh(a256, b256));
    SINK_256((__m256i)_mm256_abs_pbh(a256));
    SINK_256((__m256i)_mm256_fmadd_pbh(a256, b256, c256));
    SINK_256((__m256i)_mm256_mask_fmadd_pbh(a256, k16, b256, c256));
    SINK_256((__m256i)_mm256_fmsub_pbh(a256, b256, c256));
    SINK_256((__m256i)_mm256_fnmadd_pbh(a256, b256, c256));
    SINK_256((__m256i)_mm256_fnmsub_pbh(a256, b256, c256));
    SINK_256((__m256i)_mm256_rcp_pbh(a256));
    SINK_256((__m256i)_mm256_rsqrt_pbh(a256));
    SINK_256((__m256i)_mm256_getexp_pbh(a256));
    SINK_256((__m256i)_mm256_scalef_pbh(a256, b256));
    SINK_256((__m256i)_mm256_reduce_pbh(a256, 0));
    SINK_256((__m256i)_mm256_roundscale_pbh(a256, 0));
    SINK_256((__m256i)_mm256_getmant_pbh(a256, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));
    SINK_MASK16(_mm256_cmp_pbh_mask(a256, b256, _CMP_EQ_OQ));
    SINK_MASK16(_mm256_fpclass_pbh_mask(a256, 0x18));

    /* 128-bit BF16 arithmetic */
    SINK_128((__m128i)_mm_add_pbh(a128, b128));
    SINK_128((__m128i)_mm_mask_add_pbh(c128, k8, a128, b128));
    SINK_128((__m128i)_mm_maskz_add_pbh(k8, a128, b128));
    SINK_128((__m128i)_mm_sub_pbh(a128, b128));
    SINK_128((__m128i)_mm_mul_pbh(a128, b128));
    SINK_128((__m128i)_mm_div_pbh(a128, b128));
    SINK_128((__m128i)_mm_sqrt_pbh(a128));
    SINK_128((__m128i)_mm_min_pbh(a128, b128));
    SINK_128((__m128i)_mm_max_pbh(a128, b128));
    SINK_128((__m128i)_mm_abs_pbh(a128));
    SINK_128((__m128i)_mm_fmadd_pbh(a128, b128, c128));
    SINK_128((__m128i)_mm_fmsub_pbh(a128, b128, c128));
    SINK_128((__m128i)_mm_fnmadd_pbh(a128, b128, c128));
    SINK_128((__m128i)_mm_fnmsub_pbh(a128, b128, c128));
    SINK_128((__m128i)_mm_rcp_pbh(a128));
    SINK_128((__m128i)_mm_rsqrt_pbh(a128));
    SINK_128((__m128i)_mm_getexp_pbh(a128));
    SINK_128((__m128i)_mm_scalef_pbh(a128, b128));
    SINK_128((__m128i)_mm_reduce_pbh(a128, 0));
    SINK_128((__m128i)_mm_roundscale_pbh(a128, 0));
    SINK_128((__m128i)_mm_getmant_pbh(a128, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));
    SINK_MASK8(_mm_cmp_pbh_mask(a128, b128, _CMP_EQ_OQ));
    SINK_MASK8(_mm_fpclass_pbh_mask(a128, 0x18));

    /* BF16 load/store */
    _mm512_store_pbh((void*)buf_u16, a512);
    _mm512_storeu_pbh((void*)buf_u16, a512);
    _mm256_store_pbh((void*)buf_u16, a256);
    _mm256_storeu_pbh((void*)buf_u16, a256);
    _mm_store_pbh((void*)buf_u16, a128);
    _mm_storeu_pbh((void*)buf_u16, a128);
}

/*---------------------------------------------------------------------------
 * AVX10.2 New Dot Product Instructions (DPBSSD, DPBSUD, DPBUUD, etc.)
 *---------------------------------------------------------------------------*/
void test_avx10_2_dotproduct(void) {
    __m512i a = _mm512_set1_epi8(0x7F);
    __m512i b = _mm512_set1_epi8(0x01);
    __m512i c = _mm512_setzero_si512();
    __m256i a256 = _mm256_set1_epi8(0x7F);
    __m256i b256 = _mm256_set1_epi8(0x01);
    __m256i c256 = _mm256_setzero_si256();
    __m128i a128 = _mm_set1_epi8(0x7F);
    __m128i b128 = _mm_set1_epi8(0x01);
    __m128i c128 = _mm_setzero_si128();
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* 512-bit VDPBSSD - signed*signed to dword */
    SINK_512(_mm512_dpbssd_epi32(c, a, b));
    SINK_512(_mm512_mask_dpbssd_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbssd_epi32(k16, c, a, b));
    SINK_512(_mm512_dpbssds_epi32(c, a, b));  /* saturating */
    SINK_512(_mm512_mask_dpbssds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbssds_epi32(k16, c, a, b));

    /* 512-bit VDPBSUD - signed*unsigned to dword */
    SINK_512(_mm512_dpbsud_epi32(c, a, b));
    SINK_512(_mm512_mask_dpbsud_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbsud_epi32(k16, c, a, b));
    SINK_512(_mm512_dpbsuds_epi32(c, a, b));  /* saturating */
    SINK_512(_mm512_mask_dpbsuds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbsuds_epi32(k16, c, a, b));

    /* 512-bit VDPBUUD - unsigned*unsigned to dword */
    SINK_512(_mm512_dpbuud_epi32(c, a, b));
    SINK_512(_mm512_mask_dpbuud_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbuud_epi32(k16, c, a, b));
    SINK_512(_mm512_dpbuuds_epi32(c, a, b));  /* saturating */
    SINK_512(_mm512_mask_dpbuuds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpbuuds_epi32(k16, c, a, b));

    /* 512-bit VDPWSUD - word signed*unsigned to dword */
    SINK_512(_mm512_dpwsud_epi32(c, a, b));
    SINK_512(_mm512_mask_dpwsud_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwsud_epi32(c, k16, a, b));
    SINK_512(_mm512_dpwsuds_epi32(c, a, b));  /* saturating */
    SINK_512(_mm512_mask_dpwsuds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwsuds_epi32(c, k16, a, b));

    /* 512-bit VDPWUSD - word unsigned*signed to dword */
    SINK_512(_mm512_dpwusd_epi32(c, a, b));
    SINK_512(_mm512_mask_dpwusd_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwusd_epi32(c, k16, a, b));
    SINK_512(_mm512_dpwusds_epi32(c, a, b));  /* saturating */
    SINK_512(_mm512_mask_dpwusds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwusds_epi32(c, k16, a, b));

    /* 512-bit VDPWUUD - word unsigned*unsigned to dword */
    SINK_512(_mm512_dpwuud_epi32(c, a, b));
    SINK_512(_mm512_mask_dpwuud_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwuud_epi32(c, k16, a, b));
    SINK_512(_mm512_dpwuuds_epi32(c, a, b));  /* saturating */
    SINK_512(_mm512_mask_dpwuuds_epi32(c, k16, a, b));
    SINK_512(_mm512_maskz_dpwuuds_epi32(c, k16, a, b));

    /* 512-bit VDPPHPS - FP16 dot product to FP32 */
    __m512h h512a = _mm512_set1_ph((_Float16)1.5f);
    __m512h h512b = _mm512_set1_ph((_Float16)2.5f);
    __m512 f512 = _mm512_setzero_ps();
    SINK_512PS(_mm512_dpph_ps(f512, h512a, h512b));
    SINK_512PS(_mm512_mask_dpph_ps(f512, k16, h512a, h512b));
    SINK_512PS(_mm512_maskz_dpph_ps(k16, f512, h512a, h512b));

    /* 256-bit variants */
    SINK_256(_mm256_dpbssd_epi32(c256, a256, b256));
    SINK_256(_mm256_mask_dpbssd_epi32(c256, k8, a256, b256));
    SINK_256(_mm256_maskz_dpbssd_epi32(k8, c256, a256, b256));
    SINK_256(_mm256_dpbssds_epi32(c256, a256, b256));
    SINK_256(_mm256_mask_dpbssds_epi32(c256, k8, a256, b256));
    SINK_256(_mm256_maskz_dpbssds_epi32(k8, c256, a256, b256));
    SINK_256(_mm256_dpbsud_epi32(c256, a256, b256));
    SINK_256(_mm256_dpbsuds_epi32(c256, a256, b256));
    SINK_256(_mm256_maskz_dpbsuds_epi32(k8, c256, a256, b256));
    SINK_256(_mm256_dpbuud_epi32(c256, a256, b256));
    SINK_256(_mm256_dpbuuds_epi32(c256, a256, b256));
    SINK_256(_mm256_maskz_dpbuuds_epi32(k8, c256, a256, b256));
    SINK_256(_mm256_dpwsud_epi32(c256, a256, b256));
    SINK_256(_mm256_dpwsuds_epi32(c256, a256, b256));
    SINK_256(_mm256_maskz_dpwsuds_epi32(c256, k8, a256, b256));
    SINK_256(_mm256_dpwusd_epi32(c256, a256, b256));
    SINK_256(_mm256_dpwusds_epi32(c256, a256, b256));
    SINK_256(_mm256_maskz_dpwusds_epi32(c256, k8, a256, b256));
    SINK_256(_mm256_dpwuud_epi32(c256, a256, b256));
    SINK_256(_mm256_dpwuuds_epi32(c256, a256, b256));
    SINK_256(_mm256_maskz_dpwuuds_epi32(c256, k8, a256, b256));

    /* 256-bit VDPPHPS - FP16 dot product to FP32 */
    __m256h h256a = _mm256_set1_ph((_Float16)1.5f);
    __m256h h256b = _mm256_set1_ph((_Float16)2.5f);
    __m256 f256 = _mm256_setzero_ps();
    SINK_256PS(_mm256_dpph_ps(f256, h256a, h256b));
    SINK_256PS(_mm256_mask_dpph_ps(f256, k8, h256a, h256b));
    SINK_256PS(_mm256_maskz_dpph_ps(k8, f256, h256a, h256b));

    /* 128-bit variants */
    SINK_128(_mm_dpbssd_epi32(c128, a128, b128));
    SINK_128(_mm_dpbssds_epi32(c128, a128, b128));
    SINK_128(_mm_dpbsud_epi32(c128, a128, b128));
    SINK_128(_mm_dpbsuds_epi32(c128, a128, b128));
    SINK_128(_mm_dpbuud_epi32(c128, a128, b128));
    SINK_128(_mm_dpbuuds_epi32(c128, a128, b128));
    SINK_128(_mm_dpwsud_epi32(c128, a128, b128));
    SINK_128(_mm_dpwsuds_epi32(c128, a128, b128));
    SINK_128(_mm_dpwusd_epi32(c128, a128, b128));
    SINK_128(_mm_dpwusds_epi32(c128, a128, b128));
    SINK_128(_mm_dpwuud_epi32(c128, a128, b128));
    SINK_128(_mm_dpwuuds_epi32(c128, a128, b128));

    /* 128-bit VDPPHPS - FP16 dot product to FP32 */
    __m128h h128a = _mm_set1_ph((_Float16)1.5f);
    __m128h h128b = _mm_set1_ph((_Float16)2.5f);
    __m128 f128 = _mm_setzero_ps();
    SINK_128PS(_mm_dpph_ps(f128, h128a, h128b));
    SINK_128PS(_mm_mask_dpph_ps(f128, (__mmask8)(k8 & 0x0F), h128a, h128b));
    SINK_128PS(_mm_maskz_dpph_ps((__mmask8)(k8 & 0x0F), f128, h128a, h128b));
}

/*---------------------------------------------------------------------------
 * AVX10.2 FP8 Conversions (VCVT2PH2BF8, VCVT2PH2HF8, etc.)
 *---------------------------------------------------------------------------*/
void test_avx10_2_fp8_convert(void) {
#ifdef __AVX512FP16__
    __m512h h512a = _mm512_set1_ph((_Float16)1.5f);
    __m512h h512b = _mm512_set1_ph((_Float16)2.5f);
    __m256h h256a = _mm256_set1_ph((_Float16)1.5f);
    __m256h h256b = _mm256_set1_ph((_Float16)2.5f);
    __m128h h128a = _mm_set1_ph((_Float16)1.5f);
    __m128h h128b = _mm_set1_ph((_Float16)2.5f);
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;

    /* 512-bit: FP16 pair to BF8/HF8 */
    SINK_512(_mm512_cvt2ph_bf8(h512a, h512b));
    SINK_512(_mm512_mask_cvt2ph_bf8(_mm512_setzero_si512(), k64, h512a, h512b));
    SINK_512(_mm512_maskz_cvt2ph_bf8(k64, h512a, h512b));
    SINK_512(_mm512_cvts_2ph_bf8(h512a, h512b));  /* saturating */
    SINK_512(_mm512_cvt2ph_hf8(h512a, h512b));
    SINK_512(_mm512_mask_cvt2ph_hf8(_mm512_setzero_si512(), k64, h512a, h512b));
    SINK_512(_mm512_maskz_cvt2ph_hf8(k64, h512a, h512b));
    SINK_512(_mm512_cvts_2ph_hf8(h512a, h512b));  /* saturating */

    /* 512-bit: FP16 to BF8/HF8 (single source) */
    SINK_256(_mm512_cvtph_bf8(h512a));
    SINK_256(_mm512_mask_cvtph_bf8(_mm256_setzero_si256(), k32, h512a));
    SINK_256(_mm512_maskz_cvtph_bf8(k32, h512a));
    SINK_256(_mm512_cvts_ph_bf8(h512a));  /* saturating */
    SINK_256(_mm512_cvtph_hf8(h512a));
    SINK_256(_mm512_mask_cvtph_hf8(_mm256_setzero_si256(), k32, h512a));
    SINK_256(_mm512_maskz_cvtph_hf8(k32, h512a));
    SINK_256(_mm512_cvts_ph_hf8(h512a));  /* saturating */

    /* 512-bit: BF8/HF8 to FP16 */
    __m256i bf8_256 = _mm256_set1_epi8(0x3C);  /* BF8 representation */
    __m256i hf8_256 = _mm256_set1_epi8(0x3C);  /* HF8 representation */
    SINK_512((__m512i)_mm512_cvtbf8_ph(bf8_256));
    SINK_512((__m512i)_mm512_mask_cvtbf8_ph(_mm512_setzero_ph(), k32, bf8_256));
    SINK_512((__m512i)_mm512_maskz_cvtbf8_ph(k32, bf8_256));
    SINK_512((__m512i)_mm512_cvthf8_ph(hf8_256));
    SINK_512((__m512i)_mm512_mask_cvthf8_ph(_mm512_setzero_ph(), k32, hf8_256));
    SINK_512((__m512i)_mm512_maskz_cvthf8_ph(k32, hf8_256));

    /* 512-bit: Biased conversions */
    __m512i bias512 = _mm512_set1_epi8(0x00);
    SINK_256(_mm512_cvtbiasph_bf8(bias512, h512a));
    SINK_256(_mm512_mask_cvtbiasph_bf8(_mm256_setzero_si256(), k32, bias512, h512a));
    SINK_256(_mm512_maskz_cvtbiasph_bf8(k32, bias512, h512a));
    SINK_256(_mm512_cvts_biasph_bf8(bias512, h512a));  /* saturating */
    SINK_256(_mm512_mask_cvts_biasph_bf8(_mm256_setzero_si256(), k32, bias512, h512a));
    SINK_256(_mm512_cvtbiasph_hf8(bias512, h512a));
    SINK_256(_mm512_mask_cvtbiasph_hf8(_mm256_setzero_si256(), k32, bias512, h512a));
    SINK_256(_mm512_cvts_biasph_hf8(bias512, h512a));  /* saturating */
    SINK_256(_mm512_mask_cvts_biasph_hf8(_mm256_setzero_si256(), k32, bias512, h512a));

    /* 256-bit: FP16 pair to BF8/HF8 */
    SINK_256(_mm256_cvt2ph_bf8(h256a, h256b));
    SINK_256(_mm256_mask_cvt2ph_bf8(_mm256_setzero_si256(), k32, h256a, h256b));
    SINK_256(_mm256_maskz_cvt2ph_bf8(k32, h256a, h256b));
    SINK_256(_mm256_cvts_2ph_bf8(h256a, h256b));
    SINK_256(_mm256_cvt2ph_hf8(h256a, h256b));
    SINK_256(_mm256_cvts_2ph_hf8(h256a, h256b));

    /* 256-bit: FP16 to BF8/HF8 (single source) */
    SINK_128(_mm256_cvtph_bf8(h256a));
    SINK_128(_mm256_mask_cvtph_bf8(_mm_setzero_si128(), k16, h256a));
    SINK_128(_mm256_maskz_cvtph_bf8(k16, h256a));
    SINK_128(_mm256_cvts_ph_bf8(h256a));
    SINK_128(_mm256_cvtph_hf8(h256a));
    SINK_128(_mm256_cvts_ph_hf8(h256a));

    /* 256-bit: BF8/HF8 to FP16 */
    __m128i bf8_128 = _mm_set1_epi8(0x3C);
    __m128i hf8_128 = _mm_set1_epi8(0x3C);
    SINK_256((__m256i)_mm256_cvtbf8_ph(bf8_128));
    SINK_256((__m256i)_mm256_mask_cvtbf8_ph(_mm256_setzero_ph(), k16, bf8_128));
    SINK_256((__m256i)_mm256_maskz_cvtbf8_ph(k16, bf8_128));
    SINK_256((__m256i)_mm256_cvthf8_ph(bf8_128));

    /* 256-bit: Biased conversions */
    __m256i bias256 = _mm256_set1_epi8(0x00);
    SINK_128(_mm256_cvtbiasph_bf8(bias256, h256a));
    SINK_128(_mm256_mask_cvtbiasph_bf8(_mm_setzero_si128(), k16, bias256, h256a));
    SINK_128(_mm256_cvts_biasph_bf8(bias256, h256a));
    SINK_128(_mm256_mask_cvts_biasph_bf8(_mm_setzero_si128(), k16, bias256, h256a));
    SINK_128(_mm256_cvtbiasph_hf8(bias256, h256a));
    SINK_128(_mm256_mask_cvtbiasph_hf8(_mm_setzero_si128(), k16, bias256, h256a));
    SINK_128(_mm256_cvts_biasph_hf8(bias256, h256a));
    SINK_128(_mm256_mask_cvts_biasph_hf8(_mm_setzero_si128(), k16, bias256, h256a));

    /* 128-bit: FP16 pair to BF8/HF8 */
    SINK_128(_mm_cvt2ph_bf8(h128a, h128b));
    SINK_128(_mm_mask_cvt2ph_bf8(_mm_setzero_si128(), k16, h128a, h128b));
    SINK_128(_mm_maskz_cvt2ph_bf8(k16, h128a, h128b));
    SINK_128(_mm_cvts_2ph_bf8(h128a, h128b));
    SINK_128(_mm_cvt2ph_hf8(h128a, h128b));
    SINK_128(_mm_cvts_2ph_hf8(h128a, h128b));

    /* 128-bit: FP16 to BF8/HF8 (single source) */
    SINK_128(_mm_cvtph_bf8(h128a));
    SINK_128(_mm_cvts_ph_bf8(h128a));
    SINK_128(_mm_cvtph_hf8(h128a));
    SINK_128(_mm_cvts_ph_hf8(h128a));

    /* 128-bit: BF8/HF8 to FP16 */
    SINK_128((__m128i)_mm_cvtbf8_ph(bf8_128));
    SINK_128((__m128i)_mm_cvthf8_ph(bf8_128));

    /* 128-bit: Biased conversions */
    __m128i bias128 = _mm_set1_epi8(0x00);
    SINK_128(_mm_cvtbiasph_bf8(bias128, h128a));
    SINK_128(_mm_cvts_biasph_bf8(bias128, h128a));
    SINK_128(_mm_cvtbiasph_hf8(bias128, h128a));
    SINK_128(_mm_cvts_biasph_hf8(bias128, h128a));

    /* VCVTX2PS2PH - FP32 pair to FP16 (AVX10.2) */
    __m512 f512a = _mm512_set1_ps(1.5f);
    __m512 f512b = _mm512_set1_ps(2.5f);
    __m256 f256a = _mm256_set1_ps(1.5f);
    __m256 f256b = _mm256_set1_ps(2.5f);
    __m128 f128a = _mm_set1_ps(1.5f);
    __m128 f128b = _mm_set1_ps(2.5f);

    SINK_512((__m512i)_mm512_cvtx2ps_ph(f512a, f512b));
    SINK_512((__m512i)_mm512_mask_cvtx2ps_ph(_mm512_setzero_ph(), k32, f512a, f512b));
    SINK_512((__m512i)_mm512_maskz_cvtx2ps_ph(k32, f512a, f512b));
    SINK_256((__m256i)_mm256_cvtx2ps_ph(f256a, f256b));
    SINK_256((__m256i)_mm256_mask_cvtx2ps_ph(_mm256_setzero_ph(), k16, f256a, f256b));
    SINK_256((__m256i)_mm256_maskz_cvtx2ps_ph(k16, f256a, f256b));
    SINK_128((__m128i)_mm_cvtx2ps_ph(f128a, f128b));
    SINK_128((__m128i)_mm_mask_cvtx2ps_ph(_mm_setzero_ph(), (__mmask8)(k16 & 0xFF), f128a, f128b));
    SINK_128((__m128i)_mm_maskz_cvtx2ps_ph((__mmask8)(k16 & 0xFF), f128a, f128b));
#endif
}

/*---------------------------------------------------------------------------
 * AVX10.2 Extended YMM Masked Operations
 *---------------------------------------------------------------------------*/
void test_avx10_2_ymm_masked(void) {
    __m256i a = _mm256_set1_epi32(0x12345678);
    __m256i b = _mm256_set1_epi32(0x87654321);
    __m256d ad = _mm256_set1_pd(3.14);
    __m256d bd = _mm256_set1_pd(2.71);
    __m256 af = _mm256_set1_ps(1.5f);
    __m256 bf = _mm256_set1_ps(2.5f);
    __m128i a128 = _mm_set1_epi32(0x12345678);
    __m128i b128 = _mm_set1_epi32(0x87654321);
    __mmask8 k8 = 0x55;
    __mmask16 k16 = 0xAAAA;
    __mmask32 k32 = 0xAAAAAAAA;

    /* AVX10.2 allows EVEX encoding for legacy AVX instructions with masking */
    /* These were already available in AVX-512VL but worth testing */
    
    /* Masked integer arithmetic */
    SINK_256(_mm256_mask_add_epi32(a, k8, a, b));
    SINK_256(_mm256_maskz_add_epi32(k8, a, b));
    SINK_256(_mm256_mask_add_epi64(a, (__mmask8)(k8 & 0x0F), a, b));
    SINK_256(_mm256_maskz_add_epi64((__mmask8)(k8 & 0x0F), a, b));
    SINK_256(_mm256_mask_sub_epi32(a, k8, a, b));
    SINK_256(_mm256_mask_mullo_epi32(a, k8, a, b));
    SINK_256(_mm256_mask_and_epi32(a, k8, a, b));
    SINK_256(_mm256_mask_or_epi32(a, k8, a, b));
    SINK_256(_mm256_mask_xor_epi32(a, k8, a, b));

    /* Masked floating-point arithmetic */
    SINK_256PS(_mm256_mask_add_ps(af, k8, af, bf));
    SINK_256PS(_mm256_maskz_add_ps(k8, af, bf));
    SINK_256D(_mm256_mask_add_pd(ad, (__mmask8)(k8 & 0x0F), ad, bd));
    SINK_256D(_mm256_maskz_add_pd((__mmask8)(k8 & 0x0F), ad, bd));
    SINK_256PS(_mm256_mask_sub_ps(af, k8, af, bf));
    SINK_256PS(_mm256_mask_mul_ps(af, k8, af, bf));
    SINK_256PS(_mm256_mask_div_ps(af, k8, af, bf));

    /* Masked FMA */
    SINK_256PS(_mm256_mask_fmadd_ps(af, k8, bf, af));
    SINK_256PS(_mm256_mask3_fmadd_ps(af, bf, af, k8));
    SINK_256PS(_mm256_maskz_fmadd_ps(k8, af, bf, af));
    SINK_256D(_mm256_mask_fmadd_pd(ad, (__mmask8)(k8 & 0x0F), bd, ad));

    /* Masked comparisons */
    SINK_MASK8(_mm256_cmp_ps_mask(af, bf, _CMP_EQ_OQ));
    SINK_MASK8(_mm256_mask_cmp_ps_mask(k8, af, bf, _CMP_EQ_OQ));
    SINK_MASK8(_mm256_cmp_pd_mask(ad, bd, _CMP_EQ_OQ));
    SINK_MASK8(_mm256_cmpeq_epi32_mask(a, b));
    SINK_MASK8(_mm256_mask_cmpeq_epi32_mask(k8, a, b));

    /* Masked blends */
    SINK_256(_mm256_mask_blend_epi32(k8, a, b));
    SINK_256PS(_mm256_mask_blend_ps(k8, af, bf));
    SINK_256D(_mm256_mask_blend_pd((__mmask8)(k8 & 0x0F), ad, bd));

    /* Masked permutes */
    SINK_256(_mm256_mask_permutexvar_epi32(a, k8, b, a));
    SINK_256(_mm256_maskz_permutexvar_epi32(k8, b, a));

    /* Masked compress/expand */
    SINK_256(_mm256_mask_compress_epi32(a, k8, b));
    SINK_256(_mm256_maskz_compress_epi32(k8, a));
    SINK_256(_mm256_mask_expand_epi32(a, k8, b));
    SINK_256(_mm256_maskz_expand_epi32(k8, a));
    SINK_256PS(_mm256_mask_compress_ps(af, k8, bf));
    SINK_256PS(_mm256_maskz_compress_ps(k8, af));
    SINK_256PS(_mm256_mask_expand_ps(af, k8, bf));
    SINK_256PS(_mm256_maskz_expand_ps(k8, af));

    /* 128-bit masked operations */
    SINK_128(_mm_mask_add_epi32(a128, (__mmask8)(k8 & 0x0F), a128, b128));
    SINK_128(_mm_maskz_add_epi32((__mmask8)(k8 & 0x0F), a128, b128));
    SINK_128(_mm_mask_compress_epi32(a128, (__mmask8)(k8 & 0x0F), b128));
    SINK_128(_mm_maskz_compress_epi32((__mmask8)(k8 & 0x0F), a128));
}

#else
/* Stubs when AVX10.2 is not available */
void test_avx10_2_saturation(void) { /* AVX10.2 not available */ }
void test_avx10_2_minmax(void) { /* AVX10.2 not available */ }
void test_avx10_2_compare(void) { /* AVX10.2 not available */ }
void test_avx10_2_media(void) { /* AVX10.2 not available */ }
void test_avx10_2_bf16_enhanced(void) { /* AVX10.2 not available */ }
void test_avx10_2_dotproduct(void) { /* AVX10.2 not available */ }
void test_avx10_2_fp8_convert(void) { /* AVX10.2 not available */ }
void test_avx10_2_ymm_masked(void) { /* AVX10.2 not available */ }
void test_avx10_2_minmax_round(void) { /* AVX10.2 not available */ }
void test_avx10_2_bf16_minmax(void) { /* AVX10.2 not available */ }
void test_avx10_2_scalar_bf16(void) { /* AVX10.2 not available */ }
#endif

/*===========================================================================
 * AVX-NE-CONVERT (Non-Excepting Conversions for BF16/FP16)
 *===========================================================================*/
#ifdef __AVXNECONVERT__
void test_avx_neconvert(void) {
    __m128 f128 = _mm_set1_ps(1.5f);
    __m256 f256 = _mm256_set1_ps(2.5f);
    
    /* Broadcast NE BF16 to FP32 */
    SINK_128PS(_mm_bcstnebf16_ps((const void*)buf_u16));
    SINK_256PS(_mm256_bcstnebf16_ps((const void*)buf_u16));
    
    /* Broadcast NE FP16 (short) to FP32 */
    SINK_128PS(_mm_bcstnesh_ps((const void*)buf_u16));
    SINK_256PS(_mm256_bcstnesh_ps((const void*)buf_u16));
    
    /* Convert NE even-indexed BF16 to FP32 */
    SINK_128PS(_mm_cvtneebf16_ps((const __m128bh*)buf_u16));
    SINK_256PS(_mm256_cvtneebf16_ps((const __m256bh*)buf_u16));
    
    /* Convert NE even-indexed FP16 to FP32 */
    SINK_128PS(_mm_cvtneeph_ps((const __m128h*)buf_u16));
    SINK_256PS(_mm256_cvtneeph_ps((const __m256h*)buf_u16));
    
    /* Convert NE odd-indexed BF16 to FP32 */
    SINK_128PS(_mm_cvtneobf16_ps((const __m128bh*)buf_u16));
    SINK_256PS(_mm256_cvtneobf16_ps((const __m256bh*)buf_u16));
    
    /* Convert NE odd-indexed FP16 to FP32 */
    SINK_128PS(_mm_cvtneoph_ps((const __m128h*)buf_u16));
    SINK_256PS(_mm256_cvtneoph_ps((const __m256h*)buf_u16));
    
    /* Convert FP32 to BF16 (non-excepting) */
    SINK_128((__m128i)_mm_cvtneps_avx_pbh(f128));
    SINK_128((__m128i)_mm256_cvtneps_avx_pbh(f256));
}
#else
void test_avx_neconvert(void) { /* AVX-NE-CONVERT not available */ }
#endif

/*===========================================================================
 * SM3 Cryptographic Hash (Chinese National Standard)
 *===========================================================================*/
#ifdef __SM3__
void test_sm3(void) {
    __m128i a = _mm_set1_epi32(0x12345678);
    __m128i b = _mm_set1_epi32(0x87654321);
    __m128i c = _mm_set1_epi32(0xDEADBEEF);
    
    /* SM3MSG1 - message expansion step 1 */
    SINK_128(_mm_sm3msg1_epi32(a, b, c));
    
    /* SM3MSG2 - message expansion step 2 */
    SINK_128(_mm_sm3msg2_epi32(a, b, c));
    
    /* SM3RNDS2 - two rounds of SM3 compression */
    SINK_128(_mm_sm3rnds2_epi32(a, b, c, 0));
    SINK_128(_mm_sm3rnds2_epi32(a, b, c, 1));
    SINK_128(_mm_sm3rnds2_epi32(a, b, c, 62));
    SINK_128(_mm_sm3rnds2_epi32(a, b, c, 63));
}
#else
void test_sm3(void) { /* SM3 not available */ }
#endif

/*===========================================================================
 * SM4 Block Cipher (Chinese National Standard)
 *===========================================================================*/
#ifdef __SM4__
void test_sm4(void) {
    __m128i a128 = _mm_set1_epi32(0x12345678);
    __m128i b128 = _mm_set1_epi32(0x87654321);
    __m256i a256 = _mm256_set1_epi32(0x12345678);
    __m256i b256 = _mm256_set1_epi32(0x87654321);
    
    /* SM4KEY4 - key expansion */
    SINK_128(_mm_sm4key4_epi32(a128, b128));
    SINK_256(_mm256_sm4key4_epi32(a256, b256));
    
    /* SM4RNDS4 - four rounds of encryption/decryption */
    SINK_128(_mm_sm4rnds4_epi32(a128, b128));
    SINK_256(_mm256_sm4rnds4_epi32(a256, b256));

#if defined(__AVX10_2__) && defined(__AVX10_2_512__)
    /* SM4 with 512-bit EVEX encoding (AVX10.2) */
    __m512i a512 = _mm512_set1_epi32(0x12345678);
    __m512i b512 = _mm512_set1_epi32(0x87654321);
    
    SINK_512(_mm512_sm4key4_epi32(a512, b512));
    SINK_512(_mm512_sm4rnds4_epi32(a512, b512));
#endif
}
#else
void test_sm4(void) { /* SM4 not available */ }
#endif

/*===========================================================================
 * SHA-512 Cryptographic Hash
 *===========================================================================*/
#ifdef __SHA512__
void test_sha512(void) {
    __m256i a = _mm256_set1_epi64x(0x123456789ABCDEF0ULL);
    __m256i b = _mm256_set1_epi64x(0x0FEDCBA987654321ULL);
    __m128i c = _mm_set1_epi64x(0xDEADBEEFCAFEBABEULL);
    
    /* SHA512MSG1 - message schedule word computation (part 1) */
    SINK_256(_mm256_sha512msg1_epi64(a, c));
    
    /* SHA512MSG2 - message schedule word computation (part 2) */
    SINK_256(_mm256_sha512msg2_epi64(a, b));
    
    /* SHA512RNDS2 - two rounds of SHA-512 compression */
    SINK_256(_mm256_sha512rnds2_epi64(a, b, c));
}
#else
void test_sha512(void) { /* SHA-512 not available */ }
#endif

/*===========================================================================
 * AVX-VNNI-INT8 (8-bit Integer VNNI without AVX-512)
 *===========================================================================*/
#ifdef __AVXVNNIINT8__
void test_avx_vnni_int8(void) {
    __m128i a128 = _mm_set1_epi8(0x7F);
    __m128i b128 = _mm_set1_epi8(0x01);
    __m128i c128 = _mm_setzero_si128();
    __m256i a256 = _mm256_set1_epi8(0x7F);
    __m256i b256 = _mm256_set1_epi8(0x01);
    __m256i c256 = _mm256_setzero_si256();
    
    /* VPDPBSSD - signed*signed byte dot product to dword */
    SINK_128(_mm_dpbssd_epi32(c128, a128, b128));
    SINK_256(_mm256_dpbssd_epi32(c256, a256, b256));
    
    /* VPDPBSSDS - with saturation */
    SINK_128(_mm_dpbssds_epi32(c128, a128, b128));
    SINK_256(_mm256_dpbssds_epi32(c256, a256, b256));
    
    /* VPDPBSUD - signed*unsigned byte dot product */
    SINK_128(_mm_dpbsud_epi32(c128, a128, b128));
    SINK_256(_mm256_dpbsud_epi32(c256, a256, b256));
    
    /* VPDPBSUDS - with saturation */
    SINK_128(_mm_dpbsuds_epi32(c128, a128, b128));
    SINK_256(_mm256_dpbsuds_epi32(c256, a256, b256));
    
    /* VPDPBUUD - unsigned*unsigned byte dot product */
    SINK_128(_mm_dpbuud_epi32(c128, a128, b128));
    SINK_256(_mm256_dpbuud_epi32(c256, a256, b256));
    
    /* VPDPBUUDS - with saturation */
    SINK_128(_mm_dpbuuds_epi32(c128, a128, b128));
    SINK_256(_mm256_dpbuuds_epi32(c256, a256, b256));
}
#else
void test_avx_vnni_int8(void) { /* AVX-VNNI-INT8 not available */ }
#endif

/*===========================================================================
 * AVX-VNNI-INT16 (16-bit Integer VNNI without AVX-512)
 *===========================================================================*/
#ifdef __AVXVNNIINT16__
void test_avx_vnni_int16(void) {
    __m128i a128 = _mm_set1_epi16(0x7FFF);
    __m128i b128 = _mm_set1_epi16(0x0001);
    __m128i c128 = _mm_setzero_si128();
    __m256i a256 = _mm256_set1_epi16(0x7FFF);
    __m256i b256 = _mm256_set1_epi16(0x0001);
    __m256i c256 = _mm256_setzero_si256();
    
    /* VPDPWSUD - signed*unsigned word dot product to dword */
    SINK_128(_mm_dpwsud_epi32(c128, a128, b128));
    SINK_256(_mm256_dpwsud_epi32(c256, a256, b256));
    
    /* VPDPWSUDS - with saturation */
    SINK_128(_mm_dpwsuds_epi32(c128, a128, b128));
    SINK_256(_mm256_dpwsuds_epi32(c256, a256, b256));
    
    /* VPDPWUSD - unsigned*signed word dot product */
    SINK_128(_mm_dpwusd_epi32(c128, a128, b128));
    SINK_256(_mm256_dpwusd_epi32(c256, a256, b256));
    
    /* VPDPWUSDS - with saturation */
    SINK_128(_mm_dpwusds_epi32(c128, a128, b128));
    SINK_256(_mm256_dpwusds_epi32(c256, a256, b256));
    
    /* VPDPWUUD - unsigned*unsigned word dot product */
    SINK_128(_mm_dpwuud_epi32(c128, a128, b128));
    SINK_256(_mm256_dpwuud_epi32(c256, a256, b256));
    
    /* VPDPWUUDS - with saturation */
    SINK_128(_mm_dpwuuds_epi32(c128, a128, b128));
    SINK_256(_mm256_dpwuuds_epi32(c256, a256, b256));
}
#else
void test_avx_vnni_int16(void) { /* AVX-VNNI-INT16 not available */ }
#endif

/*===========================================================================
 * AVX-IFMA (Integer Fused Multiply-Add without AVX-512)
 *===========================================================================*/
#ifdef __AVXIFMA__
void test_avx_ifma(void) {
    __m128i a128 = _mm_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m128i b128 = _mm_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m128i c128 = _mm_set1_epi64x(0x0000000000000001ULL);
    __m256i a256 = _mm256_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m256i b256 = _mm256_set1_epi64x(0x000FFFFFFFFFFFFFULL);
    __m256i c256 = _mm256_set1_epi64x(0x0000000000000001ULL);
    
    /* VPMADD52LUQ - multiply low 52-bit, accumulate */
    SINK_128(_mm_madd52lo_avx_epu64(c128, a128, b128));
    SINK_256(_mm256_madd52lo_avx_epu64(c256, a256, b256));
    
    /* VPMADD52HUQ - multiply high 52-bit, accumulate */
    SINK_128(_mm_madd52hi_avx_epu64(c128, a128, b128));
    SINK_256(_mm256_madd52hi_avx_epu64(c256, a256, b256));
}
#else
void test_avx_ifma(void) { /* AVX-IFMA not available */ }
#endif

/*===========================================================================
 * AVX10.2 MinMax with Rounding Control
 *===========================================================================*/
#ifdef __AVX10_2__
void test_avx10_2_minmax_round(void) {
    __m512 a512 = _mm512_set1_ps(1.5f);
    __m512 b512 = _mm512_set1_ps(2.5f);
    __m512d ad512 = _mm512_set1_pd(3.14);
    __m512d bd512 = _mm512_set1_pd(2.71);
    __m128 a128 = _mm_set1_ps(1.5f);
    __m128 b128 = _mm_set1_ps(2.5f);
    __m128d ad128 = _mm_set1_pd(3.14);
    __m128d bd128 = _mm_set1_pd(2.71);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VMINMAXPS with rounding control (SAE) */
    SINK_512PS(_mm512_minmax_round_ps(a512, b512, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_512PS(_mm512_minmax_round_ps(a512, b512, 0x01, _MM_FROUND_CUR_DIRECTION));
    SINK_512PS(_mm512_mask_minmax_round_ps(a512, k16, a512, b512, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_512PS(_mm512_maskz_minmax_round_ps(k16, a512, b512, 0x00, _MM_FROUND_CUR_DIRECTION));

    /* VMINMAXPD with rounding control */
    SINK_512D(_mm512_minmax_round_pd(ad512, bd512, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_512D(_mm512_minmax_round_pd(ad512, bd512, 0x01, _MM_FROUND_CUR_DIRECTION));
    SINK_512D(_mm512_mask_minmax_round_pd(ad512, k8, ad512, bd512, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_512D(_mm512_maskz_minmax_round_pd(k8, ad512, bd512, 0x00, _MM_FROUND_CUR_DIRECTION));

    /* Scalar VMINMAXSS with rounding control */
    SINK_128PS(_mm_minmax_round_ss(a128, b128, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_128PS(_mm_minmax_round_ss(a128, b128, 0x01, _MM_FROUND_CUR_DIRECTION));
    SINK_128PS(_mm_mask_minmax_round_ss(a128, (__mmask8)0x01, a128, b128, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_128PS(_mm_maskz_minmax_round_ss((__mmask8)0x01, a128, b128, 0x00, _MM_FROUND_CUR_DIRECTION));

    /* Scalar VMINMAXSD with rounding control */
    SINK_128D(_mm_minmax_round_sd(ad128, bd128, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_128D(_mm_minmax_round_sd(ad128, bd128, 0x01, _MM_FROUND_CUR_DIRECTION));
    SINK_128D(_mm_mask_minmax_round_sd(ad128, (__mmask8)0x01, ad128, bd128, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_128D(_mm_maskz_minmax_round_sd((__mmask8)0x01, ad128, bd128, 0x00, _MM_FROUND_CUR_DIRECTION));

#ifdef __AVX512FP16__
    /* VMINMAXPH with rounding control */
    __m512h h512a = _mm512_set1_ph((_Float16)1.5f);
    __m512h h512b = _mm512_set1_ph((_Float16)2.5f);
    __m128h h128a = _mm_set1_ph((_Float16)1.5f);
    __m128h h128b = _mm_set1_ph((_Float16)2.5f);
    __mmask32 k32 = 0xAAAAAAAA;

    SINK_512((__m512i)_mm512_minmax_round_ph(h512a, h512b, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_512((__m512i)_mm512_mask_minmax_round_ph(h512a, k32, h512a, h512b, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_512((__m512i)_mm512_maskz_minmax_round_ph(k32, h512a, h512b, 0x00, _MM_FROUND_CUR_DIRECTION));

    /* Scalar VMINMAXSH with rounding control */
    SINK_128((__m128i)_mm_minmax_round_sh(h128a, h128b, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_128((__m128i)_mm_mask_minmax_round_sh(h128a, (__mmask8)0x01, h128a, h128b, 0x00, _MM_FROUND_CUR_DIRECTION));
    SINK_128((__m128i)_mm_maskz_minmax_round_sh((__mmask8)0x01, h128a, h128b, 0x00, _MM_FROUND_CUR_DIRECTION));
#endif
}

/*---------------------------------------------------------------------------
 * AVX10.2 BF16 MinMax
 *---------------------------------------------------------------------------*/
void test_avx10_2_bf16_minmax(void) {
    __m512bh a512 = _mm512_castps_pbh(_mm512_set1_ps(1.5f));
    __m512bh b512 = _mm512_castps_pbh(_mm512_set1_ps(2.5f));
    __m256bh a256 = _mm256_castps_pbh(_mm256_set1_ps(1.5f));
    __m256bh b256 = _mm256_castps_pbh(_mm256_set1_ps(2.5f));
    __m128bh a128 = _mm_castps_pbh(_mm_set1_ps(1.5f));
    __m128bh b128 = _mm_castps_pbh(_mm_set1_ps(2.5f));
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VMINMAXNEPBF16 - 512-bit */
    SINK_512((__m512i)_mm512_minmax_pbh(a512, b512, 0x00));
    SINK_512((__m512i)_mm512_minmax_pbh(a512, b512, 0x01));
    SINK_512((__m512i)_mm512_minmax_pbh(a512, b512, 0x02));
    SINK_512((__m512i)_mm512_minmax_pbh(a512, b512, 0x03));
    SINK_512((__m512i)_mm512_mask_minmax_pbh(a512, k32, a512, b512, 0x00));
    SINK_512((__m512i)_mm512_maskz_minmax_pbh(k32, a512, b512, 0x00));

    /* VMINMAXNEPBF16 - 256-bit */
    SINK_256((__m256i)_mm256_minmax_pbh(a256, b256, 0x00));
    SINK_256((__m256i)_mm256_minmax_pbh(a256, b256, 0x01));
    SINK_256((__m256i)_mm256_mask_minmax_pbh(a256, k16, a256, b256, 0x00));
    SINK_256((__m256i)_mm256_maskz_minmax_pbh(k16, a256, b256, 0x00));

    /* VMINMAXNEPBF16 - 128-bit */
    SINK_128((__m128i)_mm_minmax_pbh(a128, b128, 0x00));
    SINK_128((__m128i)_mm_minmax_pbh(a128, b128, 0x01));
    SINK_128((__m128i)_mm_mask_minmax_pbh(a128, k8, a128, b128, 0x00));
    SINK_128((__m128i)_mm_maskz_minmax_pbh(k8, a128, b128, 0x00));
}

/*---------------------------------------------------------------------------
 * AVX10.2 Scalar BF16 Operations
 *---------------------------------------------------------------------------*/
void test_avx10_2_scalar_bf16(void) {
    __m128bh a = _mm_castps_pbh(_mm_set1_ps(1.5f));
    __m128bh b = _mm_castps_pbh(_mm_set1_ps(2.5f));
    __mmask8 k8 = 0x01;

    /* Scalar BF16 set/move */
    /* Note: _mm_set_sbh requires actual __bf16 type support */
    SINK_128((__m128i)_mm_move_sbh(a, b));
    
    /* Scalar BF16 store */
    _mm_store_sbh((void*)buf_u16, a);
    _mm_mask_store_sbh((void*)buf_u16, k8, a);

    /* Scalar BF16 load */
    SINK_128((__m128i)_mm_load_sbh((void const*)buf_u16));
    SINK_128((__m128i)_mm_mask_load_sbh(a, k8, (void const*)buf_u16));
    SINK_128((__m128i)_mm_maskz_load_sbh(k8, (void const*)buf_u16));
}

#else
void test_avx10_2_minmax_round(void) { /* AVX10.2 not available */ }
void test_avx10_2_bf16_minmax(void) { /* AVX10.2 not available */ }
void test_avx10_2_scalar_bf16(void) { /* AVX10.2 not available */ }
#endif

/*===========================================================================
 * Additional AVX-512 Scalar Operations
 *===========================================================================*/
void test_avx512_scalar(void) {
    __m128 a = _mm_set_ss(1.5f);
    __m128 b = _mm_set_ss(2.5f);
    __m128d ad = _mm_set_sd(3.14159265358979);
    __m128d bd = _mm_set_sd(2.71828182845904);
    __mmask8 k8 = 0x01;

    /* Scalar masked operations */
    SINK_128PS(_mm_mask_add_ss(a, k8, a, b));
    SINK_128PS(_mm_maskz_add_ss(k8, a, b));
    SINK_128PS(_mm_mask_sub_ss(a, k8, a, b));
    SINK_128PS(_mm_mask_mul_ss(a, k8, a, b));
    SINK_128PS(_mm_mask_div_ss(a, k8, a, b));
    SINK_128PS(_mm_mask_sqrt_ss(a, k8, a, b));
    SINK_128D(_mm_mask_add_sd(ad, k8, ad, bd));
    SINK_128D(_mm_maskz_add_sd(k8, ad, bd));
    SINK_128D(_mm_mask_sub_sd(ad, k8, ad, bd));
    SINK_128D(_mm_mask_mul_sd(ad, k8, ad, bd));
    SINK_128D(_mm_mask_div_sd(ad, k8, ad, bd));
    SINK_128D(_mm_mask_sqrt_sd(ad, k8, ad, bd));

    /* Scalar FMA masked */
    SINK_128PS(_mm_mask_fmadd_ss(a, k8, b, a));
    SINK_128PS(_mm_mask3_fmadd_ss(a, b, a, k8));
    SINK_128PS(_mm_maskz_fmadd_ss(k8, a, b, a));
    SINK_128D(_mm_mask_fmadd_sd(ad, k8, bd, ad));

    /* Scalar comparisons with mask */
    SINK_MASK8(_mm_cmp_ss_mask(a, b, _CMP_EQ_OQ));
    SINK_MASK8(_mm_mask_cmp_ss_mask(k8, a, b, _CMP_EQ_OQ));
    SINK_MASK8(_mm_cmp_sd_mask(ad, bd, _CMP_EQ_OQ));
    SINK_MASK8(_mm_mask_cmp_sd_mask(k8, ad, bd, _CMP_EQ_OQ));

    /* Scalar min/max masked */
    SINK_128PS(_mm_mask_min_ss(a, k8, a, b));
    SINK_128PS(_mm_mask_max_ss(a, k8, a, b));
    SINK_128D(_mm_mask_min_sd(ad, k8, ad, bd));
    SINK_128D(_mm_mask_max_sd(ad, k8, ad, bd));

    /* Scalar rounding/scale */
    SINK_128PS(_mm_roundscale_ss(a, b, 0));
    SINK_128D(_mm_roundscale_sd(ad, bd, 0));
    SINK_128PS(_mm_mask_roundscale_ss(a, k8, a, b, 0));
    SINK_128D(_mm_mask_roundscale_sd(ad, k8, ad, bd, 0));

    /* Scalar getexp/getmant */
    SINK_128PS(_mm_getexp_ss(a, b));
    SINK_128D(_mm_getexp_sd(ad, bd));
    SINK_128PS(_mm_getmant_ss(a, b, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));
    SINK_128D(_mm_getmant_sd(ad, bd, _MM_MANT_NORM_1_2, _MM_MANT_SIGN_src));

    /* Scalar scalef */
    SINK_128PS(_mm_scalef_ss(a, b));
    SINK_128D(_mm_scalef_sd(ad, bd));

    /* Scalar fixup */
    __m128i ctrl = _mm_set1_epi32(0);
    SINK_128PS(_mm_fixupimm_ss(a, b, ctrl, 0));
    SINK_128D(_mm_fixupimm_sd(ad, bd, _mm_set1_epi64x(0), 0));

    /* Scalar range */
    SINK_128PS(_mm_range_ss(a, b, 0));
    SINK_128D(_mm_range_sd(ad, bd, 0));

    /* Scalar reduce */
    SINK_128PS(_mm_reduce_ss(a, b, 0));
    SINK_128D(_mm_reduce_sd(ad, bd, 0));

    /* Scalar rsqrt/rcp */
    SINK_128PS(_mm_rsqrt14_ss(a, b));
    SINK_128D(_mm_rsqrt14_sd(ad, bd));
    SINK_128PS(_mm_rcp14_ss(a, b));
    SINK_128D(_mm_rcp14_sd(ad, bd));

    /* Scalar conversions */
    SINK_128PS(_mm_mask_cvtsd_ss(a, k8, a, bd));
    SINK_128D(_mm_mask_cvtss_sd(ad, k8, ad, a));
}

/*===========================================================================
 * AVX-512 Prefetch Instructions (PF)
 *===========================================================================*/
void test_avx512pf(void) {
    /* Note: AVX-512PF is deprecated and only on Xeon Phi
       Including for completeness but may not compile on all targets */
#ifdef __AVX512PF__
    __m512i idx = _mm512_set_epi32(15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0);
    __mmask16 k16 = 0xAAAA;
    
    _mm512_prefetch_i32gather_ps(idx, buf_f32, 4, _MM_HINT_T0);
    _mm512_mask_prefetch_i32gather_ps(idx, k16, buf_f32, 4, _MM_HINT_T0);
    _mm512_prefetch_i32scatter_ps(buf_f32, idx, 4, _MM_HINT_T0);
    _mm512_mask_prefetch_i32scatter_ps(buf_f32, k16, idx, 4, _MM_HINT_T0);
    
    __m512i idx64 = _mm512_set_epi64(7,6,5,4,3,2,1,0);
    __mmask8 k8 = 0x55;
    
    _mm512_prefetch_i64gather_pd(idx64, buf_f64, 8, _MM_HINT_T0);
    _mm512_mask_prefetch_i64gather_pd(idx64, k8, buf_f64, 8, _MM_HINT_T0);
    _mm512_prefetch_i64scatter_pd(buf_f64, idx64, 8, _MM_HINT_T0);
    _mm512_mask_prefetch_i64scatter_pd(buf_f64, k8, idx64, 8, _MM_HINT_T0);
#endif
}

/*===========================================================================
 * Additional Miscellaneous AVX-512 Instructions
 *===========================================================================*/
void test_avx512_misc(void) {
    __m512i a = _mm512_set1_epi32(0x12345678);
    __m512i b = _mm512_set1_epi32(0x87654321);
    __m512d ad = _mm512_set1_pd(3.14);
    __m512 af = _mm512_set1_ps(2.71f);
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;

    /* VALIGN - concatenate and shift */
    SINK_512(_mm512_alignr_epi32(a, b, 3));
    SINK_512(_mm512_alignr_epi64(a, b, 2));
    SINK_512(_mm512_mask_alignr_epi32(a, k16, a, b, 3));
    SINK_512(_mm512_maskz_alignr_epi32(k16, a, b, 3));

    /* VEXTRACT/INSERT with different sizes */
    SINK_128(_mm512_extracti32x4_epi32(a, 0));
    SINK_128(_mm512_extracti32x4_epi32(a, 1));
    SINK_128(_mm512_extracti32x4_epi32(a, 2));
    SINK_128(_mm512_extracti32x4_epi32(a, 3));
    SINK_128D(_mm512_extractf64x2_pd(ad, 0));
    SINK_128PS(_mm512_extractf32x4_ps(af, 0));
    
    SINK_512(_mm512_inserti32x4(a, _mm_setzero_si128(), 0));
    SINK_512(_mm512_inserti32x4(a, _mm_setzero_si128(), 1));
    SINK_512(_mm512_inserti32x4(a, _mm_setzero_si128(), 2));
    SINK_512(_mm512_inserti32x4(a, _mm_setzero_si128(), 3));
    SINK_512D(_mm512_insertf64x2(ad, _mm_setzero_pd(), 0));
    SINK_512PS(_mm512_insertf32x4(af, _mm_setzero_ps(), 0));

    /* Permute2 - two-source permute */
    SINK_512(_mm512_permutex2var_epi32(a, b, a));
    SINK_512(_mm512_mask_permutex2var_epi32(a, k16, b, a));
    SINK_512(_mm512_maskz_permutex2var_epi32(k16, a, b, a));
    SINK_512(_mm512_mask2_permutex2var_epi32(a, b, k16, a));
    SINK_512(_mm512_permutex2var_epi64(a, b, a));
    SINK_512D(_mm512_permutex2var_pd(ad, a, ad));
    SINK_512PS(_mm512_permutex2var_ps(af, a, af));

    /* Set with mask */
    SINK_512(_mm512_mask_set1_epi32(a, k16, 42));
    SINK_512(_mm512_maskz_set1_epi32(k16, 42));
    SINK_512(_mm512_mask_set1_epi64(a, k8, 42));
    SINK_512(_mm512_maskz_set1_epi64(k8, 42));

    /* Move with mask */
    SINK_512(_mm512_mask_mov_epi32(a, k16, b));
    SINK_512(_mm512_maskz_mov_epi32(k16, a));
    SINK_512(_mm512_mask_mov_epi64(a, k8, b));
    SINK_512D(_mm512_mask_mov_pd(ad, k8, ad));
    SINK_512PS(_mm512_mask_mov_ps(af, k16, af));

    /* Cvt with mask */
    SINK_512(_mm512_mask_cvtepi8_epi32(a, k16, _mm_setzero_si128()));
    SINK_512(_mm512_maskz_cvtepi8_epi32(k16, _mm_setzero_si128()));
    SINK_512(_mm512_mask_cvtepu8_epi32(a, k16, _mm_setzero_si128()));
    SINK_512(_mm512_maskz_cvtepu8_epi32(k16, _mm_setzero_si128()));

    /* Zero-extend/sign-extend with truncate */
    SINK_128(_mm512_cvtepi32_epi8(a));
    SINK_128(_mm512_cvtsepi32_epi8(a));
    SINK_128(_mm512_cvtusepi32_epi8(a));
    SINK_256(_mm512_cvtepi32_epi16(a));
    SINK_256(_mm512_cvtsepi32_epi16(a));
    SINK_256(_mm512_cvtusepi32_epi16(a));
    SINK_256(_mm512_cvtepi64_epi32(a));
    SINK_256(_mm512_cvtsepi64_epi32(a));
    SINK_256(_mm512_cvtusepi64_epi32(a));
    SINK_128(_mm512_cvtsepi64_epi16(a));
    SINK_128(_mm512_cvtusepi64_epi16(a));
    SINK_128(_mm512_cvtepi64_epi8(a));
    SINK_128(_mm512_cvtsepi64_epi8(a));
    SINK_128(_mm512_cvtusepi64_epi8(a));
}

/*===========================================================================
 * MOVRS - Memory Read Speculative Loads (Intel Arrow Lake / Lunar Lake)
 * Speculative loads that don't block on cache misses
 *===========================================================================*/
#ifdef __MOVRS__
void test_movrs(void) {
    /* 128-bit MOVRS loads */
    SINK_128(_mm_loadrs_epi8(buf_i8));
    SINK_128(_mm_loadrs_epi16(buf_i16));
    SINK_128(_mm_loadrs_epi32(buf_i32));
    SINK_128(_mm_loadrs_epi64(buf_i64));

    /* 256-bit MOVRS loads */
    SINK_256(_mm256_loadrs_epi8(buf_i8));
    SINK_256(_mm256_loadrs_epi16(buf_i16));
    SINK_256(_mm256_loadrs_epi32(buf_i32));
    SINK_256(_mm256_loadrs_epi64(buf_i64));

    /* 512-bit MOVRS loads */
    SINK_512(_mm512_loadrs_epi8(buf_i8));
    SINK_512(_mm512_loadrs_epi16(buf_i16));
    SINK_512(_mm512_loadrs_epi32(buf_i32));
    SINK_512(_mm512_loadrs_epi64(buf_i64));

    /* Masked MOVRS loads - 128-bit */
    __mmask16 k16 = 0xAAAA;
    __mmask8 k8 = 0x55;
    __mmask32 k32 = 0xAAAAAAAA;
    __mmask64 k64 = 0xAAAAAAAAAAAAAAAAULL;
    __m128i src128 = _mm_setzero_si128();
    __m256i src256 = _mm256_setzero_si256();
    __m512i src512 = _mm512_setzero_si512();

    SINK_128(_mm_mask_loadrs_epi8(src128, k16, buf_i8));
    SINK_128(_mm_maskz_loadrs_epi8(k16, buf_i8));
    SINK_128(_mm_mask_loadrs_epi16(src128, k8, buf_i16));
    SINK_128(_mm_maskz_loadrs_epi16(k8, buf_i16));
    SINK_128(_mm_mask_loadrs_epi32(src128, k8, buf_i32));
    SINK_128(_mm_maskz_loadrs_epi32(k8, buf_i32));
    SINK_128(_mm_mask_loadrs_epi64(src128, k8, buf_i64));
    SINK_128(_mm_maskz_loadrs_epi64(k8, buf_i64));

    /* Masked MOVRS loads - 256-bit */
    SINK_256(_mm256_mask_loadrs_epi8(src256, k32, buf_i8));
    SINK_256(_mm256_maskz_loadrs_epi8(k32, buf_i8));
    SINK_256(_mm256_mask_loadrs_epi16(src256, k16, buf_i16));
    SINK_256(_mm256_maskz_loadrs_epi16(k16, buf_i16));
    SINK_256(_mm256_mask_loadrs_epi32(src256, k8, buf_i32));
    SINK_256(_mm256_maskz_loadrs_epi32(k8, buf_i32));
    SINK_256(_mm256_mask_loadrs_epi64(src256, k8, buf_i64));
    SINK_256(_mm256_maskz_loadrs_epi64(k8, buf_i64));

    /* Masked MOVRS loads - 512-bit */
    SINK_512(_mm512_mask_loadrs_epi8(src512, k64, buf_i8));
    SINK_512(_mm512_maskz_loadrs_epi8(k64, buf_i8));
    SINK_512(_mm512_mask_loadrs_epi16(src512, k32, buf_i16));
    SINK_512(_mm512_maskz_loadrs_epi16(k32, buf_i16));
    SINK_512(_mm512_mask_loadrs_epi32(src512, k16, buf_i32));
    SINK_512(_mm512_maskz_loadrs_epi32(k16, buf_i32));
    SINK_512(_mm512_mask_loadrs_epi64(src512, k8, buf_i64));
    SINK_512(_mm512_maskz_loadrs_epi64(k8, buf_i64));
}
#else
void test_movrs(void) { /* MOVRS not available */ }
#endif

/*===========================================================================
 * AMX - Advanced Matrix Extensions (Intel Sapphire Rapids+)
 * Tile-based matrix multiplication accelerator
 *===========================================================================*/
#if defined(__AMX_TILE__) && defined(__AMX_INT8__) && defined(__AMX_BF16__)
#include <amxintrin.h>

/* Tile configuration structure - must be 64-byte aligned */
typedef struct __attribute__((aligned(64))) {
    uint8_t palette_id;
    uint8_t start_row;
    uint8_t reserved[14];
    uint16_t colsb[8];   /* Bytes per row for each tile */
    uint16_t reserved2[8];
    uint8_t rows[8];     /* Number of rows for each tile */
    uint8_t reserved3[8];
} tile_config_t;

void test_amx(void) {
    /* Tile configuration: 16 rows x 64 bytes (for 16x16 tiles of INT8 or BF16) */
    static tile_config_t config = {
        .palette_id = 1,
        .start_row = 0,
        .colsb = {64, 64, 64, 64, 64, 64, 64, 64},
        .rows = {16, 16, 16, 16, 16, 16, 16, 16}
    };

    /* Load tile configuration */
    _tile_loadconfig(&config);

    /* Aligned buffers for tile operations - 1KB each (16 rows x 64 bytes) */
    static __attribute__((aligned(64))) uint8_t tile_buf_a[1024];
    static __attribute__((aligned(64))) uint8_t tile_buf_b[1024];
    static __attribute__((aligned(64))) uint8_t tile_buf_c[1024];

    /* Initialize tile data */
    for (int i = 0; i < 1024; i++) {
        tile_buf_a[i] = (uint8_t)(i & 0xFF);
        tile_buf_b[i] = (uint8_t)((i * 3) & 0xFF);
        tile_buf_c[i] = 0;
    }

    /* Zero tiles */
    _tile_zero(0);
    _tile_zero(1);
    _tile_zero(2);

    /* Load tiles from memory - stride is 64 bytes */
    _tile_loadd(0, tile_buf_a, 64);  /* tmm0 = A */
    _tile_loadd(1, tile_buf_b, 64);  /* tmm1 = B */
    _tile_loadd(2, tile_buf_c, 64);  /* tmm2 = C (accumulator) */

    /* INT8 dot products: C += A * B */
    _tile_dpbssd(2, 0, 1);   /* signed * signed -> dword */
    _tile_dpbsud(2, 0, 1);   /* signed * unsigned -> dword */
    _tile_dpbusd(2, 0, 1);   /* unsigned * signed -> dword */
    _tile_dpbuud(2, 0, 1);   /* unsigned * unsigned -> dword */

    /* Store result tile */
    _tile_stored(2, tile_buf_c, 64);

    /* BF16 operations - C += A * B (BF16 matrix multiply) */
    _tile_zero(3);
    _tile_zero(4);
    _tile_zero(5);
    _tile_loadd(3, tile_buf_a, 64);
    _tile_loadd(4, tile_buf_b, 64);
    _tile_dpbf16ps(5, 3, 4);  /* BF16 dot product -> FP32 */
    _tile_stored(5, tile_buf_c, 64);

#ifdef __AMX_FP16__
    /* FP16 operations (Granite Rapids+) */
    _tile_zero(6);
    _tile_dpfp16ps(6, 3, 4);  /* FP16 dot product -> FP32 */
    _tile_stored(6, tile_buf_c, 64);
#endif

#ifdef __AMX_COMPLEX__
    /* Complex FP16 operations */
    _tile_zero(7);
    _tile_cmmimfp16ps(7, 3, 4);  /* Complex multiply-add (imaginary) */
    _tile_cmmrlfp16ps(7, 3, 4);  /* Complex multiply-add (real) */
#endif

    /* Release tile configuration */
    _tile_release();
}
#else
void test_amx(void) { /* AMX not available */ }
#endif

/*===========================================================================
 * SHA-1 and SHA-256 Legacy Instructions (Goldmont+)
 * Hardware acceleration for SHA hash computation
 *===========================================================================*/
#ifdef __SHA__
void test_sha1_sha256(void) {
    __m128i state0 = _mm_set_epi32(0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476);
    __m128i state1 = _mm_set_epi32(0xC3D2E1F0, 0x00000000, 0x00000000, 0x00000000);
    __m128i msg0 = _mm_set_epi32(0x80000000, 0x00000000, 0x00000000, 0x00000000);
    __m128i msg1 = _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000200);
    __m128i msg2 = _mm_setzero_si128();
    __m128i msg3 = _mm_setzero_si128();

    /* ========== SHA-1 Instructions ========== */
    
    /* SHA1MSG1 - Perform intermediate calculation for next 4 SHA1 message dwords */
    SINK_128(_mm_sha1msg1_epu32(msg0, msg1));
    
    /* SHA1MSG2 - Perform final calculation for next 4 SHA1 message dwords */
    SINK_128(_mm_sha1msg2_epu32(msg0, msg1));
    
    /* SHA1NEXTE - Calculate SHA1 state E after 4 rounds */
    SINK_128(_mm_sha1nexte_epu32(state0, msg0));
    
    /* SHA1RNDS4 - Perform 4 rounds of SHA1 operation
     * imm8 selects the function: 0=F1, 1=F2, 2=F3, 3=F4 */
    SINK_128(_mm_sha1rnds4_epu32(state0, msg0, 0));  /* Rounds 0-19 (F1) */
    SINK_128(_mm_sha1rnds4_epu32(state0, msg0, 1));  /* Rounds 20-39 (F2) */
    SINK_128(_mm_sha1rnds4_epu32(state0, msg0, 2));  /* Rounds 40-59 (F3) */
    SINK_128(_mm_sha1rnds4_epu32(state0, msg0, 3));  /* Rounds 60-79 (F4) */

    /* ========== SHA-256 Instructions ========== */
    
    __m128i sha256_state0 = _mm_set_epi32(0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A);
    __m128i sha256_state1 = _mm_set_epi32(0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19);
    
    /* SHA256MSG1 - Perform intermediate calculation for next 4 SHA256 message dwords */
    SINK_128(_mm_sha256msg1_epu32(msg0, msg1));
    
    /* SHA256MSG2 - Perform final calculation for next 4 SHA256 message dwords */
    SINK_128(_mm_sha256msg2_epu32(msg0, msg1));
    
    /* SHA256RNDS2 - Perform 2 rounds of SHA256 operation
     * Uses implicit XMM0 for message schedule + K constant */
    SINK_128(_mm_sha256rnds2_epu32(sha256_state0, sha256_state1, msg0));
    
    /* Full SHA-256 message schedule expansion pattern */
    __m128i w0 = msg0;
    __m128i w1 = msg1;
    __m128i w2 = msg2;
    __m128i w3 = msg3;
    __m128i w4, w5, w6, w7;
    
    /* Calculate W[4-7] */
    w4 = _mm_sha256msg1_epu32(w0, w1);
    w4 = _mm_add_epi32(w4, _mm_alignr_epi8(w3, w2, 4));
    w4 = _mm_sha256msg2_epu32(w4, w3);
    SINK_128(w4);
    
    /* Calculate W[8-11] */
    w5 = _mm_sha256msg1_epu32(w1, w2);
    w5 = _mm_add_epi32(w5, _mm_alignr_epi8(w4, w3, 4));
    w5 = _mm_sha256msg2_epu32(w5, w4);
    SINK_128(w5);
    
    /* Multiple rounds of SHA-256 */
    __m128i tmp;
    tmp = _mm_sha256rnds2_epu32(sha256_state0, sha256_state1, w0);
    sha256_state1 = _mm_sha256rnds2_epu32(sha256_state1, tmp, _mm_shuffle_epi32(w0, 0x0E));
    sha256_state0 = tmp;
    SINK_128(sha256_state0);
    SINK_128(sha256_state1);
}
#else
void test_sha1_sha256(void) { /* SHA not available */ }
#endif

/*===========================================================================
 * PREFETCH Instructions - Various cache hint variants
 * Control data prefetching into cache hierarchy
 *===========================================================================*/
void test_prefetch(void) {
    const void *ptr = buf_i32;
    const void *ptr2 = buf_f32;
    const void *ptr3 = buf_i64;
    
    /* ========== Standard Prefetch (SSE) ========== */
    
    /* PREFETCHT0 - Prefetch into all cache levels (L1, L2, L3) */
    _mm_prefetch((const char*)ptr, _MM_HINT_T0);
    
    /* PREFETCHT1 - Prefetch into L2 and L3 cache (skip L1) */
    _mm_prefetch((const char*)ptr + 64, _MM_HINT_T1);
    
    /* PREFETCHT2 - Prefetch into L3 cache only */
    _mm_prefetch((const char*)ptr + 128, _MM_HINT_T2);
    
    /* PREFETCHNTA - Prefetch non-temporal (minimize cache pollution) */
    _mm_prefetch((const char*)ptr + 192, _MM_HINT_NTA);
    
    /* ========== Write Prefetch (PREFETCHW / PREFETCHWT1) ========== */
    
#ifdef __PRFCHW__
    /* PREFETCHW - Prefetch with intent to write (exclusive state) */
    _m_prefetchw((void*)ptr2);
    _m_prefetchw((void*)((char*)ptr2 + 64));
#endif

    /* PREFETCHWT1 - Prefetch for write into L2 (AVX-512PF) */
    /* Note: _MM_HINT_ET0 = prefetchw, _MM_HINT_ET1 = prefetchwt1 */
    _mm_prefetch((const char*)ptr2 + 128, _MM_HINT_ET0);  /* Write, L1 */
    _mm_prefetch((const char*)ptr2 + 192, _MM_HINT_ET1);  /* Write, L2 */
    
    /* ========== Multiple prefetches with stride pattern ========== */
    
    /* Prefetch a series of cache lines (common in loop optimization) */
    for (int i = 0; i < 8; i++) {
        _mm_prefetch((const char*)ptr3 + i * 64, _MM_HINT_T0);
    }
    
    /* Interleaved read/write prefetch pattern */
    _mm_prefetch((const char*)buf_i8, _MM_HINT_T0);        /* Read hint */
    _mm_prefetch((const char*)buf_i8 + 64, _MM_HINT_ET0);  /* Write hint */
    _mm_prefetch((const char*)buf_i8 + 128, _MM_HINT_T1);  /* Read, L2 */
    _mm_prefetch((const char*)buf_i8 + 192, _MM_HINT_NTA); /* Non-temporal */
    
    /* ========== Clflush / Clflushopt / Clwb ========== */
    
    /* CLFLUSH - Flush cache line (write back and invalidate) */
    _mm_clflush(buf_u8);
    _mm_clflush(buf_u8 + 64);
    
#ifdef __CLFLUSHOPT__
    /* CLFLUSHOPT - Optimized cache line flush (weakly ordered) */
    _mm_clflushopt(buf_u16);
    _mm_clflushopt(buf_u16 + 32);
#endif

#ifdef __CLWB__
    /* CLWB - Cache line write back (keep in cache) */
    _mm_clwb(buf_u32);
    _mm_clwb(buf_u32 + 16);
#endif

    /* Memory fence after cache operations */
    _mm_sfence();
    _mm_mfence();
}

/*===========================================================================
 * Entry point - calls all test suites (freestanding, no libc)
 *===========================================================================*/
void _start(void) {
    /* Initialize buffers with test patterns */
    for (int i = 0; i < 128; i++) buf_i8[i] = (int8_t)i;
    for (int i = 0; i < 128; i++) buf_u8[i] = (uint8_t)i;
    for (int i = 0; i < 64; i++) buf_i16[i] = (int16_t)(i * 100);
    for (int i = 0; i < 64; i++) buf_u16[i] = (uint16_t)(i * 100);
    for (int i = 0; i < 32; i++) buf_i32[i] = i * 10000;
    for (int i = 0; i < 32; i++) buf_u32[i] = (uint32_t)(i * 10000);
    for (int i = 0; i < 16; i++) buf_i64[i] = (int64_t)i * 100000000LL;
    for (int i = 0; i < 16; i++) buf_u64[i] = (uint64_t)i * 100000000ULL;
    for (int i = 0; i < 32; i++) buf_f32[i] = (float)i * 0.5f;
    for (int i = 0; i < 16; i++) buf_f64[i] = (double)i * 0.25;

    test_avx512f();
    test_avx512cd();
    test_avx512bw();
    test_avx512dq();
    test_avx512vl();
    test_avx512ifma();
    test_avx512vbmi();
    test_avx512vbmi2();
    test_avx512vnni();
    test_avx512bitalg();
    test_avx512vpopcntdq();
    test_avx512vp2intersect();
    test_avx512fp16();
    test_avx512bf16();
    test_avx512gfni();
    test_avx512vaes();
    test_avx512vpclmulqdq();
    test_avx512_scalar();
    test_avx512_misc();
    test_avx512pf();
    test_avx10_2_saturation();
    test_avx10_2_minmax();
    test_avx10_2_compare();
    test_avx10_2_media();
    test_avx10_2_bf16_enhanced();
    test_avx10_2_dotproduct();
    test_avx10_2_fp8_convert();
    test_avx10_2_ymm_masked();
    test_avx10_2_minmax_round();
    test_avx10_2_bf16_minmax();
    test_avx10_2_scalar_bf16();
    
    /* New ISA extensions */
    test_avx_neconvert();
    test_sm3();
    test_sm4();
    test_sha512();
    test_avx_vnni_int8();
    test_avx_vnni_int16();
    test_avx_ifma();

    /* Memory/Cache instructions */
    test_movrs();
    test_prefetch();

    /* SHA-1/SHA-256 */
    test_sha1_sha256();

    /* Advanced Matrix Extensions */
    test_amx();

    /* Halt - freestanding has no exit() */
    while(1) { __asm__ volatile("hlt"); }
}
