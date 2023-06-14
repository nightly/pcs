cmake_minimum_required (VERSION 3.22)

set(BENCHMARK_ENABLE_TESTING OFF)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/googlebenchmark" "external/googlebenchmark")
set_property(DIRECTORY "${PROJECT_SOURCE_DIR}/external/googlebenchmark" APPEND
  PROPERTY COMPILE_DEFINITIONS "-DCMAKE_BUILD_TYPE=Release")

macro(package_add_benchmark BENCHNAME)
    add_executable(${BENCHNAME} ${ARGN})
    target_compile_features(${BENCHNAME}  PRIVATE cxx_std_20)

    target_link_libraries(${BENCHNAME} PRIVATE 
        pcs
        benchmark::benchmark
        benchmark::benchmark_main)

    set_target_properties(${BENCHNAME} PROPERTIES 
        FOLDER benchmarks
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    )
endmacro()

# Benchmark Suite
message("[PCS] Compiling benchmarks")

# ==================
#   Topology
# ==================

package_add_benchmark("topology-complete-range" "topology/complete/range.cpp")

# ==================
#   Controller
# ==================

package_add_benchmark("controller-pad" "controller/pad.cpp")
package_add_benchmark("controller-hinge" "controller/hinge.cpp")
package_add_benchmark("controller-hinge-resources" "controller/hinge_resources.cpp")
package_add_benchmark("controller-hinge-resources-best" "controller/hinge_resources_best.cpp")
package_add_benchmark("controller-test-models" "controller/test_models.cpp")

# ==================
#   Experimental
# ==================

package_add_benchmark("experimental" "experimental/experimental.cpp")