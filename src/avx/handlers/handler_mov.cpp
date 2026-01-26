/*
AVX Move Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_vmov_ss_sd(codegen_t &cdg, int data_size) {
    bool is_double = (data_size == DOUBLE_SIZE);
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, data_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
        if (!is_xmm_reg(cdg.insn.Op1) || !is_xmm_reg(cdg.insn.Op2) || cdg.insn.Op3.type == o_void) {
            return MERR_INSN;
        }

        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        mreg_t l = reg2mreg(cdg.insn.Op2.reg);

        mreg_t r;
        mreg_t t_mem = mr_none;
        if (is_mem_op(cdg.insn.Op3)) {
            AvxOpLoader r_in(cdg, 2, cdg.insn.Op3);
            t_mem = cdg.mba->alloc_kreg(XMM_SIZE);
            mop_t src(r_in.reg, data_size);
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

        qstring base_name;
        base_name.cat_sprnt("_mm_move_%s", is_double ? "sd" : "ss");
        qstring iname = make_masked_intrinsic_name(base_name.c_str(), mask);

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t vt = get_type_robust(XMM_SIZE, false, is_double);

        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, vt);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        icall.add_argument_reg(l, vt);
        icall.add_argument_reg(r, vt);
        icall.set_return_reg(d, vt);
        icall.emit();

        if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
        clear_upper(cdg, d, data_size);
        return MERR_OK;
    }

    if (cdg.insn.Op3.type == o_void) {
        if (is_xmm_reg(cdg.insn.Op1)) {
            QASSERT(0xA0300, is_mem_op(cdg.insn.Op2));

            mreg_t xmm_reg = reg2mreg(cdg.insn.Op1.reg);

            AvxOpLoader src_loader(cdg, 1, cdg.insn.Op2);
            mreg_t src_reg = src_loader.reg;

            // Move the loaded float value to the lower part of XMM
            minsn_t *mov_insn = cdg.emit(m_mov, data_size, src_reg, 0, xmm_reg, 0);
            mov_insn->set_fpinsn();  // Mark as FP operation

            // For scalar moves, clear upper bits of XMM and YMM
            // This is done by IDA's builtin SSE handling when we return MERR_INSN
            // to fall back to default processing after initial placement
            return MERR_OK;
        } else {
            QASSERT(0xA0301, is_mem_op(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));
            minsn_t *out = nullptr;
            if (store_operand_hack(cdg, 0, mop_t(reg2mreg(cdg.insn.Op2.reg), data_size), 0, &out)) {
                out->set_fpinsn();
                return MERR_OK;
            }
        }
        return MERR_INSN;
    }

    QASSERT(0xA0302, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2) && is_xmm_reg(cdg.insn.Op3));
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);
    cdg.emit(m_mov, XMM_SIZE, reg2mreg(cdg.insn.Op2.reg), 0, t, 0);
    cdg.emit(m_f2f, data_size, reg2mreg(cdg.insn.Op3.reg), 0, t, 0);
    cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);
    cdg.mba->free_kreg(t, XMM_SIZE);
    clear_upper(cdg, d, data_size);
    return MERR_OK;
}

merror_t handle_vmov_sh(codegen_t &cdg) {
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, WORD_SIZE);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
        if (!is_xmm_reg(cdg.insn.Op1) || !is_xmm_reg(cdg.insn.Op2) || cdg.insn.Op3.type == o_void) {
            return MERR_INSN;
        }

        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        mreg_t l = reg2mreg(cdg.insn.Op2.reg);

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

        qstring base_name("_mm_move_sh");
        qstring iname = make_masked_intrinsic_name(base_name.c_str(), mask);

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t vt = get_type_robust(XMM_SIZE, false, false);

        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, vt);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        icall.add_argument_reg(l, vt);
        icall.add_argument_reg(r, vt);
        icall.set_return_reg(d, vt);
        icall.emit();

        if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
        clear_upper(cdg, d);
        return MERR_OK;
    }

    if (cdg.insn.Op3.type == o_void) {
        if (is_xmm_reg(cdg.insn.Op1)) {
            QASSERT(0xA0304, is_mem_op(cdg.insn.Op2));

            mreg_t xmm_reg = reg2mreg(cdg.insn.Op1.reg);

            AvxOpLoader src_loader(cdg, 1, cdg.insn.Op2);
            mreg_t src_reg = src_loader.reg;

            minsn_t *mov_insn = cdg.emit(m_mov, WORD_SIZE, src_reg, 0, xmm_reg, 0);
            mov_insn->set_fpinsn();
            return MERR_OK;
        } else {
            QASSERT(0xA0305, is_mem_op(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));
            minsn_t *out = nullptr;
            if (store_operand_hack(cdg, 0, mop_t(reg2mreg(cdg.insn.Op2.reg), WORD_SIZE), 0, &out)) {
                out->set_fpinsn();
                return MERR_OK;
            }
        }
        return MERR_INSN;
    }

    QASSERT(0xA0306, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);

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

    AVXIntrinsic icall(&cdg, "_mm_move_sh");
    tinfo_t vt = get_type_robust(XMM_SIZE, false, false);

    icall.add_argument_reg(l, vt);
    icall.add_argument_reg(r, vt);
    icall.set_return_reg(d, vt);
    icall.emit();

    if (t_mem != mr_none) cdg.mba->free_kreg(t_mem, XMM_SIZE);
    clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vmovw(codegen_t &cdg) {
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, WORD_SIZE);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    if (is_xmm_reg(cdg.insn.Op1)) {
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
        mreg_t src;

        if (is_mem_op(cdg.insn.Op2)) {
            AvxOpLoader src_loader(cdg, 1, cdg.insn.Op2);
            src = src_loader.reg;
        } else {
            src = reg2mreg(cdg.insn.Op2.reg);
        }

        qstring base_name("_mm_cvtsi16_si128");
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t src_type(BT_INT16);
        tinfo_t dst_type = get_type_robust(XMM_SIZE, true, false);

        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(dst, dst_type);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }

        icall.add_argument_reg(src, src_type);
        icall.set_return_reg(dst, dst_type);
        icall.emit();

        clear_upper(cdg, dst);
        return MERR_OK;
    }

    if (!is_xmm_reg(cdg.insn.Op2)) {
        return MERR_INSN;
    }

    mreg_t src = reg2mreg(cdg.insn.Op2.reg);
    if (is_mem_op(cdg.insn.Op1)) {
        if (!store_operand_hack(cdg, 0, mop_t(src, WORD_SIZE))) {
            return MERR_INSN;
        }
        return MERR_OK;
    }

    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    mop_t src_mop(src, WORD_SIZE);
    mop_t dst_mop(dst, WORD_SIZE);
    mop_t r;
    cdg.emit(m_mov, &src_mop, &r, &dst_mop);
    return MERR_OK;
}

merror_t handle_vmov(codegen_t &cdg, int data_size) {
    if (is_xmm_reg(cdg.insn.Op1)) {
        mreg_t xmm_reg = reg2mreg(cdg.insn.Op1.reg);

        AvxOpLoader l_loader(cdg, 1, cdg.insn.Op2);
        mreg_t l = l_loader.reg;

        // In 32-bit mode, don't extend to YMM (causes INTERR 50757)
        // Just zero-extend to XMM size
        if (!inf_is_64bit()) {
            mreg_t tmp = cdg.mba->alloc_kreg(data_size);
            cdg.emit(m_mov, data_size, l, 0, tmp, 0);

            mop_t src(tmp, data_size);
            mop_t dst(xmm_reg, XMM_SIZE);
            if (XMM_SIZE > 8) {
                dst.set_udt();
            }
            mop_t r;
            cdg.emit(m_xdu, &src, &r, &dst);

            cdg.mba->free_kreg(tmp, data_size);
            return MERR_OK;
        }

        // 64-bit mode: zero-extend to YMM
        mreg_t ymm_reg = get_ymm_mreg(xmm_reg);
        if (ymm_reg == mr_none) return MERR_INSN;

        mreg_t tmp = cdg.mba->alloc_kreg(data_size);
        cdg.emit(m_mov, data_size, l, 0, tmp, 0);

        mop_t src(tmp, data_size);
        mop_t dst(ymm_reg, YMM_SIZE);
        if (YMM_SIZE > 8) {
            dst.set_udt();
        }
        mop_t r;
        cdg.emit(m_xdu, &src, &r, &dst);

        cdg.mba->free_kreg(tmp, data_size);
        return MERR_OK;
    }

    QASSERT(0xA0303, is_xmm_reg(cdg.insn.Op2));
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);

    if (is_mem_op(cdg.insn.Op1)) {
        store_operand_hack(cdg, 0, mop_t(l, data_size));
    } else {
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        mop_t src(l, data_size);
        mop_t dst(d, data_size);
        mop_t r;
        cdg.emit(m_mov, &src, &r, &dst);
    }
    return MERR_OK;
}

merror_t handle_v_mov_ps_dq(codegen_t &cdg) {
    // Determine operand sizes
    int size;
    bool is_int = (cdg.insn.itype == NN_vmovdqa || cdg.insn.itype == NN_vmovdqu ||
                   cdg.insn.itype == NN_vmovdqa32 || cdg.insn.itype == NN_vmovdqa64 ||
                   cdg.insn.itype == NN_vmovdqu8 || cdg.insn.itype == NN_vmovdqu16 ||
                   cdg.insn.itype == NN_vmovdqu32 || cdg.insn.itype == NN_vmovdqu64);
    bool is_double = (cdg.insn.itype == NN_vmovapd || cdg.insn.itype == NN_vmovupd);

    int elem_size = is_double ? 8 : 4;
    if (is_int) {
        switch (cdg.insn.itype) {
            case NN_vmovdqa64:
            case NN_vmovdqu64:
                elem_size = 8;
                break;
            case NN_vmovdqu8:
                elem_size = 1;
                break;
            case NN_vmovdqu16:
                elem_size = 2;
                break;
            case NN_vmovdqa32:
            case NN_vmovdqu32:
            default:
                elem_size = 4;
                break;
        }
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    if (mask.has_mask) {
        // Masked move handling using load/store/mov intrinsics
        int vec_size = is_vector_reg(cdg.insn.Op1) ? get_vector_size(cdg.insn.Op1)
                                                   : get_vector_size(cdg.insn.Op2);
        tinfo_t vec_type = get_type_robust(vec_size, is_int, is_double);
        tinfo_t ptr_type;
        ptr_type.create_ptr(tinfo_t(BT_VOID));
        const char *prefix = get_size_prefix(vec_size);
        const char *mask_suffix = mask.is_zeroing ? "maskz" : "mask";

        auto build_mask_name = [&](const char *base) {
            qstring name;
            name.cat_sprnt(base, prefix, mask_suffix, elem_size * 8);
            return name;
        };

        if (is_vector_reg(cdg.insn.Op1)) {
            // DEST is register
            size = get_vector_size(cdg.insn.Op1);
            mreg_t dst = reg2mreg(cdg.insn.Op1.reg);

            if (is_vector_reg(cdg.insn.Op2)) {
                // Register-to-register masked move
                mreg_t src = reg2mreg(cdg.insn.Op2.reg);
                qstring iname;
                if (is_int) {
                    iname = build_mask_name("_mm%s_%s_mov_epi%d");
                } else {
                    const char *pf = is_double ? "pd" : "ps";
                    iname.cat_sprnt("_mm%s_%s_mov_%s", prefix, mask_suffix, pf);
                }
                AVXIntrinsic icall(&cdg, iname.c_str());
                if (!mask.is_zeroing) {
                    icall.add_argument_reg(dst, vec_type);
                }
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
                icall.add_argument_reg(src, vec_type);
                icall.set_return_reg(dst, vec_type);
                icall.emit();
            } else {
                // Memory-to-register masked load
                QASSERT(0xA0310, is_mem_op(cdg.insn.Op2));
                mreg_t addr = cdg.load_effective_address(1);
                if (addr == mr_none) {
                    return MERR_INSN;
                }
                qstring iname;
                if (is_int) {
                    iname = build_mask_name("_mm%s_%s_loadu_epi%d");
                } else {
                    const char *pf = is_double ? "pd" : "ps";
                    iname.cat_sprnt("_mm%s_%s_loadu_%s", prefix, mask_suffix, pf);
                }
                AVXIntrinsic icall(&cdg, iname.c_str());
                if (!mask.is_zeroing) {
                    icall.add_argument_reg(dst, vec_type);
                }
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
                icall.add_argument_reg(addr, ptr_type);
                icall.set_return_reg(dst, vec_type);
                icall.emit();
            }

            if (size == XMM_SIZE) clear_upper(cdg, dst);
            return MERR_OK;
        }

        // STORE case: masked store to memory
        if (!is_mem_op(cdg.insn.Op1) || !is_vector_reg(cdg.insn.Op2)) {
            return MERR_INSN;
        }

        size = get_vector_size(cdg.insn.Op2);
        mreg_t src = reg2mreg(cdg.insn.Op2.reg);
        mreg_t addr = cdg.load_effective_address(0);
        if (addr == mr_none) {
            return MERR_INSN;
        }

        qstring iname;
        if (is_int) {
            iname.cat_sprnt("_mm%s_mask_storeu_epi%d", prefix, elem_size * 8);
        } else {
            const char *pf = is_double ? "pd" : "ps";
            iname.cat_sprnt("_mm%s_mask_storeu_%s", prefix, pf);
        }
        AVXIntrinsic icall(&cdg, iname.c_str());
        icall.add_argument_reg(addr, ptr_type);
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        icall.add_argument_reg(src, vec_type);
        icall.emit_void();
        return MERR_OK;
    }

    if (is_vector_reg(cdg.insn.Op1)) {
        // LOAD case: vmovaps reg, mem/reg
        size = get_vector_size(cdg.insn.Op1);
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);

        if (is_vector_reg(cdg.insn.Op2)) {
            // Register-to-register move - use native m_mov with UDT for all sizes
            mreg_t src = reg2mreg(cdg.insn.Op2.reg);

            mop_t src_mop(src, size);
            mop_t dst_mop(dst, size);

            if (size > 8) {
                src_mop.set_udt();
                dst_mop.set_udt();
            }

            mop_t dummy;
            cdg.emit(m_mov, &src_mop, &dummy, &dst_mop);
        } else {
            // Memory-to-register load
            QASSERT(0xA0310, is_mem_op(cdg.insn.Op2));

            // Use load_operand_udt which handles ZMM via emit_zmm_load()
            mreg_t loaded = load_operand_udt(cdg, 1, size);
            if (loaded == mr_none) {
                return MERR_INSN;
            }

            mop_t src_mop(loaded, size);
            mop_t dst_mop(dst, size);

            if (size > 8) {
                src_mop.set_udt();
                dst_mop.set_udt();
            }

            mop_t dummy;
            cdg.emit(m_mov, &src_mop, &dummy, &dst_mop);
        }

        if (size == XMM_SIZE) clear_upper(cdg, dst);
        return MERR_OK;
    }

    // STORE case: vmovaps mem, reg
    if (!is_mem_op(cdg.insn.Op1) || !is_vector_reg(cdg.insn.Op2)) {
        return MERR_INSN;
    }

    size = get_vector_size(cdg.insn.Op2);
    mreg_t src = reg2mreg(cdg.insn.Op2.reg);

    // Use store_operand_hack for stores (like other handlers do)
    mop_t src_mop(src, size);
    if (size > 8) {
        src_mop.set_udt();
    }
    if (!store_operand_hack(cdg, 0, src_mop)) {
        return MERR_INSN;
    }

    return MERR_OK;
}

merror_t handle_v_compress(codegen_t &cdg) {
    bool is_int = false;
    bool is_double = false;
    int elem_size = 4;
    const char *suffix = nullptr;

    switch (cdg.insn.itype) {
        case NN_vcompressps: suffix = "ps";
            elem_size = 4;
            break;
        case NN_vcompresspd: suffix = "pd";
            is_double = true;
            elem_size = 8;
            break;
        case NN_vpcompressd: suffix = "epi32";
            is_int = true;
            elem_size = 4;
            break;
        case NN_vpcompressq: suffix = "epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpcompressb: suffix = "epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpcompressw: suffix = "epi16";
            is_int = true;
            elem_size = 2;
            break;
        default:
            return MERR_INSN;
    }

    int size = is_vector_reg(cdg.insn.Op1) ? get_vector_size(cdg.insn.Op1) : get_vector_size(cdg.insn.Op2);
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    mask.num_elements = size / elem_size;
    if (!mask.has_mask) {
        return MERR_INSN;
    }
    load_mask_operand(cdg, mask);

    tinfo_t vec_type = get_type_robust(size, is_int, is_double);

    if (is_vector_reg(cdg.insn.Op1)) {
        // Register destination: compress into register
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
        AvxOpLoader src(cdg, 1, cdg.insn.Op2);

        qstring iname;
        if (mask.is_zeroing) {
            iname.cat_sprnt("_mm%s_maskz_compress_%s", get_size_prefix(size), suffix);
        } else {
            iname.cat_sprnt("_mm%s_mask_compress_%s", get_size_prefix(size), suffix);
        }

        AVXIntrinsic icall(&cdg, iname.c_str());
        if (!mask.is_zeroing) {
            icall.add_argument_reg(dst, vec_type);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        icall.add_argument_reg(src, vec_type);
        icall.set_return_reg(dst, vec_type);
        icall.emit();

        if (size == XMM_SIZE) clear_upper(cdg, dst);
        return MERR_OK;
    }

    // Memory destination: compress store
    if (!is_mem_op(cdg.insn.Op1) || !is_vector_reg(cdg.insn.Op2)) {
        return MERR_INSN;
    }

    mreg_t src = reg2mreg(cdg.insn.Op2.reg);
    mreg_t addr = cdg.load_effective_address(0);
    if (addr == mr_none) {
        return MERR_INSN;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_mask_compressstoreu_%s", get_size_prefix(size), suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ptr_type;
    ptr_type.create_ptr(tinfo_t(BT_VOID));
    icall.add_argument_reg(addr, ptr_type);
    icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    icall.add_argument_reg(src, vec_type);
    icall.emit_void();

    return MERR_OK;
}

merror_t handle_v_expand(codegen_t &cdg) {
    bool is_int = false;
    bool is_double = false;
    int elem_size = 4;
    const char *suffix = nullptr;

    switch (cdg.insn.itype) {
        case NN_vexpandps: suffix = "ps";
            elem_size = 4;
            break;
        case NN_vexpandpd: suffix = "pd";
            is_double = true;
            elem_size = 8;
            break;
        case NN_vpexpandd: suffix = "epi32";
            is_int = true;
            elem_size = 4;
            break;
        case NN_vpexpandq: suffix = "epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpexpandb: suffix = "epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpexpandw: suffix = "epi16";
            is_int = true;
            elem_size = 2;
            break;
        default:
            return MERR_INSN;
    }

    int size = get_vector_size(cdg.insn.Op1);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    mask.num_elements = size / elem_size;
    if (!mask.has_mask) {
        return MERR_INSN;
    }
    load_mask_operand(cdg, mask);

    tinfo_t vec_type = get_type_robust(size, is_int, is_double);

    if (is_mem_op(cdg.insn.Op2)) {
        // Memory source: expand load
        mreg_t addr = cdg.load_effective_address(1);
        if (addr == mr_none) {
            return MERR_INSN;
        }

        qstring iname;
        if (mask.is_zeroing) {
            iname.cat_sprnt("_mm%s_maskz_expandloadu_%s", get_size_prefix(size), suffix);
        } else {
            iname.cat_sprnt("_mm%s_mask_expandloadu_%s", get_size_prefix(size), suffix);
        }

        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ptr_type;
        ptr_type.create_ptr(tinfo_t(BT_VOID));

        if (!mask.is_zeroing) {
            icall.add_argument_reg(dst, vec_type);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        icall.add_argument_reg(addr, ptr_type);
        icall.set_return_reg(dst, vec_type);
        icall.emit();
    } else if (is_vector_reg(cdg.insn.Op2)) {
        // Register source: expand from register
        mreg_t src = reg2mreg(cdg.insn.Op2.reg);
        qstring iname;
        if (mask.is_zeroing) {
            iname.cat_sprnt("_mm%s_maskz_expand_%s", get_size_prefix(size), suffix);
        } else {
            iname.cat_sprnt("_mm%s_mask_expand_%s", get_size_prefix(size), suffix);
        }

        AVXIntrinsic icall(&cdg, iname.c_str());
        if (!mask.is_zeroing) {
            icall.add_argument_reg(dst, vec_type);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        icall.add_argument_reg(src, vec_type);
        icall.set_return_reg(dst, vec_type);
        icall.emit();
    } else {
        return MERR_INSN;
    }

    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

merror_t handle_v_gather(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    const op_t &mem = cdg.insn.Op2;
    mreg_t base = reg2mreg(mem.reg);

    // sib_index returns the full VSIB register number
    mreg_t index_vec = reg2mreg(sib_index(cdg.insn, mem));

    // sib_scale requires only op_t
    int scale = 1 << sib_scale(mem);
    ea_t disp = mem.addr;

    const char *suffix = nullptr;
    bool is_int = false;
    bool is_double = false;
    bool use_i64_index = false;
    int data_elem_size = 4;
    switch (cdg.insn.itype) {
        case NN_vgatherdps: suffix = "ps";
            data_elem_size = 4;
            break;
        case NN_vgatherdpd: suffix = "pd";
            is_double = true;
            data_elem_size = 8;
            break;
        case NN_vgatherqps: suffix = "ps";
            use_i64_index = true;
            data_elem_size = 4;
            break;
        case NN_vgatherqpd: suffix = "pd";
            is_double = true;
            use_i64_index = true;
            data_elem_size = 8;
            break;
        case NN_vpgatherdd: suffix = "epi32";
            is_int = true;
            data_elem_size = 4;
            break;
        case NN_vpgatherdq: suffix = "epi64";
            is_int = true;
            data_elem_size = 8;
            break;
        case NN_vpgatherqd: suffix = "epi32";
            is_int = true;
            use_i64_index = true;
            data_elem_size = 4;
            break;
        case NN_vpgatherqq: suffix = "epi64";
            is_int = true;
            use_i64_index = true;
            data_elem_size = 8;
            break;
    }

    bool has_kmask = has_opmask(cdg.insn);
    MaskInfo mask_info;
    if (has_kmask) {
        mask_info = MaskInfo::from_insn(cdg.insn, data_elem_size);
        mask_info.num_elements = size / data_elem_size;
        load_mask_operand(cdg, mask_info);
    }

    int prefix_size = size;
    if (use_i64_index && data_elem_size == 4) {
        prefix_size = size * 2;
        if (prefix_size > ZMM_SIZE) prefix_size = ZMM_SIZE;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_mask_i%sgather_%s", get_size_prefix(prefix_size),
                    use_i64_index ? "64" : "32", suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_dst = get_type_robust(size, is_int, is_double);
    int index_size = size;
    if (use_i64_index && data_elem_size == 4) {
        index_size = size * 2;
        if (index_size > ZMM_SIZE) index_size = ZMM_SIZE;
    }
    tinfo_t ti_idx = get_type_robust(index_size, true, false);

    mreg_t arg_base = base;
    mreg_t t_base = mr_none;

    if (disp != 0) {
        t_base = cdg.mba->alloc_kreg(8);
        mop_t l(base, 8);
        mop_t r;
        r.make_number(disp, 8);
        mop_t d(t_base, 8);
        cdg.emit(m_add, &l, &r, &d);
        arg_base = t_base;
    }

    // Create void* pointer type for base address argument
    tinfo_t ptr_type;
    ptr_type.create_ptr(tinfo_t(BT_VOID));

    if (has_kmask) {
        mreg_t src_merge = dst;
        mreg_t zero = mr_none;
        if (mask_info.is_zeroing) {
            zero = cdg.mba->alloc_kreg(size);
            const char *zero_name = nullptr;
            if (is_int) {
                zero_name = (size == ZMM_SIZE) ? "_mm512_setzero_si512"
                           : (size == YMM_SIZE) ? "_mm256_setzero_si256"
                           : "_mm_setzero_si128";
            } else {
                if (is_double) {
                    zero_name = (size == ZMM_SIZE) ? "_mm512_setzero_pd"
                               : (size == YMM_SIZE) ? "_mm256_setzero_pd"
                               : "_mm_setzero_pd";
                } else {
                    zero_name = (size == ZMM_SIZE) ? "_mm512_setzero_ps"
                               : (size == YMM_SIZE) ? "_mm256_setzero_ps"
                               : "_mm_setzero_ps";
                }
            }
            AVXIntrinsic setz(&cdg, zero_name);
            setz.set_return_reg(zero, ti_dst);
            setz.emit();
            src_merge = zero;
        }

        icall.add_argument_reg(src_merge, ti_dst);
        icall.add_argument_mask(mask_info.mask_reg, mask_info.num_elements);
        icall.add_argument_reg(index_vec, ti_idx);
        icall.add_argument_reg(arg_base, ptr_type);
        icall.add_argument_imm(scale, BT_INT32);

        if (zero != mr_none) cdg.mba->free_kreg(zero, size);
    } else {
        mreg_t mask_vec = reg2mreg(cdg.insn.Op3.reg);
        icall.add_argument_reg(dst, ti_dst);
        icall.add_argument_reg(arg_base, ptr_type);
        icall.add_argument_reg(index_vec, ti_idx);
        icall.add_argument_reg(mask_vec, ti_dst);
        icall.add_argument_imm(scale, BT_INT32);
    }

    icall.set_return_reg(dst, ti_dst);
    icall.emit();

    if (t_base != mr_none) {
        cdg.mba->free_kreg(t_base, 8);
    }

    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

merror_t handle_v_scatter(codegen_t &cdg) {
    const op_t &mem = cdg.insn.Op1;
    if (!is_mem_op(mem) || !is_vector_reg(cdg.insn.Op2)) {
        return MERR_INSN;
    }

    int size = get_vector_size(cdg.insn.Op2);
    mreg_t src = reg2mreg(cdg.insn.Op2.reg);
    mreg_t base = reg2mreg(mem.reg);

    mreg_t index_vec = reg2mreg(sib_index(cdg.insn, mem));
    int scale = 1 << sib_scale(mem);
    ea_t disp = mem.addr;

    const char *suffix = nullptr;
    bool is_int = false;
    bool is_double = false;
    bool use_i64_index = false;
    int elem_size = 4;

    switch (cdg.insn.itype) {
        case NN_vscatterdps: suffix = "ps";
            elem_size = 4;
            break;
        case NN_vscatterdpd: suffix = "pd";
            is_double = true;
            elem_size = 8;
            break;
        case NN_vscatterqps: suffix = "ps";
            use_i64_index = true;
            elem_size = 4;
            break;
        case NN_vscatterqpd: suffix = "pd";
            is_double = true;
            use_i64_index = true;
            elem_size = 8;
            break;
        case NN_vpscatterdd: suffix = "epi32";
            is_int = true;
            elem_size = 4;
            break;
        case NN_vpscatterdq: suffix = "epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpscatterqd: suffix = "epi32";
            is_int = true;
            use_i64_index = true;
            elem_size = 4;
            break;
        case NN_vpscatterqq: suffix = "epi64";
            is_int = true;
            use_i64_index = true;
            elem_size = 8;
            break;
        default:
            return MERR_INSN;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    mask.num_elements = size / elem_size;
    if (!mask.has_mask) {
        return MERR_INSN;
    }
    load_mask_operand(cdg, mask);

    mreg_t arg_base = base;
    mreg_t t_base = mr_none;
    if (disp != 0) {
        t_base = cdg.mba->alloc_kreg(8);
        mop_t l(base, 8);
        mop_t r;
        r.make_number(disp, 8);
        mop_t d(t_base, 8);
        cdg.emit(m_add, &l, &r, &d);
        arg_base = t_base;
    }

    int prefix_size = size;
    if (use_i64_index && elem_size == 4) {
        prefix_size = size * 2;
        if (prefix_size > ZMM_SIZE) prefix_size = ZMM_SIZE;
    }


    qstring iname;
    iname.cat_sprnt("_mm%s_mask_i%sscatter_%s", get_size_prefix(prefix_size),
                    use_i64_index ? "64" : "32", suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_val = get_type_robust(size, is_int, is_double);
    int index_size = size;
    if (use_i64_index && elem_size == 4) {
        index_size = size * 2;
        if (index_size > ZMM_SIZE) index_size = ZMM_SIZE;
    }
    tinfo_t ti_idx = get_type_robust(index_size, true, false);
    tinfo_t ptr_type;
    ptr_type.create_ptr(tinfo_t(BT_VOID));

    icall.add_argument_reg(arg_base, ptr_type);
    icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    icall.add_argument_reg(index_vec, ti_idx);
    icall.add_argument_reg(src, ti_val);
    icall.add_argument_imm(scale, BT_INT32);
    icall.emit_void();

    if (t_base != mr_none) {
        cdg.mba->free_kreg(t_base, 8);
    }

    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
