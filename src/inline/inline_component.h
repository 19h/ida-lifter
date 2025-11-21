#pragma once

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include "../common/warn_on.h"

// Attach the inline toggle action to the Hex-Rays pseudocode popup menu
// for the given widget/view pair.
void inline_component_attach_to_popup(TWidget *widget,
                                      TPopupMenu *popup,
                                      vdui_t *vu);
