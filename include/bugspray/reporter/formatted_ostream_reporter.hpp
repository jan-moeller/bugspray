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

#ifndef BUGSPRAY_FORMATTED_OSTREAM_REPORTER_HPP
#define BUGSPRAY_FORMATTED_OSTREAM_REPORTER_HPP

#include "bugspray/reporter/reporter.hpp"

#include <optional>
#include <ostream>

/*
 * This reporter prints all results to a formatted stream in a human-readable way.
 */

namespace bs
{
struct formatted_ostream_reporter : reporter
{
    explicit formatted_ostream_reporter(std::ostream& stream);

    void enter_test_case(std::string_view                  name,
                         std::span<std::string_view const> tags,
                         source_location                   sloc) noexcept override;
    void leave_test_case() noexcept override;
    void start_run() noexcept override;
    void stop_run() noexcept override;
    void log_target(section_path const& target) noexcept override;
    void enter_section(std::string_view name, source_location sloc) noexcept override;
    void leave_section() noexcept override;
    void log_assertion(std::string_view            assertion,
                       source_location             sloc,
                       std::string_view            expansion,
                       std::span<bs::string const> messages,
                       bool                        result) noexcept override;
    void finalize() noexcept override;

  private:
    struct test_case_data
    {
        std::string_view                  name;
        std::span<std::string_view const> tags;
        source_location                   sloc;
    };
    struct section_data
    {
        std::string_view name;
        source_location  sloc;
    };
    struct statistics
    {
        std::size_t m_num_test_cases        = 0;
        std::size_t m_num_failed_test_cases = 0;
        std::size_t m_num_assertions        = 0;
        std::size_t m_num_failed_assertions = 0;
    };

    void report_test_case_head(test_case_data const& data);

    test_case_data              m_cur_test_case;
    std::optional<section_path> m_cur_target;
    bs::vector<section_data>    m_cur_section;
    statistics                  m_stats;
    bool                        m_failed_assertion_in_this_test_case = false;
    std::ostream&               m_stream;
};
} // namespace bs

#endif // BUGSPRAY_FORMATTED_OSTREAM_REPORTER_HPP
