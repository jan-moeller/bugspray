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

add_library(bugspray STATIC
        include/bugspray/bugspray.hpp
        include/bugspray/cli_parser/argument_parser.hpp
        include/bugspray/cli_parser/cli_parser.hpp
        include/bugspray/cli_parser/named_argument.hpp
        include/bugspray/cli_parser/positional_argument.hpp
        include/bugspray/cli_parser/test_case_selector.hpp
        include/bugspray/reporter/terminal_reporter.hpp
        include/bugspray/section_tracker.hpp
        include/bugspray/test_case_registry.hpp
        include/bugspray/test_macros.hpp
        include/bugspray/test_macros_impl.hpp
        include/bugspray/test_run.hpp
        include/bugspray/test_runner.hpp
        include/bugspray/test_tags_t.hpp
        include/bugspray/utility/naive_string.hpp
        include/bugspray/utility/naive_vector.hpp
        include/bugspray/utility/string_literal.hpp
        include/bugspray/utility/warning_suppression.hpp
        src/reporter/terminal_reporter.cpp
        src/test_case_registry.cpp
        )
set_target_properties(bugspray PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(bugspray PUBLIC include/)
target_compile_options(bugspray PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wduplicated-cond
        -Wno-duplicated-branches
        -Wlogical-op
        -Wnull-dereference
        -Wno-old-style-cast
        -Wuseless-cast
        -Wdouble-promotion
        -Wshadow
        -Wformat=2
        -Wuninitialized
        )
set_target_properties(bugspray PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

add_library(bugspray-with-main STATIC src/main.cpp)
target_link_libraries(bugspray-with-main PUBLIC bugspray)
set_target_properties(bugspray-with-main PROPERTIES LINKER_LANGUAGE CXX)
target_compile_options(bugspray-with-main PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wduplicated-cond
        -Wno-duplicated-branches
        -Wlogical-op
        -Wnull-dereference
        -Wno-old-style-cast
        -Wuseless-cast
        -Wdouble-promotion
        -Wshadow
        -Wformat=2
        -Wuninitialized
        )
set_target_properties(bugspray-with-main PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )

add_subdirectory(test)