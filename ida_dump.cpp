/**
 * IDA Pro Binary Analysis Dumper
 *
 * A headless tool for dumping assembly, microcode, and pseudocode from binaries.
 * Useful for testing IDA plugins (especially the AVX lifter) and analyzing binaries.
 *
 * Features:
 *   - Dump assembly, microcode, and/or pseudocode
 *   - Filter functions by name pattern or address
 *   - Show only functions with decompilation errors
 *   - Quiet mode to suppress IDA's verbose output
 *   - Summary statistics
 */

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cctype>
#include <cstring>
#include <algorithm>
#include <regex>

// IDA SDK Headers
#include <pro.h>
#include <ida.hpp>
#include <idp.hpp>
#include <auto.hpp>
#include <funcs.hpp>
#include <lines.hpp>
#include <name.hpp>
#include <loader.hpp>
#include <hexrays.hpp>
#include <idalib.hpp>
#include <segment.hpp>
#include <typeinf.hpp>
#include <nalt.hpp>

//=============================================================================
// Global State
//=============================================================================

hexdsp_t *hexdsp = nullptr;

//=============================================================================
// Command Line Options
//=============================================================================

struct Options {
    std::string input_file;
    std::string filter_pattern;      // Function name filter (regex)
    ea_t filter_address = BADADDR;   // Filter by specific address
    bool show_assembly = true;
    bool show_microcode = true;
    bool show_pseudocode = true;
    bool errors_only = false;        // Only show functions with errors
    bool quiet = false;              // Suppress IDA messages
    bool show_summary = true;        // Show summary at end
    bool list_functions = false;     // Just list function names
    bool verbose = false;            // Show extra metadata
};

static Options g_opts;

// Statistics
struct Stats {
    size_t total_functions = 0;
    size_t decompiled_ok = 0;
    size_t decompiled_fail = 0;
    size_t skipped = 0;
    std::vector<std::pair<std::string, std::string>> errors; // (func_name, error)
};

static Stats g_stats;

//=============================================================================
// ANSI Colors (for terminal output)
//=============================================================================

namespace Color {
    const char* Reset   = "\033[0m";
    const char* Bold    = "\033[1m";
    const char* Dim     = "\033[2m";
    const char* Red     = "\033[31m";
    const char* Green   = "\033[32m";
    const char* Yellow  = "\033[33m";
    const char* Blue    = "\033[34m";
    const char* Magenta = "\033[35m";
    const char* Cyan    = "\033[36m";
    const char* White   = "\033[37m";

    // Check if we should use colors
    bool enabled = true;

    void disable() { enabled = false; }

    const char* get(const char* color) {
        return enabled ? color : "";
    }
}

#define CLR(c) Color::get(Color::c)

//=============================================================================
// Utilities
//=============================================================================

static std::string format_address(ea_t ea) {
    qstring text;
    ea2str(&text, ea);
    return text.c_str();
}

static std::string format_size(uint64_t bytes) {
    if (bytes < 1024) return std::to_string(bytes) + " B";
    if (bytes < 1024 * 1024) return std::to_string(bytes / 1024) + " KB";
    return std::to_string(bytes / (1024 * 1024)) + " MB";
}

static bool is_spacer_line(const char *str) {
    while (*str && std::isspace((unsigned char)*str)) ++str;
    if (!std::isdigit((unsigned char)*str)) return false;
    while (std::isdigit((unsigned char)*str)) ++str;
    if (*str != '.') return false;
    ++str;
    if (!std::isspace((unsigned char)*str)) return false;
    while (*str && std::isspace((unsigned char)*str)) ++str;
    if (!std::isdigit((unsigned char)*str)) return false;
    while (std::isdigit((unsigned char)*str)) ++str;
    while (*str && std::isspace((unsigned char)*str)) ++str;
    return *str == '\0';
}

static bool matches_filter(const char* func_name, ea_t func_addr) {
    // Address filter takes precedence
    if (g_opts.filter_address != BADADDR) {
        return func_addr == g_opts.filter_address;
    }

    // Name pattern filter
    if (!g_opts.filter_pattern.empty()) {
        try {
            std::regex pattern(g_opts.filter_pattern, std::regex::icase);
            return std::regex_search(func_name, pattern);
        } catch (const std::regex_error&) {
            // Fall back to substring match
            std::string name_lower(func_name);
            std::string pattern_lower(g_opts.filter_pattern);
            std::transform(name_lower.begin(), name_lower.end(), name_lower.begin(), ::tolower);
            std::transform(pattern_lower.begin(), pattern_lower.end(), pattern_lower.begin(), ::tolower);
            return name_lower.find(pattern_lower) != std::string::npos;
        }
    }

    return true;
}

//=============================================================================
// Output Handling
//=============================================================================

class MicrocodePrinter : public vd_printer_t {
private:
    std::ostream &m_out;

public:
    explicit MicrocodePrinter(std::ostream &stream) : m_out(stream) {}

    AS_PRINTF(3, 4) int print(int indent, const char *format, ...) override {
        qstring line;
        if (indent > 0) line.fill(0, ' ', indent);

        va_list va;
        va_start(va, format);
        line.cat_vsprnt(format, va);
        va_end(va);

        tag_remove(&line);

        size_t len = line.length();
        while (len > 0 && std::isspace((unsigned char)line[len-1])) len--;
        line.resize(len);

        if (line.empty()) return 0;

        bool is_empty = true;
        for (size_t i = 0; i < len; ++i) {
            if (!std::isspace((unsigned char)line[i])) {
                is_empty = false;
                break;
            }
        }
        if (is_empty) return 0;
        if (is_spacer_line(line.c_str())) return 0;

        m_out << line.c_str() << std::endl;
        return static_cast<int>(line.length());
    }
};

// Null output stream for suppressing IDA messages
class NullBuffer : public std::streambuf {
public:
    int overflow(int c) override { return c; }
};

//=============================================================================
// Binary Information
//=============================================================================

static void print_binary_info() {
    std::cout << "\n";
    std::cout << CLR(Bold) << "Binary Information" << CLR(Reset) << "\n";
    std::cout << std::string(78, '-') << "\n";

    // File info
    char filepath[QMAXPATH];
    get_input_file_path(filepath, sizeof(filepath));
    std::cout << "  " << CLR(Cyan) << "File:      " << CLR(Reset) << filepath << "\n";

    // File format
    char fileformat[64];
    get_file_type_name(fileformat, sizeof(fileformat));
    std::cout << "  " << CLR(Cyan) << "Format:    " << CLR(Reset) << fileformat << "\n";

    // Processor
    qstring procname = inf_get_procname();
    std::cout << "  " << CLR(Cyan) << "Processor: " << CLR(Reset) << procname.c_str() << "\n";

    // Bitness
    const char* bits = inf_is_64bit() ? "64-bit" : (inf_is_32bit_exactly() ? "32-bit" : "16-bit");
    std::cout << "  " << CLR(Cyan) << "Bitness:   " << CLR(Reset) << bits << "\n";

    // Compiler
    compiler_info_t ci;
    inf_get_cc(&ci);
    const char* cc_name = "Unknown";
    switch (ci.id) {
        case COMP_MS:      cc_name = "Visual C++"; break;
        case COMP_BC:      cc_name = "Borland C++"; break;
        case COMP_WATCOM:  cc_name = "Watcom C++"; break;
        case COMP_GNU:     cc_name = "GNU C++"; break;
        case COMP_VISAGE:  cc_name = "Visual Age C++"; break;
        case COMP_BP:      cc_name = "Delphi"; break;
        default: break;
    }
    std::cout << "  " << CLR(Cyan) << "Compiler:  " << CLR(Reset) << cc_name << "\n";

    // Entry point
    ea_t entry = inf_get_start_ea();
    if (entry != BADADDR) {
        std::cout << "  " << CLR(Cyan) << "Entry:     " << CLR(Reset) << format_address(entry) << "\n";
    }

    // Segments summary
    int seg_count = get_segm_qty();
    uint64_t total_size = 0;
    for (int i = 0; i < seg_count; i++) {
        segment_t* seg = getnseg(i);
        if (seg) total_size += seg->size();
    }
    std::cout << "  " << CLR(Cyan) << "Segments:  " << CLR(Reset) << seg_count
              << " (" << format_size(total_size) << " total)" << "\n";

    // Functions
    std::cout << "  " << CLR(Cyan) << "Functions: " << CLR(Reset) << get_func_qty() << "\n";

    std::cout << "\n";
}

static void print_segments() {
    std::cout << CLR(Bold) << "Segments" << CLR(Reset) << "\n";
    std::cout << std::string(78, '-') << "\n";

    printf("  %-16s  %-16s  %10s  %-8s  %s\n", "Start", "End", "Size", "Perm", "Name");
    printf("  %s  %s  %s  %s  %s\n",
           std::string(16, '-').c_str(),
           std::string(16, '-').c_str(),
           std::string(10, '-').c_str(),
           std::string(8, '-').c_str(),
           std::string(20, '-').c_str());

    int seg_count = get_segm_qty();
    for (int i = 0; i < seg_count; i++) {
        segment_t* seg = getnseg(i);
        if (!seg) continue;

        qstring seg_name;
        get_segm_name(&seg_name, seg);

        char perms[5] = "----";
        if (seg->perm & SEGPERM_READ)  perms[0] = 'r';
        if (seg->perm & SEGPERM_WRITE) perms[1] = 'w';
        if (seg->perm & SEGPERM_EXEC)  perms[2] = 'x';

        printf("  %-16s  %-16s  %10s  %-8s  %s\n",
               format_address(seg->start_ea).c_str(),
               format_address(seg->end_ea).c_str(),
               format_size(seg->size()).c_str(),
               perms,
               seg_name.c_str());
    }
    std::cout << "\n";
}

//=============================================================================
// Function Dumping
//=============================================================================

class FunctionDumper {
public:
    // Check if function is in a special/extern segment that can't be decompiled
    static bool is_special_segment(func_t *pfn) {
        segment_t* seg = getseg(pfn->start_ea);
        if (!seg) return false;

        qstring seg_name;
        get_segm_name(&seg_name, seg);

        // Check for extern segment
        if (seg_name == "extern") return true;

        // Check segment type
        if (seg->type == SEG_XTRN) return true;

        return false;
    }

    // Returns true if decompilation succeeded
    static bool dump(func_t *pfn) {
        if (!pfn) return false;

        qstring fname;
        get_func_name(&fname, pfn->start_ea);

        // Check filter
        if (!matches_filter(fname.c_str(), pfn->start_ea)) {
            g_stats.skipped++;
            return true;
        }

        // Skip special segments entirely - they can't be decompiled
        bool is_special = is_special_segment(pfn);
        if (is_special) {
            g_stats.skipped++;
            return true;
        }

        // Try decompilation first to check for errors
        hexrays_failure_t hf;
        cfuncptr_t cfunc = decompile(pfn, &hf, DECOMP_WARNINGS);

        bool success = (cfunc != nullptr);

        // Check if this is a "special segments cannot be decompiled" error
        // These are expected and should not count as real errors
        std::string err_msg;
        if (!success) {
            err_msg = hf.desc().c_str();
            if (err_msg.find("special segment") != std::string::npos ||
                err_msg.find("call analysis failed") != std::string::npos) {
                // Skip these non-errors
                g_stats.skipped++;
                return true;
            }
        }

        // In errors_only mode, skip successful functions
        if (g_opts.errors_only && success) {
            g_stats.skipped++;
            return true;
        }

        // Print function header
        print_header(pfn, fname.c_str(), success);

        if (!success) {
            g_stats.decompiled_fail++;
            g_stats.errors.push_back({fname.c_str(), err_msg});

            std::cout << CLR(Red) << "  [ERROR] " << CLR(Reset)
                      << "Decompilation failed at " << format_address(hf.errea)
                      << ": " << err_msg << "\n\n";

            // Still show assembly if requested
            if (g_opts.show_assembly) {
                dump_assembly(pfn);
            }
            return false;
        }

        g_stats.decompiled_ok++;

        if (g_opts.show_assembly) dump_assembly(pfn);
        if (g_opts.show_microcode) dump_microcode((cfunc_t*)cfunc);
        if (g_opts.show_pseudocode) dump_pseudocode((cfunc_t*)cfunc);

        return true;
    }

    static void list(func_t *pfn) {
        if (!pfn) return;

        qstring fname;
        get_func_name(&fname, pfn->start_ea);

        if (!matches_filter(fname.c_str(), pfn->start_ea)) return;

        // Get function flags
        std::string flags;
        if (pfn->flags & FUNC_NORET) flags += "noret ";
        if (pfn->flags & FUNC_LIB) flags += "lib ";
        if (pfn->flags & FUNC_THUNK) flags += "thunk ";
        if (pfn->flags & FUNC_LUMINA) flags += "lumina ";

        printf("  %-16s  %6zu  %-20s  %s\n",
               format_address(pfn->start_ea).c_str(),
               (size_t)(pfn->end_ea - pfn->start_ea),
               flags.c_str(),
               fname.c_str());
    }

private:
    static void print_header(func_t *pfn, const char* fname, bool success) {
        // Get segment name
        qstring seg_name;
        segment_t* seg = getseg(pfn->start_ea);
        if (seg) get_segm_name(&seg_name, seg);

        std::cout << "\n";
        std::cout << CLR(Bold) << std::string(78, '=') << CLR(Reset) << "\n";

        // Function name with status indicator
        const char* status_color = success ? CLR(Green) : CLR(Red);
        const char* status_icon = success ? "[OK]" : "[FAIL]";

        std::cout << CLR(Bold) << "Function: " << CLR(Cyan) << fname << CLR(Reset);
        if (!seg_name.empty()) {
            std::cout << " " << CLR(Dim) << "(" << seg_name.c_str() << ":"
                      << format_address(pfn->start_ea) << ")" << CLR(Reset);
        }
        std::cout << "  " << status_color << status_icon << CLR(Reset) << "\n";

        // Function metadata
        if (g_opts.verbose) {
            std::cout << CLR(Dim);
            std::cout << "  Size: " << (pfn->end_ea - pfn->start_ea) << " bytes";
            std::cout << "  | Range: " << format_address(pfn->start_ea)
                      << " - " << format_address(pfn->end_ea);

            // Flags
            std::vector<std::string> flag_names;
            if (pfn->flags & FUNC_NORET) flag_names.push_back("noreturn");
            if (pfn->flags & FUNC_LIB) flag_names.push_back("library");
            if (pfn->flags & FUNC_THUNK) flag_names.push_back("thunk");
            if (pfn->flags & FUNC_FRAME) flag_names.push_back("frame");

            if (!flag_names.empty()) {
                std::cout << "  | Flags: ";
                for (size_t i = 0; i < flag_names.size(); i++) {
                    if (i > 0) std::cout << ", ";
                    std::cout << flag_names[i];
                }
            }
            std::cout << CLR(Reset) << "\n";
        }

        std::cout << std::string(78, '-') << "\n";
    }

    static void dump_assembly(func_t *pfn) {
        std::cout << CLR(Yellow) << "-- Assembly " << CLR(Dim) << std::string(65, '-') << CLR(Reset) << "\n";

        func_item_iterator_t fii;
        for (bool ok = fii.set(pfn); ok; ok = fii.next_code()) {
            ea_t ea = fii.current();
            qstring line;

            if (generate_disasm_line(&line, ea, GENDSM_REMOVE_TAGS | GENDSM_MULTI_LINE | GENDSM_FORCE_CODE)) {
                line.trim2();
                std::cout << CLR(Dim) << format_address(ea) << CLR(Reset) << "  " << line.c_str() << "\n";
            }
        }
        std::cout << "\n";
    }

    static void dump_microcode(cfunc_t *cfunc) {
        std::cout << CLR(Yellow) << "-- Microcode " << CLR(Dim) << std::string(64, '-') << CLR(Reset) << "\n";

        if (mba_t *mba = cfunc->mba) {
            MicrocodePrinter printer(std::cout);
            mba->print(printer);
        } else {
            std::cout << CLR(Dim) << "  (No microcode available)" << CLR(Reset) << "\n";
        }
        std::cout << "\n";
    }

    static void dump_pseudocode(cfunc_t *cfunc) {
        std::cout << CLR(Yellow) << "-- Pseudocode " << CLR(Dim) << std::string(63, '-') << CLR(Reset) << "\n";

        const strvec_t &sv = cfunc->get_pseudocode();
        for (size_t i = 0; i < sv.size(); ++i) {
            qstring line;
            tag_remove(&line, sv[i].line);
            line.trim2();
            std::cout << line.c_str() << "\n";
        }
        std::cout << "\n";
    }
};

//=============================================================================
// Summary
//=============================================================================

static void print_summary() {
    std::cout << "\n";
    std::cout << CLR(Bold) << std::string(78, '=') << CLR(Reset) << "\n";
    std::cout << CLR(Bold) << "Summary" << CLR(Reset) << "\n";
    std::cout << std::string(78, '-') << "\n";

    std::cout << "  Total functions:    " << g_stats.total_functions << "\n";
    std::cout << "  " << CLR(Green) << "Decompiled OK:    " << CLR(Reset) << g_stats.decompiled_ok << "\n";

    if (g_stats.decompiled_fail > 0) {
        std::cout << "  " << CLR(Red) << "Decompilation failed: " << CLR(Reset) << g_stats.decompiled_fail << "\n";
    }

    if (g_stats.skipped > 0) {
        std::cout << "  " << CLR(Dim) << "Skipped (filtered):   " << CLR(Reset) << g_stats.skipped << "\n";
    }

    // Success rate
    size_t attempted = g_stats.decompiled_ok + g_stats.decompiled_fail;
    if (attempted > 0) {
        double rate = 100.0 * g_stats.decompiled_ok / attempted;
        const char* color = rate == 100.0 ? CLR(Green) : (rate >= 90.0 ? CLR(Yellow) : CLR(Red));
        printf("  %sSuccess rate:     %.1f%%%s\n", color, rate, CLR(Reset));
    }

    // List errors
    if (!g_stats.errors.empty()) {
        std::cout << "\n" << CLR(Red) << "Errors:" << CLR(Reset) << "\n";
        for (const auto& err : g_stats.errors) {
            std::cout << "  " << CLR(Cyan) << err.first << CLR(Reset)
                      << ": " << err.second << "\n";
        }
    }

    std::cout << std::string(78, '=') << "\n";
}

//=============================================================================
// Resource Management
//=============================================================================

class HeadlessIdaContext {
public:
    HeadlessIdaContext(const char *input_file) {
        if (init_library() != 0) {
            throw std::runtime_error("Failed to initialize IDA library.");
        }

        enable_console_messages(!g_opts.quiet);

        if (open_database(input_file, true) != 0) {
            throw std::runtime_error(std::string("Failed to open: ") + input_file);
        }

        if (!g_opts.quiet) {
            std::cout << "[*] Waiting for auto-analysis..." << std::endl;
        }
        auto_wait();
        if (!g_opts.quiet) {
            std::cout << "[*] Analysis complete." << std::endl;
        }

        if (!init_hexrays_plugin()) {
            set_database_flag(DBFL_KILL);
            term_database();
            throw std::runtime_error("Hex-Rays decompiler not available.");
        }
    }

    ~HeadlessIdaContext() {
        term_hexrays_plugin();
        set_database_flag(DBFL_KILL);
        term_database();
    }

    HeadlessIdaContext(const HeadlessIdaContext&) = delete;
    HeadlessIdaContext& operator=(const HeadlessIdaContext&) = delete;
};

//=============================================================================
// Usage
//=============================================================================

static void print_usage(const char* prog) {
    std::cout << CLR(Bold) << "IDA Pro Binary Analysis Dumper" << CLR(Reset) << "\n\n";
    std::cout << CLR(Cyan) << "Usage:" << CLR(Reset) << " " << prog << " [options] <binary_file>\n\n";
    std::cout << CLR(Cyan) << "Options:" << CLR(Reset) << "\n";
    std::cout << "  -f, --filter <pattern>   Filter functions by name (regex)\n";
    std::cout << "  -a, --address <addr>     Show only function at address (hex)\n";
    std::cout << "  -e, --errors             Show only functions with decompilation errors\n";
    std::cout << "  -l, --list               List functions only (no decompilation)\n";
    std::cout << "  -q, --quiet              Suppress IDA's verbose messages\n";
    std::cout << "  -v, --verbose            Show extra metadata for each function\n";
    std::cout << "  --no-asm                 Don't show assembly\n";
    std::cout << "  --no-mc                  Don't show microcode\n";
    std::cout << "  --no-pseudo              Don't show pseudocode\n";
    std::cout << "  --asm-only               Show only assembly\n";
    std::cout << "  --mc-only                Show only microcode\n";
    std::cout << "  --pseudo-only            Show only pseudocode\n";
    std::cout << "  --no-color               Disable colored output\n";
    std::cout << "  --no-summary             Don't show summary at end\n";
    std::cout << "  -h, --help               Show this help\n";
    std::cout << "\n";
    std::cout << CLR(Cyan) << "Examples:" << CLR(Reset) << "\n";
    std::cout << "  " << prog << " program.exe                    # Dump all functions\n";
    std::cout << "  " << prog << " -f main program.exe            # Only 'main' function\n";
    std::cout << "  " << prog << " -f 'test_.*' program.exe       # Functions matching regex\n";
    std::cout << "  " << prog << " -a 0x1234 program.exe          # Function at address\n";
    std::cout << "  " << prog << " -e program.exe                 # Only show errors\n";
    std::cout << "  " << prog << " -l program.exe                 # List functions\n";
    std::cout << "  " << prog << " --pseudo-only -q program.exe   # Quiet, pseudocode only\n";
    std::cout << "\n";
}

static bool parse_args(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            exit(0);
        }
        else if (arg == "-f" || arg == "--filter") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --filter requires an argument\n";
                return false;
            }
            g_opts.filter_pattern = argv[++i];
        }
        else if (arg == "-a" || arg == "--address") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --address requires an argument\n";
                return false;
            }
            g_opts.filter_address = strtoull(argv[++i], nullptr, 16);
        }
        else if (arg == "-e" || arg == "--errors") {
            g_opts.errors_only = true;
        }
        else if (arg == "-l" || arg == "--list") {
            g_opts.list_functions = true;
        }
        else if (arg == "-q" || arg == "--quiet") {
            g_opts.quiet = true;
        }
        else if (arg == "-v" || arg == "--verbose") {
            g_opts.verbose = true;
        }
        else if (arg == "--no-asm") {
            g_opts.show_assembly = false;
        }
        else if (arg == "--no-mc") {
            g_opts.show_microcode = false;
        }
        else if (arg == "--no-pseudo") {
            g_opts.show_pseudocode = false;
        }
        else if (arg == "--asm-only") {
            g_opts.show_assembly = true;
            g_opts.show_microcode = false;
            g_opts.show_pseudocode = false;
        }
        else if (arg == "--mc-only") {
            g_opts.show_assembly = false;
            g_opts.show_microcode = true;
            g_opts.show_pseudocode = false;
        }
        else if (arg == "--pseudo-only") {
            g_opts.show_assembly = false;
            g_opts.show_microcode = false;
            g_opts.show_pseudocode = true;
        }
        else if (arg == "--no-color") {
            Color::disable();
        }
        else if (arg == "--no-summary") {
            g_opts.show_summary = false;
        }
        else if (arg[0] == '-') {
            std::cerr << "Unknown option: " << arg << "\n";
            return false;
        }
        else {
            if (g_opts.input_file.empty()) {
                g_opts.input_file = arg;
            } else {
                std::cerr << "Error: Multiple input files specified\n";
                return false;
            }
        }
    }

    if (g_opts.input_file.empty()) {
        std::cerr << "Error: No input file specified\n";
        return false;
    }

    return true;
}

//=============================================================================
// Main
//=============================================================================

int main(int argc, char *argv[]) {
    if (!parse_args(argc, argv)) {
        std::cerr << "Use --help for usage information\n";
        return EXIT_FAILURE;
    }

    try {
        HeadlessIdaContext ctx(g_opts.input_file.c_str());

        // Print binary info unless in quiet mode
        if (!g_opts.quiet) {
            print_binary_info();
            if (g_opts.verbose) {
                print_segments();
            }
        }

        g_stats.total_functions = get_func_qty();

        if (!g_opts.quiet) {
            std::cout << "[*] Processing " << g_stats.total_functions << " functions...\n";
        }

        if (g_opts.list_functions) {
            // List mode - just show function names
            printf("\n  %-16s  %6s  %-20s  %s\n", "Address", "Size", "Flags", "Name");
            printf("  %s  %s  %s  %s\n",
                   std::string(16, '-').c_str(),
                   std::string(6, '-').c_str(),
                   std::string(20, '-').c_str(),
                   std::string(30, '-').c_str());

            for (size_t i = 0; i < g_stats.total_functions; ++i) {
                func_t *pfn = getn_func(i);
                if (pfn) FunctionDumper::list(pfn);
            }
            std::cout << "\n";
        } else {
            // Normal mode - dump functions
            for (size_t i = 0; i < g_stats.total_functions; ++i) {
                func_t *pfn = getn_func(i);
                if (pfn) FunctionDumper::dump(pfn);
            }
        }

        if (g_opts.show_summary && !g_opts.list_functions) {
            print_summary();
        }

        if (!g_opts.quiet) {
            std::cout << "[*] Done.\n";
        }
    }
    catch (const std::exception &e) {
        std::cerr << CLR(Red) << "[FATAL] " << CLR(Reset) << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return g_stats.decompiled_fail > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
