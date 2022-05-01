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

#ifndef BUGSPRAY_STRUCTURAL_TUPLE_IMPL_HPP
#define BUGSPRAY_STRUCTURAL_TUPLE_IMPL_HPP

#include "bugspray/utility/static_for.hpp"

#include <compare>
#include <utility>

#include <cstdint>

namespace bs::detail
{
template<std::size_t, typename T>
struct tuple_member
{
    T value;
};

template<typename T>
struct tuple_tag
{
    using type = T;
};

template<typename T, typename U>
constexpr auto synth_three_way(T&& lhs, U&& rhs)
{
    if constexpr (std::three_way_comparable_with<T, U>)
        return lhs <=> rhs;
    else
        return lhs < rhs   ? std::weak_ordering::less
               : rhs < lhs ? std::weak_ordering::greater
                           : std::weak_ordering::equivalent;
}

template<typename, typename...>
struct tuple_impl;

template<std::size_t... Indices, typename... Ts>
struct tuple_impl<std::index_sequence<Indices...>, Ts...> : tuple_member<Indices, Ts>...
{
    constexpr explicit tuple_impl(Ts&&... args)
        : tuple_member<Indices, Ts>(std::move(args))...
    {
    }

    constexpr explicit tuple_impl(Ts const&... args)
        : tuple_member<Indices, Ts>(args)...
    {
    }

    template<typename... RhsTs>
    friend constexpr auto operator==(tuple_impl<std::index_sequence<Indices...>, Ts...> const&    lhs,
                                     tuple_impl<std::index_sequence<Indices...>, RhsTs...> const& rhs) -> bool
    {
        static_assert(sizeof...(Ts) == sizeof...(RhsTs), "Can't compare tuples of varying size.");
        bool result = true;
        bs::static_for<0, sizeof...(Ts)>(
            [&](auto I)
            {
                result = (get<I>(lhs) == get<I>(rhs));
                if (result)
                    return static_for_continuation::continue_;
                return static_for_continuation::break_;
            });
        return result;
    }

    template<typename... RhsTs>
    friend constexpr auto operator<=>(tuple_impl<std::index_sequence<Indices...>, Ts...> const&    lhs,
                                      tuple_impl<std::index_sequence<Indices...>, RhsTs...> const& rhs)
    {
        if constexpr (sizeof...(Indices) == 0)
            return std::strong_ordering::equal;
        else
        {
            int result = 0;
            bs::static_for<0, sizeof...(Ts)>(
                [&](auto I)
                {
                    auto const r = synth_three_way(get<I>(lhs), get<I>(rhs));
                    if (r < 0)
                        result = -1;
                    else if (r > 0)
                        result = 1;
                    else
                        return static_for_continuation::continue_;
                    return static_for_continuation::break_;
                });
            // TODO: infer result type via std::common_comparison_category of all elements instead of always using
            // weak_ordering
            return result < 0   ? std::weak_ordering::less
                   : result > 0 ? std::weak_ordering::greater
                                : std::weak_ordering::equivalent;
        }
    }
};

template<typename... Ts>
tuple_impl(Ts...) -> tuple_impl<std::make_index_sequence<sizeof...(Ts)>, Ts...>;

template<typename T, std::size_t I>
constexpr auto get(tuple_member<I, T> const& tuple) -> T const&
{
    return tuple.value;
}

template<typename T, std::size_t I>
constexpr auto get(tuple_member<I, T>& tuple) -> T&
{
    return tuple.value;
}

template<typename T, std::size_t I>
constexpr auto get(tuple_member<I, T> const&& tuple) -> T const&&
{
    return std::move(tuple.value);
}

template<typename T, std::size_t I>
constexpr auto get(tuple_member<I, T>&& tuple) -> T&&
{
    return std::move(tuple.value);
}

template<std::size_t I, typename T>
constexpr auto get(tuple_member<I, T> const& tuple) -> T const&
{
    return tuple.value;
}

template<std::size_t I, typename T>
constexpr auto get(tuple_member<I, T>& tuple) -> T&
{
    return tuple.value;
}

template<std::size_t I, typename T>
constexpr auto get(tuple_member<I, T> const&& tuple) -> T const&&
{
    return std::move(tuple.value);
}

template<std::size_t I, typename T>
constexpr auto get(tuple_member<I, T>&& tuple) -> T&&
{
    return std::move(tuple.value);
}

template<std::size_t I, typename T>
auto tuple_element_tag(tuple_member<I, T> const&) -> tuple_tag<T>;

template<std::size_t I, typename Tuple>
using tuple_element = decltype(tuple_element_tag<I>(std::declval<Tuple>()));
} // namespace bs::detail

#endif // BUGSPRAY_STRUCTURAL_TUPLE_IMPL_HPP
