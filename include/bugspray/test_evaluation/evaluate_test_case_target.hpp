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

#ifndef BUGSPRAY_EVALUATE_TEST_CASE_TARGET_HPP
#define BUGSPRAY_EVALUATE_TEST_CASE_TARGET_HPP

#include "bugspray/test_evaluation/test_case.hpp"
#include "bugspray/test_evaluation/test_run_data.hpp"

#include <functional>
#include <string_view>

/*
 * Executes a test for a specific section exactly once. It is the callers' responsibility to call this as many times
 * as required to cover all sections.
 * If an exception escapes the test case, this is interpreted as a failure and an appropriate message is logged.
 */

namespace bs
{
static constexpr std::string_view exception_escaped_text = "Exception escaped test case";

constexpr auto evaluate_test_case_target(test_case const& tc, test_run_data& data) noexcept -> bool
{
    try
    {
        std::invoke(tc.test_fn, data);
    }
    catch (std::exception const& e)
    {
        bs::vector<bs::string> messages{bs::string{"WITH: "} + e.what()};
        data.log_assertion(exception_escaped_text, tc.source_location, messages, false);
        data.mark_failed();
    }
    catch (...)
    {
        bs::vector<bs::string> messages{bs::string{"WITH: Unknown exception."}};
        data.log_assertion(exception_escaped_text, tc.source_location, messages, false);
        data.mark_failed();
    }
    return data.success();
}
} // namespace bs

#endif // BUGSPRAY_EVALUATE_TEST_CASE_TARGET_HPP
