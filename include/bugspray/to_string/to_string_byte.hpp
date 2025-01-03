//
// MIT License
//
// Copyright (c) 2025 Jan Möller
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
#ifndef BUGSPRAY_TO_STRING_BYTE_HPP
#define BUGSPRAY_TO_STRING_BYTE_HPP

#include "bugspray/to_string/to_string_integral.hpp"
#include "bugspray/utility/string.hpp"

#include <bit>
#include <type_traits>

namespace bs
{
constexpr auto to_string(std::byte b) -> bs::string
{
    return bs::string("0x") + to_string<16, 2>(std::bit_cast<unsigned char>(b));
}

template<std::byte B>
constexpr auto to_string([[maybe_unused]] std::integral_constant<std::byte, B> b) -> bs::string
{
    return to_string(B);
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_BYTE_HPP
