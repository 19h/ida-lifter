UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

CC = clang-20
CFLAGS = -Wall -Wextra -I. -O0 -g

# Architecture specific configuration
ifeq ($(UNAME_S),Darwin)
    ifeq ($(UNAME_M),arm64)
        CFLAGS += -arch x86_64 -mavx -mavx2 -mfma
    else
        CFLAGS += -march=native -mavx -mavx2 -mfma
    endif
else
    CFLAGS += -march=native -mavx -mavx2 -mfma
endif

# IDA Tool Configuration
IDA_PATH = /Applications/IDA Professional 9.2.app/Contents/MacOS
DUMP_TOOL = /Users/int/dev/ida-sdk/src/bin/idafn_dump

# --- Test Categorization ---

# Float Unary: __m256 (*)(__m256)
PS_UNARY = test_vsqrtps test_vrsqrtps test_vrcpps test_vroundps_floor test_vroundps_ceil test_vpermute_ps_0x1B

# Float Binary: __m256 (*)(__m256, __m256)
PS_BINARY = test_vaddps test_vsubps test_vmulps test_vdivps test_vmaxps test_vminps \
            test_vhaddps test_vhsubps test_vandps test_vorps test_vxorps test_vandnps \
            test_vcmpps_eq test_vcmpps_lt test_vcmpps_le test_vcmpps_neq \
            test_vpermute2f128_ps_0x01 test_vshuffle_ps_0x1B test_vblend_ps_0xAA

# Float Ternary: __m256 (*)(__m256, __m256, __m256)
PS_TERNARY = test_vfmadd231ps

# Int Unary: __m256i (*)(__m256i)
INT_UNARY = test_vpabsd test_vpslld_imm test_vpsrld_imm test_vpsrad_imm test_vpermq_imm

# Int Binary: __m256i (*)(__m256i, __m256i)
INT_BINARY = test_vpaddb test_vpaddw test_vpaddd test_vpaddq \
             test_vpsubb test_vpsubw test_vpsubd test_vpsubq \
             test_vpadds_b test_vpadds_w \
             test_vpmulld test_vpmuludq test_vpmaddwd \
             test_vpmaxub test_vpminub test_vpmaxsd test_vpminsd \
             test_vpsignb \
             test_vpand test_vpor test_vpxor test_vpandn \
             test_vpsllv_d test_vpsrlv_d test_vpsrav_d \
             test_vpcmpeqb test_vpcmpgtb test_vpcmpeqd test_vpcmpgtd test_vpcmpeqq test_vpcmpgtq \
             test_vpshufb test_vpermd test_vperm2i128_imm test_vpalignr_imm test_vpblendd_imm

# Conversion: PS -> Int
CVT_PS2DQ = test_vcvtps2dq

# Conversion: Int -> PS
CVT_DQ2PS = test_vcvtdq2ps

ALL_TESTS = $(PS_UNARY) $(PS_BINARY) $(PS_TERNARY) $(INT_UNARY) $(INT_BINARY) $(CVT_PS2DQ) $(CVT_DQ2PS)

# --- Targets ---

BIN_DIR = bin
STUB_DIR = stubs
TEST_DIR = tests

SUITE_TARGET = avx_lifter_test
SUITE_SRCS = main.c $(wildcard $(TEST_DIR)/*.c)
SUITE_OBJS = $(SUITE_SRCS:.c=.o)

# Default: build the suite and all individual binaries
all: suite bins

suite: $(SUITE_TARGET)

bins: $(addprefix $(BIN_DIR)/, $(ALL_TESTS))

$(SUITE_TARGET): $(SUITE_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Generic rule for object files (needed for the suite)
%.o: %.c common.h
	$(CC) $(CFLAGS) -c $< -o $@

# --- Individual Binary Rules ---

# Ensure bin directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# PS Unary
$(addprefix $(BIN_DIR)/, $(PS_UNARY)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_ps_unary.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# PS Binary
$(addprefix $(BIN_DIR)/, $(PS_BINARY)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_ps_binary.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# PS Ternary
$(addprefix $(BIN_DIR)/, $(PS_TERNARY)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_ps_ternary.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# Int Unary
$(addprefix $(BIN_DIR)/, $(INT_UNARY)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_int_unary.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# Int Binary
$(addprefix $(BIN_DIR)/, $(INT_BINARY)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_int_binary.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# CVT PS2DQ
$(addprefix $(BIN_DIR)/, $(CVT_PS2DQ)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_cvt_ps2dq.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# CVT DQ2PS
$(addprefix $(BIN_DIR)/, $(CVT_DQ2PS)): $(BIN_DIR)/%: $(TEST_DIR)/%.c $(STUB_DIR)/stub_cvt_dq2ps.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -DTEST_FUNC=$* -o $@ $^

# --- Convenience Targets ---

# Allow 'make test_vaddps' to build just that binary
$(ALL_TESTS): %: $(BIN_DIR)/%

# --- Run Targets ---

.PHONY: run
run:
	@for test in $(ALL_TESTS); do \
		echo "=== Analyzing $$test ==="; \
		DYLD_LIBRARY_PATH="$(IDA_PATH):$$DYLD_LIBRARY_PATH" $(DUMP_TOOL) $(BIN_DIR)/$$test || exit 1; \
	done; \
	echo "=== ALL TESTS PASSED ==="

# Individual targets still available
define RUN_template
.PHONY: run_$(1)
run_$(1): $(BIN_DIR)/$(1)
	@echo "Analyzing $(BIN_DIR)/$(1)..."
	DYLD_LIBRARY_PATH="$(IDA_PATH):$$DYLD_LIBRARY_PATH" $(DUMP_TOOL) $(BIN_DIR)/$(1)
endef

$(foreach test,$(ALL_TESTS),$(eval $(call RUN_template,$(test))))

# --- Cleanup ---

clean:
	rm -f $(SUITE_OBJS) $(SUITE_TARGET)
	rm -rf $(BIN_DIR)

.PHONY: all suite bins clean run $(ALL_TESTS) $(addprefix run_, $(ALL_TESTS))
