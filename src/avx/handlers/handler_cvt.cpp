/*
AVX Conversion Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#include "../../common/warn_off.h"
#include <bytes.hpp>
#include <funcs.hpp>
#include <ua.hpp>
#include "../../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

static int get_xmm_reg_index(const op_t &op) {
    if (!is_xmm_reg(op)) return -1;
    if (op.reg >= R_xmm0 && op.reg <= R_xmm15) return op.reg - R_xmm0;
    if (op.reg >= R_xmm16 && op.reg <= R_xmm31) return op.reg - R_xmm16 + 16;
    return -1;
}

static bool insn_uses_zmm_reg(const insn_t &insn) {
    return is_zmm_reg(insn.Op1) || is_zmm_reg(insn.Op2) || is_zmm_reg(insn.Op3) ||
           is_zmm_reg(insn.Op4) || is_zmm_reg(insn.Op5) || is_zmm_reg(insn.Op6);
}

static bool function_uses_zmm_reg(ea_t ea) {
    func_t *pfn = get_func(ea);
    if (pfn == nullptr) return false;

    for (ea_t item = pfn->start_ea; item < pfn->end_ea; item = next_head(item, pfn->end_ea)) {
        insn_t insn;
        if (decode_insn(&insn, item) <= 0) continue;
        if (insn_uses_zmm_reg(insn)) return true;
    }
    return false;
}

static bool previous_insn_is_zmm_call(ea_t ea) {
    func_t *pfn = get_func(ea);
    if (pfn == nullptr) return false;

    ea_t prev = prev_head(ea, pfn->start_ea);
    if (prev == BADADDR) return false;

    insn_t insn;
    if (decode_insn(&insn, prev) <= 0 || insn.itype != NN_call) return false;
    if (insn.Op1.type != o_near && insn.Op1.type != o_far) return false;
    return function_uses_zmm_reg(insn.Op1.addr);
}

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

    mreg_t src = r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op2) && src128 == QWORD_SIZE) {
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src_mop(r.reg, QWORD_SIZE);
        mop_t dst_mop(t_mem, XMM_SIZE);
        dst_mop.set_udt();
        mop_t empty;
        cdg.emit(m_xdu, &src_mop, &empty, &dst_mop);
        src = t_mem;
    }

    qstring iname = make_intrinsic_name("_mm%s_cvtps_pd", src128 * 2);
    AVXIntrinsic icall(&cdg, iname.c_str());
    icall.add_argument_reg(src, get_type_robust(16, false)); // Always __m128 input

    icall.set_return_reg(d, get_type_robust(src128 * 2, false, true));
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (src128 == QWORD_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vcvtfp2fp(codegen_t &cdg) {
    bool is_ss2sd = (cdg.insn.itype == NN_vcvtss2sd);
    int src_size = is_ss2sd ? FLOAT_SIZE : DOUBLE_SIZE;
    int dst_size = is_ss2sd ? DOUBLE_SIZE : FLOAT_SIZE;

    mreg_t l = reg2mreg(cdg.insn.Op2.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    int zmm_alias = is_ss2sd ? get_xmm_reg_index(cdg.insn.Op3) : -1;
    if (zmm_alias >= 0 && previous_insn_is_zmm_call(cdg.insn.ea)) {
        AVXIntrinsic read(&cdg, "__readzmm_f32");
        mreg_t scalar = cdg.mba->alloc_kreg(FLOAT_SIZE);
        if (scalar == mr_none) return MERR_INSN;
        read.add_argument_imm((uint64) zmm_alias, BT_INT32);
        read.set_return_reg(scalar, tinfo_t(BT_FLOAT));
        if (read.emit() == nullptr) return MERR_INSN;

        mop_t src(scalar, FLOAT_SIZE);
        mop_t dst(d, DOUBLE_SIZE);
        cdg.emit(m_f2f, &src, nullptr, &dst);
        clear_upper(cdg, d);
        return MERR_OK;
    }

    AvxOpLoader r(cdg, 2, cdg.insn.Op3);

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

    mreg_t src = r;
    mreg_t t_mem = mr_none;
    if (is_mem_op(cdg.insn.Op2) && src_size < XMM_SIZE) {
        t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
        mop_t src_mop(r.reg, src_size);
        mop_t dst_mop(t_mem, XMM_SIZE);
        dst_mop.set_udt();
        mop_t empty;
        cdg.emit(m_xdu, &src_mop, &empty, &dst_mop);
        src = t_mem;
    }

    qstring iname = make_intrinsic_name("_mm%s_cvtepi32_pd", dst_size);
    AVXIntrinsic icall(&cdg, iname.c_str());

    // Source is __m128i (contains 2 or 4 ints depending on dest size)
    icall.add_argument_reg(src, get_type_robust(XMM_SIZE, true));
    icall.set_return_reg(d, get_type_robust(dst_size, false, true));
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vmxcsr(codegen_t &cdg) {
    if (cdg.insn.itype == NN_vldmxcsr) {
        AvxOpLoader src(cdg, 0, cdg.insn.Op1);
        AVXIntrinsic icall(&cdg, "_mm_setcsr");
        icall.add_argument_reg(src, BT_INT32);
        icall.emit_void();
        return MERR_OK;
    }

    if (cdg.insn.itype == NN_vstmxcsr) {
        mreg_t tmp = cdg.mba->alloc_kreg(DWORD_SIZE);
        AVXIntrinsic icall(&cdg, "_mm_getcsr");
        icall.set_return_reg_basic(tmp, BT_INT32);
        icall.emit();
        mop_t src(tmp, DWORD_SIZE);
        store_operand_hack(cdg, 0, src);
        cdg.mba->free_kreg(tmp, DWORD_SIZE);
        return MERR_OK;
    }

    return MERR_INSN;
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
    qstring namebuf;
    bool src_is_int = false;
    bool src_is_double = false;
    bool dst_is_int = false;
    bool dst_is_double = false;
    bool has_imm = false;
    // Intrinsic width prefix is named after the wider (full 512/256/128-bit) operand.
    const char *wp = get_size_prefix(src_size > dst_size ? src_size : dst_size);

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
        // --- integer <-> fp16 (32/64-bit element conversions) ---
        case NN_vcvtdq2ph:  namebuf.cat_sprnt("_mm%s_cvtepi32_ph", wp); src_is_int = true; break;
        case NN_vcvtudq2ph: namebuf.cat_sprnt("_mm%s_cvtepu32_ph", wp); src_is_int = true; break;
        case NN_vcvtqq2ph:  namebuf.cat_sprnt("_mm%s_cvtepi64_ph", wp); src_is_int = true; break;
        case NN_vcvtuqq2ph: namebuf.cat_sprnt("_mm%s_cvtepu64_ph", wp); src_is_int = true; break;
        case NN_vcvtph2dq:  namebuf.cat_sprnt("_mm%s_cvtph_epi32", wp); dst_is_int = true; break;
        case NN_vcvttph2dq: namebuf.cat_sprnt("_mm%s_cvttph_epi32", wp); dst_is_int = true; break;
        case NN_vcvtph2udq: namebuf.cat_sprnt("_mm%s_cvtph_epu32", wp); dst_is_int = true; break;
        case NN_vcvttph2udq:namebuf.cat_sprnt("_mm%s_cvttph_epu32", wp); dst_is_int = true; break;
        case NN_vcvtph2qq:  namebuf.cat_sprnt("_mm%s_cvtph_epi64", wp); dst_is_int = true; break;
        case NN_vcvttph2qq: namebuf.cat_sprnt("_mm%s_cvttph_epi64", wp); dst_is_int = true; break;
        case NN_vcvtph2uqq: namebuf.cat_sprnt("_mm%s_cvtph_epu64", wp); dst_is_int = true; break;
        case NN_vcvttph2uqq:namebuf.cat_sprnt("_mm%s_cvttph_epu64", wp); dst_is_int = true; break;
        // --- F16C fp16 <-> fp32 (non-EVEX encodings) ---
        case NN_vcvtph2ps:  namebuf.cat_sprnt("_mm%s_cvtph_ps", wp); break;
        case NN_vcvtps2ph:  namebuf.cat_sprnt("_mm%s_cvtps_ph", wp); has_imm = true; break;
        default:
            return MERR_INSN;
    }
    if (iname == nullptr) iname = namebuf.c_str();

    AVXIntrinsic icall(&cdg, iname);
    tinfo_t ti_src = get_type_robust(src_size, src_is_int, src_is_double);
    tinfo_t ti_dst = get_type_robust(dst_size, dst_is_int, dst_is_double);

    // Source: route ZMM through the read helper (not microcode-addressable).
    if (is_zmm_reg(cdg.insn.Op2)) {
        if (!add_zmm_read_arg(cdg, icall, cdg.insn.Op2, ti_src)) return MERR_INSN;
    } else {
        icall.add_argument_reg(src, ti_src);
    }
    if (has_imm && cdg.insn.Op3.type == o_imm) {
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
    }

    if (is_zmm_reg(cdg.insn.Op1)) {
        mreg_t tmp = cdg.mba->alloc_kreg(dst_size, false);
        if (tmp == mr_none) return MERR_INSN;
        icall.set_return_reg(tmp, ti_dst);
        if (icall.emit() == nullptr) return MERR_INSN;
        if (!emit_zmm_write_call(cdg, cdg.insn.Op1, tmp, ti_dst)) return MERR_INSN;
    } else if (d != mr_none) {
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

// Scalar conversions added by AVX-512F/FP16:
//   * scalar fp -> unsigned/signed integer GPR  (vcvt[t]s{s,d,h}2usi, vcvt[t]sh2si)
//   * integer GPR -> scalar fp                  (vcvtusi2s{s,d}, vcvt[u]si2sh)
//   * scalar fp <-> scalar fp16                 (vcvts{d,s}2sh, vcvtsh2s{d,s})
merror_t handle_vcvt_scalar_ext(codegen_t &cdg) {
    uint16 it = cdg.insn.itype;

    // --- category 1: scalar fp -> integer in a GPR (1 source) ---
    struct G2I { uint16 it; const char *base; bool src_double; };
    static const G2I g2i[] = {
        {NN_vcvtsh2si,  "cvtsh_i",  false}, {NN_vcvttsh2si, "cvttsh_i", false},
        {NN_vcvtsh2usi, "cvtsh_u",  false}, {NN_vcvttsh2usi,"cvttsh_u", false},
        {NN_vcvtsd2usi, "cvtsd_u",  true},  {NN_vcvttsd2usi,"cvttsd_u", true},
        {NN_vcvtss2usi, "cvtss_u",  false}, {NN_vcvttss2usi,"cvttss_u", false},
    };
    for (const G2I &e : g2i) {
        if (e.it != it) continue;
        int gsz = (int) get_dtype_size(cdg.insn.Op1.dtype);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        if (d == mr_none) return MERR_INSN;
        AvxOpLoader s(cdg, 1, cdg.insn.Op2);
        qstring nm; nm.cat_sprnt("_mm_%s%d", e.base, gsz == 8 ? 64 : 32);
        AVXIntrinsic ic(&cdg, nm.c_str());
        // Op2 may be a scalar m16/m32/m64 memory load; widen to XMM width so the
        // argument does not read undefined upper bytes of the load (INTERR 50920).
        ic.add_argument_reg(widen_loaded_value(cdg, s.reg, s.size, XMM_SIZE),
                            get_type_robust(XMM_SIZE, false, e.src_double));
        ic.set_return_reg_basic(d, gsz == 8 ? BT_INT64 : BT_INT32);
        ic.emit();
        return MERR_OK;
    }

    // --- category 2: integer GPR -> scalar fp (dst, src1=merge xmm, src2=gpr) ---
    struct I2F { uint16 it; const char *pre; const char *suf; bool dst_double; };
    static const I2F i2f[] = {
        {NN_vcvtusi2sd, "cvtu", "sd", true},  {NN_vcvtusi2ss, "cvtu", "ss", false},
        {NN_vcvtsi2sh,  "cvti", "sh", false}, {NN_vcvtusi2sh, "cvtu", "sh", false},
    };
    for (const I2F &e : i2f) {
        if (e.it != it) continue;
        int gsz = (int) get_dtype_size(cdg.insn.Op3.dtype);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        mreg_t a = reg2mreg(cdg.insn.Op2.reg);
        if (d == mr_none || a == mr_none) return MERR_INSN;
        AvxOpLoader b(cdg, 2, cdg.insn.Op3);
        qstring nm; nm.cat_sprnt("_mm_%s%d_%s", e.pre, gsz == 8 ? 64 : 32, e.suf);
        AVXIntrinsic ic(&cdg, nm.c_str());
        ic.add_argument_reg(a, get_type_robust(XMM_SIZE, false, e.dst_double));
        ic.add_argument_reg(b, gsz == 8 ? BT_INT64 : BT_INT32);
        ic.set_return_reg(d, get_type_robust(XMM_SIZE, false, e.dst_double));
        ic.emit();
        clear_upper(cdg, d);
        return MERR_OK;
    }

    // --- category 3: scalar fp <-> scalar fp16 (dst, src1=merge a, src2=b) ---
    struct F2F { uint16 it; const char *name; bool a_double; bool b_double; };
    static const F2F f2f[] = {
        {NN_vcvtsd2sh, "_mm_cvtsd_sh", false, true},
        {NN_vcvtsh2sd, "_mm_cvtsh_sd", true,  false},
        {NN_vcvtss2sh, "_mm_cvtss_sh", false, false},
        {NN_vcvtsh2ss, "_mm_cvtsh_ss", false, false},
    };
    for (const F2F &e : f2f) {
        if (e.it != it) continue;
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        mreg_t a = reg2mreg(cdg.insn.Op2.reg);
        if (d == mr_none || a == mr_none) return MERR_INSN;
        AvxOpLoader b(cdg, 2, cdg.insn.Op3);
        AVXIntrinsic ic(&cdg, e.name);
        ic.add_argument_reg(a, get_type_robust(XMM_SIZE, false, e.a_double));
        // Op3 may be a scalar m16/m32/m64 memory load; widen to XMM width so the
        // argument does not read undefined upper bytes of the load (INTERR 50920).
        ic.add_argument_reg(widen_loaded_value(cdg, b.reg, b.size, XMM_SIZE),
                            get_type_robust(XMM_SIZE, false, e.b_double));
        ic.set_return_reg(d, get_type_robust(XMM_SIZE, false, false));
        ic.emit();
        clear_upper(cdg, d);
        return MERR_OK;
    }

    return MERR_INSN;
}

#endif // IDA_SDK_VERSION >= 750
