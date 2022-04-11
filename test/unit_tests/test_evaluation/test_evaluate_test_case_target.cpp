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
#include "bugspray/macro_interface/assertion_macros.hpp"
#include "bugspray/macro_interface/section_macro.hpp"
#include "bugspray/reporter/caching_reporter.hpp"
#include "bugspray/test_evaluation/evaluate_test_case_target.hpp"

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch_all.hpp>

using namespace bs;

CATCH_TEST_CASE("evaluate_test_case_target", "[test_evaluation]")
{
    constexpr std::string_view test_name = "foo";
    constexpr source_location  test_location{"some_file.cpp", 42};
    CATCH_SECTION("topology & assertions")
    {
        constexpr auto test = [=](auto fn)
        {
            test_case tc{
                .name            = test_name,
                .tags            = {},
                .source_location = test_location,
                .test_fn =
                    [](test_run_data& bugspray_data)
                {
                    BUGSPRAY_SUCCEED();
                    BUGSPRAY_SECTION("first")
                    {
                        BUGSPRAY_SUCCEED();
                    }
                    BUGSPRAY_SECTION("second", runtime)
                    {
                    }
                    BUGSPRAY_SECTION("third", compiletime)
                    {
                    }
                },
            };
            caching_reporter   reporter;
            test_case_topology topo;
            test_run_data      data{reporter, topo, 0};

            reporter.enter_test_case(tc.name, tc.tags, tc.source_location);
            reporter.start_run(data.target());

            evaluate_test_case_target(tc, data);

            reporter.stop_run();
            reporter.leave_test_case();

            return std::invoke(fn, reporter, topo);
        };

        constexpr auto get_cache = [](caching_reporter const& reporter, test_case_topology const&)
        {
            return reporter.cache();
        };
        constexpr auto get_topology = [](caching_reporter const&, test_case_topology const& topo)
        {
            return topo;
        };

        CATCH_SECTION("topology is discovered correctly")
        {
#define MAKE_TESTS(PREFIX)                                                                                             \
    CATCH_##PREFIX##REQUIRE(test(get_topology).size() == 3);                                                           \
    CATCH_##PREFIX##REQUIRE(test(get_topology)[0] == section_path{});                                                  \
    CATCH_##PREFIX##REQUIRE(test(get_topology)[1] == section_path{"first"});                                           \
    CATCH_##PREFIX##REQUIRE(test(get_topology)[2] == section_path{std::is_constant_evaluated() ? "third" : "second"});

            MAKE_TESTS(STATIC_)
            MAKE_TESTS()

#undef MAKE_TESTS
        }

        CATCH_SECTION("assertions are logged correctly")
        {
#define MAKE_TESTS(PREFIX)                                                                                             \
    CATCH_##PREFIX##REQUIRE(test(get_cache).size() == 1);                                                              \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].name == test_name);                                                     \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].sloc == test_location);                                                 \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].test_runs.size() == 1);                                                 \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].test_runs[0].target == section_path{});                                 \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].test_runs[0].assertions.size() == 1);                                   \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].test_runs[0].assertions[0].text == "SUCCEED()");                        \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].test_runs[0].assertions[0].result == true);                             \
    CATCH_##PREFIX##REQUIRE(test(get_cache)[0].test_runs[0].sections.size() == 0);

            MAKE_TESTS(STATIC_)
            MAKE_TESTS()

#undef MAKE_TESTS
        }
    }

    CATCH_SECTION("throwing an exception leads to failed test")
    {
        constexpr char const* what = "the what";
        constexpr auto        test = [=]()
        {
            test_case tc{
                .name            = test_name,
                .tags            = {},
                .source_location = test_location,
                .test_fn         = [](test_run_data&) { throw std::runtime_error{what}; },
            };
            caching_reporter   reporter;
            test_case_topology topo;
            test_run_data      data{reporter, topo, 0};

            reporter.enter_test_case(tc.name, tc.tags, tc.source_location);
            reporter.start_run(data.target());

            evaluate_test_case_target(tc, data);

            reporter.stop_run();
            reporter.leave_test_case();

            return reporter.cache();
        };
        CATCH_REQUIRE(test().size() == 1);
        CATCH_REQUIRE(test()[0].test_runs.size() == 1);
        CATCH_REQUIRE(test()[0].test_runs[0].assertions.size() == 1);
        CATCH_REQUIRE(test()[0].test_runs[0].assertions[0].result == false);
        CATCH_REQUIRE(test()[0].test_runs[0].assertions[0].text == exception_escaped_text);
        CATCH_REQUIRE(test()[0].test_runs[0].assertions[0].messages.size() == 1);

        // Conversion from bs::string to std::string since catch matchers only work with that
        using Catch::Matchers::EndsWith;
        auto const  m = test()[0].test_runs[0].assertions[0].messages[0];
        std::string str{m.begin(), m.end()};
        CATCH_REQUIRE_THAT(str, EndsWith(what));
    }
}