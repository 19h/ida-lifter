/*
    AVX Conversion Handlers
*/

#include "avx_handlers.h"
#include "../avx_utils.h"
#include "../avx_helpers.h"
#include "../avx_intrinsic.h"

#if IDA_SDK_VERSION >= 750

merror_t handle_vcvtdq2ps(codegen_t &cdg)
{
  int op_size = get_op_size(cdg.insn);
  mreg_t r = load_op_reg_or_mem(cdg, 1, cdg.insn.Op2);
  mreg_t d = reg2mreg(cdg.insn.Op1.reg);

  qstring iname = make_intrinsic_name("_mm%s_cvtepi32_ps", op_size);
  AVXIntrinsic icall(&cdg, iname.c_str());

  icall.set_return_reg(d, get_type_robust(op_size, false));
  icall.add_argument_reg(r, get_type_robust(op_size, true));
  icall.emit();

  if (op_size==XMM_SIZE) clear_upper(cdg, d);
  return MERR_OK;
}

merror_t handle_vcvtsi2fp(codegen_t &cdg)
{
  int src_size = (int)get_dtype_size(cdg.insn.Op3.dtype);
  int dst_size = (cdg.insn.itype == NN_vcvtsi2sd) ? DOUBLE_SIZE : FLOAT_SIZE;

  mreg_t r = load_op_reg_or_mem(cdg, 2, cdg.insn.Op3);
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

merror_t handle_vcvtps2pd(codegen_t &cdg)
{
  int src128 = is_xmm_reg(cdg.insn.Op1) ? QWORD_SIZE : XMM_SIZE; // element block
  mreg_t r = load_op_reg_or_mem(cdg, 1, cdg.insn.Op2);
  mreg_t d = reg2mreg(cdg.insn.Op1.reg);

  qstring iname = make_intrinsic_name("_mm%s_cvtps_pd", src128*2);
  AVXIntrinsic icall(&cdg, iname.c_str());
  icall.add_argument_reg(r, get_type_robust(16, false)); // Always __m128 input

  icall.set_return_reg(d, get_type_robust(src128*2, false, true));
  icall.emit();

  if (src128==QWORD_SIZE) clear_upper(cdg, d);
  return MERR_OK;
}

merror_t handle_vcvtfp2fp(codegen_t &cdg)
{
  bool is_ss2sd = (cdg.insn.itype == NN_vcvtss2sd);
  int src_size = is_ss2sd ? FLOAT_SIZE : DOUBLE_SIZE;
  int dst_size = is_ss2sd ? DOUBLE_SIZE : FLOAT_SIZE;

  mreg_t r = load_op_reg_or_mem(cdg, 2, cdg.insn.Op3);
  mreg_t l = reg2mreg(cdg.insn.Op2.reg);
  mreg_t d = reg2mreg(cdg.insn.Op1.reg);

  mreg_t t = cdg.mba->alloc_kreg(XMM_SIZE);
  cdg.emit(m_mov, XMM_SIZE, l, 0, t, 0);
  cdg.emit(m_f2f, new mop_t(r, src_size), nullptr, new mop_t(t, dst_size));
  cdg.emit(m_mov, XMM_SIZE, t, 0, d, 0);

  cdg.mba->free_kreg(t, XMM_SIZE);

  clear_upper(cdg, d);
  return MERR_OK;
}

merror_t handle_vcvtpd2ps(codegen_t &cdg)
{
  int src_size = is_ymm_reg(cdg.insn.Op2) ? YMM_SIZE : XMM_SIZE;
  mreg_t r = load_op_reg_or_mem(cdg, 1, cdg.insn.Op2);
  mreg_t d = reg2mreg(cdg.insn.Op1.reg);

  qstring iname = make_intrinsic_name("_mm%s_cvtpd_ps", src_size);
  AVXIntrinsic icall(&cdg, iname.c_str());

  icall.add_argument_reg(r, get_type_robust(src_size, false, true));
  icall.set_return_reg(d, get_type_robust(16, false));
  icall.emit();

  clear_upper(cdg, d);
  return MERR_OK;
}

merror_t handle_vcvt_ps2dq(codegen_t &cdg, bool trunc)
{
  int op_size = get_op_size(cdg.insn);
  mreg_t r = load_op_reg_or_mem(cdg, 1, cdg.insn.Op2);
  mreg_t d = reg2mreg(cdg.insn.Op1.reg);

  const char* iname = (op_size==YMM_SIZE)
   ? (trunc ? "_mm256_cvttps_epi32" : "_mm256_cvtps_epi32")
   : (trunc ? "_mm_cvttps_epi32"   : "_mm_cvtps_epi32");

  AVXIntrinsic icall(&cdg, iname);
  icall.add_argument_reg(r, get_type_robust(op_size, false));
  icall.set_return_reg(d, get_type_robust(op_size, true));
  icall.emit();

  if (op_size==XMM_SIZE) clear_upper(cdg, d);
  return MERR_OK;
}

merror_t handle_vcvt_pd2dq(codegen_t &cdg, bool trunc)
{
  int src_size = is_ymm_reg(cdg.insn.Op2) ? YMM_SIZE : XMM_SIZE;
  mreg_t r = load_op_reg_or_mem(cdg, 1, cdg.insn.Op2);
  mreg_t d = reg2mreg(cdg.insn.Op1.reg);

  qstring iname = trunc ? make_intrinsic_name("_mm%s_cvttpd_epi32", src_size)
                       : make_intrinsic_name("_mm%s_cvtpd_epi32",  src_size);
  AVXIntrinsic icall(&cdg, iname.c_str());

  icall.add_argument_reg(r, get_type_robust(src_size, false, true));
  icall.set_return_reg(d, get_type_robust(16, true));
  icall.emit();

  clear_upper(cdg, d);
  return MERR_OK;
}

#endif // IDA_SDK_VERSION >= 750
