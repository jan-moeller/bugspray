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

#ifndef BUGSPRAY_BUGSPRAY_HPP
#define BUGSPRAY_BUGSPRAY_HPP

#include "bugspray/reporter/terminal_reporter.hpp"
#include "bugspray/section_tracker.hpp"
#include "bugspray/test_case_registry.hpp"
#include "bugspray/test_macros.hpp"
#include "bugspray/test_run.hpp"
#include "bugspray/test_runner.hpp"
#include "bugspray/utility/string_literal.hpp"

namespace bs
{
template<string_literal>
constexpr void test(test_run* bugspray_test_case_data) = delete;
}

#endif // BUGSPRAY_BUGSPRAY_HPP
