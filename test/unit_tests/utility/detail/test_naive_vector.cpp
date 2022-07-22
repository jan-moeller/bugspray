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
#include "bugspray/utility/detail/naive_vector.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("naive_vector", "[utility][detail]")
{
    SECTION("default construction leaves it empty")
    {
        STATIC_REQUIRE(naive_vector<int>{}.size() == 0);
        STATIC_REQUIRE(naive_vector<int>{}.empty());
    }
    SECTION("can push into it")
    {
        constexpr auto test = []()
        {
            naive_vector<int> v;
            v.push_back(1);
            v.emplace_back(2);
            return v;
        };
        STATIC_REQUIRE(test().size() == 2);
        STATIC_REQUIRE(test()[0] == 1);
        STATIC_REQUIRE(test()[1] == 2);
    }
    SECTION("can insert into it")
    {
        constexpr auto test = []()
        {
            naive_vector<int> v;
            v.insert(v.end(), 1);
            v.insert(v.begin(), 2);
            v.insert(v.end(), 3);
            v.insert(v.begin() + 1, 4);
            return v;
        };
        CAPTURE(test());
        STATIC_REQUIRE(test().size() == 4);
        STATIC_REQUIRE(test()[0] == 2);
        STATIC_REQUIRE(test()[1] == 4);
        STATIC_REQUIRE(test()[2] == 1);
        STATIC_REQUIRE(test()[3] == 3);
    }
    SECTION("can pop from it")
    {
        constexpr auto test = []()
        {
            naive_vector<int> v{1, 2, 3};
            v.pop();
            return v;
        };
        STATIC_REQUIRE(test().size() == 2);
        STATIC_REQUIRE(test()[0] == 1);
        STATIC_REQUIRE(test()[1] == 2);
    }
    SECTION("can compare for equality")
    {
        STATIC_REQUIRE(naive_vector<int>{1, 2, 3} == naive_vector<int>{1, 2, 3});
        STATIC_REQUIRE(naive_vector<int>{1, 2, 3} != naive_vector<int>{1, 2, 3, 4});
    }
    SECTION("can three-way compare")
    {
        STATIC_REQUIRE((naive_vector<int>{1, 2, 3} <=> naive_vector<int>{1, 2, 3}) == std::strong_ordering::equal);
        STATIC_REQUIRE((naive_vector<int>{1, 2, 3} <=> naive_vector<int>{1, 3}) == std::strong_ordering::less);
    }
    SECTION("can copy it")
    {
        constexpr auto test = []()
        {
            naive_vector<int> const v{1, 2, 3};
            naive_vector<int>       v2;
            v2 = v;
            return v2;
        };
        STATIC_REQUIRE(test().size() == 3);
        STATIC_REQUIRE(test()[0] == 1);
        STATIC_REQUIRE(test()[1] == 2);
        STATIC_REQUIRE(test()[2] == 3);
#if defined(__GNUC__) and __GNUC__ > 11
        // GCC 11 chokes on self-assignment
        SECTION("can self-assign")
        {
            constexpr auto test_self_assignment = []()
            {
                naive_vector<int> v{1, 2, 3};
                v = v;
                return v;
            };
            STATIC_REQUIRE(test_self_assignment() == naive_vector<int>{1, 2, 3});
        }
#endif
    }
    SECTION("can move it")
    {
        constexpr auto test = []()
        {
            naive_vector<int> v{1, 2, 3};
            naive_vector<int> v2;
            v2 = std::move(v);
            return v2;
        };
        STATIC_REQUIRE(test().size() == 3);
        STATIC_REQUIRE(test()[0] == 1);
        STATIC_REQUIRE(test()[1] == 2);
        STATIC_REQUIRE(test()[2] == 3);
        SECTION("can self-assign")
        {
            constexpr auto test_self_assignment = []()
            {
                naive_vector<int> v{1, 2, 3};
                std::swap(v, v);
                return v;
            };
            STATIC_REQUIRE((test_self_assignment(), true)); // The resulting state is unspecified but valid
        }
    }
}