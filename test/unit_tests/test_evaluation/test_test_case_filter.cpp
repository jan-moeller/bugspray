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
#include "bugspray/test_evaluation/test_case_filter.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("test_case_filter", "[test_evaluation]")
{
    SECTION("by name")
    {
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "*") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "**") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "*foo*") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "*oo") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "f*") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo bar", {}, {}, {}}, R"("f*")") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo bar", {}, {}, {}}, R"("*bar")") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo bar", {}, {}, {}}, R"("* *")") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo bar", {}, {}, {}}, R"("foo bar")") == true);
        STATIC_REQUIRE(test_case_filter(test_case{R"(foo "quote" bar)", {}, {}, {}}, R"("*"*")") == true);

        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "~foo") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "~*o") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "~f*") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "***") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, R"("~foo")") == false);

        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "~foo foo") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", {}, {}, {}}, "foo ~foo f*") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo bar", {}, {}, {}}, R"("foo" ~foo f*)") == true);
    }
    SECTION("by tags")
    {
        using namespace std::literals;
        constexpr std::array tags{"some"sv, "tags"sv};
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[some]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[*m*]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[*]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[so*]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[*me]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "~[tag]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[tag],[tags]") == true);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[tags][some]") == true);

        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[tag][tags]") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[tags] ~[some]") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[some] ~[tags]") == false);
        STATIC_REQUIRE(test_case_filter(test_case{"foo", tags, {}, {}}, "[some][tags][undefined]") == false);
    }
}