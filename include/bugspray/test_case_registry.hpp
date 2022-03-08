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

#ifndef BUGSPRAY_TEST_CASE_REGISTRY_HPP
#define BUGSPRAY_TEST_CASE_REGISTRY_HPP

#include "bugspray/function_templates.hpp"
#include "bugspray/test_run.hpp"

#include <source_location>
#include <span>
#include <string_view>
#include <vector>

namespace bs
{
struct test_case_info
{
    std::string_view name;
    tags_fn          get_tags;
    location_fn      get_location;
    test_fn          test;
};

struct test_case_registry
{
    std::vector<test_case_info> test_cases;
};

auto get_test_case_registry() noexcept -> test_case_registry&;
} // namespace bs

#endif // BUGSPRAY_TEST_CASE_REGISTRY_HPP
