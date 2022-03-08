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

#ifndef BUGSPRAY_CLI_PARSER_HPP
#define BUGSPRAY_CLI_PARSER_HPP

#include "bugspray/cli_parser/argument_parser.hpp"
#include "bugspray/cli_parser/named_argument.hpp"
#include "bugspray/cli_parser/positional_argument.hpp"
#include "bugspray/utility/naive_vector.hpp"
#include "bugspray/utility/string_literal.hpp"

#include <algorithm>
#include <array>
#include <optional>
#include <ostream>
#include <string_view>
#include <variant>

namespace bs
{
template<typename Positional, typename... Arguments>
class cli_parser
{
  public:
    constexpr cli_parser(int argc, char const** argv)
        : m_executable_name(argv[0])
    {
        int i = 1;
        while (i < argc)
        {
            std::string_view const arg      = argv[i];
            auto const*            arg_data = s_arguments.end();
            if (arg.starts_with("--"))
            {
                arg_data = std::ranges::find_if(s_arguments,
                                                [&](auto const& v) {
                                                    return std::visit([&](auto const& a) -> bool
                                                                      { return a.name == arg.substr(2); },
                                                                      v);
                                                });
            }
            else if (arg.starts_with('-'))
            {
                arg_data = std::ranges::find_if(s_arguments,
                                                [&](auto const& v)
                                                {
                                                    return std::visit(
                                                        [&](auto const& a) -> bool
                                                        {
                                                            auto const shortname = arg.substr(1);
                                                            return shortname.size() == 1
                                                                   && a.abbreviated_name == shortname[0];
                                                        },
                                                        v);
                                                });
            }
            if (arg_data == s_arguments.end())
            {
                if (m_positional_val.has_value())
                    throw std::runtime_error{"Unexpected argument: " + std::string{arg}};
                argument_parser<Positional> arg_parser;
                m_positional_val = arg_parser.parse({arg});
                ++i;
                continue;
            }

            std::visit(
                [&](auto const& a)
                {
                    using Arg = std::remove_cvref_t<decltype(a)>;

                    if (static_cast<unsigned>(argc) < i + 1 + Arg::num_args)
                        throw std::runtime_error{"Missing argument for: " + std::string{arg}};

                    std::array<std::string_view, Arg::num_args> args;
                    std::copy_n(argv + i + 1, Arg::num_args, args.begin());
                    i += 1 + Arg::num_args;

                    argument_parser<Arg> arg_parser;
                    m_values.push_back(value<typename Arg::type>{a.name, arg_parser.parse(args)});
                },
                *arg_data);
        }
    }

    template<string_literal Name>
    constexpr auto get() const
    {
        constexpr auto find_arg = []()
        {
            return std::ranges::find_if(s_arguments,
                                        [&](auto const& v)
                                        { return std::visit([&](auto const& a) { return a.name == Name.value; }, v); });
        };
        static_assert(find_arg() != s_arguments.end(), "The name must be known to the parser object");

        auto const* arg_data = find_arg();
        return std::visit(
            [&](auto const& a)
            {
                using Arg = std::remove_cvref_t<decltype(a)>;

                std::optional<typename Arg::type> result = std::nullopt;
                auto                              v      = std::ranges::find_if(m_values,
                                              [](auto const& e) {
                                                                                    return std::visit([](auto const& e2)
                                                                    { return e2.name == Name.value; },
                                                                    e);
                                              });
                if (v != m_values.end())
                    result = std::visit([](auto const& v2) { return v2.value; }, *v);
                return result;
            },
            *arg_data);
    }

    constexpr auto get_positional() const noexcept -> std::optional<typename Positional::type>
    {
        return m_positional_val;
    }

    constexpr auto print_help(std::ostream& os) const -> std::ostream&
    {
        constexpr int indentation = 20;
        os << "Usage: " << m_executable_name << " [OPTION]... [" << Positional::description << "]\n\n";
        for (auto&& arg : s_arguments)
        {
            std::visit(
                [&](auto const& a)
                {
                    os << '-' << a.abbreviated_name << ", --" << a.name << ' ';
                    for (int i = 0; i < indentation - static_cast<int>(a.name.size()); ++i)
                        os << ' ';
                    os << a.description << '\n';
                },
                arg);
        }
        return os;
    }

  private:
    template<typename V>
    struct value
    {
        std::string_view name;
        V                value;
    };
    using args_type  = std::variant<Arguments...>;
    using value_type = std::variant<value<typename Arguments::type>...>;
    static constexpr std::array              s_arguments{args_type{Arguments{}}...};
    naive_vector<value_type>                 m_values{};
    std::optional<typename Positional::type> m_positional_val;
    std::string_view                         m_executable_name;
};
} // namespace bs

#endif // BUGSPRAY_CLI_PARSER_HPP
