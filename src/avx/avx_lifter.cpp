#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <intel.hpp>
#include <bytes.hpp>
#include <name.hpp>
#include <typeinf.hpp>
#include "../common/warn_on.h"
#include "../plugin/component_registry.h"
#include "avx_types.h"
#include "avx_intrinsic.h"
#include "avx_helpers.h"
#include "avx_utils.h"
#include "avx_debug.h"
#include "handlers/avx_handlers.h"

#if IDA_SDK_VERSION >= 750

//-----------------------------------------------------------------------------
// The microcode filter
//-----------------------------------------------------------------------------
struct ida_local AVXLifter : microcode_filter_t
{
//----- match
bool match(codegen_t &cdg) override
{
  ea_t ea = cdg.insn.ea;
  uint16 it = cdg.insn.itype;

  if (is_avx_512(cdg.insn)) return false;

  bool m = is_compare_insn(it) || is_extract_insn(it) || is_conversion_insn(it) ||
           is_move_insn(it) || is_bitwise_insn(it) || is_math_insn(it) ||
           is_broadcast_insn(it) || is_blend_insn(it) || is_packed_compare_insn(it) ||
           is_maskmov_insn(it) || is_misc_insn(it);

  if (m) {
      DEBUG_LOG("%a: MATCH itype=%u", ea, it);
  }
  return m;
}

//----- apply
merror_t apply(codegen_t &cdg) override
{
  ea_t ea = cdg.insn.ea;
  uint16 it = cdg.insn.itype;

  TRACE_ENTER("apply");

  if (try_convert_to_sse(cdg)) return MERR_INSN;

  // conversions
  if (it==NN_vcvtdq2ps)   return handle_vcvtdq2ps(cdg);
  if (it==NN_vcvtsi2ss || it==NN_vcvtsi2sd) return handle_vcvtsi2fp(cdg);
  if (it==NN_vcvtps2pd)   return handle_vcvtps2pd(cdg);
  if (it==NN_vcvtss2sd || it==NN_vcvtsd2ss) return handle_vcvtfp2fp(cdg);
  if (it==NN_vcvtpd2ps)   return handle_vcvtpd2ps(cdg);
  if (it==NN_vcvttps2dq)  return handle_vcvt_ps2dq(cdg, true);
  if (it==NN_vcvtps2dq)   return handle_vcvt_ps2dq(cdg, false);
  if (it==NN_vcvttpd2dq)  return handle_vcvt_pd2dq(cdg, true);
  if (it==NN_vcvtpd2dq)   return handle_vcvt_pd2dq(cdg, false);

  // moves
  if (it==NN_vmovd)       return handle_vmov(cdg, DWORD_SIZE);
  if (it==NN_vmovq)       return handle_vmov(cdg, QWORD_SIZE);
  if (it==NN_vmovss)      return handle_vmov_ss_sd(cdg, FLOAT_SIZE);
  if (it==NN_vmovsd)      return handle_vmov_ss_sd(cdg, DOUBLE_SIZE);
  if (it==NN_vmovaps || it==NN_vmovups || it==NN_vmovdqa || it==NN_vmovdqu)
    return handle_v_mov_ps_dq(cdg);

  // bitwise (now full 128/256-bit via intrinsics)
  if (is_bitwise_insn(it)) return handle_v_bitwise(cdg);

  // scalar math (add/sub/mul/div)
  if (it==NN_vaddss||it==NN_vsubss||it==NN_vmulss||it==NN_vdivss) return handle_v_math_ss_sd(cdg, FLOAT_SIZE);
  if (it==NN_vaddsd||it==NN_vsubsd||it==NN_vmulsd||it==NN_vdivsd) return handle_v_math_ss_sd(cdg, DOUBLE_SIZE);

  // scalar min/max
  if (is_scalar_minmax(it)) return handle_v_minmax_ss_sd(cdg);

  // packed math (+ min/max + integer add/sub + integer mul)
  if (it==NN_vaddps||it==NN_vsubps||it==NN_vmulps||it==NN_vdivps||
      it==NN_vaddpd||it==NN_vsubpd||it==NN_vmulpd||it==NN_vdivpd||
      it==NN_vpaddb||it==NN_vpsubb||it==NN_vpaddw||it==NN_vpsubw||
      it==NN_vpaddd||it==NN_vpsubd||it==NN_vpaddq||it==NN_vpsubq||
      is_packed_minmax_fp(it) || is_packed_minmax_int(it) || is_int_mul(it))
    return handle_v_math_p(cdg);

  // broadcasts
  if (it==NN_vbroadcastss || it==NN_vbroadcastsd) return handle_vbroadcast_ss_sd(cdg);
  if (it==NN_vbroadcastf128) return handle_vbroadcastf128_fp(cdg);
  if (it==NN_vbroadcasti128) return handle_vbroadcasti128_int(cdg);

  // packed compares
  if (is_packed_compare_insn(it)) return handle_vcmp_ps_pd(cdg);

  // blend
  if (is_blend_insn(it)) return handle_vblendv_ps_pd(cdg);

  // maskmov
  if (is_maskmov_insn(it)) return handle_vmaskmov_ps_pd(cdg);

  // misc
  if (it==NN_vsqrtss) return handle_vsqrtss(cdg);
  if (it==NN_vsqrtps) return handle_vsqrtps(cdg);
  if (it==NN_vshufps) return handle_vshufps(cdg);
  if (it==NN_vzeroupper) return handle_vzeroupper_nop(cdg);

  return MERR_INSN;
}
};

//-----------------------------------------------------------------------------
// Debug callback for printing disassembly and microcode
//-----------------------------------------------------------------------------
static bool g_callback_active = false;

static ssize_t idaapi hexrays_debug_callback(void *, hexrays_event_t event, va_list va)
{
  // Safety check: don't process if we're shutting down
  if (!g_callback_active)
    return 0;

  switch (event)
  {
    case hxe_maturity:
    {
      mba_t *mba = va_arg(va, mba_t *);
      mba_maturity_t new_maturity = va_argi(va, mba_maturity_t);

      // Safety check: ensure mba is valid
      if (!mba)
        break;

      DEBUG_LOG("hxe_maturity event: ea=%a maturity=%d", mba->entry_ea, new_maturity);

      // Print disassembly once when we first generate microcode
      if (new_maturity == MMAT_GENERATED)
      {
        DEBUG_LOG("Calling print_function_disassembly for %a", mba->entry_ea);
        print_function_disassembly(mba->entry_ea);
      }

      // Print microcode before our lifter processes it
      if (new_maturity == MMAT_PREOPTIMIZED)
      {
        DEBUG_LOG("Calling print_function_microcode BEFORE for %a", mba->entry_ea);
        print_function_microcode(mba, "BEFORE LIFTER");
      }

      // Print microcode after our lifter has processed it
      if (new_maturity == MMAT_LOCOPT)
      {
        DEBUG_LOG("Calling print_function_microcode AFTER for %a", mba->entry_ea);
        print_function_microcode(mba, "AFTER LIFTER");
      }
      break;
    }
    default:
      break;
  }
  return 0;
}

//-----------------------------------------------------------------------------
// Component glue
//-----------------------------------------------------------------------------
static AVXLifter *g_avx = nullptr;

static bool isMicroAvx_avail()
{
if (PH.id != PLFM_386 || !inf_is_64bit())
 return false;
return true;
}

static bool isMicroAvx_active() { return g_avx != nullptr; }

extern "C" void set_debug_logging(bool enabled)
{
  debug_logging_enabled = enabled;
  msg("[AVXLifter] Debug logging set to %s\n", enabled ? "TRUE" : "FALSE");

  // Also enable/disable debug printing
  ::set_debug_printing(enabled);
}

static void MicroAvx_init()
{
  if (g_avx) return;

  // Enable debug logging and printing by default for troubleshooting
  debug_logging_enabled = true;
  ::set_debug_printing(true);

  msg("[AVXLifter] Initializing AVXLifter component\n");

  // Install debug callback for printing disassembly/microcode
  g_callback_active = true;
  install_hexrays_callback(hexrays_debug_callback, nullptr);

  g_avx = new AVXLifter();
  install_microcode_filter(g_avx, true);
}

static void MicroAvx_done()
{
  if (!g_avx) return;

  msg("[AVXLifter] Terminating AVXLifter component\n");

  // Disable callback first to prevent any callbacks during cleanup
  g_callback_active = false;

  // Remove microcode filter before removing callback
  install_microcode_filter(g_avx, false);

  // Remove debug callback
  remove_hexrays_callback(hexrays_debug_callback, nullptr);

  // Clean up lifter instance
  delete g_avx;
  g_avx = nullptr;
}

static const char avx_short_name[] = "avx";
REGISTER_COMPONENT(isMicroAvx_avail, isMicroAvx_active, MicroAvx_init, MicroAvx_done, nullptr, "AVXLifter", avx_short_name, "AVXLifter")

#endif // IDA_SDK_VERSION >= 750
