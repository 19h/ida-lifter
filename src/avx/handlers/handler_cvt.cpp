/*
AVX Conversion Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_vcvtdq2ps(codegen_t &cdg) {
    int op_size = get_op_size(cdg.insn);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = make_intrinsic_name("_mm%s_cvtepi32_ps", op_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.set_return_reg(d, get_type_robust(op_size, false));
    icall.add_argument_reg(r, get_type_robust(op_size, true));
    icall.emit();

    if (op_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvtsi2fp(codegen_t &cdg) {
    int src_size = (int) get_dtype_size(cdg.insn.Op3.dtype);
    int dst_size = (cdg.insn.itype == NN_vcvtsi2sd) ? DOUBLE_SIZE : FLOAT_SIZE;

    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    mreg_t t_vec = cdg.mba->alloc_kreg(XMM_SIZE);
    mreg_t t_i2f = cdg.mba->alloc_kreg(src_size);

    cdg.emit(m_mov, XMM_SIZE, l, 0, t_vec, 0);
    cdg.emit(m_i2f, src_size, r, 0, t_i2f, 0);
    cdg.emit(m_f2f, new mop_t(t_i2f, src_size), nullptr, new mop_t(t_vec, dst_size));
    cdg.emit(m_mov, XMM_SIZE, t_vec, 0, d, 0);

    cdg.mba->free_kreg(t_vec, XMM_SIZE);
    cdg.mba->free_kreg(t_i2f, src_size);

    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvtps2pd(codegen_t &cdg) {
    int src128 = is_xmm_reg(cdg.insn.Op1) ? QWORD_SIZE : XMM_SIZE; // element block
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = make_intrinsic_name("_mm%s_cvtps_pd", src128 * 2);
    AVXIntrinsic icall(&cdg, iname.c_str());
    icall.add_argument_reg(r, get_type_robust(16, false)); // Always __m128 input

    icall.set_return_reg(d, get_type_robust(src128 * 2, false, true));
    icall.emit();

    if (src128 == QWORD_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvtfp2fp(codegen_t &cdg) {
    bool is_ss2sd = (cdg.insn.itype == NN_vcvtss2sd);
    int src_size = is_ss2sd ? FLOAT_SIZE : DOUBLE_SIZE;
    int dst_size = is_ss2sd ? DOUBLE_SIZE : FLOAT_SIZE;

    AvxOpLoader r(cdg, 2, cdg.insn.Op3);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, dst_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);

        const char *base_name = is_ss2sd ? "_mm_cvtss_sd" : "_mm_cvtsd_ss";
        qstring iname = make_masked_intrinsic_name(base_name, mask);

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti_dst = get_type_robust(XMM_SIZE, false, is_ss2sd);
        tinfo_t ti_a = get_type_robust(XMM_SIZE, false, is_ss2sd);
        tinfo_t ti_b = get_type_robust(XMM_SIZE, false, !is_ss2sd);

        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti_dst);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);

        mreg_t r_reg;
        mreg_t t_mem = mr_none;
        if (is_mem_op(cdg.insn.Op3)) {
            t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
            mop_t src_mop(r.reg, src_size);
            mop_t dst_mop(t_mem, XMM_SIZE);
            if (XMM_SIZE > 8) {
                dst_mop.set_udt();
            }
            mop_t empty;
            cdg.emit(m_xdu, &src_mop, &empty, &dst_mop);
            r_reg = t_mem;
        } else {
            r_reg = r.reg;
        }

        icall.add_argument_reg(l, ti_a);
        icall.add_argument_reg(r_reg, ti_b);
        icall.set_return_reg(d, ti_dst);
        icall.emit();

        if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
        clear_upper(cdg, d);
        return MERR_OK;
    }

    mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);
    cdg.emit(m_mov, XMM_SIZE, l, 0, t, 0);
    cdg.emit(m_f2f, new mop_t(r, src_size), nullptr, new mop_t(t, dst_size));
    cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);

    cdg.mba->free_kreg(t, XMM_SIZE);

    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvtpd2ps(codegen_t &cdg) {
    int src_size = (get_dtype_size(cdg.insn.Op2.dtype) == 32) ? YMM_SIZE : XMM_SIZE;
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = make_intrinsic_name("_mm%s_cvtpd_ps", src_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.add_argument_reg(r, get_type_robust(src_size, false, true));
    icall.set_return_reg(d, get_type_robust(16, false));
    icall.emit();

    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_ps2dq(codegen_t &cdg, bool trunc) {
    int op_size = get_op_size(cdg.insn);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *iname = (op_size == YMM_SIZE)
                            ? (trunc ? "_mm256_cvttps_epi32" : "_mm256_cvtps_epi32")
                            : (trunc ? "_mm_cvttps_epi32" : "_mm_cvtps_epi32");

    AVXIntrinsic icall(&cdg, iname);
    icall.add_argument_reg(r, get_type_robust(op_size, false));
    icall.set_return_reg(d, get_type_robust(op_size, true));
    icall.emit();

    if (op_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_pd2dq(codegen_t &cdg, bool trunc) {
    int src_size = (get_dtype_size(cdg.insn.Op2.dtype) == 32) ? YMM_SIZE : XMM_SIZE;
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = trunc
                        ? make_intrinsic_name("_mm%s_cvttpd_epi32", src_size)
                        : make_intrinsic_name("_mm%s_cvtpd_epi32", src_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.add_argument_reg(r, get_type_robust(src_size, false, true));
    icall.set_return_reg(d, get_type_robust(16, true));
    icall.emit();

    clear_upper(cdg, d);
    return MERR_OK;
}

// vcvtdq2pd - convert packed dword integers to packed double-precision floats
// vcvtdq2pd xmm1, xmm2/m64  (XMM: 2 ints -> 2 doubles)
// vcvtdq2pd ymm1, xmm2/m128 (YMM: 4 ints -> 4 doubles)
merror_t handle_vcvtdq2pd(codegen_t &cdg) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    int src_size = dst_size / 2;  // Source is half the size (ints are 4 bytes, doubles are 8)

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = make_intrinsic_name("_mm%s_cvtepi32_pd", dst_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    // Source is __m128i (contains 2 or 4 ints depending on dest size)
    icall.add_argument_reg(r, get_type_robust(XMM_SIZE, true));
    icall.set_return_reg(d, get_type_robust(dst_size, false, true));
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_ps2udq(codegen_t &cdg, bool trunc) {
    int op_size = get_op_size(cdg.insn);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = trunc
                        ? make_intrinsic_name("_mm%s_cvttps_epu32", op_size)
                        : make_intrinsic_name("_mm%s_cvtps_epu32", op_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.add_argument_reg(r, get_type_robust(op_size, false));
    icall.set_return_reg(d, get_type_robust(op_size, true));
    icall.emit();

    if (op_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_pd2udq(codegen_t &cdg, bool trunc) {
    int src_size = get_dtype_size(cdg.insn.Op2.dtype);
    if (src_size != XMM_SIZE && src_size != YMM_SIZE && src_size != ZMM_SIZE) {
        src_size = get_vector_size(cdg.insn.Op2);
    }

    int dst_size = get_vector_size(cdg.insn.Op1);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = trunc
                        ? make_intrinsic_name("_mm%s_cvttpd_epu32", src_size)
                        : make_intrinsic_name("_mm%s_cvtpd_epu32", src_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.add_argument_reg(r, get_type_robust(src_size, false, true));
    icall.set_return_reg(d, get_type_robust(dst_size, true));
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_udq2ps(codegen_t &cdg) {
    int op_size = get_op_size(cdg.insn);
    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = make_intrinsic_name("_mm%s_cvtepu32_ps", op_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.add_argument_reg(r, get_type_robust(op_size, true));
    icall.set_return_reg(d, get_type_robust(op_size, false));
    icall.emit();

    if (op_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_udq2pd(codegen_t &cdg) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    int src_size = dst_size / 2;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    qstring iname = make_intrinsic_name("_mm%s_cvtepu32_pd", dst_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    icall.add_argument_reg(r, get_type_robust(src_size, true));
    icall.set_return_reg(d, get_type_robust(dst_size, false, true));
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_pd2qq(codegen_t &cdg, bool trunc, bool is_unsigned) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    int src_size = is_mem_op(cdg.insn.Op2) ? get_dtype_size(cdg.insn.Op2.dtype)
                                           : get_vector_size(cdg.insn.Op2);
    if (src_size == 0) src_size = dst_size;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *fmt = nullptr;
    if (trunc) {
        fmt = is_unsigned ? "_mm%s_cvttpd_epu64" : "_mm%s_cvttpd_epi64";
    } else {
        fmt = is_unsigned ? "_mm%s_cvtpd_epu64" : "_mm%s_cvtpd_epi64";
    }

    qstring iname = make_intrinsic_name(fmt, dst_size);
    AVXIntrinsic icall(&cdg, iname.c_str());
    icall.add_argument_reg(r, get_type_robust(src_size, false, true));
    icall.set_return_reg(d, get_type_robust(dst_size, true, false));
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_ps2qq(codegen_t &cdg, bool trunc, bool is_unsigned) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    int src_size = is_mem_op(cdg.insn.Op2) ? get_dtype_size(cdg.insn.Op2.dtype)
                                           : get_vector_size(cdg.insn.Op2);
    if (src_size == 0) src_size = dst_size / 2;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *fmt = nullptr;
    if (trunc) {
        fmt = is_unsigned ? "_mm%s_cvttps_epu64" : "_mm%s_cvttps_epi64";
    } else {
        fmt = is_unsigned ? "_mm%s_cvtps_epu64" : "_mm%s_cvtps_epi64";
    }

    qstring iname = make_intrinsic_name(fmt, dst_size);
    AVXIntrinsic icall(&cdg, iname.c_str());
    icall.add_argument_reg(r, get_type_robust(src_size, false, false));
    icall.set_return_reg(d, get_type_robust(dst_size, true, false));
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_qq2fp(codegen_t &cdg, bool is_double, bool is_unsigned) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    int src_size = is_mem_op(cdg.insn.Op2) ? get_dtype_size(cdg.insn.Op2.dtype)
                                           : get_vector_size(cdg.insn.Op2);
    if (src_size == 0) src_size = is_double ? dst_size : dst_size * 2;

    AvxOpLoader r(cdg, 1, cdg.insn.Op2);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    const char *fmt = nullptr;
    if (is_double) {
        fmt = is_unsigned ? "_mm%s_cvtepu64_pd" : "_mm%s_cvtepi64_pd";
    } else {
        fmt = is_unsigned ? "_mm%s_cvtepu64_ps" : "_mm%s_cvtepi64_ps";
    }

    qstring iname = make_intrinsic_name(fmt, src_size);
    AVXIntrinsic icall(&cdg, iname.c_str());
    icall.add_argument_reg(r, get_type_robust(src_size, true, false));
    icall.set_return_reg(d, get_type_robust(dst_size, false, is_double));
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvt_fp16(codegen_t &cdg) {
    uint16 it = cdg.insn.itype;

    int dst_size = 0;
    mreg_t d = mr_none;
    if (is_vector_reg(cdg.insn.Op1)) {
        dst_size = get_vector_size(cdg.insn.Op1);
        d = reg2mreg(cdg.insn.Op1.reg);
    } else if (is_mem_op(cdg.insn.Op1)) {
        dst_size = get_dtype_size(cdg.insn.Op1.dtype);
    }
    if (dst_size == 0) {
        dst_size = XMM_SIZE;
    }

    AvxOpLoader src(cdg, 1, cdg.insn.Op2);
    int src_size = src.size > 0 ? src.size : get_vector_size(cdg.insn.Op2);
    if (src_size == 0) {
        src_size = XMM_SIZE;
    }

    const char *iname = nullptr;
    bool src_is_int = false;
    bool src_is_double = false;
    bool dst_is_int = false;
    bool dst_is_double = false;

    switch (it) {
        case NN_vcvtpd2ph:
            iname = (src_size == ZMM_SIZE) ? "_mm512_cvtpd_ph"
                   : (src_size == YMM_SIZE) ? "_mm256_cvtpd_ph"
                                            : "_mm_cvtpd_ph";
            src_is_double = true;
            break;
        case NN_vcvtph2pd:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvtph_pd"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvtph_pd"
                                            : "_mm_cvtph_pd";
            dst_is_double = true;
            break;
        case NN_vcvtph2psx:
            iname = "_mm512_cvtxph_ps";
            break;
        case NN_vcvtps2phx:
            iname = "_mm512_cvtxps_ph";
            break;
        case NN_vcvtph2w:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvtph_epi16"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvtph_epi16"
                                            : "_mm_cvtph_epi16";
            dst_is_int = true;
            break;
        case NN_vcvttph2w:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvttph_epi16"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvttph_epi16"
                                            : "_mm_cvttph_epi16";
            dst_is_int = true;
            break;
        case NN_vcvtph2uw:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvtph_epu16"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvtph_epu16"
                                            : "_mm_cvtph_epu16";
            dst_is_int = true;
            break;
        case NN_vcvttph2uw:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvttph_epu16"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvttph_epu16"
                                            : "_mm_cvttph_epu16";
            dst_is_int = true;
            break;
        case NN_vcvtw2ph:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvtepi16_ph"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvtepi16_ph"
                                            : "_mm_cvtepi16_ph";
            src_is_int = true;
            break;
        case NN_vcvtuw2ph:
            iname = (dst_size == ZMM_SIZE) ? "_mm512_cvtepu16_ph"
                   : (dst_size == YMM_SIZE) ? "_mm256_cvtepu16_ph"
                                            : "_mm_cvtepu16_ph";
            src_is_int = true;
            break;
        default:
            return MERR_INSN;
    }

    AVXIntrinsic icall(&cdg, iname);
    tinfo_t ti_src = get_type_robust(src_size, src_is_int, src_is_double);
    tinfo_t ti_dst = get_type_robust(dst_size, dst_is_int, dst_is_double);

    icall.add_argument_reg(src, ti_src);

    if (d != mr_none) {
        icall.set_return_reg(d, ti_dst);
        icall.emit();
        if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    } else if (is_mem_op(cdg.insn.Op1)) {
        mreg_t tmp = cdg.mba->alloc_kreg(dst_size);
        icall.set_return_reg(tmp, ti_dst);
        icall.emit();
        store_operand_hack(cdg, 0, mop_t(tmp, dst_size));
        cdg.mba->free_kreg(tmp, dst_size);
    } else {
        return MERR_INSN;
    }

    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
