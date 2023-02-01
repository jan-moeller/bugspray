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
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(bugspray-unit-tests
        cli/test_argument_destination.cpp
        cli/test_argument_parser.cpp
        cli/test_parameter_names.cpp
        reporter/test_caching_reporter.cpp
        test_evaluation/decomposition/test_decomposer.cpp
        test_evaluation/test_evaluate_test_case_asserting_function.cpp
        test_evaluation/test_evaluate_test_case_basic.cpp
        test_evaluation/test_evaluate_test_case_section_constraints.cpp
        test_evaluation/test_evaluate_test_case_target.cpp
        test_evaluation/test_evaluate_test_case_with_loops.cpp
        test_evaluation/test_info_capture.cpp
        test_evaluation/test_parse_tag_string.cpp
        test_evaluation/test_test_case_filter.cpp
        test_evaluation/test_test_case_topology.cpp
        test_evaluation/test_test_run_data.cpp
        to_string/test_char_to_printable_string.cpp
        to_string/test_stringify.cpp
        to_string/test_to_string_bool.cpp
        to_string/test_to_string_char.cpp
        to_string/test_to_string_container.cpp
        to_string/test_to_string_floating_point.cpp
        to_string/test_to_string_integral.cpp
        to_string/test_to_string_optional.cpp
        to_string/test_to_string_pointer.cpp
        to_string/test_to_string_string_like.cpp
        to_string/test_to_string_variant.cpp
        utility/detail/test_naive_string.cpp
        utility/detail/test_naive_vector.cpp
        utility/macros/test_get_nth_arg.cpp
        utility/macros/test_get_nth_arg_or.cpp
        utility/macros/test_get_tail.cpp
        utility/macros/test_unique_identifier.cpp
        utility/macros/test_unwrap.cpp
        utility/test_source_location.cpp
        utility/test_static_for_each_type.cpp
        utility/test_stringify_typename.cpp
        utility/test_structural_string.cpp
        utility/test_structural_tuple.cpp
        )
target_link_libraries(bugspray-unit-tests PRIVATE bugspray Catch2::Catch2WithMain)
bs_target_setup(bugspray-unit-tests)
set_target_properties(bugspray-unit-tests PROPERTIES COMPILE_FLAGS -save-temps)

catch_discover_tests(bugspray-unit-tests)
