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

#ifndef BUGSPRAY_TEST_RUN_HPP
#define BUGSPRAY_TEST_RUN_HPP

#include "bugspray/utility/naive_string.hpp"
#include "bugspray/utility/naive_vector.hpp"

#include <ranges>
#include <string_view>

namespace bs
{
struct failure_info
{
    std::string_view filename;
    long int         line;
    naive_string     detail;

    constexpr auto operator==(failure_info const&) const noexcept -> bool = default;
};

struct pass_info
{
    std::string_view filename;
    long int         line;

    constexpr auto operator==(pass_info const&) const noexcept -> bool = default;
};

struct section_run
{
    std::string_view           name;
    std::string_view           filename;
    long int                   line;
    naive_vector<failure_info> failures{};
    naive_vector<pass_info>    passes{};
    naive_vector<section_run>  subsections{};
    bool                       done = false;

    constexpr auto passed() const noexcept -> bool
    {
        if (!failures.empty())
            return false;

        for (auto&& s : subsections)
            if (!s.passed())
                return false;
        return true;
    }
};

struct test_run : section_run
{
    naive_vector<std::string_view> current_prefix{};
    bool                           executed_leaf_section = false; // TODO: This probably shouldn't be in here
};

// If the test_run object already contains a subsection for the current_prefix, then this returns a pointer to it.
// If the section doesn't exist yet, it creates it first.
constexpr auto current_section(test_run* test_case, std::string_view filename, long int line) noexcept -> section_run*
{
    auto         prefix  = test_case->current_prefix;
    section_run* section = test_case;
    for (auto&& p : prefix)
    {
        auto iter = std::ranges::find_if(section->subsections, [&p](::bs::section_run& s) { return s.name == p; });
        if (iter == section->subsections.end())
        {
            section->subsections.push_back({p, filename, line});
            iter = section->subsections.end() - 1;
        }
        section = iter;
    }
    return section;
}

// Logs failure if it isn't registered yet.
// It can happen that the same assertion is triggered multiple times due to sections.
constexpr void
log_assertion_failure(test_run* test_case, std::string_view filename, long int line, std::string_view description)
{
    auto* section = ::bs::current_section(test_case, filename, line);

    failure_info const f{filename, line, description};
    if (std::ranges::find(section->failures, f) == section->failures.end())
        section->failures.push_back(f);
}
} // namespace bs

#endif // BUGSPRAY_TEST_RUN_HPP
