/*
    AVX Type Management Utilities
*/

#include "avx_types.h"

#if IDA_SDK_VERSION >= 750

#include "../common/warn_off.h"
#include <typeinf.hpp>
#include "../common/warn_on.h"

bool debug_logging_enabled = false;

/*
Resolves or synthesizes vector types (e.g., __m128, __m256i).
CRITICAL: This function ensures the microcode engine receives valid type information.
Passing instructions with unknown/void types for data flow can lead to kernel assertions.
*/
tinfo_t get_vector_type(int size_bytes, bool is_int, bool is_double)
{
  qstring type_name;
  type_name.cat_sprnt("__m%d", size_bytes * 8);
  if (is_int) type_name.append('i');
  else if (is_double) type_name.append('d');

  tinfo_t ti;
  if (ti.get_named_type(nullptr, type_name.c_str())) {
      // CRITICAL FIX: Ensure the type has the expected size.
      // If the type is a forward declaration or invalid, get_size() returns BADSIZE.
      // We must not use such types for microcode operands as they cause INTERR 50312.
      size_t sz = ti.get_size();
      if (sz == size_bytes) {
          return ti;
      }
      if (debug_logging_enabled) {
          msg("[AVXLifter] Warning: Type '%s' found but size is %" FMT_Z " (expected %d). Ignoring and synthesizing.\n",
              type_name.c_str(), sz, size_bytes);
      }
  }

  // Fallback: Create an anonymous structure of the requested size.
  // This acts as a placeholder ensuring size constraints are met.
  udt_type_data_t udt;
  udt.total_size = size_bytes; // Size in bytes

  // Create a byte array member to ensure the type has concrete size and is not empty.
  // Empty structs can sometimes be treated as size 0 or cause issues.
  tinfo_t byte_type(BT_INT8);
  tinfo_t array_type;
  array_type.create_array(byte_type, size_bytes);

  udm_t mem;
  mem.name = "data";
  mem.type = array_type;
  mem.offset = 0;            // Offset in bits
  mem.size = size_bytes * 8; // Size in bits

  udt.push_back(mem);

  // BTF_STRUCT indicates this is a struct (not union/enum)
  ti.create_udt(udt, BTF_STRUCT);

  if (debug_logging_enabled)
      msg("[AVXLifter] Created synthetic UDT type for %s (Size: %d)\n", type_name.c_str(), ti.get_size());

  return ti;
}

#endif // IDA_SDK_VERSION >= 750
