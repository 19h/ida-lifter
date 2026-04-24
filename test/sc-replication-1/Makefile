# AVX/AVX2 Instruction Lifter Test Suite Makefile
#
# Build targets:
#   make          - Build with -O0 (preserves all instructions)
#   make O1       - Build with -O1 (may inline/optimize)
#   make O2       - Build with -O2 (aggressive optimization)
#   make asm      - Generate assembly listing
#   make check    - Verify AVX instruction coverage
#   make clean    - Remove build artifacts

CC = clang-20
CFLAGS_BASE = -target x86_64-apple-darwin -mavx2 -mavx -msse4.1 -mfma -g -Wall
CFLAGS_O0 = $(CFLAGS_BASE) -O0
CFLAGS_O1 = $(CFLAGS_BASE) -O1
CFLAGS_O2 = $(CFLAGS_BASE) -O3 -ffast-math
LDFLAGS = -lm

SRC = avx_lifter_test.c
BIN = avx_lifter_test
ASM = avx_lifter_test.s

.PHONY: all O0 O1 O2 asm check clean run

all: O0

O0: $(SRC)
	$(CC) $(CFLAGS_O0) -o $(BIN)_O0 $(SRC) $(LDFLAGS)
	@echo "Built with -O0 (maximum instruction preservation)"

O1: $(SRC)
	$(CC) $(CFLAGS_O1) -o $(BIN)_O1 $(SRC) $(LDFLAGS)
	@echo "Built with -O1"

O2: $(SRC)
	$(CC) $(CFLAGS_O2) -o $(BIN)_O2 $(SRC) $(LDFLAGS)
	@echo "Built with -O2"

asm: $(SRC)
	$(CC) $(CFLAGS_O0) -S -masm=intel -o $(ASM) $(SRC)
	@echo "Generated Intel syntax assembly: $(ASM)"

check: O0
	@echo "=== AVX/AVX2 Instruction Coverage ==="
	@echo ""
	@echo "VEX-encoded SSE (XMM):"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vmovss\|vmovaps\|vmovups" | xargs echo "  vmov variants:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vaddss\|vsubss\|vmulss\|vdivss" | xargs echo "  arithmetic:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vroundps\|vroundss" | xargs echo "  vroundps:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vcmpeqss\|vcmpless\|vcomiss" | xargs echo "  comparisons:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vandps\|vandnps\|vorps\|vxorps" | xargs echo "  logical:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vblendvps" | xargs echo "  vblendvps:"
	@echo ""
	@echo "AVX Double Precision:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vmovsd\|vmovapd\|vmovupd" | xargs echo "  vmov double:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vaddsd\|vsubsd\|vmulsd\|vdivsd" | xargs echo "  arithmetic double:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vsqrtpd\|vmovddup\|vunpckhpd" | xargs echo "  special double:"
	@echo ""
	@echo "AVX2 (256-bit YMM):"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vmovdqu\|vmovdqa.*ymm" | xargs echo "  ymm moves:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vzeroupper" | xargs echo "  vzeroupper:"
	@echo ""
	@echo "Integer/Conversion:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vcvtsi2ss\|vcvttss2si" | xargs echo "  int<->float:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vcvtsd2ss\|vcvtpd2ps" | xargs echo "  double->float:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vpextrq\|vmovq" | xargs echo "  extract/move 64:"
	@objdump -d -M intel $(BIN)_O0 | grep -c "vpcmpeqd" | xargs echo "  vpcmpeqd:"

run: O0
	./$(BIN)_O0

clean:
	rm -f $(BIN)_O0 $(BIN)_O1 $(BIN)_O2 $(ASM)
