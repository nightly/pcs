cmake_minimum_required (VERSION 3.8)

set(PCS_SOURCES "lts/lts.h" "lts/state.h" "lts/writers.h"
"lts/parsers/string_string.cpp" "lts/parsers/string_operation.cpp"
"topology/topology.cpp" "topology/stitch.cpp"
"controller/controller.cpp" 
"product/recipe.cpp"  "product/parser.cpp"
"operation/composite.cpp" "operation/guard.cpp" "operation/operation.cpp" "operation/transfer.cpp" "operation/nop.cpp"
"common/directory.cpp" "common/strings.cpp"
"machine/machine.cpp" "machine/writers.cpp")

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