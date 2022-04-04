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

#ifndef BUGSPRAY_NAIVE_STRING_HPP
#define BUGSPRAY_NAIVE_STRING_HPP

#include "naive_vector.hpp"

#include <ostream>
#include <string_view>

/*
 * Implements a subset of the functionality of std::string. This is meant as a stopgap solution while many stdlibs don't
 * yet fully support constexpr string.
 */

namespace bs
{
class naive_string
{
  public:
    using value_type      = naive_vector<char>::value_type;
    using reference       = naive_vector<char>::reference;
    using const_reference = naive_vector<char>::const_reference;
    using pointer         = naive_vector<char>::pointer;
    using const_pointer   = naive_vector<char>::const_pointer;
    using iterator        = naive_vector<char>::iterator;
    using const_iterator  = naive_vector<char>::const_iterator;
    using difference_type = naive_vector<char>::difference_type;

    constexpr naive_string() noexcept = default;
    constexpr naive_string(char const* str)
        : naive_string(std::string_view{str})
    {
    }
    constexpr naive_string(std::string_view sv)
    {
        --m_data.m_end;
        for (auto c : sv)
            m_data.push_back(c);
        m_data.push_back('\0');
    }

    constexpr auto operator=(char const* str) -> naive_string&
    {
        *this = naive_string(str);
        return *this;
    }

    [[nodiscard]] constexpr auto begin() const noexcept -> char const* { return m_data.begin(); }
    [[nodiscard]] constexpr auto begin() noexcept -> char* { return m_data.begin(); }

    [[nodiscard]] constexpr auto end() const noexcept -> char const* { return m_data.end() - 1; }
    [[nodiscard]] constexpr auto end() noexcept -> char* { return m_data.end() - 1; }

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t { return m_data.size() - 1; }
    [[nodiscard]] constexpr auto capacity() const noexcept -> std::size_t { return m_data.capacity(); }
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == 0; }

    constexpr void push_back(char value)
    {
        --m_data.m_end;
        m_data.push_back(value);
        m_data.push_back('\0');
    }

    constexpr friend auto operator+(naive_string const& lhs, char const* rhs) -> naive_string
    {
        return lhs + naive_string{rhs};
    }

    constexpr friend auto operator+(naive_string const& lhs, naive_string const& rhs) -> naive_string
    {
        auto result = lhs;
        for (auto c : rhs)
            result.push_back(c);
        return result;
    }

    constexpr auto operator[](std::size_t idx) const noexcept -> const_reference { return m_data[idx]; }
    constexpr auto operator[](std::size_t idx) noexcept -> reference { return m_data[idx]; }

    constexpr auto operator==(naive_string const&) const noexcept -> bool = default;

  private:
    naive_vector<char> m_data{'\0'};
};

inline auto operator<<(std::ostream& os, naive_string const& s) -> std::ostream&
{
    for (auto c : s)
        os << c;
    return os;
}
} // namespace bs

#endif // BUGSPRAY_NAIVE_STRING_HPP
