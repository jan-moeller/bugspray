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
#include "bugspray/cli/main_test_runner_argparser.hpp"
#include "bugspray/reporter/formatted_ostream_reporter.hpp"
#include "bugspray/reporter/xml_reporter.hpp"
#include "bugspray/test_evaluation/evaluate_test_case.hpp"
#include "bugspray/test_registration/test_case_registry.hpp"
#include "bugspray/version.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

auto main(int argc, char const** argv) -> int
{
    using namespace bs;

    argparser parser;

    config c;
    try
    {
        parser.parse<config>(argc, argv, c);
    }
    catch (std::runtime_error const& e)
    {
        std::cerr << "Failed to parse arguments: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    if (c.help)
    {
        std::cout << parser.make_help_message(argv[0]) << '\n';
        return EXIT_SUCCESS;
    }
    if (c.version)
    {
        std::cout << "Bugspray " << BUGSPRAY_VERSION << '\n';
        return EXIT_SUCCESS;
    }

    std::unique_ptr<std::ofstream> output_filestream;
    if (!c.output.empty())
        output_filestream = std::make_unique<std::ofstream>(std::filesystem::path{c.output});

    std::ostream& os = output_filestream ? *output_filestream : std::cout;

    std::default_random_engine rand_engine{c.seed};
    if (c.order == config::order_enum::lexicographic)
        std::ranges::sort(g_test_case_registry,
                          [](test_case const& lhs, test_case const& rhs) { return lhs.name < rhs.name; });
    else if (c.order == config::order_enum::random)
        std::ranges::shuffle(g_test_case_registry, rand_engine);

    bool success = true;

    auto reporter = [&]() -> std::unique_ptr<struct reporter>
    {
        switch (c.reporter)
        {
            using enum config::reporter_enum;
        case console:
            return std::make_unique<formatted_ostream_reporter>(os);
        case xml:
            return std::make_unique<xml_reporter>(os, argv[0], c.seed, c.report_durations);
        }
        return nullptr;
    }
    ();

    for (auto&& tc : g_test_case_registry)
        success &= evaluate_test_case(tc, *reporter, c.test_spec);
    reporter->finalize();
    os << std::endl;

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}