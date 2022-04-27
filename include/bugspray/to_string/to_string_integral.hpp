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

#ifndef BUGSPRAY_TO_STRING_INTEGRAL_HPP
#define BUGSPRAY_TO_STRING_INTEGRAL_HPP

#include "bugspray/utility/string.hpp"

#include <algorithm>
#include <array>
#include <concepts>
#include <utility>

#include <cstdint>

namespace bs
{
static constexpr std::array g_digits{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b',
                                     'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                     'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

template<unsigned Base = 10, unsigned MinDigits = 0>
constexpr auto to_string(std::integral auto i) -> bs::string
{
    static_assert(Base > 1 && Base < 37, "Base must be in the range of [2, 36]");

    bs::string     result;
    bool const     neg = std::cmp_less(i, 0);
    std::uintmax_t n   = neg ? static_cast<std::uintmax_t>(-(i + 1)) + 1 : i;
    do
    {
        auto const r = n % Base;
        result += g_digits[r];
        n /= Base;
    } while (n != 0);
    while (result.size() < MinDigits)
        result.push_back('0');
    if (neg)
        result += '-';
    std::ranges::reverse(result);
    return result;
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_INTEGRAL_HPP
