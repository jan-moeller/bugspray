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
#include "bugspray/utility/stringify_typename.hpp"
#include "bugspray/utility/structural_string.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

namespace foobar
{
struct baz;
}

TEST_CASE("stringify_typename", "[utility]")
{
    STATIC_CHECK(stringify_typename<int>() == "int");
    STATIC_CHECK(stringify_typename<int*>() == "int*" || stringify_typename<int*>() == "int *");
    STATIC_CHECK(stringify_typename<int[3]>() == "int [3]" || stringify_typename<int[3]>() == "int[3]");
    STATIC_CHECK(stringify_typename<const int>() == "const int");
    STATIC_CHECK(stringify_typename<const int*>() == "const int*" || stringify_typename<const int*>() == "const int *");
    STATIC_CHECK(stringify_typename<const int&>() == "const int&" || stringify_typename<const int&>() == "const int &");
    STATIC_CHECK(stringify_typename<const int* const>() == "const int* const" || stringify_typename<const int* const>() == "const int *const");
    STATIC_CHECK(stringify_typename<const volatile int* const>() == "const volatile int* const" || stringify_typename<const volatile int* const>() == "const volatile int *const");
    STATIC_CHECK(stringify_typename<foobar::baz>() == "foobar::baz");
    STATIC_CHECK(stringify_typename<std::vector<int>>() == "std::vector<int>");

    constexpr auto lifetime_test = []
    {
        std::string_view name_of_int;
        {
            constexpr std::string_view typename_of_int = stringify_typename<int, std::string_view>();
            name_of_int                                = typename_of_int;
        }
        return name_of_int;
    };
    STATIC_CHECK(lifetime_test() == "int");
    STATIC_CHECK(stringify_typename<int>() == "int");
}