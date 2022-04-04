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

#ifndef BUGSPRAY_COMPILETIME_STRING_HPP
#define BUGSPRAY_COMPILETIME_STRING_HPP

#include <algorithm>
#include <string_view>

#include <cstdint>

/*
 * compiletime_string is a very simplistic string class that can be used as NTTP. It can be implicitly constructed from
 * literals.
 */

namespace bs
{
template<std::size_t N>
struct compiletime_string
{
    constexpr compiletime_string() { std::fill_n(value, N, '\0'); }
    constexpr compiletime_string(char const (&str)[N]) { std::copy_n(str, N, value); }
    char value[N];

    constexpr auto operator==(compiletime_string const& rhs) const -> bool = default;
    constexpr auto operator==(char const* rhs) const -> bool { return std::string_view{value} == rhs; }
};

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(compiletime_string<N1> lhs, compiletime_string<N2> rhs) -> compiletime_string<N1 + N2 - 1>
{
    compiletime_string<N1 + N2 - 1> result;
    std::copy_n(lhs.value, N1, result.value + 0);
    std::copy_n(rhs.value, N2, result.value + N1 - 1);
    return result;
}

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(compiletime_string<N1> lhs, char const (&rhs)[N2]) -> compiletime_string<N1 + N2 - 1>
{
    return lhs + compiletime_string<N2>{rhs};
}

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(char const (&lhs)[N1], compiletime_string<N2> rhs) -> compiletime_string<N1 + N2 - 1>
{
    return compiletime_string<N1>{lhs} + rhs;
}
} // namespace bs

#endif // BUGSPRAY_COMPILETIME_STRING_HPP
