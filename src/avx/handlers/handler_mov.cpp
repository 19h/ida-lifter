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
            // Load case: vmovss xmm1, m32
            QASSERT(0xA0300, is_mem_op(cdg.insn.Op2));
            mop_t *lm = new mop_t(cdg.load_operand(1), data_size);
            mreg_t d = reg2mreg(cdg.insn.Op1.reg);

            // Optimization: Write directly to YMM to handle zero-extension in one step.
            // vmovss/sd zeroes bits 32-127 (XMM upper) and 128-255 (YMM upper).
            // m_xdu from data_size to YMM_SIZE achieves exactly this.
            mreg_t ymm = get_ymm_mreg(d);
            if (ymm != mr_none) {
                cdg.emit(m_xdu, lm, nullptr, new mop_t(ymm, YMM_SIZE));
            } else {
                // Fallback if YMM not found (should not happen in AVX context)
                cdg.emit(m_xdu, lm, nullptr, new mop_t(d, XMM_SIZE));
                // Note: Can't clear upper YMM if we can't find it.
            }
            return MERR_OK;
        } else {
            // Store case: vmovss m32, xmm1
            QASSERT(0xA0301, is_mem_op(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2));
            minsn_t *out = nullptr;
            if (store_operand_hack(cdg, 0, mop_t(reg2mreg(cdg.insn.Op2.reg), data_size), 0, &out)) {
                out->set_fpinsn();
                return MERR_OK;
            }
        }
        return MERR_INSN;
    }

    // Merge case: vmovss xmm1, xmm2, xmm3
    QASSERT(0xA0302, is_xmm_reg(cdg.insn.Op1) && is_xmm_reg(cdg.insn.Op2) && is_xmm_reg(cdg.insn.Op3));
    mreg_t d = reg2mreg(cdg.insn.Op1.reg);
    mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);

    // t = src1 (copy full 128 bits)
    cdg.emit(m_mov, XMM_SIZE, reg2mreg(cdg.insn.Op2.reg), 0, t, 0);

    // t[0..size] = src2[0..size] (insert scalar)
    // Use m_mov for bitwise insertion. m_f2f might be valid but m_mov is safer for bits.
    cdg.emit(m_mov, data_size, reg2mreg(cdg.insn.Op3.reg), 0, t, 0);

    // d = t
    cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);
    cdg.mba->free_kreg(t, XMM_SIZE);

    // Clear upper YMM bits (128-255).
    // Use default op_size=XMM_SIZE to extend from the 128-bit result.
    // Do NOT use data_size here, as it would zero bits 32-127 which we just merged!
    clear_upper(cdg, d);

    return MERR_OK;
}

merror_t handle_vmov(codegen_t &cdg, int data_size) {
    if (is_xmm_reg(cdg.insn.Op1)) {
        // Load case: vmovd/vmovq xmm1, r/m
        mreg_t l = is_mem_op(cdg.insn.Op2) ? cdg.load_operand(1) : reg2mreg(cdg.insn.Op2.reg);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);

        // Optimization: Write directly to YMM to handle zero-extension in one step.
        mreg_t ymm = get_ymm_mreg(d);
        if (ymm != mr_none) {
            cdg.emit(m_xdu, new mop_t(l, data_size), nullptr, new mop_t(ymm, YMM_SIZE));
        } else {
            cdg.emit(m_xdu, new mop_t(l, data_size), nullptr, new mop_t(d, XMM_SIZE));
        }
        return MERR_OK;
    }

    QASSERT(0xA0303, is_xmm_reg(cdg.insn.Op2));
    mreg_t l = reg2mreg(cdg.insn.Op2.reg);

    if (is_mem_op(cdg.insn.Op1)) {
        store_operand_hack(cdg, 0, mop_t(l, data_size));
    } else {
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        cdg.emit(m_mov, new mop_t(l, data_size), nullptr, new mop_t(d, data_size));
    }
    return MERR_OK;
}

merror_t handle_v_mov_ps_dq(codegen_t &cdg) {
    if (is_avx_reg(cdg.insn.Op1)) {
        int size = is_xmm_reg(cdg.insn.Op1) ? XMM_SIZE : YMM_SIZE;
        mreg_t l = is_mem_op(cdg.insn.Op2) ? cdg.load_operand(1) : reg2mreg(cdg.insn.Op2.reg);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        cdg.emit(m_mov, new mop_t(l, size), nullptr, new mop_t(d, size));

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
