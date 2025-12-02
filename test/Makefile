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
.PHONY: shooter
shooter: configure
	@$(MAKE) -C $(BUILD_DIR) shooter

.PHONY: decompiler_ref
decompiler_ref: configure
	@$(MAKE) -C $(BUILD_DIR) decompiler_ref

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
.PHONY: run_shooter
run_shooter: shooter
	@echo "Running shooter..."
	@$(BUILD_DIR)/shooter

.PHONY: run_comprehensive
run_comprehensive: avx_comprehensive_test
	@echo "Running comprehensive test..."
	@$(BUILD_DIR)/avx_comprehensive_test

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
	@echo "  make shooter            - Build shooter binary"
	@echo "  make decompiler_ref     - Build decompiler_ref binary"
	@echo "  make test_vaddps        - Build specific unit test"
	@echo "  make test_<name>        - Build any test by name"
	@echo ""
	@echo "Run Targets:"
	@echo "  make run_shooter        - Build and run shooter"
	@echo "  make run_comprehensive  - Build and run comprehensive test"
	@echo ""
	@echo "Maintenance:"
	@echo "  make clean              - Remove build directory"
	@echo "  make rebuild            - Clean and rebuild all"
	@echo "  make help               - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                           # Build everything"
	@echo "  make test_vaddps               # Build single unit test"
	@echo "  make shooter && build/shooter  # Build and run shooter"
	@echo "  make clean && make             # Clean rebuild"

# Prevent make from deleting intermediate files
.SECONDARY:

# Declare phony targets
.PHONY: all configure build unit_tests integration_tests physics_tests \
        shooter decompiler_ref avx_comprehensive_test \
        clean rebuild run_shooter run_comprehensive help
