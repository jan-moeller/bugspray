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

#ifndef BUGSPRAY_EVALUATE_TEST_CASE_HPP
#define BUGSPRAY_EVALUATE_TEST_CASE_HPP

#include "bugspray/reporter/constexpr_reporter.hpp"
#include "bugspray/reporter/noop_reporter.hpp"
#include "bugspray/reporter/reporter.hpp"
#include "bugspray/test_evaluation/evaluate_test_case_target.hpp"
#include "bugspray/test_evaluation/test_case.hpp"
#include "bugspray/test_evaluation/test_run_data.hpp"
#include "bugspray/utility/compiletime_string.hpp"

/*
 * Evaluates a test case by calling the function multiple times until every section has been called once.
 */

namespace bs
{
template<bool AbortEarly = false>
constexpr auto evaluate_test_case(test_case const& tc, reporter& the_reporter) -> bool
{
    bool               success = true;
    test_case_topology topo;

    the_reporter.enter_test_case(tc.name, tc.tags, tc.source_location);
    for (std::size_t i = 0; i < topo.size(); ++i)
    {
        the_reporter.start_run(topo[i]);

        test_run_data data{the_reporter, topo, i};
        success &= evaluate_test_case_target(tc, data);

        the_reporter.stop_run();

        if constexpr (AbortEarly)
        {
            if (!success)
                break;
        }
    }
    the_reporter.leave_test_case();

    return success;
}

constexpr auto evaluate_test_case(test_case const& tc) -> bool
{
    noop_reporter reporter;
    return evaluate_test_case(tc, reporter);
}

template<auto>
constexpr auto evaluate_test_case_constexpr(test_case const& tc)
{
    ::bs::constexpr_reporter reporter;
    auto const               result = evaluate_test_case<true>(tc, reporter);
    return std::make_pair(result, reporter.messages());
}

template<compiletime_string... Messages>
constexpr void compile_time_error()
{
    static_assert((sizeof...(Messages), false), "Test evaluation failed.");
}
} // namespace bs

#endif // BUGSPRAY_EVALUATE_TEST_CASE_HPP
