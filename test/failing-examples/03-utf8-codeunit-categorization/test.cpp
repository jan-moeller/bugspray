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
#include "bugspray/bugspray.hpp"

#include <bit>
#include <bitset>
#include <iostream>
#include <ostream>
#include <type_traits>

enum class utf8_codeunit_category
{
    leading_one_byte    = 1u << 0u,
    leading_two_bytes   = 1u << 1u,
    leading_three_bytes = 1u << 2u,
    leading_four_bytes  = 1u << 3u,
    trailing_multibyte  = 1u << 4u,
    invalid             = 1u << 5u,
    leading_multibyte   = leading_two_bytes | leading_three_bytes | leading_four_bytes,
};

auto operator<<(std::ostream& os, utf8_codeunit_category c) -> std::ostream&
{
    switch (c)
    {
        using enum utf8_codeunit_category;
#define MAKE_CASE(name)                                                                                                \
    case name:                                                                                                         \
        os << #name;                                                                                                   \
        return os
        MAKE_CASE(leading_one_byte);
        MAKE_CASE(leading_two_bytes);
        MAKE_CASE(leading_three_bytes);
        MAKE_CASE(leading_four_bytes);
        MAKE_CASE(trailing_multibyte);
        MAKE_CASE(invalid);
    default:
        os << std::bitset<8>(static_cast<std::underlying_type_t<utf8_codeunit_category>>(c));
#undef MAKE_CASE
    }
    return os;
}

constexpr auto categorize_utf8_codeunit(uint8_t c) noexcept -> utf8_codeunit_category
{
    using enum utf8_codeunit_category;

    auto const leading_bits = std::countl_one(c);
    switch (leading_bits)
    {
    case 0:
        return leading_one_byte;
    case 1:
        return trailing_multibyte;
    case 2:
        return leading_two_bytes;
    case 3:
        return leading_three_bytes;
    case 4:
        return leading_four_bytes;
    }

    if (std::is_constant_evaluated())
        return invalid;
    else
    {
        std::cout << "encountered weird code unit: " << std::hex << (int)c << '\n'; // Just do some logging...
        return leading_multibyte;                                                   // Ooops! Meant invalid here.
    }
}

TEST_CASE("categorize_utf8_codeunit")
{
    using enum utf8_codeunit_category;
    CHECK(categorize_utf8_codeunit(0x24u) == leading_one_byte);
    CHECK(categorize_utf8_codeunit(0xA3u) == trailing_multibyte);
    CHECK(categorize_utf8_codeunit(0xC2u) == leading_two_bytes);
    CHECK(categorize_utf8_codeunit(0xE0u) == leading_three_bytes);
    CHECK(categorize_utf8_codeunit(0xF0u) == leading_four_bytes);
    CHECK(categorize_utf8_codeunit(0xF8u) == invalid);
}
EVAL_TEST_CASE("categorize_utf8_codeunit");
