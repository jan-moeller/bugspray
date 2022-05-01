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

#ifndef BUGSPRAY_BINARY_EXPR_HPP
#define BUGSPRAY_BINARY_EXPR_HPP

#include "bugspray/to_string/stringify.hpp"
#include "bugspray/utility/dependent_false.hpp"
#include "bugspray/utility/string.hpp"
#include "bugspray/utility/structural_string.hpp"

#include <type_traits>

namespace bs
{
template<structural_string Op, typename T, typename U, typename V>
struct binary_expr
{
    using lhs_value_type    = std::remove_cvref_t<T>;
    using rhs_value_type    = std::remove_cvref_t<U>;
    using result_value_type = std::remove_cvref_t<V>;

    constexpr binary_expr(lhs_value_type const* lhs, rhs_value_type const* rhs)
        : m_lhs(lhs)
        , m_rhs(rhs)
    {
        if constexpr (Op == ",")
            m_result = (*m_lhs, *m_rhs);

            // clang-format off
#define MAKE_RESULT(op)                                                                                                \
    if constexpr (Op == #op)                                                                                           \
        m_result = (*m_lhs op *m_rhs);
        // clang-format on

        MAKE_RESULT(*)
        MAKE_RESULT(/)
        MAKE_RESULT(%)
        MAKE_RESULT(+)
        MAKE_RESULT(-)
        MAKE_RESULT(<<)
        MAKE_RESULT(>>)
        MAKE_RESULT(<)
        MAKE_RESULT(<=)
        MAKE_RESULT(>)
        MAKE_RESULT(>=)
        MAKE_RESULT(==)
        MAKE_RESULT(!=)
        MAKE_RESULT(&)
        MAKE_RESULT(^)
        MAKE_RESULT(|)
        MAKE_RESULT(&&)
        MAKE_RESULT(||)
#undef MAKE_RESULT
    }

    [[nodiscard]] constexpr auto result() const -> result_value_type const&
    {
        return m_result;
    }
    [[nodiscard]] constexpr auto str_lhs() const noexcept -> bs::string
    {
        return stringify(*m_lhs);
    }
    [[nodiscard]] constexpr auto str_rhs() const noexcept -> bs::string
    {
        return stringify(*m_rhs);
    }
    [[nodiscard]] constexpr auto str_op() const noexcept -> bs::string
    {
        return Op.value;
    }
    [[nodiscard]] constexpr auto str() const noexcept -> bs::string
    {
        return str_lhs() + " " + str_op() + " " + str_rhs();
    }

#define MAKE_OPERATOR(op, msg)                                                                                         \
    template<typename Other>                                                                                           \
    constexpr auto operator op(Other&& rhs) const                                                                      \
    {                                                                                                                  \
        static_assert(dependent_false<Other>, msg);                                                                    \
    }
#define MAKE_REGULAR_OPERATOR(op)                                                                                      \
    MAKE_OPERATOR(op, "Only binary expressions supported. Wrap partial expressions in parentheses.")
#define MAKE_ASSIGNMENT_OPERATOR(op) MAKE_OPERATOR(op, "Assignment in assertions not supported.")

    MAKE_REGULAR_OPERATOR(*)
    MAKE_REGULAR_OPERATOR(/)
    MAKE_REGULAR_OPERATOR(%)
    MAKE_REGULAR_OPERATOR(+)
    MAKE_REGULAR_OPERATOR(-)
    MAKE_REGULAR_OPERATOR(<<)
    MAKE_REGULAR_OPERATOR(>>)
    MAKE_REGULAR_OPERATOR(<)
    MAKE_REGULAR_OPERATOR(<=)
    MAKE_REGULAR_OPERATOR(>)
    MAKE_REGULAR_OPERATOR(>=)
    MAKE_REGULAR_OPERATOR(==)
    MAKE_REGULAR_OPERATOR(!=)
    MAKE_REGULAR_OPERATOR(&)
    MAKE_REGULAR_OPERATOR(^)
    MAKE_REGULAR_OPERATOR(|)
    MAKE_REGULAR_OPERATOR(&&)
    MAKE_REGULAR_OPERATOR(||)
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
#undef MAKE_REGULAR_OPERATOR
#undef MAKE_ASSIGNMENT_OPERATOR
#undef MAKE_OPERATOR

  private:
    lhs_value_type const* m_lhs;
    rhs_value_type const* m_rhs;
    result_value_type     m_result;
};
} // namespace bs

#endif // BUGSPRAY_BINARY_EXPR_HPP
