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

#ifndef BUGSPRAY_MACRO_WARNING_SUPPRESSION_HPP
#define BUGSPRAY_MACRO_WARNING_SUPPRESSION_HPP

/*
 * Provides macros that disable certain warnings.
 *
 * To be used like this:
 * BUGSPRAY_DISABLE_WARNING_PUSH
 * BUGSPRAY_DISABLE_WARNING_<warning>
 * ...
 * BUGSPRAY_DISABLE_WARNING_POP
 */

#if defined(__GNUC__) || defined(__clang__)
#define BUGSPRAY_PRAGMA(X) _Pragma(#X)
#define BUGSPRAY_DISABLE_WARNING_PUSH BUGSPRAY_PRAGMA(GCC diagnostic push)
#define BUGSPRAY_DISABLE_WARNING_POP BUGSPRAY_PRAGMA(GCC diagnostic pop)
#define BUGSPRAY_DISABLE_WARNING(warningName) BUGSPRAY_PRAGMA(GCC diagnostic ignored #warningName)

// clang-format off
#define BUGSPRAY_DISABLE_WARNING_SHADOW_LOCAL BUGSPRAY_DISABLE_WARNING(-Wshadow)
#define BUGSPRAY_DISABLE_WARNING_UNUSED_VALUE BUGSPRAY_DISABLE_WARNING(-Wunused-value)
#define BUGSPRAY_DISABLE_WARNING_UNUSED_RESULT BUGSPRAY_DISABLE_WARNING(-Wunused-result)
#define BUGSPRAY_DISABLE_WARNING_MISSING_FIELD_INITIALIZERS BUGSPRAY_DISABLE_WARNING(-Wmissing-field-initializers)
// clang-format on

#else
#define BUGSPRAY_DISABLE_WARNING_PUSH
#define BUGSPRAY_DISABLE_WARNING_POP

#define BUGSPRAY_DISABLE_WARNING_SHADOW_LOCAL
#define BUGSPRAY_DISABLE_WARNING_UNUSED_VALUE
#define BUGSPRAY_DISABLE_WARNING_UNUSED_RESULT
#define BUGSPRAY_DISABLE_WARNING_MISSING_FIELD_INITIALIZERS

#endif

#endif // BUGSPRAY_MACRO_WARNING_SUPPRESSION_HPP