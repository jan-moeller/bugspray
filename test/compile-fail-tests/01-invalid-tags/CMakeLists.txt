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
cmake_minimum_required(VERSION 3.20)
project(bugspray-compile-fail-tests-01-invalid-tags)

add_library(bugspray-with-main STATIC IMPORTED)
set_target_properties(bugspray-with-main PROPERTIES
        IMPORTED_LOCATION ${BUGSPRAY_LIB_DIR}/libbugspray-with-main.a
        INTERFACE_INCLUDE_DIRECTORIES ${BUGSPRAY_INCLUDE_DIR}
        INTERFACE_LINK_LIBRARIES ${BUGSPRAY_LIB_DIR}/libbugspray.a
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

function(create_executable name TAG_STRING QUALIFIER)
    configure_file(main.cpp.in main_${name}.cpp @ONLY)
    add_executable(01-invalid-tags-testcase-${name} ${CMAKE_CURRENT_BINARY_DIR}/main_${name}.cpp)
    target_link_libraries(01-invalid-tags-testcase-${name} PUBLIC bugspray-with-main)
    set_target_properties(01-invalid-tags-testcase-${name} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            )
endfunction()

function(create_test_executables name TAG_STRING)
    create_executable(${name}-both ${TAG_STRING} "both")
    create_executable(${name}-runtime ${TAG_STRING} "runtime")
endfunction()

create_test_executables(tag_not_closed "[")
create_test_executables(tag_not_opened "]")
create_test_executables(not_a_tag "not a tag")
create_test_executables(tag_empty "[]")
