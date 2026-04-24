/*
AVX Intrinsic Call Builder
*/

#include "avx_intrinsic.h"
#include "avx_helpers.h"

#if IDA_SDK_VERSION >= 750

#include "../common/warn_off.h"
#include <intel.hpp>
#include <pro.h>
#include "../common/warn_on.h"

static int get_zmm_index_from_mreg(mreg_t mreg) {
    int reg = mreg2reg(mreg, ZMM_SIZE);
    if (reg >= R_zmm0 && reg <= R_zmm31) return reg - R_zmm0;

    reg = mreg2reg(mreg, YMM_SIZE);
    if (reg >= R_ymm0 && reg <= R_ymm15) return reg - R_ymm0;
    if (reg >= R_ymm16 && reg <= R_ymm31) return 16 + (reg - R_ymm16);

    reg = mreg2reg(mreg, XMM_SIZE);
    if (reg >= R_xmm0 && reg <= R_xmm15) return reg - R_xmm0;
    if (reg >= R_xmm16 && reg <= R_xmm31) return 16 + (reg - R_xmm16);

    return -1;
}

AVXIntrinsic::AVXIntrinsic(codegen_t *cdg_, const char *name)
    : cdg(cdg_), call_info(nullptr), call_insn(nullptr), mov_insn(nullptr), emitted(false), stk_off(0),
      virtual_return_zmm_index(-1) {
    // Allocate call_info with IDA's allocator
    call_info = (mcallinfo_t *) qalloc(sizeof(mcallinfo_t));
    new(call_info) mcallinfo_t();
    call_info->cc = CM_CC_SPECIAL;
    call_info->flags = FCI_SPLOK | FCI_FINAL | FCI_PROP;
    call_info->return_type = tinfo_t(BT_VOID); // Default to void

    // Allocate call_insn with IDA's allocator
    call_insn = (minsn_t *) qalloc(sizeof(minsn_t));
    new(call_insn) minsn_t(cdg->insn.ea);
    call_insn->opcode = m_call;
    call_insn->l.make_helper(name);
    call_insn->d.t = mop_f;
    call_insn->d.f = call_info;
    call_insn->d.size = 0;
}

AVXIntrinsic::~AVXIntrinsic() {
    if (!emitted) {
        DEBUG_LOG("%a: AVXIntrinsic dtor: cleaning up unused instructions", cdg->insn.ea);
        // If not emitted, we must clean up to prevent leaks.
        if (mov_insn) {
            mov_insn->~minsn_t();
            qfree(mov_insn);
        } else if (call_insn) {
            call_insn->~minsn_t();
            qfree(call_insn);
        } else if (call_info) {
            call_info->~mcallinfo_t();
            qfree(call_info);
        }
    }
}

void AVXIntrinsic::set_return_reg(mreg_t mreg, const tinfo_t &ret_ti) {
    size_t size = ret_ti.get_size();
    if (size == 0 || size > 64) {
        ERROR_LOG("Invalid return type size %" FMT_Z " for mreg %d. Aborting intrinsic setup.", size, mreg);
        return;
    }

    if (size == ZMM_SIZE) {
        int zmm_index = get_zmm_index_from_mreg(mreg);
        if (zmm_index >= 0) {
            virtual_return_zmm_index = zmm_index;
            virtual_return_type = ret_ti;
        }
    }

    call_info->return_type = ret_ti;
    call_insn->d.size = (int) size;

    // For sizes > 8 bytes (non-standard sizes), mark as UDT to pass verification
    // IDA's mop_t::verify allows non-standard sizes (like 16, 32, 64) only for UDTs
    if (size > 8) {
        call_insn->d.set_udt();
    }

    if (virtual_return_zmm_index >= 0) {
        return;
    }

    // Create the wrapper move instruction
    mov_insn = (minsn_t *) qalloc(sizeof(minsn_t));
    new(mov_insn) minsn_t(cdg->insn.ea);
    mov_insn->opcode = m_mov;
    mov_insn->l.make_insn(call_insn);
    mov_insn->l.size = call_insn->d.size;
    mov_insn->d.make_reg(mreg, call_insn->d.size);

    // For sizes > 8 bytes (non-standard sizes), mark as UDT to pass verification
    if (size > 8) {
        mov_insn->l.set_udt();
        mov_insn->d.set_udt();
    }

    if (ret_ti.is_decl_floating()) {
        mov_insn->set_fpinsn();
    }
}

void AVXIntrinsic::set_return_reg(mreg_t mreg, const char *type_name) {
    tinfo_t ti;
    bool ok = ti.get_named_type(nullptr, type_name);

    // Validate size even if type exists
    if (ok) {
        size_t sz = ti.get_size();
        if (sz == 0 || sz > 64) ok = false;
    }

    if (!ok) {
        // Fallback logic using robust synthesizer
        if (strstr(type_name, "512")) ti = get_vector_type(64, false, false);
        else if (strstr(type_name, "256")) ti = get_vector_type(32, false, false);
        else ti = get_vector_type(16, false, false);
    }
    set_return_reg(mreg, ti);
}

void AVXIntrinsic::set_return_reg_basic(mreg_t mreg, type_t basic_type) {
    set_return_reg(mreg, tinfo_t(basic_type));
}

void AVXIntrinsic::add_argument_reg(mreg_t mreg, const tinfo_t &arg_ti) {
    int ti_size = (int) arg_ti.get_size();

    if (ti_size == ZMM_SIZE) {
        int zmm_index = get_zmm_index_from_mreg(mreg);
        if (zmm_index >= 0) {
            mop_t read_mop;
            read_mop.make_insn(make_zmm_read_call(*cdg, zmm_index, arg_ti));
            read_mop.size = ti_size;
            read_mop.set_udt();
            add_argument_mop(read_mop, arg_ti);
            return;
        }
    }

    mcallarg_t ca(mop_t(mreg, ti_size));
    ca.type = arg_ti;
    ca.size = (decltype(ca.size)) ti_size;

    // For sizes > 8 bytes (non-standard sizes), mark as UDT to pass verification
    // IDA's mop_t::verify allows non-standard sizes (like 16, 32, 64) only for UDTs
    if (ca.size > 8) {
        ca.set_udt();
    }

    // Assign dummy stack location to satisfy verification
    // Align to natural size of argument (min 8 bytes for stack slot)
    int align = ca.size;
    if (align < 8) align = 8;
    // Ensure power of 2 alignment for vectors (up to 64 for ZMM)
    if (align > 64) align = 64;

    stk_off = (stk_off + align - 1) & ~(align - 1);
    ca.argloc.set_stkoff(stk_off);
    stk_off += ca.size;

    call_info->args.add(ca);
    call_info->solid_args++;
}

void AVXIntrinsic::add_argument_reg(mreg_t mreg, const char *type_name) {
    tinfo_t ti;
    bool ok = ti.get_named_type(nullptr, type_name);
    if (ok && (ti.get_size() == 0 || ti.get_size() > 64)) ok = false;

    if (!ok) {
        if (strstr(type_name, "512")) ti = get_vector_type(64, false, false);
        else if (strstr(type_name, "256")) ti = get_vector_type(32, false, false);
        else if (strstr(type_name, "128")) ti = get_vector_type(16, false, false);
        else ti = tinfo_t(BT_INT); // Safe fallback
    }
    add_argument_reg(mreg, ti);
}

void AVXIntrinsic::add_argument_reg(mreg_t mreg, type_t bt) {
    add_argument_reg(mreg, tinfo_t(bt));
}

void AVXIntrinsic::add_argument_mop(const mop_t &arg, const tinfo_t &arg_ti) {
    int ti_size = (int) arg_ti.get_size();
    mcallarg_t ca(arg);
    ca.type = arg_ti;
    ca.size = (decltype(ca.size)) ti_size;

    if (ca.size > 8) {
        ca.set_udt();
    }

    int align = ca.size;
    if (align < 8) align = 8;
    if (align > 64) align = 64;

    stk_off = (stk_off + align - 1) & ~(align - 1);
    ca.argloc.set_stkoff(stk_off);
    stk_off += ca.size;

    call_info->args.add(ca);
    call_info->solid_args++;
}

void AVXIntrinsic::add_argument_reg_with_size(mreg_t mreg, int size) {
    // Create appropriate integer type for the given size (used for pointers)
    type_t bt;
    switch (size) {
        case 1: bt = BT_INT8; break;
        case 2: bt = BT_INT16; break;
        case 4: bt = BT_INT32; break;
        case 8:
        default: bt = BT_INT64; break;
    }

    tinfo_t ti(bt);
    mcallarg_t ca(mop_t(mreg, size));
    ca.type = ti;
    ca.size = size;

    // Assign dummy stack location to satisfy verification
    int align = size;
    if (align < 8) align = 8;

    stk_off = (stk_off + align - 1) & ~(align - 1);
    ca.argloc.set_stkoff(stk_off);
    stk_off += ca.size;

    call_info->args.add(ca);
    call_info->solid_args++;
}

void AVXIntrinsic::add_argument_imm(uint64 value, type_t bt) {
    tinfo_t ti(bt);
    mcallarg_t ca;
    ca.make_number(value, (int) ti.get_size());
    ca.type = ti;
    ca.size = (decltype(ca.size)) ti.get_size();

    // Assign dummy stack location to satisfy verification
    int align = ca.size;
    if (align < 8) align = 8;

    stk_off = (stk_off + align - 1) & ~(align - 1);
    ca.argloc.set_stkoff(stk_off);
    stk_off += ca.size;

    call_info->args.add(ca);
    call_info->solid_args++;
}

void AVXIntrinsic::add_argument_mask(mreg_t mreg, int num_elements) {
    // Determine mask type based on number of elements
    // __mmask8  for 8 elements (e.g., ZMM with 64-bit elements)
    // __mmask16 for 16 elements (e.g., ZMM with 32-bit elements, or YMM with 16-bit)
    // __mmask32 for 32 elements (e.g., ZMM with 16-bit elements)
    // __mmask64 for 64 elements (e.g., ZMM with 8-bit elements)

    type_t bt;
    int size;
    if (num_elements <= 8) {
        bt = BT_INT8;
        size = 1;
    } else if (num_elements <= 16) {
        bt = BT_INT16;
        size = 2;
    } else if (num_elements <= 32) {
        bt = BT_INT32;
        size = 4;
    } else {
        bt = BT_INT64;
        size = 8;
    }

    tinfo_t ti(bt);
    mcallarg_t ca;

    // Check if mreg is actually a k-register number encoded as negative value
    // load_mask_operand() encodes k-reg number N as -(N+1)
    if (mreg < 0) {
        // It's a k-register number, pass as immediate
        int kreg_num = -(mreg + 1);
        // Use a symbolic value: 0xK0, 0xK1, etc. for better readability
        // Actually just pass the register number - it will show as a constant
        ca.make_number((uint64)kreg_num, size);
    } else {
        // It's a valid mreg
        ca = mcallarg_t(mop_t(mreg, size));
    }

    ca.type = ti;
    ca.size = size;

    // Assign dummy stack location to satisfy verification
    int align = size;
    if (align < 8) align = 8;

    stk_off = (stk_off + align - 1) & ~(align - 1);
    ca.argloc.set_stkoff(stk_off);
    stk_off += ca.size;

    call_info->args.add(ca);
    call_info->solid_args++;
}

minsn_t *AVXIntrinsic::emit() {
    if (virtual_return_zmm_index >= 0) {
        if (!cdg->mb) {
            ERROR_LOG("Microblock is NULL");
            return nullptr;
        }
        if (!call_insn) {
            ERROR_LOG("Call instruction is NULL");
            return nullptr;
        }

        mop_t value;
        value.make_insn(call_insn);
        value.size = call_insn->d.size;
        value.set_udt();

        emitted = true; // call_insn ownership is transferred into the write helper argument.
        AVXIntrinsic write(cdg, "__lifter_zmm_write");
        write.add_argument_imm((uint64) virtual_return_zmm_index, BT_INT32);
        write.add_argument_mop(value, virtual_return_type);
        minsn_t *result = write.emit_void();
        if (result == nullptr) {
            ERROR_LOG("Failed to emit virtual ZMM write for zmm%d", virtual_return_zmm_index);
        }
        return result;
    }

    if (!mov_insn) {
        ERROR_LOG("Attempted to emit intrinsic without return register set");
        return nullptr;
    }
    if (!cdg->mb) {
        ERROR_LOG("Microblock is NULL");
        return nullptr;
    }

    minsn_t *result = cdg->mb->insert_into_block(mov_insn, cdg->mb->tail);
    emitted = true; // Ownership transferred to block
    return result;
}

minsn_t *AVXIntrinsic::emit_void() {
    // Emit a void-returning intrinsic (like store intrinsics)
    // Don't require mov_insn - just emit the call directly
    if (!cdg->mb) {
        ERROR_LOG("Microblock is NULL");
        return nullptr;
    }
    if (!call_insn) {
        ERROR_LOG("Call instruction is NULL");
        return nullptr;
    }

    // For void return, set the destination size to 0
    call_insn->d.size = 0;

    minsn_t *result = cdg->mb->insert_into_block(call_insn, cdg->mb->tail);
    emitted = true; // Ownership transferred to block
    return result;
}

#endif // IDA_SDK_VERSION >= 750
