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
#include "bugspray/utility/structural_tuple.hpp"

#include <catch2/catch_all.hpp>

#include <tuple>
#include <type_traits>

using namespace bs;

using TestTuple = structural_tuple<int, float, char>;

template<TestTuple Arg>
struct test_structurality
{
    static constexpr TestTuple value = Arg;
};

TEST_CASE("structural_tuple", "[utility]")
{

    SECTION("std::tuple_size")
    {
        STATIC_REQUIRE(std::tuple_size_v<TestTuple> == 3);
    }
    SECTION("std::tuple_element")
    {
        STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<0, TestTuple>, int>);
        STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<1, TestTuple>, float>);
        STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<2, TestTuple>, char>);
    }

    constexpr auto v1     = 42;
    constexpr auto v2     = 3.141f;
    constexpr auto v3     = 'a';
    constexpr auto alt_v3 = 'b';
    TestTuple      t{v1, v2, v3};

    SECTION("get")
    {
        SECTION("by index")
        {
            REQUIRE(get<0>(t) == v1);
            REQUIRE(get<1>(t) == v2);
            REQUIRE(get<2>(t) == v3);

            REQUIRE(get<0>(std::as_const(t)) == v1);
            REQUIRE(get<1>(std::as_const(t)) == v2);
            REQUIRE(get<2>(std::as_const(t)) == v3);

            REQUIRE(get<0>(TestTuple{v1, v2, v3}) == v1);
            REQUIRE(get<1>(TestTuple{v1, v2, v3}) == v2);
            REQUIRE(get<2>(TestTuple{v1, v2, v3}) == v3);

            REQUIRE(get<0>(static_cast<TestTuple const&&>(TestTuple{v1, v2, v3})) == v1);
            REQUIRE(get<1>(static_cast<TestTuple const&&>(TestTuple{v1, v2, v3})) == v2);
            REQUIRE(get<2>(static_cast<TestTuple const&&>(TestTuple{v1, v2, v3})) == v3);
        }
        SECTION("by type")
        {
            REQUIRE(get<std::decay_t<decltype(v1)>>(t) == v1);
            REQUIRE(get<std::decay_t<decltype(v2)>>(t) == v2);
            REQUIRE(get<std::decay_t<decltype(v3)>>(t) == v3);

            REQUIRE(get<std::decay_t<decltype(v1)>>(std::as_const(t)) == v1);
            REQUIRE(get<std::decay_t<decltype(v2)>>(std::as_const(t)) == v2);
            REQUIRE(get<std::decay_t<decltype(v3)>>(std::as_const(t)) == v3);

            REQUIRE(get<std::decay_t<decltype(v1)>>(TestTuple{v1, v2, v3}) == v1);
            REQUIRE(get<std::decay_t<decltype(v2)>>(TestTuple{v1, v2, v3}) == v2);
            REQUIRE(get<std::decay_t<decltype(v3)>>(TestTuple{v1, v2, v3}) == v3);

            REQUIRE(get<std::decay_t<decltype(v1)>>(static_cast<TestTuple const&&>(TestTuple{v1, v2, v3})) == v1);
            REQUIRE(get<std::decay_t<decltype(v2)>>(static_cast<TestTuple const&&>(TestTuple{v1, v2, v3})) == v2);
            REQUIRE(get<std::decay_t<decltype(v3)>>(static_cast<TestTuple const&&>(TestTuple{v1, v2, v3})) == v3);
        }
    }

    SECTION("structured binding support")
    {
        auto [a, b, c] = t;
        REQUIRE(a == v1);
        REQUIRE(b == v2);
        REQUIRE(c == v3);
    }

    SECTION("comparison")
    {
        REQUIRE(t == t);
        REQUIRE(structural_tuple(v3, v2, v1) != t);
        REQUIRE(structural_tuple(v1, v2, alt_v3) > t);
    }

    SECTION("structurality")
    {
        test_structurality<structural_tuple{v1, v2, v3}> some_t;
        REQUIRE(some_t.value == t);
    }
}