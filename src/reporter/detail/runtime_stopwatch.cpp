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

#include "bugspray/reporter/detail/runtime_stopwatch.hpp"

#include <cassert>

namespace bs::detail
{
runtime_stopwatch::~runtime_stopwatch()
{
    assert(m_section_start_time.empty());
}

void runtime_stopwatch::start_test_case_timer()
{
    m_test_case_start_time = std::chrono::steady_clock::now();
}

auto runtime_stopwatch::stop_test_case_timer() -> std::chrono::milliseconds
{
    auto const now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_test_case_start_time);
}

void runtime_stopwatch::start_section_timer()
{
    auto const now = std::chrono::steady_clock::now();
    m_section_start_time.push_back(now);
}

auto runtime_stopwatch::stop_section_timer() -> std::chrono::milliseconds
{
    auto const now = std::chrono::steady_clock::now();

    assert(!m_section_start_time.empty());
    auto const start = m_section_start_time.back();
    m_section_start_time.pop_back();

    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
}
} // namespace bs::detail