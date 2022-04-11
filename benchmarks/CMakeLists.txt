cmake_minimum_required (VERSION 3.8)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
add_subdirectory("${PROJECT_SOURCE_DIR}/external/googlebenchmark" "external/googlebenchmark")

macro(package_add_benchmark BENCHNAME)
    add_executable(${BENCHNAME} ${ARGN})
    target_compile_features(${BENCHNAME}  PRIVATE cxx_std_20)

    target_link_libraries(${BENCHNAME} PRIVATE 
        pcs
        benchmark::benchmark
        benchmark::benchmark_main)

    set_target_properties(${BENCHNAME} 
    PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/benchmarks/${CMAKE_BUILD_TYPE}")
endmacro()

# Benchmark Suite
package_add_benchmark("topology-generation" "controller/topology.cpp")