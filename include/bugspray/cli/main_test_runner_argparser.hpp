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

#ifndef BUGSPRAY_MAIN_TEST_RUNNER_ARGPARSER_HPP
#define BUGSPRAY_MAIN_TEST_RUNNER_ARGPARSER_HPP

#include "argument_destination.hpp"
#include "argument_parser.hpp"
#include "main_test_runner_config.hpp"
#include "parameter.hpp"
#include "parameter_names.hpp"

namespace bs
{
namespace detail
{
constexpr auto reporter_parser = [](std::string_view arg, config::reporter_enum& out)
{
    if (arg == "console")
    {
        out = config::reporter_enum::console;
        return true;
    }
    if (arg == "xml")
    {
        out = config::reporter_enum::xml;
        return true;
    }
    return false;
};
constexpr auto order_parser = [](std::string_view arg, config::order_enum& out)
{
    if (arg == "decl")
    {
        out = config::order_enum::declaration;
        return true;
    }
    if (arg == "lex")
    {
        out = config::order_enum::lexicographic;
        return true;
    }
    if (arg == "rand")
    {
        out = config::order_enum::random;
        return true;
    }
    return false;
};

constexpr parameter<decltype(parameter_names{"-h", "--help"}),
                    decltype(argument_destination{&config::help}),
                    parsers::arg_parser,
                    structural_string{"show this help message and exit"}.size() + 1>
    help_param{
        .names       = parameter_names{"-h", "--help"},
        .destination = argument_destination{&config::help},
        .help        = structural_string{"show this help message and exit"},
    };
constexpr parameter<decltype(parameter_names{"--version"}),
                    decltype(argument_destination{&config::version}),
                    parsers::arg_parser,
                    structural_string{"show the bugspray version and exit"}.size() + 1>
    version_param{
        .names       = parameter_names{"--version"},
        .destination = argument_destination{&config::version},
        .help        = structural_string{"show the bugspray version and exit"},
    };
constexpr parameter<decltype(parameter_names{"-r", "--reporter"}),
                    decltype(argument_destination{&config::reporter}),
                    decltype(reporter_parser),
                    structural_string{"select reporter from [console, xml]"}.size() + 1>
    reporter_param{
        .names       = parameter_names{"-r", "--reporter"},
        .destination = argument_destination{&config::reporter},
        .parser      = reporter_parser,
        .help        = structural_string{"select reporter from [console, xml]"},
    };
constexpr parameter<decltype(parameter_names{"-o", "--out"}),
                    decltype(argument_destination{&config::output}),
                    parsers::arg_parser,
                    structural_string{"send all output to a file"}.size() + 1>
    output_param{
        .names       = parameter_names{"-o", "--out"},
        .destination = argument_destination{&config::output},
        .help        = structural_string{"send all output to a file"},
    };
constexpr parameter<decltype(parameter_names{"-d", "--durations"}),
                    decltype(argument_destination{&config::report_durations}),
                    parsers::arg_parser,
                    structural_string{"specify whether durations are reported"}.size() + 1>
    durations_param{
        .names       = parameter_names{"-d", "--durations"},
        .destination = argument_destination{&config::report_durations},
        .help        = structural_string{"specify whether durations are reported"},
    };
constexpr parameter<decltype(parameter_names{"--order"}),
                    decltype(argument_destination{&config::order}),
                    decltype(order_parser),
                    structural_string{"specify order of test case execution from [decl, lex, rand]"}.size() + 1>
    order_param{
        .names       = parameter_names{"--order"},
        .destination = argument_destination{&config::order},
        .parser      = order_parser,
        .help        = structural_string{"specify order of test case execution from [decl, lex, rand]"},
    };
constexpr parameter<decltype(parameter_names{"--rng-seed"}),
                    decltype(argument_destination{&config::seed}),
                    parsers::arg_parser,
                    structural_string{"specify the seed for the random number generator used by bugspray"}.size() + 1>
    order_rng_seed{
        .names       = parameter_names{"--rng-seed"},
        .destination = argument_destination{&config::seed},
        .help        = structural_string{"specify the seed for the random number generator used by bugspray"},
    };
constexpr parameter<decltype(parameter_names{"test-spec"}),
                    decltype(argument_destination{&config::test_spec}),
                    parsers::arg_parser,
                    structural_string{"specify which tests to run"}.size() + 1>
    test_spec_param{
        .names       = parameter_names{"test-spec"},
        .destination = argument_destination{&config::test_spec},
        .help        = structural_string{"specify which tests to run"},
    };
} // namespace detail

using argparser = argument_parser<detail::help_param,
                                  detail::version_param,
                                  detail::reporter_param,
                                  detail::output_param,
                                  detail::durations_param,
                                  detail::order_param,
                                  detail::order_rng_seed,
                                  detail::test_spec_param>;
} // namespace bs

#endif // BUGSPRAY_MAIN_TEST_RUNNER_ARGPARSER_HPP
