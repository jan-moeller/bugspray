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
add_test(NAME 00-fail-trivial
        COMMAND ${CMAKE_CTEST_COMMAND}
        --build-and-test
        ${CMAKE_CURRENT_LIST_DIR}/00-fail-trivial
        ${CMAKE_CURRENT_BINARY_DIR}/00-fail-trivial
        --build-generator ${CMAKE_GENERATOR}
        --build-options -DBUGSPRAY_LIB_DIR:STR=$<TARGET_PROPERTY:bugspray-with-main,BINARY_DIR> -DBUGSPRAY_INCLUDE_DIR:STR=$<TARGET_PROPERTY:bugspray-with-main,INCLUDE_DIRECTORIES>
        --test-command ${CMAKE_CTEST_COMMAND}
        )
set_tests_properties(00-fail-trivial PROPERTIES
        PASS_REGULAR_EXPRESSION "EVAL_TEST_CASE\\(\"this will fail\"\\).*FAIL\\(\\)"
        )