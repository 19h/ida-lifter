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
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *op = nullptr;
    const char *suf = (elem_size == FLOAT_SIZE) ? "ss" : "sd";
    switch (cdg.insn.itype) {
        case NN_vaddss:
        case NN_vaddsd: op = "add";
            break;
        case NN_vsubss:
        case NN_vsubsd: op = "sub";
            break;
        case NN_vmulss:
        case NN_vmulsd: op = "mul";
            break;
        case NN_vdivss:
        case NN_vdivsd: op = "div";
            break;
        default: return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm_%s_%s", op, suf);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vec_type = get_type_robust(XMM_SIZE, false, (elem_size == DOUBLE_SIZE));

    icall.add_argument_reg(l, vec_type);

    mreg_t r_arg = r_in;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        // For scalar loads, just promote to XMM size directly
        // The intrinsic expects an XMM register, and only the lower elem_size matters
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);

        // Move the loaded scalar float directly to XMM-sized temp
        // Upper bits are undefined but the intrinsic only uses the lower scalar
        minsn_t *mov = cdg.emit(m_mov, elem_size, r_in, 0, t_mem, 0);
        mov->set_fpinsn();  // Mark as FP operation

        r_arg = t_mem;
    }
    icall.add_argument_reg(r_arg, vec_type);

    icall.set_return_reg(d, vec_type);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);

    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_minmax_ss_sd(codegen_t &cdg) {
    QASSERT(0xA0503, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    bool is_double = (cdg.insn.itype == NN_vminsd || cdg.insn.itype == NN_vmaxsd);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
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
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
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
        // INT saturating add/sub
        case NN_vpaddsb: fmt = "_mm%s_adds_epi8";
            is_int = true;
            break;
        case NN_vpsubsb: fmt = "_mm%s_subs_epi8";
            is_int = true;
            break;
        case NN_vpaddsw: fmt = "_mm%s_adds_epi16";
            is_int = true;
            break;
        case NN_vpsubsw: fmt = "_mm%s_subs_epi16";
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
        // INT average
        case NN_vpavgb: fmt = "_mm%s_avg_epu8";
            is_int = true;
            break;
        case NN_vpavgw: fmt = "_mm%s_avg_epu16";
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

merror_t handle_v_abs(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *suffix = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpabsb: suffix = "epi8";
            break;
        case NN_vpabsw: suffix = "epi16";
            break;
        case NN_vpabsd: suffix = "epi32";
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_abs_%s", size == YMM_SIZE ? "256" : "", suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_sign(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *suffix = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpsignb: suffix = "epi8";
            break;
        case NN_vpsignw: suffix = "epi16";
            break;
        case NN_vpsignd: suffix = "epi32";
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_sign_%s", size == YMM_SIZE ? "256" : "", suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);
    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_fma(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t op1 = reg2mreg(cdg.insn.Op1.reg); // Dest/Src1
    mreg_t op2 = reg2mreg(cdg.insn.Op2.reg); // Src2
    AvxOpLoader op3_in(cdg, 2, cdg.insn.Op3); // Src3

    const char *op = nullptr;
    const char *type = nullptr;
    int order = 0; // 132, 213, 231
    bool is_scalar = false;
    bool is_double = false;

    uint16 it = cdg.insn.itype;

    auto check = [&](uint16 base, const char *t, bool dbl, bool scl) {
        if (it == base) {
            type = t;
            order = 132;
            is_double = dbl;
            is_scalar = scl;
            return true;
        }
        if (it == base + 1) {
            type = t;
            order = 213;
            is_double = dbl;
            is_scalar = scl;
            return true;
        }
        if (it == base + 2) {
            type = t;
            order = 231;
            is_double = dbl;
            is_scalar = scl;
            return true;
        }
        return false;
    };

    if (check(NN_vfmadd132ps, "ps", false, false)) { op = "fmadd"; } else if (
        check(NN_vfmadd132pd, "pd", true, false)) { op = "fmadd"; } else if (
        check(NN_vfmadd132ss, "ss", false, true)) { op = "fmadd"; } else if (
        check(NN_vfmadd132sd, "sd", true, true)) { op = "fmadd"; } else if (
        check(NN_vfmsub132ps, "ps", false, false)) { op = "fmsub"; } else if (
        check(NN_vfmsub132pd, "pd", true, false)) { op = "fmsub"; } else if (
        check(NN_vfmsub132ss, "ss", false, true)) { op = "fmsub"; } else if (
        check(NN_vfmsub132sd, "sd", true, true)) { op = "fmsub"; } else if (
        check(NN_vfnmadd132ps, "ps", false, false)) { op = "fnmadd"; } else if (
        check(NN_vfnmadd132pd, "pd", true, false)) { op = "fnmadd"; } else if (
        check(NN_vfnmadd132ss, "ss", false, true)) { op = "fnmadd"; } else if (
        check(NN_vfnmadd132sd, "sd", true, true)) { op = "fnmadd"; } else if (
        check(NN_vfnmsub132ps, "ps", false, false)) { op = "fnmsub"; } else if (
        check(NN_vfnmsub132pd, "pd", true, false)) { op = "fnmsub"; } else if (
        check(NN_vfnmsub132ss, "ss", false, true)) { op = "fnmsub"; } else if (
        check(NN_vfnmsub132sd, "sd", true, true)) { op = "fnmsub"; } else return MERR_INSN;

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_%s", (size == YMM_SIZE && !is_scalar) ? "256" : "", op, type);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    mreg_t op3 = op3_in;
    mreg_t t_mem = mr_none;
    if (is_scalar && is_mem_op(cdg.insn.Op3)) {
        int elem_size = is_double ? 8 : 4;
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(op3_in, elem_size);
        mop_t dst(t_mem, XMM_SIZE);
        mop_t r;
        cdg.emit(m_xdu, &src, &r, &dst);
        op3 = t_mem;
    }

    // Argument ordering
    // 132: Op1 * Op3 + Op2 -> (Op1, Op3, Op2)
    // 213: Op2 * Op1 + Op3 -> (Op2, Op1, Op3)
    // 231: Op2 * Op3 + Op1 -> (Op2, Op3, Op1)

    mreg_t arg1, arg2, arg3;
    if (order == 132) {
        arg1 = op1;
        arg2 = op3;
        arg3 = op2;
    } else if (order == 213) {
        arg1 = op2;
        arg2 = op1;
        arg3 = op3;
    } else {
        arg1 = op2;
        arg2 = op3;
        arg3 = op1;
    } // 231

    icall.add_argument_reg(arg1, ti);
    icall.add_argument_reg(arg2, ti);
    icall.add_argument_reg(arg3, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vsqrtss(codegen_t &cdg) {
    QASSERT(0xA0600, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
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
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
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
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    bool is_double = (cdg.insn.itype == NN_vhaddpd || cdg.insn.itype == NN_vhsubpd);
    bool is_int = (cdg.insn.itype == NN_vphaddw || cdg.insn.itype == NN_vphaddsw ||
                   cdg.insn.itype == NN_vphaddd || cdg.insn.itype == NN_vphsubd);

    const char *op = nullptr;
    const char *type = nullptr;

    if (is_int) {
        switch (cdg.insn.itype) {
            case NN_vphaddw: op = "hadd";
                type = "epi16";
                break;
            case NN_vphaddsw: op = "hadds";
                type = "epi16";
                break;
            case NN_vphaddd: op = "hadd";
                type = "epi32";
                break;
            case NN_vphsubd: op = "hsub";
                type = "epi32";
                break;
        }
    } else {
        op = (cdg.insn.itype == NN_vhaddps || cdg.insn.itype == NN_vhaddpd) ? "hadd" : "hsub";
        type = is_double ? "pd" : "ps";
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_%s", size == YMM_SIZE ? "256" : "", op, type);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, is_int, is_double);

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
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
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
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
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
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
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
