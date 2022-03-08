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

#ifndef BUGSPRAY_FUNCTION_TEMPLATES_HPP
#define BUGSPRAY_FUNCTION_TEMPLATES_HPP

#include "bugspray/test_run.hpp"
#include "bugspray/utility/string_literal.hpp"

#include <source_location>
#include <span>
#include <string_view>

namespace bs
{
namespace detail
{
template<::bs::string_literal Name>
struct tags_for;
}

using test_fn     = void (*)(test_run*);
using tags_fn     = std::span<std::string_view const> (*)();
using location_fn = std::source_location (*)();

template<string_literal>
constexpr void test(test_run* bugspray_test_case_data) = delete;

template<string_literal>
constexpr auto test_tags() noexcept -> std::span<std::string_view const> = delete;

template<string_literal>
constexpr auto test_location() noexcept -> std::source_location = delete;
} // namespace bs

#endif // BUGSPRAY_FUNCTION_TEMPLATES_HPP
