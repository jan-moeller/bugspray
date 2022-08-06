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

#ifndef BUGSPRAY_TEST_CASE_MACROS_HPP
#define BUGSPRAY_TEST_CASE_MACROS_HPP

#include "bugspray/test_evaluation/evaluate_test_case.hpp"
#include "bugspray/test_evaluation/parse_tag_string.hpp"
#include "bugspray/test_registration/test_case_registry.hpp"
#include "bugspray/utility/macros.hpp"
#include "bugspray/utility/source_location.hpp"

#include <array>
#include <functional>

/*
 * BUGSPRAY_REGISTER_TEST_CASE(<function>, <name>, <OPT: tags>, <OPT: qualifier>) registers a function as test case.
 * Arguments:
 *   <function>: Function to register
 *   <name>: Name of the test case as string. Must be unique.
 *   <tags>: Optional string of tags, with tags enclosed by square brackets. Eg: "[tag1][tag2]".
 *   <qualifier>: Optional qualifier. One of [runtime, compiletime, both]. Defaults to both.
 *
 * BUGSPRAY_TEST_CASE(<name>, <OPT: tags>, <OPT: qualifier>) registers the function that is following as a test case.
 * Arguments:
 *   <name>: Name of the test case as string. Must be unique.
 *   <tags>: Optional string of tags, with tags enclosed by square brackets. Eg: "[tag1][tag2]".
 *   <qualifier>: Optional qualifier. One of [runtime, compiletime, both]. Defaults to both.
 *
 * BUGSPRAY_EVAL_TEST_CASE(<name>) constexpr-evaluates a previously defined test case.
 * Arguments:
 *   <name>: Name of the test case as string.
 */

#ifndef BUGSPRAY_COMPILE_EVAL_TEST_SPEC
#define BUGSPRAY_COMPILE_EVAL_TEST_SPEC ""
#endif

// clang-format off
#define BUGSPRAY_REGISTER_TEST_CASE_IMPL_runtime(name, test_case_id)                                                   \
    static bool const BUGSPRAY_UNIQUE_IDENTIFIER(bugspray_registration)                                                \
        = (::bs::g_test_case_registry.push_back(std::cref(test_case_id)), false);
// clang-format on
#define BUGSPRAY_REGISTER_TEST_CASE_IMPL_compiletime(name, test_case_id)                                               \
    template<int Bogus>                                                                                                \
    struct bs::evaluate_compiletime_test<name, Bogus>                                                                  \
    {                                                                                                                  \
        constexpr auto operator()() -> bool                                                                            \
        {                                                                                                              \
            constexpr auto results = ::bs::evaluate_test_case_constexpr<Bogus>(test_case_id,                           \
                                                                               BUGSPRAY_COMPILE_EVAL_TEST_SPEC);       \
            if constexpr (!results.first)                                                                              \
            {                                                                                                          \
                constexpr auto m = ::bs::trim<results.second>();                                                       \
                ::bs::compile_time_error<m>();                                                                         \
            }                                                                                                          \
            return results.first;                                                                                      \
        }                                                                                                              \
    };
// clang-format off
#define BUGSPRAY_REGISTER_TEST_CASE_IMPL_both(name, test_case_id)                                                      \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL_runtime(name, test_case_id)                                                       \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL_compiletime(name, test_case_id)
// clang-format on

#define BUGSPRAY_REGISTER_TEST_CASE_IMPL_COMMON(test_case_id, tags_id, fn, name, tags, qualifier)                      \
    static constexpr std::array      tags_id = ::bs::parse_tag_string<tags>();                                         \
    static constexpr ::bs::test_case test_case_id{name, tags_id, BUGSPRAY_THIS_LOCATION(), fn};                        \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL_##qualifier(name, test_case_id)

#define BUGSPRAY_REGISTER_TEST_CASE_IMPL(fn, name, tags, qualifier)                                                    \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL_COMMON(BUGSPRAY_UNIQUE_IDENTIFIER(bugspray_test_case),                            \
                                            BUGSPRAY_UNIQUE_IDENTIFIER(bugspray_tags),                                 \
                                            fn,                                                                        \
                                            name,                                                                      \
                                            tags,                                                                      \
                                            qualifier)

#define BUGSPRAY_REGISTER_TEST_CASE(...)                                                                               \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL(BUGSPRAY_GET_1ST_ARG(__VA_ARGS__),                                                \
                                     BUGSPRAY_GET_2ND_ARG(__VA_ARGS__),                                                \
                                     BUGSPRAY_GET_3RD_ARG_OR("", __VA_ARGS__),                                         \
                                     BUGSPRAY_GET_4TH_ARG_OR(both, __VA_ARGS__))

#define BUGSPRAY_TEST_CASE_IMPL_CONSTEXPR_runtime
#define BUGSPRAY_TEST_CASE_IMPL_CONSTEXPR_compiletime constexpr
#define BUGSPRAY_TEST_CASE_IMPL_CONSTEXPR_both constexpr

#define BUGSPRAY_TEST_CASE_IMPL_COMMON(name, tags, qualifier, test_fn_id)                                              \
    static BUGSPRAY_TEST_CASE_IMPL_CONSTEXPR_##qualifier void test_fn_id(::bs::test_run_data& bugspray_data);          \
    BUGSPRAY_REGISTER_TEST_CASE(test_fn_id, name, tags, qualifier);                                                    \
    static BUGSPRAY_TEST_CASE_IMPL_CONSTEXPR_##qualifier void test_fn_id(::bs::test_run_data& bugspray_data)

#define BUGSPRAY_TEST_CASE_IMPL(name, tags, qualifier)                                                                 \
    BUGSPRAY_TEST_CASE_IMPL_COMMON(name, tags, qualifier, BUGSPRAY_UNIQUE_IDENTIFIER(bugspray_test_case_fn))

#define BUGSPRAY_TEST_CASE(...)                                                                                        \
    BUGSPRAY_TEST_CASE_IMPL(BUGSPRAY_GET_1ST_ARG(__VA_ARGS__),                                                         \
                            BUGSPRAY_GET_2ND_ARG_OR("", __VA_ARGS__),                                                  \
                            BUGSPRAY_GET_3RD_ARG_OR(both, __VA_ARGS__))

#define BUGSPRAY_EVAL_TEST_CASE(name) static_assert(::bs::evaluate_compiletime_test<name, 0>{}())

#ifndef BUGSPRAY_NO_SHORT_MACROS
#define REGISTER_TEST_CASE(...) BUGSPRAY_REGISTER_TEST_CASE(__VA_ARGS__)
#define TEST_CASE(...) BUGSPRAY_TEST_CASE(__VA_ARGS__)
#define EVAL_TEST_CASE(...) BUGSPRAY_EVAL_TEST_CASE(__VA_ARGS__)
#endif

#endif // BUGSPRAY_TEST_CASE_MACROS_HPP
