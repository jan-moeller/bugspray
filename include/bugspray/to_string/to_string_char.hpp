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

#ifndef BUGSPRAY_TO_STRING_CHAR_HPP
#define BUGSPRAY_TO_STRING_CHAR_HPP

#include "bugspray/to_string/char_to_printable_string.hpp"
#include "bugspray/utility/string.hpp"

namespace bs
{
constexpr auto to_string(char c) -> bs::string
{
    return R"(')" + char_to_printable_string(c) + R"(')";
}
constexpr auto to_string(wchar_t c) -> bs::string
{
    return R"(')" + char_to_printable_string(c) + R"(')";
}
constexpr auto to_string(char8_t c) -> bs::string
{
    return to_string(static_cast<char>(c));
}
constexpr auto to_string(char16_t c) -> bs::string
{
    return R"(')" + char_to_printable_string(c) + R"(')";
}
constexpr auto to_string(char32_t c) -> bs::string
{
    return R"(')" + char_to_printable_string(c) + R"(')";
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_CHAR_HPP
