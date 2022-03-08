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
#include "bugspray/cli_parser/cli_parser.hpp"

#include <iostream>

#include <cstdlib>

auto main(int argc, char const** argv) -> int
{
    using namespace ::bs;

    cli_parser<positional_argument<test_case_selector, "SELECTOR">,
               named_argument<bool, "help", 'h', "display this help and exit", 0>>
               parser{argc, argv};
    auto const selector = parser.get_positional().value_or(test_case_selector{});

    if (parser.get<"help">().value_or(false))
    {
        parser.print_help(std::cout);
        return EXIT_SUCCESS;
    }

    terminal_reporter reporter;
    for (auto&& t : get_test_case_registry().test_cases)
    {
        if (selector(t.name, t.get_tags()))
        {
            test_run data = run_test(t);
            reporter.report(data);
        }
    }

    bool const success = reporter.finalize();

    if (success)
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}
