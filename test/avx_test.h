#ifndef AVX_TEST_H
#define AVX_TEST_H

#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Align to 32 bytes for AVX/AVX2
#define ALIGN32 __attribute__((aligned(32)))

typedef struct {
    float ALIGN32 f32[16];
    double ALIGN32 f64[8];
    int32_t ALIGN32 i32[16];
    int64_t ALIGN32 i64[8];
    int16_t ALIGN32 i16[32];
    int8_t ALIGN32 i8[64];
} TestData;

void init_test_data(TestData *data);
void run_avx_ops(TestData *data);
void run_avx2_ops(TestData *data);

// Helpers to prevent optimization of unused results
// 256-bit sinks
static inline void sink_m256(__m256 v) {
    volatile __m256 sink = v;
    (void)sink;
}

static inline void sink_m256d(__m256d v) {
    volatile __m256d sink = v;
    (void)sink;
}

static inline void sink_m256i(__m256i v) {
    volatile __m256i sink = v;
    (void)sink;
}

// 128-bit sinks (needed for conversions/extractions)
static inline void sink_m128(__m128 v) {
    volatile __m128 sink = v;
    (void)sink;
}

static inline void sink_m128d(__m128d v) {
    volatile __m128d sink = v;
    (void)sink;
}

static inline void sink_m128i(__m128i v) {
    volatile __m128i sink = v;
    (void)sink;
}

#endif // AVX_TEST_H
