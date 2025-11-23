#include "common.h"
#include <stdlib.h>

// Global sink to prevent optimization
volatile float g_sink_f = 0.0f;
volatile uint64_t g_sink_i = 0;

// Helper to sum a vector
float hsum_ps(__m256 v) {
    float *p = (float*)&v;
    float s = 0.0f;
    for(int i=0; i<8; ++i) s += p[i];
    return s;
}

uint64_t hsum_epi(__m256i v) {
    uint64_t *p = (uint64_t*)&v;
    uint64_t s = 0;
    for(int i=0; i<4; ++i) s += p[i];
    return s;
}

int main() {
    printf("Initializing AVX Lifter Test Suite...\n");

    // Initialize inputs with some deterministic non-zero data
    __m256 va = _mm256_set_ps(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
    __m256 vb = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
    __m256 vc = _mm256_set1_ps(0.5f);

    __m256i via = _mm256_set_epi32(10, 20, 30, 40, 50, 60, 70, 80);
    __m256i vib = _mm256_set_epi32(80, 70, 60, 50, 40, 30, 20, 10);

    float total_f = 0.0f;
    uint64_t total_i = 0;

    // --- Run AVX Tests ---
    printf("Testing AVX (Float)...\n");
    
    total_f += hsum_ps(test_vaddps(va, vb));
    total_f += hsum_ps(test_vsubps(va, vb));
    total_f += hsum_ps(test_vmulps(va, vb));
    total_f += hsum_ps(test_vdivps(va, vb));
    
    total_f += hsum_ps(test_vsqrtps(va));
    total_f += hsum_ps(test_vrsqrtps(va));
    total_f += hsum_ps(test_vrcpps(va));
    
    total_f += hsum_ps(test_vmaxps(va, vb));
    total_f += hsum_ps(test_vminps(va, vb));
    
    total_f += hsum_ps(test_vroundps_floor(va));
    total_f += hsum_ps(test_vroundps_ceil(va));
    
    total_f += hsum_ps(test_vhaddps(va, vb));
    total_f += hsum_ps(test_vhsubps(va, vb));
    total_f += hsum_ps(test_vfmadd231ps(va, vb, vc));
    
    total_f += hsum_ps(test_vandps(va, vb));
    total_f += hsum_ps(test_vorps(va, vb));
    total_f += hsum_ps(test_vxorps(va, vb));
    total_f += hsum_ps(test_vandnps(va, vb));
    
    total_f += hsum_ps(test_vcmpps_eq(va, vb));
    total_f += hsum_ps(test_vcmpps_lt(va, vb));
    total_f += hsum_ps(test_vcmpps_le(va, vb));
    total_f += hsum_ps(test_vcmpps_neq(va, vb));
    
    total_f += hsum_ps(test_vpermute_ps_0x1B(va));
    total_f += hsum_ps(test_vpermute2f128_ps_0x01(va, vb));
    total_f += hsum_ps(test_vshuffle_ps_0x1B(va, vb));
    total_f += hsum_ps(test_vblend_ps_0xAA(va, vb));
    
    // Conversions
    total_i += hsum_epi(test_vcvtps2dq(va));
    total_f += hsum_ps(test_vcvtdq2ps(via));

    // --- Run AVX2 Tests ---
    printf("Testing AVX2 (Int)...\n");

    total_i += hsum_epi(test_vpaddb(via, vib));
    total_i += hsum_epi(test_vpaddw(via, vib));
    total_i += hsum_epi(test_vpaddd(via, vib));
    total_i += hsum_epi(test_vpaddq(via, vib));
    
    total_i += hsum_epi(test_vpsubb(via, vib));
    total_i += hsum_epi(test_vpsubw(via, vib));
    total_i += hsum_epi(test_vpsubd(via, vib));
    total_i += hsum_epi(test_vpsubq(via, vib));
    
    total_i += hsum_epi(test_vpadds_b(via, vib));
    total_i += hsum_epi(test_vpadds_w(via, vib));
    
    total_i += hsum_epi(test_vpmulld(via, vib));
    total_i += hsum_epi(test_vpmuludq(via, vib));
    total_i += hsum_epi(test_vpmaddwd(via, vib));
    
    total_i += hsum_epi(test_vpmaxub(via, vib));
    total_i += hsum_epi(test_vpminub(via, vib));
    total_i += hsum_epi(test_vpmaxsd(via, vib));
    total_i += hsum_epi(test_vpminsd(via, vib));
    
    total_i += hsum_epi(test_vpabsd(via));
    total_i += hsum_epi(test_vpsignb(via, vib));
    
    total_i += hsum_epi(test_vpand(via, vib));
    total_i += hsum_epi(test_vpor(via, vib));
    total_i += hsum_epi(test_vpxor(via, vib));
    total_i += hsum_epi(test_vpandn(via, vib));
    
    total_i += hsum_epi(test_vpslld_imm(via));
    total_i += hsum_epi(test_vpsrld_imm(via));
    total_i += hsum_epi(test_vpsrad_imm(via));
    
    total_i += hsum_epi(test_vpsllv_d(via, vib));
    total_i += hsum_epi(test_vpsrlv_d(via, vib));
    total_i += hsum_epi(test_vpsrav_d(via, vib));
    
    total_i += hsum_epi(test_vpcmpeqb(via, vib));
    total_i += hsum_epi(test_vpcmpgtb(via, vib));
    total_i += hsum_epi(test_vpcmpeqd(via, vib));
    total_i += hsum_epi(test_vpcmpgtd(via, vib));
    total_i += hsum_epi(test_vpcmpeqq(via, vib));
    total_i += hsum_epi(test_vpcmpgtq(via, vib));
    
    total_i += hsum_epi(test_vpshufb(via, vib));
    total_i += hsum_epi(test_vpermd(vib, via)); // vib as index
    total_i += hsum_epi(test_vpermq_imm(via));
    total_i += hsum_epi(test_vperm2i128_imm(via, vib));
    total_i += hsum_epi(test_vpalignr_imm(via, vib));
    total_i += hsum_epi(test_vpblendd_imm(via, vib));

    // Sink results
    g_sink_f = total_f;
    g_sink_i = total_i;

    printf("Tests Complete. Checksums: F=%f, I=%lu\n", total_f, total_i);
    return 0;
}
