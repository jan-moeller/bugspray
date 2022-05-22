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
#include "bugspray/to_string/to_string_floating_point.hpp"

#include <catch2/catch_all.hpp>

#include <numbers>

template<std::chars_format Format, typename Out>
auto from_str(bs::string const& str) -> Out
{
    Out r;
    auto [ptr, ec] = std::from_chars(str.begin(), str.end(), r, Format);
    if (ec != std::errc())
        throw std::make_error_code(ec).message();
    if (ptr != str.end())
        throw "unparsed characters left";
    return r;
}

template<std::chars_format Format, typename T>
void test_with_format(T n)
{
    auto const s          = bs::to_string<Format>(n);
    auto const b          = from_str<Format, T>(s);
    auto const format_str = []
    {
        if (Format == std::chars_format::fixed)
            return "fixed";
        if (Format == std::chars_format::hex)
            return "hex";
        if (Format == std::chars_format::scientific)
            return "scientific";
        if (Format == std::chars_format::general)
            return "general";
    }();
    CAPTURE(n, s, b, format_str);
    CHECK(b == n);
}

void test(auto n)
{
    test_with_format<std::chars_format::general>(n);
    test_with_format<std::chars_format::scientific>(n);
    test_with_format<std::chars_format::hex>(n);
    test_with_format<std::chars_format::fixed>(n);
}

TEST_CASE("to_string(floating_point)", "[to_string]")
{
    test(0.0);
    test(1.0);
    test(-1.0);
    test(std::numbers::pi);
    test(std::numbers::e);
    test(std::numbers::log2e);
    test(std::numbers::log10e);
    test(std::numeric_limits<float>::max());
    test(std::numeric_limits<float>::min());
    test(std::numeric_limits<float>::lowest());
    test(std::numeric_limits<double>::max());
    test(std::numeric_limits<double>::min());
    test(std::numeric_limits<double>::lowest());
    test(std::numeric_limits<long double>::max());
    test(std::numeric_limits<long double>::min());
    test(std::numeric_limits<long double>::lowest());
}