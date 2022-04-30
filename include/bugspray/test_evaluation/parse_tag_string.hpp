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

#ifndef BUGSPRAY_PARSE_TAG_STRING_HPP
#define BUGSPRAY_PARSE_TAG_STRING_HPP

#include "bugspray/utility/structural_string.hpp"
#include "bugspray/utility/vector.hpp"

#include <array>
#include <string_view>

/*
 * Parses a compiletime tag string and returns a container of tags.
 * By default, the container is std::array. If UseVector is set, then the returned container is bs::vector.
 */

namespace bs
{
template<structural_string TagString, bool UseVector = false>
constexpr auto parse_tag_string()
{
    constexpr auto parse_tags_internal = [](std::string_view tags)
    {
        bs::vector<std::string_view> result;

        char const* begin = nullptr;
        for (char const& c : tags)
        {
            if ((begin == nullptr && c != '['))
                throw "Invalid tag string: Tags must be enclosed in square brackets. Expected '['.";

            switch (c)
            {
            case '[':
                if (begin != nullptr)
                    throw "Invalid tag string: '[' is not allowed inside a tag.";
                begin = &c + 1;
                break;
            case ']':
                if (&c - begin < 1)
                    throw "Invalid tag string: Tags cannot be empty.";
                result.emplace_back(begin, &c);
                begin = nullptr;
                break;
            default:;
            }
        }
        if (begin != nullptr)
            throw "Invalid tag string: Missing closing square bracket. Expected ']'.";

        return result;
    };

    if constexpr (UseVector)
        return parse_tags_internal(TagString.value);
    else
    {
        constexpr auto n = parse_tags_internal(TagString.value).size();

        std::array<std::string_view, n> result;
        std::ranges::copy(parse_tags_internal(TagString.value), result.begin());
        return result;
    }
}
} // namespace bs

#endif // BUGSPRAY_PARSE_TAG_STRING_HPP
