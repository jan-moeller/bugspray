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
project(bugspray
        VERSION 0.4.1
        LANGUAGES CXX
)

#############################################################################################################
# Prevent in-source builds
#############################################################################################################
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

#############################################################################################################
# Dependencies
#############################################################################################################
include(cmake/CPM.cmake)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.11.0")

#############################################################################################################
# Build configuration
#############################################################################################################
option(BUGSPRAY_DONT_USE_STD_VECTOR "Forces bugspray to use its own vector implementation rather than std::vector" OFF)
option(BUGSPRAY_DONT_USE_STD_STRING "Forces bugspray to use its own string implementation rather than std::string" OFF)

message(STATUS "------------------------------------------------------------------------------")
message(STATUS "    ${PROJECT_NAME} (${PROJECT_VERSION})")
message(STATUS "------------------------------------------------------------------------------")
message(STATUS "Build type:                ${CMAKE_BUILD_TYPE}")
message(STATUS "DONT_USE_STD_VECTOR:       ${BUGSPRAY_DONT_USE_STD_VECTOR}")
message(STATUS "DONT_USE_STD_STRING:       ${BUGSPRAY_DONT_USE_STD_STRING}")

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
#############################################################################################################
# Targets
#############################################################################################################
include(cmake/bs_target_setup.cmake)
add_library(${PROJECT_NAME} STATIC
        include/bugspray/bugspray.hpp
        include/bugspray/cli/argument_destination.hpp
        include/bugspray/cli/argument_parser.hpp
        include/bugspray/cli/main_test_runner_argparser.hpp
        include/bugspray/cli/main_test_runner_config.hpp
        include/bugspray/cli/parameter.hpp
        include/bugspray/cli/parameter_names.hpp
        include/bugspray/cli/parsers/arg_parser.hpp
        include/bugspray/cli/parsers/parse_bool.hpp
        include/bugspray/cli/parsers/parse_integral.hpp
        include/bugspray/cli/parsers/parse_string_view.hpp
        include/bugspray/macro_interface/asserting_function_macros.hpp
        include/bugspray/macro_interface/assertion_macros.hpp
        include/bugspray/macro_interface/capture_macro.hpp
        include/bugspray/macro_interface/section_macro.hpp
        include/bugspray/macro_interface/test_case_macros.hpp
        include/bugspray/reporter/caching_reporter.hpp
        include/bugspray/reporter/constexpr_reporter.hpp
        include/bugspray/reporter/detail/runtime_stopwatch.hpp
        include/bugspray/reporter/formatted_ostream_reporter.hpp
        include/bugspray/reporter/noop_reporter.hpp
        include/bugspray/reporter/reporter.hpp
        include/bugspray/reporter/xml_reporter.hpp
        include/bugspray/test_evaluation/decomposition/binary_expr.hpp
        include/bugspray/test_evaluation/decomposition/decomposer.hpp
        include/bugspray/test_evaluation/decomposition/decomposition_result.hpp
        include/bugspray/test_evaluation/decomposition/unary_expr.hpp
        include/bugspray/test_evaluation/evaluate_test_case.hpp
        include/bugspray/test_evaluation/evaluate_test_case_target.hpp
        include/bugspray/test_evaluation/info_capture.hpp
        include/bugspray/test_evaluation/parse_tag_string.hpp
        include/bugspray/test_evaluation/section_path.hpp
        include/bugspray/test_evaluation/section_tracker.hpp
        include/bugspray/test_evaluation/test_case.hpp
        include/bugspray/test_evaluation/test_case_filter.hpp
        include/bugspray/test_evaluation/test_case_fn.hpp
        include/bugspray/test_evaluation/test_case_topology.hpp
        include/bugspray/test_evaluation/test_run_data.hpp
        include/bugspray/test_registration/test_case_registry.hpp
        include/bugspray/to_string/char_to_printable_string.hpp
        include/bugspray/to_string/stringify.hpp
        include/bugspray/to_string/to_string_bool.hpp
        include/bugspray/to_string/to_string_byte.hpp
        include/bugspray/to_string/to_string_char.hpp
        include/bugspray/to_string/to_string_container.hpp
        include/bugspray/to_string/to_string_floating_point.hpp
        include/bugspray/to_string/to_string_integral.hpp
        include/bugspray/to_string/to_string_optional.hpp
        include/bugspray/to_string/to_string_pair.hpp
        include/bugspray/to_string/to_string_pointer.hpp
        include/bugspray/to_string/to_string_string_like.hpp
        include/bugspray/to_string/to_string_tag.hpp
        include/bugspray/to_string/to_string_tuple.hpp
        include/bugspray/to_string/to_string_variant.hpp
        include/bugspray/utility/c_array.hpp
        include/bugspray/utility/character.hpp
        include/bugspray/utility/dependent_false.hpp
        include/bugspray/utility/detail/naive_string.hpp
        include/bugspray/utility/detail/naive_vector.hpp
        include/bugspray/utility/detail/structural_tuple_impl.hpp
        include/bugspray/utility/isspace.hpp
        include/bugspray/utility/macros.hpp
        include/bugspray/utility/macros/macro_concatenate.hpp
        include/bugspray/utility/macros/macro_get_nth_arg.hpp
        include/bugspray/utility/macros/macro_get_nth_arg_or.hpp
        include/bugspray/utility/macros/macro_get_tail.hpp
        include/bugspray/utility/macros/macro_identity.hpp
        include/bugspray/utility/macros/macro_stringify.hpp
        include/bugspray/utility/macros/macro_unique_identifier.hpp
        include/bugspray/utility/macros/macro_unwrap.hpp
        include/bugspray/utility/macros/macro_warning_suppression.hpp
        include/bugspray/utility/source_location.hpp
        include/bugspray/utility/static_for.hpp
        include/bugspray/utility/static_for_each_type.hpp
        include/bugspray/utility/string.hpp
        include/bugspray/utility/stringify_typename.hpp
        include/bugspray/utility/structural_string.hpp
        include/bugspray/utility/structural_tuple.hpp
        include/bugspray/utility/trim.hpp
        include/bugspray/utility/vector.hpp
        include/bugspray/utility/xml_writer.hpp
        src/reporter/detail/runtime_stopwatch.cpp
        src/reporter/formatted_ostream_reporter.cpp
        src/reporter/xml_reporter.cpp
        src/utility/xml_writer.cpp
)
target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
bs_target_setup(${PROJECT_NAME})

add_library(${PROJECT_NAME}-with-main STATIC src/main.cpp)
target_link_libraries(${PROJECT_NAME}-with-main PUBLIC ${PROJECT_NAME})
bs_target_setup(${PROJECT_NAME}-with-main)

#############################################################################################################
# Make it installable
#############################################################################################################
string(TOLOWER ${PROJECT_NAME}/version.hpp VERSION_HEADER_LOCATION)
packageProject(
        NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        NAMESPACE ${PROJECT_NAME}
        BINARY_DIR ${PROJECT_BINARY_DIR}
        INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
        INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
        VERSION_HEADER "${VERSION_HEADER_LOCATION}"
        COMPATIBILITY SameMajorVersion
        DEPENDENCIES ""
)
