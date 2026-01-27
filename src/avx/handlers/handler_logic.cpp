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
                   cdg.insn.itype == NN_vpandn || cdg.insn.itype == NN_vpandd || cdg.insn.itype == NN_vpandq ||
                   cdg.insn.itype == NN_vpandnd || cdg.insn.itype == NN_vpandnq || cdg.insn.itype == NN_vpord ||
                   cdg.insn.itype == NN_vporq || cdg.insn.itype == NN_vpxord || cdg.insn.itype == NN_vpxorq);
    bool is_double = (cdg.insn.itype == NN_vandpd || cdg.insn.itype == NN_vorpd || cdg.insn.itype == NN_vxorpd ||
                      cdg.insn.itype == NN_vandnpd);

    const char *opname = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpand:
        case NN_vpandd:
        case NN_vpandq:
        case NN_vandps:
        case NN_vandpd: opname = "and";
            break;
        case NN_vpandn:
        case NN_vpandnd:
        case NN_vpandnq:
        case NN_vandnps:
        case NN_vandnpd: opname = "andnot";
            break;
        case NN_vpor:
        case NN_vpord:
        case NN_vporq:
        case NN_vorps:
        case NN_vorpd: opname = "or";
            break;
        case NN_vpxor:
        case NN_vpxord:
        case NN_vpxorq:
        case NN_vxorps:
        case NN_vxorpd: opname = "xor";
            break;
        default: QASSERT(0xA0401, false);
    }

    int elem_size = 4;
    if (is_double) {
        elem_size = 8;
    } else if (is_int) {
        switch (cdg.insn.itype) {
            case NN_vpandq:
            case NN_vpandnq:
            case NN_vporq:
            case NN_vpxorq:
                elem_size = 8;
                break;
            case NN_vpandd:
            case NN_vpandnd:
            case NN_vpord:
            case NN_vpxord:
                elem_size = 4;
                break;
            default:
                elem_size = 4;
                break;
        }
    }

    const char *prefix = get_size_prefix(size);
    qstring base_name;
    if (is_int) {
        base_name.cat_sprnt("_mm%s_%s_si%d", prefix, opname, get_vector_bits(size));
    } else {
        const char *pf = is_double ? "pd" : "ps";
        base_name.cat_sprnt("_mm%s_%s_%s", prefix, opname, pf);
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vti = get_type_robust(size, is_int, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, vti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

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
        case NN_vpsraq: op = "sra";
            bits = 64;
            break;
    }

    int elem_size = bits / 8;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    if (cdg.insn.Op3.type == o_imm) {
        // Immediate shift: _mm256_slli_epi16
        qstring base_name;
        base_name.cat_sprnt("_mm%s_%si_epi%d", get_size_prefix(size), op, bits);
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }
        icall.add_argument_reg(s, ti);
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else {
        // Register/Mem shift: _mm256_sll_epi16
        // Count is always 128-bit (XMM or m128)
        AvxOpLoader count(cdg, 2, cdg.insn.Op3);
        qstring base_name;
        base_name.cat_sprnt("_mm%s_%s_epi%d", get_size_prefix(size), op, bits);
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti_vec = get_type_robust(size, true, false);
        tinfo_t ti_count = get_type_robust(XMM_SIZE, true, false); // Count is always __m128i

        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti_vec);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }

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
        case NN_vpsllvw: op = "sllv";
            bits = 16;
            break;
        case NN_vpsllvd: op = "sllv";
            bits = 32;
            break;
        case NN_vpsllvq: op = "sllv";
            bits = 64;
            break;
        case NN_vpsrlvw: op = "srlv";
            bits = 16;
            break;
        case NN_vpsrlvd: op = "srlv";
            bits = 32;
            break;
        case NN_vpsrlvq: op = "srlv";
            bits = 64;
            break;
        case NN_vpsravw: op = "srav";
            bits = 16;
            break;
        case NN_vpsravd: op = "srav";
            bits = 32;
            break;
        case NN_vpsravq: op = "srav";
            bits = 64;
            break;
    }

    int elem_size = bits / 8;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_epi%d", get_size_prefix(size), op, bits);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(s, ti);
    icall.add_argument_reg(c, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_rotate(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);

    QASSERT(0xA0402, cdg.insn.Op3.type == o_imm);
    uint64 imm = cdg.insn.Op3.value;

    bool is_qword = (cdg.insn.itype == NN_vprolq || cdg.insn.itype == NN_vprorq);
    bool is_left = (cdg.insn.itype == NN_vprold || cdg.insn.itype == NN_vprolq);
    int bits = is_qword ? 64 : 32;
    int elem_size = bits / 8;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_epi%d", get_size_prefix(size), is_left ? "rol" : "ror", bits);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(s, ti);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_var_rotate(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader c(cdg, 2, cdg.insn.Op3);

    bool is_qword = (cdg.insn.itype == NN_vprolvq || cdg.insn.itype == NN_vprorvq);
    bool is_left = (cdg.insn.itype == NN_vprolvd || cdg.insn.itype == NN_vprolvq);
    int bits = is_qword ? 64 : 32;
    int elem_size = bits / 8;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%sv_epi%d", get_size_prefix(size), is_left ? "rol" : "ror", bits);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(s, ti);
    icall.add_argument_reg(c, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_shift_double(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    uint16 it = cdg.insn.itype;
    bool is_left = (it == NN_vpshldw || it == NN_vpshldd || it == NN_vpshldq ||
                    it == NN_vpshldvw || it == NN_vpshldvd || it == NN_vpshldvq);
    bool is_var = (it == NN_vpshldvw || it == NN_vpshldvd || it == NN_vpshldvq ||
                   it == NN_vpshrdvw || it == NN_vpshrdvd || it == NN_vpshrdvq);

    int bits = 0;
    switch (it) {
        case NN_vpshldw:
        case NN_vpshldvw:
        case NN_vpshrdw:
        case NN_vpshrdvw:
            bits = 16;
            break;
        case NN_vpshldd:
        case NN_vpshldvd:
        case NN_vpshrdd:
        case NN_vpshrdvd:
            bits = 32;
            break;
        case NN_vpshldq:
        case NN_vpshldvq:
        case NN_vpshrdq:
        case NN_vpshrdvq:
            bits = 64;
            break;
        default:
            return MERR_INSN;
    }

    int elem_size = bits / 8;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s%s_epi%d", get_size_prefix(size),
                        is_left ? "shld" : "shrd", is_var ? "v" : "i", bits);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (is_var) {
        mreg_t a = reg2mreg(cdg.insn.Op1.reg);
        AvxOpLoader b(cdg, 1, cdg.insn.Op2);
        AvxOpLoader c(cdg, 2, cdg.insn.Op3);

        if (mask.has_mask) {
            if (mask.is_zeroing) {
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
                icall.add_argument_reg(a, ti);
            } else {
                icall.add_argument_reg(a, ti);
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            }
        } else {
            icall.add_argument_reg(a, ti);
        }

        icall.add_argument_reg(b, ti);
        icall.add_argument_reg(c, ti);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else {
        AvxOpLoader a(cdg, 1, cdg.insn.Op2);
        AvxOpLoader b(cdg, 2, cdg.insn.Op3);
        QASSERT(0xA0410, cdg.insn.Op4.type == o_imm);

        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }

        icall.add_argument_reg(a, ti);
        icall.add_argument_reg(b, ti);
        icall.add_argument_imm(cdg.insn.Op4.value, BT_INT8);
        icall.set_return_reg(d, ti);
        icall.emit();
    }

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_multishift(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader a(cdg, 1, cdg.insn.Op2);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 1);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_multishift_epi64_epi8", get_size_prefix(size));
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
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

    int elem_size = 4;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_shuffle_ps", get_size_prefix(size));
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

    int elem_size = 8;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_shuffle_pd", get_size_prefix(size));
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, true);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

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
    int elem_size = 4;

    switch (cdg.insn.itype) {
        case NN_vpshufb: op = "shuffle";
            suffix = "epi8";
            has_imm = false;
            elem_size = 1;
            break;
        case NN_vpshufd: op = "shuffle";
            suffix = "epi32";
            has_imm = true;
            elem_size = 4;
            break;
        case NN_vpshufhw: op = "shufflehi";
            suffix = "epi16";
            has_imm = true;
            elem_size = 2;
            break;
        case NN_vpshuflw: op = "shufflelo";
            suffix = "epi16";
            has_imm = true;
            elem_size = 2;
            break;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s_%s", get_size_prefix(size), op, suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

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

    int elem_size = 8;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_permute4x64_pd", get_size_prefix(size));
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, false, true);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(r, ti);
    icall.add_argument_imm(imm8, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    return MERR_OK;
}

merror_t handle_v_perm_int(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    int elem_size = 4;
    if (cdg.insn.itype == NN_vpermq) {
        elem_size = 8;
    } else if (cdg.insn.itype == NN_vpermd) {
        elem_size = 4;
    } else if (cdg.insn.itype == NN_vpermilpd) {
        elem_size = 8;
    } else if (cdg.insn.itype == NN_vpermilps) {
        elem_size = 4;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    if (cdg.insn.itype == NN_vpermq) {
        // vpermq ymm/zmm, ymm/zmm, imm8
        // vpermq ymm/zmm, ymm/zmm, ymm/zmm (permutexvar_epi64)
        AvxOpLoader s(cdg, 1, cdg.insn.Op2);

        if (size != YMM_SIZE && size != ZMM_SIZE) {
            return MERR_INSN;
        }

        tinfo_t ti = get_type_robust(size, true, false);

        if (cdg.insn.Op3.type == o_imm) {
            qstring base_name;
            base_name.cat_sprnt("_mm%s_permutex_epi64", get_size_prefix(size));
            qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
            AVXIntrinsic icall(&cdg, iname.c_str());
            if (mask.has_mask) {
                if (!mask.is_zeroing) {
                    icall.add_argument_reg(d, ti);
                }
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            }
            icall.add_argument_reg(s, ti);
            icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
            icall.set_return_reg(d, ti);
            icall.emit();
        } else {
            // Variable permute: _mm512_permutexvar_epi64(idx, a)
            AvxOpLoader ctrl(cdg, 2, cdg.insn.Op3);
            qstring base_name;
            base_name.cat_sprnt("_mm%s_permutexvar_epi64", get_size_prefix(size));
            qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
            AVXIntrinsic icall(&cdg, iname.c_str());
            if (mask.has_mask) {
                if (!mask.is_zeroing) {
                    icall.add_argument_reg(d, ti);
                }
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            }
            icall.add_argument_reg(ctrl, ti);
            icall.add_argument_reg(s, ti);
            icall.set_return_reg(d, ti);
            icall.emit();
        }
    } else if (cdg.insn.itype == NN_vpermd) {
        // vpermd ymm/zmm, ymm/zmm, ymm/zmm
        // _mm256/_mm512_permutexvar_epi32(src, idx)
        // Instruction: vpermd dest, idx, src
        mreg_t idx = reg2mreg(cdg.insn.Op2.reg);
        AvxOpLoader src(cdg, 2, cdg.insn.Op3);

        if (size != YMM_SIZE && size != ZMM_SIZE) {
            return MERR_INSN;
        }

        qstring base_name;
        base_name.cat_sprnt("_mm%s_permutexvar_epi32", get_size_prefix(size));
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }
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
            qstring base_name;
            base_name.cat_sprnt("_mm%s_permute_%s", get_size_prefix(size), is_double ? "pd" : "ps");
            qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
            AVXIntrinsic icall(&cdg, iname.c_str());
            tinfo_t ti = get_type_robust(size, false, is_double);
            if (mask.has_mask) {
                if (!mask.is_zeroing) {
                    icall.add_argument_reg(d, ti);
                }
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            }
            icall.add_argument_reg(s, ti);
            icall.add_argument_imm(cdg.insn.Op3.value, BT_INT32);
            icall.set_return_reg(d, ti);
            icall.emit();
        } else {
            // Variable form: _mm_permutevar_ps/_mm256_permutevar_ps
            AvxOpLoader ctrl(cdg, 2, cdg.insn.Op3);
            qstring base_name;
            base_name.cat_sprnt("_mm%s_permutevar_%s", get_size_prefix(size), is_double ? "pd" : "ps");
            qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
            AVXIntrinsic icall(&cdg, iname.c_str());
            tinfo_t ti = get_type_robust(size, false, is_double);
            tinfo_t ti_ctrl = get_type_robust(size, true, false); // Control is integer
            if (mask.has_mask) {
                if (!mask.is_zeroing) {
                    icall.add_argument_reg(d, ti);
                }
                icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            }
            icall.add_argument_reg(s, ti);
            icall.add_argument_reg(ctrl, ti_ctrl);
            icall.set_return_reg(d, ti);
            icall.emit();
        }
        if (size == XMM_SIZE) clear_upper(cdg, d);
    }
    return MERR_OK;
}

merror_t handle_v_permutex(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t idx = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader src(cdg, 2, cdg.insn.Op3);

    bool is_word = (cdg.insn.itype == NN_vpermw);
    int elem_size = is_word ? 2 : 1;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    const char *suffix = is_word ? "epi16" : "epi8";
    qstring base_name;
    base_name.cat_sprnt("_mm%s_permutexvar_%s", get_size_prefix(size), suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(idx, ti);
    icall.add_argument_reg(src, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_permutex2(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t a = reg2mreg(cdg.insn.Op1.reg);
    mreg_t idx = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    const char *suffix = nullptr;
    bool is_int = false;
    bool is_double = false;
    int elem_size = 4;

    switch (cdg.insn.itype) {
        case NN_vpermt2b: suffix = "epi8";
            is_int = true;
            elem_size = 1;
            break;
        case NN_vpermt2w: suffix = "epi16";
            is_int = true;
            elem_size = 2;
            break;
        case NN_vpermt2d: suffix = "epi32";
            is_int = true;
            elem_size = 4;
            break;
        case NN_vpermt2q: suffix = "epi64";
            is_int = true;
            elem_size = 8;
            break;
        case NN_vpermt2ps: suffix = "ps";
            elem_size = 4;
            break;
        case NN_vpermt2pd: suffix = "pd";
            is_double = true;
            elem_size = 8;
            break;
        default: return MERR_INSN;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_permutex2var_%s", get_size_prefix(size), suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_table = get_type_robust(size, is_int, is_double);
    tinfo_t ti_idx = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (mask.is_zeroing) {
            // maskz: k, a, idx, b
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            icall.add_argument_reg(a, ti_table);
            icall.add_argument_reg(idx, ti_idx);
            icall.add_argument_reg(b, ti_table);
        } else {
            // mask: a, k, idx, b
            icall.add_argument_reg(a, ti_table);
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
            icall.add_argument_reg(idx, ti_idx);
            icall.add_argument_reg(b, ti_table);
        }
    } else {
        icall.add_argument_reg(a, ti_table);
        icall.add_argument_reg(idx, ti_idx);
        icall.add_argument_reg(b, ti_table);
    }

    icall.set_return_reg(d, ti_table);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_ternary_logic(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t a = reg2mreg(cdg.insn.Op1.reg);
    mreg_t b = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader c(cdg, 2, cdg.insn.Op3);
    QASSERT(0xA0608, cdg.insn.Op4.type == o_imm);

    bool is_qword = (cdg.insn.itype == NN_vpternlogq);
    int elem_size = is_qword ? 8 : 4;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_ternarylogic_epi%d", get_size_prefix(size), elem_size * 8);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
    icall.add_argument_reg(c, ti);
    icall.add_argument_imm(cdg.insn.Op4.value, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_conflict(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    bool is_qword = (cdg.insn.itype == NN_vpconflictq);
    int elem_size = is_qword ? 8 : 4;

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_conflict_epi%d", get_size_prefix(size), elem_size * 8);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(src, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_popcnt(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    int elem_size = 1;
    const char *suffix = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpopcntb: elem_size = 1; suffix = "epi8"; break;
        case NN_vpopcntw: elem_size = 2; suffix = "epi16"; break;
        case NN_vpopcntd: elem_size = 4; suffix = "epi32"; break;
        case NN_vpopcntq: elem_size = 8; suffix = "epi64"; break;
        default: return MERR_INSN;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_popcnt_%s", get_size_prefix(size), suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(src, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_lzcnt(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    int elem_size = 0;
    const char *suffix = nullptr;
    switch (cdg.insn.itype) {
        case NN_vplzcntd: elem_size = 4; suffix = "epi32"; break;
        case NN_vplzcntq: elem_size = 8; suffix = "epi64"; break;
        default: return MERR_INSN;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_lzcnt_%s", get_size_prefix(size), suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(src, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_gfni(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t a = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    const char *suffix = nullptr;
    bool has_imm = false;
    switch (cdg.insn.itype) {
        case NN_vgf2p8affineqb:
            suffix = "gf2p8affine_epi64_epi8";
            has_imm = true;
            break;
        case NN_vgf2p8affineinvqb:
            suffix = "gf2p8affineinv_epi64_epi8";
            has_imm = true;
            break;
        case NN_vgf2p8mulb:
            suffix = "gf2p8mul_epi8";
            break;
        default:
            return MERR_INSN;
    }

    if (has_imm) {
        QASSERT(0xA0A30, cdg.insn.Op4.type == o_imm);
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, 1);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_%s", get_size_prefix(size), suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
    if (has_imm) {
        icall.add_argument_imm(cdg.insn.Op4.value, BT_INT8);
    }
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_pclmul(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t a = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    QASSERT(0xA0A31, cdg.insn.Op4.type == o_imm);
    uint64 imm = cdg.insn.Op4.value;

    qstring iname;
    if (size == XMM_SIZE) {
        iname = "_mm_clmulepi64_si128";
    } else {
        iname.cat_sprnt("_mm%s_clmulepi64_epi128", get_size_prefix(size));
    }

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
    icall.add_argument_imm(imm, BT_INT8);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_aes(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t a = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader b(cdg, 2, cdg.insn.Op3);

    const char *op = nullptr;
    switch (cdg.insn.itype) {
        case NN_vaesenc: op = "aesenc"; break;
        case NN_vaesenclast: op = "aesenclast"; break;
        case NN_vaesdec: op = "aesdec"; break;
        case NN_vaesdeclast: op = "aesdeclast"; break;
        default: return MERR_INSN;
    }

    qstring iname;
    if (size == XMM_SIZE) {
        iname.cat_sprnt("_mm_%s_si128", op);
    } else {
        iname.cat_sprnt("_mm%s_%s_epi128", get_size_prefix(size), op);
    }

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_v_sha(codegen_t &cdg) {
    QASSERT(0xA0A32, is_xmm_reg(cdg.insn.Op1));

    uint16 it = cdg.insn.itype;
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t a = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader b(cdg, 1, cdg.insn.Op2);

    const char *iname = nullptr;
    bool has_imm = false;
    bool has_third = false;

    switch (it) {
        case NN_sha1msg1: iname = "_mm_sha1msg1_epu32"; break;
        case NN_sha1msg2: iname = "_mm_sha1msg2_epu32"; break;
        case NN_sha1nexte: iname = "_mm_sha1nexte_epu32"; break;
        case NN_sha1rnds4: iname = "_mm_sha1rnds4_epu32"; has_imm = true; break;
        case NN_sha256msg1: iname = "_mm_sha256msg1_epu32"; break;
        case NN_sha256msg2: iname = "_mm_sha256msg2_epu32"; break;
        case NN_sha256rnds2: iname = "_mm_sha256rnds2_epu32"; has_third = true; break;
        default: return MERR_INSN;
    }

    AVXIntrinsic icall(&cdg, iname);
    tinfo_t ti = get_type_robust(XMM_SIZE, true, false);

    icall.add_argument_reg(a, ti);
    icall.add_argument_reg(b, ti);

    if (has_third) {
        AvxOpLoader c(cdg, 2, cdg.insn.Op3);
        icall.add_argument_reg(c, ti);
    }

    if (has_imm) {
        QASSERT(0xA0A33, cdg.insn.Op3.type == o_imm);
        icall.add_argument_imm(cdg.insn.Op3.value, BT_INT8);
    }

    icall.set_return_reg(d, ti);
    icall.emit();
    return MERR_OK;
}

merror_t handle_cache_ctrl(codegen_t &cdg) {
    const op_t &op = cdg.insn.Op1;
    if (!is_mem_op(op)) {
        return MERR_INSN;
    }

    const char *iname = nullptr;
    switch (cdg.insn.itype) {
        case NN_clflushopt: iname = "_mm_clflushopt"; break;
        case NN_clwb: iname = "_mm_clwb"; break;
        default: return MERR_INSN;
    }

    int addr_size = inf_is_64bit() ? 8 : 4;
    mreg_t addr_reg = mr_none;

    if (op.type == o_mem) {
        addr_reg = cdg.mba->alloc_kreg(addr_size);
        mop_t addr_imm;
        addr_imm.make_number(op.addr, addr_size);
        mop_t addr_dst;
        addr_dst.make_reg(addr_reg, addr_size);
        mop_t empty;
        cdg.emit(m_mov, &addr_imm, &empty, &addr_dst);
    } else {
        addr_reg = cdg.load_effective_address(0);
    }

    if (addr_reg == mr_none) {
        return MERR_INSN;
    }

    tinfo_t ptr_type;
    ptr_type.create_ptr(tinfo_t(BT_VOID));

    AVXIntrinsic icall(&cdg, iname);
    icall.add_argument_reg(addr_reg, ptr_type);
    icall.emit_void();
    return MERR_OK;
}

merror_t handle_v_align(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t s1 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader s2(cdg, 2, cdg.insn.Op3);
    QASSERT(0xA0607, cdg.insn.Op4.type == o_imm);

    int elem_size;
    const char *suffix;
    switch (cdg.insn.itype) {
        case NN_vpalignr:
            elem_size = 1;
            suffix = "epi8";
            break;
        case NN_valignd:
            elem_size = 4;
            suffix = "epi32";
            break;
        case NN_valignq:
            elem_size = 8;
            suffix = "epi64";
            break;
        default:
            return MERR_INSN;
    }
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_alignr_%s", get_size_prefix(size), suffix);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, ti);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

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

merror_t handle_vbroadcast_x4(codegen_t &cdg) {
    QASSERT(0xA0704, is_vector_reg(cdg.insn.Op1));

    int dst_size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    int src_size = XMM_SIZE;
    bool is_int = false;
    bool is_double = false;
    const char *suffix = nullptr;

    switch (cdg.insn.itype) {
        case NN_vbroadcastf32x4:
            suffix = "f32x4";
            src_size = XMM_SIZE;
            break;
        case NN_vbroadcastf64x4:
            suffix = "f64x4";
            src_size = YMM_SIZE;
            is_double = true;
            break;
        case NN_vbroadcasti32x4:
            suffix = "i32x4";
            src_size = XMM_SIZE;
            is_int = true;
            break;
        case NN_vbroadcasti64x4:
            suffix = "i64x4";
            src_size = YMM_SIZE;
            is_int = true;
            break;
        default:
            return MERR_INSN;
    }

    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    qstring iname;
    iname.cat_sprnt("_mm%s_broadcast_%s", get_size_prefix(dst_size), suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t src_type = get_type_robust(src_size, is_int, is_double);
    tinfo_t dst_type = get_type_robust(dst_size, is_int, is_double);

    icall.add_argument_reg(src, src_type);
    icall.set_return_reg(d, dst_type);
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
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

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

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
    qstring base_name;
    base_name.cat_sprnt("_mm%s_cmp_%s", get_size_prefix(size), suf);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, vt);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

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
    int elem_size = 4;

    switch (cdg.insn.itype) {
        case NN_vpcmpeqb: op = "eq";
            type = "epi8";
            elem_size = 1;
            break;
        case NN_vpcmpeqw: op = "eq";
            type = "epi16";
            elem_size = 2;
            break;
        case NN_vpcmpeqd: op = "eq";
            type = "epi32";
            elem_size = 4;
            break;
        case NN_vpcmpeqq: op = "eq";
            type = "epi64";
            elem_size = 8;
            break;
        case NN_vpcmpgtb: op = "gt";
            type = "epi8";
            elem_size = 1;
            break;
        case NN_vpcmpgtw: op = "gt";
            type = "epi16";
            elem_size = 2;
            break;
        case NN_vpcmpgtd: op = "gt";
            type = "epi32";
            elem_size = 4;
            break;
        case NN_vpcmpgtq: op = "gt";
            type = "epi64";
            elem_size = 8;
            break;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_cmp%s_%s", get_size_prefix(size), op, type);
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

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

merror_t handle_vblendv_ps_pd(codegen_t &cdg) {
    int size = get_op_size(cdg.insn);
    bool is_double = (cdg.insn.itype == NN_vblendvpd);

    AvxOpLoader x(cdg, 1, cdg.insn.Op2);
    AvxOpLoader y(cdg, 2, cdg.insn.Op3);
    mreg_t m = reg2mreg(cdg.insn.Op4.reg);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    int elem_size = is_double ? 8 : 4;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_blendv_%s", get_size_prefix(size), is_double ? "pd" : "ps");
    qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t vt = get_type_robust(size, false, is_double);

    if (mask.has_mask) {
        if (!mask.is_zeroing) {
            icall.add_argument_reg(d, vt);
        }
        icall.add_argument_mask(mask.mask_reg, mask.num_elements);
    }

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

    int elem_size = is_double ? 8 : 4;
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    qstring base_name;
    base_name.cat_sprnt("_mm%s_blend_%s", get_size_prefix(size), is_double ? "pd" : "ps");
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

    int elem_size = 1;
    if (cdg.insn.itype == NN_vpblendd) {
        elem_size = 4;
    } else if (cdg.insn.itype == NN_vpblendw) {
        elem_size = 2;
    }

    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }

    if (cdg.insn.itype == NN_vpblendvb) {
        // vpblendvb: xmm1, xmm2, xmm3/m128, xmm4
        mreg_t blend_mask = reg2mreg(cdg.insn.Op4.reg);
        qstring base_name;
        base_name.cat_sprnt("_mm%s_blendv_epi8", get_size_prefix(size));
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }
        icall.add_argument_reg(l, ti);
        icall.add_argument_reg(r, ti);
        icall.add_argument_reg(blend_mask, ti);
        icall.set_return_reg(d, ti);
        icall.emit();
    } else {
        // vpblendd/vpblendw: xmm1, xmm2, xmm3/m128, imm8
        QASSERT(0xA0610, cdg.insn.Op4.type == o_imm);
        uval_t imm8 = cdg.insn.Op4.value;

        qstring base_name;
        if (cdg.insn.itype == NN_vpblendd) {
            base_name.cat_sprnt("_mm%s_blend_epi32", get_size_prefix(size));
        } else { // NN_vpblendw
            base_name.cat_sprnt("_mm%s_blend_epi16", get_size_prefix(size));
        }
        qstring iname = mask.has_mask ? make_masked_intrinsic_name(base_name.c_str(), mask) : base_name;
        AVXIntrinsic icall(&cdg, iname.c_str());
        tinfo_t ti = get_type_robust(size, true, false);
        if (mask.has_mask) {
            if (!mask.is_zeroing) {
                icall.add_argument_reg(d, ti);
            }
            icall.add_argument_mask(mask.mask_reg, mask.num_elements);
        }
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
    QASSERT(0xA0900, cdg.insn.Op3.type == o_imm);

    int src_size = get_vector_size(cdg.insn.Op2);
    int dst_size = XMM_SIZE;
    bool is_int = true;
    qstring base_name;

    switch (cdg.insn.itype) {
        case NN_vextractf128:
            src_size = YMM_SIZE;
            dst_size = XMM_SIZE;
            is_int = false;
            base_name = "_mm256_extractf128_ps";
            break;
        case NN_vextracti128:
            src_size = YMM_SIZE;
            dst_size = XMM_SIZE;
            is_int = true;
            base_name = "_mm256_extracti128_si256";
            break;
        case NN_vextracti32x4:
            dst_size = XMM_SIZE;
            is_int = true;
            base_name.cat_sprnt("_mm%s_extracti32x4_epi32", get_size_prefix(src_size));
            break;
        case NN_vextracti32x8:
            dst_size = YMM_SIZE;
            is_int = true;
            base_name.cat_sprnt("_mm%s_extracti32x8_epi32", get_size_prefix(src_size));
            break;
        case NN_vextracti64x4:
            dst_size = YMM_SIZE;
            is_int = true;
            base_name.cat_sprnt("_mm%s_extracti64x4_epi64", get_size_prefix(src_size));
            break;
        default:
            return MERR_INSN;
    }

    mreg_t src = reg2mreg(cdg.insn.Op2.reg);
    uint64 imm = cdg.insn.Op3.value;

    AVXIntrinsic icall(&cdg, base_name.c_str());
    tinfo_t vt_src = get_type_robust(src_size, is_int, false);
    tinfo_t vt_dst = get_type_robust(dst_size, is_int, false);

    icall.add_argument_reg(src, vt_src);
    icall.add_argument_imm(imm, BT_INT32);

    if (is_xmm_reg(cdg.insn.Op1) || is_ymm_reg(cdg.insn.Op1)) {
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
        icall.set_return_reg(dst, vt_dst);
        icall.emit();
        if (dst_size == XMM_SIZE) {
            clear_upper(cdg, dst);
        }
    } else {
        QASSERT(0xA0902, is_mem_op(cdg.insn.Op1));
        mreg_t tmp = cdg.mba->alloc_kreg(dst_size);
        icall.set_return_reg(tmp, vt_dst);
        icall.emit();
        store_operand_hack(cdg, 0, mop_t(tmp, dst_size));
        cdg.mba->free_kreg(tmp, dst_size);
    }

    return MERR_OK;
}

merror_t handle_vinsertf128(codegen_t &cdg) {
    QASSERT(0xA0910, cdg.insn.Op4.type == o_imm);

    int dst_size = get_vector_size(cdg.insn.Op1);
    int src2_size = XMM_SIZE;
    bool is_int = true;
    bool is_double = false;
    qstring base_name;

    switch (cdg.insn.itype) {
        case NN_vinsertf128:
            dst_size = YMM_SIZE;
            src2_size = XMM_SIZE;
            is_int = false;
            is_double = false;
            base_name = "_mm256_insertf128_ps";
            break;
        case NN_vinsertf32x4:
            src2_size = XMM_SIZE;
            is_int = false;
            is_double = false;
            base_name.cat_sprnt("_mm%s_insertf32x4_ps", get_size_prefix(dst_size));
            break;
        case NN_vinsertf64x4:
            src2_size = YMM_SIZE;
            is_int = false;
            is_double = true;
            base_name.cat_sprnt("_mm%s_insertf64x4_pd", get_size_prefix(dst_size));
            break;
        case NN_vinserti128:
            dst_size = YMM_SIZE;
            src2_size = XMM_SIZE;
            is_int = true;
            base_name = "_mm256_inserti128_si256";
            break;
        case NN_vinserti32x4:
            src2_size = XMM_SIZE;
            is_int = true;
            base_name.cat_sprnt("_mm%s_inserti32x4_epi32", get_size_prefix(dst_size));
            break;
        case NN_vinserti32x8:
            src2_size = YMM_SIZE;
            is_int = true;
            base_name.cat_sprnt("_mm%s_inserti32x8_epi32", get_size_prefix(dst_size));
            break;
        case NN_vinserti64x4:
            src2_size = YMM_SIZE;
            is_int = true;
            base_name.cat_sprnt("_mm%s_inserti64x4_epi64", get_size_prefix(dst_size));
            break;
        default:
            return MERR_INSN;
    }

    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    mreg_t src1 = reg2mreg(cdg.insn.Op2.reg);
    AvxOpLoader src2(cdg, 2, cdg.insn.Op3);
    uint64 imm = cdg.insn.Op4.value;

    AVXIntrinsic icall(&cdg, base_name.c_str());
    tinfo_t vt_dst = get_type_robust(dst_size, is_int, is_double);
    tinfo_t vt_src2 = get_type_robust(src2_size, is_int, is_double);

    icall.add_argument_reg(src1, vt_dst);
    icall.add_argument_reg(src2, vt_src2);
    icall.add_argument_imm(imm, BT_INT32);
    icall.set_return_reg(dst, vt_dst);
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
    bool is_half = false;

    uint16 it = cdg.insn.itype;

    // vfmaddsub: odd elements are added, even elements are subtracted
    // vfmsubadd: odd elements are subtracted, even elements are added
    if (it == NN_vfmaddsub132ph || it == NN_vfmaddsub213ph || it == NN_vfmaddsub231ph) {
        op = "fmaddsub";
        is_half = true;
        type = "ph";
        order = (it == NN_vfmaddsub132ph) ? 132 : (it == NN_vfmaddsub213ph) ? 213 : 231;
    } else if (it == NN_vfmsubadd132ph || it == NN_vfmsubadd213ph || it == NN_vfmsubadd231ph) {
        op = "fmsubadd";
        is_half = true;
        type = "ph";
        order = (it == NN_vfmsubadd132ph) ? 132 : (it == NN_vfmsubadd213ph) ? 213 : 231;
    } else if (it >= NN_vfmaddsub132ps && it <= NN_vfmaddsub231pd) {
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

    int elem_size = is_half ? WORD_SIZE : (is_double ? 8 : 4);
    MaskInfo mask = MaskInfo::from_insn(cdg.insn, elem_size);
    if (mask.has_mask) {
        load_mask_operand(cdg, mask);
    }
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
    if (cdg.insn.itype == NN_vmovntdqa) {
        QASSERT(0xA0A00, is_mem_op(cdg.insn.Op2));

        int size = get_vector_size(cdg.insn.Op1);
        AvxOpLoader src(cdg, 1, cdg.insn.Op2);
        mreg_t dst = reg2mreg(cdg.insn.Op1.reg);

        cdg.emit(m_mov, size, src, 0, dst, 0);
        if (size == XMM_SIZE) clear_upper(cdg, dst);
        return MERR_OK;
    }

    // Non-temporal stores: memory destination, register source
    QASSERT(0xA0A01, is_mem_op(cdg.insn.Op1));

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

merror_t handle_v_mask_to_vec(codegen_t &cdg) {
    int size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);

    int elem_size = 1;
    const char *suffix = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpmovm2b:
            elem_size = 1;
            suffix = "epi8";
            break;
        case NN_vpmovm2w:
            elem_size = 2;
            suffix = "epi16";
            break;
        case NN_vpmovm2d:
            elem_size = 4;
            suffix = "epi32";
            break;
        case NN_vpmovm2q:
            elem_size = 8;
            suffix = "epi64";
            break;
        default:
            return MERR_INSN;
    }

    QASSERT(0xA0A02, cdg.insn.Op2.type == o_kreg || cdg.insn.Op2.type == o_reg);
    int kreg_num = cdg.insn.Op2.reg - R_k0;
    mreg_t mask_reg = (mreg_t)(-(kreg_num + 1));
    int num_elements = size / elem_size;

    qstring iname;
    iname.cat_sprnt("_mm%s_movm_%s", get_size_prefix(size), suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti = get_type_robust(size, true, false);

    icall.add_argument_mask(mask_reg, num_elements);
    icall.set_return_reg(d, ti);
    icall.emit();

    if (size == XMM_SIZE) clear_upper(cdg, d);
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

// vpmovwb/vpmovswb/vpmovuswb - narrow packed words to bytes
merror_t handle_vpmovwb(codegen_t &cdg) {
    int dst_size = get_vector_size(cdg.insn.Op1);
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    AvxOpLoader src(cdg, 1, cdg.insn.Op2);

    const char *suffix = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpmovwb: suffix = "cvtepi16_epi8"; break;
        case NN_vpmovswb: suffix = "cvtsepi16_epi8"; break;
        case NN_vpmovuswb: suffix = "cvtusepi16_epi8"; break;
        default: return MERR_INSN;
    }

    int src_size = src.size > 0 ? src.size : dst_size * 2;
    qstring iname;
    iname.cat_sprnt("_mm%s_%s", get_size_prefix(src_size), suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_src = get_type_robust(src_size, true, false);
    tinfo_t ti_dst = get_type_robust(dst_size, true, false);

    icall.add_argument_reg(src, ti_src);
    icall.set_return_reg(d, ti_dst);
    icall.emit();

    if (dst_size == XMM_SIZE) clear_upper(cdg, d);
    return MERR_OK;
}

merror_t handle_vpmov_down(codegen_t &cdg) {
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

    const char *iname = nullptr;
    switch (cdg.insn.itype) {
        case NN_vpmovdb: iname = "_mm512_cvtepi32_epi8"; break;
        case NN_vpmovdw: iname = "_mm512_cvtepi32_epi16"; break;
        case NN_vpmovqb: iname = "_mm512_cvtepi64_epi8"; break;
        case NN_vpmovqd: iname = "_mm512_cvtepi64_epi32"; break;
        case NN_vpmovsdb: iname = "_mm512_cvtsepi32_epi8"; break;
        case NN_vpmovsdw: iname = "_mm512_cvtsepi32_epi16"; break;
        case NN_vpmovsqb: iname = "_mm512_cvtsepi64_epi8"; break;
        case NN_vpmovsqd: iname = "_mm512_cvtsepi64_epi32"; break;
        case NN_vpmovsqw: iname = "_mm512_cvtsepi64_epi16"; break;
        case NN_vpmovusdb: iname = "_mm512_cvtusepi32_epi8"; break;
        case NN_vpmovusdw: iname = "_mm512_cvtusepi32_epi16"; break;
        case NN_vpmovusqb: iname = "_mm512_cvtusepi64_epi8"; break;
        case NN_vpmovusqd: iname = "_mm512_cvtusepi64_epi32"; break;
        case NN_vpmovusqw: iname = "_mm512_cvtusepi64_epi16"; break;
        default: return MERR_INSN;
    }

    AVXIntrinsic icall(&cdg, iname);
    tinfo_t ti_src = get_type_robust(src_size, true, false);
    tinfo_t ti_dst = get_type_robust(dst_size, true, false);

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
