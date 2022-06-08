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

#ifndef BUGSPRAY_MAIN_TEST_RUNNER_CONFIG_HPP
#define BUGSPRAY_MAIN_TEST_RUNNER_CONFIG_HPP

#include <random>
#include <string_view>

#include <cstddef>

namespace bs
{
struct config
{
    bool help    = false;
    bool version = false;

    enum class reporter_enum
    {
        console,
        xml,
    } reporter = reporter_enum::console;
    std::string_view output;

    enum class order_enum
    {
        declaration,
        lexicographic,
        random,
    } order = order_enum::declaration;

    bool        report_durations = false;
    std::size_t seed             = std::random_device{}();

    std::string_view test_spec;
};
} // namespace bs

#endif // BUGSPRAY_MAIN_TEST_RUNNER_CONFIG_HPP
