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
project(bugspray-success-tests)

set(CMAKE_MODULE_PATH ${BUGSPRAY_CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})
include(bs_target_setup)
include(bs_test_add_imported_bs_lib)

function(add_success_test name)
    set(full_name success_tests-${name})
    add_executable(${full_name} ${name}.cpp)
    target_link_libraries(${full_name} PUBLIC bugspray-with-main)
    bs_target_setup(${full_name})

    add_test(NAME ${full_name} COMMAND ${full_name})
endfunction()

add_success_test(empty_both)
add_success_test(empty_runtime)
add_success_test(section_both)
add_success_test(section_runtime)
add_success_test(with_assertions_both)
add_success_test(with_assertions_runtime)
