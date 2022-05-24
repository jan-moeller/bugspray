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
    constexpr explicit test_run_data(reporter& the_reporter, test_case_topology& topo, std::size_t target_idx)
        : m_reporter(the_reporter)
        , m_topology(topo)
        , m_target_idx(target_idx)
    {
    }

    [[nodiscard]] constexpr auto topology() noexcept -> test_case_topology& { return m_topology; }
    [[nodiscard]] constexpr auto target() const noexcept -> section_path const& { return m_topology[m_target_idx]; }
    [[nodiscard]] constexpr auto current() const noexcept -> section_path
    {
        return section_path{target().begin(), target().begin() + m_cur_level};
    }

    [[nodiscard]] constexpr auto can_enter_section(std::string_view name) const noexcept -> bool
    {
        return target().size() > m_cur_level && target()[m_cur_level] == name;
    }

    constexpr void enter_section(std::string_view name, source_location sloc) noexcept
    {
        assert(target()[m_cur_level] == name);
        ++m_cur_level;
        m_reporter.enter_section(name, sloc);
    }

    constexpr void leave_section() noexcept
    {
        assert(m_cur_level > 0);
        --m_cur_level;
        m_reporter.leave_section();
    }

    constexpr void               mark_failed() noexcept { m_success = false; }
    [[nodiscard]] constexpr auto success() const noexcept -> bool { return m_success; }

    constexpr void
    log_assertion(std::string_view assertion, source_location sloc, std::string_view expansion, bool result) noexcept
    {
        m_reporter.log_assertion(assertion, sloc, expansion, m_messages, result);
    }

    constexpr void push_message(bs::string const& message) { m_messages.push_back(message); }

    constexpr void pop_message()
    {
        assert(!m_messages.empty());
        m_messages.pop();
    }

    constexpr ~test_run_data() = default;

    constexpr test_run_data(test_run_data const&)                    = delete;
    constexpr test_run_data(test_run_data&&)                         = delete;
    constexpr auto operator=(test_run_data const&) -> test_run_data& = delete;
    constexpr auto operator=(test_run_data&&) -> test_run_data&      = delete;

  private:
    reporter&              m_reporter;
    test_case_topology&    m_topology;
    std::size_t const      m_target_idx;
    std::size_t            m_cur_level = 0;
    bool                   m_success   = true;
    bs::vector<bs::string> m_messages;
};
} // namespace bs

#endif // BUGSPRAY_TEST_RUN_DATA_HPP
