#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cctype> // For isdigit, isspace

// IDA SDK Headers
// 'pro.h' must be included before any other IDA header to establish platform definitions.
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

//-------------------------------------------------------------------------
// Global Definitions
//-------------------------------------------------------------------------

// Hex-Rays API pointer definition.
// Required for standalone applications using the Hex-Rays API.
// The SDK header declares this as extern; we must define the storage.
// This pointer is populated by init_hexrays_plugin().
hexdsp_t *hexdsp = nullptr;

//-------------------------------------------------------------------------
// Utilities
//-------------------------------------------------------------------------

/**
 * @brief Formats an effective address (ea_t) into a hexadecimal string.
 *
 * @param ea The effective address to format.
 * @return std::string The formatted address string.
 */
static std::string format_address(ea_t ea)
{
  qstring text;
  ea2str(&text, ea);
  return text.c_str();
}

/**
 * @brief Checks if a microcode line is just a spacer (Block.Index) with no content.
 *
 * Raw MBA dump often produces lines like "0. 0" which are just indices.
 * We want to filter these out to make the output cleaner.
 *
 * @param str The line to check.
 * @return true If the line is just indices.
 */
static bool is_spacer_line(const char *str)
{
  // Skip leading whitespace
  while ( *str && std::isspace((unsigned char)*str) ) ++str;

  // Expect Digit (Block Index)
  if ( !std::isdigit((unsigned char)*str) ) return false;
  while ( std::isdigit((unsigned char)*str) ) ++str;

  // Expect Dot
  if ( *str != '.' ) return false;
  ++str;

  // Expect Space(s)
  if ( !std::isspace((unsigned char)*str) ) return false;
  while ( *str && std::isspace((unsigned char)*str) ) ++str;

  // Expect Digit (Instruction Index)
  if ( !std::isdigit((unsigned char)*str) ) return false;
  while ( std::isdigit((unsigned char)*str) ) ++str;

  // Expect End or Trailing Whitespace
  while ( *str && std::isspace((unsigned char)*str) ) ++str;

  return *str == '\0';
}

//-------------------------------------------------------------------------
// Output Handling
//-------------------------------------------------------------------------

/**
 * @brief Implements the Hex-Rays virtual printer interface with noise filtering.
 *
 * This class handles the formatting of microcode, stripping internal tags,
 * removing trailing whitespace, and suppressing empty spacer lines.
 */
class MicrocodePrinter : public vd_printer_t
{
private:
  std::ostream &m_out;

public:
  explicit MicrocodePrinter(std::ostream &stream) : m_out(stream) {}

  /**
   * @brief Callback for printing formatted text.
   *
   * @param indent Indentation level.
   * @param format Printf-style format string.
   * @param ... Variable arguments.
   * @return int Number of characters printed.
   */
  AS_PRINTF(3, 4) int print(int indent, const char *format, ...) override
  {
    qstring line;

    // Apply indentation
    if ( indent > 0 )
      line.fill(0, ' ', indent);

    // Format the string
    va_list va;
    va_start(va, format);
    line.cat_vsprnt(format, va);
    va_end(va);

    // Remove internal IDA color tags for clean text output
    tag_remove(&line);

    // Manual right-trim using standard isspace to ensure all whitespace types are caught
    size_t len = line.length();
    while ( len > 0 && std::isspace((unsigned char)line[len-1]) )
      len--;
    line.resize(len);

    // Skip empty lines
    if ( line.empty() )
      return 0;

    // Check if line is just whitespace (e.g. indentation only)
    bool is_empty_content = true;
    for ( size_t i = 0; i < len; ++i ) {
        if ( !std::isspace((unsigned char)line[i]) ) {
            is_empty_content = false;
            break;
        }
    }
    if ( is_empty_content )
      return 0;

    // Filter out lines that are just "BlockIdx. InsnIdx" with no content.
    if ( is_spacer_line(line.c_str()) )
      return 0;

    m_out << line.c_str() << std::endl;
    return static_cast<int>(line.length());
  }
};

//-------------------------------------------------------------------------
// Resource Management
//-------------------------------------------------------------------------

/**
 * @brief RAII Wrapper for IDA Library and Hex-Rays Plugin initialization.
 *
 * Ensures that the library is initialized, the database is opened/closed correctly,
 * and the Hex-Rays plugin is loaded/unloaded safely.
 */
class HeadlessIdaContext
{
public:
  /**
   * @brief Initializes IDA, opens the database, and loads Hex-Rays.
   *
   * @param argc Argument count.
   * @param argv Argument vector.
   * @param input_file Path to the binary file to analyze.
   * @throws std::runtime_error If initialization fails at any stage.
   */
  HeadlessIdaContext(int argc, char *argv[], const char *input_file)
  {
    // 1. Initialize IDA Library
    // We pass 0 arguments to ensure standard headless behavior, similar to idacli.
    // Passing argc/argv can trigger IDA's internal command line parsing which might
    // alter plugin loading or batch mode behavior.
    if ( init_library() != 0 )
    {
      throw std::runtime_error("Failed to initialize IDA library.");
    }

    // Enable console messages for visibility into analysis progress
    enable_console_messages(true);

    // 2. Open Database
    // 'true' triggers auto-analysis immediately upon load.
    if ( open_database(input_file, true) != 0 )
    {
      throw std::runtime_error(std::string("Failed to open input file: ") + input_file);
    }

    // 3. Wait for Analysis
    // Essential to ensure function boundaries and code/data cross-refs are established.
    std::cout << "[*] Waiting for auto-analysis to complete..." << std::endl;
    auto_wait();
    std::cout << "[*] Analysis complete." << std::endl;

    // 4. Initialize Hex-Rays Plugin
    // This populates the global 'hexdsp' pointer.
    if ( !init_hexrays_plugin() )
    {
      // Clean up database before throwing
      set_database_flag(DBFL_KILL);
      term_database();
      throw std::runtime_error("Hex-Rays decompiler is not available or license check failed.");
    }
  }

  /**
   * @brief Terminates the Hex-Rays plugin and closes the database.
   *
   * Sets DBFL_KILL to ensure no changes are saved to the IDB/I64 file,
   * preserving the original state of the analysis database.
   */
  ~HeadlessIdaContext()
  {
    term_hexrays_plugin();
    set_database_flag(DBFL_KILL);
    term_database();
  }

  // Disable copying
  HeadlessIdaContext(const HeadlessIdaContext&) = delete;
  HeadlessIdaContext& operator=(const HeadlessIdaContext&) = delete;
};

//-------------------------------------------------------------------------
// Analysis Logic
//-------------------------------------------------------------------------

/**
 * @brief Encapsulates logic for dumping function information.
 */
class FunctionDumper
{
public:
  /**
   * @brief Dumps Assembly, Microcode, and Pseudocode for a given function.
   *
   * @param pfn Pointer to the function structure.
   */
  static void dump(func_t *pfn)
  {
    if ( !pfn )
      return;

    qstring fname;
    // API Note: get_func_name takes 2 arguments in modern SDKs (buffer, address)
    get_func_name(&fname, pfn->start_ea);

    std::cout << "================================================================================" << std::endl;
    std::cout << "Function: " << fname.c_str() << " (" << format_address(pfn->start_ea) << ")" << std::endl;
    std::cout << "================================================================================" << std::endl;

    dump_assembly(pfn);

    // Decompile to get access to both Microcode (MBA) and Pseudocode (CTree)
    // decompile() requires a non-const func_t* as it may update the function cache.
    hexrays_failure_t hf;
    cfuncptr_t cfunc = decompile(pfn, &hf, DECOMP_WARNINGS);

    if ( cfunc == nullptr )
    {
      std::cerr << "[!] Decompilation failed at " << format_address(hf.errea)
                << ": " << hf.desc().c_str() << std::endl << std::endl;
      return;
    }

    // cfuncptr_t is a smart pointer (qrefcnt_t). We pass the raw pointer to helpers.
    // We cast to cfunc_t* because helpers need non-const access for some methods.
    dump_microcode((cfunc_t*)cfunc);
    dump_pseudocode((cfunc_t*)cfunc);
  }

private:
  static void dump_assembly(func_t *pfn)
  {
    std::cout << "--- Assembly -------------------------------------------------------------------" << std::endl;

    // func_item_iterator_t requires non-const func_t*
    func_item_iterator_t fii;
    for ( bool ok = fii.set(pfn); ok; ok = fii.next_code() )
    {
      ea_t ea = fii.current();
      qstring line;

      // GENDSM_FORCE_CODE: Ensure we treat it as code
      // GENDSM_MULTI_LINE: Handle complex instructions spanning lines
      // GENDSM_REMOVE_TAGS: Get raw text
      if ( generate_disasm_line(&line, ea, GENDSM_REMOVE_TAGS | GENDSM_MULTI_LINE | GENDSM_FORCE_CODE) )
      {
        // Clean up the line
        line.trim2();
        std::cout << format_address(ea) << ": " << line.c_str() << std::endl;
      }
    }
    std::cout << std::endl;
  }

  static void dump_microcode(cfunc_t *cfunc)
  {
    std::cout << "--- Microcode ------------------------------------------------------------------" << std::endl;

    // Access the public 'mba' member directly.
    if ( mba_t *mba = cfunc->mba )
    {
      MicrocodePrinter printer(std::cout);
      mba->print(printer);
    }
    else
    {
      std::cout << "[!] No microcode available." << std::endl;
    }
    std::cout << std::endl;
  }

  static void dump_pseudocode(cfunc_t *cfunc)
  {
    std::cout << "--- Pseudocode -----------------------------------------------------------------" << std::endl;

    // get_pseudocode() is non-const as it may generate/cache the text.
    const strvec_t &sv = cfunc->get_pseudocode();
    for ( size_t i = 0; i < sv.size(); ++i )
    {
      qstring line;
      // Pseudocode lines contain color tags; strip them for display
      tag_remove(&line, sv[i].line);

      // Trim trailing whitespace only (preserve indentation)
      line.trim2();

      std::cout << line.c_str() << std::endl;
    }
    std::cout << std::endl;
  }
};

//-------------------------------------------------------------------------
// Main Entry Point
//-------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  if ( argc < 2 )
  {
    std::cerr << "Usage: " << argv[0] << " <binary_file> [ida_args...]" << std::endl;
    return EXIT_FAILURE;
  }

  const char *input_path = argv[1];

  std::cout << "--------------------------------------------------------------------------------" << std::endl;
  std::cout << "   IDA Pro Binary Analysis Dumper (Assembly / Microcode / Pseudocode)           " << std::endl;
  std::cout << "--------------------------------------------------------------------------------" << std::endl;

  try
  {
    // Initialize IDA Context (Library, Database, Hex-Rays)
    HeadlessIdaContext ctx(argc, argv, input_path);

    const size_t func_qty = get_func_qty();
    std::cout << "[*] Processing " << func_qty << " functions..." << std::endl;

    for ( size_t i = 0; i < func_qty; ++i )
    {
      // getn_func returns a non-const func_t*
      func_t *pfn = getn_func(i);
      if ( pfn != nullptr )
      {
        FunctionDumper::dump(pfn);
      }
    }

    std::cout << "[*] Processing complete." << std::endl;
  }
  catch ( const std::exception &e )
  {
    std::cerr << "[FATAL] " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
