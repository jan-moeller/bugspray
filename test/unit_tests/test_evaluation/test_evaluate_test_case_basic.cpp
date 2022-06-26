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
    BUGSPRAY_REQUIRE(bugspray_data.current() == section_path{});
    BUGSPRAY_SECTION("t1")
    {
        BUGSPRAY_REQUIRE(bugspray_data.current() == section_path{"t1"});
        BUGSPRAY_REQUIRE(bugspray_data.target().size() > std::size_t{0});
        BUGSPRAY_REQUIRE(bugspray_data.target()[0] == "t1");
        BUGSPRAY_SECTION("t11")
        {
            BUGSPRAY_REQUIRE(bugspray_data.current() == section_path{"t1", "t11"});
            BUGSPRAY_REQUIRE(bugspray_data.target() == bugspray_data.current());
        }
        BUGSPRAY_SECTION("t12")
        {
            int const        i   = 42;
            bs::string const foo = "foo";
            BUGSPRAY_CAPTURE(i, foo);
            BUGSPRAY_REQUIRE(bugspray_data.current() == section_path{"t1", "t12"});
            BUGSPRAY_REQUIRE(bugspray_data.target() == bugspray_data.current());
        }
    }
    BUGSPRAY_SECTION("t2", runtime)
    {
        BUGSPRAY_REQUIRE(bugspray_data.current() == section_path{"t2"});
        BUGSPRAY_REQUIRE(bugspray_data.target().size() > std::size_t{0});
        BUGSPRAY_REQUIRE(bugspray_data.target()[0] == "t2");
    }
    BUGSPRAY_SECTION("t3", compiletime)
    {
        BUGSPRAY_REQUIRE(bugspray_data.current() == section_path{"t3"});
        BUGSPRAY_REQUIRE(bugspray_data.target().size() > std::size_t{0});
        BUGSPRAY_REQUIRE(bugspray_data.target()[0] == "t3");
    }
}

TEST_CASE("evaluate_test_case (basic)", "[test_evaluation]")
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
    PREFIX##REQUIRE(test(return_cache).size() == 1);                                                                   \
    PREFIX##REQUIRE(test(return_cache)[0].name == test_name);                                                          \
    PREFIX##REQUIRE(test(return_cache)[0].sloc == test_sloc);                                                          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs.size() == 5);                                                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[0].target == section_path{});                                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[0].sections.empty());                                              \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[0].assertions.size() == 1);                                        \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[0].assertions[0].result == true);                                  \
                                                                                                                       \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].target == section_path{"t1"});                                  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections.size() == 1);                                          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections[0].name == "t1");                                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections[0].sections.empty());                                  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections[0].assertions.size() == 3);                            \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections[0].assertions[0].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections[0].assertions[1].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].sections[0].assertions[2].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].assertions.size() == 1);                                        \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[1].assertions[0].result == true);                                  \
                                                                                                                       \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].target == section_path{"t1", "t11"});                           \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections.size() == 1);                                          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].name == "t1");                                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].sections.size() == 1);                              \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].sections[0].name == "t11");                         \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].sections[0].sections.size() == 0);                  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].sections[0].assertions.size() == 2);                \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].sections[0].assertions[0].result == true);          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].sections[0].assertions[1].result == true);          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].assertions.size() == 3);                            \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].assertions[0].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].assertions[1].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].sections[0].assertions[2].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].assertions.size() == 1);                                        \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[2].assertions[0].result == true);                                  \
                                                                                                                       \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].target == section_path{"t1", "t12"});                           \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections.size() == 1);                                          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].name == "t1");                                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections.size() == 1);                              \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].name == "t12");                         \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].sections.size() == 0);                  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions.size() == 2);                \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions[0].result == true);          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions[0].expansion                 \
                    == R"({ "t1", "t12" } == { "t1", "t12" })");                                                       \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions[0].messages.size() == 2);    \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions[0].messages[0] == "i: 42");  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions[0].messages[1]               \
                    == R"(foo: "foo")");                                                                               \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].sections[0].assertions[1].result == true);          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].assertions.size() == 3);                            \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].assertions[0].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].assertions[1].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].sections[0].assertions[2].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].assertions.size() == 1);                                        \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[3].assertions[0].result == true);                                  \
                                                                                                                       \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].target                                                          \
                    == section_path{std::is_constant_evaluated() ? "t3" : "t2"});                                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections.size() == 1);                                          \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections[0].name                                                \
                    == (std::is_constant_evaluated() ? "t3" : "t2"));                                                  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections[0].sections.empty());                                  \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections[0].assertions.size() == 3);                            \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections[0].assertions[0].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections[0].assertions[1].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].sections[0].assertions[2].result == true);                      \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].assertions.size() == 1);                                        \
    PREFIX##REQUIRE(test(return_cache)[0].test_runs[4].assertions[0].result == true);

    MAKE_TESTS()
    MAKE_TESTS(STATIC_)
}
