// Test scalar AVX operations
#include <immintrin.h>

// Horizontal sum of a __m256 vector
float hsum_ps(__m256 v) {
    // Extract high 128 bits
    __m128 hi = _mm256_extractf128_ps(v, 1);
    // Get low 128 bits
    __m128 lo = _mm256_castps256_ps128(v);
    // Add them together
    __m128 sum = _mm_add_ps(lo, hi);

    // Horizontal sum of __m128
    __m128 shuf = _mm_movehdup_ps(sum);  // or use vshufps
    sum = _mm_add_ps(sum, shuf);
    shuf = _mm_movehl_ps(shuf, sum);
    sum = _mm_add_ss(sum, shuf);

    return _mm_cvtss_f32(sum);
}

// Simple scalar add test
float scalar_add(float a, float b) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_add_ss(va, vb);
    return _mm_cvtss_f32(vc);
}

// Simple scalar mul test
float scalar_mul(float a, float b) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_mul_ss(va, vb);
    return _mm_cvtss_f32(vc);
}

// Scalar div test
float scalar_div(float a, float b) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_div_ss(va, vb);
    return _mm_cvtss_f32(vc);
}

// Scalar min/max test
float scalar_min(float a, float b) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_min_ss(va, vb);
    return _mm_cvtss_f32(vc);
}

float scalar_max(float a, float b) {
    __m128 va = _mm_set_ss(a);
    __m128 vb = _mm_set_ss(b);
    __m128 vc = _mm_max_ss(va, vb);
    return _mm_cvtss_f32(vc);
}

int main() {
    __m256 v = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
    float sum = hsum_ps(v);

    float add_result = scalar_add(1.0f, 2.0f);
    float mul_result = scalar_mul(3.0f, 4.0f);
    float div_result = scalar_div(10.0f, 2.0f);
    float min_result = scalar_min(5.0f, 3.0f);
    float max_result = scalar_max(5.0f, 3.0f);

    return (int)(sum + add_result + mul_result + div_result + min_result + max_result);
}
