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
#include "bugspray/utility/detail/naive_string.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("naive_string", "[utility][detail]")
{
    SECTION("default construction leaves it empty")
    {
        STATIC_REQUIRE(naive_string{}.size() == 0);
        STATIC_REQUIRE(naive_string{}.empty());
    }
    SECTION("can push into it")
    {
        constexpr auto test = []()
        {
            naive_string v;
            v.push_back('a');
            v.push_back('b');
            return v;
        };
        STATIC_REQUIRE(test().size() == 2);
        STATIC_REQUIRE(test()[0] == 'a');
        STATIC_REQUIRE(test()[1] == 'b');
    }
    SECTION("can concatenate")
    {
        constexpr auto test = []()
        {
            naive_string v1 = "foo";
            naive_string v2 = "bar";
            return v1 + v2;
        };
        STATIC_REQUIRE(test().size() == 6);
        STATIC_REQUIRE(test() == "foobar");
    }
    SECTION("can compare for equality")
    {
        STATIC_REQUIRE(naive_string{"foo"} == naive_string{"foo"});
        STATIC_REQUIRE(naive_string{"foo"} != naive_string{"bar"});
    }
}