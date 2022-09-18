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

#ifndef BUGSPRAY_TEST_RUN_DATA_HPP
#define BUGSPRAY_TEST_RUN_DATA_HPP

#include "bugspray/reporter/reporter.hpp"
#include "bugspray/test_evaluation/section_path.hpp"
#include "bugspray/test_evaluation/test_case_topology.hpp"
#include "bugspray/utility/source_location.hpp"
#include "bugspray/utility/string.hpp"

#include <optional>
#include <span>
#include <string_view>

#include <cstdint>

/*
 * test_run_data bundles all state required to run a test case. In particular:
 *   - the target section. This informs the test case which sections to enter and which to skip.
 *   - the test case topology. This is an out parameter, and used to inform the test runner about future targets.
 *   - the current section. Used by the test case to chart the topology.
 *   - ways to enter sections, log assertions, and mark the test run as failed.
 * Instances of this class are neither copyable nor movable, since they should only be passed by mutable reference
 * inside the test case.
 */

namespace bs
{
struct test_run_data
{
    constexpr explicit test_run_data(reporter& the_reporter, test_case_topology& topo)
        : m_reporter(the_reporter)
        , m_topology(topo)
    {
    }

    [[nodiscard]] constexpr auto topology() noexcept -> test_case_topology& { return m_topology; }
    [[nodiscard]] constexpr auto target() const noexcept -> std::optional<section_path> const& { return m_target; }
    [[nodiscard]] constexpr auto current() const noexcept -> section_path const& { return m_cur_path; }

    [[nodiscard]] constexpr auto can_enter_section(std::string_view name) const noexcept -> bool
    {
        section_path const proj_path = [&]
        {
            auto p = m_cur_path;
            p.push_back(bs::string{name});
            return p;
        }();
        if (m_target)
        {
            // Allow re-entering sections already entered this run
            // This is to support loops in the test case
            if (proj_path.size() > m_target->size())
                return false;
            return std::ranges::equal(m_target->begin(),
                                      m_target->begin() + proj_path.size(),
                                      proj_path.begin(),
                                      proj_path.end());
        }
        return !m_topology.is_done(proj_path);
    }

    constexpr void enter_section(std::string_view name, source_location sloc) noexcept
    {
        m_cur_path.push_back(bs::string{name});
        m_reporter.enter_section(name, sloc);
    }

    constexpr void leave_section() noexcept
    {
        assert(!m_cur_path.empty());
        if (!m_target)
        {
            m_target = m_cur_path;
            m_reporter.log_target(m_target.value());
        }
        m_cur_path.pop_back();
        m_reporter.leave_section();
    }

    constexpr void               mark_failed() noexcept { m_success = false; }
    constexpr void               mark_aborted() noexcept { m_abort = true; }
    [[nodiscard]] constexpr auto success() const noexcept -> bool { return m_success; }
    [[nodiscard]] constexpr auto aborted() const noexcept -> bool { return m_abort; }

    constexpr void
    log_assertion(std::string_view assertion, source_location sloc, std::string_view expansion, bool result) noexcept
    {
        m_reporter.log_assertion(assertion, sloc, expansion, m_messages, result);
    }

    constexpr void push_message(bs::string const& message) { m_messages.push_back(message); }

    constexpr void pop_message()
    {
        assert(!m_messages.empty());
        m_messages.pop_back();
    }

    constexpr ~test_run_data() = default;

    constexpr test_run_data(test_run_data const&)                    = delete;
    constexpr test_run_data(test_run_data&&)                         = delete;
    constexpr auto operator=(test_run_data const&) -> test_run_data& = delete;
    constexpr auto operator=(test_run_data&&) -> test_run_data&      = delete;

  private:
    reporter&                   m_reporter;
    test_case_topology&         m_topology;
    section_path                m_cur_path;
    std::optional<section_path> m_target;
    bool                        m_success = true;
    bool                        m_abort   = false;
    bs::vector<bs::string>      m_messages;
};
} // namespace bs

#endif // BUGSPRAY_TEST_RUN_DATA_HPP
