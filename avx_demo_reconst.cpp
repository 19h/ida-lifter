#include <immintrin.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using f32 = float;
using f64 = double;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

constexpr std::size_t N_SAXPY   = 0x400000u;   // 4,194,304 elements
constexpr int         WIDTH     = 1920;
constexpr int         HEIGHT    = 1080;
constexpr std::size_t N_PIXELS  = static_cast<std::size_t>(WIDTH) * HEIGHT; // 2,073,600
constexpr std::size_t N_COMPLEX = 0x40000u;    // 262,144 elements
constexpr int         FIR_TAPS  = 16;

// -----------------------------------------------------------------------------
// ScopedTimer: RAII timing helper writing elapsed time [ms] to a referenced slot
// -----------------------------------------------------------------------------
struct ScopedTimer
{
    using clock = std::chrono::high_resolution_clock;

    std::string        label;
    double            &out_ms;
    clock::time_point  t0;

    ScopedTimer(const std::string &name, double &target)
        : label(name), out_ms(target), t0(clock::now())
    {
    }

    ~ScopedTimer()
    {
        const auto t1 = clock::now();
        out_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    }
};

// -----------------------------------------------------------------------------
// ComplexSoA: separate real/imaginary storage for AVX-friendly layout
// -----------------------------------------------------------------------------
struct ComplexSoA
{
    std::vector<f32> re;
    std::vector<f32> im;

    explicit ComplexSoA(std::size_t n = 0) : re(n), im(n) {}

    std::size_t size() const { return re.size(); }
};

// -----------------------------------------------------------------------------
// Checksums
// -----------------------------------------------------------------------------
static double checksum_real(const std::vector<f32> &v)
{
    double sum = 0.0;
    for (f32 x : v)
        sum += static_cast<double>(x);
    return sum;
}

static double checksum_complex(const ComplexSoA &c)
{
    double sum = 0.0;
    const std::size_t n = c.size();
    for (std::size_t i = 0; i < n; ++i)
    {
        sum += static_cast<double>(c.re[i]) +
               static_cast<double>(c.im[i]);
    }
    return sum;
}

// -----------------------------------------------------------------------------
// Initialisation helpers
// -----------------------------------------------------------------------------
static void init_saxpy_vectors(std::vector<f32> &x,
                               std::vector<f32> &y)
{
    const std::size_t n = x.size();
    std::mt19937 rng(0x1234abcdU);
    std::uniform_real_distribution<f32> dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < n; ++i)
    {
        x[i] = dist(rng);
        y[i] = dist(rng);
    }
}

static void init_image(std::vector<f32> &img,
                       int width,
                       int height)
{
    const std::size_t n = static_cast<std::size_t>(width) * height;
    std::mt19937 rng(0x9876fedcU);
    std::uniform_real_distribution<f32> dist(0.0f, 1.0f);
    for (std::size_t i = 0; i < n; ++i)
        img[i] = dist(rng);
}

static void fill_complex(ComplexSoA &c, u32 seed)
{
    const std::size_t n = c.size();
    std::mt19937 rng(seed);
    std::uniform_real_distribution<f32> dist(-1.0f, 1.0f);
    for (std::size_t i = 0; i < n; ++i)
    {
        c.re[i] = dist(rng);
        c.im[i] = dist(rng);
    }
}

// FIR kernel: 16-tap Gaussian-shaped low-pass, normalised to sum=1
static std::vector<f32> make_fir_kernel()
{
    std::vector<f32> h(FIR_TAPS);
    const float center = 7.5f;     // matches (i - 7.5) in decompiled constants
    const float scale  = 0.125f;   // 0x3e000000
    const float pi     = 3.1415927410f; // 0x40490fdb

    for (int i = 0; i < FIR_TAPS; ++i)
    {
        const float x = (static_cast<float>(i) - center) * scale;
        const float g = std::exp(-x * x);
        h[i] = g;

        // Extra trigonometric path used only to exercise exp/cos-like work;
        // result not stored.
        const float phase = (x + 0.5f) * pi * 0.0625f; // 0x3f000000 * pi * 0x3d800000
        (void)std::cos(phase);
    }

    float sum = 0.0f;
    for (float v : h)
        sum += v;
    if (sum != 0.0f)
    {
        const float inv = 1.0f / sum;
        for (float &v : h)
            v *= inv;
    }
    return h;
}

// -----------------------------------------------------------------------------
// Workload 1: SAXPY + cosine similarity
// -----------------------------------------------------------------------------
static double saxpy_scalar(std::vector<f32>       &y,
                           const std::vector<f32> &x,
                           f32                     a)
{
    const std::size_t n = y.size();
    double sum = 0.0;
    for (std::size_t i = 0; i < n; ++i)
    {
        const float yi = a * x[i] + y[i];
        y[i] = yi;
        sum += static_cast<double>(yi);
    }
    return sum;
}

static double saxpy_avx(std::vector<f32>       &y,
                        const std::vector<f32> &x,
                        f32                     a)
{
    const std::size_t n = y.size();
    const float      *px = x.data();
    float            *py = y.data();

    __m256 va   = _mm256_set1_ps(a);
    __m256 vacc = _mm256_setzero_ps();

    std::size_t i = 0;
    alignas(32) float tmp[8];

    for (; i + 8 <= n; i += 8)
    {
        __m256 vx = _mm256_loadu_ps(px + i);
        __m256 vy = _mm256_loadu_ps(py + i);
        __m256 vz = _mm256_add_ps(_mm256_mul_ps(va, vx), vy);
        _mm256_storeu_ps(py + i, vz);
        vacc = _mm256_add_ps(vacc, vz);
    }

    _mm256_storeu_ps(tmp, vacc);
    double sum = 0.0;
    for (int j = 0; j < 8; ++j)
        sum += static_cast<double>(tmp[j]);

    for (; i < n; ++i)
    {
        const float yi = a * px[i] + py[i];
        py[i] = yi;
        sum += static_cast<double>(yi);
    }

    return sum;
}

static double cosine_scalar(const std::vector<f32> &x,
                            const std::vector<f32> &y)
{
    const std::size_t n = x.size();
    double dot = 0.0;
    double nx2 = 0.0;
    double ny2 = 0.0;
    for (std::size_t i = 0; i < n; ++i)
    {
        const double xi = x[i];
        const double yi = y[i];
        dot += xi * yi;
        nx2 += xi * xi;
        ny2 += yi * yi;
    }
    const double denom = std::sqrt(nx2) * std::sqrt(ny2);
    if (denom == 0.0)
        return 0.0;
    return dot / denom;
}

static double cosine_avx(const std::vector<f32> &x,
                         const std::vector<f32> &y)
{
    const std::size_t n  = x.size();
    const float      *px = x.data();
    const float      *py = y.data();

    __m256 vdot = _mm256_setzero_ps();
    __m256 vnx2 = _mm256_setzero_ps();
    __m256 vny2 = _mm256_setzero_ps();

    std::size_t i = 0;
    alignas(32) float tmp_dot[8];
    alignas(32) float tmp_nx2[8];
    alignas(32) float tmp_ny2[8];

    for (; i + 8 <= n; i += 8)
    {
        __m256 vx = _mm256_loadu_ps(px + i);
        __m256 vy = _mm256_loadu_ps(py + i);

        __m256 vx2 = _mm256_mul_ps(vx, vx);
        __m256 vy2 = _mm256_mul_ps(vy, vy);
        __m256 prod = _mm256_mul_ps(vx, vy);

        vdot = _mm256_add_ps(vdot, prod);
        vnx2 = _mm256_add_ps(vnx2, vx2);
        vny2 = _mm256_add_ps(vny2, vy2);
    }

    _mm256_storeu_ps(tmp_dot, vdot);
    _mm256_storeu_ps(tmp_nx2, vnx2);
    _mm256_storeu_ps(tmp_ny2, vny2);

    double dot = 0.0;
    double nx2 = 0.0;
    double ny2 = 0.0;
    for (int j = 0; j < 8; ++j)
    {
        dot += static_cast<double>(tmp_dot[j]);
        nx2 += static_cast<double>(tmp_nx2[j]);
        ny2 += static_cast<double>(tmp_ny2[j]);
    }

    for (; i < n; ++i)
    {
        const double xi = px[i];
        const double yi = py[i];
        dot += xi * yi;
        nx2 += xi * xi;
        ny2 += yi * yi;
    }

    const double denom = std::sqrt(nx2) * std::sqrt(ny2);
    if (denom == 0.0)
        return 0.0;
    return dot / denom;
}

// -----------------------------------------------------------------------------
// Workload 2: 2D 5-point blur on 1080p image
// -----------------------------------------------------------------------------
static double blur5_scalar(const std::vector<f32> &src,
                           std::vector<f32>       &dst,
                           int                     width,
                           int                     height)
{
    const std::size_t w = static_cast<std::size_t>(width);
    const std::size_t h = static_cast<std::size_t>(height);

    // Copy borders
    for (std::size_t x = 0; x < w; ++x)
    {
        dst[x]                 = src[x];
        dst[(h - 1) * w + x]   = src[(h - 1) * w + x];
    }
    for (std::size_t y = 1; y + 1 < h; ++y)
    {
        dst[y * w]           = src[y * w];
        dst[y * w + (w - 1)] = src[y * w + (w - 1)];
    }

    // Interior 5-point stencil
    const float scale = 0.2f;
    for (std::size_t y = 1; y + 1 < h; ++y)
    {
        for (std::size_t x = 1; x + 1 < w; ++x)
        {
            const std::size_t idx = y * w + x;
            const float c   = src[idx];
            const float up  = src[idx - w];
            const float dn  = src[idx + w];
            const float lf  = src[idx - 1];
            const float rt  = src[idx + 1];
            dst[idx] = scale * (c + up + dn + lf + rt);
        }
    }

    return checksum_real(dst);
}

static double blur5_avx(const std::vector<f32> &src,
                        std::vector<f32>       &dst,
                        int                     width,
                        int                     height)
{
    const std::size_t w = static_cast<std::size_t>(width);
    const std::size_t h = static_cast<std::size_t>(height);
    const float      *ps = src.data();
    float            *pd = dst.data();

    // Copy borders (scalar)
    for (std::size_t x = 0; x < w; ++x)
    {
        pd[x]               = ps[x];
        pd[(h - 1) * w + x] = ps[(h - 1) * w + x];
    }
    for (std::size_t y = 1; y + 1 < h; ++y)
    {
        pd[y * w]           = ps[y * w];
        pd[y * w + (w - 1)] = ps[y * w + (w - 1)];
    }

    const __m256 vscale = _mm256_set1_ps(0.2f);

    for (std::size_t y = 1; y + 1 < h; ++y)
    {
        std::size_t x = 1;
        for (; x + 7 < w - 1; x += 8)
        {
            const std::size_t idx = y * w + x;

            __m256 c  = _mm256_loadu_ps(ps + idx);
            __m256 up = _mm256_loadu_ps(ps + idx - w);
            __m256 dn = _mm256_loadu_ps(ps + idx + w);
            __m256 lf = _mm256_loadu_ps(ps + idx - 1);
            __m256 rt = _mm256_loadu_ps(ps + idx + 1);

            __m256 sum1 = _mm256_add_ps(c, up);
            __m256 sum2 = _mm256_add_ps(dn, lf);
            __m256 sum  = _mm256_add_ps(_mm256_add_ps(sum1, sum2), rt);
            __m256 out  = _mm256_mul_ps(sum, vscale);

            _mm256_storeu_ps(pd + idx, out);
        }

        // Tail
        for (; x + 1 < w; ++x)
        {
            const std::size_t idx = y * w + x;
            const float c   = ps[idx];
            const float up  = ps[idx - w];
            const float dn  = ps[idx + w];
            const float lf  = ps[idx - 1];
            const float rt  = ps[idx + 1];
            pd[idx] = 0.2f * (c + up + dn + lf + rt);
        }
    }

    return checksum_real(dst);
}

// -----------------------------------------------------------------------------
// Workload 3: Complex multiply + FIR convolution
// -----------------------------------------------------------------------------
static double complex_mul_scalar(const ComplexSoA &a,
                                 const ComplexSoA &b,
                                 ComplexSoA       &out)
{
    const std::size_t n = a.size();
    double sum = 0.0;

    for (std::size_t i = 0; i < n; ++i)
    {
        const float ar = a.re[i];
        const float ai = a.im[i];
        const float br = b.re[i];
        const float bi = b.im[i];

        const float cr = ar * br - ai * bi;
        const float ci = ar * bi + ai * br;

        out.re[i] = cr;
        out.im[i] = ci;

        sum += static_cast<double>(cr) + static_cast<double>(ci);
    }

    return sum;
}

static double complex_mul_avx(const ComplexSoA &a,
                              const ComplexSoA &b,
                              ComplexSoA       &out)
{
    const std::size_t n  = a.size();
    const float      *ar = a.re.data();
    const float      *ai = a.im.data();
    const float      *br = b.re.data();
    const float      *bi = b.im.data();
    float            *or_ = out.re.data();
    float            *oi  = out.im.data();

    __m256 acc_re = _mm256_setzero_ps();
    __m256 acc_im = _mm256_setzero_ps();

    std::size_t i = 0;
    alignas(32) float tmp_re[8];
    alignas(32) float tmp_im[8];

    for (; i + 8 <= n; i += 8)
    {
        __m256 ar_v = _mm256_loadu_ps(ar + i);
        __m256 ai_v = _mm256_loadu_ps(ai + i);
        __m256 br_v = _mm256_loadu_ps(br + i);
        __m256 bi_v = _mm256_loadu_ps(bi + i);

        __m256 arbr = _mm256_mul_ps(ar_v, br_v);
        __m256 aibi = _mm256_mul_ps(ai_v, bi_v);
        __m256 arbi = _mm256_mul_ps(ar_v, bi_v);
        __m256 aibr = _mm256_mul_ps(ai_v, br_v);

        __m256 cr = _mm256_sub_ps(arbr, aibi);
        __m256 ci = _mm256_add_ps(arbi, aibr);

        _mm256_storeu_ps(or_ + i, cr);
        _mm256_storeu_ps(oi + i, ci);

        acc_re = _mm256_add_ps(acc_re, cr);
        acc_im = _mm256_add_ps(acc_im, ci);
    }

    _mm256_storeu_ps(tmp_re, acc_re);
    _mm256_storeu_ps(tmp_im, acc_im);

    double sum = 0.0;
    for (int j = 0; j < 8; ++j)
        sum += static_cast<double>(tmp_re[j]) + static_cast<double>(tmp_im[j]);

    for (; i < n; ++i)
    {
        const float ar_ = ar[i];
        const float ai_ = ai[i];
        const float br_ = br[i];
        const float bi_ = bi[i];

        const float cr = ar_ * br_ - ai_ * bi_;
        const float ci = ar_ * bi_ + ai_ * br_;

        or_[i] = cr;
        oi[i]  = ci;

        sum += static_cast<double>(cr) + static_cast<double>(ci);
    }

    return sum;
}

static double complex_fir_scalar(const ComplexSoA       &in,
                                 const std::vector<f32> &h,
                                 ComplexSoA             &out)
{
    const std::size_t n     = in.size();
    const int         taps  = static_cast<int>(h.size());
    double            sum   = 0.0;

    for (std::size_t i = 0; i < n; ++i)
    {
        float acc_re = 0.0f;
        float acc_im = 0.0f;

        const std::size_t limit = (i + 1 < static_cast<std::size_t>(taps))
                                ? (i + 1)
                                : static_cast<std::size_t>(taps);

        for (std::size_t k = 0; k < limit; ++k)
        {
            const float coeff = h[static_cast<int>(k)];
            const std::size_t idx = i - k;
            acc_re += coeff * in.re[idx];
            acc_im += coeff * in.im[idx];
        }

        out.re[i] = acc_re;
        out.im[i] = acc_im;

        sum += static_cast<double>(acc_re) + static_cast<double>(acc_im);
    }

    return sum;
}

static double complex_fir_avx(const ComplexSoA       &in,
                              const std::vector<f32> &h,
                              ComplexSoA             &out)
{
    const std::size_t n     = in.size();
    const int         taps  = static_cast<int>(h.size());
    const float      *re    = in.re.data();
    const float      *im    = in.im.data();
    float            *ore   = out.re.data();
    float            *oim   = out.im.data();

    double sum = 0.0;

    // Head region: scalar (insufficient history for a full vector block)
    const std::size_t start = static_cast<std::size_t>(taps - 1);
    for (std::size_t i = 0; i < std::min(start, n); ++i)
    {
        float acc_re = 0.0f;
        float acc_im = 0.0f;
        const std::size_t limit = (i + 1 < static_cast<std::size_t>(taps))
                                ? (i + 1)
                                : static_cast<std::size_t>(taps);
        for (std::size_t k = 0; k < limit; ++k)
        {
            const float coeff = h[static_cast<int>(k)];
            const std::size_t idx = i - k;
            acc_re += coeff * re[idx];
            acc_im += coeff * im[idx];
        }
        ore[i] = acc_re;
        oim[i] = acc_im;
        sum += static_cast<double>(acc_re) + static_cast<double>(acc_im);
    }

    // Vectorised interior
    std::size_t i = start;
    alignas(32) float tmp_re[8];
    alignas(32) float tmp_im[8];

    for (; i + 8 <= n; i += 8)
    {
        __m256 acc_re = _mm256_setzero_ps();
        __m256 acc_im = _mm256_setzero_ps();

        for (int k = 0; k < taps; ++k)
        {
            const float coeff = h[k];
            const float *pre  = re + i - k;
            const float *pim  = im + i - k;

            __m256 vcoeff = _mm256_set1_ps(coeff);
            __m256 vre    = _mm256_loadu_ps(pre);
            __m256 vim    = _mm256_loadu_ps(pim);

            acc_re = _mm256_add_ps(acc_re, _mm256_mul_ps(vcoeff, vre));
            acc_im = _mm256_add_ps(acc_im, _mm256_mul_ps(vcoeff, vim));
        }

        _mm256_storeu_ps(ore + i, acc_re);
        _mm256_storeu_ps(oim + i, acc_im);

        _mm256_storeu_ps(tmp_re, acc_re);
        _mm256_storeu_ps(tmp_im, acc_im);
        for (int j = 0; j < 8; ++j)
            sum += static_cast<double>(tmp_re[j]) + static_cast<double>(tmp_im[j]);
    }

    // Tail region: scalar
    for (; i < n; ++i)
    {
        float acc_re = 0.0f;
        float acc_im = 0.0f;
        for (int k = 0; k < taps; ++k)
        {
            if (i < static_cast<std::size_t>(k))
                break;
            const std::size_t idx = i - static_cast<std::size_t>(k);
            const float coeff     = h[k];
            acc_re += coeff * re[idx];
            acc_im += coeff * im[idx];
        }
        ore[i] = acc_re;
        oim[i] = acc_im;
        sum += static_cast<double>(acc_re) + static_cast<double>(acc_im);
    }

    return sum;
}

// -----------------------------------------------------------------------------
// Workload 4: Soft clip / limiter on FIR output
// -----------------------------------------------------------------------------
static inline float soft_clip_scalar_sample(float x, float threshold)
{
    const float t = threshold;
    if (x <= -t)
        return -t;
    if (x >= t)
        return t;

    const float x2 = x * x;
    const float x3 = x2 * x;
    const float t2 = t * t;
    return x - x3 / t2;
}

static double soft_clip_scalar(const std::vector<f32> &in,
                               std::vector<f32>       &out,
                               float                   threshold)
{
    const std::size_t n = in.size();
    double sum = 0.0;
    for (std::size_t i = 0; i < n; ++i)
    {
        const float y = soft_clip_scalar_sample(in[i], threshold);
        out[i] = y;
        sum += static_cast<double>(y);
    }
    return sum;
}

static double soft_clip_avx(const std::vector<f32> &in,
                            std::vector<f32>       &out,
                            float                   threshold)
{
    const std::size_t n = in.size();
    const float      *pin = in.data();
    float            *pout = out.data();

    const __m256 vth  = _mm256_set1_ps(threshold);
    const __m256 vmin = _mm256_sub_ps(_mm256_setzero_ps(), vth); // -threshold
    const __m256 vt2  = _mm256_set1_ps(threshold * threshold);

    __m256 vacc = _mm256_setzero_ps();
    std::size_t i = 0;
    alignas(32) float tmp[8];

    for (; i + 8 <= n; i += 8)
    {
        __m256 x  = _mm256_loadu_ps(pin + i);
        __m256 x1 = _mm256_min_ps(_mm256_max_ps(x, vmin), vth); // clamped
        __m256 x2 = _mm256_mul_ps(x1, x1);
        __m256 x3 = _mm256_mul_ps(x2, x1);
        __m256 frac = _mm256_div_ps(x3, vt2);
        __m256 y    = _mm256_sub_ps(x1, frac);

        _mm256_storeu_ps(pout + i, y);
        vacc = _mm256_add_ps(vacc, y);
    }

    _mm256_storeu_ps(tmp, vacc);
    double sum = 0.0;
    for (int j = 0; j < 8; ++j)
        sum += static_cast<double>(tmp[j]);

    for (; i < n; ++i)
    {
        const float y = soft_clip_scalar_sample(pin[i], threshold);
        pout[i] = y;
        sum += static_cast<double>(y);
    }

    return sum;
}

// -----------------------------------------------------------------------------
// main()
// -----------------------------------------------------------------------------
int main()
{
    // -------------------------------------------------------------------------
    // Workload 1: SAXPY + cosine similarity
    // -------------------------------------------------------------------------
    std::vector<f32> x(N_SAXPY);
    std::vector<f32> y_scalar(N_SAXPY);
    std::vector<f32> y_avx(N_SAXPY);

    init_saxpy_vectors(x, y_scalar);
    y_avx = y_scalar;

    const float alpha = 0.5f;

    double t_saxpy_scalar = 0.0;
    double t_saxpy_avx    = 0.0;
    double t_cos_scalar   = 0.0;
    double t_cos_avx      = 0.0;

    double saxpy_scalar_sum = 0.0;
    double saxpy_avx_sum    = 0.0;
    double cos_scalar_val   = 0.0;
    double cos_avx_val      = 0.0;

    std::cout << "=== Workload 1: SAXPY + cosine similarity ===\n";

    {
        ScopedTimer timer("saxpy_scalar", t_saxpy_scalar);
        saxpy_scalar_sum = saxpy_scalar(y_scalar, x, alpha);
    }
    {
        ScopedTimer timer("saxpy_avx", t_saxpy_avx);
        saxpy_avx_sum = saxpy_avx(y_avx, x, alpha);
    }
    {
        ScopedTimer timer("cosine_scalar", t_cos_scalar);
        cos_scalar_val = cosine_scalar(x, y_scalar);
    }
    {
        ScopedTimer timer("cosine_avx", t_cos_avx);
        cos_avx_val = cosine_avx(x, y_avx);
    }

    std::cout << "SAXPY  scalar: checksum=" << saxpy_scalar_sum
              << "  time=" << t_saxpy_scalar << " ms\n";
    std::cout << "SAXPY  AVX   : checksum=" << saxpy_avx_sum
              << "  time=" << t_saxpy_avx << " ms\n";
    std::cout << "Cosine scalar: value=" << cos_scalar_val
              << "  time=" << t_cos_scalar << " ms\n";
    std::cout << "Cosine AVX   : value=" << cos_avx_val
              << "  time=" << t_cos_avx << " ms\n";
    std::cout << "--------------------------------------------------------\n\n";

    // -------------------------------------------------------------------------
    // Workload 2: 2D 5-point blur on 1080p image
    // -------------------------------------------------------------------------
    std::vector<f32> img_src(N_PIXELS);
    std::vector<f32> img_blur_scalar(N_PIXELS);
    std::vector<f32> img_blur_avx(N_PIXELS);

    init_image(img_src, WIDTH, HEIGHT);

    double t_blur_scalar = 0.0;
    double t_blur_avx    = 0.0;
    double blur_scalar_sum = 0.0;
    double blur_avx_sum    = 0.0;

    std::cout << "=== Workload 2: 2D 5-point blur on 1080p image ===\n";

    {
        ScopedTimer timer("blur_scalar", t_blur_scalar);
        blur_scalar_sum = blur5_scalar(img_src, img_blur_scalar, WIDTH, HEIGHT);
    }
    {
        ScopedTimer timer("blur_avx", t_blur_avx);
        blur_avx_sum = blur5_avx(img_src, img_blur_avx, WIDTH, HEIGHT);
    }

    std::cout << "Blur scalar: checksum=" << blur_scalar_sum
              << "  time=" << t_blur_scalar << " ms\n";
    std::cout << "Blur AVX   : checksum=" << blur_avx_sum
              << "  time=" << t_blur_avx << " ms\n";

    const double blur_delta = blur_avx_sum - blur_scalar_sum;
    std::cout << "Checksum delta (AVX - scalar): " << blur_delta << "\n";
    std::cout << "--------------------------------------------------------\n\n";

    // -------------------------------------------------------------------------
    // Workload 3: Complex multiply + FIR convolution
    // -------------------------------------------------------------------------
    ComplexSoA a(N_COMPLEX);
    ComplexSoA b(N_COMPLEX);
    ComplexSoA cmul_scalar(N_COMPLEX);
    ComplexSoA cmul_avx(N_COMPLEX);
    ComplexSoA fir_scalar(N_COMPLEX);
    ComplexSoA fir_avx(N_COMPLEX);

    fill_complex(a, 0x1234abcdU);
    fill_complex(b, 0x9876fedcU);

    const std::vector<f32> fir_kernel = make_fir_kernel();

    double t_cmul_scalar = 0.0;
    double t_cmul_avx    = 0.0;
    double t_fir_scalar  = 0.0;
    double t_fir_avx     = 0.0;

    double cmul_scalar_sum = 0.0;
    double cmul_avx_sum    = 0.0;
    double fir_scalar_sum  = 0.0;
    double fir_avx_sum     = 0.0;

    std::cout << "=== Workload 3: Complex multiply + FIR convolution ===\n";

    {
        ScopedTimer timer("complex_mul_scalar", t_cmul_scalar);
        cmul_scalar_sum = complex_mul_scalar(a, b, cmul_scalar);
    }
    {
        ScopedTimer timer("complex_mul_avx", t_cmul_avx);
        cmul_avx_sum = complex_mul_avx(a, b, cmul_avx);
    }
    {
        ScopedTimer timer("complex_fir_scalar", t_fir_scalar);
        fir_scalar_sum = complex_fir_scalar(cmul_scalar, fir_kernel, fir_scalar);
    }
    {
        ScopedTimer timer("complex_fir_avx", t_fir_avx);
        fir_avx_sum = complex_fir_avx(cmul_avx, fir_kernel, fir_avx);
    }

    std::cout << "Complex mul scalar: checksum=" << cmul_scalar_sum
              << "  time=" << t_cmul_scalar << " ms\n";
    std::cout << "Complex mul AVX   : checksum=" << cmul_avx_sum
              << "  time=" << t_cmul_avx << " ms\n";
    std::cout << "FIR      scalar   : checksum=" << fir_scalar_sum
              << "  time=" << t_fir_scalar << " ms\n";
    std::cout << "FIR      AVX      : checksum=" << fir_avx_sum
              << "  time=" << t_fir_avx << " ms\n";

    const double cmul_delta = cmul_avx_sum - cmul_scalar_sum;
    const double fir_delta  = fir_avx_sum - fir_scalar_sum;

    std::cout << "Delta cmul checksum (AVX - scalar): " << cmul_delta << "\n";
    std::cout << "Delta FIR  checksum (AVX - scalar): " << fir_delta  << "\n";
    std::cout << "--------------------------------------------------------\n\n";

    // -------------------------------------------------------------------------
    // Workload 4: Soft clip / limiter on FIR output
    // -------------------------------------------------------------------------
    std::vector<f32> clip_in_scalar(N_COMPLEX);
    std::vector<f32> clip_in_avx(N_COMPLEX);
    std::vector<f32> clip_out_scalar(N_COMPLEX);
    std::vector<f32> clip_out_avx(N_COMPLEX);

    // Use magnitude of FIR output as input to limiter
    for (std::size_t i = 0; i < N_COMPLEX; ++i)
    {
        const float rs = fir_scalar.re[i];
        const float is = fir_scalar.im[i];
        const float ra = fir_avx.re[i];
        const float ia = fir_avx.im[i];

        clip_in_scalar[i] = std::sqrt(rs * rs + is * is);
        clip_in_avx[i]    = std::sqrt(ra * ra + ia * ia);
    }

    const float clip_threshold = 1.0f;

    double t_clip_scalar = 0.0;
    double t_clip_avx    = 0.0;
    double clip_scalar_sum = 0.0;
    double clip_avx_sum    = 0.0;

    std::cout << "=== Workload 4: Soft clip / limiter on FIR output ===\n";

    {
        ScopedTimer timer("soft_clip_scalar", t_clip_scalar);
        clip_scalar_sum = soft_clip_scalar(clip_in_scalar, clip_out_scalar, clip_threshold);
    }
    {
        ScopedTimer timer("soft_clip_avx", t_clip_avx);
        clip_avx_sum = soft_clip_avx(clip_in_avx, clip_out_avx, clip_threshold);
    }

    std::cout << "Soft clip scalar: checksum=" << clip_scalar_sum
              << "  time=" << t_clip_scalar << " ms\n";
    std::cout << "Soft clip AVX   : checksum=" << clip_avx_sum
              << "  time=" << t_clip_avx << " ms\n";

    const double clip_delta = clip_avx_sum - clip_scalar_sum;
    std::cout << "Delta clip checksum (AVX - scalar): " << clip_delta << "\n";
    std::cout << "\nDone.\n";

    return 0;
}
