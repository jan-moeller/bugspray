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
#include "bugspray/to_string/to_string_floating_point.hpp"
#include "bugspray/to_string/to_string_integral.hpp"
#include "bugspray/utility/macros/macro_warning_suppression.hpp"

#include <filesystem>

namespace bs
{
xml_reporter::xml_reporter(std::ostream& stream, std::string_view appname, std::size_t seed, bool report_timings)
    : m_writer(stream)
    , m_report_timings(report_timings)
{
    m_writer.open_element("Catch2TestRun");
    m_writer.write_attribute("name", std::filesystem::path{appname}.stem().string());
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

    if (!tags.empty())
    {
        std::string tag_string;
        for (auto&& t : tags)
            tag_string += "[" + std::string{t} + "]";
        m_writer.write_attribute("tags", tag_string);
    }

    m_writer.write_attribute("filename", sloc.file_name);
    m_writer.write_attribute("line", std::string_view{to_string(sloc.line)});
    m_writer.close_attribute_section();

    m_stopwatch.start_test_case_timer();
}

void xml_reporter::leave_test_case() noexcept
{
    auto const duration = m_stopwatch.stop_test_case_timer();

    if (m_failed)
        ++m_results_test_cases.failures;
    else
        ++m_results_test_cases.successes;

    for (auto&& s : m_section_root.sections)
        write_section(s);
    results r;
    write_assertions(r, m_section_root.assertions);

    m_writer.open_element("OverallResult");
    m_writer.write_attribute("success", m_failed ? "false" : "true");
    if (m_report_timings)
        m_writer.write_attribute("durationInSeconds", std::string_view{to_string(duration.count() / 1000.)});
    m_writer.close_attribute_and_element();

    m_writer.close_element();

    m_section_root = section_data{};
}

void xml_reporter::start_run() noexcept
{
    m_stopwatch.start_section_timer();
}

void xml_reporter::stop_run() noexcept
{
    current_data().runtime_in_seconds = m_stopwatch.stop_section_timer().count() / 1000.;
    m_current_target.reset();
}

void xml_reporter::log_target(section_path const& target) noexcept
{
    m_current_target = target;
}

void xml_reporter::enter_section(std::string_view name, source_location sloc) noexcept
{
    m_current_path.push_back(bs::string{name});

    auto& d = current_data();
    d.name  = bs::string{name};
    d.sloc  = sloc;
}

void xml_reporter::leave_section() noexcept
{
    m_current_path.pop_back();
}

void xml_reporter::log_assertion(std::string_view            assertion,
                                 source_location             sloc,
                                 std::string_view            expansion,
                                 std::span<bs::string const> messages,
                                 bool                        result) noexcept
{
    if (result)
        ++m_total_results.successes;
    else
    {
        ++m_total_results.failures;
        m_failed = true;
    }

    current_data().assertions.emplace_back(assertion_data{
        .text      = assertion,
        .sloc      = sloc,
        .expansion = bs::string{expansion},
        .messages  = bs::vector<bs::string>{messages.begin(), messages.end()},
        .result    = result,
    });
}

void xml_reporter::finalize() noexcept
{
    m_writer.open_element("OverallResults");
    m_writer.write_attribute("successes", std::string_view{to_string(m_total_results.successes)});
    m_writer.write_attribute("failures", std::string_view{to_string(m_total_results.failures)});
    m_writer.write_attribute("expectedFailures", std::string_view{to_string(m_total_results.expected_failures)});
    m_writer.close_attribute_and_element();

    m_writer.open_element("OverallResultsCases");
    m_writer.write_attribute("successes", std::string_view{to_string(m_results_test_cases.successes)});
    m_writer.write_attribute("failures", std::string_view{to_string(m_results_test_cases.failures)});
    m_writer.write_attribute("expectedFailures", std::string_view{to_string(m_results_test_cases.expected_failures)});
    m_writer.close_attribute_and_element();

    m_writer.close_element();
}

auto xml_reporter::current_data() -> section_data&
{
    section_data* p = &m_section_root;
    for (auto&& s : m_current_path)
    {
        auto* iter = std::ranges::find_if(p->sections, [s](section_data const& sd) { return sd.name == s; });
        if (iter == p->sections.end())
        {
            BUGSPRAY_DISABLE_WARNING_PUSH
            BUGSPRAY_DISABLE_WARNING_MISSING_FIELD_INITIALIZERS
            // Missing field initializers is okay in this instance. The other fields are supposed to be
            // default-initialized (vectors), or will be overwritten later (timing, sloc).
            iter = &p->sections.emplace_back(section_data{.name = s});
            BUGSPRAY_DISABLE_WARNING_POP
        }
        p = iter;
    }
    return *p;
}

void xml_reporter::write_section(section_data const& sd)
{
    m_writer.open_element("Section");
    m_writer.write_attribute("name", sd.name);
    m_writer.write_attribute("filename", sd.sloc.file_name);
    m_writer.write_attribute("line", std::string_view{to_string(sd.sloc.line)});
    m_writer.close_attribute_section();

    for (auto&& sub : sd.sections)
        write_section(sub);

    results r;
    write_assertions(r, sd.assertions);

    m_writer.open_element("OverallResults");
    m_writer.write_attribute("successes", std::string_view{to_string(r.successes)});
    m_writer.write_attribute("failures", std::string_view{to_string(r.failures)});
    m_writer.write_attribute("expectedFailures", std::string_view{to_string(r.expected_failures)});

    if (m_report_timings)
        m_writer.write_attribute("durationInSeconds", std::string_view{to_string(sd.runtime_in_seconds)});

    m_writer.close_attribute_and_element();
    m_writer.close_element();
}

void xml_reporter::write_assertions(results& r, bs::vector<assertion_data> const& ad)
{
    for (auto&& a : ad)
    {
        if (a.result)
        {
            ++r.successes;
            continue; // TODO: Implement mode where everything is logged, not only failed assertions
        }
        ++r.failures;

        for (auto&& msg : a.messages)
        {
            m_writer.open_element("Info");
            m_writer.close_attribute_section();
            m_writer.write_content(std::string_view{msg});
            m_writer.close_element();
        }
        bool const use_failure_tag = !a.result && a.expansion.empty();
        if (use_failure_tag)
        {
            m_writer.open_element("Failure");
            m_writer.write_attribute("filename", a.sloc.file_name);
            m_writer.write_attribute("line", std::string_view{to_string(a.sloc.line)});
            m_writer.close_attribute_and_element();
        }
        else
        {
            m_writer.open_element("Expression");
            m_writer.write_attribute("success", std::string_view{to_string(a.result)});
            m_writer.write_attribute("type", "REQUIRE"); // TODO: improve
            m_writer.write_attribute("filename", a.sloc.file_name);
            m_writer.write_attribute("line", std::string_view{to_string(a.sloc.line)});
            m_writer.close_attribute_section();
            m_writer.open_element("Original");
            m_writer.close_attribute_section();
            m_writer.write_content(a.text);
            m_writer.close_element();
            m_writer.open_element("Expanded");
            m_writer.close_attribute_section();
            m_writer.write_content(a.expansion);
            m_writer.close_element();
            m_writer.close_element();
        }
    }
}
} // namespace bs