/*
AVX Logic and Misc Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_v_bitwise(codegen_t &cdg) {
    QASSERT(0xA0400, is_avx_reg(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2));

    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    bool is_int = (cdg.insn.itype == NN_vpand || cdg.insn.itype == NN_vpor || cdg.insn.itype == NN_vpxor ||
                   cdg.insn.itype == NN_vpandn);
    bool is_double = (cdg.insn.itype == NN_vandpd || cdg.insn.itype == NN_vorpd || cdg.insn.itype == NN_vxorpd ||
                      cdg.insn.itype == NN_vandnpd);

    const char *opname = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpand:
        case NN_vandps:
        case NN_vandpd: opname = "and";
            break;
        case NN_vpandn:
        case NN_vandnps:
        case NN_vandnpd: opname = "andnot";
            break;
        case NN_vpor:
        case NN_vorps:
        case NN_vorpd: opname = "or";
            break;
        case NN_vpxor:
        case NN_vxorps:
        case NN_vxorpd: opname = "xor";
            break;
        default: QASSERT(0xA0401, false);
    }

    qstring iname;
    if (is_int) {
        iname.cat_sprnt("_mm%s_%s_si%s", size == YMM_SIZE ? "256" : "", opname, size == YMM_SIZE ? "256" : "128");
    } else {
        const char *pf = is_double ? "pd" : "ps";
        iname.cat_sprnt("_mm%s_%s_%s", size == YMM_SIZE ? "256" : "", opname, pf);
    }

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vti = get_type_robust(size, is_int, is_double);

    icall.add_argument_reg(l, vti);
    icall.add_argument_reg(r, vti);
    icall.set_return_reg(d, vti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_shift(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    // Op2 can be mem if Op3 is imm, otherwise Op2 is reg.
    AvxOpLoader s(cdg, 1, cdg.insn.Op2);

    const char *op = nullptr;
    int bits = 0;
    switch (cdg.insn.itype) {
        case NN_vpsllw: op = "sll";
            bits = 16;
            break;
        case NN_vpslld: op = "sll";
            bits = 32;
            break;
        case NN_vpsllq: op = "sll";
            bits = 64;
            break;
        case NN_vpsrlw: op = "srl";
            bits = 16;
            break;
        case NN_vpsrld: op = "srl";
            bits = 32;
            break;
        case NN_vpsrlq: op = "srl";
            bits = 64;
            break;
        case NN_vpsraw: op = "sra";
            bits = 16;
            break;
        case NN_vpsrad: op = "sra";
            bits = 32;
            break;
    }

    if (cdg.insn.Op3.type == o_imm) {
        // Immediate shift: _mm256_slli_epi16
        qstring iname;
        iname.cat_sprnt("_mm%s_%si_epi%d", size == YMM_SIZE ? "256" : "", op, bits);
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        icall.add_argument_reg(s, ti);
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else {
        // Register/Mem shift: _mm256_sll_epi16
        // Count is always 128-bit (XMM or m128)
        AvxOpLoader count(cdg, 2, cdg.insn.Op3);
        qstring iname;
        iname.cat_sprnt("_mm%s_%s_epi%d", size == YMM_SIZE ? "256" : "", op, bits);
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti_vec = get_type_robust(size, true, false);
        tinfo_t ti_count = get_type_robust(XMM_SIZE, true, false); // Count is always __m128i

        icall.add_argument_reg(s, ti_vec);
        icall.add_argument_reg(count, ti_count);
        icall.set_return_reg(d, ti_vec);
        icall.emit();
    }
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_var_shift(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader c(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    int bits = 0;
    switch (cdg.insn.itype) {
        case NN_vpsllvd: op = "sllv";
            bits = 32;
            break;
        case NN_vpsllvq: op = "sllv";
            bits = 64;
            break;
        case NN_vpsrlvd: op = "srlv";
            bits = 32;
            break;
        case NN_vpsrlvq: op = "srlv";
            bits = 64;
            break;
        case NN_vpsravd: op = "srav";
            bits = 32;
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_epi%d", size == YMM_SIZE ? "256" : "", op, bits);
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);
    icall.add_argument_reg(s, ti);
    icall.add_argument_reg(c, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vshufps(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    QASSERT(0xA0601, cdg.insn.Op4.type==o_imm);
    uval_t imm8 = cdg.insn.Op4.value;
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_shuffle_ps", size == YMM_SIZE ? "256" : "");
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm8, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vshufpd(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    QASSERT(0xA0602, cdg.insn.Op4.type==o_imm);
    uval_t imm8 = cdg.insn.Op4.value;
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_shuffle_pd", size == YMM_SIZE ? "256" : "");
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, true);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm8, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_shuffle_int(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);

    const char *op = nullptr;
    const char *suffix = nullptr;
    bool has_imm = false;

    switch (cdg.insn.itype) {
        case NN_vpshufb: op = "shuffle";
            suffix = "epi8";
            has_imm = false;
            break;
        case NN_vpshufd: op = "shuffle";
            suffix = "epi32";
            has_imm = true;
            break;
        case NN_vpshufhw: op = "shufflehi";
            suffix = "epi16";
            has_imm = true;
            break;
        case NN_vpshuflw: op = "shufflelo";
            suffix = "epi16";
            has_imm = true;
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_%s", size == YMM_SIZE ? "256" : "", op, suffix);
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(s, ti);

    if (has_imm) {
        QASSERT(0xA0605, cdg.insn.Op3.type == o_imm);
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
    } else {
        // vpshufb: mask is Op3
        AvxOpLoader mask(cdg, 2, cdg.insn.Op3);
        icall.add_argument_reg(mask, ti);
    }

    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vpermpd(codegen_t &cdg) {
    // vpermpd is AVX2, YMM only usually.
    // vpermpd ymm1, ymm2/m256, imm8
    int size = YMM_SIZE;
    if (!is_ymm_reg(cdg.insn.Op1)) {
        // Should not happen for vpermpd, but check
        size = XMM_SIZE;
    }

    QASSERT(0xA0603, cdg.insn.Op3.type==o_imm);
    uval_t imm8 = cdg.insn.Op3.value;
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    AVXIntrinsic icall(&cdg, "_mm256_permute4x64_pd");
    tinfo_t ti = get_type_robust(size, false, true);

    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm8, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    return MERR_OK;
}

merror_t handle_v_perm_int(codegen_t &cdg) {
    // vpermq and vpermd are AVX2 (YMM)
    int size = YMM_SIZE;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    if (cdg.insn.itype == NN_vpermq) {
        // vpermq ymm1, ymm2/m256, imm8
        AvxOpLoader s(cdg, 1, cdg.insn.Op2);
        QASSERT(0xA0606, cdg.insn.Op3.type == o_imm);

        AVXIntrinsic icall(&cdg, "_mm256_permute4x64_epi64");
        tinfo_t ti = get_type_robust(size, true, false);
        icall.add_argument_reg(s, ti);
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else {
        // vpermd ymm1, ymm2, ymm3/m256
        // _mm256_permutevar8x32_epi32(src, idx)
        // Instruction: vpermd dest, idx, src
        mreg_t idx = reg2mreg(cdg.insn.Op2.reg);
        AvxOpLoader src(cdg, 2, cdg.insn.Op3);

        AVXIntrinsic icall(&cdg, "_mm256_permutevar8x32_epi32");
        tinfo_t ti = get_type_robust(size, true, false);
        icall.add_argument_reg(src, ti);
        icall.add_argument_reg(idx, ti);
        icall.set_return_reg(d, ti);
        icall.emit();
    }
    return MERR_OK;
}

merror_t handle_v_align(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s1 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader s2(cdg, 2, cdg.insn.Op3);
    QASSERT(0xA0607, cdg.insn.Op4.type == o_imm);

    qstring iname;
    iname.cat_sprnt("_mm%s_alignr_epi8", size == YMM_SIZE ? "256" : "");
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    // _mm_alignr_epi8(a, b, n) -> concatenates a and b, shifts right by n.
    // Instruction: vpalignr dest, src1, src2, imm8
    // Dest = (Src1 << ...) | (Src2 >> ...)
    // Intrinsic maps Op2 to 'a' and Op3 to 'b'.
    icall.add_argument_reg(s1, ti);
    icall.add_argument_reg(s2, ti);
    icall.add_argument_imm(cdg.insn.Op4.value, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vzeroupper_nop(codegen_t &) { return MERR_OK; }

merror_t handle_vbroadcast_ss_sd(codegen_t &cdg) {
    int size = get_op_size(cdg.insn);
    bool is_double = (cdg.insn.itype == NN_vbroadcastsd);
    int scalar_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    // AVX2 allows register source for vbroadcastss/sd.
    // We use load_op_reg_or_mem to handle both memory and register operands.
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    mreg_t scalar = cdg.mba->alloc_kreg(scalar_size);

    // Extract scalar (low element).
    // If src is a vector register, m_mov with scalar_size extracts the low bits.
    cdg.emit(m_mov, scalar_size, src, 0, scalar, 0);

    AVXIntrinsic icall(&cdg, (size == YMM_SIZE)
                                 ? (is_double ? "_mm256_set1_pd" : "_mm256_set1_ps")
                                 : (is_double ? "_mm_set1_pd" : "_mm_set1_ps"));
    tinfo_t vt = get_type_robust(size, false, is_double);
    icall.set_return_reg(d, vt);
    icall.add_argument_reg(scalar, is_double ? BTF_DOUBLE : BT_FLOAT);
    icall.emit();

    cdg.mba->free_kreg(scalar, scalar_size);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vbroadcastf128_fp(codegen_t &cdg) {
    QASSERT(0xA0701, is_ymm_reg(cdg.insn.Op1) && is_mem_op(cdg.insn.Op2));
    mreg_t src128 = cdg.load_operand(1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    tinfo_t vec_type = get_type_robust(YMM_SIZE, false, false);

    AVXIntrinsic cast_intr(&cdg, "_mm256_castps128_ps256");
    mreg_t tmp = cdg.mba->alloc_kreg(YMM_SIZE);
    cast_intr.add_argument_reg(src128, get_type_robust(16, false, false));
    cast_intr.set_return_reg(tmp, vec_type);
    cast_intr.emit();

    AVXIntrinsic ins_intr(&cdg, "_mm256_insertf128_ps");
    ins_intr.add_argument_reg(tmp, vec_type);
    ins_intr.add_argument_reg(src128, get_type_robust(16, false, false));
    ins_intr.add_argument_imm(1, BT_INT8);
    ins_intr.set_return_reg(d, vec_type);
    ins_intr.emit();

    cdg.mba->free_kreg(tmp, YMM_SIZE);
    return MERR_OK;
}

merror_t handle_vbroadcasti128_int(codegen_t &cdg) {
    QASSERT(0xA0703, is_ymm_reg(cdg.insn.Op1) && is_mem_op(cdg.insn.Op2));
    mreg_t src128 = cdg.load_operand(1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    AVXIntrinsic intr(&cdg, "_mm256_broadcastsi128_si256");
    intr.add_argument_reg(src128, get_type_robust(16, true, false));
    intr.set_return_reg(d, get_type_robust(32, true, false));
    intr.emit();
    return MERR_OK;
}

merror_t handle_vcmp_ps_pd(codegen_t &cdg) {
    int size = get_op_size(cdg.insn);
    bool is_double = (cdg.insn.itype >= NN_vcmpeqpd && cdg.insn.itype <= NN_vcmptrue_uspd) ||
                     (cdg.insn.itype >= NN_vcmpeqsd && cdg.insn.itype <= NN_vcmptrue_ussd);
    bool is_scalar = (cdg.insn.itype >= NN_vcmpeqss && cdg.insn.itype <= NN_vcmptrue_usss) ||
                     (cdg.insn.itype >= NN_vcmpeqsd && cdg.insn.itype <= NN_vcmptrue_ussd);
    uint8 pred = get_cmp_predicate(cdg.insn.itype);

    AvxOpLoader a(cdg, 1, cdg.insn.Op2);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *suf = is_scalar ? (is_double ? "sd" : "ss") : (is_double ? "pd" : "ps");
    qstring iname;
    iname.cat_sprnt("_mm%s_cmp_%s", size == YMM_SIZE ? "256" : "", suf);
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, is_double);

    icall.add_argument_reg(a, vt);
    icall.add_argument_reg(b, vt);
    icall.add_argument_imm(pred, BT_INT8);
    icall.set_return_reg(d, vt);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vpcmp_int(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *op = nullptr;
    const char *type = nullptr;

    switch (cdg.insn.itype) {
        case NN_vpcmpeqb: op = "eq";
            type = "epi8";
            break;
        case NN_vpcmpeqw: op = "eq";
            type = "epi16";
            break;
        case NN_vpcmpeqd: op = "eq";
            type = "epi32";
            break;
        case NN_vpcmpeqq: op = "eq";
            type = "epi64";
            break;
        case NN_vpcmpgtb: op = "gt";
            type = "epi8";
            break;
        case NN_vpcmpgtw: op = "gt";
            type = "epi16";
            break;
        case NN_vpcmpgtd: op = "gt";
            type = "epi32";
            break;
        case NN_vpcmpgtq: op = "gt";
            type = "epi64";
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_cmp%s_%s", size == YMM_SIZE ? "256" : "", op, type);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);
    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vblendv_ps_pd(codegen_t &cdg) {
    int size = get_op_size(cdg.insn);
    bool is_double = (cdg.insn.itype == NN_vblendvpd);

    AvxOpLoader x(cdg, 1, cdg.insn.Op2);
    AvxOpLoader y(cdg, 2, cdg.insn.Op3);
    mreg_t m = reg2mreg(cdg.insn.Op4.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_blendv_%s", size == YMM_SIZE ? "256" : "", is_double ? "pd" : "ps");
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, is_double);

    icall.add_argument_reg(x, vt);
    icall.add_argument_reg(y, vt);
    icall.add_argument_reg(m, vt);
    icall.set_return_reg(d, vt);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vblend_imm_ps_pd(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    bool is_double = (cdg.insn.itype == NN_vblendpd);

    QASSERT(0xA0604, cdg.insn.Op4.type==o_imm);
    uval_t imm8 = cdg.insn.Op4.value;

    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_blend_%s", size == YMM_SIZE ? "256" : "", is_double ? "pd" : "ps");
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm8, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vmaskmov_ps_pd(codegen_t &cdg) {
    bool is_double = (cdg.insn.itype == NN_vmaskmovpd);
    int size = (is_xmm_reg(cdg.insn.Op1) || (is_mem_op(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2)))
                   ? XMM_SIZE
                   : YMM_SIZE;
    tinfo_t vt = get_type_robust(size, false, is_double);

    if (is_avx_reg(cdg.insn.Op1)) {
        // maskload: dst(reg), mask(reg), src(mem)
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
        mreg_t mask = reg2mreg(cdg.insn.Op2.reg);
        QASSERT(0xA0800, is_mem_op(cdg.insn.Op3));
        mreg_t memv = cdg.load_operand(2);

        const char *setz = (size == YMM_SIZE)
                               ? (is_double ? "_mm256_setzero_pd" : "_mm256_setzero_ps")
                               : (is_double ? "_mm_setzero_pd" : "_mm_setzero_ps");
        AVXIntrinsic setz_ic(&cdg, setz);
        mreg_t zero = cdg.mba->alloc_kreg(size);
        setz_ic.set_return_reg(zero, vt);
        setz_ic.emit();

        const char *blend = (size == YMM_SIZE)
                                ? (is_double ? "_mm256_blendv_pd" : "_mm256_blendv_ps")
                                : (is_double ? "_mm_blendv_pd" : "_mm_blendv_ps");
        AVXIntrinsic bl(&cdg, blend);
        bl.add_argument_reg(zero, vt);
        bl.add_argument_reg(memv, vt);
        bl.add_argument_reg(mask, vt);
        bl.set_return_reg(dst, vt);
        bl.emit();

        cdg.mba->free_kreg(zero, size);
        if (size == XMM_SIZE) clear_upper(cdg, dst);
        return MERR_OK;
    }

    // maskstore: mem, mask(reg), src(reg)
    QASSERT(0xA0801, is_mem_op(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2) && is_avx_reg(cdg.insn.Op3));
    mreg_t mask = reg2mreg(cdg.insn.Op2.reg);
    mreg_t src = reg2mreg(cdg.insn.Op3.reg);

    mreg_t oldv = cdg.load_operand(0);

    const char *blend = (size == YMM_SIZE)
                            ? (is_double ? "_mm256_blendv_pd" : "_mm256_blendv_ps")
                            : (is_double ? "_mm_blendv_pd" : "_mm_blendv_ps");
    AVXIntrinsic bl(&cdg, blend);
    mreg_t res = cdg.mba->alloc_kreg(size);
    bl.add_argument_reg(oldv, vt);
    bl.add_argument_reg(src, vt);
    bl.add_argument_reg(mask, vt);
    bl.set_return_reg(res, vt);
    bl.emit();

    store_operand_hack(cdg, 0, mop_t(res, size));
    cdg.mba->free_kreg(res, size);

    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
