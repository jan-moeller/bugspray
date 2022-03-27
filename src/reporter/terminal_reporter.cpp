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

#include "bugspray/reporter/terminal_reporter.hpp"

#include "bugspray/utility/vector.hpp"

#include <iostream>
#include <ranges>

namespace bs
{
namespace
{
auto combine_test_results(test_results lhs, test_results rhs) -> test_results
{
    return {
        .m_test_cases_passed = lhs.m_test_cases_passed + rhs.m_test_cases_passed,
        .m_test_cases_failed = lhs.m_test_cases_failed + rhs.m_test_cases_failed,
        .m_assertions_passed = lhs.m_assertions_passed + rhs.m_assertions_passed,
        .m_assertions_failed = lhs.m_assertions_failed + rhs.m_assertions_failed,
    };
}

void report_test_case_head(test_run const& data)
{
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << data.name << '\n';
    std::cout << "-------------------------------------------------------------------------------\n";
    std::cout << data.filename << ':' << data.line << '\n';
    std::cout << "...............................................................................\n";
    std::cout << '\n';
}

auto report_section_failures(section_run const& data, bs::vector<section_run const*> const history) -> test_results
{
    test_results results;
    auto const   child_history = [&]
    {
        auto h = history;
        h.push_back(&data);
        return h;
    }();
    for (auto&& subsection : data.subsections)
    {
        auto const sub_results = report_section_failures(subsection, child_history);
        results                = combine_test_results(results, sub_results);
    }

    results.m_assertions_passed += data.passes.size();

    for (auto&& failure : data.failures)
    {
        ++results.m_assertions_failed;

        std::cout << failure.filename << ':' << failure.line << ": FAILED:\n";
        std::cout << "  " << failure.detail << '\n';
        std::cout << '\n';
        for (auto&& parent : std::ranges::reverse_view(child_history))
            std::cout << "IN: " << parent->filename << ':' << parent->line << ": " << parent->name << '\n';
        std::cout << '\n';
    }

    return results;
}
} // namespace

void terminal_reporter::report(test_run const& data)
{
    auto const passed = data.passed();
    if (!passed)
        report_test_case_head(data);
    auto results = report_section_failures(data, {});
    if (passed)
        ++results.m_test_cases_passed;
    else
        ++results.m_test_cases_failed;
    m_results = combine_test_results(results, m_results);
}

auto terminal_reporter::finalize() -> bool
{
    auto const test_cases_passed = m_results.m_test_cases_passed;
    auto const test_cases_failed = m_results.m_test_cases_failed;
    auto const assertions_passed = m_results.m_assertions_passed;
    auto const assertions_failed = m_results.m_assertions_failed;
    auto const total_test_cases  = test_cases_passed + test_cases_failed;
    auto const total_assertions  = assertions_passed + assertions_failed;
    std::cout << "===============================================================================\n";
    std::cout << "test cases: " << total_test_cases << "\t| " << test_cases_passed << " passed\t| " << test_cases_failed
              << " failed\n";
    std::cout << "assertions: " << total_assertions << "\t| " << assertions_passed << " passed\t| " << assertions_failed
              << " failed\n";

    return assertions_failed == 0;
}
} // namespace bs