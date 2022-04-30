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

#include "bugspray/test_evaluation/test_case.hpp"
#include "bugspray/utility/compiletime_string.hpp"
#include "bugspray/utility/vector.hpp"

#include <functional>

namespace bs
{
inline bs::vector<std::reference_wrapper<test_case const>> g_test_case_registry;

template<compiletime_string Name, int Bogus>
struct evaluate_compiletime_test
{
    constexpr auto operator()() -> bool
    {
        static_assert((Name, false), "There is no constexpr-enabled test with this name");
        return false;
    }
};
} // namespace bs

#endif // BUGSPRAY_TEST_CASE_REGISTRY_HPP
