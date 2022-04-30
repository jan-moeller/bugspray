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
#include "bugspray/test_evaluation/parse_tag_string.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("parse_tag_string", "[test_evaluation]")
{
    constexpr structural_string empty_tags = "";
    STATIC_REQUIRE(parse_tag_string<empty_tags>().size() == 0);

    constexpr structural_string single_tag = "[a]";
    STATIC_REQUIRE(parse_tag_string<single_tag>().size() == 1);
    STATIC_REQUIRE(parse_tag_string<single_tag>()[0] == "a");

    constexpr structural_string three_tags = "[a][b][ab]";
    STATIC_REQUIRE(parse_tag_string<three_tags>().size() == 3);
    STATIC_REQUIRE(parse_tag_string<three_tags>()[0] == "a");
    STATIC_REQUIRE(parse_tag_string<three_tags>()[1] == "b");
    STATIC_REQUIRE(parse_tag_string<three_tags>()[2] == "ab");

    REQUIRE_THROWS_WITH((parse_tag_string<"[", true>()), //
                        Catch::Matchers::ContainsSubstring("Missing closing square bracket"));
    REQUIRE_THROWS_WITH((parse_tag_string<"]", true>()), //
                        Catch::Matchers::ContainsSubstring("Tags must be enclosed in square brackets"));
    REQUIRE_THROWS_WITH((parse_tag_string<"[]", true>()), //
                        Catch::Matchers::ContainsSubstring("Tags cannot be empty"));
    REQUIRE_THROWS_WITH((parse_tag_string<"[asd[]]", true>()), //
                        Catch::Matchers::ContainsSubstring("'[' is not allowed inside a tag"));
}