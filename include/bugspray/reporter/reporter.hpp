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

#ifndef BUGSPRAY_REPORTER_HPP
#define BUGSPRAY_REPORTER_HPP

#include "bugspray/test_evaluation/section_path.hpp"
#include "bugspray/utility/source_location.hpp"
#include "bugspray/utility/string.hpp"

#include <span>
#include <string_view>

/*
 * Virtual base of any test result reporter. Declares APIs that will be called on important events during test
 * execution.
 */

namespace bs
{
struct reporter
{
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
    // TODO: check if gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=93413 is fixed
    virtual constexpr ~reporter(){};
#else
    virtual constexpr ~reporter() = default;
#endif

    virtual constexpr void enter_test_case(std::string_view                  name, //
                                           std::span<std::string_view const> tags,
                                           source_location                   sloc) noexcept = 0;
    virtual constexpr void leave_test_case() noexcept                     = 0;

    virtual constexpr void start_run(section_path const& target) noexcept = 0;
    virtual constexpr void stop_run() noexcept                            = 0;

    virtual constexpr void enter_section(std::string_view name, source_location sloc) noexcept = 0;
    virtual constexpr void leave_section() noexcept                                            = 0;

    virtual constexpr void log_assertion(std::string_view            assertion,
                                         source_location             sloc,
                                         std::string_view            expansion,
                                         std::span<bs::string const> messages,
                                         bool                        result) noexcept = 0;

    virtual constexpr void finalize() noexcept = 0;
};
} // namespace bs

#endif // BUGSPRAY_REPORTER_HPP
