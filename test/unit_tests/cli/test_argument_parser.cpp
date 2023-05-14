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
#include "bugspray/cli/argument_destination.hpp"
#include "bugspray/cli/argument_parser.hpp"
#include "bugspray/cli/parameter.hpp"
#include "bugspray/cli/parameter_names.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("argument_parser", "[cli]")
{
    struct config
    {
        bool help = false;
        struct foo
        {
            int  amount;
            bool sticky;
            struct viscosity
            {
                double value;
            } viscosity;
        } foo;
        struct bar
        {
            int stock;
        } bar;
    };

    argument_parser<parameter<decltype(parameter_names{"-h", "--help"}),
                              decltype(argument_destination{&config::help}),
                              parsers::arg_parser,
                              structural_string{"show this help message and exit"}.size() + 1>{
                        .names       = parameter_names{"-h", "--help"},
                        .destination = argument_destination{&config::help},
                        .help        = structural_string{"show this help message and exit"},
                    },
                    parameter<decltype(parameter_names{"-s", "--sticky"}),
                              decltype(argument_destination{&config::foo, &config::foo::sticky})>{
                        .names       = parameter_names{"-s", "--sticky"},
                        .destination = argument_destination{&config::foo, &config::foo::sticky},
                    },
                    parameter<decltype(parameter_names{"amount"}),
                              decltype(argument_destination{&config::foo, &config::foo::amount})>{
                        .names       = parameter_names{"amount"},
                        .destination = argument_destination{&config::foo, &config::foo::amount},
                    }>
        parser;

    std::array   argv = {"tool_name", "--help", "-s", "true", "393"};
    config const c    = parser.parse<config>(argv.size(), argv.data());
    REQUIRE(c.help == true);
    REQUIRE(c.foo.sticky == true);
    REQUIRE(c.foo.amount == 393);
}