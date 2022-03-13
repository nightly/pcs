cmake_minimum_required (VERSION 3.8)

set(PCS_SOURCES "pcs/lts/lts.cpp"
"pcs/lts/state.cpp"
"pcs/lts/merge.cpp"
"pcs/lts/parser.cpp"
)

add_library(pcs STATIC ${PCS_SOURCES}  "pcs/lts/merge.h" "pcs/lts/merge.cpp")

target_compile_features(pcs PRIVATE cxx_std_23)

target_include_directories(pcs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# Dependencies 
