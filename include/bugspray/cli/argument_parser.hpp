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

#include "bugspray/utility/structural_string.hpp"
#include "bugspray/utility/structural_tuple.hpp"

#include <stdexcept>
#include <string_view>

namespace bs
{
template<auto... Parameters>
struct argument_parser
{
    using parameters_t = structural_tuple<decltype(Parameters)...>;
    constexpr static parameters_t parameters{Parameters...};

    static_assert((Parameters.names.valid() && ...), "All parameters must either be positional or named");

    template<typename Config>
    constexpr auto parse(int argc, char const* argv[]) -> Config
    {
        Config c;
        parse(argc, argv, c);
        return c;
    }

    constexpr void parse(int argc, char const* argv[], auto& out)
    {
        auto apply_to_matching_param = [pos = std::size_t{0}, &out](std::string_view arg, auto&& fn) mutable
        {
            bool        success           = false;
            bool const  looking_for_named = arg.starts_with('-');
            std::size_t positional_count  = 0;
            static_for<0, std::tuple_size_v<parameters_t>>(
                [&](auto I) -> static_for_continuation
                {
                    constexpr auto const&   p    = get<I>(parameters);
                    static_for_continuation cont = static_for_continuation::continue_;
                    if (looking_for_named && p.names.contains(arg))
                        cont = static_for_continuation::break_;
                    if (!looking_for_named && p.names.is_unnamed)
                    {
                        if (positional_count == pos)
                        {
                            ++pos;
                            cont = static_for_continuation::break_;
                        }
                        ++positional_count;
                    }
                    if (cont == static_for_continuation::break_)
                    {
                        fn(p, argument_destination_ref<p.destination>(out));
                        success = true;
                    }
                    return cont;
                });
            if (!success)
                throw std::runtime_error{"unexpected argument"};
        };

        for (int i = 0; i < argc;)
        {
            std::string_view arg = argv[i];

            apply_to_matching_param(arg,
                                    [&](auto const& p, auto& result)
                                    {
                                        bool const     parse_this    = p.names.is_unnamed;
                                        bool const     parse_next    = p.names.is_named && i + 1 < argc;
                                        constexpr bool bool_shortcut = std::convertible_to<decltype(result), bool>;

                                        if (parse_this)
                                        {
                                            bool const success = p.parser(arg, result);
                                            if (success)
                                            {
                                                i += 1;
                                                return;
                                            }
                                            throw std::runtime_error{std::string{"Parse error: "} + std::string{arg}};
                                        }
                                        if (parse_next)
                                        {
                                            auto const& next_arg = argv[i + 1];
                                            bool const  success  = p.parser(next_arg, result);
                                            if (success)
                                            {
                                                i += 2;
                                                return;
                                            }
                                            if constexpr (!bool_shortcut)
                                                throw std::runtime_error{std::string{"Parse error: "}
                                                                         + std::string{next_arg}};
                                        }
                                        if constexpr (bool_shortcut)
                                        {
                                            i += 1;
                                            result = true;
                                            return;
                                        }
                                    });
        }
    }
};
} // namespace bs

#endif // BUGSPRAY_ARGUMENT_PARSER_HPP
