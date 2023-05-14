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

#include "bugspray/reporter/formatted_ostream_reporter.hpp"

#include <ranges>

namespace bs
{
formatted_ostream_reporter::formatted_ostream_reporter(std::ostream& stream)
    : m_stream(stream)
{
}

void formatted_ostream_reporter::enter_test_case(std::string_view                  name,
                                                 std::span<std::string_view const> tags,
                                                 source_location                   sloc) noexcept
{
    m_cur_test_case = {
        .name = name,
        .tags = tags,
        .sloc = sloc,
    };
    m_failed_assertion_in_this_test_case = false;
    ++m_stats.m_num_test_cases;
}

void formatted_ostream_reporter::leave_test_case() noexcept
{
}

void formatted_ostream_reporter::start_run() noexcept
{
}

void formatted_ostream_reporter::stop_run() noexcept
{
    m_cur_target.reset();
}

void formatted_ostream_reporter::log_target(section_path const& target) noexcept
{
    m_cur_target = target;
}

void formatted_ostream_reporter::enter_section(std::string_view name, source_location sloc) noexcept
{
    m_cur_section.push_back({name, sloc});
}

void formatted_ostream_reporter::leave_section() noexcept
{
    m_cur_section.pop_back();
}

void formatted_ostream_reporter::log_assertion(std::string_view            assertion,
                                               source_location             sloc,
                                               std::string_view            expansion,
                                               std::span<bs::string const> messages,
                                               bool                        result) noexcept
{
    ++m_stats.m_num_assertions;
    if (result)
        return;

    ++m_stats.m_num_failed_assertions;
    if (!m_failed_assertion_in_this_test_case)
    {
        m_failed_assertion_in_this_test_case = true;
        report_test_case_head(m_cur_test_case);
        ++m_stats.m_num_failed_test_cases;
    }

    if (m_cur_target)
    {
        if (m_cur_target->size() > 0)
        {
            m_stream << "When evaluating section " << (*m_cur_target)[0];
            if (m_cur_target->size() > 1)
            {
                for (std::size_t i = 1; i < m_cur_target->size(); ++i)
                    m_stream << " --> " << (*m_cur_target)[i];
            }
            m_stream << ":\n";
        }
    }
    else
    {
        m_stream << "Before reaching target section\n";
    }

    m_stream << sloc.file_name << ':' << sloc.line << ": FAILED:\n";
    m_stream << "  " << assertion << '\n';

    if (!expansion.empty())
        m_stream << "WITH EXPANSION: " << expansion << '\n' << '\n';

    for (auto&& m : messages)
        m_stream << m << '\n';
    m_stream << '\n';

    for (auto iter = m_cur_section.rbegin(); iter != m_cur_section.rend(); ++iter)
        m_stream << "IN: " << iter->sloc.file_name << ':' << iter->sloc.line << ": " << iter->name << '\n';
    m_stream << '\n';
}

void formatted_ostream_reporter::finalize() noexcept
{
    auto const total_test_cases  = m_stats.m_num_test_cases;
    auto const test_cases_failed = m_stats.m_num_failed_test_cases;
    auto const test_cases_passed = total_test_cases - test_cases_failed;
    auto const total_assertions  = m_stats.m_num_assertions;
    auto const assertions_failed = m_stats.m_num_failed_assertions;
    auto const assertions_passed = total_assertions - assertions_failed;
    m_stream << "===============================================================================\n";
    m_stream << "test cases: " << total_test_cases << "\t| " << test_cases_passed << " passed\t| " << test_cases_failed
             << " failed\n";
    m_stream << "assertions: " << total_assertions << "\t| " << assertions_passed << " passed\t| " << assertions_failed
             << " failed\n";
}

void formatted_ostream_reporter::report_test_case_head(test_case_data const& data)
{
    m_stream << "-------------------------------------------------------------------------------\n";
    m_stream << data.name << '\n';
    m_stream << "-------------------------------------------------------------------------------\n";
    m_stream << data.sloc.file_name << ':' << data.sloc.line << '\n';
    m_stream << "...............................................................................\n";
    m_stream << '\n';
}

} // namespace bs