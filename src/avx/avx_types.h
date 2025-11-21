/*
    AVX Type Management Utilities
*/

#pragma once

#include "../common/warn_off.h"
#include <pro.h>       // Ensure IDA_SDK_VERSION is defined
#include <hexrays.hpp>
#include "../common/warn_on.h"

// Constants
#define XMM_SIZE 16
#define YMM_SIZE 32
#define ZMM_SIZE 64

#define FLOAT_SIZE   4
#define DOUBLE_SIZE  8
#define DWORD_SIZE   4
#define QWORD_SIZE   8

// Type management
tinfo_t get_vector_type(int size_bytes, bool is_int, bool is_double);

// Debug logging control
extern bool debug_logging_enabled;

#define DEBUG_LOG(fmt, ...)        \
do {                              \
  if (debug_logging_enabled)      \
    msg("[AVXLifter::DEBUG] " fmt "\n", ##__VA_ARGS__); \
} while (0)

#define ERROR_LOG(fmt, ...) msg("[AVXLifter::ERROR] " fmt "\n", ##__VA_ARGS__)

#define TRACE_ENTER(func_name) \
DEBUG_LOG("%a: >>> ENTER %s", cdg.insn.ea, func_name)

#define TRACE_EXIT(func_name, result) \
DEBUG_LOG("%a: <<< EXIT %s, result=%d", cdg.insn.ea, func_name, result)

#define TRACE_OP(idx, op) \
DEBUG_LOG("%a:   Op%d: type=%d, dtype=%d, reg=%d", cdg.insn.ea, idx, (int)op.type, (int)op.dtype, (int)op.reg)

#define TRACE_MREG(name, mreg) \
DEBUG_LOG("%a:   %s = mreg %d", cdg.insn.ea, name, (int)mreg)

#define TRACE_SIZE(name, size) \
DEBUG_LOG("%a:   %s = %d bytes", cdg.insn.ea, name, size)

#define TRACE_INSN(opcode) \
DEBUG_LOG("%a:   emitting microcode: %s", cdg.insn.ea, #opcode)
