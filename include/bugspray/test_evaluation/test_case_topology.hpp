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
#include <span>

#include <cassert>

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
        path.push_back(bs::string{next});
        chart(path);
    }
    constexpr void chart(section_path const& path) { chart(m_root, std::span<bs::string const>(path)); }

    [[nodiscard]] constexpr auto all_done() const noexcept -> bool { return m_root.done; }
    [[nodiscard]] constexpr auto is_done(section_path const& path) const -> bool
    {
        return is_done(m_root, std::span<bs::string const>(path));
    }
    constexpr void mark_done(section_path const& path) { mark_done(m_root, std::span<bs::string const>(path)); }

    [[nodiscard]] constexpr auto node_count() const noexcept -> std::size_t { return node_count(m_root); }
    [[nodiscard]] constexpr auto leaf_count() const noexcept -> std::size_t { return leaf_count(m_root); }

    constexpr auto operator==(test_case_topology const&) const noexcept -> bool = default;

  private:
    struct node
    {
        bs::string       name;
        bool             done = false;
        bs::vector<node> children{};

        constexpr auto operator==(node const& other) const noexcept -> bool
        {
            auto const same          = name == other.name && done == other.done;
            auto const children_same = [&]
            {
                if (children.size() != other.children.size())
                    return false;
                for (std::size_t i = 0; i < children.size(); ++i)
                    if (children[i] != other.children[i])
                        return false;
                return true;
            }();
            return same && children_same;
        }
    };

    constexpr void chart(node& n, std::span<bs::string const> sections)
    {
        if (sections.empty())
            return;
        auto iter = std::ranges::find_if(n.children, [&](node const& c) { return c.name == sections.front(); });
        if (iter == n.children.end())
            iter = n.children.insert(n.children.end(), node{sections.front()});
        chart(*iter, sections.subspan(1));
    }

    [[nodiscard]] constexpr auto is_done(node const& n, std::span<bs::string const> sections) const -> bool
    {
        if (sections.empty())
            return n.done;
        auto iter = std::ranges::find_if(n.children, [&](node const& c) { return c.name == sections.front(); });
        assert(iter != n.children.end());
        return is_done(*iter, sections.subspan(1));
    }

    constexpr void mark_done(node& n, std::span<bs::string const> sections)
    {
        if (sections.empty())
        {
            n.done = true;
            return;
        }
        auto iter = std::ranges::find_if(n.children, [&](node const& c) { return c.name == sections.front(); });
        assert(iter != n.children.end());
        mark_done(*iter, sections.subspan(1));
        if (std::ranges::all_of(n.children, [](auto const& c) { return c.done; }))
            n.done = true;
    }

    [[nodiscard]] constexpr auto node_count(node const& n) const -> std::size_t
    {
        std::size_t count = 1;
        for (auto&& c : n.children)
            count += node_count(c);
        return count;
    }

    [[nodiscard]] constexpr auto leaf_count(node const& n) const -> std::size_t
    {
        std::size_t count = n.children.empty() ? 1 : 0;
        for (auto&& c : n.children)
            count += leaf_count(c);
        return count;
    }

    node m_root;
};
} // namespace bs

#endif // BUGSPRAY_TEST_CASE_TOPOLOGY_HPP
