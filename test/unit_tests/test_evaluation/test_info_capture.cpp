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
#include "bugspray/reporter/noop_reporter.hpp"
#include "bugspray/test_evaluation/info_capture.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("split_expression_string", "[test_evaluation]")
{
    using namespace std::string_view_literals;

    STATIC_CHECK(split_expression_string<"">().size() == 0);
    STATIC_CHECK(split_expression_string<"   ">().size() == 0);
    STATIC_CHECK(split_expression_string<"a">() == std::array{"a"sv});
    STATIC_CHECK(split_expression_string<" a ">() == std::array{"a"sv});
    STATIC_CHECK(split_expression_string<"a, b">() == std::array{"a"sv, "b"sv});
    STATIC_CHECK(split_expression_string<"a(), int(b), (int)c">() == std::array{"a()"sv, "int(b)"sv, "(int)c"sv});
    STATIC_CHECK(split_expression_string<"foo(1, bar, baz<int, 3>{})">() == std::array{"foo(1, bar, baz<int, 3>{})"sv});
}

TEST_CASE("info_capture", "[test_evaluation]")
{
    noop_reporter      reporter{};
    test_case_topology topo;
    test_run_data      data{reporter, topo, 0};
    info_capture       ic{data, std::array{bs::string{"msg1"}, bs::string{"msg2"}}};

    REQUIRE(ic.m_count == 2);
}