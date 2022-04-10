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

#ifndef BUGSPRAY_TEST_CASE_TOPOLOGY_HPP
#define BUGSPRAY_TEST_CASE_TOPOLOGY_HPP

#include "bugspray/test_evaluation/section_path.hpp"
#include "bugspray/utility/vector.hpp"

#include <algorithm>

/*
 * test_case_topology is a helper class to be used when discovering the topology of a test case; i.e. when running
 * a test case, the contained sections are discovered on the fly, and can then be run on subsequent executions.
 *
 * An object of this type always contains at least one element, the empty section_path. This is because every test case
 * contains an unnamed root section.
 */

namespace bs
{
struct test_case_topology
{
    constexpr void chart(section_path path, std::string_view next)
    {
        path.push_back(next);
        chart(path);
    }

    constexpr void chart(section_path const& path)
    {
        auto* const iter = std::ranges::find(m_paths, path);
        if (iter == m_paths.end())
            m_paths.push_back(path);
    }

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t { return m_paths.size(); }

    [[nodiscard]] constexpr auto begin() const noexcept { return m_paths.begin(); }
    [[nodiscard]] constexpr auto end() const noexcept { return m_paths.end(); }

    constexpr auto operator[](std::size_t idx) const noexcept -> section_path const& { return m_paths[idx]; }
    constexpr auto operator==(test_case_topology const&) const noexcept -> bool = default;

  private:
    bs::vector<section_path> m_paths{{}};
};
} // namespace bs

#endif // BUGSPRAY_TEST_CASE_TOPOLOGY_HPP
