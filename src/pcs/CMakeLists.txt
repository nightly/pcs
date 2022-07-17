cmake_minimum_required (VERSION 3.22)

set(PCS_SOURCES

"topology/topology.h" "topology/core.cpp" "topology/complete.cpp" "topology/incremental.cpp" 
 "topology/stitch.cpp" 

"controller/solvers/dfs.cpp" "controller/solvers/best.cpp" 
"controller/core.cpp" "controller/plan_transition.cpp" "controller/parts.cpp"
"controller/highlighter.cpp"

"product/recipe.cpp"  "product/parsers/recipe.cpp"

"operation/operation.h"  "operation/composite.cpp" "operation/guard.cpp" "operation/task_expression.cpp"
"operation/observable.cpp" "operation/transfer.cpp" "operation/transfer_hash.h" "operation/nop.cpp"
"operation/parsers/label.cpp" 

"environment/environment.cpp" "environment/writers.cpp"

"common/directory.cpp" "common/strings.cpp" "common/empty.h" "common/pch.h")

add_library(pcs STATIC ${PCS_SOURCES})

target_compile_features(pcs PRIVATE cxx_std_20)

target_include_directories(pcs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

if (PCS_PCH)
    target_precompile_headers(pcs 
        PUBLIC
         "${CMAKE_CURRENT_SOURCE_DIR}/common/pch.h"
    )
endif()


# ==================
#   Dependencies
# ==================

# =========
#   Internal
# =========

add_subdirectory("${PROJECT_SOURCE_DIR}/src/pcs/lts" "src/pcs/lts")

# =================
#  General
# =================

## add_subdirectory("${PROJECT_SOURCE_DIR}/external/json" "external/json") # public dependency of lts
add_subdirectory("${PROJECT_SOURCE_DIR}/external/spdlog" "external/spdlog")

# =========
#   Boost
# =========
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/core" "external/boost/core")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/container_hash" "external/boost/container_hash")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/assert" "external/boost/assert")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/config" "external/boost/config")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/detail" "external/boost/detail")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/integer" "external/boost/integer")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/static_assert" "external/boost/static_assert")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/type_traits" "external/boost/type_traits")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/preprocessor" "external/boost/preprocessor")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/exception" "external/boost/exception")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/throw_exception" "external/boost/throw_exception")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/smart_ptr" "external/boost/smart_ptr")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/tuple" "external/boost/tuple")
add_subdirectory("${PROJECT_SOURCE_DIR}/external/boost/move" "external/boost/move")


target_link_libraries(pcs 
    PUBLIC 
        lts
        nlohmann_json
        spdlog::spdlog
        Boost::container_hash
)


set_target_properties(pcs
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

