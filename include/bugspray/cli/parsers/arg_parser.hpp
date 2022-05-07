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

#ifndef BUGSPRAY_ARG_PARSER_HPP
#define BUGSPRAY_ARG_PARSER_HPP

#include "parse_bool.hpp"
#include "parse_integral.hpp"

#include <string_view>

namespace bs::parsers
{
struct arg_parser
{
    constexpr auto operator()(std::string_view input, bool& out) const -> bool { return parse_bool(input, out); }
    constexpr auto operator()(std::string_view input, std::integral auto& out) const -> bool
    {
        return parse_integral(input, out);
    }
};
} // namespace bs::parsers

#endif // BUGSPRAY_ARG_PARSER_HPP
