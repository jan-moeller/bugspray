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
#include "bugspray/reporter/caching_reporter.hpp"
#include "bugspray/test_evaluation/test_case_topology.hpp"
#include "bugspray/test_evaluation/test_run_data.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("test_run_data", "[test_evaluation]")
{
    constexpr auto test = []()
    {
        section_path const target{"foo", "bar"};

        caching_reporter reporter;
        reporter.enter_test_case("", {}, source_location{});
        reporter.start_run(target);

        test_case_topology topo;
        topo.chart(target);

        test_run_data data{reporter, topo, 1};

        assert((data.target() == target));
        assert((data.current() == section_path{}));

        assert(!data.can_enter_section("bar"));
        assert(data.can_enter_section("foo"));

        data.log_assertion("test", source_location{}, {}, true);

        data.enter_section("foo", source_location{});

        assert(data.can_enter_section("bar"));
        assert(!data.can_enter_section("foo"));

        data.log_assertion("test2", source_location{}, {}, true);
        data.leave_section();

        auto const& c = reporter.cache();
        return c.size() == 1 && c.front().test_runs.size() == 1 && c.front().test_runs.front().assertions.size() == 1;
    };

    STATIC_REQUIRE(test());
    REQUIRE(test());
}