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

#ifndef BUGSPRAY_MACRO_GET_NTH_ARG_OR_HPP
#define BUGSPRAY_MACRO_GET_NTH_ARG_OR_HPP

#include "macro_get_nth_arg.hpp"
#include "macro_get_tail.hpp"

/*
 * This is functionality to retrieve the nth argument from a variadic macro or an alternative default value if the
 * variadic argument list was shorter than what was requested.
 */

#define BUGSPRAY_GET_1ST_ARG_OR(alt, ...) BUGSPRAY_GET_1ST_ARG(__VA_OPT__(__VA_ARGS__, ) alt)
#define BUGSPRAY_GET_2ND_ARG_OR(alt, ...) BUGSPRAY_GET_1ST_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_3RD_ARG_OR(alt, ...) BUGSPRAY_GET_2ND_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_4TH_ARG_OR(alt, ...) BUGSPRAY_GET_3RD_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_5TH_ARG_OR(alt, ...) BUGSPRAY_GET_4TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_6TH_ARG_OR(alt, ...) BUGSPRAY_GET_5TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_7TH_ARG_OR(alt, ...) BUGSPRAY_GET_6TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_8TH_ARG_OR(alt, ...) BUGSPRAY_GET_7TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_9TH_ARG_OR(alt, ...) BUGSPRAY_GET_8TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))

#endif // BUGSPRAY_MACRO_GET_NTH_ARG_OR_HPP
