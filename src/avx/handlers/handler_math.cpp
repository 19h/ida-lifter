/*
 AVX Math Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_v_math_ss_sd(codegen_t &cdg, int elem_size) {
    QASSERT(0xA0500, is_avx_reg(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2));
    mreg_t r = is_mem_op(cdg.insn.Op3) ? cdg.load_operand(2) : reg2mreg(cdg.insn.Op3.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    mcode_t opc = m_nop;
    switch (cdg.insn.itype) {
        case NN_vaddss:
        case NN_vaddsd: opc = m_fadd;
            break;
        case NN_vsubss:
        case NN_vsubsd: opc = m_fsub;
            break;
        case NN_vmulss:
        case NN_vmulsd: opc = m_fmul;
            break;
        case NN_vdivss:
        case NN_vdivsd: opc = m_fdiv;
            break;
    }

    op_dtype_t odt = (elem_size == FLOAT_SIZE) ? dt_float : dt_double;
    mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);
    cdg.emit(m_mov, XMM_SIZE, l, 0, t, 0);
    cdg.emit_micro_mvm(opc, odt, l, r, t, 0);
    cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);
    cdg.mba->free_kreg(t, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_minmax_ss_sd(codegen_t &cdg) {
    QASSERT(0xA0503, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    bool is_double = (cdg.insn.itype == NN_vminsd || cdg.insn.itype == NN_vmaxsd);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t r = is_mem_op(cdg.insn.Op3) ? cdg.load_operand(2) : reg2mreg(cdg.insn.Op3.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *which = (cdg.insn.itype == NN_vminss || cdg.insn.itype == NN_vminsd) ? "min" : "max";
    qstring iname;
    iname.cat_sprnt("_mm_%s_%s", which, is_double ? "sd" : "ss");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(16, false, is_double);
    icall.add_argument_reg(l, vt);
    icall.add_argument_reg(r, vt);
    icall.set_return_reg(d, vt);
    icall.emit();

    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_math_p(codegen_t &cdg) {
    QASSERT(0xA0501, is_avx_reg(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2));

    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t r = is_mem_op(cdg.insn.Op3) ? cdg.load_operand(2) : reg2mreg(cdg.insn.Op3.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *fmt = nullptr;
    bool is_int = false;
    bool is_double = false;

    switch (cdg.insn.itype) {
        // FP basic
        case NN_vaddps: fmt = "_mm%s_add_ps";
            break;
        case NN_vsubps: fmt = "_mm%s_sub_ps";
            break;
        case NN_vmulps: fmt = "_mm%s_mul_ps";
            break;
        case NN_vdivps: fmt = "_mm%s_div_ps";
            break;
        case NN_vaddpd: fmt = "_mm%s_add_pd";
            is_double = true;
            break;
        case NN_vsubpd: fmt = "_mm%s_sub_pd";
            is_double = true;
            break;
        case NN_vmulpd: fmt = "_mm%s_mul_pd";
            is_double = true;
            break;
        case NN_vdivpd: fmt = "_mm%s_div_pd";
            is_double = true;
            break;
        // FP min/max
        case NN_vminps: fmt = "_mm%s_min_ps";
            break;
        case NN_vmaxps: fmt = "_mm%s_max_ps";
            break;
        case NN_vminpd: fmt = "_mm%s_min_pd";
            is_double = true;
            break;
        case NN_vmaxpd: fmt = "_mm%s_max_pd";
            is_double = true;
            break;
        // INT add/sub
        case NN_vpaddb: fmt = "_mm%s_add_epi8";
            is_int = true;
            break;
        case NN_vpsubb: fmt = "_mm%s_sub_epi8";
            is_int = true;
            break;
        case NN_vpaddw: fmt = "_mm%s_add_epi16";
            is_int = true;
            break;
        case NN_vpsubw: fmt = "_mm%s_sub_epi16";
            is_int = true;
            break;
        case NN_vpaddd: fmt = "_mm%s_add_epi32";
            is_int = true;
            break;
        case NN_vpsubd: fmt = "_mm%s_sub_epi32";
            is_int = true;
            break;
        case NN_vpaddq: fmt = "_mm%s_add_epi64";
            is_int = true;
            break;
        case NN_vpsubq: fmt = "_mm%s_sub_epi64";
            is_int = true;
            break;
        // INT min/max (signed)
        case NN_vpminsb: fmt = "_mm%s_min_epi8";
            is_int = true;
            break;
        case NN_vpmaxsb: fmt = "_mm%s_max_epi8";
            is_int = true;
            break;
        case NN_vpminsw: fmt = "_mm%s_min_epi16";
            is_int = true;
            break;
        case NN_vpmaxsw: fmt = "_mm%s_max_epi16";
            is_int = true;
            break;
        case NN_vpminsd: fmt = "_mm%s_min_epi32";
            is_int = true;
            break;
        case NN_vpmaxsd: fmt = "_mm%s_max_epi32";
            is_int = true;
            break;
        // INT min/max (unsigned)
        case NN_vpminub: fmt = "_mm%s_min_epu8";
            is_int = true;
            break;
        case NN_vpmaxub: fmt = "_mm%s_max_epu8";
            is_int = true;
            break;
        case NN_vpminuw: fmt = "_mm%s_min_epu16";
            is_int = true;
            break;
        case NN_vpmaxuw: fmt = "_mm%s_max_epu16";
            is_int = true;
            break;
        case NN_vpminud: fmt = "_mm%s_min_epu32";
            is_int = true;
            break;
        case NN_vpmaxud: fmt = "_mm%s_max_epu32";
            is_int = true;
            break;
        // INT multiply
        case NN_vpmullw: fmt = "_mm%s_mullo_epi16";
            is_int = true;
            break;
        case NN_vpmulld: fmt = "_mm%s_mullo_epi32";
            is_int = true;
            break;
        case NN_vpmulhw: fmt = "_mm%s_mulhi_epi16";
            is_int = true;
            break;
        case NN_vpmulhuw: fmt = "_mm%s_mulhi_epu16";
            is_int = true;
            break;
        case NN_vpmuldq: fmt = "_mm%s_mul_epi32";
            is_int = true;
            break;
        case NN_vpmuludq: fmt = "_mm%s_mul_epu32";
            is_int = true;
            break;
        default: QASSERT(0xA0502, false);
    }

    qstring iname;
    iname.cat_sprnt(fmt, size == YMM_SIZE ? "256" : "");
    AVXIntrinsic icall(&cdg, iname.c_str());

    tinfo_t ti = get_type_robust(size, is_int, is_double);
    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vsqrtss(codegen_t &cdg) {
    QASSERT(0xA0600, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));
    mreg_t r = is_xmm_reg(cdg.insn.Op3) ? reg2mreg(cdg.insn.Op3.reg) : cdg.load_operand(2);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);
    cdg.emit(m_mov, XMM_SIZE, l, 0, t, 0);

    AVXIntrinsic icall(&cdg, "fsqrt");
    icall.add_argument_reg(r, BT_FLOAT);
    icall.set_return_reg_basic(t, BT_FLOAT);
    icall.emit();

    cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);
    cdg.mba->free_kreg(t, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vsqrt_ps_pd(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    bool is_double = (cdg.insn.itype == NN_vsqrtpd);
    mreg_t r = is_mem_op(cdg.insn.Op2) ? cdg.load_operand(1) : reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_sqrt_%s", size == YMM_SIZE ? "256" : "", is_double ? "pd" : "ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_hmath(codegen_t &cdg) {
    QASSERT(0xA0504, is_avx_reg(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2));

    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t r = is_mem_op(cdg.insn.Op3) ? cdg.load_operand(2) : reg2mreg(cdg.insn.Op3.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    bool is_double = (cdg.insn.itype == NN_vhaddpd || cdg.insn.itype == NN_vhsubpd);
    const char *op = (cdg.insn.itype == NN_vhaddps || cdg.insn.itype == NN_vhaddpd) ? "add" : "sub";
    const char *type = is_double ? "pd" : "ps";

    qstring iname;
    iname.cat_sprnt("_mm%s_h%s_%s", size == YMM_SIZE ? "256" : "", op, type);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_dot(codegen_t &cdg) {
    QASSERT(0xA0505, is_avx_reg(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2));

    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t r = is_mem_op(cdg.insn.Op3) ? cdg.load_operand(2) : reg2mreg(cdg.insn.Op3.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Op4 is the immediate mask
    QASSERT(0xA0506, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    qstring iname;
    iname.cat_sprnt("_mm%s_dp_ps", size == YMM_SIZE ? "256" : "");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vrcp_rsqrt(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t r = is_mem_op(cdg.insn.Op2) ? cdg.load_operand(1) : reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *op = (cdg.insn.itype == NN_vrcpps) ? "rcp" : "rsqrt";
    qstring iname;
    iname.cat_sprnt("_mm%s_%s_ps", size == YMM_SIZE ? "256" : "", op);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, false);

    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vround(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    bool is_double = (cdg.insn.itype == NN_vroundpd);
    mreg_t r = is_mem_op(cdg.insn.Op2) ? cdg.load_operand(1) : reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Op3 is the rounding mode immediate
    QASSERT(0xA0507, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value;

    qstring iname;
    iname.cat_sprnt("_mm%s_round_%s", size == YMM_SIZE ? "256" : "", is_double ? "pd" : "ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32); // Rounding mode is int
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
