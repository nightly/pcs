cmake_minimum_required (VERSION 3.8)

set(PCS_SOURCES "lts/lts.cpp"
"lts/state.cpp"
"lts/merge.cpp"
"lts/parser.cpp"
 "lts/writers.cpp")

add_library(pcs STATIC ${PCS_SOURCES})

target_compile_features(pcs PRIVATE cxx_std_20)

target_include_directories(pcs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 