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

#ifndef BUGSPRAY_DECOMPOSITION_RESULT_HPP
#define BUGSPRAY_DECOMPOSITION_RESULT_HPP

#include "bugspray/test_evaluation/decomposition/binary_expr.hpp"
#include "bugspray/test_evaluation/decomposition/unary_expr.hpp"
#include "bugspray/utility/string.hpp"

#include <type_traits>

namespace bs
{
template<typename T>
struct decomposition_result
{
    using value_type = std::remove_cvref_t<T>;

    bs::string m_str;
    value_type m_result;

    constexpr decomposition_result(unary_expr<T> const& unary)
        : m_str(unary.str())
        , m_result(unary.result())
    {
    }

    template<structural_string Op, typename U, typename V>
    constexpr decomposition_result(binary_expr<Op, U, V, T> const& binary)
        : m_str(binary.str())
        , m_result(binary.result())
    {
    }

    constexpr auto operator=(unary_expr<T> const& unary) -> decomposition_result&
    {
        m_str    = unary.str();
        m_result = unary.result();
        return *this;
    }

    template<structural_string Op, typename U, typename V>
    constexpr auto operator=(binary_expr<Op, U, V, T> const& binary) -> decomposition_result&
    {
        m_str    = binary.str();
        m_result = binary.result();
        return *this;
    }

    [[nodiscard]] constexpr auto str() const noexcept -> bs::string { return m_str; }
    [[nodiscard]] constexpr auto result() const noexcept -> value_type const& { return m_result; }
};
} // namespace bs

#endif // BUGSPRAY_DECOMPOSITION_RESULT_HPP
