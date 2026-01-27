/*
 * VMX/VT-x Microcode Lifter
 *
 * Lifts Intel VMX (Virtual Machine Extensions) instructions to intrinsic-style
 * function calls in IDA Pro's decompiled pseudocode.
 *
 * Supported instructions:
 * - vmxon, vmxoff        - Enter/leave VMX operation
 * - vmcall               - Call VM monitor
 * - vmlaunch, vmresume   - Launch/resume VM
 * - vmptrld, vmptrst     - Load/store VMCS pointer
 * - vmclear              - Clear VMCS
 * - vmread, vmwrite      - Read/write VMCS field
 * - invept, invvpid      - Invalidate TLB translations
 * - vmfunc               - VM function call
 */

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <intel.hpp>
#include <bytes.hpp>
#include <name.hpp>
#include <typeinf.hpp>
#include "../common/warn_on.h"
#include "../plugin/component_registry.h"

#if IDA_SDK_VERSION >= 750

//-----------------------------------------------------------------------------
// Debug logging
//-----------------------------------------------------------------------------
static bool vmx_debug_logging = false;

#define VMX_DEBUG_LOG(fmt, ...) \
    do { if (vmx_debug_logging) msg("[VMXLifter] " fmt "\n", ##__VA_ARGS__); } while(0)

#define VMX_ERROR_LOG(fmt, ...) \
    do { msg("[VMXLifter] ERROR: " fmt "\n", ##__VA_ARGS__); } while(0)

//-----------------------------------------------------------------------------
// VMX Instruction Classification
//-----------------------------------------------------------------------------
static bool is_vmx_insn(uint16 it) {
    switch (it) {
        case NN_vmxon:
        case NN_vmxoff:
        case NN_vmcall:
        case NN_vmlaunch:
        case NN_vmresume:
        case NN_vmptrld:
        case NN_vmptrst:
        case NN_vmclear:
        case NN_vmread:
        case NN_vmwrite:
        case NN_invept:
        case NN_invvpid:
        case NN_vmfunc:
            return true;
        default:
            return false;
    }
}

// Instructions that take a memory operand (pointer to 64-bit region)
static bool vmx_has_mem_operand(uint16 it) {
    switch (it) {
        case NN_vmxon:
        case NN_vmptrld:
        case NN_vmptrst:
        case NN_vmclear:
            return true;
        default:
            return false;
    }
}

// Instructions with no operands
static bool vmx_is_no_operand(uint16 it) {
    switch (it) {
        case NN_vmxoff:
        case NN_vmcall:
        case NN_vmlaunch:
        case NN_vmresume:
        case NN_vmfunc:
            return true;
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// VMX Intrinsic Call Builder
//-----------------------------------------------------------------------------
struct VMXIntrinsic {
    codegen_t *cdg;
    mcallinfo_t *call_info;
    minsn_t *call_insn;
    minsn_t *mov_insn;
    bool emitted;
    int stk_off;

    explicit VMXIntrinsic(codegen_t *cdg_, const char *name)
        : cdg(cdg_), call_info(nullptr), call_insn(nullptr), mov_insn(nullptr),
          emitted(false), stk_off(0) {
        call_info = (mcallinfo_t *)qalloc(sizeof(mcallinfo_t));
        new (call_info) mcallinfo_t();
        call_info->cc = CM_CC_FASTCALL;
        call_info->flags = FCI_SPLOK | FCI_FINAL | FCI_PROP;
        call_info->return_type = tinfo_t(BT_VOID);

        call_insn = (minsn_t *)qalloc(sizeof(minsn_t));
        new (call_insn) minsn_t(cdg->insn.ea);
        call_insn->opcode = m_call;
        call_insn->l.make_helper(name);
        call_insn->d.t = mop_f;
        call_insn->d.f = call_info;
        call_insn->d.size = 0;
    }

    ~VMXIntrinsic() {
        if (!emitted) {
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

    void set_return_type(const tinfo_t &ret_ti) {
        call_info->return_type = ret_ti;
        call_insn->d.size = (int)ret_ti.get_size();
    }

    void set_return_reg(mreg_t mreg, const tinfo_t &ret_ti) {
        size_t size = ret_ti.get_size();
        if (size == 0 || size > 8) {
            VMX_ERROR_LOG("Invalid return type size %" FMT_Z, size);
            return;
        }

        call_info->return_type = ret_ti;
        call_insn->d.size = (int)size;

        mov_insn = (minsn_t *)qalloc(sizeof(minsn_t));
        new (mov_insn) minsn_t(cdg->insn.ea);
        mov_insn->opcode = m_mov;
        mov_insn->l.make_insn(call_insn);
        mov_insn->l.size = call_insn->d.size;
        mov_insn->d.make_reg(mreg, call_insn->d.size);
    }

    void add_argument_reg(mreg_t mreg, const tinfo_t &arg_ti) {
        int ti_size = (int)arg_ti.get_size();
        mcallarg_t ca(mop_t(mreg, ti_size));
        ca.type = arg_ti;
        ca.size = (decltype(ca.size))ti_size;

        int align = ti_size;
        if (align < 8) align = 8;
        stk_off = (stk_off + align - 1) & ~(align - 1);
        ca.argloc.set_stkoff(stk_off);
        stk_off += ca.size;

        call_info->args.add(ca);
        call_info->solid_args++;
    }

    void add_argument_imm(uint64 value, type_t bt, int size) {
        tinfo_t ti(bt);
        mcallarg_t ca;
        ca.make_number(value, size);
        ca.type = ti;
        ca.size = size;

        int align = size;
        if (align < 8) align = 8;
        stk_off = (stk_off + align - 1) & ~(align - 1);
        ca.argloc.set_stkoff(stk_off);
        stk_off += ca.size;

        call_info->args.add(ca);
        call_info->solid_args++;
    }

    // Emit void-returning intrinsic
    minsn_t *emit_void() {
        if (!cdg->mb) {
            VMX_ERROR_LOG("Microblock is NULL");
            return nullptr;
        }
        call_insn->d.size = 0;
        minsn_t *result = cdg->mb->insert_into_block(call_insn, cdg->mb->tail);
        emitted = true;
        return result;
    }

    // Emit intrinsic with return value
    minsn_t *emit() {
        if (!mov_insn) {
            VMX_ERROR_LOG("Return register not set");
            return nullptr;
        }
        if (!cdg->mb) {
            VMX_ERROR_LOG("Microblock is NULL");
            return nullptr;
        }
        minsn_t *result = cdg->mb->insert_into_block(mov_insn, cdg->mb->tail);
        emitted = true;
        return result;
    }
};

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

// Get address size based on mode
static int get_addr_size(codegen_t &cdg) {
    return inf_is_64bit() ? 8 : 4;
}

// Create void* pointer type
static tinfo_t get_ptr_type() {
    tinfo_t ptr_type;
    ptr_type.create_ptr(tinfo_t(BT_VOID));
    return ptr_type;
}

// Load effective address of memory operand into a register
static mreg_t load_mem_address(codegen_t &cdg, int opidx) {
    return cdg.load_effective_address(opidx);
}

//-----------------------------------------------------------------------------
// VMX Instruction Handlers
//-----------------------------------------------------------------------------

// Handle no-operand instructions: vmxoff, vmcall, vmlaunch, vmresume, vmfunc
static merror_t handle_vmx_no_op(codegen_t &cdg, const char *name) {
    VMX_DEBUG_LOG("%a: %s", cdg.insn.ea, name);
    VMXIntrinsic icall(&cdg, name);
    icall.emit_void();
    return MERR_OK;
}

// Handle memory-operand instructions: vmxon, vmptrld, vmclear
static merror_t handle_vmx_mem_op(codegen_t &cdg, const char *name) {
    VMX_DEBUG_LOG("%a: %s [mem]", cdg.insn.ea, name);

    mreg_t addr = load_mem_address(cdg, 0);
    if (addr == mr_none) {
        VMX_ERROR_LOG("%a: Failed to load address for %s", cdg.insn.ea, name);
        return MERR_INSN;
    }

    VMXIntrinsic icall(&cdg, name);
    icall.add_argument_reg(addr, get_ptr_type());
    icall.emit_void();
    return MERR_OK;
}

// Handle vmptrst: Store VMCS pointer to memory
static merror_t handle_vmptrst(codegen_t &cdg) {
    VMX_DEBUG_LOG("%a: vmptrst [mem]", cdg.insn.ea);

    mreg_t addr = load_mem_address(cdg, 0);
    if (addr == mr_none) {
        VMX_ERROR_LOG("%a: Failed to load address for vmptrst", cdg.insn.ea);
        return MERR_INSN;
    }

    VMXIntrinsic icall(&cdg, "__vmptrst");
    icall.add_argument_reg(addr, get_ptr_type());
    icall.emit_void();
    return MERR_OK;
}

// Handle vmread: Read VMCS field
// vmread r/m64, r64  (destination, encoding)
static merror_t handle_vmread(codegen_t &cdg) {
    const insn_t &insn = cdg.insn;
    VMX_DEBUG_LOG("%a: vmread", insn.ea);

    int addr_size = get_addr_size(cdg);
    tinfo_t uint_type = inf_is_64bit() ? tinfo_t(BT_INT64) : tinfo_t(BT_INT32);

    // Op1 = destination (reg or mem), Op2 = encoding (reg)
    mreg_t encoding_reg = reg2mreg(insn.Op2.reg);
    
    // Check if destination is memory or register
    if (insn.Op1.type == o_reg) {
        // vmread reg, encoding -> reg = __vmread(encoding)
        mreg_t dst_reg = reg2mreg(insn.Op1.reg);
        
        VMXIntrinsic icall(&cdg, "__vmread");
        icall.add_argument_reg(encoding_reg, uint_type);
        icall.set_return_reg(dst_reg, uint_type);
        icall.emit();
    } else {
        // vmread [mem], encoding -> __vmread_mem(&dest, encoding)
        mreg_t addr = load_mem_address(cdg, 0);
        if (addr == mr_none) {
            VMX_ERROR_LOG("%a: Failed to load address for vmread", insn.ea);
            return MERR_INSN;
        }

        VMXIntrinsic icall(&cdg, "__vmread");
        icall.add_argument_reg(addr, get_ptr_type());
        icall.add_argument_reg(encoding_reg, uint_type);
        icall.emit_void();
    }
    return MERR_OK;
}

// Handle vmwrite: Write VMCS field
// vmwrite r64, r/m64  (encoding, source)
static merror_t handle_vmwrite(codegen_t &cdg) {
    const insn_t &insn = cdg.insn;
    VMX_DEBUG_LOG("%a: vmwrite", insn.ea);

    int addr_size = get_addr_size(cdg);
    tinfo_t uint_type = inf_is_64bit() ? tinfo_t(BT_INT64) : tinfo_t(BT_INT32);

    // Op1 = encoding (reg), Op2 = source (reg or mem)
    mreg_t encoding_reg = reg2mreg(insn.Op1.reg);
    
    VMXIntrinsic icall(&cdg, "__vmwrite");
    icall.add_argument_reg(encoding_reg, uint_type);
    
    if (insn.Op2.type == o_reg) {
        mreg_t src_reg = reg2mreg(insn.Op2.reg);
        icall.add_argument_reg(src_reg, uint_type);
    } else {
        // Load from memory
        mreg_t val = cdg.load_operand(1);
        if (val == mr_none) {
            VMX_ERROR_LOG("%a: Failed to load operand for vmwrite", insn.ea);
            return MERR_INSN;
        }
        icall.add_argument_reg(val, uint_type);
    }
    
    icall.emit_void();
    return MERR_OK;
}

// Handle invept: Invalidate EPT translations
// invept r32/r64, m128  (type, descriptor)
static merror_t handle_invept(codegen_t &cdg) {
    const insn_t &insn = cdg.insn;
    VMX_DEBUG_LOG("%a: invept", insn.ea);

    tinfo_t uint_type = inf_is_64bit() ? tinfo_t(BT_INT64) : tinfo_t(BT_INT32);

    // Op1 = type (reg), Op2 = descriptor (mem)
    mreg_t type_reg = reg2mreg(insn.Op1.reg);
    mreg_t desc_addr = load_mem_address(cdg, 1);
    if (desc_addr == mr_none) {
        VMX_ERROR_LOG("%a: Failed to load descriptor address for invept", insn.ea);
        return MERR_INSN;
    }

    VMXIntrinsic icall(&cdg, "__invept");
    icall.add_argument_reg(type_reg, uint_type);
    icall.add_argument_reg(desc_addr, get_ptr_type());
    icall.emit_void();
    return MERR_OK;
}

// Handle invvpid: Invalidate VPID translations
// invvpid r32/r64, m128  (type, descriptor)
static merror_t handle_invvpid(codegen_t &cdg) {
    const insn_t &insn = cdg.insn;
    VMX_DEBUG_LOG("%a: invvpid", insn.ea);

    tinfo_t uint_type = inf_is_64bit() ? tinfo_t(BT_INT64) : tinfo_t(BT_INT32);

    // Op1 = type (reg), Op2 = descriptor (mem)
    mreg_t type_reg = reg2mreg(insn.Op1.reg);
    mreg_t desc_addr = load_mem_address(cdg, 1);
    if (desc_addr == mr_none) {
        VMX_ERROR_LOG("%a: Failed to load descriptor address for invvpid", insn.ea);
        return MERR_INSN;
    }

    VMXIntrinsic icall(&cdg, "__invvpid");
    icall.add_argument_reg(type_reg, uint_type);
    icall.add_argument_reg(desc_addr, get_ptr_type());
    icall.emit_void();
    return MERR_OK;
}

//-----------------------------------------------------------------------------
// The VMX Microcode Filter
//-----------------------------------------------------------------------------
struct ida_local VMXLifter : microcode_filter_t {
    bool match(codegen_t &cdg) override {
        uint16 it = cdg.insn.itype;
        bool m = is_vmx_insn(it);
        if (m) {
            VMX_DEBUG_LOG("%a: MATCH itype=%u", cdg.insn.ea, it);
        }
        return m;
    }

    merror_t apply(codegen_t &cdg) override {
        uint16 it = cdg.insn.itype;
        VMX_DEBUG_LOG("%a: APPLY itype=%u", cdg.insn.ea, it);

        switch (it) {
            // No-operand instructions
            case NN_vmxoff:   return handle_vmx_no_op(cdg, "__vmxoff");
            case NN_vmcall:   return handle_vmx_no_op(cdg, "__vmcall");
            case NN_vmlaunch: return handle_vmx_no_op(cdg, "__vmlaunch");
            case NN_vmresume: return handle_vmx_no_op(cdg, "__vmresume");
            case NN_vmfunc:   return handle_vmx_no_op(cdg, "__vmfunc");

            // Memory-operand instructions
            case NN_vmxon:    return handle_vmx_mem_op(cdg, "__vmxon");
            case NN_vmptrld:  return handle_vmx_mem_op(cdg, "__vmptrld");
            case NN_vmclear:  return handle_vmx_mem_op(cdg, "__vmclear");
            case NN_vmptrst:  return handle_vmptrst(cdg);

            // VMCS read/write
            case NN_vmread:   return handle_vmread(cdg);
            case NN_vmwrite:  return handle_vmwrite(cdg);

            // TLB invalidation
            case NN_invept:   return handle_invept(cdg);
            case NN_invvpid:  return handle_invvpid(cdg);

            default:
                return MERR_INSN;
        }
    }
};

//-----------------------------------------------------------------------------
// Component Registration
//-----------------------------------------------------------------------------
static VMXLifter *g_vmx = nullptr;

static bool isVMXLifter_avail() {
    // Only support x86/x64 (metapc processor)
    return PH.id == PLFM_386;
}

static bool isVMXLifter_active() {
    return g_vmx != nullptr;
}

extern "C" void set_vmx_debug_logging(bool enabled) {
    vmx_debug_logging = enabled;
    msg("[VMXLifter] Debug logging set to %s\n", enabled ? "TRUE" : "FALSE");
}

static void VMXLifter_init() {
    if (g_vmx) return;

    vmx_debug_logging = true;
    msg("[VMXLifter] Initializing VMXLifter component\n");

    g_vmx = new VMXLifter();
    install_microcode_filter(g_vmx, true);
}

static void VMXLifter_done() {
    if (!g_vmx) return;

    msg("[VMXLifter] Terminating VMXLifter component\n");
    install_microcode_filter(g_vmx, false);
    delete g_vmx;
    g_vmx = nullptr;
}

static const char vmx_short_name[] = "vmx";
REGISTER_COMPONENT(isVMXLifter_avail, isVMXLifter_active, VMXLifter_init, VMXLifter_done,
                   nullptr, "VMXLifter", vmx_short_name, "VMXLifter")

#endif // IDA_SDK_VERSION >= 750
