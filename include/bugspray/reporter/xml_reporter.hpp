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

#ifndef BUGSPRAY_NOOP_REPORTER_HPP
#define BUGSPRAY_NOOP_REPORTER_HPP

#include "bugspray/reporter/detail/runtime_stopwatch.hpp"
#include "bugspray/reporter/reporter.hpp"
#include "bugspray/utility/xml_writer.hpp"

#include <chrono>
#include <optional>
#include <ostream>

/*
 * Reports results in a xml file format
 */

namespace bs
{
struct xml_reporter : reporter
{
    explicit xml_reporter(std::ostream& stream, std::string_view appname, std::size_t seed, bool report_timings);

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
    struct assertion_data
    {
        std::string_view       text;
        source_location        sloc;
        bs::string             expansion;
        bs::vector<bs::string> messages;
        bool                   result;
    };
    struct section_data;
    struct assertion_and_section_holder
    {
        bs::vector<assertion_data> assertions;
        bs::vector<section_data>   sections;
    };
    struct section_data : assertion_and_section_holder
    {
        bs::string      name;
        source_location sloc;
        double          runtime_in_seconds;
    };

    struct results
    {
        std::size_t successes         = 0;
        std::size_t failures          = 0;
        std::size_t expected_failures = 0;
    };

    section_data                m_section_root;
    section_path                m_current_path;
    std::optional<section_path> m_current_target;

    auto current_data() -> section_data&;
    void write_section(section_data const& sd);
    void write_assertions(results& r, bs::vector<assertion_data> const& ad);

    xml_writer m_writer;

    bool                      m_report_timings;
    detail::runtime_stopwatch m_stopwatch;

    results m_results_test_cases;
    results m_total_results;
    bool    m_failed = false;
};
} // namespace bs

#endif // BUGSPRAY_NOOP_REPORTER_HPP
