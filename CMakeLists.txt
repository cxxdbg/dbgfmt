

cmake_minimum_required(VERSION 3.20)
project(dbgfmt CXX)

set(CMAKE_CXX_STANDARD 20)
include(CTest)

# Boost provides its own package config (BoostConfig.cmake); CMake's bundled
# FindBoost module is deprecated.
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.74 REQUIRED COMPONENTS log unit_test_framework program_options)

# Locates the cxxtn library. If ON, fetches it fresh via FetchContent.
# If OFF, locates it via find_package() instead (QUIET: when dbgfmt is
# consumed as a subdirectory of a parent project that already provides a
# `cxxtn` target itself, e.g. cxxdbg, which add_subdirectory()s its own
# thirdparty/cxxtn before this one, find_package() will simply find
# nothing and downstream targets fall back to that pre-existing target).
option(DBGFMT_FETCH_CXXTN "If ON, fetch cxxtn via FetchContent; if OFF, locate it via find_package" ON)

if(DBGFMT_FETCH_CXXTN)
    include(FetchContent)
    FetchContent_Declare(cxxtn
        GIT_REPOSITORY git@github.com:cxxdbg/cxxtn.git
        GIT_TAG main)
    FetchContent_MakeAvailable(cxxtn)
else()
    find_package(cxxtn QUIET)
endif()

# Locates the cm/cm-cxx libraries. Same ON/OFF semantics as
# DBGFMT_FETCH_CXXTN above. cm's own cmake_minimum_required(VERSION 3.6)
# means its option() calls only honor cache variables (CMP0077 OLD
# behavior), so these must be forced into the cache rather than set as
# plain variables.
option(DBGFMT_FETCH_CM "If ON, fetch cm via FetchContent; if OFF, locate it via find_package" ON)

if(DBGFMT_FETCH_CM)
    set(CM_ENABLE_CXX ON CACHE BOOL "" FORCE)
    set(CM_CXX_ENABLE_CLANG OFF CACHE BOOL "" FORCE)
    set(CM_ENABLE_SRC OFF CACHE BOOL "" FORCE)
    set(CM_ENABLE_TOOLS OFF CACHE BOOL "" FORCE)
    include(FetchContent)
    FetchContent_Declare(cm
        GIT_REPOSITORY git@github.com:cxx-lang-tools/cm.git
        GIT_TAG main)
    FetchContent_MakeAvailable(cm)
else()
    find_package(cm QUIET)
endif()

set(DBGFMT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(src)
