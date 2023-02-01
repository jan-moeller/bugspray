//
// MIT License
//
// Copyright (c) 2023 Jan Möller
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
#include "bugspray/to_string/stringify.hpp"
#include "bugspray/to_string/to_string_variant.hpp"

#include <catch2/catch_all.hpp>

#include <variant>

TEST_CASE("to_string(variant)", "[to_string]")
{
#define MAKE_TESTS(PREFIX)                                                                                             \
    PREFIX##CHECK(bs::to_string(std::variant<int, bool>{42}) == R"({ 42 })");                                          \
    PREFIX##CHECK(bs::to_string(std::variant<int, bool>{false}) == R"({ false })");                                    \
    PREFIX##CHECK(bs::to_string(std::variant<int, std::variant<bool>>{std::variant<bool>{false}})                      \
                  == R"({ { false } })");

    MAKE_TESTS()
    MAKE_TESTS(STATIC_)
#undef MAKE_TESTS
}