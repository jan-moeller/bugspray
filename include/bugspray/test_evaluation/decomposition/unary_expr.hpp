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

#ifndef BUGSPRAY_UNARY_EXPR_HPP
#define BUGSPRAY_UNARY_EXPR_HPP

#include "bugspray/test_evaluation/decomposition/binary_expr.hpp"
#include "bugspray/to_string/stringify.hpp"
#include "bugspray/utility/dependent_false.hpp"
#include "bugspray/utility/string.hpp"

#include <type_traits>
#include <utility>

namespace bs
{
template<typename T>
struct unary_expr
{
    using value_type = std::remove_cvref_t<T>;

    constexpr explicit unary_expr(value_type const* value)
        : m_value(value)
    {
    }

    template<typename U>
    constexpr auto operator,(U&& rhs) const
                            -> binary_expr<",", T, U, std::decay_t<decltype(std::declval<T>(), std::declval<U>())>>
    {
        return {m_value, &rhs};
    }

#define MAKE_OPERATOR(op)                                                                                              \
    template<typename U>                                                                                               \
    constexpr auto operator op(U&& rhs)                                                                                \
        const->binary_expr<#op, T, U, decltype(std::declval<T>() op std::declval<U>())>                                \
    {                                                                                                                  \
        return {m_value, &rhs};                                                                                        \
    }

#define MAKE_ASSIGNMENT_OPERATOR(op)                                                                                   \
    template<typename Other>                                                                                           \
    constexpr auto operator op(Other&& rhs) const                                                                      \
    {                                                                                                                  \
        static_assert(dependent_false<Other>, "Assignment in assertions not supported.");                              \
    }

    MAKE_OPERATOR(*)
    MAKE_OPERATOR(/)
    MAKE_OPERATOR(%)
    MAKE_OPERATOR(+)
    MAKE_OPERATOR(-)
    MAKE_OPERATOR(<<)
    MAKE_OPERATOR(>>)
    MAKE_OPERATOR(<)
    MAKE_OPERATOR(<=)
    MAKE_OPERATOR(>)
    MAKE_OPERATOR(>=)
    MAKE_OPERATOR(==)
    MAKE_OPERATOR(!=)
    MAKE_OPERATOR(&)
    MAKE_OPERATOR(^)
    MAKE_OPERATOR(|)
    MAKE_OPERATOR(&&)
    MAKE_OPERATOR(||)
    MAKE_OPERATOR(<=>)
    MAKE_ASSIGNMENT_OPERATOR(=)
    MAKE_ASSIGNMENT_OPERATOR(+=)
    MAKE_ASSIGNMENT_OPERATOR(-=)
    MAKE_ASSIGNMENT_OPERATOR(*=)
    MAKE_ASSIGNMENT_OPERATOR(/=)
    MAKE_ASSIGNMENT_OPERATOR(%=)
    MAKE_ASSIGNMENT_OPERATOR(<<=)
    MAKE_ASSIGNMENT_OPERATOR(>>=)
    MAKE_ASSIGNMENT_OPERATOR(&=)
    MAKE_ASSIGNMENT_OPERATOR(^=)
    MAKE_ASSIGNMENT_OPERATOR(|=)
#undef MAKE_ASSIGNMENT_OPERATOR
#undef MAKE_OPERATOR

    [[nodiscard]] constexpr auto result() const noexcept -> value_type const& { return *m_value; }
    [[nodiscard]] constexpr auto str() const noexcept -> bs::string { return stringify(*m_value); }

  private:
    value_type const* m_value;
};
} // namespace bs

#endif // BUGSPRAY_UNARY_EXPR_HPP
