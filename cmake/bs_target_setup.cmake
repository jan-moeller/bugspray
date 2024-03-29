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
function(bs_target_setup target)
    set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_compile_options(${target} PRIVATE
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
    endif ()
    set_target_properties(${target} PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            )
    if (${BUGSPRAY_DONT_USE_STD_VECTOR})
        target_compile_definitions(${target} PUBLIC BUGSPRAY_DONT_USE_STD_VECTOR)
    endif ()
    if (${BUGSPRAY_DONT_USE_STD_STRING})
        target_compile_definitions(${target} PUBLIC BUGSPRAY_DONT_USE_STD_STRING)
    endif ()

endfunction()