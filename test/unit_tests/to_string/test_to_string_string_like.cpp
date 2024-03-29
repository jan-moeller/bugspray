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
#include "bugspray/to_string/to_string_string_like.hpp"

#include <catch2/catch_all.hpp>

TEST_CASE("to_string(string-like)", "[to_string]")
{
    constexpr char const* test_cstr = "foobar";

#define MAKE_TESTS(PREFIX)                                                                                             \
    PREFIX##CHECK(bs::to_string("foo") == R"("foo")");                                                                 \
    PREFIX##CHECK(bs::to_string(bs::string{"\n"}) == R"("\n")");                                                       \
    PREFIX##CHECK(bs::to_string(std::string_view{"foo\nbar"}) == R"("foo\nbar")");                                     \
    PREFIX##CHECK(bs::to_string(L"\u2665") == R"("\x00002665")");                                                      \
    PREFIX##CHECK(bs::to_string(L"\U0001f970") == R"("\x0001f970")");                                                  \
    PREFIX##CHECK(bs::to_string(u8"\u2665") == "\"\u2665\"");                                                          \
    PREFIX##CHECK(bs::to_string(u8"\U0001f970") == "\"\U0001f970\"");                                                  \
    PREFIX##CHECK(bs::to_string(u"\u2665") == R"("\x2665")");                                                          \
    PREFIX##CHECK(bs::to_string(u"\U0001f970") == R"("\xd83e\xdd70")");                                                \
    PREFIX##CHECK(bs::to_string(U"\u2665") == R"("\x00002665")");                                                      \
    PREFIX##CHECK(bs::to_string(U"\U0001f970") == R"("\x0001f970")");                                                  \
    PREFIX##CHECK(bs::to_string(test_cstr) == R"("foobar")");

    MAKE_TESTS()
    MAKE_TESTS(STATIC_)
#undef MAKE_TESTS
}