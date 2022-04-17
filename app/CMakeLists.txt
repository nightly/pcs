cmake_minimum_required (VERSION 3.8)

set(PCS_CLI_SOURCES "pcs.cpp" "examples/pad/pad.cpp" "examples/merge/merge.cpp")

add_executable(pcs_cli ${PCS_CLI_SOURCES})
target_compile_features(pcs_cli PRIVATE cxx_std_20)
target_link_libraries(pcs_cli PRIVATE pcs)

set_target_properties(pcs_cli 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)