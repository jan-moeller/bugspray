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
#include "bugspray/reporter/caching_reporter.hpp"

#include <catch2/catch_all.hpp>

#include <array>

using namespace bs;

TEST_CASE("caching_reporter", "[reporter]")
{
    constexpr std::string_view filename = "some_file.cpp";

    constexpr std::string_view test_case_name = "test_case";
    constexpr std::string_view test_case_tag  = "tag";
    constexpr std::array       test_case_tags = {test_case_tag};
    constexpr std::size_t      test_case_line = 10;

    constexpr std::string_view section_name = "section";
    constexpr std::size_t      section_line = 20;

    constexpr std::string_view assertion         = "FAIL()";
    constexpr std::size_t      assertion_line    = 30;
    constexpr std::string_view assertion_message = "WITH something super important";
    constexpr bool             assertion_result  = false;

    constexpr auto test = [filename,
                           test_case_name,
                           test_case_tags,
                           test_case_line,
                           section_name,
                           section_line,
                           assertion,
                           assertion_line,
                           assertion_message,
                           assertion_result]()
    {
        caching_reporter reporter;

        reporter.enter_test_case(test_case_name,
                                 test_case_tags,
                                 source_location{.file_name = filename, .line = test_case_line});

        reporter.start_run();
        reporter.log_target({});
        reporter.stop_run();

        reporter.start_run();
        reporter.enter_section(section_name, source_location{.file_name = filename, .line = section_line});

        bs::vector<bs::string> messages{};
        // TODO: check if gcc bug https://gcc.gnu.org/bugzilla/show_bug.cgi?id=100495 has been fixed
        if (!std::is_constant_evaluated()) // This works around above bug
            messages.push_back(bs::string{assertion_message});
        reporter.log_assertion(assertion,
                               source_location{.file_name = filename, .line = assertion_line},
                               {},
                               messages,
                               assertion_result);

        reporter.leave_section();
        reporter.log_target(section_path{bs::string{section_name}});
        reporter.stop_run();
        reporter.leave_test_case();

        return reporter.cache();
    };

#define MAKE_TESTS(PREFIX)                                                                                             \
    PREFIX##REQUIRE(test().size() == 1);                                                                               \
    PREFIX##REQUIRE(test()[0].name == test_case_name);                                                                 \
    PREFIX##REQUIRE(test()[0].sloc.file_name == filename);                                                             \
    PREFIX##REQUIRE(test()[0].sloc.line == test_case_line);                                                            \
    PREFIX##REQUIRE(test()[0].tags.size() == 1);                                                                       \
    PREFIX##REQUIRE(test()[0].tags[0] == test_case_tag);                                                               \
    PREFIX##REQUIRE(test()[0].test_runs.size() == 2);                                                                  \
    PREFIX##REQUIRE(test()[0].test_runs[0].target == section_path{});                                                  \
    PREFIX##REQUIRE(test()[0].test_runs[0].assertions.size() == 0);                                                    \
    PREFIX##REQUIRE(test()[0].test_runs[0].sections.size() == 0);                                                      \
    PREFIX##REQUIRE(test()[0].test_runs[1].target == section_path{bs::string{section_name}});                          \
    PREFIX##REQUIRE(test()[0].test_runs[1].assertions.size() == 0);                                                    \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections.size() == 1);                                                      \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].name == section_name);                                          \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].sloc.file_name == filename);                                    \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].sloc.line == section_line);                                     \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].sections.size() == 0);                                          \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].assertions.size() == 1);                                        \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].assertions[0].text == assertion);                               \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].assertions[0].sloc.file_name == filename);                      \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].assertions[0].sloc.line == assertion_line);                     \
    PREFIX##REQUIRE(test()[0].test_runs[1].sections[0].assertions[0].result == assertion_result);

    MAKE_TESTS(STATIC_)
    MAKE_TESTS()

    // TODO: The following tests are only made at runtime due to gcc bug 100495, see above.
    REQUIRE(test()[0].test_runs[1].sections[0].assertions[0].messages.size() == 1);
    REQUIRE(test()[0].test_runs[1].sections[0].assertions[0].messages[0] == assertion_message);

#undef MAKE_TESTS
}