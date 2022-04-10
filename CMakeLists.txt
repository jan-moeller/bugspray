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
project(bugspray)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(bs_target_setup)

add_library(bugspray STATIC
        include/bugspray/macro_interface/assertion_macros.hpp
        include/bugspray/macro_interface/section_macro.hpp
        include/bugspray/reporter/caching_reporter.hpp
        include/bugspray/reporter/noop_reporter.hpp
        include/bugspray/reporter/reporter.hpp
        include/bugspray/test_evaluation/section_path.hpp
        include/bugspray/test_evaluation/section_tracker.hpp
        include/bugspray/test_evaluation/test_case.hpp
        include/bugspray/test_evaluation/test_case_fn.hpp
        include/bugspray/test_evaluation/test_case_topology.hpp
        include/bugspray/test_evaluation/test_run_data.hpp
        include/bugspray/utility/compiletime_string.hpp
        include/bugspray/utility/detail/naive_string.hpp
        include/bugspray/utility/detail/naive_vector.hpp
        include/bugspray/utility/macros.hpp
        include/bugspray/utility/macros/concatenate.hpp
        include/bugspray/utility/macros/get_nth_arg.hpp
        include/bugspray/utility/macros/get_nth_arg_or.hpp
        include/bugspray/utility/macros/get_tail.hpp
        include/bugspray/utility/macros/identity.hpp
        include/bugspray/utility/macros/stringify.hpp
        include/bugspray/utility/macros/unique_identifier.hpp
        include/bugspray/utility/macros/warning_suppression.hpp
        include/bugspray/utility/source_location.hpp
        include/bugspray/utility/string.hpp
        include/bugspray/utility/vector.hpp
        )
target_include_directories(bugspray PUBLIC include/)
bs_target_setup(bugspray)

enable_testing()
add_subdirectory(test)
