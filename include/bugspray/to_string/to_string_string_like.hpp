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

#ifndef BUGSPRAY_TO_STRING_STRING_LIKE_HPP
#define BUGSPRAY_TO_STRING_STRING_LIKE_HPP

#include "bugspray/to_string/char_to_printable_string.hpp"
#include "bugspray/utility/character.hpp"
#include "bugspray/utility/string.hpp"

#include <ranges>

namespace bs
{
template<std::ranges::forward_range T>
constexpr auto to_string(T&& s) -> bs::string
    requires character<std::ranges::range_value_t<T>>
{
    bs::string result = "\"";
    for (auto&& c : s)
        result += char_to_printable_string(c);
    result += "\"";
    return result;
}

template<character CharT>
constexpr auto to_string(CharT const* s) -> bs::string
{
    return to_string(std::basic_string_view<CharT>{s});
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_STRING_LIKE_HPP
