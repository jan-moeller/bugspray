# Bugspray

![Bugspay logo](https://jan.fam-moe.de/bugspray_512.png)

[![GCC](https://github.com/jan-moeller/bugspray/actions/workflows/gcc.yml/badge.svg)](https://github.com/jan-moeller/bugspray/actions/workflows/gcc.yml)

Bugspray is a C++20 unit testing framework. It's partially
[Catch2](https://github.com/catchorg/Catch2)-compatible
(and borrows much of its user-facing API from it), but,
unlike Catch, it allows executing tests both at run time
and at compile time.

## How Is It Useful?

In modern C++, more and more code is `constexpr`-enabled.
However, just labeling a function `constexpr` does not
guarantee that it can actually be called at compile time.
Generally, compilers will only let you know whether they
are able to constant evaluate a function when you force
them to do just that.

Traditionally, unit testing frameworks are concerned with
run time tests, and therefore they do not provide much
help here. C++ itself provides limited compile time
testing functionality via `static_assert`, however, beyond
testing simple expressions, this facility doesn't scale.

Another downside of `static_assert` is that it can _only_
test compile time execution. Most `constexpr`-enabled code,
though, is supposed to work both at run time and compile
time. The advent of `std::is_constant_evaluated()` makes
testing both ways even more necessary.

This is exactly the problem Bugspray attempts to solve:
It allows you to write tests once, and then execute them
both at run time and compile time. And it even gives you
useful output if a test fails at compile time.

## Example

```c++
constexpr auto fib(std::size_t n) noexcept -> std::size_t
{
    // BUG: n == 0 should result in 0, not 1
    if (n <= 2)
        return 1;
    return fib(n - 1) + fib(n - 2);
}

TEST_CASE("fib")
{
    std::array const expected_values{0, 1, 1, 2, 3, 5, 8};
    for (std::size_t i = 0; i < expected_values.size(); ++i)
    {
        CAPTURE(i);
        CHECK(fib(i) == expected_values[i]);
    }
}
EVAL_TEST_CASE("fib"); // Evaluates test case at compile time
```

The above code results in the following compile error
(line breaks added for easier readability):

```
In file included from bugspray/include/bugspray/macro_interface/test_case_macros.hpp:28,
                 from bugspray/include/bugspray/macro_interface/asserting_function_macros.hpp:29,
                 from bugspray/include/bugspray/bugspray.hpp:28,
                 from bugspray/test/failing-examples/01-fibonacci/test.cpp:24:
bugspray/include/bugspray/test_evaluation/evaluate_test_case.hpp:
    In instantiation of ‘constexpr void bs::compile_time_error() [with structural_string<...auto...> ...Messages = {bs::structural_string<276>{"
    FAILURE: bugspray/test/failing-examples/01-fibonacci/test.cpp:42:
    CHECK(fib(i) == expected_values[i]);
        WITH EXPANSION: 1 == 0;
        WITH: i: 0;
    IN: bugspray/test/failing-examples/01-fibonacci/test.cpp:36: fib"}}]’:
bugspray/test/failing-examples/01-fibonacci/test.cpp:36:1:   required from here
bugspray/test/failing-examples/01-fibonacci/test.cpp:45:1:   in ‘constexpr’ expansion of ‘bs::evaluate_compiletime_test<bs::structural_string<4>{"fib"}, 0>{}.bs::evaluate_compiletime_test<bs::structural_string<4>{"fib"}, 0>::operator()()’
bugspray/include/bugspray/test_evaluation/evaluate_test_case.hpp:92:39: error: static assertion failed: Test evaluation failed.
   92 |     static_assert((sizeof...(Messages), false), "Test evaluation failed.");
      |                   ~~~~~~~~~~~~~~~~~~~~^~~~~~~~
bugspray/include/bugspray/test_evaluation/evaluate_test_case.hpp:92:39: note: ‘((void)0, false)’ evaluates to false
In file included from bugspray/include/bugspray/macro_interface/asserting_function_macros.hpp:29,
                 from bugspray/include/bugspray/bugspray.hpp:28,
                 from bugspray/test/failing-examples/01-fibonacci/test.cpp:24:
bugspray/test/failing-examples/01-fibonacci/test.cpp:45:1: error: static assertion failed
   45 | EVAL_TEST_CASE("fib");
      | ^~~~~~~~~~~~~~
```

## Further Documentation

01. [Build Configuration](doc/build-configuration.md)
02. [Test Cases](doc/test-cases.md)
03. [Sections](doc/sections.md)
04. [Assertions](doc/assertions.md)
05. [Asserting Functions](doc/asserting-functions.md)
06. [Captures](doc/captures.md)
07. [Evaluation Order](doc/evaluation-order.md)
08. [Stringification](doc/stringification.md)
09. [Utilities](doc/utilities.md)
10. [Runtime Evaluation](doc/runtime-evaluation.md)
11. [Compatibility](doc/compatibility.md)
12. [Limitations](doc/limitations.md)
