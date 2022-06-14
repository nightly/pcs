cmake_minimum_required (VERSION 3.22)

project ("pcs" VERSION 0 LANGUAGES CXX)

option(PCS_RUN_TESTS "Run tests" ON)
option(PCS_COMPILE_BENCHMARKS "Compile benchmarks" ON)
option(PCS_VERBOSE_OUTPUT "Verbose output" OFF)

option(PCS_PCH "Use precompiled headers" ON)

# Library
add_subdirectory(src/pcs)

# Application
add_subdirectory(app)

# Preprocessor definitions
if (PCS_VERBOSE_OUTPUT)
    target_compile_definitions(pcs PRIVATE PCS_VERBOSE=1)
    target_compile_definitions(pcs_cli PRIVATE PCS_VERBOSE=1)
endif()

# Tests
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND PCS_RUN_TESTS)
    include(CTest)
    enable_testing()
    include(GoogleTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(tests)
endif()

# Benchmarks
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND PCS_COMPILE_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()
