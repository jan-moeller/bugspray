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

#ifndef BUGSPRAY_ASSERTION_MACROS_HPP
#define BUGSPRAY_ASSERTION_MACROS_HPP

#include "bugspray/test_evaluation/decomposition/decomposer.hpp"
#include "bugspray/test_evaluation/decomposition/decomposition_result.hpp"
#include "bugspray/utility/macros.hpp"
#include "bugspray/utility/source_location.hpp"

#include <string_view>

/*
 *  BUGSPRAY_REQUIRE(<cond>): Fails and aborts the test if condition is false.
 *  BUGSPRAY_CHECK(<cond>): Fails and continues the test if condition is false.
 *  BUGSPRAY_FAIL() Fails and aborts the test unconditionally.
 *  BUGSPRAY_FAIL_CHECK() Fails and continues the test unconditionally.
 *  BUGSPRAY_SUCCEED() Logs a successful assertion and continues.
 *  BUGSPRAY_REQUIRE_NOTHROW(<expr>): Fails and aborts the test if expression throws.
 *  BUGSPRAY_CHECK_NOTHROW(<expr>): Fails and continues the test if expression throws.
 *  BUGSPRAY_REQUIRE_THROWS(<expr>): Fails and aborts the test if expression doesn't throw.
 *  BUGSPRAY_CHECK_THROWS(<expr>): Fails and continues the test if expression doesn't throw.
 *  BUGSPRAY_REQUIRE_THROWS_AS(<type>, <expr>): Fails and aborts the test if expression doesn't throw an expression of
 *                                              the given type.
 *  BUGSPRAY_CHECK_THROWS_AS(<type>, <expr>): Fails and continues the test if expression doesn't throw an
 *                                            expression of the given type.
 */

#define BUGSPRAY_ASSERTION_IMPL2(type, text, decomp_str, result)                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        bugspray_data.log_assertion(text, BUGSPRAY_THIS_LOCATION(), decomp_str, result);                               \
        if (!result)                                                                                                   \
        {                                                                                                              \
            bugspray_data.mark_failed();                                                                               \
            if constexpr (std::string_view{#type} == "REQUIRE")                                                        \
                return;                                                                                                \
        }                                                                                                              \
    } while (false)
#define BUGSPRAY_ASSERTION_IMPL(type, text, ...)                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        ::bs::decomposition_result const decom = ::bs::decomposer{} % __VA_ARGS__;                                     \
        BUGSPRAY_ASSERTION_IMPL2(type, text, decom.str(), decom.result());                                             \
    } while (false)
#define BUGSPRAY_ASSERTION_IMPL_MAKE_TEXT(type, ...) #type "(" BUGSPRAY_STRINGIFY_EXPANSION(__VA_ARGS__) ")"

#define BUGSPRAY_REQUIRE(...)                                                                                          \
    BUGSPRAY_ASSERTION_IMPL(REQUIRE, BUGSPRAY_ASSERTION_IMPL_MAKE_TEXT(REQUIRE, __VA_ARGS__), __VA_ARGS__)
#define BUGSPRAY_CHECK(...)                                                                                            \
    BUGSPRAY_ASSERTION_IMPL(CHECK, BUGSPRAY_ASSERTION_IMPL_MAKE_TEXT(CHECK, __VA_ARGS__), __VA_ARGS__)
#define BUGSPRAY_FAIL() BUGSPRAY_ASSERTION_IMPL2(REQUIRE, "FAIL()", "", false)
#define BUGSPRAY_FAIL_CHECK() BUGSPRAY_ASSERTION_IMPL2(CHECK, "FAIL_CHECK()", "", false)
#define BUGSPRAY_SUCCEED() BUGSPRAY_ASSERTION_IMPL2(REQUIRE, "SUCCEED()", "", true)

#define BUGSPRAY_ASSERTION_IMPL_NOTHROW(type, ...)                                                                     \
    try                                                                                                                \
    {                                                                                                                  \
        (void)(__VA_ARGS__);                                                                                           \
    }                                                                                                                  \
    catch (...)                                                                                                        \
    {                                                                                                                  \
        BUGSPRAY_ASSERTION_IMPL2(type, #type "_NOTHROW(" #__VA_ARGS__ ")", "", false);                                 \
    }
#define BUGSPRAY_REQUIRE_NOTHROW(...) BUGSPRAY_ASSERTION_IMPL_NOTHROW(REQUIRE, __VA_ARGS__)
#define BUGSPRAY_CHECK_NOTHROW(...) BUGSPRAY_ASSERTION_IMPL_NOTHROW(CHECK, __VA_ARGS__)

#define BUGSPRAY_ASSERTION_IMPL_THROWS(type, ...)                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        bool did_throw = false;                                                                                        \
        try                                                                                                            \
        {                                                                                                              \
            (void)(__VA_ARGS__);                                                                                       \
        }                                                                                                              \
        catch (...)                                                                                                    \
        {                                                                                                              \
            did_throw = true;                                                                                          \
        }                                                                                                              \
        BUGSPRAY_ASSERTION_IMPL2(type, #type "_THROWS(" #__VA_ARGS__ ")", "", did_throw);                              \
    } while (false)
#define BUGSPRAY_REQUIRE_THROWS(...) BUGSPRAY_ASSERTION_IMPL_THROWS(REQUIRE, __VA_ARGS__)
#define BUGSPRAY_CHECK_THROWS(...) BUGSPRAY_ASSERTION_IMPL_THROWS(CHECK, __VA_ARGS__)

#define BUGSPRAY_ASSERTION_IMPL_THROWS_AS(type, exception_type, ...)                                                   \
    do                                                                                                                 \
    {                                                                                                                  \
        bool did_throw = false;                                                                                        \
        try                                                                                                            \
        {                                                                                                              \
            (void)(__VA_ARGS__);                                                                                       \
        }                                                                                                              \
        catch (exception_type const&)                                                                                  \
        {                                                                                                              \
            did_throw = true;                                                                                          \
        }                                                                                                              \
        catch (...)                                                                                                    \
        {                                                                                                              \
        }                                                                                                              \
        BUGSPRAY_ASSERTION_IMPL2(type, #type "_THROWS_AS(" #__VA_ARGS__ ")", "", did_throw);                           \
    } while (false)
#define BUGSPRAY_REQUIRE_THROWS_AS(exception_type, ...)                                                                \
    BUGSPRAY_ASSERTION_IMPL_THROWS_AS(REQUIRE, exception_type, __VA_ARGS__)
#define BUGSPRAY_CHECK_THROWS_AS(exception_type, ...)                                                                  \
    BUGSPRAY_ASSERTION_IMPL_THROWS_AS(CHECK, exception_type, __VA_ARGS__)

#ifndef BUGSPRAY_NO_SHORT_MACROS
#define REQUIRE(...) BUGSPRAY_REQUIRE(__VA_ARGS__)
#define CHECK(...) BUGSPRAY_CHECK(__VA_ARGS__)
#define FAIL(...) BUGSPRAY_FAIL(__VA_ARGS__)
#define FAIL_CHECK(...) BUGSPRAY_FAIL_CHECK(__VA_ARGS__)
#define SUCCEED(...) BUGSPRAY_SUCCEED(__VA_ARGS__)

#define REQUIRE_NOTHROW(...) BUGSPRAY_REQUIRE_NOTHROW(__VA_ARGS__)
#define CHECK_NOTHROW(...) BUGSPRAY_CHECK_NOTHROW(__VA_ARGS__)
#define REQUIRE_THROWS(...) BUGSPRAY_REQUIRE_THROWS(__VA_ARGS__)
#define CHECK_THROWS(...) BUGSPRAY_CHECK_THROWS(__VA_ARGS__)
#define REQUIRE_THROWS_AS(exception_type, ...) BUGSPRAY_REQUIRE_THROWS_AS(exception_type, __VA_ARGS__)
#define CHECK_THROWS_AS(exception_type, ...) BUGSPRAY_CHECK_THROWS_AS(exception_type, __VA_ARGS__)

#endif

#endif // BUGSPRAY_ASSERTION_MACROS_HPP
