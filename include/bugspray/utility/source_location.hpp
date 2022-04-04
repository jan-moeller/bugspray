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

#ifndef BUGSPRAY_SOURCE_LOCATION_HPP
#define BUGSPRAY_SOURCE_LOCATION_HPP

#include <string_view>

#include <cstdint>

namespace bs
{
struct source_location
{
    std::string_view    file_name;
    std::uint_least32_t line;

    constexpr auto operator==(source_location const&) const noexcept -> bool = default;
};

#define BUGSPRAY_THIS_LOCATION                                                                                         \
    ::bs::source_location                                                                                              \
    {                                                                                                                  \
        __FILE__, __LINE__                                                                                             \
    }
} // namespace bs

#endif // BUGSPRAY_SOURCE_LOCATION_HPP
