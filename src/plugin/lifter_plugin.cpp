#include "../common/warn_off.h"
#include <hexrays.hpp>
#include <cstring>
#include "../common/warn_on.h"

#include "component_registry.h"

// Component headers are included here to trigger registration
#include "../avx/avx_lifter.h"
#include "../inline/inline_component.h"

//--------------------------------------------------------------------------
// Hexrays Callback - Add popup menu items
//--------------------------------------------------------------------------

static bool is_suppressed_lifter_warning(const hexwarn_t &warn) {
    // The AVX lifter virtualizes ZMM state through helper calls, but Hex-Rays
    // can still attach this native register-tracking warning after successful
    // decompilation. Keep other unsupported-register warnings visible.
    return warn.id == WARN_UNSUPP_REG && std::strstr(warn.text.c_str(), "unsupported processor register 'zmm") != nullptr;
}

static void suppress_lifter_warnings(hexwarns_t &warnings) {
    for (hexwarns_t::iterator it = warnings.begin(); it != warnings.end();) {
        if (is_suppressed_lifter_warning(*it)) {
            it = warnings.erase(it);
        } else {
            ++it;
        }
    }
}

static void suppress_lifter_warnings(cfunc_t *cfunc) {
    if (cfunc == nullptr) return;

    suppress_lifter_warnings(cfunc->get_warnings());
    if (cfunc->mba != nullptr) {
        suppress_lifter_warnings(cfunc->mba->notes);
    }
}

static ssize_t idaapi hexrays_callback(void *, hexrays_event_t event, va_list va) {
    switch (event) {
        case hxe_maturity: {
            cfunc_t *cfunc = va_arg(va, cfunc_t *);
            ctree_maturity_t maturity = va_argi(va, ctree_maturity_t);
            if (maturity == CMAT_FINAL) suppress_lifter_warnings(cfunc);
            break;
        }

        case hxe_print_func: {
            cfunc_t *cfunc = va_arg(va, cfunc_t *);
            suppress_lifter_warnings(cfunc);
            break;
        }

        case hxe_func_printed: {
            cfunc_t *cfunc = va_arg(va, cfunc_t *);
            suppress_lifter_warnings(cfunc);
            break;
        }

        case hxe_populating_popup: {
            TWidget *widget = va_arg(va, TWidget *);
            TPopupMenu *popup = va_arg(va, TPopupMenu *);
            vdui_t *vu = va_arg(va, vdui_t *);

            // Add separator if we have any components
            if (component_registry_t::get_count() > 0)
                attach_action_to_popup(widget, popup, nullptr);

            // Attach all component actions
            component_registry_t::attach_to_popup(widget, popup, vu);
            break;
        }

        default:
            break;
    }
    return 0;
}

//--------------------------------------------------------------------------
// Plugin Initialization (PLUGIN_MULTI model)
//
// We must use PLUGIN_MULTI rather than the legacy PLUGIN_FIX/init()-returns-
// PLUGIN_KEEP style. PLUGIN_FIX plugins are initialized by init_plugins() very
// early during kernel startup (in idalib this happens inside init_library(),
// before any database is opened and before the decompiler is initialized).
// At that point init_hexrays_plugin() fails and the plugin would bail out,
// leaving the microcode filters uninstalled (everything falls back to __asm).
//
// PLUGIN_MULTI plugmods are instantiated per-database, after the decompiler is
// available, so init_hexrays_plugin() succeeds and the filters install. This
// mirrors the shipped 'deobf' plugin.
//--------------------------------------------------------------------------

struct lifter_plugmod_t : public plugmod_t {
    bool hexrays_ready = false;

    lifter_plugmod_t() {
        if (!init_hexrays_plugin()) {
            // No decompiler for this database: stay inert. The plugmod is kept
            // around but installs nothing.
            return;
        }
        hexrays_ready = true;

        // Install hexrays callback for popup menus / warning suppression
        install_hexrays_callback(hexrays_callback, this);

        int initialized = component_registry_t::init_all();
        msg("[lifter] Plugin ready (%d/%d components initialized)\n",
            initialized, (int) component_registry_t::get_count());
    }

    virtual ~lifter_plugmod_t() {
        if (!hexrays_ready)
            return;

        // Remove hexrays callback
        remove_hexrays_callback(hexrays_callback, this);

        // Unregister all component actions
        component_registry_t::unregister_all_actions();

        component_registry_t::done_all();
    }

    virtual bool idaapi run(size_t) override {
        return false;
    }
};

static plugmod_t * idaapi init(void) {
    return new lifter_plugmod_t;
}

//--------------------------------------------------------------------------
// Plugin Descriptor
//--------------------------------------------------------------------------

plugin_t PLUGIN =
{
    IDP_INTERFACE_VERSION,
    PLUGIN_MULTI, // plugin flags: modern per-database plugmod model
    init, // initialize -> returns plugmod_t*
    nullptr, // term must be nullptr for PLUGIN_MULTI
    nullptr, // run must be nullptr for PLUGIN_MULTI (use plugmod_t::run)
    "AVX Lifter Plugin", // long comment about the plugin
    "Lifts AVX instructions for Hex-Rays Decompiler", // help text
    "AVX Lifter", // preferred short name
    "" // preferred hotkey
};
