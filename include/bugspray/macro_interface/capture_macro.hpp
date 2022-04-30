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

#ifndef BUGSPRAY_CAPTURE_MACRO_HPP
#define BUGSPRAY_CAPTURE_MACRO_HPP

#include "bugspray/test_evaluation/info_capture.hpp"
#include "bugspray/utility/macros.hpp"

#include <array>

// clang-format off
#define BUGSPRAY_CAPTURE_IMPL(names, ...)                                                                              \
    constexpr std::array names = ::bs::split_expression_string<#__VA_ARGS__>();                                        \
    ::bs::info_capture BUGSPRAY_UNIQUE_IDENTIFIER(info_capture)                                                        \
         = ::bs::info_capture_stringification_helper(bugspray_data, names __VA_OPT__(, ) __VA_ARGS__)
// clang-format on
#define BUGSPRAY_CAPTURE(...) BUGSPRAY_CAPTURE_IMPL(BUGSPRAY_UNIQUE_IDENTIFIER(info_capture_names), __VA_ARGS__)

#ifndef BUGSPRAY_NO_SHORT_MACROS
#define CAPTURE(...) BUGSPRAY_CAPTURE(__VA_ARGS__)
#endif

#endif // BUGSPRAY_CAPTURE_MACRO_HPP
