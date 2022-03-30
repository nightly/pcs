cmake_minimum_required (VERSION 3.8)

set(PCS_SOURCES "lts/lts.cpp" "lts/state.cpp" "lts/parsers.cpp" "lts/writers.cpp"
"controller/topology.cpp" "controller/controller.cpp" "controller/realisability.cpp"
"product/recipe.cpp"  "product/parser.cpp"
"operation/composite.cpp" "operation/guard.cpp" "operation/operation.cpp"
"machine/machine.cpp" "machine/writers.cpp")

add_library(pcs STATIC ${PCS_SOURCES})

target_compile_features(pcs PRIVATE cxx_std_20)

target_include_directories(pcs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 
add_subdirectory("${PROJECT_SOURCE_DIR}/external/json" "external/json")
target_link_libraries(pcs PUBLIC nlohmann_json)