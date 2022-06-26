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
#include "bugspray/utility/static_for_each_type.hpp"
#include "bugspray/utility/stringify_typename.hpp"

#include <concepts>
#include <limits>

template<typename T>
    requires(std::is_arithmetic_v<T>)
constexpr auto my_abs(T value) -> T
{
    if (value < 0)
        return -value;
    return value;
}

TEST_CASE("template test")
{
    using namespace bs;
    static_for_each_type<signed int, unsigned int, float, double>(
        [&]<typename T>()
        {
            DYNAMIC_SECTION("my_abs(" + stringify_typename<T>() + ")")
            {
                CHECK(my_abs(0) == 0);
                CHECK(my_abs(std::numeric_limits<T>::max()) == std::numeric_limits<T>::max());
                if constexpr (std::signed_integral<T>)
                    CHECK(my_abs(std::numeric_limits<T>::lowest() + 1) == std::numeric_limits<T>::max());
                if constexpr (std::floating_point<T>)
                    CHECK(my_abs(std::numeric_limits<T>::lowest()) == std::numeric_limits<T>::max());
            }
        });

    static_for_each_type<std::string_view, std::string>(
        [&]<typename T>()
        {
            DYNAMIC_SECTION("v.size() (" + stringify_typename<T>() + ")", runtime_if(std::same_as<T, std::string>))
            {
                if (std::is_constant_evaluated() && std::same_as<T, std::string>)
                    FAIL(); // Will never get called due to runtime_if() above
                CHECK(T{"foo"}.size() == 3);
            }
            DYNAMIC_SECTION("v.remove_prefix() (" + stringify_typename<T>() + ")",
                            only_if(std::same_as<T, std::string_view>))
            {
                // Will only be executed with string_view due to only_if() above
                T s{"foo"};
                s.remove_prefix(2);
                CHECK(s == "o");
            }
        });
}
EVAL_TEST_CASE("template test");
