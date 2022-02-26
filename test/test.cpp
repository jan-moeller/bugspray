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

#include "bugspray/bugspray.hpp"

constexpr void some_test(TEST_CASE_FN_PARAMS)
{
    CHECK(5 != 42);
    SECTION("important")
    {
        SECTION("subsection", runtime)
        {
            REQUIRE(false);
        }
    }
}
REGISTER_TEST_CASE(some_test, "some test", "[register]");
EVAL_TEST_CASE("some test");

void other_test(TEST_CASE_FN_PARAMS)
{
    CHECK(2 == 3);
    SECTION("subsection")
    {
        CHECK(1 == 1);
    }
    SECTION("another subsection")
    {
        CHECK(1 == 1);
    }
    REQUIRE(1 == 2);
    REQUIRE(2 == 3);
}
REGISTER_TEST_CASE(other_test, "other test", "[register]", runtime);

TEST_CASE("foo")
{
    REQUIRE(1 == 1);
}

TEST_CASE("foo with tags", "[tag]")
{
    CHECK(1 == 1);

    SECTION("A.A")
    {
        CHECK(1 == 1);
        SECTION("A.A.A")
        {
            CHECK(1 == 1);
        }
        SECTION("A.A.B", runtime)
        {
            REQUIRE(1 == 2);
        }
    }
    SECTION("A.B")
    {
        CHECK(1 == 1);
        SECTION("A.B.A")
        {
            CHECK(1 == 1);
        }
    }
}
EVAL_TEST_CASE("foo with tags");

TEST_CASE("foo only at runtime", "[tag]", runtime)
{
    REQUIRE(1 == 2);
}

auto main() -> int
{
    ::bs::terminal_reporter reporter;
    for (auto&& t : ::bs::get_test_case_registry().test_cases)
    {
        ::bs::test_run data = ::bs::run_test(t);
        reporter.report(data);
    }

    bool const success = reporter.finalize();

    if (success)
        return EXIT_SUCCESS;
    return EXIT_FAILURE;
}