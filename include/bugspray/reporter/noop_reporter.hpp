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

#ifndef BUGSPRAY_NOOP_REPORTER_HPP
#define BUGSPRAY_NOOP_REPORTER_HPP

#include "bugspray/reporter/reporter.hpp"

/*
 * The most simple reporter imaginable: It does nothing.
 */

namespace bs
{
struct noop_reporter : reporter
{
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    // TODO: check if gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93413 is fixed
    constexpr ~noop_reporter(){};
#endif

    constexpr void enter_test_case(std::string_view /*name*/,
                                   std::span<std::string_view const> /*tags*/,
                                   source_location /*sloc*/) noexcept override
    {
    }
    constexpr void leave_test_case() noexcept override
    {
    }

    constexpr void start_run(section_path const& /*target*/) noexcept override
    {
    }
    constexpr void stop_run() noexcept override
    {
    }

    constexpr void enter_section(std::string_view /*name*/, source_location /*sloc*/) noexcept override
    {
    }
    constexpr void leave_section() noexcept override
    {
    }

    constexpr void log_assertion(std::string_view /*assertion*/,
                                 source_location /*sloc*/,
                                 std::span<bs::string const> /*messages*/,
                                 bool /*result*/) noexcept override
    {
    }

    constexpr void finalize() noexcept override
    {
    }
};
} // namespace bs

#endif // BUGSPRAY_NOOP_REPORTER_HPP
