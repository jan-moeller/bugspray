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
add_build_test(00-fail-trivial)
set_tests_properties(00-fail-trivial PROPERTIES
        PASS_REGULAR_EXPRESSION "EVAL_TEST_CASE\\(\"this will fail\"\\).*FAIL\\(\\)"
        )

function(make_invalid_tags_testcase name)
    add_build_test(01-invalid-tags-testcase-${name}-both 01-invalid-tags-testcase-${name}-both 01-invalid-tags)
    set_tests_properties(01-invalid-tags-testcase-${name}-both PROPERTIES
            PASS_REGULAR_EXPRESSION "Invalid tag string"
            )

    add_build_test(01-invalid-tags-testcase-${name}-runtime 01-invalid-tags-testcase-${name}-runtime 01-invalid-tags)
    set_tests_properties(01-invalid-tags-testcase-${name}-runtime PROPERTIES
            PASS_REGULAR_EXPRESSION "Invalid tag string"
            )
endfunction()

make_invalid_tags_testcase(tag_not_closed)
make_invalid_tags_testcase(tag_not_opened)
make_invalid_tags_testcase(not_a_tag)
make_invalid_tags_testcase(tag_empty)