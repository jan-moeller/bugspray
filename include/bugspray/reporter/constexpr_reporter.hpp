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

#ifndef BUGSPRAY_CONSTEXPR_REPORTER_HPP
#define BUGSPRAY_CONSTEXPR_REPORTER_HPP

#include "bugspray/reporter/reporter.hpp"
#include "bugspray/to_string/to_string_integral.hpp"
#include "bugspray/utility/structural_string.hpp"

#include <algorithm>
#include <array>
#include <ranges>

/*
 * Reporter to be used for constexpr evaluation. It stores the first n messages of the first failed assertion in a
 * constexpr-friendly way. It assumes that compilation will abort after the first assertion failed, and therefore only
 * one assertion is ever logged as a failure.
 */

namespace bs
{
struct constexpr_reporter : reporter
{
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    // TODO: check if gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93413 is fixed
    constexpr ~constexpr_reporter(){};
#endif

    constexpr void enter_test_case(std::string_view name,
                                   std::span<std::string_view const> /*tags*/,
                                   source_location sloc) noexcept override
    {
        m_sections.emplace_back(bs::string{name}, sloc);
    }
    constexpr void leave_test_case() noexcept override { m_sections.pop(); }

    constexpr void start_run() noexcept override {}
    constexpr void stop_run() noexcept override { m_target.reset(); }

    constexpr void log_target(section_path const& target) noexcept override { m_target = target; }

    constexpr void enter_section(std::string_view name, source_location sloc) noexcept override
    {
        m_sections.emplace_back(bs::string{name}, sloc);
    }
    constexpr void leave_section() noexcept override { m_sections.pop(); }

    constexpr void log_assertion(std::string_view            assertion,
                                 source_location             sloc,
                                 std::string_view            expansion,
                                 std::span<bs::string const> messages,
                                 bool                        result) noexcept override
    {
        if (!result)
            compose_messages(assertion, sloc, expansion, messages);
    }

    constexpr void finalize() noexcept override {}

    [[nodiscard]] constexpr auto messages() const noexcept { return m_messages; }

  private:
    static constexpr std::size_t            s_max_message_length = 2048;
    structural_string<s_max_message_length> m_messages;
    std::optional<section_path>             m_target;

    struct section_info
    {
        bs::string      name;
        source_location sloc;
    };
    bs::vector<section_info> m_sections;

    constexpr void compose_messages(std::string_view            assertion,
                                    source_location             sloc,
                                    std::string_view            expansion,
                                    std::span<bs::string const> messages)
    {
        if (m_messages.size() > 0)
            m_messages.append_as_fit(" ### ");
        m_messages.append_as_fit("FAILURE: ");
        m_messages.append_as_fit(bs::string{sloc.file_name} + ':' + to_string(sloc.line) + ": "
                                 + bs::string{assertion});
        if (!expansion.empty())
            m_messages.append_as_fit(bs::string{"; WITH EXPANSION: "} + bs::string{expansion});
        for (auto&& m : messages)
            m_messages.append_as_fit(bs::string{"; WITH: "} + m);
        if (m_target)
        {
            m_messages.append_as_fit(bs::string{"; AFTER RUNNING: "} + m_sections.front().name);
            for (auto&& s : *m_target)
                m_messages.append_as_fit(bs::string{"->"} + s);
        }
        else
        {
            m_messages.append_as_fit(bs::string{"; BEFORE REACHING TARGET"});
        }
        for (auto&& s : std::ranges::reverse_view(m_sections))
            m_messages.append_as_fit(bs::string{"; IN: "} + bs::string{s.sloc.file_name} + ':' + to_string(s.sloc.line)
                                     + ": " + s.name);
    }
};
} // namespace bs

#endif // BUGSPRAY_CONSTEXPR_REPORTER_HPP
