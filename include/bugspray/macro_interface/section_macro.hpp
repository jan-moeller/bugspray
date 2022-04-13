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
#define BUGSPRAY_SECTION(...) if (BUGSPRAY_SECTION_IMPL_TRACKER(__VA_ARGS__))

#ifndef BUGSPRAY_NO_SHORT_MACROS
#define SECTION(...) BUGSPRAY_SECTION(__VA_ARGS__)
#endif

#endif // BUGSPRAY_SECTION_MACRO_HPP
