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

#ifndef BUGSPRAY_SECTION_TRACKER_HPP
#define BUGSPRAY_SECTION_TRACKER_HPP

#include "test_run.hpp"

namespace bs
{
struct section_tracker
{
    bs::test_run*    m_test_case;
    bs::section_run* m_section;
    bool             m_execute_this = false;

    constexpr section_tracker(test_run*        test_case,
                              std::string_view section_name,
                              std::string_view filename,
                              long int         line)
        : m_test_case(test_case)
        , m_section((m_test_case->current_prefix.push_back(section_name), current_section(m_test_case, filename, line)))
        , m_execute_this(!m_section->done && !m_test_case->executed_leaf_section)
    {
    }

    constexpr ~section_tracker() noexcept
    {
        m_test_case->current_prefix.pop();
        if (m_execute_this && !m_test_case->executed_leaf_section)
        {
            m_test_case->executed_leaf_section = true;
            m_section->done                = true;
        }
    }

    [[nodiscard]] constexpr auto should_enter() const noexcept -> bool { return m_execute_this; }
};
} // namespace bs

#endif // BUGSPRAY_SECTION_TRACKER_HPP
