#
# MIT License
#
# Copyright (c) 2021 Jan Möller
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
cmake_minimum_required(VERSION 3.14)

project(bugspray-tests LANGUAGES CXX)

include(CTest)
enable_testing()

include(../cmake/CPM.cmake)
CPMAddPackage(
        NAME bugspray
        SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..
)
CPMAddPackage("gh:catchorg/Catch2@3.3.2")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})

function(add_build_test name)
    set(target all)
    set(folder ${name})
    add_test(NAME ${name}
            COMMAND ${CMAKE_CTEST_COMMAND}
            --build-and-test
            ${CMAKE_CURRENT_LIST_DIR}/${folder}
            ${CMAKE_CURRENT_BINARY_DIR}/${folder}
            --build-target ${target}
            --build-generator ${CMAKE_GENERATOR}
            --build-options
                -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                -DBUGSPRAY_CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
                -DBUGSPRAY_LIB_DIR:STR=$<TARGET_PROPERTY:bugspray-with-main,BINARY_DIR>
                -DBUGSPRAY_INCLUDE_DIR:STR=$<TARGET_PROPERTY:bugspray-with-main,INCLUDE_DIRECTORIES>
                -DBUGSPRAY_DONT_USE_STD_VECTOR:STR=${BUGSPRAY_DONT_USE_STD_VECTOR}
                -DBUGSPRAY_DONT_USE_STD_STRING:STR=${BUGSPRAY_DONT_USE_STD_STRING}
            --test-command ${CMAKE_CTEST_COMMAND} --verbose
            )
    if (${ARGC} GREATER 1)
        set_tests_properties(${name} PROPERTIES PASS_REGULAR_EXPRESSION ${ARGV1})
    endif ()
endfunction()

add_build_test(examples)
add_build_test(failing-examples/00-simple "42 == 0.*other_var: 6.*static assertion failed: Test evaluation failed.")
add_build_test(failing-examples/01-fibonacci "fib\\(i\\) == expected_values\\[i\\].*1 == 0.*i: 0.*static assertion failed: Test evaluation failed.")
add_build_test(failing-examples/02-invalid-tag-string "Invalid tag string")
add_build_test(failing-examples/03-utf8-codeunit-categorization "categorize_utf8_codeunit\\(0xF8u\\) == invalid.*00001110 == invalid")
add_build_test(failing-examples/04-undefined-behavior "undefined behavior.*outside the bounds of array")

add_subdirectory(unit_tests)

add_subdirectory(catch2-compatibility-tests)
