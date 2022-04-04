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

#ifndef BUGSPRAY_GET_NTH_ARG_HPP
#define BUGSPRAY_GET_NTH_ARG_HPP

/*
 * This is functionality to retrieve the nth argument from a variadic macro.
 *
 * It's an error if not at least that many arguments have been passed.
 */

#define BUGSPRAY_GET_1ST_ARG(arg1, ...) arg1
#define BUGSPRAY_GET_2ND_ARG(arg1, ...) BUGSPRAY_GET_1ST_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_3RD_ARG(arg1, ...) BUGSPRAY_GET_2ND_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_4TH_ARG(arg1, ...) BUGSPRAY_GET_3RD_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_5TH_ARG(arg1, ...) BUGSPRAY_GET_4TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_6TH_ARG(arg1, ...) BUGSPRAY_GET_5TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_7TH_ARG(arg1, ...) BUGSPRAY_GET_6TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_8TH_ARG(arg1, ...) BUGSPRAY_GET_7TH_ARG(__VA_ARGS__)
#define BUGSPRAY_GET_9TH_ARG(arg1, ...) BUGSPRAY_GET_8TH_ARG(__VA_ARGS__)

#endif // BUGSPRAY_GET_NTH_ARG_HPP
