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
#include "bugspray/reporter/formatted_ostream_reporter.hpp"
#include "bugspray/test_evaluation/evaluate_test_case.hpp"
#include "bugspray/test_registration/test_case_registry.hpp"

#include <iostream>

auto main(int /*argc*/, char const** /*argv*/) -> int
{
    bool success = true;

    ::bs::formatted_ostream_reporter reporter{std::cout};
    for (auto&& tc : ::bs::g_test_case_registry)
        success &= ::bs::evaluate_test_case(tc, reporter);

    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}