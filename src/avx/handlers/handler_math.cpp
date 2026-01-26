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

    // Determine microcode opcode (use FP opcodes for floating-point operations)
    mcode_t opcode;
    switch (cdg.insn.itype) {
        case NN_vaddss:
        case NN_vaddsd: opcode = m_fadd;
            break;
        case NN_vsubss:
        case NN_vsubsd: opcode = m_fsub;
            break;
        case NN_vmulss:
        case NN_vmulsd: opcode = m_fmul;
            break;
        case NN_vdivss:
        case NN_vdivsd: opcode = m_fdiv;
            break;
        default: return MERR_INSN;
    }

    // Emit scalar FP operation using native microcode
    // vaddss xmm1, xmm2, xmm3/mem semantics:
    // - xmm1[31:0] = xmm2[31:0] op xmm3/mem[31:0]
    // - xmm1[127:32] = xmm2[127:32] (copy upper bits from first source)
    // - xmm1[255:128] = 0 (VEX zeros upper bits)

    // First, copy the full XMM from source 2 to dest (preserves upper bits)
    if (l != d) {
        cdg.emit(m_mov, XMM_SIZE, l, 0, d, 0);
    }

    // Then do the scalar operation on the low element
    mreg_t r = r_in;
    mop_t l_mop(l, elem_size);
    mop_t r_mop(r, elem_size);
    mop_t d_mop(d, elem_size);
    cdg.emit(opcode, &l_mop, &r_mop, &d_mop);
    // Note: m_f* opcodes are implicitly floating-point, no need for set_fpinsn()

    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_minmax_ss_sd(codegen_t &cdg) {
    QASSERT(0xA0503, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    bool is_double = (cdg.insn.itype == NN_vminsd || cdg.insn.itype == NN_vmaxsd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Handle memory operands: load scalar and zero-extend to XMM
    mreg_t r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        // Load scalar from memory
        AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
        // Zero-extend to XMM size for use in intrinsic
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(r_in.reg, elem_size);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);
        r = t_mem;
    } else {
        r = reg2mreg(cdg.insn.Op3.reg);
    }

    const char *which = (cdg.insn.itype == NN_vminss || cdg.insn.itype == NN_vminsd) ? "min" : "max";

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm_%s_%s", which, is_double ? "sd" : "ss");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(16, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, vt);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, vt);
    icall.add_argument_reg(r, vt);
    icall.set_return_reg(d, vt);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_math_ph(codegen_t &cdg) {
    QASSERT(0xA0700, is_vector_reg(cdg.insn.Op1) && is_vector_reg(cdg.insn.Op2));

    int size = get_vector_size(cdg.insn.Op1);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vaddph: op = "add"; break;
        case NN_vsubph: op = "sub"; break;
        case NN_vmulph: op = "mul"; break;
        case NN_vdivph: op = "div"; break;
        case NN_vminph: op = "min"; break;
        case NN_vmaxph: op = "max"; break;
        default: return MERR_INSN;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 2);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_ph", get_size_prefix(size), op);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_math_sh(codegen_t &cdg) {
    QASSERT(0xA0701, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vaddsh: op = "add"; break;
        case NN_vsubsh: op = "sub"; break;
        case NN_vmulsh: op = "mul"; break;
        case NN_vdivsh: op = "div"; break;
        case NN_vminsh: op = "min"; break;
        case NN_vmaxsh: op = "max"; break;
        default: return MERR_INSN;
    }

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    mreg_t r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(r_in.reg, WORD_SIZE);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);
        r = t_mem;
    } else {
        r = reg2mreg(cdg.insn.Op3.reg);
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, WORD_SIZE);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm_%s_sh", op);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(XMM_SIZE, false, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_sqrt_ph(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 2);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_sqrt_ph", get_size_prefix(size));
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_fma_ph(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t op1 = reg2mreg(cdg.insn.Op1.reg);
    mreg_t op2 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader op3_in(cdg, 2, cdg.insn.Op3);

    int order = 0;
    bool is_scalar = false;

    switch (cdg.insn.itype) {
        case NN_vfmadd132ph: order = 132; break;
        case NN_vfmadd213ph: order = 213; break;
        case NN_vfmadd231ph: order = 231; break;
        case NN_vfmadd132sh: order = 132; is_scalar = true; break;
        case NN_vfmadd213sh: order = 213; is_scalar = true; break;
        case NN_vfmadd231sh: order = 231; is_scalar = true; break;
        default: return MERR_INSN;
    }

    qstring base_name;
    if (is_scalar) {
        base_name = "_mm_fmadd_sh";
    } else {
        base_name.cat_sprnt("_mm%s_fmadd_ph", get_size_prefix(size));
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, WORD_SIZE);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring iname = make_masked_intrinsic_name(base_name.c_str(), mask);
    AVXIntrinsic icall(&cdg, iname.c_str());

    int vec_size = is_scalar ? XMM_SIZE : size;
    tinfo_t ti = get_type_robust(vec_size, false, false);

    mreg_t op3 = op3_in;
    mreg_t t_mem = mr_none;
    if (is_scalar && is_mem_op(cdg.insn.Op3)) {
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(op3_in, WORD_SIZE);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);
        op3 = t_mem;
    }

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

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_complex_ph(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    bool is_ternary = false;
    switch (cdg.insn.itype) {
        case NN_vfcmulcph: op = "fcmul"; break;
        case NN_vfmulcph: op = "fmul"; break;
        case NN_vfcmaddcph: op = "fcmadd"; is_ternary = true; break;
        case NN_vfmaddcph: op = "fmadd"; is_ternary = true; break;
        default: return MERR_INSN;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 4);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_pch", get_size_prefix(size), op);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    mreg_t a = is_ternary ? reg2mreg(cdg.insn.Op1.reg) : reg2mreg(cdg.insn.Op2.reg);
    mreg_t b_reg = is_ternary ? reg2mreg(cdg.insn.Op2.reg) : b;

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b_reg, ti);
    if (is_ternary) {
        icall.add_argument_reg(b, ti);
    }
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_math_p(codegen_t &cdg) {
    QASSERT(0xA0501, is_vector_reg(cdg.insn.Op1) && is_vector_reg(cdg.insn.Op2));

    int size = get_vector_size(cdg.insn.Op1);

    // Note: ZMM memory operands are now handled via emit_zmm_load/emit_zmm_store
    // which bypass cdg.load_operand() and manually emit m_ldx/m_stx with UDT flags

    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *fmt = nullptr;
    bool is_int = false;
    bool is_double = false;
    int elem_size = 4; // Default to 32-bit elements

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
            elem_size = 8;
            break;
        case NN_vsubpd: fmt = "_mm%s_sub_pd";
            is_double = true;
            elem_size = 8;
            break;
        case NN_vmulpd: fmt = "_mm%s_mul_pd";
            is_double = true;
            elem_size = 8;
            break;
        case NN_vdivpd: fmt = "_mm%s_div_pd";
            is_double = true;
            elem_size = 8;
            break;
        // FP min/max
        case NN_vminps: fmt = "_mm%s_min_ps";
            break;
        case NN_vmaxps: fmt = "_mm%s_max_ps";
            break;
        case NN_vminpd: fmt = "_mm%s_min_pd";
            is_double = true;
            elem_size = 8;
            break;
        case NN_vmaxpd: fmt = "_mm%s_max_pd";
            is_double = true;
            elem_size = 8;
            break;
        // INT add/sub
        case NN_vpaddb: fmt = "_mm%s_add_epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpsubb: fmt = "_mm%s_sub_epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpaddw: fmt = "_mm%s_add_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpsubw: fmt = "_mm%s_sub_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpaddd: fmt = "_mm%s_add_epi32";
            is_int = true;
            break;
        case NN_vpsubd: fmt = "_mm%s_sub_epi32";
            is_int = true;
            break;
        case NN_vpaddq: fmt = "_mm%s_add_epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpsubq: fmt = "_mm%s_sub_epi64";
            is_int = true;
            elem_size = 8;
            break;
        // INT saturating add/sub
        case NN_vpaddsb: fmt = "_mm%s_adds_epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpsubsb: fmt = "_mm%s_subs_epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpaddsw: fmt = "_mm%s_adds_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpsubsw: fmt = "_mm%s_subs_epi16";
            is_int = true;
            elem_size = 2;
            break;
        // INT min/max (signed)
        case NN_vpminsb: fmt = "_mm%s_min_epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpmaxsb: fmt = "_mm%s_max_epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpminsw: fmt = "_mm%s_min_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpmaxsw: fmt = "_mm%s_max_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpminsd: fmt = "_mm%s_min_epi32";
            is_int = true;
            break;
        case NN_vpminsq: fmt = "_mm%s_min_epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpmaxsd: fmt = "_mm%s_max_epi32";
            is_int = true;
            break;
        case NN_vpmaxsq: fmt = "_mm%s_max_epi64";
            is_int = true;
            elem_size = 8;
            break;
        // INT min/max (unsigned)
        case NN_vpminub: fmt = "_mm%s_min_epu8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpmaxub: fmt = "_mm%s_max_epu8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpminuw: fmt = "_mm%s_min_epu16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpmaxuw: fmt = "_mm%s_max_epu16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpminud: fmt = "_mm%s_min_epu32";
            is_int = true;
            break;
        case NN_vpminuq: fmt = "_mm%s_min_epu64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpmaxud: fmt = "_mm%s_max_epu32";
            is_int = true;
            break;
        case NN_vpmaxuq: fmt = "_mm%s_max_epu64";
            is_int = true;
            elem_size = 8;
            break;
        // INT multiply
        case NN_vpmullw: fmt = "_mm%s_mullo_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpmulld: fmt = "_mm%s_mullo_epi32";
            is_int = true;
            break;
        case NN_vpmullq: fmt = "_mm%s_mullo_epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpmulhw: fmt = "_mm%s_mulhi_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpmulhuw: fmt = "_mm%s_mulhi_epu16";
            is_int = true;
            elem_size = 2;
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
            elem_size = 1;
            break;
        case NN_vpavgw: fmt = "_mm%s_avg_epu16";
            is_int = true;
            elem_size = 2;
            break;
        // INT multiply-add
        case NN_vpmaddwd: fmt = "_mm%s_madd_epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpmaddubsw: fmt = "_mm%s_maddubs_epi16";
            is_int = true;
            elem_size = 1;
            break;
        default: QASSERT(0xA0502, false);
    }

    // Check for AVX-512 masking
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    // Build base intrinsic name
    qstring base_name;
    base_name.cat_sprnt(fmt, get_size_prefix(size));

    // Transform to masked name if needed
    qstring iname = make_masked_intrinsic_name(base_name.c_str(), mask);
    AVXIntrinsic icall(&cdg, iname.c_str());

    tinfo_t ti = get_type_robust(size, is_int, is_double);

    // For merge-masking: src, k, a, b -> result
    // For zero-masking:  k, a, b -> result
    // For no masking:    a, b -> result
    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            // Merge-masking: first arg is src (original dest value)
            icall.add_argument_reg(d, ti);
        }
        // Add mask argument (may be negative for k-reg number encoding)
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_abs(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
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
        case NN_vpabsq: suffix = "epi64";
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_abs_%s", get_size_prefix(size), suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_sign(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
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
    iname.cat_sprnt("_mm%s_sign_%s", get_size_prefix(size), suffix);

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
    int size = get_vector_size(cdg.insn.Op1);
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

    // FMA enum layout in IDA SDK:
    // NN_vfmadd132pd, NN_vfmadd132ps, NN_vfmadd132sd, NN_vfmadd132ss,
    // NN_vfmadd213pd, NN_vfmadd213ps, NN_vfmadd213sd, NN_vfmadd213ss,
    // NN_vfmadd231pd, NN_vfmadd231ps, NN_vfmadd231sd, NN_vfmadd231ss, ...
    // So stride between 132->213->231 is 4 (not 1!)

    auto check = [&](uint16 base132, const char *t, bool dbl, bool scl) {
        if (it == base132) {
            type = t;
            order = 132;
            is_double = dbl;
            is_scalar = scl;
            return true;
        }
        if (it == base132 + 4) {  // 132->213 stride is 4
            type = t;
            order = 213;
            is_double = dbl;
            is_scalar = scl;
            return true;
        }
        if (it == base132 + 8) {  // 132->231 stride is 8
            type = t;
            order = 231;
            is_double = dbl;
            is_scalar = scl;
            return true;
        }
        return false;
    };

    // Check each type variant with its specific base132 instruction
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

    const char *prefix = is_scalar ? "" : get_size_prefix(size);
    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_%s", prefix, op, type);

    int elem_size = is_double ? 8 : 4;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }
    qstring iname = make_masked_intrinsic_name(base_name.c_str(), mask);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    mreg_t op3 = op3_in;
    mreg_t t_mem = mr_none;
    if (is_scalar && is_mem_op(cdg.insn.Op3)) {
        // For scalar FMA with memory operand, zero-extend the loaded scalar to XMM size
        // Reuse elem_size for scalar load size
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(op3_in, elem_size);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
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

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_ifma(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t acc = d;
    mreg_t a = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpmadd52luq: op = "madd52lo"; break;
        case NN_vpmadd52huq: op = "madd52hi"; break;
        default: return MERR_INSN;
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_epu64", get_size_prefix(size), op);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 8);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (mask.is_zeroing) {
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            icall.add_argument_reg(acc, ti);
        } else {
            icall.add_argument_reg(acc, ti);
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }
    } else {
        icall.add_argument_reg(acc, ti);
    }

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_vnni(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t acc = d;
    mreg_t a = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpdpbusd: op = "dpbusd"; break;
        case NN_vpdpbusds: op = "dpbusds"; break;
        case NN_vpdpwssd: op = "dpwssd"; break;
        case NN_vpdpwssds: op = "dpwssds"; break;
        default: return MERR_INSN;
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_epi32", get_size_prefix(size), op);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 4);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (mask.is_zeroing) {
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            icall.add_argument_reg(acc, ti);
        } else {
            icall.add_argument_reg(acc, ti);
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }
    } else {
        icall.add_argument_reg(acc, ti);
    }

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_bf16(codegen_t &cdg) {
    uint16 it = cdg.insn.itype;

    if (it == NN_vdpbf16ps) {
        int size = get_vector_size(cdg.insn.Op1);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        mreg_t acc = d;
        mreg_t a = reg2mreg(cdg.insn.Op2.reg);
        AvxOpLoader b(cdg, 2, cdg.insn.Op3);

        MaskInfo mask = MaskInfo::from_insn(cdg.insn, 4);
        if (mask.has_mask) {
            load_mask_operand(cdg, mask);
        }

        qstring base_name;
        base_name.cat_sprnt("_mm%s_dpbf16_ps", get_size_prefix(size));
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti_out = get_type_robust(size, false, false);
        tinfo_t ti_bf = get_type_robust(size, true, false);

        if (mask.has_mask) {
            if (mask.is_zeroing) {
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
                icall.add_argument_reg(acc, ti_out);
            } else {
                icall.add_argument_reg(acc, ti_out);
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            }
        } else {
            icall.add_argument_reg(acc, ti_out);
        }

        icall.add_argument_reg(a, ti_bf);
        icall.add_argument_reg(b, ti_bf);
        icall.set_return_reg(d, ti_out);
        icall.emit();

        if (size == XMM_SIZE) clear_upper(cdg, d);
        return MERR_OK;
    }

    if (it == NN_vcvtne2ps2bf16) {
        int size = get_vector_size(cdg.insn.Op1);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        AvxOpLoader a(cdg, 1, cdg.insn.Op2);
        AvxOpLoader b(cdg, 2, cdg.insn.Op3);

        MaskInfo mask = MaskInfo::from_insn(cdg.insn, 2);
        if (mask.has_mask) {
            load_mask_operand(cdg, mask);
        }

        qstring base_name;
        base_name.cat_sprnt("_mm%s_cvtne2ps_pbh", get_size_prefix(size));
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti_dst = get_type_robust(size, true, false);
        tinfo_t ti_src = get_type_robust(size, false, false);

        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti_dst);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }

        icall.add_argument_reg(a, ti_src);
        icall.add_argument_reg(b, ti_src);
        icall.set_return_reg(d, ti_dst);
        icall.emit();

        if (size == XMM_SIZE) clear_upper(cdg, d);
        return MERR_OK;
    }

    if (it == NN_vcvtneps2bf16) {
        int dst_size = get_vector_size(cdg.insn.Op1);
        int src_size = is_mem_op(cdg.insn.Op2) ? get_dtype_size(cdg.insn.Op2.dtype)
                                                : get_vector_size(cdg.insn.Op2);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        AvxOpLoader src(cdg, 1, cdg.insn.Op2);

        MaskInfo mask = MaskInfo::from_insn(cdg.insn, 2);
        if (mask.has_mask) {
            load_mask_operand(cdg, mask);
        }

        qstring base_name;
        base_name.cat_sprnt("_mm%s_cvtneps_pbh", get_size_prefix(src_size));
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti_dst = get_type_robust(dst_size, true, false);
        tinfo_t ti_src = get_type_robust(src_size, false, false);

        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti_dst);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }

        icall.add_argument_reg(src, ti_src);
        icall.set_return_reg(d, ti_dst);
        icall.emit();

        if (dst_size == XMM_SIZE) clear_upper(cdg, d);
        return MERR_OK;
    }

    return MERR_INSN;
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
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vsqrtpd);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    iname.cat_sprnt("_mm%s_sqrt_%s", get_size_prefix(size), is_double ? "pd" : "ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_hmath(codegen_t &cdg) {
    QASSERT(0xA0504, is_vector_reg(cdg.insn.Op1) && is_vector_reg(cdg.insn.Op2));

    int size = get_vector_size(cdg.insn.Op1);
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
    iname.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, type);

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
    QASSERT(0xA0505, is_vector_reg(cdg.insn.Op1) && is_vector_reg(cdg.insn.Op2));

    int size = get_vector_size(cdg.insn.Op1);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Op4 is the immediate mask
    QASSERT(0xA0506, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    qstring iname;
    iname.cat_sprnt("_mm%s_dp_ps", get_size_prefix(size));

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
    int size = get_vector_size(cdg.insn.Op1);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    bool is_double = (cdg.insn.itype == NN_vrcp14pd || cdg.insn.itype == NN_vrsqrt14pd);
    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vrcpps: op = "rcp"; break;
        case NN_vrsqrtps: op = "rsqrt"; break;
        case NN_vrcp14ps:
        case NN_vrcp14pd: op = "rcp14"; break;
        case NN_vrsqrt14ps:
        case NN_vrsqrt14pd: op = "rsqrt14"; break;
        default: return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, is_double ? "pd" : "ps");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vround(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vroundpd || cdg.insn.itype == NN_vrndscalepd);
    bool is_scale = (cdg.insn.itype == NN_vrndscaleps || cdg.insn.itype == NN_vrndscalepd);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Op3 is the rounding mode immediate
    QASSERT(0xA0507, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value;

    qstring iname;
    if (is_scale) {
        iname.cat_sprnt("_mm%s_roundscale_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    } else {
        iname.cat_sprnt("_mm%s_round_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    }

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32); // Rounding mode is int
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_getexp(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vgetexppd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_getexp_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_getmant(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vgetmantpd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    QASSERT(0xA0620, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_getmant_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_fixupimm(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vfixupimmpd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    QASSERT(0xA0621, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_fixupimm_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_scalef(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vscalefpd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_scalef_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_range(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vrangepd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    QASSERT(0xA0622, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_range_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_reduce(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vreducepd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    QASSERT(0xA0623, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_reduce_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// Scalar approximations: vrsqrtss, vrcpss
// vrcp/rsqrtss xmm1, xmm2, xmm3/m32 - scalar reciprocal/rsqrt
merror_t handle_vrcp_rsqrt_ss(codegen_t &cdg) {
    QASSERT(0xA0608, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Handle memory operand for Op3
    mreg_t r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
        // Zero-extend scalar to XMM for intrinsic
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(r_in.reg, FLOAT_SIZE);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);
        r = t_mem;
    } else {
        r = reg2mreg(cdg.insn.Op3.reg);
    }

    const char *op = (cdg.insn.itype == NN_vrcpss) ? "rcp" : "rsqrt";
    qstring iname;
    iname.cat_sprnt("_mm_%s_ss", op);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(XMM_SIZE, false, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

// Scalar rounding: vroundss, vroundsd
// vroundss/sd xmm1, xmm2, xmm3/m32, imm8
merror_t handle_vround_ss_sd(codegen_t &cdg) {
    QASSERT(0xA0609, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    bool is_double = (cdg.insn.itype == NN_vroundsd);
    int elem_size = is_double ? DOUBLE_SIZE : FLOAT_SIZE;
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Handle memory operand for Op3
    mreg_t r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(r_in.reg, elem_size);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);
        r = t_mem;
    } else {
        r = reg2mreg(cdg.insn.Op3.reg);
    }

    // Op4 is the rounding mode immediate
    QASSERT(0xA0610, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    qstring iname;
    iname.cat_sprnt("_mm_round_%s", is_double ? "sd" : "ss");

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(XMM_SIZE, false, is_double);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

// Scalar sqrt for double: vsqrtsd xmm1, xmm2, xmm3/m64
merror_t handle_vsqrtsd(codegen_t &cdg) {
    QASSERT(0xA0611, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    // Handle memory operand for Op3
    mreg_t r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op3)) {
        AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src(r_in.reg, DOUBLE_SIZE);
        mop_t dst(t_mem, XMM_SIZE);
        if (XMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t empty;
        cdg.emit(m_xdu, &src, &empty, &dst);
        r = t_mem;
    } else {
        r = reg2mreg(cdg.insn.Op3.reg);
    }

    // Copy upper bits from src2, compute sqrt on low element
    mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);
    cdg.emit(m_mov, XMM_SIZE, l, 0, t, 0);

    AVXIntrinsic icall(&cdg, "fsqrt");
    icall.add_argument_reg(r, BTF_DOUBLE);
    icall.set_return_reg_basic(t, BTF_DOUBLE);
    icall.emit();

    cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);
    cdg.mba->free_kreg(t, XMM_SIZE);
    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

// vaddsubps/pd - alternating add/sub
merror_t handle_vaddsubps_pd(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_double = (cdg.insn.itype == NN_vaddsubpd);

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    int elem_size = is_double ? 8 : 4;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_addsub_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

// vpsadbw - compute sum of absolute differences of unsigned bytes
// vpsadbw xmm1, xmm2, xmm3/m128
// vpsadbw ymm1, ymm2, ymm3/m256
// vmpsadbw - compute multiple packed sums of absolute differences
// vmpsadbw xmm1, xmm2, xmm3/m128, imm8
// vmpsadbw ymm1, ymm2, ymm3/m256, imm8
merror_t handle_vsad(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    bool is_mpsadbw = (cdg.insn.itype == NN_vmpsadbw);

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname;
    if (is_mpsadbw) {
        iname = make_intrinsic_name("_mm%s_mpsadbw_epu8", size);
    } else {
        iname = make_intrinsic_name("_mm%s_sad_epu8", size);
    }

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(l, ti);
    icall.add_argument_reg(r, ti);

    if (is_mpsadbw) {
        // vmpsadbw has an immediate operand
        uint8 imm = (uint8)cdg.insn.Op4.value;
        icall.add_argument_imm(imm, 4);
    }

    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
