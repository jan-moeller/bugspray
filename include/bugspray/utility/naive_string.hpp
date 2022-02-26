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

namespace bs
{
class naive_string
{
  public:
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

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t { return m_data.size(); }
    [[nodiscard]] constexpr auto capacity() const noexcept -> std::size_t { return m_data.capacity(); }
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == 1; }

    constexpr void push_back(char value)
    {
        --m_data.m_end;
        m_data.push_back(value);
        m_data.push_back('\0');
    }

    constexpr friend auto operator+(naive_string const& lhs, char const* rhs) -> naive_string
    {
        auto result = lhs;
        for (std::size_t i = 0; rhs[i] != '\0'; ++i)
            result.push_back(rhs[i]);
        return result;
    }

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
