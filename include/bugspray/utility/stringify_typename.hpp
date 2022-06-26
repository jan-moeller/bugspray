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

#ifndef BUGSPRAY_STRINGIFY_TYPENAME_HPP
#define BUGSPRAY_STRINGIFY_TYPENAME_HPP

#include "bugspray/utility/structural_string.hpp"

#include <array>
#include <string_view>
#include <tuple>

#include <cstddef>

namespace bs
{
namespace detail
{
template<typename T>
[[nodiscard]] constexpr auto pretty_function() -> std::string_view
{
#ifdef __clang__
    return __PRETTY_FUNCTION__;
#elif defined(__GNUC__)
    return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
    return __FUNCSIG__;
#else
    static_assert(false, "Unsupported compiler");
#endif
}

struct typename_probe;
struct pretty_function_metadata
{
    std::size_t prefix;
    std::size_t suffix;
};

constexpr auto compute_pretty_function_metadata() -> pretty_function_metadata
{
    using namespace std::string_view_literals;
    constexpr std::array options{"class bs::detail::typename_probe"sv,
                                 "bs::detail::typename_probe"sv,
                                 "typename_probe"sv};
    constexpr auto       pretty_function_output = pretty_function<typename_probe>();
    constexpr auto const result                 = [&]() -> std::pair<std::string_view, std::size_t>
    {
        for (auto&& opt : options)
        {
            auto const s = pretty_function_output.find(opt);
            if (s != std::string_view::npos)
                return std::make_pair(opt, s);
        }
        return std::make_pair("", -1);
    }();
    static_assert(result.second != std::string_view::npos,
                  "This compiler formats __PRETTY_FUNCTION__ (or equivalent) in an unexpected way");
    constexpr auto prefix = result.second;
    constexpr auto suffix = pretty_function_output.size() - result.first.size() - prefix;
    return pretty_function_metadata{prefix, suffix};
}
constexpr auto pretty_function_metadata = compute_pretty_function_metadata();

template<typename T, typename As>
struct stringifier;

template<typename T>
struct stringifier<T, std::string_view>
{
    [[nodiscard]] constexpr static auto stringify() -> std::string_view
    {
        constexpr auto name = pretty_function<T>();
        constexpr auto meta = compute_pretty_function_metadata();
        return name.substr(meta.prefix, name.size() - meta.prefix - meta.suffix);
    }
};

template<typename T>
struct stringifier<T, void>
{
    [[nodiscard]] constexpr static auto stringify()
    {
        constexpr auto sv = stringifier<T, std::string_view>::stringify();
        return structural_string<sv.size()>{sv};
    }
};
} // namespace detail

/*
 * Returns a string representation of the type T. The optional second template parameter can either be std::string_view
 * of void, where void is a stand-in for a structural_string of the correct size.
 */

template<typename T, typename As = void>
[[nodiscard]] constexpr auto stringify_typename()
{
    using namespace detail;
    return stringifier<T, As>::stringify();
}
} // namespace bs

#endif // BUGSPRAY_STRINGIFY_TYPENAME_HPP
