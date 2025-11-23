/*
AVX Move Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_vmov_ss_sd(codegen_t &cdg, int data_size) {
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

merror_t handle_vmov(codegen_t &cdg, int data_size) {
    if (is_xmm_reg(cdg.insn.Op1)) {
        mreg_t xmm_reg = reg2mreg(cdg.insn.Op1.reg);
        mreg_t ymm_reg = get_ymm_mreg(xmm_reg);
        if (ymm_reg == mr_none) return MERR_INSN;

        AvxOpLoader l_loader(cdg, 1, cdg.insn.Op2);
        mreg_t l = l_loader.reg;

        // Move value to lower part of XMM, then use m_xdu to zero-extend to YMM
        mreg_t tmp = cdg.mba->alloc_kreg(data_size);
        cdg.emit(m_mov, data_size, l, 0, tmp, 0);

        mop_t src(tmp, data_size);
        mop_t dst(ymm_reg, YMM_SIZE);
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
    if (is_avx_reg(cdg.insn.Op1)) {
        int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
        AvxOpLoader l_loader(cdg, 1, cdg.insn.Op2);
        mreg_t l = l_loader.reg;
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);

        mop_t src(l, size);
        mop_t dst(d, size);
        mop_t r;
        cdg.emit(m_mov, &src, &r, &dst);

        if (size == XMM_SIZE) clear_upper(cdg, d);
        return MERR_OK;
    }

    QASSERT(0xA0304, is_mem_op(cdg.insn.Op1) && is_avx_reg(cdg.insn.Op2));
    int size = is_xmm_reg(cdg.insn.Op2) ? XMM_SIZE : YMM_SIZE;
    mreg_t s = reg2mreg(cdg.insn.Op2.reg);
    store_operand_hack(cdg, 0, mop_t(s, size));
    return MERR_OK;
}

merror_t handle_v_gather(codegen_t &cdg) {
    int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
    mreg_t dst = reg2mreg(cdg.insn.Op1.reg);
    mreg_t mask = reg2mreg(cdg.insn.Op3.reg);
    const op_t &mem = cdg.insn.Op2;
    mreg_t base = reg2mreg(mem.reg);

    static int xmm0_idx = -1;
    if (xmm0_idx == -1) xmm0_idx = str2reg("xmm0");

    // sib_index requires insn and op
    mreg_t index_vec = reg2mreg(xmm0_idx + sib_index(cdg.insn, mem));

    // sib_scale requires only op_t
    int scale = 1 << sib_scale(mem);
    ea_t disp = mem.addr;

    const char *suffix = nullptr;
    bool is_int = false;
    bool is_double = false;
    switch (cdg.insn.itype) {
        case NN_vgatherdps: suffix = "ps";
            break;
        case NN_vgatherdpd: suffix = "pd";
            is_double = true;
            break;
        case NN_vpgatherdd: suffix = "epi32";
            is_int = true;
            break;
        case NN_vpgatherdq: suffix = "epi64";
            is_int = true;
            break;
    }

    qstring iname;
    iname.cat_sprnt("_mm%s_mask_i32gather_%s", size == YMM_SIZE ? "256" : "", suffix);

    AVXIntrinsic icall(&cdg, iname.c_str());
    tinfo_t ti_dst = get_type_robust(size, is_int, is_double);
    int idx_size = (is_double || cdg.insn.itype == NN_vpgatherdq) ? XMM_SIZE : size;
    tinfo_t ti_idx = get_type_robust(idx_size, true, false);

    icall.add_argument_reg(dst, ti_dst);

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

    icall.add_argument_reg(arg_base, tinfo_t(BT_PTR));
    icall.add_argument_reg(index_vec, ti_idx);
    icall.add_argument_reg(mask, ti_dst);
    icall.add_argument_imm(scale, BT_INT32);
    icall.set_return_reg(dst, ti_dst);
    icall.emit();

    if (t_base != mr_none) {
        cdg.mba->free_kreg(t_base, 8);
    }

    if (size == XMM_SIZE) clear_upper(cdg, dst);
    return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
