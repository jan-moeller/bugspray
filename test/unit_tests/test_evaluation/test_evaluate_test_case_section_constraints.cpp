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
#define BUGSPRAY_NO_SHORT_MACROS
#include "bugspray/macro_interface/assertion_macros.hpp"
#include "bugspray/macro_interface/capture_macro.hpp"
#include "bugspray/macro_interface/section_macro.hpp"
#include "bugspray/reporter/caching_reporter.hpp"
#include "bugspray/test_evaluation/evaluate_test_case.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

static constexpr void a_test_case_fn(test_run_data& bugspray_data)
{
    BUGSPRAY_SECTION("both", both)
    {
    }
    BUGSPRAY_SECTION("runtime", runtime)
    {
    }
    BUGSPRAY_SECTION("compiletime", compiletime)
    {
    }
    BUGSPRAY_SECTION("runtime_if(true)", runtime_if(true))
    {
    }
    BUGSPRAY_SECTION("compiletime_if(true)", compiletime_if(true))
    {
    }
    BUGSPRAY_SECTION("only_if(true)", only_if(true))
    {
    }
    BUGSPRAY_SECTION("only_runtime_if(true)", only_runtime_if(true))
    {
    }
    BUGSPRAY_SECTION("only_compiletime_if(true)", only_compiletime_if(true))
    {
    }
    BUGSPRAY_SECTION("disabled", disabled)
    {
    }
}

TEST_CASE("evaluate_test_case (section constraints)", "[test_evaluation]")
{
    constexpr std::string_view test_name = "foo";
    constexpr source_location  test_sloc{"some_file.cpp", 42};

    constexpr auto test = [=](auto fn)
    {
        constexpr test_case tc{
            .name            = test_name,
            .tags            = {},
            .source_location = test_sloc,
            .test_fn         = &a_test_case_fn,
        };
        caching_reporter the_reporter;
        bool const       success = evaluate_test_case(tc, the_reporter);
        return std::invoke(fn, success, the_reporter.cache());
    };
    constexpr auto return_result = [](bool result, bs::vector<caching_reporter::test_case_data> const&)
    {
        return result;
    };
    constexpr auto return_cache = [](bool, bs::vector<caching_reporter::test_case_data> const& cache)
    {
        return cache;
    };

#define MAKE_TESTS(PREFIX)                                                                                             \
    PREFIX##CHECK(test(return_result) == true);                                                                        \
    PREFIX##CHECK(test(return_cache).size() == 1);                                                                     \
    PREFIX##CHECK(                                                                                                     \
        std::ranges::any_of(test(return_cache)[0].test_runs, [](auto const& trd) { return trd.target.empty(); })       \
        == true);                                                                                                      \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd)                                                              \
                                      { return trd.target.size() == 1 && trd.target.front() == "both"; })              \
                  == true);                                                                                            \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd)                                                              \
                                      { return trd.target.size() == 1 && trd.target.front() == "runtime"; })           \
                  == !std::is_constant_evaluated());                                                                   \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd)                                                              \
                                      { return trd.target.size() == 1 && trd.target.front() == "compiletime"; })       \
                  == std::is_constant_evaluated());                                                                    \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd)                                                              \
                                      { return trd.target.size() == 1 && trd.target.front() == "runtime_if(true)"; })  \
                  == !std::is_constant_evaluated());                                                                   \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd) {                                                            \
                                          return trd.target.size() == 1                                                \
                                                 && trd.target.front() == "compiletime_if(true)";                      \
                                      })                                                                               \
                  == std::is_constant_evaluated());                                                                    \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd)                                                              \
                                      { return trd.target.size() == 1 && trd.target.front() == "only_if(true)"; })     \
                  == true);                                                                                            \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd) {                                                            \
                                          return trd.target.size() == 1                                                \
                                                 && trd.target.front() == "only_runtime_if(true)";                     \
                                      })                                                                               \
                  == !std::is_constant_evaluated());                                                                   \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd) {                                                            \
                                          return trd.target.size() == 1                                                \
                                                 && trd.target.front() == "only_compiletime_if(true)";                 \
                                      })                                                                               \
                  == std::is_constant_evaluated());                                                                    \
    PREFIX##CHECK(std::ranges::any_of(test(return_cache)[0].test_runs,                                                 \
                                      [](auto const& trd)                                                              \
                                      { return trd.target.size() == 1 && trd.target.front() == "disabled"; })          \
                  == false);

    MAKE_TESTS()
    MAKE_TESTS(STATIC_)
}
