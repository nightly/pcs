cmake_minimum_required (VERSION 3.8)

add_executable(pcs_app "pcs.cpp")

target_compile_features(pcs_app PRIVATE cxx_std_20)

target_link_libraries(pcs_app PRIVATE pcs)

