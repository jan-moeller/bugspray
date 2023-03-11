//
// MIT License
//
// Copyright (c) 2023 Jan Möller
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

#ifndef BUGSPRAY_TO_STRING_PAIR_HPP
#define BUGSPRAY_TO_STRING_PAIR_HPP

#include "bugspray/utility/string.hpp"

#include <utility>

namespace bs
{
template<typename T>
constexpr auto stringify(T&& thing) -> bs::string;

template<typename T, typename U>
constexpr auto to_string(std::pair<T, U> const& p) -> bs::string
{
    bs::string result = "{ ";
    result += stringify(p.first);
    result += ", ";
    result += stringify(p.second);
    result += " }";
    return result;
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_PAIR_HPP
