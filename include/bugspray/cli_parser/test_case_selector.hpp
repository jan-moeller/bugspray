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

#ifndef BUGSPRAY_TEST_CASE_SELECTOR_HPP
#define BUGSPRAY_TEST_CASE_SELECTOR_HPP

#include "bugspray/utility/string.hpp"
#include "bugspray/utility/vector.hpp"

#include <algorithm>
#include <span>
#include <string_view>

namespace bs
{
struct test_case_selector
{
    constexpr auto operator()(std::string_view name, std::span<std::string_view const> tags) const noexcept -> bool
    {
        if (m_selector.empty())
            return true;
        for (auto const& s : m_selector)
        {
            bool success = true;
            for (auto const& e : s)
            {
                if ((e.is_tag && std::ranges::find(tags, e.str) == tags.end()) || (!e.is_tag && name != e.str))
                {
                    success = false;
                    break;
                }
            }
            if (success)
                return true;
        }
        return false;
    }
    struct element
    {
        bs::string str;
        bool       is_tag = false;
    };
    bs::vector<bs::vector<element>> m_selector; // 1st level: OR, 2nd level: AND
};

constexpr auto parse_selector_string(std::string_view str) -> test_case_selector
{
    test_case_selector selector{};
    selector.m_selector.emplace_back();

    bool        is_tag = false;
    char const* begin  = nullptr;
    for (char const& c : str)
    {
        if (begin == nullptr)
        {
            if (c == ',')
            {
                selector.m_selector.emplace_back();
                continue;
            }
            is_tag = (c == '[');
            if (is_tag)
                begin = &c + 1;
            else
                begin = &c;
        }
        else
        {
            if (is_tag)
            {
                if (c == ']')
                {
                    selector.m_selector.back().emplace_back(std::string_view{begin, &c}, true);
                    begin = nullptr;
                }
                else if (c == '[')
                    throw std::runtime_error{"invalid selector string"};
            }
            else if (!is_tag && c == ',')
            {
                selector.m_selector.back().emplace_back(std::string_view{begin, &c}, false);
                selector.m_selector.emplace_back();
                begin = nullptr;
            }
        }
    }
    if (begin != nullptr)
    {
        if (is_tag)
            throw std::runtime_error{"invalid selector string"};
        selector.m_selector.back().emplace_back(std::string_view{begin, str.end()}, false);
    }

    return selector;
}
} // namespace bs

#endif // BUGSPRAY_TEST_CASE_SELECTOR_HPP
