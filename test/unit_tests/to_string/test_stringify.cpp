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
#include "bugspray/to_string/stringify.hpp"

#include <catch2/catch_all.hpp>

namespace ns
{
struct foo
{
};
struct bar
{
};
struct baz
{
};
struct bam
{
};

constexpr auto to_string(foo const&) -> bs::string
{
    return "adl";
}

constexpr auto to_string(bar const&) -> bs::string
{
    return "adl";
}
} // namespace ns

namespace bs
{
constexpr auto to_string(bs::to_string_tag, ns::foo const&) -> bs::string
{
    return "bs";
}
constexpr auto to_string(bs::to_string_tag, ns::bar const&) -> bs::string
{
    return "bs";
}
constexpr auto to_string(bs::to_string_tag, std::same_as<ns::baz> auto&&) -> bs::string
{
    return "bs";
}
} // namespace bs

namespace bs
{
constexpr auto to_string(to_string_override_tag, ns::foo const&) -> bs::string
{
    return "override";
}
} // namespace bs

TEST_CASE("stringify", "[to_string]")
{
#define MAKE_TESTS(PREFIX)                                                                                             \
    PREFIX##CHECK(bs::stringify(ns::foo{}) == "override");                                                             \
    PREFIX##CHECK(bs::stringify(ns::bar{}) == "adl");                                                                  \
    PREFIX##CHECK(bs::stringify(ns::baz{}) == "bs");                                                                   \
    PREFIX##CHECK(bs::stringify(ns::bam{}) == "<?>");                                                                  \
    PREFIX##CHECK(bs::stringify(true) == "true");                                                                      \
    PREFIX##CHECK(bs::stringify('a') == R"('a')");                                                                     \
    PREFIX##CHECK(bs::stringify("foo") == R"("foo")");                                                                 \
    PREFIX##CHECK(bs::stringify(std::string_view{"foo"}) == R"("foo")");                                               \
    PREFIX##CHECK(bs::stringify(bs::string{"foo"}) == R"("foo")");                                                     \
    PREFIX##CHECK(bs::stringify(42) == R"(42)");

    CHECK(bs::stringify(42.1) == R"(42.1)"); // TODO: Add static check once constexpr to_string(double) is implemented

    MAKE_TESTS()
    MAKE_TESTS(STATIC_)
#undef MAKE_TESTS
}