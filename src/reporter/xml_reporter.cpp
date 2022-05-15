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

#include "bugspray/reporter/xml_reporter.hpp"

#include "bugspray/to_string/to_string_bool.hpp"
#include "bugspray/to_string/to_string_integral.hpp"

namespace bs
{
xml_reporter::xml_reporter(std::ostream& stream, std::string_view appname, std::size_t seed, bool report_timings)
    : m_writer(stream)
    , m_report_timings(report_timings)
{
    m_writer.open_element("Catch2TestRun");
    m_writer.write_attribute("name", appname);
    m_writer.write_attribute("rng-seed", std::string_view{to_string(seed)});
    m_writer.close_attribute_section();
}

void xml_reporter::enter_test_case(std::string_view                  name,
                                   std::span<std::string_view const> tags,
                                   source_location                   sloc) noexcept
{
    m_failed = false;

    m_writer.open_element("TestCase");
    m_writer.write_attribute("name", name);
    std::string tag_string;
    for (auto&& t : tags)
        tag_string += "[" + std::string{t} + "]";
    m_writer.write_attribute("tags", tag_string);
    m_writer.write_attribute("filename", sloc.file_name);
    m_writer.write_attribute("line", std::string_view{to_string(sloc.line)});
    m_writer.close_attribute_section();

    m_test_case_start_time = std::chrono::steady_clock::now();
}

void xml_reporter::leave_test_case() noexcept
{
    auto const stop_time = std::chrono::steady_clock::now();

    if (m_failed)
        ++m_results_test_cases.failures;
    else
        ++m_results_test_cases.successes;

    m_writer.open_element("OverallResult");
    m_writer.write_attribute("success", m_failed ? "false" : "true");
    if (m_report_timings)
    {
        using namespace std::chrono;
        auto const diff = stop_time - m_test_case_start_time;
        auto const ms   = duration_cast<seconds>(diff);
        m_writer.write_attribute("durationInSeconds",
                                 std::string_view{to_string(ms.count())}); // TODO: sub-second accuracy
    }
    m_writer.close_attribute_section();
    m_writer.close_element();

    m_writer.close_element();
}

void xml_reporter::start_run(section_path const& target) noexcept
{
}

void xml_reporter::stop_run() noexcept
{
}

void xml_reporter::enter_section(std::string_view name, source_location sloc) noexcept
{
    m_writer.open_element("Section");
    m_writer.write_attribute("name", name);
    m_writer.write_attribute("filename", sloc.file_name);
    m_writer.write_attribute("line", std::string_view{to_string(sloc.line)});
    m_writer.close_attribute_section();

    if (m_results.empty())
        m_results.emplace_back();
    else
        m_results.push_back(m_results.back());

    m_section_start_time.emplace_back(std::chrono::steady_clock::now());
}

void xml_reporter::leave_section() noexcept
{
    auto const stop_time = std::chrono::steady_clock::now();

    m_writer.open_element("OverallResults");
    m_writer.write_attribute("successes", std::string_view{to_string(m_results.back().successes)});
    m_writer.write_attribute("failures", std::string_view{to_string(m_results.back().failures)});
    m_writer.write_attribute("expectedFailures", std::string_view{to_string(m_results.back().expected_failures)});

    if (m_report_timings)
    {
        using namespace std::chrono;
        auto const diff = stop_time - m_section_start_time.back();
        auto const ms   = duration_cast<seconds>(diff);
        m_writer.write_attribute("durationInSeconds",
                                 std::string_view{to_string(ms.count())}); // TODO: sub-second accuracy
    }
    m_section_start_time.pop_back();

    m_writer.close_attribute_section();
    m_writer.close_element();

    m_writer.close_element();

    m_results.pop_back();
}

void xml_reporter::log_assertion(std::string_view            assertion,
                                 source_location             sloc,
                                 std::span<bs::string const> messages,
                                 bool                        result) noexcept
{
    if (result)
    {
        ++m_results.back().successes;
        ++m_total_results.successes;
    }
    else
    {
        ++m_results.back().failures;
        ++m_total_results.failures;
    }

    if (!result)
    {
        m_failed = true;

        for (auto&& msg : messages)
        {
            m_writer.open_element("Info");
            m_writer.close_attribute_section();
            m_writer.write_content(std::string_view{msg});
            m_writer.close_element();
        }
        m_writer.open_element("Expression");
        m_writer.write_attribute("success", std::string_view{to_string(result)});
        m_writer.write_attribute("type", "REQUIRE"); // TODO: improve
        m_writer.write_attribute("filename", sloc.file_name);
        m_writer.write_attribute("line", std::string_view{to_string(sloc.line)});
        m_writer.close_attribute_section();
        m_writer.open_element("Original");
        m_writer.close_attribute_section();
        m_writer.write_content(assertion);
        m_writer.close_element();
        m_writer.open_element("Expanded");
        m_writer.close_attribute_section();
        // m_writer.write_content(); // TODO: Implement expansion
        m_writer.close_element();
        m_writer.close_element();
    }
}

void xml_reporter::finalize() noexcept
{
    m_writer.open_element("OverallResults");
    m_writer.write_attribute("successes", std::string_view{to_string(m_total_results.successes)});
    m_writer.write_attribute("failures", std::string_view{to_string(m_total_results.failures)});
    m_writer.write_attribute("expectedFailures", std::string_view{to_string(m_total_results.expected_failures)});
    m_writer.close_attribute_section();
    m_writer.close_element();

    m_writer.open_element("OverallResultsCases");
    m_writer.write_attribute("successes", std::string_view{to_string(m_results_test_cases.successes)});
    m_writer.write_attribute("failures", std::string_view{to_string(m_results_test_cases.failures)});
    m_writer.write_attribute("expectedFailures", std::string_view{to_string(m_results_test_cases.expected_failures)});
    m_writer.close_attribute_section();
    m_writer.close_element();

    m_writer.close_element();
}
} // namespace bs