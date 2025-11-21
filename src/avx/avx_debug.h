/*
    AVX Lifter Debug Utilities
*/

#pragma once

#include "../common/warn_off.h"
#include <hexrays.hpp>
#include "../common/warn_on.h"

#if IDA_SDK_VERSION >= 750

// Print disassembly for a function
void print_function_disassembly(ea_t func_ea);

// Print microcode for a function at a specific maturity level
void print_function_microcode(mba_t *mba, const char *stage_name);

// Global flag to control debug printing
extern bool g_print_debug_info;

// Set debug printing on/off
void set_debug_printing(bool enabled);

#endif // IDA_SDK_VERSION >= 750
