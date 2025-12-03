# Makefile for AVX Lifter Test Suite
# Wrapper around CMake build system

# Build directory
BUILD_DIR = build

# Default target
.PHONY: all
all: configure
	@$(MAKE) -C $(BUILD_DIR)

# Configure CMake
.PHONY: configure
configure:
	@if [ ! -d "$(BUILD_DIR)" ]; then \
		mkdir -p $(BUILD_DIR); \
		cd $(BUILD_DIR) && cmake ..; \
	fi

# Build all tests
.PHONY: build
build: configure
	@$(MAKE) -C $(BUILD_DIR)

# Build specific test categories
.PHONY: unit_tests
unit_tests: configure
	@$(MAKE) -C $(BUILD_DIR) unit_tests

.PHONY: integration_tests
integration_tests: configure
	@$(MAKE) -C $(BUILD_DIR) integration_tests

.PHONY: physics_tests
physics_tests: configure
	@$(MAKE) -C $(BUILD_DIR) physics_tests

# Build specific tests
.PHONY: decompiler_ref
decompiler_ref: configure
	@$(MAKE) -C $(BUILD_DIR) decompiler_ref

.PHONY: shooter
shooter: configure
	@$(MAKE) -C $(BUILD_DIR) shooter

.PHONY: avx_comprehensive_test
avx_comprehensive_test: configure
	@$(MAKE) -C $(BUILD_DIR) avx_comprehensive_test

# Pattern rule for building individual unit tests
test_%: configure
	@$(MAKE) -C $(BUILD_DIR) $@

# Clean build artifacts
.PHONY: clean
clean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		echo "Cleaning build directory..."; \
		rm -rf $(BUILD_DIR); \
	fi

# Full rebuild
.PHONY: rebuild
rebuild: clean all

# Run tests
.PHONY: run_decompiler_ref
run_decompiler_ref: decompiler_ref
	@echo "Running decompiler_ref (AVX physics suite)..."
	@$(BUILD_DIR)/decompiler_ref

.PHONY: run_shooter
run_shooter: shooter
	@echo "Running shooter..."
	@$(BUILD_DIR)/shooter

.PHONY: run_comprehensive
run_comprehensive: avx_comprehensive_test
	@echo "Running comprehensive test..."
	@$(BUILD_DIR)/avx_comprehensive_test

# =============================================================================
# EMSCRIPTEN / WEBASSEMBLY BUILD
# =============================================================================

# Emscripten build directory
WASM_BUILD_DIR = shooter/wasm_build

# Shooter source files
SHOOTER_SOURCES = \
	shooter/src/main.c \
	shooter/src/core/game.c \
	shooter/src/level/level.c \
	shooter/src/entity/entity.c \
	shooter/src/ai/enemy_ai.c \
	shooter/src/ai/player_ai.c \
	shooter/src/ai/enemy_ai_advanced.c \
	shooter/src/ai/player_ai_advanced.c \
	shooter/src/combat/bullet.c \
	shooter/src/physics/physics.c \
	shooter/src/physics/physics_avx.c \
	shooter/src/render/render.c

# Shooter include paths
SHOOTER_INCLUDES = \
	-I./shooter/include \
	-I./shooter/include/core \
	-I./shooter/include/math \
	-I./shooter/include/level \
	-I./shooter/include/entity \
	-I./shooter/include/ai \
	-I./shooter/include/combat \
	-I./shooter/include/physics \
	-I./shooter/include/render \
	-I./shooter/include/game

# Emscripten compiler flags - maximum optimization and size reduction
EMCC_FLAGS = -msimd128 -mavx -mavx2 -Oz -DNDEBUG -flto \
	-fno-exceptions -fno-rtti \
	-ffunction-sections -fdata-sections

# Build shooter as WebAssembly with HTML shell
.PHONY: shooter-wasm
shooter-wasm:
	@echo "Building shooter for WebAssembly (via Docker emscripten/emsdk)..."
	@mkdir -p $(WASM_BUILD_DIR)
	@docker run --rm \
		-v "$(CURDIR)":/src \
		-w /src \
		emscripten/emsdk \
		emcc $(EMCC_FLAGS) $(SHOOTER_INCLUDES) \
		$(SHOOTER_SOURCES) \
		-o $(WASM_BUILD_DIR)/shooter.html \
		--shell-file shooter/wasm_shell.html \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s INITIAL_MEMORY=33554432 \
		-s WASM_BIGINT=1 \
		-s FORCE_FILESYSTEM=1 \
		-s SUPPORT_LONGJMP=0 \
		-s DISABLE_EXCEPTION_CATCHING=1 \
		-s NO_EXIT_RUNTIME=1 \
		-s MALLOC=emmalloc \
		-s ABORTING_MALLOC=0 \
		-s SUPPORT_ERRNO=0 \
		--closure 1 \
		-Wl,--gc-sections
	@echo "Build complete: $(WASM_BUILD_DIR)/shooter.html"
	@echo "To run: cd $(WASM_BUILD_DIR) && python3 -m http.server 8080"

# Build shooter as WebAssembly (JS + WASM only, no HTML)
.PHONY: shooter-wasm-lib
shooter-wasm-lib:
	@echo "Building shooter WASM library (via Docker emscripten/emsdk)..."
	@mkdir -p $(WASM_BUILD_DIR)
	@docker run --rm \
		-v "$(CURDIR)":/src \
		-w /src \
		emscripten/emsdk \
		emcc $(EMCC_FLAGS) $(SHOOTER_INCLUDES) \
		$(SHOOTER_SOURCES) \
		-o $(WASM_BUILD_DIR)/shooter.js \
		-s ALLOW_MEMORY_GROWTH=1 \
		-s INITIAL_MEMORY=33554432 \
		-s WASM_BIGINT=1 \
		-s MODULARIZE=1 \
		-s EXPORT_NAME="ShooterModule"
	@echo "Build complete: $(WASM_BUILD_DIR)/shooter.js + shooter.wasm"

# Clean WebAssembly build
.PHONY: clean-wasm
clean-wasm:
	@echo "Cleaning WebAssembly build..."
	@rm -rf $(WASM_BUILD_DIR)
	@rm -f shooter/shooter.js shooter/shooter.wasm shooter/shooter.html

# =============================================================================
# HELP
# =============================================================================

# Help target
.PHONY: help
help:
	@echo "AVX Lifter Test Suite - Makefile Targets"
	@echo "========================================"
	@echo ""
	@echo "Build Targets:"
	@echo "  make                    - Build all tests (default)"
	@echo "  make build              - Build all tests"
	@echo "  make unit_tests         - Build only unit tests"
	@echo "  make integration_tests  - Build only integration tests"
	@echo "  make physics_tests      - Build only physics tests"
	@echo ""
	@echo "Specific Targets:"
	@echo "  make decompiler_ref     - Build decompiler_ref (AVX physics suite)"
	@echo "  make shooter            - Build shooter (bullet hell game)"
	@echo "  make test_vaddps        - Build specific unit test"
	@echo "  make test_<name>        - Build any test by name"
	@echo ""
	@echo "Run Targets:"
	@echo "  make run_decompiler_ref - Build and run AVX physics suite"
	@echo "  make run_shooter        - Build and run shooter game"
	@echo "  make run_comprehensive  - Build and run comprehensive test"
	@echo ""
	@echo "WebAssembly Targets (requires Docker):"
	@echo "  make shooter-wasm       - Build shooter as WebAssembly with HTML"
	@echo "  make shooter-wasm-lib   - Build shooter as WASM library (JS+WASM)"
	@echo "  make clean-wasm         - Clean WebAssembly build artifacts"
	@echo ""
	@echo "Maintenance:"
	@echo "  make clean              - Remove build directory"
	@echo "  make clean-wasm         - Remove WebAssembly build"
	@echo "  make rebuild            - Clean and rebuild all"
	@echo "  make help               - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                                  # Build everything"
	@echo "  make test_vaddps                      # Build single unit test"
	@echo "  make run_decompiler_ref               # Build and run physics suite"
	@echo "  make shooter-wasm                     # Build WASM version"
	@echo "  make clean && make                    # Clean rebuild"

# Prevent make from deleting intermediate files
.SECONDARY:

# Declare phony targets
.PHONY: all configure build unit_tests integration_tests physics_tests \
        decompiler_ref shooter avx_comprehensive_test \
        clean rebuild run_decompiler_ref run_shooter run_comprehensive help \
        shooter-wasm shooter-wasm-lib clean-wasm
