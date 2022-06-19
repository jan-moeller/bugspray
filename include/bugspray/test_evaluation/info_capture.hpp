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

#ifndef BUGSPRAY_INFO_CAPTURE_HPP
#define BUGSPRAY_INFO_CAPTURE_HPP

#include "bugspray/test_evaluation/test_run_data.hpp"
#include "bugspray/to_string/stringify.hpp"
#include "bugspray/utility/string.hpp"
#include "bugspray/utility/structural_string.hpp"
#include "bugspray/utility/trim.hpp"

#include <algorithm>
#include <array>
#include <ranges>

/*
 * info_capture is used in a test case to capture informative strings in a scoped way.
 */

namespace bs
{
struct info_capture
{
    constexpr explicit info_capture(test_run_data& bugspray_data, bs::string const& message)
        : m_bugspray_data(bugspray_data)
        , m_count(1)
    {
        m_bugspray_data.push_message(message);
    }

    template<std::ranges::forward_range Container>
        requires(std::same_as<std::ranges::range_value_t<Container>, bs::string>)
    constexpr explicit info_capture(test_run_data& bugspray_data, Container&& messages)
        : m_bugspray_data(bugspray_data)
        , m_count(messages.size())
    {
        for (auto&& m : messages)
            m_bugspray_data.push_message(m);
    }

    constexpr ~info_capture() noexcept
    {
        for (std::size_t i = 0; i < m_count; ++i)
            m_bugspray_data.pop_message();
    }

    info_capture(info_capture const&) = delete;
    info_capture(info_capture&&)      = delete;

    auto operator=(info_capture const&) -> info_capture& = delete;
    auto operator=(info_capture&&) -> info_capture&      = delete;

    test_run_data& m_bugspray_data;
    std::size_t    m_count;
};

template<structural_string S>
constexpr auto split_expression_string() // splits an expression by comma into std::array. To be used with #__VA_ARGS__.
{
    if constexpr (trim<S, ' '>().size() == 0)
        return std::array<std::string_view, 0>{};
    else
    {
        constexpr auto create = []
        {
            std::size_t                  level = 0;
            bs::vector<std::string_view> expressions;
            auto                         begin = std::begin(S.value);
            auto                         cur   = begin;
            for (char c : S.value)
            {
                if (c == '(')
                {
                    ++level;
                    ++cur;
                }
                else if (c == ')')
                {
                    --level;
                    ++cur;
                }
                else if (level == 0 && c == ',')
                {
                    std::string_view v{begin, cur};
                    expressions.emplace_back(trim(v));
                    begin = ++cur;
                }
                else
                    ++cur;
            }
            std::string_view v{begin, cur - 1}; // -1 since we do not want the closing \0
            expressions.emplace_back(trim(v));
            if (level != 0)
                throw "Wrap expressions containing a comma in parentheses";
            return expressions;
        };
        constexpr std::size_t size = create().size();
        auto const            strs = create();

        std::array<std::string_view, size> expressions;
        std::ranges::copy(strs, expressions.begin());
        return expressions;
    }
}

template<std::size_t I, typename... Ts>
constexpr auto info_capture_stringification_helper(test_run_data&                         bugspray_data,
                                                   std::array<std::string_view, I> const& names,
                                                   Ts&&... things)
{
    static_assert(I == sizeof...(Ts));
    std::array<bs::string, I> values{stringify(std::forward<Ts>(things))...};
    for (std::size_t i = 0; i < I; ++i)
        values[i] = bs::string{names[i]} + ": " + values[i];
    return info_capture{bugspray_data, values};
}
} // namespace bs

#endif // BUGSPRAY_INFO_CAPTURE_HPP
