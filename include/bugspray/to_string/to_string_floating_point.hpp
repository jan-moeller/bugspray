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

#ifndef BUGSPRAY_TO_STRING_FLOATING_POINT_HPP
#define BUGSPRAY_TO_STRING_FLOATING_POINT_HPP

#include "bugspray/utility/string.hpp"

#include <array>
#include <charconv>
#include <concepts>
#include <string_view>
#include <type_traits>
#include <utility>

#include <cassert>
#include <cstdint>

namespace bs
{
namespace detail
{
constexpr auto log10ceil(auto num) -> int
{
    return num < 10 ? 1 : 1 + log10ceil(num / 10);
}
} // namespace detail

template<std::chars_format Format = std::chars_format::general, std::floating_point T>
constexpr auto to_string(T f) -> bs::string
{
    if (std::is_constant_evaluated()) // TODO: Implement for constexpr evaluation
        return "<?>";
    else
    {
        constexpr auto length = []
        {
            if constexpr (Format == std::chars_format::scientific || Format == std::chars_format::general)
                return 4 + std::numeric_limits<T>::max_digits10
                       + std::max(2, detail::log10ceil(std::numeric_limits<T>::max_exponent10));
            else if constexpr (Format == std::chars_format::fixed || Format == std::chars_format::hex)
                return 2 + std::numeric_limits<T>::max_exponent10 + std::numeric_limits<T>::max_digits10;
        }();
        std::array<char, length> digits{};
        auto [ptr, ec] = std::to_chars(digits.data(), digits.data() + length, f, Format);
        assert(ec == std::errc());
        return bs::string{std::string_view{digits.data(), ptr}};
    }
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_FLOATING_POINT_HPP
