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
#include "bugspray/bugspray.hpp"

#include <map>

// Simulate a partially constexpr-enabled struct
struct foo
{
    constexpr explicit foo(int i)
        : m_i(i)
    {
    }

    constexpr auto get() -> int { return m_i; }
    void           set(int i) { m_i = i; }

    int m_i;
};

TEST_CASE("foo", "[important][partially constexpr]")
{
    foo f(42);
    SECTION("getter returns expected value")
    {
        REQUIRE(f.get() == 42);
    }
    SECTION("can set value", runtime) // Will be skipped at compiletime
    {
        f.set(9);
        REQUIRE(f.get() == 9);
    }
}
EVAL_TEST_CASE("foo");

TEST_CASE("std::map", "[std]", runtime) // Won't be executed at compile time at all
{
    std::map<char, int> m;
    SECTION("size returns 0 after construction")
    {
        REQUIRE(m.size() == 0);
    }
}