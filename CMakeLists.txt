cmake_minimum_required (VERSION 3.8)

project ("pcs" VERSION 0 LANGUAGES CXX)

option(PCS_RUN_TESTS "Run tests" ON)
option(PCS_RUN_BENCHMARKS "Run benchmarks" ON)

# Library
add_subdirectory(src/pcs)

# Tests
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND PCS_RUN_TESTS)
    include(CTest)
    enable_testing()
    include(GoogleTest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    add_subdirectory(tests)
endif()

# Application
add_subdirectory(app)

# Benchmarks
if (PCS_RUN_BENCHMARKS) 
    add_subdirectory(benchmarks)
endif()

# Output directories
set_target_properties(pcs pcs_cli 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)