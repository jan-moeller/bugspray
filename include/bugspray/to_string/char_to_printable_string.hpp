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

#ifndef BUGSPRAY_CHAR_TO_PRINTABLE_STRING_HPP
#define BUGSPRAY_CHAR_TO_PRINTABLE_STRING_HPP

#include "bugspray/to_string/to_string_integral.hpp"
#include "bugspray/utility/string.hpp"

namespace bs
{
constexpr auto char_to_printable_string(char c) -> bs::string
{
    switch (c)
    {
    case '\\':
        return R"(\\)";
    case '\a':
        return R"(\a)";
    case '\b':
        return R"(\b)";
    case '\f':
        return R"(\f)";
    case '\n':
        return R"(\n)";
    case '\r':
        return R"(\r)";
    case '\t':
        return R"(\t)";
    case '\v':
        return R"(\v)";
    case '\0':
        return R"(\0)";
    default:;
    }
    if ((c > '\x00' && c < '\x1f') || c == '\x7f')
    {
        auto const s = to_string<16, 2>(static_cast<unsigned char>(c));
        return R"(\x)" + s;
    }
    return bs::string{c};
}
} // namespace bs

#endif // BUGSPRAY_CHAR_TO_PRINTABLE_STRING_HPP
