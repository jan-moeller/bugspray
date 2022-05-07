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

#ifndef BUGSPRAY_ARGUMENT_DESTINATION_HPP
#define BUGSPRAY_ARGUMENT_DESTINATION_HPP

#include "bugspray/utility/structural_tuple.hpp"

#include <functional>
#include <type_traits>
#include <utility>

namespace bs
{
template<typename... Ts>
struct argument_destination
{
    structural_tuple<Ts...> fns;

    constexpr explicit argument_destination(Ts&&... ts)
        : fns(std::forward<Ts>(ts)...)
    {
    }

    constexpr static auto size = sizeof...(Ts);
};

namespace detail
{
constexpr auto argument_destination_ref(auto const& dest, auto& base, auto I) -> decltype(auto)
{
    if constexpr (std::cmp_less(I.value, dest.size))
    {
        auto fn = get<I>(dest.fns);
        return argument_destination_ref(dest, std::invoke(fn, base), std::integral_constant<int, I + 1>{});
    }
    else
        return base;
}
} // namespace detail

template<argument_destination Dest>
constexpr auto argument_destination_ref(auto& out) -> decltype(auto)
{
    return detail::argument_destination_ref(Dest, out, std::integral_constant<int, 0>{});
}
} // namespace bs

#endif // BUGSPRAY_ARGUMENT_DESTINATION_HPP
