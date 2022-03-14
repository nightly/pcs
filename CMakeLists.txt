cmake_minimum_required (VERSION 3.8)

project ("pcs" VERSION 0 LANGUAGES CXX)

option(PCS_RUN_TESTS "Run tests" ON)

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