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

#ifndef BUGSPRAY_STRUCTURAL_STRING_HPP
#define BUGSPRAY_STRUCTURAL_STRING_HPP

#include <algorithm>
#include <string_view>

#include <cstdint>

/*
 * structural_string is a very simplistic string class that can be used as NTTP. It can be implicitly constructed from
 * literals.
 */

// TODO: Elevate this to a proper, generally useful facility with less gotchas

namespace bs
{
template<std::size_t N>
struct structural_string
{
    constexpr structural_string() { std::fill_n(value, N, '\0'); }
    constexpr structural_string(char const (&str)[N]) { std::copy_n(str, N, value); }
    constexpr structural_string(std::string_view sv)
    {
        auto const n = std::min(N, sv.size());
        std::copy_n(sv.data(), n, value);
        std::fill(&value[n], &value[N], '\0');
    }
    char value[N];

    constexpr auto operator==(structural_string const& rhs) const -> bool
    {
        auto const other = static_cast<std::string_view>(rhs);
        return *this == other;
    }
    constexpr auto operator==(char const* rhs) const -> bool { return *this == std::string_view{rhs}; }
    constexpr auto operator==(std::string_view rhs) const -> bool
    {
        auto const self = static_cast<std::string_view>(*this);
        return self == rhs;
    }

    constexpr void append_as_fit(std::string_view sv) noexcept
    {
        auto const length_available = capacity() - size();
        auto const length_to_copy   = std::min(sv.size(), length_available);
        std::ranges::copy_n(sv.begin(), length_to_copy, value + size());
    }

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t
    {
        return static_cast<std::string_view>(*this).size();
    }
    [[nodiscard]] constexpr static auto capacity() noexcept -> std::size_t { return N; }

    constexpr explicit operator std::string_view() const noexcept
    {
        std::string_view self = {value, N};
        while (self.ends_with('\0'))
            self.remove_suffix(1);
        return self;
    }
};

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(structural_string<N1> lhs, structural_string<N2> rhs) -> structural_string<N1 + N2>
{
    structural_string<N1 + N2> result;
    std::copy_n(lhs.value, lhs.size(), result.value + 0);
    std::copy_n(rhs.value, rhs.size(), result.value + lhs.size());
    return result;
}

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(structural_string<N1> lhs, char const (&rhs)[N2]) -> structural_string<N1 + N2>
{
    return lhs + structural_string<N2>{rhs};
}

template<std::size_t N1, std::size_t N2>
constexpr auto operator+(char const (&lhs)[N1], structural_string<N2> rhs) -> structural_string<N1 + N2>
{
    return structural_string<N1>{lhs} + rhs;
}

template<structural_string S, char C = '\0'>
constexpr auto trim_left() noexcept
{
    if constexpr (S.capacity() == 0)
        return S;
    else
    {
        constexpr auto          iter = std::ranges::find_if(S.value, [](char c) { return c != C; });
        constexpr auto          size = std::end(S.value) - iter;
        structural_string<size> result{};
        std::ranges::copy_n(iter, size - 1, result.value);
        return result;
    }
}

template<structural_string S, char C = '\0'>
constexpr auto trim_right() noexcept
{
    if constexpr (S.capacity() == 0)
        return S;
    else
    {
        constexpr auto          iter = std::ranges::find_if(std::rbegin(S.value) + 1,
                                                   std::rend(S.value),
                                                   [](char c) { return c != C; });
        constexpr auto          size = -(iter - std::rend(S.value)) + 1; // +1 for '\0'
        structural_string<size> result{};
        std::ranges::copy_n(S.value, size - 1, result.value);
        return result;
    }
}

template<structural_string S, char C = '\0'>
constexpr auto trim() noexcept
{
    constexpr auto trim_right_result = trim_right<S, C>();
    constexpr auto trim_left_result  = trim_left<trim_right_result, C>();
    return trim_left_result;
}
} // namespace bs

#endif // BUGSPRAY_STRUCTURAL_STRING_HPP
