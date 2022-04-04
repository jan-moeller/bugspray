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

#ifndef BUGSPRAY_MACROS_HPP
#define BUGSPRAY_MACROS_HPP

#define BUGSPRAY_IDENTIY(...) __VA_ARGS__

#define BUGSPRAY_UNIQUE_IDENTIFIER_IMPL2(prefix, arg) prefix##_##arg
#define BUGSPRAY_UNIQUE_IDENTIFIER_IMPL(prefix, arg) BUGSPRAY_UNIQUE_IDENTIFIER_IMPL2(prefix, arg)
#define BUGSPRAY_UNIQUE_IDENTIFIER(prefix) BUGSPRAY_UNIQUE_IDENTIFIER_IMPL(prefix, __COUNTER__)

#define BUGSPRAY_GET_1ST_ARG(arg1, ...) arg1
#define BUGSPRAY_GET_2ND_ARG(arg1, ...) BUGSPRAY_GET_1ST_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_3RD_ARG(arg1, ...) BUGSPRAY_GET_2ND_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_4TH_ARG(arg1, ...) BUGSPRAY_GET_3RD_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_5TH_ARG(arg1, ...) BUGSPRAY_GET_4TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_6TH_ARG(arg1, ...) BUGSPRAY_GET_5TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_7TH_ARG(arg1, ...) BUGSPRAY_GET_6TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_8TH_ARG(arg1, ...) BUGSPRAY_GET_7TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_9TH_ARG(arg1, ...) BUGSPRAY_GET_8TH_ARG(__VA_ARGS__)

#define BUGSPRAY_GET_TAIL_IMPL(arg1, ...) __VA_ARGS__
#define BUGSPRAY_GET_TAIL(...) __VA_OPT__(BUGSPRAY_GET_TAIL_IMPL(__VA_ARGS__))

#define BUGSPRAY_GET_1ST_ARG_OR(alt, ...) BUGSPRAY_GET_1ST_ARG(__VA_OPT__(__VA_ARGS__, ) alt)
#define BUGSPRAY_GET_2ND_ARG_OR(alt, ...) BUGSPRAY_GET_1ST_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_3RD_ARG_OR(alt, ...) BUGSPRAY_GET_2ND_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_4TH_ARG_OR(alt, ...) BUGSPRAY_GET_3RD_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_5TH_ARG_OR(alt, ...) BUGSPRAY_GET_4TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_6TH_ARG_OR(alt, ...) BUGSPRAY_GET_5TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_7TH_ARG_OR(alt, ...) BUGSPRAY_GET_6TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_8TH_ARG_OR(alt, ...) BUGSPRAY_GET_7TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))
#define BUGSPRAY_GET_9TH_ARG_OR(alt, ...) BUGSPRAY_GET_8TH_ARG_OR(alt, BUGSPRAY_GET_TAIL(__VA_ARGS__))

// Works up to 8
#define BUGSPRAY_COUNT_ARGS(...)                                                                                       \
    BUGSPRAY_GET_1ST_ARG_OR(0, __VA_OPT__(BUGSPRAY_GET_9TH_ARG(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)))

#define BUGSPRAY_LINE_STR_IMPL_2(L) #L
#define BUGSPRAY_LINE_STR_IMPL(L) BUGSPRAY_LINE_STR_IMPL_2(L)
#define BUGSPRAY_LINE_STR BUGSPRAY_LINE_STR_IMPL(__LINE__)

#endif // BUGSPRAY_MACROS_HPP
