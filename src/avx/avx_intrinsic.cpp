/*
    AVX Intrinsic Call Builder
*/

#include "avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

#include "../common/warn_off.h"
#include <pro.h>
#include "../common/warn_on.h"

AVXIntrinsic::AVXIntrinsic(codegen_t *cdg_, const char *name)
    : cdg(cdg_), call_info(nullptr), call_insn(nullptr), mov_insn(nullptr), emitted(false)
{
  // Allocate call_info with IDA's allocator
  call_info = (mcallinfo_t*)qalloc(sizeof(mcallinfo_t));
  new (call_info) mcallinfo_t();
  call_info->cc = CM_CC_FASTCALL;
  call_info->flags = FCI_SPLOK | FCI_FINAL | FCI_PROP;
  call_info->return_type = tinfo_t(BT_VOID); // Default to void

  // Allocate call_insn with IDA's allocator
  call_insn = (minsn_t*)qalloc(sizeof(minsn_t));
  new (call_insn) minsn_t(cdg->insn.ea);
  call_insn->opcode = m_call;
  call_insn->l.make_helper(name);
  call_insn->d.t = mop_f;
  call_insn->d.f = call_info;
  call_insn->d.size = 0;
}

AVXIntrinsic::~AVXIntrinsic()
{
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

void AVXIntrinsic::set_return_reg(mreg_t mreg, const tinfo_t &ret_ti)
{
  size_t size = ret_ti.get_size();
  if (size == 0 || size > 64) {
      ERROR_LOG("Invalid return type size %" FMT_Z " for mreg %d. Aborting intrinsic setup.", size, mreg);
      return;
  }

  call_info->return_type = ret_ti;
  call_insn->d.size = (int)size;

  // Create the wrapper move instruction
  mov_insn = (minsn_t*)qalloc(sizeof(minsn_t));
  new (mov_insn) minsn_t(cdg->insn.ea);
  mov_insn->opcode = m_mov;
  mov_insn->l.make_insn(call_insn);
  mov_insn->l.size = call_insn->d.size;
  mov_insn->d.make_reg(mreg, call_insn->d.size);

  if (ret_ti.is_decl_floating())
  {
    mov_insn->set_fpinsn();
  }
}

void AVXIntrinsic::set_return_reg(mreg_t mreg, const char *type_name)
{
  tinfo_t ti;
  bool ok = ti.get_named_type(nullptr, type_name);

  // Validate size even if type exists
  if (ok) {
      size_t sz = ti.get_size();
      if (sz == 0 || sz > 64) ok = false;
  }

  if (!ok) {
      // Fallback logic using robust synthesizer
      if (strstr(type_name, "256")) ti = get_vector_type(32, false, false);
      else ti = get_vector_type(16, false, false);
  }
  set_return_reg(mreg, ti);
}

void AVXIntrinsic::set_return_reg_basic(mreg_t mreg, type_t basic_type)
{
  set_return_reg(mreg, tinfo_t(basic_type));
}

void AVXIntrinsic::add_argument_reg(mreg_t mreg, const tinfo_t &arg_ti)
{
  mcallarg_t ca(mop_t(mreg, (int)arg_ti.get_size()));
  ca.type = arg_ti;
  ca.size = (decltype(ca.size))arg_ti.get_size();
  call_info->args.add(ca);
  call_info->solid_args++;
}

void AVXIntrinsic::add_argument_reg(mreg_t mreg, const char *type_name)
{
  tinfo_t ti;
  bool ok = ti.get_named_type(nullptr, type_name);
  if (ok && (ti.get_size() == 0 || ti.get_size() > 64)) ok = false;

  if (!ok) {
       if (strstr(type_name, "256")) ti = get_vector_type(32, false, false);
       else if (strstr(type_name, "128")) ti = get_vector_type(16, false, false);
       else ti = tinfo_t(BT_INT); // Safe fallback
  }
  add_argument_reg(mreg, ti);
}

void AVXIntrinsic::add_argument_reg(mreg_t mreg, type_t bt)
{
  add_argument_reg(mreg, tinfo_t(bt));
}

void AVXIntrinsic::add_argument_imm(uint64 value, type_t bt)
{
  tinfo_t ti(bt);
  mcallarg_t ca;
  ca.make_number(value, (int)ti.get_size());
  ca.type = ti;
  ca.size = (decltype(ca.size))ti.get_size();
  call_info->args.add(ca);
  call_info->solid_args++;
}

minsn_t *AVXIntrinsic::emit()
{
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

#endif // IDA_SDK_VERSION >= 750
