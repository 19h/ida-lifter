/*
 AVX Lifter Debug Utilities
*/

#include "avx_debug.h"

#if IDA_SDK_VERSION >= 750

#include "../common/warn_off.h"
#include <funcs.hpp>
#include <lines.hpp>
#include <ua.hpp>
#include "../common/warn_on.h"

bool g_print_debug_info = false;

void set_debug_printing(bool enabled) {
    g_print_debug_info = enabled;
    msg("[AVXLifter] Debug printing %s\n", enabled ? "ENABLED" : "DISABLED");
}

// Print disassembly for a function
void print_function_disassembly(ea_t func_ea) {
    if (!g_print_debug_info)
        return;

    // Safety check: validate address
    if (func_ea == BADADDR || func_ea == 0)
        return;

    func_t *pfn = get_func(func_ea);
    if (!pfn) {
        // Silently skip - this can happen during decompiler internal processing
        return;
    }

    qstring func_name;
    get_func_name(&func_name, pfn->start_ea);

    msg("\n");
    msg("================================================================================\n");
    msg("DISASSEMBLY: %s (at %a)\n", func_name.c_str(), pfn->start_ea);
    msg("================================================================================\n");

    // Iterate through all instructions in the function
    ea_t ea = pfn->start_ea;
    int line_count = 0;
    const int MAX_LINES = 200; // Limit output to prevent UI flooding

    while (ea < pfn->end_ea && line_count < MAX_LINES) {
        // Generate disassembly line
        qstring line;
        generate_disasm_line(&line, ea, GENDSM_FORCE_CODE);

        // Remove color tags using SDK qstring API
        qstring clean_line;
        tag_remove(&clean_line, line);

        // Print address and disassembly
        msg("%a: %s\n", ea, clean_line.c_str());

        // Move to next instruction
        ea = next_head(ea, pfn->end_ea);
        line_count++;
    }

    if (line_count >= MAX_LINES)
        msg("... (truncated at %d lines)\n", MAX_LINES);

    msg("================================================================================\n\n");
}

// Print microcode for a function at a specific maturity level
void print_function_microcode(mba_t *mba, const char *stage_name) {
    if (!g_print_debug_info || !mba || !stage_name)
        return;

    // Safety check: validate mba structure
    if (mba->entry_ea == BADADDR || mba->entry_ea == 0 || mba->qty < 0)
        return;

    qstring func_name;
    get_func_name(&func_name, mba->entry_ea);

    msg("\n");
    msg("================================================================================\n");
    msg("MICROCODE [%s]: %s (at %a)\n", stage_name, func_name.c_str(), mba->entry_ea);
    msg("================================================================================\n");
    msg("Maturity level: %d\n", mba->maturity);
    msg("Number of blocks: %d\n", mba->qty);
    msg("Number of instructions: %d\n", mba->qty > 0 ? mba->get_mblock(0)->tail ? 1 : 0 : 0);
    msg("--------------------------------------------------------------------------------\n");

    // Print summary of each block
    const int MAX_BLOCKS = 50; // Limit number of blocks printed
    int blocks_printed = 0;

    for (int i = 0; i < mba->qty && blocks_printed < MAX_BLOCKS; i++) {
        mblock_t *blk = mba->get_mblock(i);
        if (!blk)
            continue;

        msg("  Block %d: serial=%d\n", i, blk->serial);

        // Print instructions in this block
        minsn_t *insn = blk->head;
        int insn_count = 0;
        const int MAX_INSNS = 200; // Safety limit reduced to prevent flooding

        while (insn && insn_count < MAX_INSNS) {
            // Build instruction text using opcode name and basic info
            char buf[256];
            qsnprintf(buf, sizeof(buf), "op=%d ea=%a", insn->opcode, insn->ea);
            msg("    [%d] %s\n", insn_count, buf);

            insn_count++;
            insn = insn->next;
        }

        if (insn_count >= MAX_INSNS)
            msg("    ... (truncated at %d instructions)\n", MAX_INSNS);

        blocks_printed++;
    }

    if (blocks_printed >= MAX_BLOCKS)
        msg("  ... (truncated at %d blocks)\n", MAX_BLOCKS);

    msg("================================================================================\n\n");
}

#endif // IDA_SDK_VERSION >= 750
