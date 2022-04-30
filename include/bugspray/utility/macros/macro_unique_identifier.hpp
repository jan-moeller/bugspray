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

#ifndef BUGSPRAY_MACRO_UNIQUE_IDENTIFIER_HPP
#define BUGSPRAY_MACRO_UNIQUE_IDENTIFIER_HPP

/*
 * BUGSPRAY_UNIQUE_IDENTIFIER(prefix) evaluates to something that starts with the argument followed by something that
 * makes it unique.
 */

#define BUGSPRAY_UNIQUE_IDENTIFIER_IMPL2(prefix, arg) prefix##_##arg
#define BUGSPRAY_UNIQUE_IDENTIFIER_IMPL(prefix, arg) BUGSPRAY_UNIQUE_IDENTIFIER_IMPL2(prefix, arg)
#define BUGSPRAY_UNIQUE_IDENTIFIER(prefix) BUGSPRAY_UNIQUE_IDENTIFIER_IMPL(prefix, __COUNTER__)

#endif // BUGSPRAY_MACRO_UNIQUE_IDENTIFIER_HPP
