#
# MIT License
#
# Copyright (c) 2022 Jan Möller
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

set(BUGSPRAY_TEST_FRAMEWORK_HEADER "bugspray/bugspray.hpp")
configure_file(test_catch_compatibility.cpp.in test_catch_compatibility_bugspray.cpp @ONLY)
set(BUGSPRAY_TEST_FRAMEWORK_HEADER "catch2/catch_all.hpp")
configure_file(test_catch_compatibility.cpp.in test_catch_compatibility_catch.cpp @ONLY)

add_executable(bugspray_test_catch_compatbility_bugspray ${CMAKE_CURRENT_BINARY_DIR}/test_catch_compatibility_bugspray.cpp)
target_link_libraries(bugspray_test_catch_compatbility_bugspray PUBLIC bugspray-with-main)
bs_target_setup(bugspray_test_catch_compatbility_bugspray)

add_executable(bugspray_test_catch_compatbility_catch ${CMAKE_CURRENT_BINARY_DIR}/test_catch_compatibility_catch.cpp)
target_link_libraries(bugspray_test_catch_compatbility_catch PUBLIC Catch2::Catch2WithMain)

add_custom_command(
        TARGET bugspray_test_catch_compatbility_bugspray
        POST_BUILD
        COMMAND bugspray_test_catch_compatbility_bugspray -r xml --rng-seed 0 -o ${CMAKE_CURRENT_BINARY_DIR}/bugspray.out
        VERBATIM)
add_custom_command(
        TARGET bugspray_test_catch_compatbility_catch
        POST_BUILD
        COMMAND bugspray_test_catch_compatbility_catch -r xml --rng-seed 0 -o ${CMAKE_CURRENT_BINARY_DIR}/catch.out
        VERBATIM)

add_test(NAME catch2-compatibility-tests
        COMMAND python3 ${CMAKE_CURRENT_LIST_DIR}/compare_xml_output.py ${CMAKE_CURRENT_BINARY_DIR}/bugspray.out ${CMAKE_CURRENT_BINARY_DIR}/catch.out
        )
