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

#ifndef BUGSPRAY_STATIC_FOR_EACH_TYPE_HPP
#define BUGSPRAY_STATIC_FOR_EACH_TYPE_HPP

#include "bugspray/utility/static_for.hpp"
#include "bugspray/utility/structural_tuple.hpp"

#include <tuple>

namespace bs
{
template<typename... Types, typename Fn>
constexpr void static_for_each_type(Fn&& fn)
{
    using types = structural_tuple<Types...>;
    static_for<0, sizeof...(Types)>(
        [&fn](auto I)
        {
            using T = std::tuple_element_t<I, types>;
            std::forward<Fn>(fn).template operator()<T>();
        });
}
} // namespace bs

#endif // BUGSPRAY_STATIC_FOR_EACH_TYPE_HPP
