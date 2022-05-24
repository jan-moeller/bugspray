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

#ifndef BUGSPRAY_STRINGIFY_HPP
#define BUGSPRAY_STRINGIFY_HPP

#include "bugspray/to_string/to_string_bool.hpp"
#include "bugspray/to_string/to_string_char.hpp"
#include "bugspray/to_string/to_string_container.hpp"
#include "bugspray/to_string/to_string_floating_point.hpp"
#include "bugspray/to_string/to_string_integral.hpp"
#include "bugspray/to_string/to_string_string_like.hpp"
#include "bugspray/to_string/to_string_tag.hpp"
#include "bugspray/utility/string.hpp"

#include <sstream>

namespace bs
{
namespace detail
{
template<typename T>
auto stringify_using_ostream(T&& thing) -> bs::string
{
    std::ostringstream ss;
    ss << std::forward<T>(thing);
    return bs::string{ss.str()};
}
} // namespace detail

template<typename T>
constexpr auto stringify(T&& thing) -> bs::string
{
    if constexpr (requires { to_string(bs::to_string_override_tag{}, std::forward<T>(thing)); })
        return to_string(bs::to_string_override_tag{}, std::forward<T>(thing));
    else if constexpr (requires { to_string(std::forward<T>(thing)); })
        return to_string(std::forward<T>(thing));
    else if constexpr (requires { to_string(bs::to_string_tag{}, std::forward<T>(thing)); })
        return to_string(bs::to_string_tag{}, std::forward<T>(thing));
    else if constexpr (requires(std::ostringstream & ss) { ss << std::forward<T>(thing); })
    {
        if (!std::is_constant_evaluated())
            return detail::stringify_using_ostream(std::forward<T>(thing));
    }
    return "<?>";
}
} // namespace bs

#endif // BUGSPRAY_STRINGIFY_HPP
