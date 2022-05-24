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

#ifndef BUGSPRAY_CACHING_REPORTER_HPP
#define BUGSPRAY_CACHING_REPORTER_HPP

#include "bugspray/reporter/reporter.hpp"
#include "bugspray/utility/source_location.hpp"
#include "bugspray/utility/string.hpp"
#include "bugspray/utility/vector.hpp"

#include <algorithm>
#include <span>
#include <string_view>

/*
 * This reporter simply caches all data for later use.
 */

namespace bs
{
struct caching_reporter : reporter
{
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
        std::string_view name;
        source_location  sloc;
    };
    struct test_run_data : assertion_and_section_holder
    {
        section_path target;
    };
    struct test_case_data
    {
        std::string_view                  name;
        std::span<std::string_view const> tags;
        source_location                   sloc;

        bs::vector<test_run_data> test_runs;
    };

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    // TODO: check if gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93413 is fixed
    constexpr ~caching_reporter(){};
#endif

    constexpr void enter_test_case(std::string_view                  name,
                                   std::span<std::string_view const> tags,
                                   source_location                   sloc) noexcept override
    {
        m_test_cases.emplace_back(name, tags, sloc, bs::vector<test_run_data>{});
    }

    constexpr void leave_test_case() noexcept override {}

    constexpr void start_run(section_path const& target) noexcept override
    {
        m_test_cases.back().test_runs.push_back({{}, target});
    }

    constexpr void stop_run() noexcept override {}

    constexpr void enter_section(std::string_view name, source_location sloc) noexcept override
    {
        create_section(name, sloc);
        m_current_section.push_back(name);
    }

    constexpr void leave_section() noexcept override { m_current_section.pop(); }

    constexpr void log_assertion(std::string_view            assertion,
                                 source_location             sloc,
                                 std::string_view            expansion,
                                 std::span<bs::string const> messages,
                                 bool                        result) noexcept override
    {
        auto*                  s = find_section(m_current_section);
        bs::vector<bs::string> msgs{messages.begin(), messages.end()};
        s->assertions.emplace_back(assertion, sloc, bs::string{expansion}, msgs, result);
    }

    constexpr void finalize() noexcept override {}

    [[nodiscard]] constexpr auto cache() const noexcept -> bs::vector<test_case_data> const& { return m_test_cases; }

  private:
    constexpr auto find_section(section_path const& path) noexcept -> assertion_and_section_holder*
    {
        auto&                         cur_test_case = m_test_cases.back();
        assertion_and_section_holder* cur_section   = &cur_test_case.test_runs.back();
        for (std::size_t i = 0; i < path.size(); ++i)
        {
            auto const s    = path[i];
            auto*      iter = std::ranges::find_if(cur_section->sections,
                                              [&s](section_data& subsection) { return subsection.name == s; });
            assert(iter != cur_section->sections.end());
            cur_section = iter;
        }
        return cur_section;
    }

    constexpr void create_section(std::string_view name, source_location const& sloc) noexcept
    {
        assertion_and_section_holder* cur = find_section(m_current_section);
        cur->sections.push_back({{}, name, sloc});
    }

    bs::vector<test_case_data> m_test_cases;
    section_path               m_current_section;
};
} // namespace bs

#endif // BUGSPRAY_CACHING_REPORTER_HPP
