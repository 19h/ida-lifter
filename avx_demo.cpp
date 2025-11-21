// avx_demo.cpp
// Complex AVX/AVX2 test program with several "real-life" style workloads.

#include <immintrin.h>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#if !defined(__AVX__)
#  error "This demo requires AVX support (compile with -mavx)."
#endif

#if !defined(__AVX2__)
#  warning "AVX2 not enabled; some integer AVX2 ops are not used in this demo."
#endif

#if !defined(__FMA__)
#  warning "FMA not enabled; FMA intrinsics will be emulated by the compiler."
#endif

// Simple portable timer
struct ScopedTimer
{
    using clock = std::chrono::high_resolution_clock;

    std::string label;
    clock::time_point start;
    double &out_ms;

    ScopedTimer(const std::string &lbl, double &ms_ref)
        : label(lbl), start(clock::now()), out_ms(ms_ref)
    {
    }

    ~ScopedTimer()
    {
        auto end = clock::now();
        auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        out_ms = static_cast<double>(us) / 1000.0;
    }
};

// Simple deterministic pseudo-random filler (no <random> overhead)
static inline uint32_t lcg_next(uint32_t &state)
{
    state = state * 1664525u + 1013904223u;
    return state;
}

static void fill_random(std::vector<float> &v, uint32_t seed = 0x12345678u)
{
    uint32_t s = seed;
    for (float &x : v)
    {
        uint32_t r = lcg_next(s);
        // Map to [-1.0, 1.0]
        x = (static_cast<int32_t>(r) / 2147483648.0f);
    }
}

// Horizontal sum of a __m256 using SSE fallbacks (per lane)
static inline float hsum256_ps(__m256 v)
{
    __m128 vlow  = _mm256_castps256_ps128(v);
    __m128 vhigh = _mm256_extractf128_ps(v, 1);
    vlow         = _mm_add_ps(vlow, vhigh);          // add the two 128-bit halves

    __m128 shuf  = _mm_movehdup_ps(vlow);            // (v3,v3,v1,v1)
    __m128 sums  = _mm_add_ps(vlow, shuf);           // (v0+v3, v1+v3, v2+v1, v3+v1)
    shuf         = _mm_movehl_ps(shuf, sums);        // (   ,   , sums3, sums2)
    sums         = _mm_add_ss(sums, shuf);
    return _mm_cvtss_f32(sums);
}

// ============================================================================
// Workload 1: SAXPY and cosine similarity (vectorized finance/signal-processing)
// ============================================================================

// y = a * x + y (scalar reference)
static float saxpy_scalar(float a, const float *x, float *y, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        y[i] = a * x[i] + y[i];

    // checksum
    float acc = 0.0f;
    for (size_t i = 0; i < n; ++i)
        acc += y[i];
    return acc;
}

// y = a * x + y (AVX)
static float saxpy_avx(float a, const float *x, float *y, size_t n)
{
    const size_t step = 8;
    size_t i = 0;
    __m256 av = _mm256_set1_ps(a);
    for (; i + step <= n; i += step)
    {
        __m256 vx = _mm256_loadu_ps(x + i);
        __m256 vy = _mm256_loadu_ps(y + i);
        __m256 r  = _mm256_fmadd_ps(av, vx, vy); // av*vx + vy
        _mm256_storeu_ps(y + i, r);
    }
    // tail
    for (; i < n; ++i)
        y[i] = a * x[i] + y[i];

    float acc = 0.0f;
    for (size_t j = 0; j < n; ++j)
        acc += y[j];
    return acc;
}

// Cosine similarity scalar
static float cosine_similarity_scalar(const float *x, const float *y, size_t n)
{
    double dot = 0.0;
    double nx  = 0.0;
    double ny  = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
        double xi = x[i];
        double yi = y[i];
        dot += xi * yi;
        nx  += xi * xi;
        ny  += yi * yi;
    }
    double denom = std::sqrt(nx * ny);
    if (denom == 0.0)
        return 0.0f;
    return static_cast<float>(dot / denom);
}

// Cosine similarity AVX
static float cosine_similarity_avx(const float *x, const float *y, size_t n)
{
    const size_t step = 8;
    size_t i = 0;
    __m256 dotv = _mm256_setzero_ps();
    __m256 nxv  = _mm256_setzero_ps();
    __m256 nyv  = _mm256_setzero_ps();

    for (; i + step <= n; i += step)
    {
        __m256 vx = _mm256_loadu_ps(x + i);
        __m256 vy = _mm256_loadu_ps(y + i);
        __m256 prod = _mm256_mul_ps(vx, vy);
        dotv = _mm256_add_ps(dotv, prod);
        nxv  = _mm256_fmadd_ps(vx, vx, nxv);
        nyv  = _mm256_fmadd_ps(vy, vy, nyv);
    }

    float dot = hsum256_ps(dotv);
    float nx  = hsum256_ps(nxv);
    float ny  = hsum256_ps(nyv);

    for (; i < n; ++i)
    {
        float xi = x[i];
        float yi = y[i];
        dot += xi * yi;
        nx  += xi * xi;
        ny  += yi * yi;
    }

    double denom = std::sqrt(static_cast<double>(nx) * static_cast<double>(ny));
    if (denom == 0.0)
        return 0.0f;
    return static_cast<float>(dot / denom);
}

// ============================================================================
// Workload 2: 2D image blur on float "image" (e.g., 1080p grayscale)
// new[y,x] = 0.2 * (center + left + right + up + down)
// ============================================================================

struct ImageF
{
    int w;
    int h;
    std::vector<float> data;

    ImageF(int width, int height)
        : w(width), h(height), data(static_cast<size_t>(width) * height)
    {
    }

    float *row(int y) { return data.data() + static_cast<size_t>(y) * w; }
    const float *row(int y) const { return data.data() + static_cast<size_t>(y) * w; }
};

// Scalar blur
static float blur5_scalar(const ImageF &src, ImageF &dst)
{
    const int w = src.w;
    const int h = src.h;

    // Copy border as-is
    std::memcpy(dst.row(0), src.row(0), sizeof(float) * w);
    std::memcpy(dst.row(h - 1), src.row(h - 1), sizeof(float) * w);
    for (int y = 1; y < h - 1; ++y)
    {
        dst.row(y)[0]     = src.row(y)[0];
        dst.row(y)[w - 1] = src.row(y)[w - 1];
    }

    float checksum = 0.0f;
    for (int y = 1; y < h - 1; ++y)
    {
        const float *row_c = src.row(y);
        const float *row_u = src.row(y - 1);
        const float *row_d = src.row(y + 1);
        float *row_o       = dst.row(y);
        for (int x = 1; x < w - 1; ++x)
        {
            float val = row_c[x]
                      + row_c[x - 1]
                      + row_c[x + 1]
                      + row_u[x]
                      + row_d[x];
            val *= 0.2f;
            row_o[x] = val;
            checksum += val;
        }
    }
    return checksum;
}

// AVX blur: processes interior pixels in 8-wide blocks
static float blur5_avx(const ImageF &src, ImageF &dst)
{
    const int w = src.w;
    const int h = src.h;

    std::memcpy(dst.row(0), src.row(0), sizeof(float) * w);
    std::memcpy(dst.row(h - 1), src.row(h - 1), sizeof(float) * w);
    for (int y = 1; y < h - 1; ++y)
    {
        dst.row(y)[0]     = src.row(y)[0];
        dst.row(y)[w - 1] = src.row(y)[w - 1];
    }

    const __m256 scale = _mm256_set1_ps(0.2f);
    float checksum = 0.0f;

    for (int y = 1; y < h - 1; ++y)
    {
        const float *row_c = src.row(y);
        const float *row_u = src.row(y - 1);
        const float *row_d = src.row(y + 1);
        float *row_o       = dst.row(y);

        int x = 1;
        const int max_x = w - 1;
        const int vec_end = max_x - 8 + 1; // last x for a full 8-wide block

        for (; x <= vec_end; x += 8)
        {
            __m256 vc = _mm256_loadu_ps(row_c + x);
            __m256 vl = _mm256_loadu_ps(row_c + x - 1);
            __m256 vr = _mm256_loadu_ps(row_c + x + 1);
            __m256 vu = _mm256_loadu_ps(row_u + x);
            __m256 vd = _mm256_loadu_ps(row_d + x);

            __m256 sum = _mm256_add_ps(vc, vl);
            sum        = _mm256_add_ps(sum, vr);
            sum        = _mm256_add_ps(sum, vu);
            sum        = _mm256_add_ps(sum, vd);
            sum        = _mm256_mul_ps(sum, scale);

            _mm256_storeu_ps(row_o + x, sum);
            checksum += hsum256_ps(sum);
        }

        // tail in this row
        for (; x < max_x; ++x)
        {
            float val = row_c[x]
                      + row_c[x - 1]
                      + row_c[x + 1]
                      + row_u[x]
                      + row_d[x];
            val *= 0.2f;
            row_o[x] = val;
            checksum += val;
        }
    }

    return checksum;
}

// ============================================================================
// Workload 3: Complex multiply + FIR convolution with AVX/FMA
// Complex data in SoA form (real[] and imag[] arrays)
// ============================================================================

struct ComplexSoA
{
    std::vector<float> re;
    std::vector<float> im;

    ComplexSoA(size_t n = 0)
        : re(n), im(n)
    {
    }

    void resize(size_t n)
    {
        re.resize(n);
        im.resize(n);
    }

    size_t size() const { return re.size(); }
};

static void fill_complex(ComplexSoA &c, uint32_t seed = 0xCAFEBABEu)
{
    uint32_t s = seed;
    for (size_t i = 0; i < c.size(); ++i)
    {
        uint32_t r1 = lcg_next(s);
        uint32_t r2 = lcg_next(s);
        c.re[i] = (static_cast<int32_t>(r1) / 2147483648.0f);
        c.im[i] = (static_cast<int32_t>(r2) / 2147483648.0f);
    }
}

// Complex multiply scalar: out = a * b
static float complex_mul_scalar(const ComplexSoA &a, const ComplexSoA &b, ComplexSoA &out)
{
    const size_t n = a.size();
    out.resize(n);
    float checksum = 0.0f;
    for (size_t i = 0; i < n; ++i)
    {
        float ar = a.re[i];
        float ai = a.im[i];
        float br = b.re[i];
        float bi = b.im[i];
        float zr = ar * br - ai * bi;
        float zi = ar * bi + ai * br;
        out.re[i] = zr;
        out.im[i] = zi;
        checksum += zr * 0.5f + zi * 0.25f;
    }
    return checksum;
}

// Complex multiply AVX/FMA: out = a * b (SoA)
static float complex_mul_avx(const ComplexSoA &a, const ComplexSoA &b, ComplexSoA &out)
{
    const size_t n = a.size();
    out.resize(n);
    const size_t step = 8;
    size_t i = 0;
    float checksum = 0.0f;

    for (; i + step <= n; i += step)
    {
        __m256 ar = _mm256_loadu_ps(a.re.data() + i);
        __m256 ai = _mm256_loadu_ps(a.im.data() + i);
        __m256 br = _mm256_loadu_ps(b.re.data() + i);
        __m256 bi = _mm256_loadu_ps(b.im.data() + i);

        // zr = ar*br - ai*bi
        __m256 zr = _mm256_fmsub_ps(ar, br, _mm256_mul_ps(ai, bi));
        // zi = ar*bi + ai*br
        __m256 zi = _mm256_fmadd_ps(ar, bi, _mm256_mul_ps(ai, br));

        _mm256_storeu_ps(out.re.data() + i, zr);
        _mm256_storeu_ps(out.im.data() + i, zi);

        // simple checksum: linear combination
        __m256 c1 = _mm256_set1_ps(0.5f);
        __m256 c2 = _mm256_set1_ps(0.25f);
        __m256 tmp = _mm256_add_ps(_mm256_mul_ps(zr, c1), _mm256_mul_ps(zi, c2));
        checksum += hsum256_ps(tmp);
    }

    for (; i < n; ++i)
    {
        float ar = a.re[i];
        float ai = a.im[i];
        float br = b.re[i];
        float bi = b.im[i];
        float zr = ar * br - ai * bi;
        float zi = ar * bi + ai * br;
        out.re[i] = zr;
        out.im[i] = zi;
        checksum += zr * 0.5f + zi * 0.25f;
    }

    return checksum;
}

// FIR convolution (scalar) on ComplexSoA:
// y[k] = sum_{i=0..L-1} h[i] * x[k-i], real-valued taps h.
static float complex_fir_scalar(const ComplexSoA &x, const std::vector<float> &h, ComplexSoA &y)
{
    const size_t n = x.size();
    const size_t L = h.size();
    y.resize(n);
    float checksum = 0.0f;
    for (size_t k = 0; k < n; ++k)
    {
        float acc_re = 0.0f;
        float acc_im = 0.0f;
        for (size_t i = 0; i < L; ++i)
        {
            if (k < i)
                break;
            float tap = h[i];
            size_t idx = k - i;
            acc_re += tap * x.re[idx];
            acc_im += tap * x.im[idx];
        }
        y.re[k] = acc_re;
        y.im[k] = acc_im;
        checksum += acc_re * 0.75f + acc_im * 0.33f;
    }
    return checksum;
}

// FIR convolution AVX on ComplexSoA with real taps, unrolled over taps
// Uses AVX for inner products over 8 samples at a time.
static float complex_fir_avx(const ComplexSoA &x, const std::vector<float> &h, ComplexSoA &y)
{
    const size_t n = x.size();
    const size_t L = h.size();
    y.resize(n);
    float checksum = 0.0f;

    for (size_t k = 0; k < n; ++k)
    {
        __m256 acc_re_vec = _mm256_setzero_ps();
        __m256 acc_im_vec = _mm256_setzero_ps();

        size_t i = 0;
        // Vectorized over taps in chunks of 8, but respecting bounds k-i >= 0.
        for (; i + 8 <= L; i += 8)
        {
            if (k + 1 < i + 8)
                break; // would underflow indexes

            // taps h[i..i+7]
            __m256 ht = _mm256_loadu_ps(h.data() + i);

            // indices x[k-i], reversed window:
            //   idx0 = k-i
            //   idx1 = k-(i+1)
            //   ...
            // This reverse pattern is not contiguous, so here we use a simplified
            // strategy: approximate by convolving over forward indexes when possible.
            // For a realistic case you'd pre-reverse h or x into a contiguous buffer.

            size_t base = k - i - 7;
            if (base + 8 > n)
                continue;

            __m256 xr = _mm256_loadu_ps(x.re.data() + base);
            __m256 xi = _mm256_loadu_ps(x.im.data() + base);

            acc_re_vec = _mm256_fmadd_ps(xr, ht, acc_re_vec);
            acc_im_vec = _mm256_fmadd_ps(xi, ht, acc_im_vec);
        }

        float acc_re = hsum256_ps(acc_re_vec);
        float acc_im = hsum256_ps(acc_im_vec);

        // scalar remainder over taps (including underflow-safe region)
        for (; i < L; ++i)
        {
            if (k < i)
                break;
            float tap = h[i];
            size_t idx = k - i;
            acc_re += tap * x.re[idx];
            acc_im += tap * x.im[idx];
        }

        y.re[k] = acc_re;
        y.im[k] = acc_im;
        checksum += acc_re * 0.75f + acc_im * 0.33f;
    }

    return checksum;
}

// ============================================================================
// Utility: soft clip / limiter using AVX compare + blend
// y = clip(x, -threshold, +threshold)
// ============================================================================

static float soft_clip_scalar(float *x, size_t n, float threshold)
{
    float sum = 0.0f;
    for (size_t i = 0; i < n; ++i)
    {
        float v = x[i];
        if (v > threshold)
            v = threshold;
        else if (v < -threshold)
            v = -threshold;
        x[i] = v;
        sum += v;
    }
    return sum;
}

static float soft_clip_avx(float *x, size_t n, float threshold)
{
    const size_t step = 8;
    size_t i = 0;
    __m256 th  = _mm256_set1_ps(threshold);
    __m256 nth = _mm256_set1_ps(-threshold);
    float sum = 0.0f;

    for (; i + step <= n; i += step)
    {
        __m256 v  = _mm256_loadu_ps(x + i);
        // clamp high: v = min(v, th)
        __m256 v_hi = _mm256_min_ps(v, th);
        // clamp low: v = max(v_hi, -th)
        __m256 v_clamped = _mm256_max_ps(v_hi, nth);
        _mm256_storeu_ps(x + i, v_clamped);
        sum += hsum256_ps(v_clamped);
    }

    for (; i < n; ++i)
    {
        float v = x[i];
        if (v > threshold)
            v = threshold;
        else if (v < -threshold)
            v = -threshold;
        x[i] = v;
        sum += v;
    }

    return sum;
}

// ============================================================================
// Main
// ============================================================================

int main()
{
    // 1) Vector workloads: SAXPY + cosine similarity
    const size_t N_vec = 1u << 22; // 4M floats (~16 MiB per vector)

    std::vector<float> x(N_vec), y(N_vec), y2(N_vec), z(N_vec), z2(N_vec);

    fill_random(x, 0x11111111u);
    fill_random(y, 0x22222222u);
    fill_random(z, 0x33333333u);
    z2 = z; // copy for AVX path

    std::cout << "=== Workload 1: SAXPY + cosine similarity ===\n";

    double ms_saxpy_scalar = 0.0, ms_saxpy_avx = 0.0;
    float saxpy_cs_scalar = 0.0f, saxpy_cs_avx = 0.0f;

    {
        y2 = y;
        ScopedTimer t("saxpy_scalar", ms_saxpy_scalar);
        saxpy_cs_scalar = saxpy_scalar(1.2345f, x.data(), y2.data(), N_vec);
    }
    {
        y2 = y;
        ScopedTimer t("saxpy_avx", ms_saxpy_avx);
        saxpy_cs_avx = saxpy_avx(1.2345f, x.data(), y2.data(), N_vec);
    }

    double ms_cos_scalar = 0.0, ms_cos_avx = 0.0;
    float cos_scalar = 0.0f, cos_avx = 0.0f;

    {
        ScopedTimer t("cosine_scalar", ms_cos_scalar);
        cos_scalar = cosine_similarity_scalar(x.data(), z.data(), N_vec);
    }
    {
        ScopedTimer t("cosine_avx", ms_cos_avx);
        cos_avx = cosine_similarity_avx(x.data(), z2.data(), N_vec);
    }

    std::cout << "SAXPY  scalar: checksum=" << saxpy_cs_scalar << "  time=" << ms_saxpy_scalar << " ms\n";
    std::cout << "SAXPY  AVX   : checksum=" << saxpy_cs_avx    << "  time=" << ms_saxpy_avx    << " ms\n";
    std::cout << "Cosine scalar: value="    << cos_scalar      << "  time=" << ms_cos_scalar   << " ms\n";
    std::cout << "Cosine AVX   : value="    << cos_avx         << "  time=" << ms_cos_avx      << " ms\n";
    std::cout << "--------------------------------------------------------\n\n";

    // 2) Image blur (1080p)
    const int W = 1920;
    const int H = 1080;
    ImageF img(W, H);
    ImageF blur_ref(W, H);
    ImageF blur_avx(W, H);
    fill_random(img.data, 0xA5A5A5A5u);

    std::cout << "=== Workload 2: 2D 5-point blur on 1080p image ===\n";

    double ms_blur_scalar = 0.0, ms_blur_avx = 0.0;
    float cs_blur_scalar = 0.0f, cs_blur_avx = 0.0f;

    {
        ScopedTimer t("blur_scalar", ms_blur_scalar);
        cs_blur_scalar = blur5_scalar(img, blur_ref);
    }
    {
        ScopedTimer t("blur_avx", ms_blur_avx);
        cs_blur_avx = blur5_avx(img, blur_avx);
    }

    std::cout << "Blur scalar: checksum=" << cs_blur_scalar << "  time=" << ms_blur_scalar << " ms\n";
    std::cout << "Blur AVX   : checksum=" << cs_blur_avx    << "  time=" << ms_blur_avx    << " ms\n";

    // Quick consistency check: difference in checksum
    std::cout << "Checksum delta (AVX - scalar): "
              << (cs_blur_avx - cs_blur_scalar) << "\n";
    std::cout << "--------------------------------------------------------\n\n";

    // 3) Complex workloads
    const size_t N_cplx = 1u << 18; // 262,144 complex samples
    ComplexSoA a(N_cplx), b(N_cplx), c_ref, c_avx, fir_ref, fir_avx;
    fill_complex(a, 0x1234ABCDu);
    fill_complex(b, 0x9876FEDCu);

    // FIR taps (e.g. 16-tap low-pass prototype)
    const size_t L = 16;
    std::vector<float> taps(L);
    for (size_t i = 0; i < L; ++i)
    {
        // simple symmetric shape
        float x_rel = (static_cast<float>(i) - (L - 1) / 2.0f) / (L / 2.0f);
        float win   = 0.5f - 0.5f * std::cos(3.14159265358979323846f * (i + 0.5f) / L);
        taps[i]     = win * std::exp(-x_rel * x_rel);
    }

    std::cout << "=== Workload 3: Complex multiply + FIR convolution ===\n";

    double ms_cmul_scalar = 0.0, ms_cmul_avx = 0.0;
    float cs_cmul_scalar = 0.0f, cs_cmul_avx = 0.0f;

    {
        ScopedTimer t("complex_mul_scalar", ms_cmul_scalar);
        cs_cmul_scalar = complex_mul_scalar(a, b, c_ref);
    }
    {
        ScopedTimer t("complex_mul_avx", ms_cmul_avx);
        cs_cmul_avx = complex_mul_avx(a, b, c_avx);
    }

    double ms_fir_scalar = 0.0, ms_fir_avx = 0.0;
    float cs_fir_scalar = 0.0f, cs_fir_avx = 0.0f;

    {
        ScopedTimer t("complex_fir_scalar", ms_fir_scalar);
        cs_fir_scalar = complex_fir_scalar(a, taps, fir_ref);
    }
    {
        ScopedTimer t("complex_fir_avx", ms_fir_avx);
        cs_fir_avx = complex_fir_avx(a, taps, fir_avx);
    }

    std::cout << "Complex mul scalar: checksum=" << cs_cmul_scalar << "  time=" << ms_cmul_scalar << " ms\n";
    std::cout << "Complex mul AVX   : checksum=" << cs_cmul_avx    << "  time=" << ms_cmul_avx    << " ms\n";
    std::cout << "FIR      scalar   : checksum=" << cs_fir_scalar  << "  time=" << ms_fir_scalar  << " ms\n";
    std::cout << "FIR      AVX      : checksum=" << cs_fir_avx     << "  time=" << ms_fir_avx     << " ms\n";

    std::cout << "Delta cmul checksum (AVX - scalar): " << (cs_cmul_avx - cs_cmul_scalar) << "\n";
    std::cout << "Delta FIR  checksum (AVX - scalar): " << (cs_fir_avx - cs_fir_scalar)   << "\n";
    std::cout << "--------------------------------------------------------\n\n";

    // 4) Soft clipping on FIR output (just to exercise AVX clamp / min / max)
    std::cout << "=== Workload 4: Soft clip / limiter on FIR output ===\n";
    double ms_clip_scalar = 0.0, ms_clip_avx = 0.0;
    float cs_clip_scalar = 0.0f, cs_clip_avx = 0.0f;

    // Pack FIR real part into separate working buffer
    std::vector<float> fir_real = fir_ref.re;
    std::vector<float> fir_real2 = fir_real;

    {
        ScopedTimer t("soft_clip_scalar", ms_clip_scalar);
        cs_clip_scalar = soft_clip_scalar(fir_real.data(), fir_real.size(), 0.8f);
    }
    {
        ScopedTimer t("soft_clip_avx", ms_clip_avx);
        cs_clip_avx = soft_clip_avx(fir_real2.data(), fir_real2.size(), 0.8f);
    }

    std::cout << "Soft clip scalar: checksum=" << cs_clip_scalar << "  time=" << ms_clip_scalar << " ms\n";
    std::cout << "Soft clip AVX   : checksum=" << cs_clip_avx    << "  time=" << ms_clip_avx    << " ms\n";
    std::cout << "Delta clip checksum (AVX - scalar): "
              << (cs_clip_avx - cs_clip_scalar) << "\n";

    std::cout << "\nDone.\n";
    return 0;
}
