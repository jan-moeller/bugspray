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

#ifndef BUGSPRAY_TEST_MACROS_HPP
#define BUGSPRAY_TEST_MACROS_HPP

#include "bugspray/test_macros_impl.hpp"

#define TEST_CASE_FN_PARAMS BUGSPRAY_TEST_CASE_FN_PARAMS
#define TEST_CASE_FN_ARGS BUGSPRAY_TEST_CASE_FN_ARGS

#define REGISTER_TEST_CASE(...) BUGSPRAY_REGISTER_TEST_CASE(__VA_ARGS__)
#define TEST_CASE(...) BUGSPRAY_TEST_CASE(__VA_ARGS__)

#define EVAL_TEST_CASE(name) BUGSPRAY_EVAL_TEST_CASE(name)

#define SECTION(...) BUGSPRAY_SECTION(__VA_ARGS__)

#define REQUIRE(...) BUGSPRAY_REQUIRE(__VA_ARGS__)
#define CHECK(...) BUGSPRAY_CHECK(__VA_ARGS__)

#define FAIL(...) REQUIRE(false)
#define FAIL_CHECK(...) CHECK(false)
#define SUCCEED(...) CHECK(true)

#endif // BUGSPRAY_TEST_MACROS_HPP
