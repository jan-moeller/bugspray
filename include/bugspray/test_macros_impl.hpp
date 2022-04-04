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

#ifndef BUGSPRAY_TEST_MACROS_IMPL_HPP
#define BUGSPRAY_TEST_MACROS_IMPL_HPP

#include "bugspray/utility/macros.hpp"
#include "bugspray/utility/warning_suppression.hpp"

#define BUGSPRAY_QUALIFIER_both constexpr
#define BUGSPRAY_QUALIFIER_runtime
#define BUGSPRAY_CONSTEXPR_CHOOSER(qualifier) BUGSPRAY_QUALIFIER_##qualifier

// Register fn with name and tags for execution at runtime
#define BUGSPRAY_REGISTER_RUNTIME_TEST_CASE_IMPL(fn, name, tags)                                                       \
    [[maybe_unused]] static bool const BUGSPRAY_UNIQUE_IDENTIFIER(                                                     \
        faux_registration) = (::bs::get_test_case_registry().test_cases.push_back({name,                               \
                                                                                   &::bs::test_tags<name>,             \
                                                                                   &::bs::test_location<name>,         \
                                                                                   &::bs::test<name>}),                \
                              true)

#define BUGSPRAY_TEST_CASE_FN_PARAMS ::bs::test_run* bugspray_test_case_data
#define BUGSPRAY_TEST_CASE_FN_ARGS bugspray_test_case_data

// Register fn with name and tags for execution at runtime. If qualifier is "both", the function is constexpr
#define BUGSPRAY_DECLARE_TEST_FN(name, qualifier)                                                                      \
    template<>                                                                                                         \
    BUGSPRAY_CONSTEXPR_CHOOSER(qualifier)                                                                              \
    void ::bs::test<name>(BUGSPRAY_TEST_CASE_FN_PARAMS)
#define BUGSPRAY_DEFINE_TEST_TAGS_FN(name, tags)                                                                       \
    namespace bs::detail                                                                                               \
    {                                                                                                                  \
    template<>                                                                                                         \
    struct tags_for<name>                                                                                              \
    {                                                                                                                  \
        static constexpr auto get() noexcept -> std::span<std::string_view const>                                      \
        {                                                                                                              \
            return m_data.m_tags;                                                                                      \
        }                                                                                                              \
        static constexpr auto m_data = ::bs::parse_tags<tags>();                                                       \
    };                                                                                                                 \
    }                                                                                                                  \
    template<>                                                                                                         \
    constexpr auto ::bs::test_tags<name>() noexcept -> std::span<std::string_view const>                               \
    {                                                                                                                  \
        return ::bs::detail::tags_for<name>::get();                                                                    \
    }
#define BUGSPRAY_DEFINE_TEST_LOCATION_FN(name)                                                                         \
    template<>                                                                                                         \
    constexpr auto ::bs::test_location<name>() noexcept -> std::source_location                                        \
    {                                                                                                                  \
        return std::source_location::current();                                                                        \
    }
#define BUGSPRAY_REGISTER_TEST_CASE_IMPL(fn, name, tags, qualifier)                                                    \
    BUGSPRAY_DEFINE_TEST_TAGS_FN(name, tags)                                                                           \
    BUGSPRAY_DEFINE_TEST_LOCATION_FN(name)                                                                             \
    BUGSPRAY_DECLARE_TEST_FN(name, qualifier)                                                                          \
    {                                                                                                                  \
        fn(BUGSPRAY_TEST_CASE_FN_ARGS);                                                                                \
    }                                                                                                                  \
    BUGSPRAY_REGISTER_RUNTIME_TEST_CASE_IMPL(fn, name, tags)

// Register block after macro as test case with name and tags. If qualifier is "both", the function is constexpr
#define BUGSPRAY_TEST_CASE_IMPL(name, tags, qualifier)                                                                 \
    BUGSPRAY_DECLARE_TEST_FN(name, qualifier);                                                                         \
    BUGSPRAY_DEFINE_TEST_TAGS_FN(name, tags)                                                                           \
    BUGSPRAY_DEFINE_TEST_LOCATION_FN(name)                                                                             \
    BUGSPRAY_REGISTER_RUNTIME_TEST_CASE_IMPL(fn, name, tags);                                                          \
    BUGSPRAY_DECLARE_TEST_FN(name, qualifier)

// Register fn. name, tags and qualifier are optional
#define BUGSPRAY_REGISTER_TEST_CASE_FN_IMPL(fn) BUGSPRAY_REGISTER_TEST_CASE_IMPL(fn, fn, "", both)
#define BUGSPRAY_REGISTER_TEST_CASE_FN_NAME_IMPL(fn, name) BUGSPRAY_REGISTER_TEST_CASE_IMPL(fn, name, "", both)
#define BUGSPRAY_REGISTER_TEST_CASE_FN_NAME_TAGS_IMPL(fn, name, tags)                                                  \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL(fn, name, tags, both)
#define BUGSPRAY_REGISTER_TEST_CASE_FN_NAME_TAGS_QUALIFIER_IMPL(fn, name, tags, qualifier)                             \
    BUGSPRAY_REGISTER_TEST_CASE_IMPL(fn, name, tags, qualifier)
#define BUGSPRAY_REGISTER_TEST_CASE_MACRO_CHOOSER_IMPL(...)                                                            \
    BUGSPRAY_GET_5TH_ARG(__VA_ARGS__,                                                                                  \
                         BUGSPRAY_REGISTER_TEST_CASE_FN_NAME_TAGS_QUALIFIER_IMPL,                                      \
                         BUGSPRAY_REGISTER_TEST_CASE_FN_NAME_TAGS_IMPL,                                                \
                         BUGSPRAY_REGISTER_TEST_CASE_FN_NAME_IMPL,                                                     \
                         BUGSPRAY_REGISTER_TEST_CASE_FN_IMPL, )
#define BUGSPRAY_REGISTER_TEST_CASE(...) BUGSPRAY_REGISTER_TEST_CASE_MACRO_CHOOSER_IMPL(__VA_ARGS__)(__VA_ARGS__)

// Register block after macro as test case with name. tags and qualifier are optional
#define BUGSPRAY_TEST_CASE_NAME_IMPL(name) BUGSPRAY_TEST_CASE_IMPL(name, "", both)
#define BUGSPRAY_TEST_CASE_NAME_TAGS_IMPL(name, tags) BUGSPRAY_TEST_CASE_IMPL(name, tags, both)
#define BUGSPRAY_TEST_CASE_NAME_TAGS_QUALIFIER_IMPL(name, tags, qualifier)                                             \
    BUGSPRAY_TEST_CASE_IMPL(name, tags, qualifier)
#define BUGSPRAY_TEST_CASE_MACRO_CHOOSER_IMPL(...)                                                                     \
    BUGSPRAY_GET_4TH_ARG(__VA_ARGS__,                                                                                  \
                         BUGSPRAY_TEST_CASE_NAME_TAGS_QUALIFIER_IMPL,                                                  \
                         BUGSPRAY_TEST_CASE_NAME_TAGS_IMPL,                                                            \
                         BUGSPRAY_TEST_CASE_NAME_IMPL, )
#define BUGSPRAY_TEST_CASE(...) BUGSPRAY_TEST_CASE_MACRO_CHOOSER_IMPL(__VA_ARGS__)(__VA_ARGS__)

// Constant-evaluates test case
#define BUGSPRAY_EVAL_TEST_CASE(name)                                                                                  \
    static_assert(                                                                                                     \
        ::bs::run_test(::bs::test_case_info{name, ::bs::test_tags<name>, ::bs::test_location<name>, ::bs::test<name>}) \
            .passed())

// Section inside a test case
#define BUGSPRAY_SECTION_IMPL_runtime(name)                                                                            \
    if ([[maybe_unused]] auto bugspray_tracker = ::bs::section_tracker{BUGSPRAY_TEST_CASE_FN_ARGS,                     \
                                                                       name,                                           \
                                                                       __FILE__,                                       \
                                                                       __LINE__};                                      \
        !std::is_constant_evaluated() && bugspray_tracker.should_enter())
#define BUGSPRAY_SECTION_IMPL_both(name)                                                                               \
    if ([[maybe_unused]] auto bugspray_tracker = ::bs::section_tracker{BUGSPRAY_TEST_CASE_FN_ARGS,                     \
                                                                       name,                                           \
                                                                       __FILE__,                                       \
                                                                       __LINE__};                                      \
        bugspray_tracker.should_enter())
#define BUGSPRAY_SECTION_IMPL(name, qualifier) BUGSPRAY_SECTION_IMPL_##qualifier(name)
#define BUGSPRAY_SECTION_NAME_IMPL(name) BUGSPRAY_SECTION_IMPL(name, both)
#define BUGSPRAY_SECTION_NAME_QUALIFIER_IMPL(name, qualifier) BUGSPRAY_SECTION_IMPL(name, qualifier)
#define BUGSPRAY_SECTION_MACRO_CHOOSER_IMPL(...)                                                                       \
    BUGSPRAY_GET_3RD_ARG(__VA_ARGS__, BUGSPRAY_SECTION_NAME_QUALIFIER_IMPL, BUGSPRAY_SECTION_NAME_IMPL, )
#define BUGSPRAY_SECTION_WARNING_SUPPRESSION_ON BUGSPRAY_DISABLE_WARNING_PUSH BUGSPRAY_DISABLE_WARNING_SHADOW_LOCAL
#define BUGSPRAY_SECTION_WARNING_SUPPRESSION_OFF BUGSPRAY_DISABLE_WARNING_POP
#define BUGSPRAY_SECTION(...)                                                                                          \
    BUGSPRAY_SECTION_WARNING_SUPPRESSION_ON                                                                            \
    BUGSPRAY_SECTION_MACRO_CHOOSER_IMPL(__VA_ARGS__)(__VA_ARGS__) BUGSPRAY_SECTION_WARNING_SUPPRESSION_OFF

// assert a conditional and log failure
#define BUGSPRAY_ASSERT(type, ...)                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        bool const pass = (__VA_ARGS__);                                                                               \
        if (!BUGSPRAY_TEST_CASE_FN_ARGS->executed_leaf_section && !pass)                                               \
        {                                                                                                              \
            if (std::is_constant_evaluated())                                                                          \
                throw "TEST FAILED";                                                                                   \
            else                                                                                                       \
            {                                                                                                          \
                ::bs::log_assertion_failure(BUGSPRAY_TEST_CASE_FN_ARGS,                                                \
                                            __FILE__,                                                                  \
                                            __LINE__,                                                                  \
                                            type "(" #__VA_ARGS__ ")");                                                \
            }                                                                                                          \
            if constexpr (std::string_view{type} == "REQUIRE")                                                         \
                return;                                                                                                \
        }                                                                                                              \
        if (::bs::pass_info pass_info{__FILE__, __LINE__};                                                             \
            std::ranges::find(BUGSPRAY_TEST_CASE_FN_ARGS->passes, pass_info)                                           \
                == BUGSPRAY_TEST_CASE_FN_ARGS->passes.end()                                                            \
            && pass)                                                                                                   \
            BUGSPRAY_TEST_CASE_FN_ARGS->passes.push_back(pass_info);                                                   \
    } while (false)
#define BUGSPRAY_REQUIRE(...) BUGSPRAY_ASSERT("REQUIRE", __VA_ARGS__)
#define BUGSPRAY_CHECK(...) BUGSPRAY_ASSERT("CHECK", __VA_ARGS__)
#endif // BUGSPRAY_TEST_MACROS_IMPL_HPP
