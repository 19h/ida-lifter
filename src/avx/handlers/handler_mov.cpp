/*
 AVX Move Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_vmov_ss_sd(codegen_t &cdg, int data_size) {
    if (cdg.insn.Op3.type == o_void) {
        if (is_xmm_reg(cdg.insn.Op1)) {
            QASSERT(0xA0300, is_mem_op(cdg.insn.Op2));
            mop_t *lm = new mop_t(cdg.load_operand(1), data_size);
            mreg_t d = reg2mreg(cdg.insn.Op1.reg);
            cdg.emit(m_xdu, lm, nullptr, new mop_t(d, XMM_SIZE));
            clear_upper(cdg, d, data_size);
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
        mreg_t l = is_mem_op(cdg.insn.Op2) ? cdg.load_operand(1) : reg2mreg(cdg.insn.Op2.reg);
        mreg_t d = reg2mreg(cdg.insn.Op1.reg);
        cdg.emit(m_xdu, new mop_t(l, data_size), nullptr, new mop_t(d, XMM_SIZE));
        clear_upper(cdg, d);
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

#endif // IDA_SDK_VERSION >= 750
