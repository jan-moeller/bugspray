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

TEST_CASE("From a second file", "[TAG]")
{
    SECTION("What")
    {
        SECTION("Is")
        {
            SECTION("Going")
            {
                SECTION("On", runtime)
                {
                    REQUIRE(false);
                }
            }
        }
    }
}
EVAL_TEST_CASE("From a second file");

constexpr void foobar(TEST_CASE_FN_PARAMS, int i)
{
    CHECK(i == 0);
}

TEST_CASE("I call another function")
{
    CHECK(false);
    foobar(TEST_CASE_FN_ARGS, 3);
    CHECK(true);
}

TEST_CASE("This shouldn't fail")
{
    int i = 0;
    SECTION("subsection")
    {
        i = 1;
    }
    REQUIRE(i == 0);
}
EVAL_TEST_CASE("This shouldn't fail");

TEST_CASE("This throws", "", runtime)
{
    SECTION("subsection")
    {
        throw "wtf";
    }
}