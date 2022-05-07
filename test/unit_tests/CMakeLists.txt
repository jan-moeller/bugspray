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
include(FetchContent)

fetchcontent_declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG 797c3e731883bfb80dff5e237f2559232314310c
)
fetchcontent_makeavailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(bugspray-unit-tests
        cli/test_argument_destination.cpp
        cli/test_argument_parser.cpp
        cli/test_parameter_names.cpp
        reporter/test_caching_reporter.cpp
        test_evaluation/decomposition/test_decomposer.cpp
        test_evaluation/test_evaluate_test_case.cpp
        test_evaluation/test_evaluate_test_case_target.cpp
        test_evaluation/test_parse_tag_string.cpp
        test_evaluation/test_test_case_topology.cpp
        test_evaluation/test_test_run_data.cpp
        to_string/test_char_to_printable_string.cpp
        to_string/test_stringify.cpp
        to_string/test_to_string_bool.cpp
        to_string/test_to_string_char.cpp
        to_string/test_to_string_container.cpp
        to_string/test_to_string_integral.cpp
        to_string/test_to_string_string_like.cpp
        utility/detail/test_naive_string.cpp
        utility/detail/test_naive_vector.cpp
        utility/macros/test_get_nth_arg.cpp
        utility/macros/test_get_nth_arg_or.cpp
        utility/macros/test_get_tail.cpp
        utility/macros/test_unique_identifier.cpp
        utility/test_source_location.cpp
        utility/test_structural_string.cpp
        utility/test_structural_tuple.cpp
        )
target_link_libraries(bugspray-unit-tests PRIVATE bugspray Catch2::Catch2WithMain)
bs_target_setup(bugspray-unit-tests)

catch_discover_tests(bugspray-unit-tests)
