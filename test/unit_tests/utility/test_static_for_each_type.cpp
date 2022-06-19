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
#include "bugspray/utility/static_for_each_type.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("static_for_each_type", "[utility]")
{
    using namespace bs;

    int num_int        = 0;
    int num_float      = 0;
    int num_double     = 0;
    int num_unexpected = 0;

    static_for_each_type<int, float, double>(
        [&]<typename T>()
        {
            if constexpr (std::same_as<T, int>)
                ++num_int;
            else if constexpr (std::same_as<T, float>)
                ++num_float;
            else if constexpr (std::same_as<T, double>)
                ++num_double;
            else
                ++num_unexpected;
        });

    CHECK(num_int == 1);
    CHECK(num_float == 1);
    CHECK(num_double == 1);
    CHECK(num_unexpected == 0);
}