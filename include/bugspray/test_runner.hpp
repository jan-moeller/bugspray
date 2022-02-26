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

#ifndef BUGSPRAY_TEST_RUNNER_HPP
#define BUGSPRAY_TEST_RUNNER_HPP

#include "test_case_registry.hpp"
#include "test_run.hpp"

namespace bs
{
constexpr auto run_test(test_case_info const& t) noexcept -> test_run
{
    ::bs::test_run data{{t.name, t.filename, t.line}};
    try
    {
        do
        {
            data.executed_leaf_section = false;

            t.fn(&data);

            if (std::ranges::all_of(data.subsections, [](section_run const& s) { return s.done; }))
                data.done = true;
        } while (!data.done);
    }
    catch (std::exception const& e)
    {
        data.failures.emplace_back(t.filename, t.line, naive_string{"Exception escaped test case: "} + e.what());
    }
    catch (...)
    {
        data.failures.emplace_back(t.filename, t.line, naive_string{"Unknown exception escaped test case."});
    }
    return data;
}
} // namespace bs

#endif // BUGSPRAY_TEST_RUNNER_HPP
