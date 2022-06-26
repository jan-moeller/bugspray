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

#ifndef BUGSPRAY_SECTION_MACRO_HPP
#define BUGSPRAY_SECTION_MACRO_HPP

#include "bugspray/test_evaluation/section_tracker.hpp"
#include "bugspray/utility/macros.hpp"
#include "bugspray/utility/source_location.hpp"

/*
 * BUGSPRAY_SECTION(<name>, <OPT: qualifier>) defines a section in a test case.
 * Arguments:
 *   <name>: Required name string. String must be unique among sections of the same nesting level.
 *   <qualifier>: Optional qualifier. One of [runtime, compiletime, both]. Defaults to both.
 *
 * BUGSPRAY_DYNAMIC_SECTION(<dynamic name>, <OPT: qualifier>) defines a section with a dynamic (non-literal) name in a
 * test case.
 * Arguments:
 *   <dynamic name>: A structural_string
 *   <qualifier>: Optional qualifier. One of [runtime, compiletime, both]. Defaults to both.
 */

#define BUGSPRAY_SECTION_IMPL_SECTION_NAME(...) BUGSPRAY_GET_1ST_ARG(__VA_ARGS__)
#define BUGSPRAY_SECTION_IMPL_SECTION_QUALIFIER(...)                                                                   \
    BUGSPRAY_CONCATENATE_EXPANSION(BUGSPRAY_SECTION_IMPL_EXTRA_COND_, BUGSPRAY_GET_2ND_ARG_OR(both, __VA_ARGS__))
#define BUGSPRAY_SECTION_IMPL_TRACKER(...)                                                                             \
    ::bs::section_tracker BUGSPRAY_UNIQUE_IDENTIFIER(section_tracker)                                                  \
    {                                                                                                                  \
        bugspray_data, BUGSPRAY_SECTION_IMPL_SECTION_NAME(__VA_ARGS__), BUGSPRAY_THIS_LOCATION(),                      \
            BUGSPRAY_SECTION_IMPL_SECTION_QUALIFIER(__VA_ARGS__)                                                       \
    }

#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_runtime !std::is_constant_evaluated()
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_compiletime std::is_constant_evaluated()
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_both true
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_runtime_if(...)                                                               \
    (__VA_ARGS__) ? (BUGSPRAY_SECTION_IMPL_EXTRA_COND_runtime) : (BUGSPRAY_SECTION_IMPL_EXTRA_COND_compiletime)
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_compiletime_if(...)                                                           \
    (__VA_ARGS__) ? (BUGSPRAY_SECTION_IMPL_EXTRA_COND_compiletime) : (BUGSPRAY_SECTION_IMPL_EXTRA_COND_runtime)
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_only_if(...) (__VA_ARGS__)
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_only_runtime_if(...) BUGSPRAY_SECTION_IMPL_EXTRA_COND_runtime
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_only_compiletime_if(...) BUGSPRAY_SECTION_IMPL_EXTRA_COND_compiletime
#define BUGSPRAY_SECTION_IMPL_EXTRA_COND_disabled false

#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_runtime true
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_compiletime true
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_both true
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_runtime_if(...) true
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_compiletime_if(...) true
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_only_if(...) (__VA_ARGS__)
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_only_runtime_if(...) (__VA_ARGS__)
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_only_compiletime_if(...) (__VA_ARGS__)
#define BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_disabled false

#define BUGSPRAY_SECTION(...)                                                                                          \
    if constexpr (BUGSPRAY_CONCATENATE_EXPANSION(BUGSPRAY_SECTION_IMPL_CONSTEXPR_CONDITION_,                           \
                                                 BUGSPRAY_GET_2ND_ARG_OR(both, __VA_ARGS__)))                          \
        if (BUGSPRAY_SECTION_IMPL_TRACKER(__VA_ARGS__))

#define BUGSPRAY_DYNAMIC_SECTION_IMPL(storage_id, name, ...)                                                           \
    constexpr ::bs::structural_string storage_id{name};                                                                \
    BUGSPRAY_SECTION(std::string_view{storage_id}, __VA_ARGS__)
#define BUGSPRAY_DYNAMIC_SECTION(...)                                                                                  \
    BUGSPRAY_DYNAMIC_SECTION_IMPL(BUGSPRAY_UNIQUE_IDENTIFIER(dynamic_section), __VA_ARGS__)

#ifndef BUGSPRAY_NO_SHORT_MACROS
#define SECTION(...) BUGSPRAY_SECTION(__VA_ARGS__)
#define DYNAMIC_SECTION(...) BUGSPRAY_DYNAMIC_SECTION(__VA_ARGS__)
#endif

#endif // BUGSPRAY_SECTION_MACRO_HPP
