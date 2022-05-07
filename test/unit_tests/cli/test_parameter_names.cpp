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
#include "bugspray/cli/parameter_names.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("parameter_names", "[cli]")
{
    SECTION("only short")
    {
        constexpr parameter_names p{"-s"};
        STATIC_REQUIRE(p.is_named);
        STATIC_REQUIRE(!p.is_unnamed);
        STATIC_REQUIRE(p.valid());
    }
    SECTION("only long")
    {
        constexpr parameter_names p{"-s", "--long"};
        STATIC_REQUIRE(p.is_named);
        STATIC_REQUIRE(!p.is_unnamed);
        STATIC_REQUIRE(p.valid());
    }
    SECTION("only positional")
    {
        constexpr parameter_names p{"positional"};
        STATIC_REQUIRE(!p.is_named);
        STATIC_REQUIRE(p.is_unnamed);
        STATIC_REQUIRE(p.valid());
    }
    SECTION("mixed")
    {
        constexpr parameter_names p{"-s", "q"};
        STATIC_REQUIRE(p.is_named);
        STATIC_REQUIRE(p.is_unnamed);
        STATIC_REQUIRE(!p.valid());
    }
    SECTION("empty string")
    {
        constexpr parameter_names p{""};
        STATIC_REQUIRE(!p.is_named);
        STATIC_REQUIRE(!p.is_unnamed);
        STATIC_REQUIRE(!p.valid());
    }
}