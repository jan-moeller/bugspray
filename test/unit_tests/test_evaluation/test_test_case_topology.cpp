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
#include "bugspray/test_evaluation/test_case_topology.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("test_case_topology", "[test_evaluation]")
{
    constexpr auto test = []()
    {
        test_case_topology t;
        t.chart(section_path{"foo"});
        t.chart(section_path{"foo", "bar"});
        t.chart(section_path{"foo", "bar"}); // Same again
        t.chart(section_path{"foo", "bar", "baz"});
        t.chart(section_path{"foo", "bam"});
        t.chart(section_path{"foo", "bam", "blerp"});
        return t;
    };
    STATIC_REQUIRE(test().size() == 6);
    STATIC_REQUIRE(test()[0] == section_path{});
    STATIC_REQUIRE(test()[1] == section_path{"foo"});
    STATIC_REQUIRE(test()[2] == section_path{"foo", "bar"});
    STATIC_REQUIRE(test()[3] == section_path{"foo", "bar", "baz"});
    STATIC_REQUIRE(test()[4] == section_path{"foo", "bam"});
    STATIC_REQUIRE(test()[5] == section_path{"foo", "bam", "blerp"});
}