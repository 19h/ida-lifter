/*
 AVX Utility Functions and Classification
*/

#include "avx_utils.h"
#include "avx_helpers.h"

#if IDA_SDK_VERSION >= 750

mreg_t load_op_reg_or_mem(codegen_t &cdg, int op_idx, const op_t &op) {
    if (is_mem_op(op)) {
        return cdg.load_operand(op_idx);
    } else {
        return reg2mreg(op.reg);
    }
}

int get_op_size(const insn_t &insn) {
    if (is_xmm_reg(insn.Op1)) return XMM_SIZE;
    if (is_ymm_reg(insn.Op1)) return YMM_SIZE;
    if (is_zmm_reg(insn.Op1)) return ZMM_SIZE;
    return XMM_SIZE;
}

qstring make_intrinsic_name(const char *fmt, int op_size) {
    qstring n;
    n.cat_sprnt(fmt, op_size == YMM_SIZE ? "256" : "");
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
           it == NN_vcvtsi2sd || it == NN_vcvtsd2ss;
}

bool is_move_insn(uint16 it) {
    return it == NN_vmovd || it == NN_vmovq || it == NN_vmovss || it == NN_vmovsd ||
           it == NN_vmovaps || it == NN_vmovups || it == NN_vmovdqa || it == NN_vmovdqu;
}

bool is_bitwise_insn(uint16 it) {
    return it == NN_vpor || it == NN_vorps || it == NN_vorpd || it == NN_vpand || it == NN_vandps || it == NN_vandpd ||
           it == NN_vpxor || it == NN_vxorps || it == NN_vxorpd;
}

bool is_scalar_minmax(uint16 it) {
    return it == NN_vminss || it == NN_vminsd || it == NN_vmaxss || it == NN_vmaxsd;
}

bool is_packed_minmax_fp(uint16 it) {
    return it == NN_vminps || it == NN_vminpd || it == NN_vmaxps || it == NN_vmaxpd;
}

bool is_packed_minmax_int(uint16 it) {
    return it == NN_vpminsb || it == NN_vpminsw || it == NN_vpminsd ||
           it == NN_vpminub || it == NN_vpminuw || it == NN_vpminud ||
           it == NN_vpmaxsb || it == NN_vpmaxsw || it == NN_vpmaxsd ||
           it == NN_vpmaxub || it == NN_vpmaxuw || it == NN_vpmaxud;
}

bool is_int_mul(uint16 it) {
    return it == NN_vpmullw || it == NN_vpmulld || it == NN_vpmulhw || it == NN_vpmulhuw ||
           it == NN_vpmuldq || it == NN_vpmuludq;
}

bool is_math_insn(uint16 it) {
    return it == NN_vaddss || it == NN_vsubss || it == NN_vmulss || it == NN_vdivss ||
           it == NN_vaddsd || it == NN_vsubsd || it == NN_vmulsd || it == NN_vdivsd ||
           it == NN_vaddps || it == NN_vsubps || it == NN_vmulps || it == NN_vdivps ||
           it == NN_vaddpd || it == NN_vsubpd || it == NN_vmulpd || it == NN_vdivpd ||
           it == NN_vpaddb || it == NN_vpsubb || it == NN_vpaddw || it == NN_vpsubw ||
           it == NN_vpaddd || it == NN_vpsubd || it == NN_vpaddq || it == NN_vpsubq ||
           is_scalar_minmax(it) || is_packed_minmax_fp(it) || is_packed_minmax_int(it) || is_int_mul(it);
}

bool is_broadcast_insn(uint16 it) {
    return it == NN_vbroadcastss || it == NN_vbroadcastsd || it == NN_vbroadcastf128 || it == NN_vbroadcasti128;
}

bool is_misc_insn(uint16 it) {
    return it == NN_vsqrtss || it == NN_vsqrtps || it == NN_vsqrtpd || it == NN_vshufps || it == NN_vzeroupper;
}

bool is_blend_insn(uint16 it) { return it == NN_vblendvps || it == NN_vblendvpd; }
bool is_maskmov_insn(uint16 it) { return it == NN_vmaskmovps || it == NN_vmaskmovpd; }

bool is_packed_compare_insn(uint16 it) {
    return (it >= NN_vcmpeqps && it <= NN_vcmptrue_usps) ||
           (it >= NN_vcmpeqpd && it <= NN_vcmptrue_uspd) ||
           (it >= NN_vcmpeqss && it <= NN_vcmptrue_usss) ||
           (it >= NN_vcmpeqsd && it <= NN_vcmptrue_ussd);
}

uint8 get_cmp_predicate(uint16 it) {
    if (it >= NN_vcmpeqps && it <= NN_vcmptrue_usps) return (uint8) (it - NN_vcmpeqps);
    if (it >= NN_vcmpeqpd && it <= NN_vcmptrue_uspd) return (uint8) (it - NN_vcmpeqpd);
    if (it >= NN_vcmpeqss && it <= NN_vcmptrue_usss) return (uint8) (it - NN_vcmpeqss);
    if (it >= NN_vcmpeqsd && it <= NN_vcmptrue_ussd) return (uint8) (it - NN_vcmpeqsd);
    return 0;
}

bool is_horizontal_math(uint16 it) {
    return it == NN_vhaddps || it == NN_vhaddpd || it == NN_vhsubps || it == NN_vhsubpd;
}

bool is_dot_product(uint16 it) {
    return it == NN_vdpps;
}

bool is_approx_insn(uint16 it) {
    return it == NN_vrcpps || it == NN_vrsqrtps;
}

bool is_round_insn(uint16 it) {
    return it == NN_vroundps || it == NN_vroundpd;
}

#endif // IDA_SDK_VERSION >= 750
