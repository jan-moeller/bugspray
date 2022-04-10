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

#include "bugspray/test_evaluation/test_run_data.hpp"
#include "bugspray/utility/source_location.hpp"

#include <string_view>

/*
 * This class serves two purposes:
 *   1. Decide whether a test case should be entered in a specific test run.
 *   2. Enter the section it represents into the topology.
 * It can also be manually deactivated, for example if a section should be skipped because of runtime/compiletime
 * requirements.
 */

namespace bs
{
struct section_tracker
{
    constexpr explicit section_tracker(test_run_data&   data,
                                       std::string_view name,
                                       source_location  sloc,
                                       bool             active) noexcept
        : m_data(data)
    {
        if (active)
        {
            m_data.topology().chart(data.current(), name);
            if (m_data.can_enter_section(name))
            {
                m_data.enter_section(name, sloc);
                m_active = true;
            }
        }
    }

    constexpr ~section_tracker() noexcept
    {
        if (m_active)
            m_data.leave_section();
    }

    constexpr operator bool() const noexcept { return m_active; }

  private:
    test_run_data& m_data;
    bool           m_active = false;
};
} // namespace bs

#endif // BUGSPRAY_SECTION_TRACKER_HPP
