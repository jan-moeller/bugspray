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

#ifndef BUGSPRAY_TO_STRING_POINTER_HPP
#define BUGSPRAY_TO_STRING_POINTER_HPP

#include "bugspray/to_string/to_string_integral.hpp"
#include "bugspray/utility/string.hpp"

#include <bit>
#include <type_traits>

#include <cstdint>

namespace bs
{
template<typename T>
    requires(std::is_pointer_v<T>)
constexpr auto to_string([[maybe_unused]] T ptr) -> bs::string
{
    if (ptr == nullptr)
        return "nullptr";
    if (std::is_constant_evaluated())
        return "unknown pointer value";
    return to_string<16>(reinterpret_cast<std::uintptr_t>(ptr));
}

constexpr auto to_string(std::nullptr_t) -> bs::string
{
    return "nullptr";
}
} // namespace bs

#endif // BUGSPRAY_TO_STRING_POINTER_HPP
