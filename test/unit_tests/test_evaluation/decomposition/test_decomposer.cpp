//
// MIT License
//
// Copyright (c) 2022 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "bugspray/utility/macros/macro_warning_suppression.hpp"

BUGSPRAY_DISABLE_WARNING_PUSH
BUGSPRAY_DISABLE_WARNING_UNUSED_VALUE

#include "bugspray/test_evaluation/decomposition/decomposer.hpp"
#include "bugspray/test_evaluation/decomposition/decomposition_result.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("decomposer", "[test_evaluation][decomposition]")
{
#define MAKE_SECTION(prefix, ...)                                                                                      \
    SECTION(#__VA_ARGS__)                                                                                              \
    {                                                                                                                  \
        prefix##CHECK(decomposition_result{(decomposer{} % __VA_ARGS__)}.result() == (__VA_ARGS__));                   \
        prefix##CHECK(decomposition_result{(decomposer{} % __VA_ARGS__)}.str() == #__VA_ARGS__);                       \
    }

    SECTION("unary")
    {
#define MAKE_SET(prefix)                                                                                               \
    MAKE_SECTION(prefix, 5)                                                                                            \
    MAKE_SECTION(prefix, false)

        MAKE_SET();
        MAKE_SET(STATIC_);
#undef MAKE_SET
    }

    SECTION("binary")
    {

        // clang-format off
#define MAKE_SET(prefix)                                                                                               \
    MAKE_SECTION(prefix, 1 * 2)                                                                                        \
    MAKE_SECTION(prefix, 4 / 2)                                                                                        \
    MAKE_SECTION(prefix, 3 % 2)                                                                                        \
    MAKE_SECTION(prefix, 3 + 2)                                                                                        \
    MAKE_SECTION(prefix, 3 - 2)                                                                                        \
    MAKE_SECTION(prefix, 1 << 2)                                                                                       \
    MAKE_SECTION(prefix, 16 >> 2)                                                                                      \
    MAKE_SECTION(prefix, 3 < 2)                                                                                        \
    MAKE_SECTION(prefix, 3 <= 2)                                                                                       \
    MAKE_SECTION(prefix, 3 > 2)                                                                                        \
    MAKE_SECTION(prefix, 3 >= 2)                                                                                       \
    MAKE_SECTION(prefix, 3 == 2)                                                                                       \
    MAKE_SECTION(prefix, 3 != 2)                                                                                       \
    MAKE_SECTION(prefix, 1 & 2)                                                                                        \
    MAKE_SECTION(prefix, 1 ^ 2)                                                                                        \
    MAKE_SECTION(prefix, 1 | 2)                                                                                        \
    MAKE_SECTION(prefix, true && false)                                                                                \
    MAKE_SECTION(prefix, true || false)                                                                                \
    MAKE_SECTION(prefix, 1 , 3)
        // clang-format on

        MAKE_SET()
        MAKE_SET(STATIC_)

#undef MAKE_SET
#undef MAKE_SECTION
    }
}

BUGSPRAY_DISABLE_WARNING_POP