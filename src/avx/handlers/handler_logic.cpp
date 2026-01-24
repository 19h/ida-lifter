/*
AVX Logic and Misc Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_v_bitwise(codegen_t &cdg) {
    QASSERT(0xA0400, is_vector_reg(cdg.insn.Op1) && is_vector_reg(cdg.insn.Op2));

    int size = get_vector_size(cdg.insn.Op1);
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

    const char *prefix = get_size_prefix(size);
    qstring iname;
    if (is_int) {
        iname.cat_sprnt("_mm%s_%s_si%d", prefix, opname, get_vector_bits(size));
    } else {
        const char *pf = is_double ? "pd" : "ps";
        iname.cat_sprnt("_mm%s_%s_%s", prefix, opname, pf);
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
    int size = get_vector_size(cdg.insn.Op1);
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
        iname.cat_sprnt("_mm%s_%si_epi%d", get_size_prefix(size), op, bits);
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
        iname.cat_sprnt("_mm%s_%s_epi%d", get_size_prefix(size), op, bits);
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
    int size = get_vector_size(cdg.insn.Op1);
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
    iname.cat_sprnt("_mm%s_%s_epi%d", get_size_prefix(size), op, bits);
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
    int size = get_vector_size(cdg.insn.Op1);
    QASSERT(0xA0601, cdg.insn.Op4.type==o_imm);
    uval_t imm8 = cdg.insn.Op4.value;
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_shuffle_ps", get_size_prefix(size));
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
    int size = get_vector_size(cdg.insn.Op1);
    QASSERT(0xA0602, cdg.insn.Op4.type==o_imm);
    uval_t imm8 = cdg.insn.Op4.value;
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_shuffle_pd", get_size_prefix(size));
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
    int size = get_vector_size(cdg.insn.Op1);
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
    iname.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, suffix);
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
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    if (cdg.insn.itype == NN_vpermq) {
        // vpermq ymm1, ymm2/m256, imm8
        AvxOpLoader s(cdg, 1, cdg.insn.Op2);
        QASSERT(0xA0606, cdg.insn.Op3.type == o_imm);

        AVXIntrinsic icall(&cdg, "_mm256_permute4x64_epi64");
        tinfo_t ti = get_type_robust(YMM_SIZE, true, false);
        icall.add_argument_reg(s, ti);
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else if (cdg.insn.itype == NN_vpermd) {
        // vpermd ymm1, ymm2, ymm3/m256
        // _mm256_permutevar8x32_epi32(src, idx)
        // Instruction: vpermd dest, idx, src
        mreg_t idx = reg2mreg(cdg.insn.Op2.reg);
        AvxOpLoader src(cdg, 2, cdg.insn.Op3);

        AVXIntrinsic icall(&cdg, "_mm256_permutevar8x32_epi32");
        tinfo_t ti = get_type_robust(YMM_SIZE, true, false);
        icall.add_argument_reg(src, ti);
        icall.add_argument_reg(idx, ti);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else if (cdg.insn.itype == NN_vpermilps || cdg.insn.itype == NN_vpermilpd) {
        // vpermilps/vpermilpd xmm1, xmm2, imm8 or xmm1, xmm2, xmm3/m128
        bool is_double = (cdg.insn.itype == NN_vpermilpd);
        AvxOpLoader s(cdg, 1, cdg.insn.Op2);

        if (cdg.insn.Op3.type == o_imm) {
            // Immediate form: _mm_permute_ps/_mm256_permute_ps
            qstring iname;
            iname.cat_sprnt("_mm%s_permute_%s", get_size_prefix(size), is_double ? "pd" : "ps");
            AVXIntrinsic icall(&cdg, iname.c_str());
            tinfo_t ti = get_type_robust(size, false, is_double);
            icall.add_argument_reg(s, ti);
            icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
            icall.set_return_reg(d, ti);
            icall.emit();
        } else {
            // Variable form: _mm_permutevar_ps/_mm256_permutevar_ps
            AvxOpLoader ctrl(cdg, 2, cdg.insn.Op3);
            qstring iname;
            iname.cat_sprnt("_mm%s_permutevar_%s", get_size_prefix(size), is_double ? "pd" : "ps");
            AVXIntrinsic icall(&cdg, iname.c_str());
            tinfo_t ti = get_type_robust(size, false, is_double);
            tinfo_t ti_ctrl = get_type_robust(size, true, false); // Control is integer
            icall.add_argument_reg(s, ti);
            icall.add_argument_reg(ctrl, ti_ctrl);
            icall.set_return_reg(d, ti);
            icall.emit();
        }
        if (size == XMM_SIZE) clear_upper(cdg, d);
    }
    return MERR_OK;
}

merror_t handle_v_align(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s1 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader s2(cdg, 2, cdg.insn.Op3);
    QASSERT(0xA0607, cdg.insn.Op4.type == o_imm);

    qstring iname;
    iname.cat_sprnt("_mm%s_alignr_epi8", get_size_prefix(size));
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

// vzeroupper is a microarchitectural optimization with no semantic effect
// Emit a NOP instruction so it doesn't appear as __asm block
merror_t handle_vzeroupper_nop(codegen_t &cdg) {
    // Emit a m_nop to consume the instruction without any visible effect
    // m_nop requires proper arguments: emit(opcode, width, l, r, d, offsize)
    cdg.emit(m_nop, 0, 0, 0, 0, 0);
    return MERR_OK;
}

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

    const char *bcast = nullptr;
    if (size == ZMM_SIZE) {
        bcast = is_double ? "_mm512_set1_pd" : "_mm512_set1_ps";
    } else if (size == YMM_SIZE) {
        bcast = is_double ? "_mm256_set1_pd" : "_mm256_set1_ps";
    } else {
        bcast = is_double ? "_mm_set1_pd" : "_mm_set1_ps";
    }
    AVXIntrinsic icall(&cdg, bcast);
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

    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    // Load first operand (always register for VEX-encoded compares)
    AvxOpLoader a(cdg, 1, cdg.insn.Op2);

    // Load second operand - handle scalar memory operands specially
    // vcmpeqss/vcmpeqsd with memory operand loads only 4/8 bytes but intrinsic expects 16 bytes
    mreg_t b;
    mreg_t t_mem = mr_none;
    if (is_scalar && is_mem_op(cdg.insn.Op3)) {
        // Scalar compare with memory operand: load scalar, zero-extend to XMM
        AvxOpLoader b_in(cdg, 2, cdg.insn.Op3);
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(b_in.reg, elem_size);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);  // Zero-extend to XMM
        b = t_mem;
    } else {
        AvxOpLoader b_in(cdg, 2, cdg.insn.Op3);
        b = b_in.reg;
    }

    const char *suf = is_scalar ? (is_double ? "sd" : "ss") : (is_double ? "pd" : "ps");
    qstring iname;
    iname.cat_sprnt("_mm%s_cmp_%s", get_size_prefix(size), suf);
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, is_double);

    icall.add_argument_reg(a, vt);
    icall.add_argument_reg(b, vt);
    icall.add_argument_imm(pred, BT_INT8);
    icall.set_return_reg(d, vt);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vpcmp_int(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
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
    iname.cat_sprnt("_mm%s_cmp%s_%s", get_size_prefix(size), op, type);

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
    iname.cat_sprnt("_mm%s_blendv_%s", get_size_prefix(size), is_double ? "pd" : "ps");
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
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vblendpd);

    QASSERT(0xA0604, cdg.insn.Op4.type==o_imm);
    uval_t imm8 = cdg.insn.Op4.value;

    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_blend_%s", get_size_prefix(size), is_double ? "pd" : "ps");
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

merror_t handle_vblend_int(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);

    if (cdg.insn.itype == NN_vpblendvb) {
        // vpblendvb: xmm1, xmm2, xmm3/m128, xmm4
        mreg_t mask = reg2mreg(cdg.insn.Op4.reg);
        qstring iname;
        iname.cat_sprnt("_mm%s_blendv_epi8", get_size_prefix(size));
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        icall.add_argument_reg(l, ti);
        icall.add_argument_reg(r, ti);
        icall.add_argument_reg(mask, ti);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else {
        // vpblendd/vpblendw: xmm1, xmm2, xmm3/m128, imm8
        QASSERT(0xA0610, cdg.insn.Op4.type == o_imm);
        uval_t imm8 = cdg.insn.Op4.value;

        qstring iname;
        if (cdg.insn.itype == NN_vpblendd) {
            iname.cat_sprnt("_mm%s_blend_epi32", get_size_prefix(size));
        } else { // NN_vpblendw
            iname.cat_sprnt("_mm%s_blend_epi16", get_size_prefix(size));
        }
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        icall.add_argument_reg(l, ti);
        icall.add_argument_reg(r, ti);
        icall.add_argument_imm(imm8, BT_INT8);
        icall.set_return_reg(d, ti);
        icall.emit();
    }

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vextractf128(codegen_t &cdg) {
    // vextractf128 xmm1/m128, ymm2, imm8
    // Extracts 128 bits from ymm2 based on imm8
    QASSERT(0xA0900, is_ymm_reg(cdg.insn.Op2));
    QASSERT(0xA0901, cdg.insn.Op3.type == o_imm);

    mreg_t src = reg2mreg(cdg.insn.Op2.reg);
    uint64 imm = cdg.insn.Op3.value & 1;

    qstring iname;
    iname.cat_sprnt("_mm256_extractf128_ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt_src = get_type_robust(YMM_SIZE, false, false);
    tinfo_t vt_dst = get_type_robust(XMM_SIZE, false, false);

    icall.add_argument_reg(src, vt_src);
    icall.add_argument_imm(imm, BT_INT32);

    if (is_xmm_reg(cdg.insn.Op1)) {
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
        icall.set_return_reg(dst, vt_dst);
        icall.emit();
        clear_upper(cdg, dst);
    } else {
        // Memory destination - store the result
        QASSERT(0xA0902, is_mem_op(cdg.insn.Op1));
        mreg_t tmp = cdg.mba->alloc_kreg(XMM_SIZE);
        icall.set_return_reg(tmp, vt_dst);
        icall.emit();
        store_operand_hack(cdg, 0, mop_t(tmp, XMM_SIZE));
        cdg.mba->free_kreg(tmp, XMM_SIZE);
    }

    return MERR_OK;
}

merror_t handle_vinsertf128(codegen_t &cdg) {
    // vinsertf128 ymm1, ymm2, xmm3/m128, imm8
    // Inserts 128 bits into ymm
    QASSERT(0xA0910, is_ymm_reg(cdg.insn.Op1) && is_ymm_reg(cdg.insn.Op2));
    QASSERT(0xA0911, cdg.insn.Op4.type == o_imm);

    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    mreg_t src1 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader src2(cdg, 2, cdg.insn.Op3);
    uint64 imm = cdg.insn.Op4.value & 1;

    AVXIntrinsic icall(&cdg, "_mm256_insertf128_ps");
    tinfo_t vt_ymm = get_type_robust(YMM_SIZE, false, false);
    tinfo_t vt_xmm = get_type_robust(XMM_SIZE, false, false);

    icall.add_argument_reg(src1, vt_ymm);
    icall.add_argument_reg(src2, vt_xmm);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(dst, vt_ymm);
    icall.emit();

    return MERR_OK;
}

merror_t handle_vmovshdup(codegen_t &cdg) {
    // vmovshdup xmm1, xmm2/m128 or ymm1, ymm2/m256
    // Replicate odd-indexed single-precision floating-point values
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    qstring iname;
    iname.cat_sprnt("_mm%s_movehdup_ps", get_size_prefix(size));

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, false);

    icall.add_argument_reg(src, vt);
    icall.set_return_reg(dst, vt);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

merror_t handle_vmovsldup(codegen_t &cdg) {
    // vmovsldup xmm1, xmm2/m128 or ymm1, ymm2/m256
    // Replicate even-indexed single-precision floating-point values
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    qstring iname;
    iname.cat_sprnt("_mm%s_moveldup_ps", get_size_prefix(size));

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, false);

    icall.add_argument_reg(src, vt);
    icall.set_return_reg(dst, vt);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

merror_t handle_vmovddup(codegen_t &cdg) {
    // vmovddup xmm1, xmm2/m64 or ymm1, ymm2/m256
    // Duplicate the low double-precision element
    // XMM variant: loads 64-bit from memory, duplicates to fill 128-bit register
    // YMM variant: loads 256-bit from memory
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);

    // Handle memory operand for XMM variant specially:
    // vmovddup xmm, m64 loads only 8 bytes but intrinsic expects __m128d (16 bytes)
    mreg_t src;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op2) && size == XMM_SIZE) {
        // XMM variant with memory: load 8 bytes, zero-extend to 16 bytes
        AvxOpLoader src_in(cdg, 1, cdg.insn.Op2);
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src_mop(src_in.reg, DOUBLE_SIZE);  // 8 bytes loaded
        mop_t dst_mop(t_mem, XMM_SIZE);          // 16 bytes for intrinsic
        if (XMM_SIZE > 8) {
            dst_mop.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src_mop, &empty, &dst_mop);  // zero-extend
        src = t_mem;
    } else if (is_mem_op(cdg.insn.Op2)) {
        // YMM variant with memory: load full 32 bytes
        AvxOpLoader src_in(cdg, 1, cdg.insn.Op2);
        src = src_in.reg;
    } else {
        // Register operand
        src = reg2mreg(cdg.insn.Op2.reg);
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_movedup_pd", get_size_prefix(size));

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, true); // double type

    icall.add_argument_reg(src, vt);
    icall.set_return_reg(dst, vt);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

merror_t handle_vunpck(codegen_t &cdg) {
    // vunpckhps/vunpcklps/vunpckhpd/vunpcklpd
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    mreg_t src1 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader src2(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    bool is_double = false;
    switch (cdg.insn.itype) {
        case NN_vunpckhps: op = "unpackhi"; break;
        case NN_vunpcklps: op = "unpacklo"; break;
        case NN_vunpckhpd: op = "unpackhi"; is_double = true; break;
        case NN_vunpcklpd: op = "unpacklo"; is_double = true; break;
        default: return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, is_double ? "pd" : "ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, is_double);

    icall.add_argument_reg(src1, vt);
    icall.add_argument_reg(src2, vt);
    icall.set_return_reg(dst, vt);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

// vpbroadcastd/q from XMM register or memory
// vpbroadcastd ymm1, xmm2/m32
// vpbroadcastq ymm1, xmm2/m64
// Note: AVX-512 variant can broadcast from GPR - we fall back to IDA for that
merror_t handle_vpbroadcast_d_q(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    bool is_qword = (cdg.insn.itype == NN_vpbroadcastq);
    int elem_size = is_qword ? 8 : 4;

    // AVX-512 variant can broadcast from GPR - fall back to IDA for that case
    // Check if source is NOT a vector register and NOT memory
    if (!is_vector_reg(cdg.insn.Op2) && !is_mem_op(cdg.insn.Op2)) {
        return MERR_INSN;  // Let IDA handle GPR source
    }

    // Source can be XMM register or memory
    mreg_t src;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op2)) {
        AvxOpLoader src_in(cdg, 1, cdg.insn.Op2);
        // Zero-extend to XMM for intrinsic argument
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t s(src_in.reg, elem_size);
        mop_t dst_op(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst_op.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &s, &empty, &dst_op);
        src = t_mem;
    } else {
        src = reg2mreg(cdg.insn.Op2.reg);
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_broadcastd_epi%d", get_size_prefix(size), is_qword ? 64 : 32);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_src = get_type_robust(XMM_SIZE, true, false);
    tinfo_t ti_dst = get_type_robust(size, true, false);

    icall.add_argument_reg(src, ti_src);
    icall.set_return_reg(d, ti_dst);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vperm2f128/vperm2i128 - permute 128-bit lanes
// vperm2f128 ymm1, ymm2, ymm3/m256, imm8
merror_t handle_vperm2f128_i128(codegen_t &cdg) {
    QASSERT(0xA0700, is_ymm_reg(cdg.insn.Op1) && is_ymm_reg(cdg.insn.Op2));

    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);

    QASSERT(0xA0701, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    bool is_int = (cdg.insn.itype == NN_vperm2i128);
    qstring iname;
    iname.cat_sprnt("_mm256_permute2%s128_%s", is_int ? "x" : "f", is_int ? "si256" : "ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(YMM_SIZE, is_int, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    return MERR_OK;
}

// vphsubsw - horizontal packed subtract with saturation
// vphsubsw ymm1, ymm2, ymm3/m256
merror_t handle_vphsub_sw(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);

    qstring iname;
    bool is_w = (cdg.insn.itype == NN_vphsubw);
    bool is_sw = (cdg.insn.itype == NN_vphsubsw);
    const char *suffix = is_sw ? "hsubs" : "hsub";
    const char *type = is_w || is_sw ? "epi16" : "epi32";

    iname.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), suffix, type);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vpackssdw/vpacksswb - pack with signed saturation
// vpackssdw ymm1, ymm2, ymm3/m256
merror_t handle_vpack(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpackssdw: op = "packs_epi32"; break;
        case NN_vpacksswb: op = "packs_epi16"; break;
        case NN_vpackusdw: op = "packus_epi32"; break;
        case NN_vpackuswb: op = "packus_epi16"; break;
        default: return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s", get_size_prefix(size), op);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vptest - logical compare
// vptest ymm1, ymm2/m256
// Note: vptest sets flags (ZF, CF), not a register destination
// This is difficult to lift properly since IDA expects register destinations
// For now, emit a NOP and let IDA handle the flag-setting behavior natively
merror_t handle_vptest(codegen_t &cdg) {
    // vptest is a flag-setting instruction with no register destination
    // We cannot properly lift this to an intrinsic since intrinsics return values
    // Fall back to IDA's native handling
    return MERR_INSN;
}

// vfmaddsub/vfmsubadd - FMA with alternating add/sub
// vfmaddsub132ps/pd, vfmaddsub213ps/pd, vfmaddsub231ps/pd
// vfmsubadd132ps/pd, vfmsubadd213ps/pd, vfmsubadd231ps/pd
merror_t handle_vfmaddsub(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t op1 = reg2mreg(cdg.insn.Op1.reg);
    mreg_t op2 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader op3_in(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    const char *type = nullptr;
    int order = 0;
    bool is_double = false;

    uint16 it = cdg.insn.itype;

    // vfmaddsub: odd elements are added, even elements are subtracted
    // vfmsubadd: odd elements are subtracted, even elements are added
    if (it >= NN_vfmaddsub132ps && it <= NN_vfmaddsub231pd) {
        op = "fmaddsub";
        int base = it - NN_vfmaddsub132ps;
        order = (base / 2) == 0 ? 132 : ((base / 2) == 1 ? 213 : 231);
        is_double = (base % 2) == 1;
        type = is_double ? "pd" : "ps";
    } else if (it >= NN_vfmsubadd132ps && it <= NN_vfmsubadd231pd) {
        op = "fmsubadd";
        int base = it - NN_vfmsubadd132ps;
        order = (base / 2) == 0 ? 132 : ((base / 2) == 1 ? 213 : 231);
        is_double = (base % 2) == 1;
        type = is_double ? "pd" : "ps";
    } else {
        return MERR_INSN;
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, type);

    int elem_size = is_double ? 8 : 4;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    qstring iname = make_masked_intrinsic_name(base_name.c_str(), mask);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    mreg_t op3 = op3_in;

    // Argument ordering (same as regular FMA)
    mreg_t arg1, arg2, arg3;
    if (order == 132) {
        arg1 = op1; arg2 = op3; arg3 = op2;
    } else if (order == 213) {
        arg1 = op2; arg2 = op1; arg3 = op3;
    } else {
        arg1 = op2; arg2 = op3; arg3 = op1;
    }

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(arg1, ti);
    icall.add_argument_reg(arg2, ti);
    icall.add_argument_reg(arg3, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vmovmskps/vmovmskpd/vpmovmskb - move sign bits to GPR
// vmovmskps r32, xmm/ymm
merror_t handle_vmovmsk(codegen_t &cdg) {
    // Destination is a GPR, source is a vector register
    int size = get_vector_size(cdg.insn.Op2);
    mreg_t src = reg2mreg(cdg.insn.Op2.reg);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    bool is_int = (cdg.insn.itype == NN_vpmovmskb);
    bool is_double = (cdg.insn.itype == NN_vmovmskpd);

    if (is_int) {
        iname.cat_sprnt("_mm%s_movemask_epi8", get_size_prefix(size));
    } else {
        iname.cat_sprnt("_mm%s_movemask_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    }

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, is_int, is_double);

    icall.add_argument_reg(src, ti);
    icall.set_return_reg_basic(dst, BT_INT32);
    icall.emit();

    return MERR_OK;
}

// vmovntps/vmovntpd/vmovntdq - non-temporal store
// vmovntps m128/m256, xmm/ymm
merror_t handle_vmovnt(codegen_t &cdg) {
    // Non-temporal stores: memory destination, register source
    QASSERT(0xA0A00, is_mem_op(cdg.insn.Op1));

    int size = get_vector_size(cdg.insn.Op2);
    mreg_t src = reg2mreg(cdg.insn.Op2.reg);

    bool is_int = (cdg.insn.itype == NN_vmovntdq);
    bool is_double = (cdg.insn.itype == NN_vmovntpd);

    qstring iname;
    if (is_int) {
        iname.cat_sprnt("_mm%s_stream_si%d", get_size_prefix(size), get_vector_bits(size));
    } else {
        iname.cat_sprnt("_mm%s_stream_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    }

    // For non-temporal stores, we emit as a regular store
    // The intrinsic is a hint; for decompilation purposes, treat as store
    mop_t src_mop(src, size);
    store_operand_hack(cdg, 0, src_mop);

    return MERR_OK;
}

// vpbroadcastb/vpbroadcastw - broadcast byte/word from XMM or memory
// vpbroadcastb ymm1, xmm2/m8
// vpbroadcastw ymm1, xmm2/m16
merror_t handle_vpbroadcast_b_w(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    bool is_word = (cdg.insn.itype == NN_vpbroadcastw);
    int elem_size = is_word ? 2 : 1;

    // Source can be XMM register or memory
    mreg_t src;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op2)) {
        AvxOpLoader src_in(cdg, 1, cdg.insn.Op2);
        // Zero-extend to XMM for intrinsic argument
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t s(src_in.reg, elem_size);
        mop_t dst_op(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst_op.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &s, &empty, &dst_op);
        src = t_mem;
    } else {
        src = reg2mreg(cdg.insn.Op2.reg);
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_broadcast%s_epi%d", get_size_prefix(size), is_word ? "w" : "b", is_word ? 16 : 8);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_src = get_type_robust(XMM_SIZE, true, false);
    tinfo_t ti_dst = get_type_robust(size, true, false);

    icall.add_argument_reg(src, ti_src);
    icall.set_return_reg(d, ti_dst);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vpinsrb/vpinsrw/vpinsrd/vpinsrq - insert into vector
// vpinsrd xmm1, xmm2, r32/m32, imm8
merror_t handle_vpinsert(codegen_t &cdg) {
    int size = XMM_SIZE;  // Always XMM for insert instructions
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);

    int elem_size;
    const char *suffix;
    switch (cdg.insn.itype) {
        case NN_vpinsrb: elem_size = 1; suffix = "epi8"; break;
        case NN_vpinsrw: elem_size = 2; suffix = "epi16"; break;
        case NN_vpinsrd: elem_size = 4; suffix = "epi32"; break;
        case NN_vpinsrq: elem_size = 8; suffix = "epi64"; break;
        default: return MERR_INSN;
    }

    // Op3 is GPR or memory, Op4 is immediate
    QASSERT(0xA0A10, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    mreg_t val;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        AvxOpLoader val_in(cdg, 2, cdg.insn.Op3);
        val = val_in.reg;
    } else {
        val = reg2mreg(cdg.insn.Op3.reg);
    }

    qstring iname;
    iname.cat_sprnt("_mm_insert_%s", suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(s, ti);
    icall.add_argument_reg(val, elem_size == 8 ? BT_INT64 : BT_INT32);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    clear_upper(cdg, d);
    return MERR_OK;
}

// vpmovsxbw/bd/bq/wd/wq/dq - sign extend packed integers
// Source sizes vary based on instruction:
//   vpmovsxbw xmm/ymm: source is 64/128 bits (half of dest)
//   vpmovsxbd xmm/ymm: source is 32/64 bits (quarter of dest)
//   vpmovsxbq xmm/ymm: source is 16/32 bits (eighth of dest)
//   vpmovsxwd xmm/ymm: source is 64/128 bits (half of dest)
//   vpmovsxwq xmm/ymm: source is 32/64 bits (quarter of dest)
//   vpmovsxdq xmm/ymm: source is 64/128 bits (half of dest)
merror_t handle_vpmovsx(codegen_t &cdg) {
    DEBUG_LOG("handle_vpmovsx: Op1.dtype=%d Op2.dtype=%d Op2.type=%d itype=%d",
              cdg.insn.Op1.dtype, cdg.insn.Op2.dtype, cdg.insn.Op2.type, cdg.insn.itype);

    int dst_size = get_vector_size(cdg.insn.Op1);
    DEBUG_LOG("handle_vpmovsx: dst_size=%d", dst_size);

    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    DEBUG_LOG("handle_vpmovsx: d=%d", d);

    // Determine source size based on instruction variant
    // The source contains packed elements that will be sign-extended
    int src_size;
    const char *suffix;
    switch (cdg.insn.itype) {
        case NN_vpmovsxbw: // byte -> word: source is half of dest
            suffix = "epi8_epi16";
            src_size = dst_size / 2;
            break;
        case NN_vpmovsxbd: // byte -> dword: source is quarter of dest
            suffix = "epi8_epi32";
            src_size = dst_size / 4;
            break;
        case NN_vpmovsxbq: // byte -> qword: source is eighth of dest
            suffix = "epi8_epi64";
            src_size = dst_size / 8;
            break;
        case NN_vpmovsxwd: // word -> dword: source is half of dest
            suffix = "epi16_epi32";
            src_size = dst_size / 2;
            break;
        case NN_vpmovsxwq: // word -> qword: source is quarter of dest
            suffix = "epi16_epi64";
            src_size = dst_size / 4;
            break;
        case NN_vpmovsxdq: // dword -> qword: source is half of dest
            suffix = "epi32_epi64";
            src_size = dst_size / 2;
            break;
        default: return MERR_INSN;
    }
    DEBUG_LOG("handle_vpmovsx: src_size=%d suffix=%s", src_size, suffix);

    AvxOpLoader src(cdg, 1, cdg.insn.Op2);
    DEBUG_LOG("handle_vpmovsx: src.reg=%d src.size=%d is_mem=%d", src.reg, src.size, is_mem_op(cdg.insn.Op2));

    qstring iname;
    iname.cat_sprnt("_mm%s_cvt%s", get_size_prefix(dst_size), suffix);
    DEBUG_LOG("handle_vpmovsx: intrinsic=%s", iname.c_str());

    AVXIntrinsic icall(&cdg, iname.c_str());
    // For the source type, use the actual size that was loaded (for memory ops)
    // or XMM_SIZE for register operands (Intel intrinsics take __m128i)
    int actual_src_size = src.size > 0 ? src.size : XMM_SIZE;
    tinfo_t ti_src = get_type_robust(actual_src_size, true, false);
    tinfo_t ti_dst = get_type_robust(dst_size, true, false);
    DEBUG_LOG("handle_vpmovsx: actual_src_size=%d ti_src.size=%d ti_dst.size=%d",
              actual_src_size, (int)ti_src.get_size(), (int)ti_dst.get_size());

    icall.add_argument_reg(src, ti_src);
    DEBUG_LOG("handle_vpmovsx: added argument");
    icall.set_return_reg(d, ti_dst);
    DEBUG_LOG("handle_vpmovsx: set return reg");
    icall.emit();
    DEBUG_LOG("handle_vpmovsx: emitted, returning MERR_OK");

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vpmovzxbw/bd/bq/wd/wq/dq - zero extend packed integers
merror_t handle_vpmovzx(codegen_t &cdg) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    const char *suffix;
    switch (cdg.insn.itype) {
        case NN_vpmovzxbw: suffix = "epu8_epi16"; break;
        case NN_vpmovzxbd: suffix = "epu8_epi32"; break;
        case NN_vpmovzxbq: suffix = "epu8_epi64"; break;
        case NN_vpmovzxwd: suffix = "epu16_epi32"; break;
        case NN_vpmovzxwq: suffix = "epu16_epi64"; break;
        case NN_vpmovzxdq: suffix = "epu32_epi64"; break;
        default: return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_cvt%s", get_size_prefix(dst_size), suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    // Use actual loaded size for memory operands, XMM_SIZE for registers
    int actual_src_size = src.size > 0 ? src.size : XMM_SIZE;
    tinfo_t ti_src = get_type_robust(actual_src_size, true, false);
    tinfo_t ti_dst = get_type_robust(dst_size, true, false);

    icall.add_argument_reg(src, ti_src);
    icall.set_return_reg(d, ti_dst);
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vpslldq/vpsrldq - byte shift
// vpslldq xmm1, xmm2, imm8
merror_t handle_vpslldq_vpsrldq(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);

    QASSERT(0xA0A20, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value;

    bool is_left = (cdg.insn.itype == NN_vpslldq);
    qstring iname;
    iname.cat_sprnt("_mm%s_%slli_si%d", get_size_prefix(size), is_left ? "s" : "sr", get_vector_bits(size));

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(s, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vpunpckhbw/vpunpcklbw/etc - integer unpack
merror_t handle_vpunpck(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);

    const char *op;
    const char *suffix;
    switch (cdg.insn.itype) {
        case NN_vpunpckhbw: op = "unpackhi"; suffix = "epi8"; break;
        case NN_vpunpcklbw: op = "unpacklo"; suffix = "epi8"; break;
        case NN_vpunpckhwd: op = "unpackhi"; suffix = "epi16"; break;
        case NN_vpunpcklwd: op = "unpacklo"; suffix = "epi16"; break;
        case NN_vpunpckhdq: op = "unpackhi"; suffix = "epi32"; break;
        case NN_vpunpckldq: op = "unpacklo"; suffix = "epi32"; break;
        case NN_vpunpckhqdq: op = "unpackhi"; suffix = "epi64"; break;
        case NN_vpunpcklqdq: op = "unpacklo"; suffix = "epi64"; break;
        default: return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vinsertps - insert single precision float value
// vinsertps xmm1, xmm2, xmm3/m32, imm8
merror_t handle_vinsertps(codegen_t &cdg) {
    // Destination XMM
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    // Source XMM
    mreg_t src1 = reg2mreg(cdg.insn.Op2.reg);

    QASSERT(0xA0A40, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value & 0xFF;

    AVXIntrinsic icall(&cdg, "_mm_insert_ps");
    tinfo_t ti = get_type_robust(XMM_SIZE, false, false);

    icall.add_argument_reg(src1, ti);

    // Third operand can be XMM or m32
    if (is_mem_op(cdg.insn.Op3)) {
        // Memory operand - load 4 bytes into temp, broadcast/use for insert
        // _mm_insert_ps expects __m128, so we need to load and use appropriately
        // Actually for memory operand, it loads a single float and inserts
        AvxOpLoader src2(cdg, 2, cdg.insn.Op3);
        // For memory, the loaded size should be 4 bytes (single float)
        // But _mm_insert_ps takes __m128, so we create a scalar-to-vector type
        tinfo_t ti_scalar = get_type_robust(4, false, false);
        // Use the loaded operand as-is; the intrinsic handles scalar memory
        icall.add_argument_reg(src2, ti_scalar);
    } else {
        mreg_t src2 = reg2mreg(cdg.insn.Op3.reg);
        icall.add_argument_reg(src2, ti);
    }

    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(dst, ti);
    icall.emit();

    clear_upper(cdg, dst);
    return MERR_OK;
}

// vextractps - extract single float to GPR or memory
// vextractps r32/m32, xmm1, imm8
merror_t handle_vextractps(codegen_t &cdg) {
    // Source is XMM, destination is GPR or memory
    mreg_t src = reg2mreg(cdg.insn.Op2.reg);

    QASSERT(0xA0A30, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value & 0x3;  // Only low 2 bits matter

    // _mm_extract_ps returns int (bit representation of float)
    AVXIntrinsic icall(&cdg, "_mm_extract_ps");
    tinfo_t ti = get_type_robust(XMM_SIZE, false, false);

    icall.add_argument_reg(src, ti);
    icall.add_argument_imm(imm, BT_INT32);

    if (is_mem_op(cdg.insn.Op1)) {
        // Memory destination - extract to temp then store
        mreg_t tmp = cdg.mba->alloc_kreg(4);
        icall.set_return_reg_basic(tmp, BT_INT32);
        icall.emit();
        mop_t src_mop(tmp, 4);
        store_operand_hack(cdg, 0, src_mop);
        cdg.mba->free_kreg(tmp, 4);
    } else {
        // GPR destination
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
        icall.set_return_reg_basic(dst, BT_INT32);
        icall.emit();
    }

    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
