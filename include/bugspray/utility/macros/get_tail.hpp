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

#ifndef BUGSPRAY_GET_TAIL_HPP
#define BUGSPRAY_GET_TAIL_HPP

/*
 * BUGSPRAY_GET_TAIL() evaluates to one of the following:
 *  <empty> if the variadic argument list had 0 or 1 arguments
 *  all arguments except the first if the variadic argument list had 2 or more arguments
 */

#define BUGSPRAY_GET_TAIL_IMPL(arg1, ...) __VA_ARGS__
#define BUGSPRAY_GET_TAIL(...) __VA_OPT__(BUGSPRAY_GET_TAIL_IMPL(__VA_ARGS__))

#endif // BUGSPRAY_GET_TAIL_HPP
