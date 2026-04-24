# Makefile for AVX-512 / AVX10 / AVX10.2 Comprehensive Test Suite
#
# This builds a freestanding x86_64 object file containing test functions
# for all major AVX-512 and AVX10 instruction families.
#
# Requirements:
#   - clang with AVX-512 and AVX10.2 support (LLVM 18+)
#   - Cross-compilation from ARM Mac or native x86_64

# Compiler settings
CC := clang
TARGET := x86_64-unknown-linux-gnu

# Source files
SRCS := avx512_avx10_comprehensive.c
OBJS := $(SRCS:.c=.o)
BIN := avx512_test.o

# ISA feature flags - comprehensive coverage
AVX512_FLAGS := \
	-mavx512f \
	-mavx512cd \
	-mavx512bw \
	-mavx512dq \
	-mavx512vl \
	-mavx512ifma \
	-mavx512vbmi \
	-mavx512vbmi2 \
	-mavx512vnni \
	-mavx512bitalg \
	-mavx512vpopcntdq \
	-mavx512vp2intersect \
	-mavx512bf16 \
	-mavx512fp16

CRYPTO_FLAGS := \
	-mgfni \
	-maes \
	-mvaes \
	-mvpclmulqdq \
	-msha \
	-msha512 \
	-msm3 \
	-msm4

# AVX extensions (non-512 variants)
AVX_EXT_FLAGS := \
	-mavxneconvert \
	-mavxvnniint8 \
	-mavxvnniint16 \
	-mavxifma

# AMX - Advanced Matrix Extensions
AMX_FLAGS := \
	-mamx-tile \
	-mamx-int8 \
	-mamx-bf16 \
	-mamx-fp16 \
	-mamx-complex

# Memory/Cache extensions
MEMORY_FLAGS := \
	-mmovrs \
	-mprfchw \
	-mclflushopt \
	-mclwb

AVX10_FLAGS := \
	-mavx10.2

ALL_ISA_FLAGS := $(AVX512_FLAGS) $(CRYPTO_FLAGS) $(AVX_EXT_FLAGS) $(AMX_FLAGS) $(MEMORY_FLAGS) $(AVX10_FLAGS)

# Compiler flags
CFLAGS := \
	-O0 \
	--target=$(TARGET) \
	-ffreestanding \
	$(ALL_ISA_FLAGS) \
	-Wall \
	-Wextra \
	-Wno-unused-variable \
	-Wno-unused-but-set-variable

# Optimization levels for different build targets
CFLAGS_O0 := $(CFLAGS) -O0
CFLAGS_O2 := $(CFLAGS) -O2
CFLAGS_O3 := $(CFLAGS) -O3

# Default target
.PHONY: all
all: $(BIN)

# Build with no optimization (default - best for analysis)
$(BIN): $(SRCS)
	$(CC) $(CFLAGS_O0) -c -o $@ $<
	@echo "Built: $@ ($$(wc -c < "$@" | tr -d ' ') bytes)"
	@file $@

# Build with O2 optimization
.PHONY: o2
o2: avx512_test_o2.o
avx512_test_o2.o: $(SRCS)
	$(CC) $(CFLAGS_O2) -c -o $@ $<
	@echo "Built: $@ ($$(wc -c < "$@" | tr -d ' ') bytes)"

# Build with O3 optimization
.PHONY: o3
o3: avx512_test_o3.o
avx512_test_o3.o: $(SRCS)
	$(CC) $(CFLAGS_O3) -c -o $@ $<
	@echo "Built: $@ ($$(wc -c < "$@" | tr -d ' ') bytes)"

# Build all optimization levels
.PHONY: all-opts
all-opts: $(BIN) o2 o3
	@echo "Built all optimization variants"

# Link into executable (for testing with emulator/VM)
avx512_test.elf: $(BIN)
	$(CC) --target=$(TARGET) -ffreestanding -nostdlib -static \
		-Wl,-e,_start -o $@ $<
	@echo "Linked: $@"
	@file $@

# Disassemble to see generated instructions
.PHONY: disasm
disasm: $(BIN)
	llvm-objdump -d --x86-asm-syntax=intel $< | head -1000

# Full disassembly to file
.PHONY: disasm-full
disasm-full: $(BIN)
	llvm-objdump -d --x86-asm-syntax=intel $< > avx512_test.asm
	@echo "Disassembly written to avx512_test.asm"
	@wc -l avx512_test.asm

# Show instruction statistics
.PHONY: stats
stats: $(BIN)
	@echo "=== Instruction Statistics ==="
	@echo "Total instructions:"
	@llvm-objdump -d $< | grep -E "^\s+[0-9a-f]+:" | wc -l
	@echo ""
	@echo "AVX-512 instructions (v-prefix):"
	@llvm-objdump -d $< | grep -oE '\bv[a-z]+' | sort | uniq -c | sort -rn | head -30
	@echo ""
	@echo "Mask register usage:"
	@llvm-objdump -d $< | grep -oE '\{k[0-7]\}' | sort | uniq -c | sort -rn

# Show symbols
.PHONY: symbols
symbols: $(BIN)
	llvm-nm $< | grep -E "^[0-9a-f]+ T"

# Check for unsupported instructions (should be empty)
.PHONY: check
check: $(BIN)
	@echo "Checking for potential issues..."
	@if llvm-objdump -d $< 2>&1 | grep -q "invalid\|unknown"; then \
		echo "WARNING: Found invalid/unknown instructions"; \
		llvm-objdump -d $< 2>&1 | grep -E "invalid|unknown"; \
	else \
		echo "OK: No invalid instructions found"; \
	fi

# Clean build artifacts
.PHONY: clean
clean:
	rm -f $(BIN) avx512_test_o2.o avx512_test_o3.o avx512_test.elf avx512_test.asm

# Show compiler info
.PHONY: info
info:
	@echo "=== Build Configuration ==="
	@echo "Compiler: $(CC)"
	@$(CC) --version | head -1
	@echo ""
	@echo "Target: $(TARGET)"
	@echo ""
	@echo "ISA Features:"
	@echo "  AVX-512:  F, CD, BW, DQ, VL, IFMA, VBMI, VBMI2, VNNI, BITALG, VPOPCNTDQ, VP2INTERSECT, BF16, FP16"
	@echo "  Crypto:   GFNI, AES, VAES, VPCLMULQDQ, SHA, SHA512, SM3, SM4"
	@echo "  AVX-Ext:  AVX-NE-CONVERT, AVX-VNNI-INT8, AVX-VNNI-INT16, AVX-IFMA"
	@echo "  AMX:      TILE, INT8, BF16, FP16, COMPLEX"
	@echo "  Memory:   MOVRS, PRFCHW, CLFLUSHOPT, CLWB"
	@echo "  AVX10:    AVX10.2 (256-bit and 512-bit)"

# Help
.PHONY: help
help:
	@echo "AVX-512 / AVX10 Test Suite Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build test object file (O0, default)"
	@echo "  o2         - Build with O2 optimization"
	@echo "  o3         - Build with O3 optimization"
	@echo "  all-opts   - Build all optimization levels"
	@echo "  disasm     - Disassemble and show first 1000 lines"
	@echo "  disasm-full- Full disassembly to avx512_test.asm"
	@echo "  stats      - Show instruction statistics"
	@echo "  symbols    - List exported symbols"
	@echo "  check      - Check for invalid instructions"
	@echo "  info       - Show build configuration"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help"
