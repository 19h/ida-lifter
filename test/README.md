# AVX Lifter Test Suite

Comprehensive test suite for the AVX/AVX2/AVX-512 microcode lifter plugin for IDA Pro.

## Directory Structure

```
test/
├── CMakeLists.txt          # Unified CMake build configuration
├── unit/                   # Individual AVX instruction tests (~75 tests)
│   ├── sources/            # Test function implementations
│   └── stubs/              # Test harnesses by signature type
├── integration/            # Multi-instruction complex tests
│   ├── sources/            # Fluid dynamics and AVX-512 tests
│   ├── avx_comprehensive_test.c  # Comprehensive instruction coverage
│   ├── main.c              # Test suite runner
│   └── test_scalar.c       # Scalar AVX operation tests
├── physics/                # Real-world SIMD workloads
│   └── src/                # Physics simulations (shooter game, fluids, etc.)
└── bin/                    # Build output (created by CMake)
```

## Test Categories

### Unit Tests (`unit/`)

Individual instruction tests organized by signature:

- **PS Unary** (`__m256 (*)(m256)`): sqrt, rsqrt, rcp, round, permute
- **PS Binary** (`__m256 (*)(m256, m256)`): add, sub, mul, div, min, max, logical ops, compare, blend
- **PS Ternary** (`__m256 (*)(m256, m256, m256)`): FMA variants
- **Int Unary** (`__m256i (*)(__m256i)`): shifts, permute, absolute value
- **Int Binary** (`__m256i (*)(__m256i, __m256i)`): integer arithmetic, logical, compare, shuffle
- **Conversions**: ps↔dq (float↔int32)
- **AVX-512**: ZMM register operations with/without masking

Each test is a minimal function exercising a single instruction, compiled with its appropriate stub harness.

### Integration Tests (`integration/`)

Complex multi-instruction tests:

- `avx_comprehensive_test.c` - Exhaustive coverage of all AVX/AVX2/AVX-512 instructions
- `test_fluid_advect.c` - Fluid dynamics advection (gather, round, FMA, blend)
- `test_fluid_diffuse.c` - Fluid dynamics diffusion (FMA with memory operands)
- `test_avx512_mem_addps.c` - AVX-512 memory operand handling
- `test_scalar.c` - Scalar AVX operations (vaddss, vmulss, etc.)

### Physics Tests (`physics/`)

Real-world SIMD workloads from game physics and scientific computing:

**`decompiler_ref`** - Comprehensive physics simulation testbed (4 simulations, ~10 sec each):
- `nbody.c` - N-body gravitational simulation (AVX-512 ZMM)
- `waves.c` - 2D wave interference patterns (AVX2 YMM)
- `particles.c` - Particle swarm dynamics (AVX YMM)
- `fluid.c` - Navier-Stokes fluid vortex simulation (AVX2 YMM)

**`shooter`** - 2D bullet-hell shooter game:
- `bullet.c` - Projectile physics, collision detection, A* pathfinding, sound-based AI (AVX YMM)
- Procedural level generation with rooms and corridors
- Multiple enemy types with graduated awareness AI

Both binaries serve as the primary validation suite for the lifter plugin.

## Building Tests

### Prerequisites

- CMake 3.10+
- C compiler with AVX support (GCC/Clang)
- x86_64 architecture (or Apple Silicon with Rosetta 2)

### Quick Start with Makefile (Recommended)

The test directory includes a convenient Makefile wrapper around CMake:

```bash
cd test

# Build all tests
make

# Build specific categories
make unit_tests          # Build only unit tests
make integration_tests   # Build only integration tests
make physics_tests       # Build only physics tests

# Build specific tests
make test_vaddps         # Unit test
make shooter             # Physics test
make avx_comprehensive_test  # Integration test

# Run tests
make run_shooter         # Build and run shooter (runs indefinitely)
make run_decompiler_ref  # Build and run physics simulations (~40 sec total)

# Maintenance
make clean               # Remove build directory
make rebuild             # Clean and rebuild all
make help                # Show all available targets
```

### Manual CMake Build

If you prefer direct CMake usage:

```bash
cd test
mkdir build && cd build
cmake ..
make

# Build specific test categories
make unit_tests
make integration_tests
make physics_tests
```

### Cross-compilation (Apple Silicon)

On ARM64 Macs, CMake automatically configures x86_64 cross-compilation for Rosetta 2:

```bash
make  # Automatically detects Apple Silicon and configures -arch x86_64
```

## Running Tests

### With IDA Pro Decompilation

Use the `idafn_dump` tool to batch decompile and check for errors:

```bash
# Run all physics tests (recommended for validation)
DYLD_LIBRARY_PATH="/path/to/ida" /path/to/idafn_dump build/shooter

# Run specific unit test
DYLD_LIBRARY_PATH="/path/to/ida" /path/to/idafn_dump build/test_vaddps
```

### Direct Execution

Tests can also be executed directly (though their output is minimal):

```bash
./build/avx_comprehensive_test
./build/shooter
```

## Test Development

### Adding a New Unit Test

1. Create test implementation in `unit/sources/test_<name>.c`:
```c
#include <immintrin.h>
__m256 test_vaddps(__m256 a, __m256 b) {
    return _mm256_add_ps(a, b);
}
```

2. Add to appropriate category in `CMakeLists.txt`:
```cmake
set(PS_BINARY_TESTS
    # ... existing tests ...
    test_vaddps
)
```

3. Rebuild: `make test_vaddps`

### Adding a New Integration Test

1. Create test file in `integration/` or `integration/sources/`
2. Add executable in `CMakeLists.txt`:
```cmake
add_executable(test_myfeature integration/test_myfeature.c)
```

## Validation Criteria

**Success**: All functions decompile without INTERR errors when the lifter plugin is loaded.

**Common Issues**:
- INTERR 50920: Operand size mismatch (scalar memory operands not zero-extended)
- INTERR 50757: Bad operand size (>8 bytes without UDT flag)
- INTERR 50732: Wrong argument location (invalid pointer types)

## IDA Configuration

### Plugin Installation

```bash
# Build and install plugin
cd .. && make install  # Installs to ~/.idapro/plugins/

# macOS: Code-sign is automatic via Makefile
```

### Batch Decompilation

The physics test suite serves as the primary validation:

```bash
cd test/build

# Validate shooter (main test - ~48 functions)
DYLD_LIBRARY_PATH="/Applications/IDA Pro.app/Contents/MacOS" \
  /path/to/idafn_dump shooter

# Validate decompiler_ref (physics simulations)
DYLD_LIBRARY_PATH="/Applications/IDA Pro.app/Contents/MacOS" \
  /path/to/idafn_dump decompiler_ref
```

**Expected**: All functions decompile successfully with the lifter enabled.

## CI Integration

The test suite is designed for automated testing:

```bash
# Build all tests
cmake -B build -S test
cmake --build build

# Run decompilation tests (requires IDA license)
for binary in build/test_*; do
    idafn_dump "$binary" || exit 1
done
```

## Test Statistics

- **Unit tests**: ~75 individual instruction tests
- **Integration tests**: 5 complex multi-instruction tests
- **Physics tests**: 2 binaries
  - `decompiler_ref`: 4 physics simulations (nbody, wave, particle, fluid)
  - `shooter`: Comprehensive game AI with ~48 functions
- **Instruction coverage**: AVX, AVX2, FMA, limited AVX-512 (ZMM register-to-register)

## Known Limitations

Tests involving AVX-512 instructions with:
- Memory operands (ZMM 64-byte loads/stores)
- Opmask register manipulation (kandw, korw, etc.)
- Compare-to-mask operations (vcmpeqps k1, ...)

These fall back to IDA's default handling due to SDK limitations. See `CLAUDE.md` § "AVX-512 (ZMM) Support" for details.

Hex-Rays also emits warnings like `unsupported processor register 'zmm0'` when decompiling ZMM tests. This is a decompiler limitation; the test functions should still decompile without INTERRs.
