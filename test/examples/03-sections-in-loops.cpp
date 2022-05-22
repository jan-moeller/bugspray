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

TEST_CASE("foo")
{
    int  i         = 0;
    bool entered_a = false;
    bool entered_b = false;
    bool entered_c = false;
    for (int j = 0; j < 10; ++j)
    {
        SECTION("a")
        {
            entered_a = true;
            ++i;
        }
        SECTION("b")
        {
            entered_b = true;
            ++i;
            for (int k = 0; k < 5; ++k)
            {
                SECTION("b.c")
                {
                    entered_c = true;
                    ++i;
                }
            }
        }
    }
    CAPTURE(entered_a, entered_b, entered_c);
    REQUIRE((!entered_a && !entered_b) || (entered_a != entered_b));
    REQUIRE((entered_c ? entered_b : true));
    if (entered_a)
        REQUIRE(i == 10);
    else if (entered_b)
    {
        if (entered_c)
            REQUIRE(i == 60);
        else
            REQUIRE(i == 10);
    }
    else
        REQUIRE(i == 0);
}
EVAL_TEST_CASE("foo");
