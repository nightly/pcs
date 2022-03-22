cmake_minimum_required (VERSION 3.8)

add_executable(pcs_cli "pcs.cpp")

target_compile_features(pcs_cli PRIVATE cxx_std_20)

target_link_libraries(pcs_cli PRIVATE pcs)

