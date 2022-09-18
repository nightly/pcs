cmake_minimum_required (VERSION 3.22)

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
    set_target_properties(${TESTNAME} PROPERTIES 
        FOLDER tests
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/tests")
endmacro()

# Test Suite
message("[PCS] Compiling tests")

package_add_test("product-lts-parser" "product/parser.cpp")

package_add_test("topology-complete" "topology/complete.cpp")

package_add_test("controller" "controller/controller.cpp")
package_add_test("unify" "controller/unify.cpp")
package_add_test("best-controller" "controller/best.cpp")
package_add_test("controller-parts" "controller/parts.cpp")
package_add_test("controller-parse-costs" "controller/parse_costs.cpp")

package_add_test("composite-operation" "operation/composite.cpp")
package_add_test("operation-label-parse" "operation/parsers/label.cpp")
package_add_test("parameterizedOp-parse" "operation/parsers/parameterized_op.cpp")