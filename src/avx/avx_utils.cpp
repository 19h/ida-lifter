/*
AVX Utility Functions and Classification
*/

#include "avx_utils.h"
#include "avx_helpers.h"

#if IDA_SDK_VERSION >= 750

int get_op_size(const insn_t &insn) {
    if (is_xmm_reg(insn.Op1)) return XMM_SIZE;
    if (is_ymm_reg(insn.Op1)) return YMM_SIZE;
    if (is_zmm_reg(insn.Op1)) return ZMM_SIZE;
    return XMM_SIZE;
}

qstring make_intrinsic_name(const char *fmt, int op_size) {
    qstring n;
    const char *prefix = "";
    if (op_size == ZMM_SIZE) prefix = "512";
    else if (op_size == YMM_SIZE) prefix = "256";
    n.cat_sprnt(fmt, prefix);
    return n;
}

tinfo_t get_type_robust(int op_size, bool is_int, bool is_double) {
    return get_vector_type(op_size, is_int, is_double);
}

//----- AVX→SSE simple aliases
struct InsnMapping {
    uint16 avx_itype;
    uint16 sse_itype;
};

// NOTE: keep only aliases that do NOT write an XMM destination (thus no VEX.128 zero-upper is required)
static constexpr InsnMapping avx_to_sse_map[] =
{
    // flag-setting compares (no XMM dest write)
    {NN_vcomiss, NN_comiss}, {NN_vcomisd, NN_comisd}, {NN_vucomiss, NN_ucomiss}, {NN_vucomisd, NN_ucomisd},
    // extract to GPR/mem (no XMM dest write)
    {NN_vpextrb, NN_pextrb}, {NN_vpextrw, NN_pextrw}, {NN_vpextrd, NN_pextrd}, {NN_vpextrq, NN_pextrq},
    // int conversions to GPR (dest is GPR)
    {NN_vcvttss2si, NN_cvttss2si}, {NN_vcvttsd2si, NN_cvttsd2si}, {NN_vcvtsd2si, NN_cvtsd2si}
};

bool try_convert_to_sse(codegen_t &cdg) {
    for (size_t i = 0; i < qnumber(avx_to_sse_map); i++)
        if (cdg.insn.itype == avx_to_sse_map[i].avx_itype) {
            cdg.insn.itype = avx_to_sse_map[i].sse_itype;
            return true;
        }
    return false;
}

//----- classification
bool is_compare_insn(uint16 it) {
    return it == NN_vcomiss || it == NN_vcomisd || it == NN_vucomiss || it == NN_vucomisd;
}

bool is_extract_insn(uint16 it) {
    return it == NN_vpextrb || it == NN_vpextrw || it == NN_vpextrd || it == NN_vpextrq;
}

bool is_conversion_insn(uint16 it) {
    return it == NN_vcvttss2si || it == NN_vcvtdq2ps || it == NN_vcvtsi2ss || it == NN_vcvtps2pd ||
           it == NN_vcvtss2sd || it == NN_vcvttsd2si || it == NN_vcvtsd2si || it == NN_vcvtpd2ps ||
           it == NN_vcvttpd2dq || it == NN_vcvtpd2dq || it == NN_vcvttps2dq || it == NN_vcvtps2dq ||
           it == NN_vcvtsi2sd || it == NN_vcvtsd2ss || it == NN_vcvtdq2pd ||
           it == NN_vcvtps2udq || it == NN_vcvttps2udq || it == NN_vcvtudq2ps ||
           it == NN_vcvtudq2pd || it == NN_vcvtpd2udq || it == NN_vcvttpd2udq;
}

bool is_sad_insn(uint16 it) {
    return it == NN_vpsadbw || it == NN_vmpsadbw;
}

bool is_move_insn(uint16 it) {
    // Note: vmovss/vmovsd scalar moves are excluded to let IDA handle them natively
    return it == NN_vmovd || it == NN_vmovq ||
           it == NN_vmovaps || it == NN_vmovups || it == NN_vmovdqa || it == NN_vmovdqu ||
           it == NN_vmovapd || it == NN_vmovupd ||
           // AVX-512 move variants
           it == NN_vmovdqa32 || it == NN_vmovdqa64 ||
           it == NN_vmovdqu8 || it == NN_vmovdqu16 || it == NN_vmovdqu32 || it == NN_vmovdqu64;
}

bool is_bitwise_insn(uint16 it) {
    return it == NN_vpor || it == NN_vorps || it == NN_vorpd ||
           it == NN_vpand || it == NN_vandps || it == NN_vandpd ||
           it == NN_vpxor || it == NN_vxorps || it == NN_vxorpd ||
           it == NN_vandnps || it == NN_vandnpd || it == NN_vpandn ||
           it == NN_vpandd || it == NN_vpandq || it == NN_vpandnd || it == NN_vpandnq ||
           it == NN_vpord || it == NN_vporq || it == NN_vpxord || it == NN_vpxorq;
}

bool is_scalar_minmax(uint16 it) {
    return it == NN_vminss || it == NN_vminsd || it == NN_vmaxss || it == NN_vmaxsd;
}

bool is_scalar_math(uint16 it) {
    return it == NN_vaddss || it == NN_vsubss || it == NN_vmulss || it == NN_vdivss ||
           it == NN_vaddsd || it == NN_vsubsd || it == NN_vmulsd || it == NN_vdivsd;
}

bool is_scalar_move(uint16 it) {
    return it == NN_vmovss || it == NN_vmovsd;
}

bool is_vzeroupper(uint16 it) {
    return it == NN_vzeroupper;
}

bool is_packed_minmax_fp(uint16 it) {
    return it == NN_vminps || it == NN_vminpd || it == NN_vmaxps || it == NN_vmaxpd;
}

bool is_packed_minmax_int(uint16 it) {
    return it == NN_vpminsb || it == NN_vpminsw || it == NN_vpminsd ||
           it == NN_vpminub || it == NN_vpminuw || it == NN_vpminud ||
           it == NN_vpminsq || it == NN_vpminuq ||
           it == NN_vpmaxsb || it == NN_vpmaxsw || it == NN_vpmaxsd ||
           it == NN_vpmaxub || it == NN_vpmaxuw || it == NN_vpmaxud ||
           it == NN_vpmaxsq || it == NN_vpmaxuq;
}

bool is_int_mul(uint16 it) {
    return it == NN_vpmullw || it == NN_vpmulld || it == NN_vpmullq || it == NN_vpmulhw || it == NN_vpmulhuw ||
           it == NN_vpmuldq || it == NN_vpmuludq || it == NN_vpmaddwd || it == NN_vpmaddubsw;
}

bool is_avg_insn(uint16 it) {
    return it == NN_vpavgb || it == NN_vpavgw;
}

bool is_abs_insn(uint16 it) {
    return it == NN_vpabsb || it == NN_vpabsw || it == NN_vpabsd || it == NN_vpabsq;
}

bool is_sign_insn(uint16 it) {
    return it == NN_vpsignb || it == NN_vpsignw || it == NN_vpsignd;
}

bool is_shift_insn(uint16 it) {
    return it == NN_vpsllw || it == NN_vpslld || it == NN_vpsllq ||
           it == NN_vpsrlw || it == NN_vpsrld || it == NN_vpsrlq ||
           it == NN_vpsraw || it == NN_vpsrad || it == NN_vpsraq;
}

bool is_var_shift_insn(uint16 it) {
    return it == NN_vpsllvd || it == NN_vpsllvq ||
           it == NN_vpsrlvd || it == NN_vpsrlvq ||
           it == NN_vpsravd || it == NN_vpsravq;
}

bool is_rotate_insn(uint16 it) {
    return it == NN_vprold || it == NN_vprord || it == NN_vprolq || it == NN_vprorq;
}

bool is_var_rotate_insn(uint16 it) {
    return it == NN_vprolvd || it == NN_vprorvd || it == NN_vprolvq || it == NN_vprorvq;
}

bool is_shift_double_insn(uint16 it) {
    return it == NN_vpshldw || it == NN_vpshldd || it == NN_vpshldq ||
           it == NN_vpshldvw || it == NN_vpshldvd || it == NN_vpshldvq ||
           it == NN_vpshrdw || it == NN_vpshrdd || it == NN_vpshrdq ||
           it == NN_vpshrdvw || it == NN_vpshrdvd || it == NN_vpshrdvq;
}

bool is_multishift_insn(uint16 it) {
    return it == NN_vpmultishiftqb;
}

bool is_fp16_packed_math_insn(uint16 it) {
    return it == NN_vaddph || it == NN_vsubph || it == NN_vmulph || it == NN_vdivph ||
           it == NN_vminph || it == NN_vmaxph;
}

bool is_fp16_scalar_math_insn(uint16 it) {
    return it == NN_vaddsh || it == NN_vsubsh || it == NN_vmulsh || it == NN_vdivsh ||
           it == NN_vminsh || it == NN_vmaxsh;
}

bool is_fp16_sqrt_insn(uint16 it) {
    return it == NN_vsqrtph;
}

bool is_fp16_fma_insn(uint16 it) {
    return it == NN_vfmadd132ph || it == NN_vfmadd213ph || it == NN_vfmadd231ph ||
           it == NN_vfmadd132sh || it == NN_vfmadd213sh || it == NN_vfmadd231sh;
}

bool is_fp16_fmaddsub_insn(uint16 it) {
    return it == NN_vfmaddsub132ph || it == NN_vfmaddsub213ph || it == NN_vfmaddsub231ph;
}

bool is_fp16_complex_insn(uint16 it) {
    return it == NN_vfcmulcph || it == NN_vfmulcph || it == NN_vfcmaddcph || it == NN_vfmaddcph;
}

bool is_shuffle_insn(uint16 it) {
    return it == NN_vpshufb || it == NN_vpshufd || it == NN_vpshufhw || it == NN_vpshuflw;
}

bool is_perm_insn(uint16 it) {
    return it == NN_vpermq || it == NN_vpermd || it == NN_vpermilps || it == NN_vpermilpd;
}

bool is_permutex_insn(uint16 it) {
    return it == NN_vpermb || it == NN_vpermw;
}

bool is_permutex2_insn(uint16 it) {
    return it == NN_vpermt2b || it == NN_vpermt2w || it == NN_vpermt2d ||
           it == NN_vpermt2q || it == NN_vpermt2ps || it == NN_vpermt2pd;
}

bool is_align_insn(uint16 it) {
    return it == NN_vpalignr || it == NN_valignd || it == NN_valignq;
}

bool is_gather_insn(uint16 it) {
    return it == NN_vgatherdps || it == NN_vgatherdpd ||
           it == NN_vgatherqps || it == NN_vgatherqpd ||
           it == NN_vpgatherdd || it == NN_vpgatherdq ||
           it == NN_vpgatherqd || it == NN_vpgatherqq;
}

bool is_scatter_insn(uint16 it) {
    return it == NN_vscatterdps || it == NN_vscatterdpd ||
           it == NN_vscatterqps || it == NN_vscatterqpd ||
           it == NN_vpscatterdd || it == NN_vpscatterdq ||
           it == NN_vpscatterqd || it == NN_vpscatterqq;
}

bool is_compress_insn(uint16 it) {
    return it == NN_vcompressps || it == NN_vcompresspd ||
           it == NN_vpcompressd || it == NN_vpcompressq ||
           it == NN_vpcompressb || it == NN_vpcompressw;
}

bool is_expand_insn(uint16 it) {
    return it == NN_vexpandps || it == NN_vexpandpd ||
           it == NN_vpexpandd || it == NN_vpexpandq ||
           it == NN_vpexpandb || it == NN_vpexpandw;
}

bool is_fma_insn(uint16 it) {
    return (it >= NN_vfmadd132ps && it <= NN_vfmadd231sd) ||
           (it >= NN_vfmsub132ps && it <= NN_vfmsub231sd) ||
           (it >= NN_vfnmadd132ps && it <= NN_vfnmadd231sd) ||
           (it >= NN_vfnmsub132ps && it <= NN_vfnmsub231sd);
}

bool is_ifma_insn(uint16 it) {
    return it == NN_vpmadd52luq || it == NN_vpmadd52huq;
}

bool is_vnni_insn(uint16 it) {
    return it == NN_vpdpbusd || it == NN_vpdpbusds ||
           it == NN_vpdpwssd || it == NN_vpdpwssds;
}

bool is_bf16_insn(uint16 it) {
    return it == NN_vdpbf16ps || it == NN_vcvtne2ps2bf16 || it == NN_vcvtneps2bf16;
}

bool is_fp16_move_insn(uint16 it) {
    return it == NN_vmovsh || it == NN_vmovw;
}

bool is_ternary_logic_insn(uint16 it) {
    return it == NN_vpternlogd || it == NN_vpternlogq;
}

bool is_conflict_insn(uint16 it) {
    return it == NN_vpconflictd || it == NN_vpconflictq;
}

bool is_popcnt_insn(uint16 it) {
    return it == NN_vpopcntb || it == NN_vpopcntw || it == NN_vpopcntd || it == NN_vpopcntq;
}

bool is_lzcnt_insn(uint16 it) {
    return it == NN_vplzcntd || it == NN_vplzcntq;
}

bool is_gfni_insn(uint16 it) {
    return it == NN_vgf2p8affineqb || it == NN_vgf2p8affineinvqb || it == NN_vgf2p8mulb;
}

bool is_pclmul_insn(uint16 it) {
    return it == NN_vpclmulqdq;
}

bool is_aes_insn(uint16 it) {
    return it == NN_vaesenc || it == NN_vaesenclast || it == NN_vaesdec || it == NN_vaesdeclast;
}

bool is_math_insn(uint16 it) {
    // Note: Scalar operations (vaddss/sd, vsubss/sd, vmulss/sd, vdivss/sd) are excluded
    // to let IDA handle them natively, avoiding type/verification issues
    return it == NN_vaddps || it == NN_vsubps || it == NN_vmulps || it == NN_vdivps ||
           it == NN_vaddpd || it == NN_vsubpd || it == NN_vmulpd || it == NN_vdivpd ||
           it == NN_vpaddb || it == NN_vpsubb || it == NN_vpaddw || it == NN_vpsubw ||
           it == NN_vpaddd || it == NN_vpsubd || it == NN_vpaddq || it == NN_vpsubq ||
           it == NN_vpaddsb || it == NN_vpsubsb || it == NN_vpaddsw || it == NN_vpsubsw ||
           is_packed_minmax_fp(it) || is_packed_minmax_int(it) || is_int_mul(it) ||
           is_avg_insn(it) || is_abs_insn(it) || is_sign_insn(it) ||
           is_shift_insn(it) || is_var_shift_insn(it) ||
           is_shuffle_insn(it) || is_perm_insn(it) || is_align_insn(it) ||
           is_gather_insn(it) || is_fma_insn(it);
}

bool is_packed_math_insn(uint16 it) {
    return it == NN_vaddps || it == NN_vsubps || it == NN_vmulps || it == NN_vdivps ||
           it == NN_vaddpd || it == NN_vsubpd || it == NN_vmulpd || it == NN_vdivpd ||
           it == NN_vpaddb || it == NN_vpsubb || it == NN_vpaddw || it == NN_vpsubw ||
           it == NN_vpaddd || it == NN_vpsubd || it == NN_vpaddq || it == NN_vpsubq ||
           it == NN_vpaddsb || it == NN_vpsubsb || it == NN_vpaddsw || it == NN_vpsubsw ||
           is_packed_minmax_fp(it) || is_packed_minmax_int(it) || is_int_mul(it) ||
           is_avg_insn(it);
}

bool is_broadcast_insn(uint16 it) {
    return it == NN_vbroadcastss || it == NN_vbroadcastsd || it == NN_vbroadcastf128 || it == NN_vbroadcasti128;
}

bool is_misc_insn(uint16 it) {
    return it == NN_vsqrtss || it == NN_vsqrtps || it == NN_vsqrtpd ||
           it == NN_vshufps || it == NN_vshufpd ||
           it == NN_vpermpd;
}

bool is_extract_insert_insn(uint16 it) {
    return it == NN_vextractf128 || it == NN_vinsertf128 ||
           it == NN_vextracti128 || it == NN_vinserti128 ||
           it == NN_vextracti32x4 || it == NN_vextracti32x8 ||
           it == NN_vextracti64x4 || it == NN_vinserti32x4 ||
           it == NN_vinserti32x8 || it == NN_vinserti64x4;
}

bool is_movdup_insn(uint16 it) {
    return it == NN_vmovshdup || it == NN_vmovsldup ||
           it == NN_vmovddup;
}

bool is_unpack_insn(uint16 it) {
    return it == NN_vunpckhps || it == NN_vunpcklps ||
           it == NN_vunpckhpd || it == NN_vunpcklpd;
}

bool is_blend_insn(uint16 it) {
    return it == NN_vblendvps || it == NN_vblendvpd ||
           it == NN_vblendps || it == NN_vblendpd ||
           it == NN_vpblendd || it == NN_vpblendw || it == NN_vpblendvb;
}

bool is_maskmov_insn(uint16 it) { return it == NN_vmaskmovps || it == NN_vmaskmovpd; }

bool is_packed_compare_insn(uint16 it) {
    return (it >= NN_vcmpeqps && it <= NN_vcmptrue_usps) ||
           (it >= NN_vcmpeqpd && it <= NN_vcmptrue_uspd) ||
           (it >= NN_vcmpeqss && it <= NN_vcmptrue_usss) ||
           (it >= NN_vcmpeqsd && it <= NN_vcmptrue_ussd);
}

bool is_packed_int_compare_insn(uint16 it) {
    return it == NN_vpcmpeqb || it == NN_vpcmpeqw || it == NN_vpcmpeqd || it == NN_vpcmpeqq ||
           it == NN_vpcmpgtb || it == NN_vpcmpgtw || it == NN_vpcmpgtd || it == NN_vpcmpgtq;
}

uint8 get_cmp_predicate(uint16 it) {
    if (it >= NN_vcmpeqps && it <= NN_vcmptrue_usps) return (uint8) (it - NN_vcmpeqps);
    if (it >= NN_vcmpeqpd && it <= NN_vcmptrue_uspd) return (uint8) (it - NN_vcmpeqpd);
    if (it >= NN_vcmpeqss && it <= NN_vcmptrue_usss) return (uint8) (it - NN_vcmpeqss);
    if (it >= NN_vcmpeqsd && it <= NN_vcmptrue_ussd) return (uint8) (it - NN_vcmpeqsd);
    return 0;
}

bool is_horizontal_math(uint16 it) {
    return it == NN_vhaddps || it == NN_vhaddpd || it == NN_vhsubps || it == NN_vhsubpd ||
           it == NN_vphaddw || it == NN_vphaddsw || it == NN_vphaddd || it == NN_vphsubd;
}

bool is_dot_product(uint16 it) {
    return it == NN_vdpps;
}

bool is_approx_insn(uint16 it) {
    return it == NN_vrcpps || it == NN_vrsqrtps ||
           it == NN_vrcp14ps || it == NN_vrsqrt14ps ||
           it == NN_vrcp14pd || it == NN_vrsqrt14pd;
}

bool is_scalar_approx_insn(uint16 it) {
    return it == NN_vrcpss || it == NN_vrsqrtss;
}

bool is_round_insn(uint16 it) {
    return it == NN_vroundps || it == NN_vroundpd ||
           it == NN_vrndscaleps || it == NN_vrndscalepd;
}

bool is_scalar_round_insn(uint16 it) {
    return it == NN_vroundss || it == NN_vroundsd;
}

bool is_getexp_insn(uint16 it) {
    return it == NN_vgetexpps || it == NN_vgetexppd;
}

bool is_getmant_insn(uint16 it) {
    return it == NN_vgetmantps || it == NN_vgetmantpd;
}

bool is_fixupimm_insn(uint16 it) {
    return it == NN_vfixupimmps || it == NN_vfixupimmpd;
}

bool is_scalef_insn(uint16 it) {
    return it == NN_vscalefps || it == NN_vscalefpd;
}

bool is_range_insn(uint16 it) {
    return it == NN_vrangeps || it == NN_vrangepd;
}

bool is_reduce_insn(uint16 it) {
    return it == NN_vreduceps || it == NN_vreducepd;
}

bool is_addsub_insn(uint16 it) {
    return it == NN_vaddsubps || it == NN_vaddsubpd;
}

bool is_vpbroadcast_d_q(uint16 it) {
    return it == NN_vpbroadcastd || it == NN_vpbroadcastq;
}

bool is_vperm2_insn(uint16 it) {
    return it == NN_vperm2f128 || it == NN_vperm2i128;
}

bool is_phsub_insn(uint16 it) {
    return it == NN_vphsubw || it == NN_vphsubsw || it == NN_vphsubd;
}

bool is_pack_insn(uint16 it) {
    return it == NN_vpackssdw || it == NN_vpacksswb ||
           it == NN_vpackusdw || it == NN_vpackuswb;
}

bool is_ptest_insn(uint16 it) {
    return it == NN_vptest;
}

bool is_fmaddsub_insn(uint16 it) {
    return (it >= NN_vfmaddsub132ps && it <= NN_vfmaddsub231pd) ||
           (it >= NN_vfmsubadd132ps && it <= NN_vfmsubadd231pd);
}

bool is_movmsk_insn(uint16 it) {
    return it == NN_vmovmskps || it == NN_vmovmskpd || it == NN_vpmovmskb;
}

bool is_movnt_insn(uint16 it) {
    return it == NN_vmovntps || it == NN_vmovntpd || it == NN_vmovntdq || it == NN_vmovntdqa;
}

bool is_mask_to_vec_insn(uint16 it) {
    return it == NN_vpmovm2b || it == NN_vpmovm2w || it == NN_vpmovm2d || it == NN_vpmovm2q;
}

bool is_vpbroadcast_b_w(uint16 it) {
    return it == NN_vpbroadcastb || it == NN_vpbroadcastw;
}

bool is_pinsert_insn(uint16 it) {
    return it == NN_vpinsrb || it == NN_vpinsrw || it == NN_vpinsrd || it == NN_vpinsrq;
}

bool is_pmovsx_insn(uint16 it) {
    return it == NN_vpmovsxbw || it == NN_vpmovsxbd || it == NN_vpmovsxbq ||
           it == NN_vpmovsxwd || it == NN_vpmovsxwq || it == NN_vpmovsxdq;
}

bool is_pmovzx_insn(uint16 it) {
    return it == NN_vpmovzxbw || it == NN_vpmovzxbd || it == NN_vpmovzxbq ||
           it == NN_vpmovzxwd || it == NN_vpmovzxwq || it == NN_vpmovzxdq;
}

bool is_byte_shift_insn(uint16 it) {
    return it == NN_vpslldq || it == NN_vpsrldq;
}

bool is_punpck_insn(uint16 it) {
    return it == NN_vpunpckhbw || it == NN_vpunpcklbw ||
           it == NN_vpunpckhwd || it == NN_vpunpcklwd ||
           it == NN_vpunpckhdq || it == NN_vpunpckldq ||
           it == NN_vpunpckhqdq || it == NN_vpunpcklqdq;
}

bool is_extractps_insn(uint16 it) {
    return it == NN_vextractps;
}

bool is_insertps_insn(uint16 it) {
    return it == NN_vinsertps;
}

qstring make_masked_intrinsic_name(const char *base_name, const MaskInfo &mask_info) {
    if (!mask_info.has_mask) {
        return qstring(base_name);
    }

    // Find the position after "_mm" or "_mm256" or "_mm512" prefix
    // Pattern: _mm[256|512]_<op>_<type>
    // Masked: _mm[256|512]_mask[z]_<op>_<type>
    qstring result;
    const char *p = base_name;

    // Copy "_mm" prefix
    if (strncmp(p, "_mm", 3) == 0) {
        result.append("_mm");
        p += 3;

        // Check for "256" or "512" suffix
        if (strncmp(p, "512", 3) == 0) {
            result.append("512");
            p += 3;
        } else if (strncmp(p, "256", 3) == 0) {
            result.append("256");
            p += 3;
        }

        // Skip the underscore before operation name
        if (*p == '_') {
            result.append("_");
            p++;
        }

        // Insert "mask" or "maskz"
        if (mask_info.is_zeroing) {
            result.append("maskz_");
        } else {
            result.append("mask_");
        }

        // Append the rest (operation and type suffix)
        result.append(p);
    } else {
        // Fallback: just return original name
        result = base_name;
    }

    return result;
}

// Load the opmask register value into a microcode register
// K-registers are not fully supported by Hex-Rays microcode (reg2mreg returns mr_none).
// Instead, we encode the k-register number as a negative value in mask_reg.
// add_argument_mask() will then pass this as an immediate constant.
//
// Encoding: k0 -> -1, k1 -> -2, ..., k7 -> -8
// Decoding in add_argument_mask: kreg_num = -(mask_reg + 1)
mreg_t load_mask_operand(codegen_t &cdg, MaskInfo &mask) {
    if (!mask.has_mask)
        return mr_none;

    // Check if Op6 is a k-register
    if (cdg.insn.Op6.type != o_kreg && cdg.insn.Op6.type != o_reg) {
        return mr_none;
    }

    // Verify it's a valid opmask register (k1-k7, k0 means no masking)
    if (cdg.insn.Op6.reg < R_k1 || cdg.insn.Op6.reg > R_k7) {
        return mr_none;
    }

    // Get the k-register number (0-7)
    int kreg_num = cdg.insn.Op6.reg - R_k0;

    // Store the k-register number encoded as negative value
    // This signals to add_argument_mask() to pass it as immediate
    mask.mask_reg = (mreg_t)(-(kreg_num + 1));

    return mask.mask_reg;
}

#endif // IDA_SDK_VERSION >= 750
