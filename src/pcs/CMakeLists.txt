cmake_minimum_required (VERSION 3.8)

set(PCS_SOURCES "lts/lts.h" "lts/state.h" "lts/writers.h"
"lts/parsers/string_string.cpp" "lts/parsers/string_operation.cpp"

"topology/topology.cpp" "topology/stitch.cpp" "topology/resource.cpp"

"controller/controller.cpp" "controller/highlighter.cpp"

"product/recipe.cpp"  "product/parser.cpp"

"operation/composite.cpp" "operation/guard.cpp" "operation/operation.cpp" "operation/transfer.cpp" "operation/nop.cpp"
"operation/label.cpp"

"machine/machine.cpp" "machine/writers.cpp"

"common/directory.cpp" "common/strings.cpp")

add_library(pcs STATIC ${PCS_SOURCES})

target_compile_features(pcs PRIVATE cxx_std_20)

target_include_directories(pcs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 
add_subdirectory("${PROJECT_SOURCE_DIR}/external/json" "external/json")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/spdlog" "external/spdlog")
target_link_libraries(pcs 
    PUBLIC 
        nlohmann_json
        spdlog::spdlog
)

set_target_properties(pcs
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)