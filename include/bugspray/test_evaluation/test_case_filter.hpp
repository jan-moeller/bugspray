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

#ifndef BUGSPRAY_TEST_CASE_FILTER_HPP
#define BUGSPRAY_TEST_CASE_FILTER_HPP

#include "bugspray/test_evaluation/test_case.hpp"
#include "bugspray/utility/isspace.hpp"
#include "bugspray/utility/vector.hpp"

#include <algorithm>
#include <stdexcept>
#include <string_view>

namespace bs
{
namespace detail
{
constexpr auto split_spec(std::string_view test_spec) noexcept -> bs::vector<std::string_view>
{
    bs::vector<std::string_view> components;
    bool                         open   = false;
    bool                         quoted = false;
    std::size_t                  begin  = 0;
    for (std::size_t i = 0; i < test_spec.size(); ++i)
    {
        char const c = test_spec[i];
        if (open)
        {
            if ((!quoted && bs::isspace(c)) || (quoted && c == '"'))
            {
                quoted = false;
                open   = false;
                components.push_back(test_spec.substr(begin, i - begin));
            }
        }
        else
        {
            if (bs::isspace(c))
                continue;
            open = true;
            if (c == '"')
            {
                quoted = true;
                begin  = i + 1;
            }
            else
                begin = i;
        }
    }
    if (open)
        components.push_back(test_spec.substr(begin));
    return components;
}

constexpr auto split_tags(std::string_view tags) -> bs::vector<bs::vector<std::string_view>>
{
    bs::vector<bs::vector<std::string_view>> components{{}};
    bool                                     open  = false;
    std::size_t                              begin = 0;
    for (std::size_t i = 0; i < tags.size(); ++i)
    {
        char const c = tags[i];
        if (open)
        {
            if (c == ']')
            {
                components.back().push_back(tags.substr(begin, i - begin));
                open = false;
            }
        }
        else
        {
            if (c == '[')
            {
                begin = i + 1;
                open  = true;
            }
            else if (c == ',')
                components.emplace_back();
        }
    }
    if (open)
        components.back().push_back(tags.substr(begin));

    return components;
}

constexpr auto match_string(std::string_view sv, std::string_view pattern) -> bool
{
    bool const fuzzy_start = pattern.starts_with('*');
    bool const fuzzy_stop  = pattern.ends_with('*');

    if (fuzzy_start)
        pattern.remove_prefix(1);
    if (fuzzy_stop && !pattern.empty())
        pattern.remove_suffix(1);

    if (fuzzy_start && fuzzy_stop)
        return sv.find(pattern) != std::string_view::npos;
    if (fuzzy_start)
        return sv.ends_with(pattern);
    if (fuzzy_stop)
        return sv.starts_with(pattern);
    return sv == pattern;
}
} // namespace detail

constexpr auto test_case_filter(test_case const& tc, std::string_view test_spec) -> bool
{
    auto const components = detail::split_spec(test_spec);
    bool       result     = components.empty();
    bool       unset      = true;
    for (auto c : components)
    {
        bool const is_exclusion = [&]
        {
            if (c.starts_with("exclude:"))
            {
                c = c.substr(8);
                return true;
            }
            if (c.starts_with("~"))
            {
                c = c.substr(1);
                return true;
            }
            return false;
        }();
        bool match = false;
        if (c.starts_with('['))
        {
            auto const alternatives = detail::split_tags(c);
            for (auto&& alt : alternatives)
            {
                match = true;
                for (auto&& m : alt)
                {
                    auto const iter = std::ranges::find_if(tc.tags,
                                                           [&m](std::string_view sv)
                                                           { return detail::match_string(sv, m); });
                    match &= iter != tc.tags.end();
                    if (!match)
                        break;
                }
                if (match)
                    break;
            }
        }
        else
            match = detail::match_string(tc.name, c);
        if (unset)
        {
            result = is_exclusion == !match;
            unset  = false;
        }

        if (is_exclusion && match)
            result = false;
        else if (!is_exclusion && match)
            result = true;
    }
    return result;
}
} // namespace bs

#endif // BUGSPRAY_TEST_CASE_FILTER_HPP
