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

#ifndef BUGSPRAY_TO_STRING_TUPLE_HPP
#define BUGSPRAY_TO_STRING_TUPLE_HPP

#include "bugspray/utility/static_for.hpp"
#include "bugspray/utility/string.hpp"

#include <tuple>

namespace bs
{
template<typename T>
constexpr auto stringify(T&& thing) -> bs::string;

template<typename... Ts>
constexpr auto to_string(std::tuple<Ts...> const& t) -> bs::string
{
    bs::string result = "{ ";

    if constexpr (sizeof...(Ts) > 0)
    {
        result += stringify(std::get<0>(t));
        static_for<1, sizeof...(Ts)>([&](auto I) { result += ", " + stringify(std::get<I>(t)); });
        result += " }";
    }
    else
        result += "}";
    return result;
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_TUPLE_HPP
