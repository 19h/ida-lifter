cmake_minimum_required(VERSION 3.10)
project(shooter C)

# =============================================================================
# Platform and Architecture Detection
# =============================================================================

if(APPLE)
    execute_process(COMMAND uname -m OUTPUT_VARIABLE UNAME_M OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(UNAME_M STREQUAL "arm64")
        # Apple Silicon: cross-compile to x86_64 for AVX support
        set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architecture" FORCE)
        message(STATUS "Apple Silicon detected: building for x86_64 (Rosetta 2)")
    endif()
endif()

# =============================================================================
# Compiler Configuration
# =============================================================================

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Warning flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")

# AVX/AVX2/FMA flags
if(CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64" OR CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    add_compile_options(-mavx -mavx2 -mfma)
    message(STATUS "Enabling AVX/AVX2/FMA instruction sets")
endif()

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# =============================================================================
# Include Directories
# =============================================================================

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
    ${CMAKE_CURRENT_SOURCE_DIR}/include/math
    ${CMAKE_CURRENT_SOURCE_DIR}/include/level
    ${CMAKE_CURRENT_SOURCE_DIR}/include/entity
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ai
    ${CMAKE_CURRENT_SOURCE_DIR}/include/combat
    ${CMAKE_CURRENT_SOURCE_DIR}/include/physics
    ${CMAKE_CURRENT_SOURCE_DIR}/include/render
    ${CMAKE_CURRENT_SOURCE_DIR}/include/game
)

# =============================================================================
# Source Files
# =============================================================================

set(SHOOTER_SOURCES
    src/main.c
    src/core/game.c
    src/level/level.c
    src/entity/entity.c
    src/ai/enemy_ai.c
    src/ai/player_ai.c
    src/combat/bullet.c
    src/physics/physics.c
    src/render/render.c
)

# =============================================================================
# Targets
# =============================================================================

# Main shooter executable
add_executable(shooter ${SHOOTER_SOURCES})

# Link math library
target_link_libraries(shooter m)

# Apply optimizations
set_target_properties(shooter PROPERTIES
    COMPILE_FLAGS "-O3 -ffast-math -msse4.1"
)

# =============================================================================
# Installation
# =============================================================================

install(TARGETS shooter DESTINATION bin)

# =============================================================================
# Summary
# =============================================================================

message(STATUS "")
message(STATUS "=== Shooter Build Configuration ===")
message(STATUS "  Build type:    ${CMAKE_BUILD_TYPE}")
message(STATUS "  C Compiler:    ${CMAKE_C_COMPILER}")
message(STATUS "  Install path:  ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
