cmake_minimum_required(VERSION 3.10)
project(avx_lifter_tests C CXX)

# Detect platform and architecture
if(APPLE)
    execute_process(COMMAND uname -m OUTPUT_VARIABLE UNAME_M OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(UNAME_M STREQUAL "arm64")
        # Apple Silicon: cross-compile to x86_64 for AVX support
        set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architecture" FORCE)
        message(STATUS "Apple Silicon detected: building for x86_64 (Rosetta 2)")
    endif()
endif()

# Compiler configuration
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# AVX/AVX2/FMA flags (globally applied)
if(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    add_compile_options(-mavx -mavx2 -mfma)
    message(STATUS "Enabling AVX/AVX2/FMA instruction sets")
endif()

# AVX-512 flags (applied only to AVX-512 tests)
set(AVX512_FLAGS -mavx512f -mavx512bw -mavx512dq -mavx512vl)

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/unit)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/integration)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/physics/src)

#==============================================================================
# UNIT TESTS - Individual AVX instruction tests
#==============================================================================

# Get all test source files
file(GLOB UNIT_TEST_SOURCES "unit/sources/*.c")

# Get all stub files
set(STUB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit/stubs")

# Categorize tests by their stub requirements
set(PS_UNARY_TESTS
    test_vsqrtps test_vrsqrtps test_vrcpps
    test_vroundps_floor test_vroundps_ceil test_vpermute_ps_0x1B
)

set(PS_BINARY_TESTS
    test_vaddps test_vsubps test_vmulps test_vdivps
    test_vmaxps test_vminps test_vhaddps test_vhsubps
    test_vandps test_vorps test_vxorps test_vandnps
    test_vcmpps_eq test_vcmpps_lt test_vcmpps_le test_vcmpps_neq
    test_vpermute2f128_ps_0x01 test_vshuffle_ps_0x1B test_vblend_ps_0xAA
)

set(PS_TERNARY_TESTS test_vfmadd231ps)

set(INT_UNARY_TESTS
    test_vpabsd test_vpslld_imm test_vpsrld_imm
    test_vpsrad_imm test_vpermq_imm
)

set(INT_BINARY_TESTS
    test_vpaddb test_vpaddw test_vpaddd test_vpaddq
    test_vpsubb test_vpsubw test_vpsubd test_vpsubq
    test_vpadds_b test_vpadds_w
    test_vpmulld test_vpmuludq test_vpmaddwd
    test_vpmaxub test_vpminub test_vpmaxsd test_vpminsd
    test_vpsignb
    test_vpand test_vpor test_vpxor test_vpandn
    test_vpsllv_d test_vpsrlv_d test_vpsrav_d
    test_vpcmpeqb test_vpcmpgtb test_vpcmpeqd test_vpcmpgtd
    test_vpcmpeqq test_vpcmpgtq
    test_vpshufb test_vpermd test_vperm2i128_imm
    test_vpalignr_imm test_vpblendd_imm
)

set(CVT_PS2DQ_TESTS test_vcvtps2dq)
set(CVT_DQ2PS_TESTS test_vcvtdq2ps)

# ZMM tests (AVX-512)
set(ZMM_BINARY_TESTS test_avx512_addps)
set(ZMM_MASK_TESTS test_avx512_mask_addps)
set(ZMM_MASKZ_TESTS test_avx512_maskz_addps)

# Helper macro to create unit test executables
macro(add_unit_test TEST_NAME STUB_FILE)
    add_executable(${TEST_NAME}
        unit/sources/${TEST_NAME}.c
        ${STUB_DIR}/${STUB_FILE}
    )
    target_compile_definitions(${TEST_NAME} PRIVATE TEST_FUNC=${TEST_NAME})
endmacro()

# Generate all unit test targets
foreach(TEST ${PS_UNARY_TESTS})
    add_unit_test(${TEST} stub_ps_unary.c)
endforeach()

foreach(TEST ${PS_BINARY_TESTS})
    add_unit_test(${TEST} stub_ps_binary.c)
endforeach()

foreach(TEST ${PS_TERNARY_TESTS})
    add_unit_test(${TEST} stub_ps_ternary.c)
endforeach()

foreach(TEST ${INT_UNARY_TESTS})
    add_unit_test(${TEST} stub_int_unary.c)
endforeach()

foreach(TEST ${INT_BINARY_TESTS})
    add_unit_test(${TEST} stub_int_binary.c)
endforeach()

foreach(TEST ${CVT_PS2DQ_TESTS})
    add_unit_test(${TEST} stub_cvt_ps2dq.c)
endforeach()

foreach(TEST ${CVT_DQ2PS_TESTS})
    add_unit_test(${TEST} stub_cvt_dq2ps.c)
endforeach()

foreach(TEST ${ZMM_BINARY_TESTS})
    add_unit_test(${TEST} stub_zmm_binary.c)
    target_compile_options(${TEST} PRIVATE ${AVX512_FLAGS})
endforeach()

foreach(TEST ${ZMM_MASK_TESTS})
    add_unit_test(${TEST} stub_zmm_mask_binary.c)
    target_compile_options(${TEST} PRIVATE ${AVX512_FLAGS})
endforeach()

foreach(TEST ${ZMM_MASKZ_TESTS})
    add_unit_test(${TEST} stub_zmm_maskz_binary.c)
    target_compile_options(${TEST} PRIVATE ${AVX512_FLAGS})
endforeach()

# Collect all unit test targets
set(ALL_UNIT_TESTS
    ${PS_UNARY_TESTS}
    ${PS_BINARY_TESTS}
    ${PS_TERNARY_TESTS}
    ${INT_UNARY_TESTS}
    ${INT_BINARY_TESTS}
    ${CVT_PS2DQ_TESTS}
    ${CVT_DQ2PS_TESTS}
    ${ZMM_BINARY_TESTS}
    ${ZMM_MASK_TESTS}
    ${ZMM_MASKZ_TESTS}
)

#==============================================================================
# INTEGRATION TESTS - Multi-instruction complex tests
#==============================================================================

# Comprehensive test suite
file(GLOB INTEGRATION_TEST_FILES "integration/sources/*.c")
# Exclude files that have their own main() and are built separately
list(FILTER INTEGRATION_TEST_FILES EXCLUDE REGEX "test_fluid_advect\.c$")
list(FILTER INTEGRATION_TEST_FILES EXCLUDE REGEX "test_fluid_diffuse\.c$")
list(FILTER INTEGRATION_TEST_FILES EXCLUDE REGEX "test_avx512_mem_addps\.c$")
list(FILTER INTEGRATION_TEST_FILES EXCLUDE REGEX "test_avx512_addps\.c$")
add_executable(avx_comprehensive_test
    integration/main.c
    ${INTEGRATION_TEST_FILES}
)
target_compile_options(avx_comprehensive_test PRIVATE ${AVX512_FLAGS})

# Fluid dynamics tests
add_executable(test_fluid_advect integration/sources/test_fluid_advect.c)
add_executable(test_fluid_diffuse integration/sources/test_fluid_diffuse.c)

# AVX-512 memory tests
add_executable(test_avx512_mem_addps integration/sources/test_avx512_mem_addps.c)
target_compile_options(test_avx512_mem_addps PRIVATE ${AVX512_FLAGS})

# Test scalar operations
add_executable(test_scalar integration/test_scalar.c)

set(ALL_INTEGRATION_TESTS
    avx_comprehensive_test
    test_fluid_advect
    test_fluid_diffuse
    test_avx512_mem_addps
    test_scalar
)

#==============================================================================
# PHYSICS TESTS - Real-world SIMD workloads
#==============================================================================

set(PHYSICS_COMMON_SOURCES
    physics/src/vis.c
    physics/src/bullet.c
    physics/src/fluid.c
    physics/src/asteroid.c
    physics/src/interplanetary.c
)

# Reference binary (comprehensive test)
add_executable(decompiler_ref
    physics/src/main.c
    ${PHYSICS_COMMON_SOURCES}
)
target_link_libraries(decompiler_ref m)

# Shooter binary (game physics)
add_executable(shooter
    physics/src/shooter_main.c
    ${PHYSICS_COMMON_SOURCES}
)
target_link_libraries(shooter m)

# Apply physics-specific optimizations
set_target_properties(decompiler_ref shooter PROPERTIES
    COMPILE_FLAGS "-O3 -ffast-math -msse4.1"
)

set(ALL_PHYSICS_TESTS
    decompiler_ref
    shooter
)

#==============================================================================
# CUSTOM TARGETS
#==============================================================================

# Build all tests
add_custom_target(all_tests
    DEPENDS ${ALL_UNIT_TESTS} ${ALL_INTEGRATION_TESTS} ${ALL_PHYSICS_TESTS}
)

# Build only unit tests
add_custom_target(unit_tests
    DEPENDS ${ALL_UNIT_TESTS}
)

# Build only integration tests
add_custom_target(integration_tests
    DEPENDS ${ALL_INTEGRATION_TESTS}
)

# Build only physics tests
add_custom_target(physics_tests
    DEPENDS ${ALL_PHYSICS_TESTS}
)

message(STATUS "Configured ${CMAKE_PROJECT_NAME}:")
message(STATUS "  Unit tests:        ${CMAKE_CURRENT_LIST_LENGTH} targets")
message(STATUS "  Integration tests: 5 targets")
message(STATUS "  Physics tests:     2 targets")
