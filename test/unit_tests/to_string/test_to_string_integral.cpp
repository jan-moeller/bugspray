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
#include "bugspray/to_string/to_string_integral.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("to_string(integral)", "[to_string]")
{
#define MAKE_TESTS(PREFIX)                                                                                             \
    PREFIX##CHECK(bs::to_string(0) == "0");                                                                            \
    PREFIX##CHECK(bs::to_string(1) == "1");                                                                            \
    PREFIX##CHECK(bs::to_string(10) == "10");                                                                          \
    PREFIX##CHECK(bs::to_string(-1) == "-1");                                                                          \
    PREFIX##CHECK(bs::to_string(-10) == "-10");                                                                        \
    PREFIX##CHECK(bs::to_string<16>(0) == "0");                                                                        \
    PREFIX##CHECK(bs::to_string<16>(1) == "1");                                                                        \
    PREFIX##CHECK(bs::to_string<16>(10) == "a");                                                                       \
    PREFIX##CHECK(bs::to_string<16>(-1) == "-1");                                                                      \
    PREFIX##CHECK(bs::to_string<16>(-10) == "-a");                                                                     \
    PREFIX##CHECK(bs::to_string<36>(35) == "z");                                                                       \
    PREFIX##CHECK(bs::to_string<16, 2>(3) == "03");                                                                    \
    PREFIX##CHECK(bs::to_string<10, 3>(std::integral_constant<int, 42>{}) == "042");

    auto const max = std::numeric_limits<std::uintmax_t>::max();
    auto const min = std::numeric_limits<std::intmax_t>::min();
    CHECK(bs::to_string(max) == bs::string{std::to_string(max)});
    CHECK(bs::to_string(min) == bs::string{std::to_string(min)});

    MAKE_TESTS()
    MAKE_TESTS(STATIC_)
#undef MAKE_TESTS
}