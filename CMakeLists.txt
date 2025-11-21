cmake_minimum_required(VERSION 3.10)
project(lifter)

# Find IDA SDK
SET(IDASDK_VER "92" CACHE STRING "92")
SET(IDASDK $ENV{IDASDK})
MESSAGE("${PROJECT_NAME}")
MESSAGE("Using IDA SDK dir: ${IDASDK}")
MESSAGE("-------------------")

IF( NOT CMAKE_BUILD_TYPE )
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
ENDIF()
SET(CMAKE_SKIP_RPATH TRUE)

include(${CMAKE_CURRENT_LIST_DIR}/ida-cmake/bootstrap.cmake)
find_package(idasdk REQUIRED)

# Set plugin variables before including addons.cmake
ida_add_plugin(lifter
  SOURCES
    src/plugin/lifter_plugin.cpp
    src/plugin/component_registry.cpp
    src/avx/avx_lifter.cpp
    src/avx/avx_types.cpp
    src/avx/avx_intrinsic.cpp
    src/avx/avx_helpers.cpp
    src/avx/avx_utils.cpp
    src/avx/avx_debug.cpp
    src/avx/handlers/handler_cvt.cpp
    src/avx/handlers/handler_mov.cpp
    src/avx/handlers/handler_math.cpp
    src/avx/handlers/handler_logic.cpp
    src/inline/inline_component.cpp
)
