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
#include "bugspray/utility/macros/macro_get_nth_arg_or.hpp"

#include <catch2/catch_all.hpp>

#include <string_view>

TEST_CASE("get_nth_arg_or", "[utility][macros]")
{
    STATIC_REQUIRE(BUGSPRAY_GET_1ST_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 1);
    STATIC_REQUIRE(BUGSPRAY_GET_2ND_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 2);
    STATIC_REQUIRE(BUGSPRAY_GET_3RD_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 3);
    STATIC_REQUIRE(BUGSPRAY_GET_4TH_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 4);
    STATIC_REQUIRE(BUGSPRAY_GET_5TH_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 5);
    STATIC_REQUIRE(BUGSPRAY_GET_6TH_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 6);
    STATIC_REQUIRE(BUGSPRAY_GET_7TH_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 7);
    STATIC_REQUIRE(BUGSPRAY_GET_8TH_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 8);
    STATIC_REQUIRE(BUGSPRAY_GET_9TH_ARG_OR("default", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10) == 9);

    STATIC_REQUIRE(BUGSPRAY_GET_1ST_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_2ND_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_3RD_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_4TH_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_5TH_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_6TH_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_7TH_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_8TH_ARG_OR("default") == std::string_view{"default"});
    STATIC_REQUIRE(BUGSPRAY_GET_9TH_ARG_OR("default") == std::string_view{"default"});
}