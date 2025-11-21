#include "inline_component.h"
#include "../plugin/component_registry.h"

#include <set>
#include <funcs.hpp>
#include <xref.hpp>
#include <hexrays.hpp>
#include <kernwin.hpp>

using std::set;

//------------------------------------------------------------------------------
// Action identifiers
//------------------------------------------------------------------------------

static const char ACTION_MARK_INLINE[] = "lifter:mark_inline";
static const char ACTION_MARK_OUTLINE[] = "lifter:mark_outline";

static bool g_inline_actions_registered = false;

//------------------------------------------------------------------------------
// Utility: mark target function and all callers as "dirty" for Hex-Rays
//------------------------------------------------------------------------------

static void mark_callers_dirty(ea_t entry_ea) {
    // Clear cached cfunc/microcode for the target itself.
    mark_cfunc_dirty(entry_ea, false);

    set<ea_t> caller_entries;

    xrefblk_t xb;
    // Iterate over all xrefs *to* the function entry.
    for (bool ok = xb.first_to(entry_ea, XREF_ALL); ok; ok = xb.next_to()) {
        func_t *caller = get_func(xb.from);
        if (caller != nullptr)
            caller_entries.insert(caller->start_ea);
    }

    for (ea_t fea: caller_entries)
        mark_cfunc_dirty(fea, false);
}

//------------------------------------------------------------------------------
// Utility: toggle FUNC_OUTLINE for function under the cursor in pseudocode
//------------------------------------------------------------------------------

static int do_toggle_inline(action_activation_ctx_t *ctx, bool set_outline_flag) {
    if (ctx == nullptr || ctx->widget == nullptr)
        return 0;

    vdui_t *vu = get_widget_vdui(ctx->widget);
    if (vu == nullptr || vu->cfunc == nullptr)
        return 0;

    ea_t entry = vu->cfunc->entry_ea;
    func_t *pfn = get_func(entry);
    if (pfn == nullptr)
        return 0;

    bool current_outline = (pfn->flags & FUNC_OUTLINE) != 0;

    // Desired state already reached; nothing to do.
    if (current_outline == set_outline_flag)
        return 0;

    if (set_outline_flag)
        pfn->flags |= FUNC_OUTLINE;
    else
        pfn->flags &= ~FUNC_OUTLINE;

    if (!update_func(pfn)) {
        msg("[inline] Failed to update function flags at %a\n", pfn->start_ea);
        return 0;
    }

    // Flush decompiler caches for this function and all its callers.
    mark_callers_dirty(entry);

    // Force re-decompilation of the current function in this view.
    vu->refresh_view(true);

    // Non-zero return value tells IDA to refresh windows.
    return 1;
}

//------------------------------------------------------------------------------
// Action handlers
//------------------------------------------------------------------------------

struct inline_action_handler_t : action_handler_t {
    // If true, set FUNC_OUTLINE; if false, clear it.
    bool set_outline;

    inline_action_handler_t(bool set_outline_flag)
        : action_handler_t(0),
          set_outline(set_outline_flag) {
    }

    int idaapi activate(action_activation_ctx_t *ctx) override {
        return do_toggle_inline(ctx, set_outline);
    }

    action_state_t idaapi update(action_update_ctx_t *ctx) override {
        // Action only makes sense in pseudocode views.
        if (ctx == nullptr || ctx->widget == nullptr)
            return AST_DISABLE;

        if (ctx->widget_type != BWN_PSEUDOCODE)
            return AST_DISABLE;

        vdui_t *vu = get_widget_vdui(ctx->widget);
        if (vu == nullptr || vu->cfunc == nullptr)
            return AST_DISABLE;

        func_t *pfn = get_func(vu->cfunc->entry_ea);
        if (pfn == nullptr)
            return AST_DISABLE;

        bool is_outline = (pfn->flags & FUNC_OUTLINE) != 0;

        // "Mark as inline" only when outline flag is clear.
        // "Mark as outline" only when outline flag is set.
        if (set_outline && is_outline)
            return AST_DISABLE_FOR_WIDGET;

        if (!set_outline && !is_outline)
            return AST_DISABLE_FOR_WIDGET;

        return AST_ENABLE_FOR_WIDGET;
    }
};

// Heap-allocated handler instances - IDA will manage their lifetime
static inline_action_handler_t *g_mark_inline_ah = nullptr;
static inline_action_handler_t *g_mark_outline_ah = nullptr;

//------------------------------------------------------------------------------
// Component lifecycle (registered via component_registry_t)
//------------------------------------------------------------------------------

static bool inline_avail() {
    // Always available; Hex-Rays presence is already checked in plugin init.
    return true;
}

static bool inline_active() {
    // Consider the component active once actions are registered.
    return g_inline_actions_registered;
}

static void inline_init() {
    if (g_inline_actions_registered)
        return;

    // Allocate handlers - IDA takes ownership when actions are registered
    g_mark_inline_ah = new inline_action_handler_t(true);
    g_mark_outline_ah = new inline_action_handler_t(false);

    action_desc_t desc_inline;
    desc_inline.cb = sizeof(desc_inline);
    desc_inline.name = ACTION_MARK_INLINE;
    desc_inline.label = "Mark as inline";
    desc_inline.handler = g_mark_inline_ah;
    desc_inline.owner = nullptr;
    desc_inline.shortcut = nullptr;
    desc_inline.tooltip = "Mark function as inline (FUNC_OUTLINE) and clear decompiler caches for callers";
    desc_inline.icon = -1;
    desc_inline.flags = 0;

    action_desc_t desc_outline;
    desc_outline.cb = sizeof(desc_outline);
    desc_outline.name = ACTION_MARK_OUTLINE;
    desc_outline.label = "Mark as outline";
    desc_outline.handler = g_mark_outline_ah;
    desc_outline.owner = nullptr;
    desc_outline.shortcut = nullptr;
    desc_outline.tooltip = "Unmark function as inline (clear FUNC_OUTLINE) and clear decompiler caches for callers";
    desc_outline.icon = -1;
    desc_outline.flags = 0;

    bool ok1 = register_action(desc_inline);
    bool ok2 = register_action(desc_outline);

    g_inline_actions_registered = ok1 && ok2;

    if (!g_inline_actions_registered)
        msg("[inline] Failed to register inline toggle actions\n");
}

static void inline_done() {
    if (!g_inline_actions_registered)
        return;

    unregister_action(ACTION_MARK_INLINE);
    unregister_action(ACTION_MARK_OUTLINE);
    g_inline_actions_registered = false;
}

// Provide a short-name symbol for the registry macro (it must be an identifier)
static const char inline_short_name[] = "inline";

// Popup integration
void inline_component_attach_to_popup(TWidget *widget,
                                      TPopupMenu *popup,
                                      vdui_t *vu) {
    if (!g_inline_actions_registered)
        return;

    if (widget == nullptr || popup == nullptr || vu == nullptr || vu->cfunc == nullptr)
        return;

    func_t *pfn = get_func(vu->cfunc->entry_ea);
    if (pfn == nullptr)
        return;

    bool is_outline = (pfn->flags & FUNC_OUTLINE) != 0;
    const char *action_name = is_outline ? ACTION_MARK_OUTLINE : ACTION_MARK_INLINE;

    attach_action_to_popup(widget, popup, action_name);
}

// Auto-registration into the component registry used by lifter_plugin.cpp
REGISTER_COMPONENT(inline_avail,
                   inline_active,
                   inline_init,
                   inline_done,
                   inline_component_attach_to_popup,
                   "Inline Component",
                   inline_short_name,
                   "inline")
