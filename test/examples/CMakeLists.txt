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
project(bugspray-examples-00-trivial)

message(STATUS "BUGSPRAY_CMAKE_MODULE_PATH: ${BUGSPRAY_CMAKE_MODULE_PATH}")

set(CMAKE_MODULE_PATH ${BUGSPRAY_CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})
include(bs_target_setup)
include(bs_import_lib_for_testing)
include(CTest)

bs_import_lib_for_testing()

function(register_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PUBLIC bugspray bugspray-with-main)
    bs_target_setup(${name})
    add_test(NAME ${name} COMMAND ${name})
endfunction()

register_test(00-trivial)
register_test(01-sections)
register_test(02-section-independence)
register_test(03-sections-in-loops)
register_test(04-partially-constexpr-type)
register_test(05-exceptions)
register_test(06-testing-templates)
