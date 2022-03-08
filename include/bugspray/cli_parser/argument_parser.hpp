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

#ifndef BUGSPRAY_ARGUMENT_PARSER_HPP
#define BUGSPRAY_ARGUMENT_PARSER_HPP

#include "bugspray/cli_parser/named_argument.hpp"
#include "bugspray/cli_parser/positional_argument.hpp"
#include "bugspray/utility/string_literal.hpp"

#include <array>
#include <string_view>

namespace bs
{
template<typename Arg>
struct argument_parser
{
    constexpr auto parse(std::array<std::string_view, Arg::num_args> args) const -> typename Arg::type = delete;
};

template<string_literal Name, char Abbreviated, string_literal Description>
struct argument_parser<named_argument<bool, Name, Abbreviated, Description, 0>>
{
    [[nodiscard]] constexpr auto parse(std::array<std::string_view, 0> /*args*/) const -> bool { return true; }
};
} // namespace bs

#endif // BUGSPRAY_ARGUMENT_PARSER_HPP
