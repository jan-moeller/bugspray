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

#ifndef BUGSPRAY_STATIC_FOR_HPP
#define BUGSPRAY_STATIC_FOR_HPP

#include <concepts>
#include <type_traits>
#include <utility>

namespace bs
{
enum class static_for_continuation
{
    continue_,
    break_,
};

template<std::integral auto Start, std::integral auto End, typename Fn>
constexpr void static_for(Fn&& fn)
{
    using iterator = std::common_type_t<decltype(Start), decltype(End)>;
    if constexpr (std::cmp_equal(Start, End))
        return;
    else
    {
        constexpr bool     increasing = std::cmp_less(Start, End);
        constexpr iterator mod        = increasing ? 1 : -1;

        using arg_type = std::integral_constant<iterator, Start>;
        constexpr arg_type arg;

        auto continuation = static_for_continuation::continue_;
        if constexpr (std::same_as<std::invoke_result_t<Fn, arg_type>, static_for_continuation>)
            continuation = std::forward<Fn>(fn)(arg);
        else
            std::forward<Fn>(fn)(arg);

        if (continuation == static_for_continuation::continue_)
            static_for<Start + mod, End>(std::forward<Fn>(fn));
    }
};
} // namespace bs

#endif // BUGSPRAY_STATIC_FOR_HPP
