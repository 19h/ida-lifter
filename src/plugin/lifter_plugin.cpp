#include "../common/warn_off.h"
#include <hexrays.hpp>
#include "../common/warn_on.h"

#include "component_registry.h"

// Component headers are included here to trigger registration
#include "../avx/avx_lifter.h"
#include "../inline/inline_component.h"

//--------------------------------------------------------------------------
// Hexrays Callback - Add popup menu items
//--------------------------------------------------------------------------

static ssize_t idaapi hexrays_callback(void *, hexrays_event_t event, va_list va)
{
  if (event == hxe_populating_popup) {
    TWidget *widget = va_arg(va, TWidget *);
    TPopupMenu *popup = va_arg(va, TPopupMenu *);
    vdui_t *vu = va_arg(va, vdui_t *);

    // Add separator if we have any components
    if (component_registry_t::get_count() > 0)
      attach_action_to_popup(widget, popup, nullptr);

    // Attach all component actions
    component_registry_t::attach_to_popup(widget, popup, vu);
  }
  return 0;
}

//--------------------------------------------------------------------------
// Plugin Initialization
//--------------------------------------------------------------------------

static plugmod_t* idaapi init(void)
{
  if (!init_hexrays_plugin()) {
    msg("[lifter] Plugin requires Hex-Rays decompiler\n");
    return PLUGIN_SKIP;
  }

  msg("[lifter] Plugin initializing (%d components registered)\n", (int)component_registry_t::get_count());

  // Install hexrays callback for popup menus
  install_hexrays_callback(hexrays_callback, nullptr);

  int initialized = component_registry_t::init_all();
  msg("[lifter] Plugin ready (%d components initialized)\n", initialized);
  return PLUGIN_KEEP;
}

static void idaapi term(void)
{
  msg("[lifter] Plugin terminating\n");

  // Remove hexrays callback
  remove_hexrays_callback(hexrays_callback, nullptr);

  // Unregister all component actions
  component_registry_t::unregister_all_actions();

  int terminated = component_registry_t::done_all();
  msg("[lifter] Plugin done (%d components terminated)\n", terminated);
}

static bool idaapi run(size_t)
{
  return false;
}

//--------------------------------------------------------------------------
// Plugin Descriptor
//--------------------------------------------------------------------------

plugin_t PLUGIN =
{
  IDP_INTERFACE_VERSION,
  PLUGIN_FIX,                           // plugin flags
  init,                                 // initialize
  term,                                 // terminate
  run,                                  // invoke plugin procedure
  "AVX Lifter Plugin",                  // long comment about the plugin
  "Lifts AVX instructions for Hex-Rays Decompiler",  // help text
  "AVX Lifter",                         // preferred short name
  ""                                    // preferred hotkey
};
