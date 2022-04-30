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
#include "bugspray/utility/structural_string.hpp"

#include <algorithm>
#include <array>

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

    constexpr void enter_test_case(std::string_view /*name*/,
                                   std::span<std::string_view const> /*tags*/,
                                   source_location /*sloc*/) noexcept override
    {
    }
    constexpr void leave_test_case() noexcept override
    {
    }

    constexpr void start_run(section_path const& /*target*/) noexcept override
    {
    }
    constexpr void stop_run() noexcept override
    {
    }

    constexpr void enter_section(std::string_view /*name*/, source_location /*sloc*/) noexcept override
    {
    }
    constexpr void leave_section() noexcept override
    {
    }

    constexpr void log_assertion(std::string_view /*assertion*/,
                                 source_location /*sloc*/,
                                 std::span<bs::string const> messages,
                                 bool                        result) noexcept override
    {
        if (!result)
        {
            for (std::size_t i = 0; i < std::min(s_max_message_length, messages.size()); ++i)
                std::ranges::copy_n(messages[i].begin(),
                                    std::min(messages[i].size(), s_max_message_length),
                                    m_messages[i].value);
        }
    }

    constexpr void finalize() noexcept override
    {
    }

    [[nodiscard]] constexpr auto messages() const noexcept
    {
        return m_messages;
    }

  private:
    static constexpr std::size_t s_max_messages       = 3;
    static constexpr std::size_t s_max_message_length = 1024;

    std::array<structural_string<s_max_message_length>, s_max_messages> m_messages;
};
} // namespace bs

#endif // BUGSPRAY_CONSTEXPR_REPORTER_HPP
