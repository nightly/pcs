cmake_minimum_required (VERSION 3.8)

# Dependencies
add_subdirectory("${PROJECT_SOURCE_DIR}/external/googletest" "external/googletest")

# Macro
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})

    target_compile_features(${TESTNAME}  PRIVATE cxx_std_20)

    target_link_libraries(${TESTNAME} 
    PRIVATE 
        pcs
        gtest 
        gmock
        gtest_main)

    gtest_discover_tests(${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

# Test Suite
package_add_test("lts" "lts/lts.cpp")
package_add_test("parser" "lts/parser.cpp")
package_add_test("merge" "lts/merge.cpp")