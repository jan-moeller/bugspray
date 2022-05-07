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

#ifndef BUGSPRAY_PARAMETER_NAMES_HPP
#define BUGSPRAY_PARAMETER_NAMES_HPP

#include "bugspray/utility/c_array.hpp"
#include "bugspray/utility/static_for.hpp"
#include "bugspray/utility/structural_string.hpp"
#include "bugspray/utility/structural_tuple.hpp"

#include <ranges>

namespace bs
{
template<std::size_t... Ns>
struct parameter_names
{
    using names_t                     = structural_tuple<structural_string<Ns>...>;
    static constexpr std::size_t size = std::tuple_size_v<names_t>;

    names_t names;
    bool    is_unnamed;
    bool    is_named;

    constexpr parameter_names() = delete;

    constexpr explicit parameter_names(c_array<char const, Ns> const&... n)
        : parameter_names(structural_string(n)...)
    {
    }

    constexpr explicit parameter_names(structural_string<Ns>... n)
        : names(n...)
        , is_unnamed(((!std::string_view(n.value).empty() && !std::string_view(n.value).starts_with('-')) || ...))
        , is_named((std::string_view(n.value).starts_with('-') || ...))
    {
    }

    [[nodiscard]] constexpr auto valid() const -> bool { return is_unnamed != is_named; }

    [[nodiscard]] constexpr auto contains(std::string_view arg) const -> bool
    {
        bool found = false;
        static_for<0, size>(
            [&](auto I) -> static_for_continuation
            {
                auto const& name = get<I>(names);
                if (arg == name)
                {
                    found = true;
                    return static_for_continuation::break_;
                }
                return static_for_continuation::continue_;
            });
        return found;
    }
};
} // namespace bs

#endif // BUGSPRAY_PARAMETER_NAMES_HPP
