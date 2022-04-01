cmake_minimum_required (VERSION 3.8)

set(PCS_CLI_SOURCES "pcs.cpp" "examples/pad.cpp" "examples/merge.cpp")

add_executable(pcs_cli ${PCS_CLI_SOURCES})
target_compile_features(pcs_cli PRIVATE cxx_std_20)
target_link_libraries(pcs_cli PRIVATE pcs)

