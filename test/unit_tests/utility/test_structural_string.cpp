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
#include "bugspray/utility/structural_string.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

template<structural_string S>
struct foo
{
    static constexpr std::string_view str = S.value;
};

TEST_CASE("structural_string", "[utility]")
{
    SECTION("default construct")
    {
        STATIC_REQUIRE(structural_string<15>{} == "");
        STATIC_REQUIRE(structural_string<15>{}.size() == 0);
    }
    SECTION("infer size from literal")
    {
        STATIC_REQUIRE(structural_string{"foobar"}.size() == 6);
        STATIC_REQUIRE(structural_string{"foobar"}.capacity() == 7);
    }
    SECTION("construct from string_view")
    {
        constexpr std::string_view const foo{"foo"};
        STATIC_REQUIRE(structural_string<9>{foo}.size() == foo.size());
        STATIC_REQUIRE(structural_string<9>{foo} == foo);
    }
    SECTION("compare structural_strings")
    {
        constexpr structural_string a{"foo"};
        constexpr structural_string b{"bar"};
        STATIC_REQUIRE(a != b);
    }
    SECTION("compare structural_strings to char const*")
    {
        constexpr structural_string a{"foo"};
        constexpr char const*       b{"bar"};
        STATIC_REQUIRE(a != b);
    }
    SECTION("compare structural_strings to string_view")
    {
        constexpr structural_string a{"foo"};
        constexpr std::string_view  b{"bar"};
        STATIC_REQUIRE(a != b);
    }
    SECTION("cast to string_view")
    {
        constexpr structural_string a{"foo"};
        auto const                  b = static_cast<std::string_view>(a);
        REQUIRE(a == b);
    }
    SECTION("usage as template argument")
    {
        STATIC_REQUIRE(foo<"bar">::str == "bar");
    }
    SECTION("simple string concatenation")
    {
        STATIC_REQUIRE(structural_string{"foo"} + structural_string{"bar"} == "foobar");
    }
    SECTION("trim")
    {
        STATIC_REQUIRE(trim_left<"  foo", ' '>() == "foo");
        STATIC_REQUIRE(trim_right<"foo  ", ' '>() == "foo");
        STATIC_REQUIRE(trim<"foo\0\0">() == "foo");
        STATIC_REQUIRE(trim_right<" a ", ' '>() == " a");
        STATIC_REQUIRE(trim_left<" a ", ' '>() == "a ");
        STATIC_REQUIRE(trim<" a ", ' '>() == "a");
    }
}