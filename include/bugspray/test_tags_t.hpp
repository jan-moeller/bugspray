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

#ifndef BUGSPRAY_TEST_TAGS_T_HPP
#define BUGSPRAY_TEST_TAGS_T_HPP

#include "bugspray/utility/string_literal.hpp"
#include "bugspray/utility/vector.hpp"

#include <string_view>

namespace bs
{
template<std::size_t N>
struct test_tags_t
{
    std::array<std::string_view, N> m_tags;
};

template<string_literal Tags>
constexpr auto parse_tags()
{
    constexpr auto parse_tags_internal = [](std::string_view tags)
    {
        bs::vector<std::string_view> result;

        char const* begin = nullptr;
        for (char const& c : tags)
        {
            if ((begin == nullptr && c != '[') || (begin != nullptr && c == '['))
                throw std::runtime_error{"Invalid tag string"};

            switch (c)
            {
            case '[':
                begin = &c + 1;
                break;
            case ']':
                if (&c - begin <= 1)
                    throw std::runtime_error{"Invalid tag string"};
                result.emplace_back(begin, &c);
                begin = nullptr;
                break;
            default:;
            }
        }
        if (begin != nullptr)
            throw std::runtime_error{"Invalid tag string"};

        return result;
    };
    constexpr auto n = parse_tags_internal(Tags.value).size();
    test_tags_t<n> result;
    std::ranges::copy(parse_tags_internal(Tags.value), result.m_tags.begin());
    return result;
}
} // namespace bs

#endif // BUGSPRAY_TEST_TAGS_T_HPP
