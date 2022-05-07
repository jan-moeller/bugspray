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
#include "bugspray/cli/argument_destination.hpp"

#include <catch2/catch_all.hpp>

using namespace bs;

TEST_CASE("argument_destination", "[cli]")
{
    struct a
    {
        char i = 'i';
        struct b
        {
            char j = 'j';
            char k = 'k';
            struct c
            {
                char l = 'l';
            } c;
        } b;
    };

    SECTION("constexpr")
    {
        constexpr a a;
        STATIC_REQUIRE(argument_destination_ref<argument_destination{&a::i}>(a) == 'i');
        STATIC_REQUIRE(argument_destination_ref<argument_destination{&a::b, &a::b::j}>(a) == 'j');
        STATIC_REQUIRE(argument_destination_ref<argument_destination{&a::b, &a::b::c, &a::b::c::l}>(a) == 'l');
        STATIC_REQUIRE(argument_destination_ref<argument_destination{[](auto& _a) -> char const&
                                                                     {
                                                                         return _a.b.k;
                                                                     }}>(a)
                       == 'k');
    }
    SECTION("mutable")
    {
        a a;
        argument_destination_ref<argument_destination{&a::i}>(a) = 'q';
        REQUIRE(argument_destination_ref<argument_destination{&a::i}>(a) == 'q');
    }
}