# Bugspray

![Bugspay logo](https://jan.fam-moe.de/bugspray_512.jpg)

[![CMake](https://github.com/jan-moeller/bugspray/actions/workflows/cmake.yml/badge.svg)](https://github.com/jan-moeller/bugspray/actions/workflows/cmake.yml)

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

## The API

Bugspray's API is going to look very familiar to developers
that have used Catch before. For example:

```c++
TEST_CASE("fixed_vector") { // Constexpr-enabled by default
    fixed_vector<int, 3> vector;
    REQUIRE(vector.empty());
}
EVAL_TEST_CASE("fixed_vector"); // Forces compile time evaluation
```

### Test Cases

The `TEST_CASE` macro introduces a test case and registers it
for runtime evaluation. In the simplest case it looks like
this:

```c++
TEST_CASE("some name") {}
```

Unfortunately, there does not seem to be a way to automatically
register a test case for constant evaluation. Therefore, the
`EVAL_TEST_CASE` macro has to be used to force compile time
evaluation.

#### Tags

Just like Catch, the `TEST_CASE` macro supports an optional
second argument: A list of tags. This list can be used to
specify which tests to run.

```c++
TEST_CASE("some name", "[tag1][tag2][...]") {}
```

Tags need to be enclosed in square brackets and are allowed
to contain any ascii character except square brackets. Invalid
tag strings are rejected at compilation time.

#### Evaluation Constraints

The `TEST_CASE` macro supports an optional third argument that
specifies when the test case can be run. It can be one of three
values:

- runtime: test case isn't constexpr-enabled. `EVAL_TEST_CASE`
  macro is not going to work for this test.
- compiletime: test case isn't runtime-enabled. It won't be
  registered for runtime evaluation.
- both: test case is both constexpr-enabled and runtime-enabled.

The default is both.

Example:

```c++
TEST_CASE("some name", "[tags]", runtime) {}
```

### Sections

Just like Catch, Bugspray supports (possibly nested) sections.
Sections are introduced with the `SECTION` macro:

```c++
SECTION("section name") {}
```

Sections never depend on sections of the same or deeper nesting
level. They will only be executed as part of their containing
section.

Example:

```c++
TEST_CASE("test") {
    int i = 0;
    SECTION("a") { ++i; }
    SECTION("b") {
        ++i;
        SECTION("c") {}
    }
}
```

In section `"a"`, `i` is incremented from `0` to `1`. In section
`"b"`, `i` is _also_ incremented from `0` to `1`, because it does
not depend on section `"a"`. Section `"c"` sees `i` as `1` because
it is part of section `"b"` and therefore sees all of its changes.

The `SECTION` macro supports an optional second argument. Just
like with test cases, you can specify an evaluation constraint:
It can be one of three values:

- runtime: the section is only executed at run time. It will be
  skipped during compile time evaluation.
- compiletime: the section is only executed at compile time. It
  will be skipped during run time evaluation.
- both: the section is executed during both compile time and
  run time evaluation.
- runtime_if({boolean-expression}): runtime, if the boolean
  expression evaluates to true, compiletime otherwise.
- compiletime_if({boolean-expression}): compiletime, if the
  boolean expression evaluates to true, runtime otherwise.
- only_if({boolean-expression}): both, if the boolean expression
  evaluates to true, disabled otherwise.
- only_runtime_if({boolean-expression}): runtime, if the boolean
  expression evaluates to true, disabled otherwise.
- only_compiletime_if({boolean-expression}): compiletime, if the
  boolean expression evaluates to true, disabled otherwise.
- disabled: Entirely disabled; this section never runs.

The default is both.

Example:

```c++
struct partially_constexpr_type {
    constexpr partially_constexpr_type();
    constexpr bool this_is_constexpr();
    bool this_isnt_constexpr();
};

TEST_CASE("partially_constexpr_type") {
    partially_constexpr_type t;
    SECTION("test the constexpr-enabled part of the API") {
        REQUIRE(t.this_is_constexpr());
    }
    SECTION("test the remaining API", runtime) {
        REQUIRE(t.this_isnt_constexpr());
    }
}
EVAL_TEST_CASE("partially_constexpr_type");
```

### Assertions

Bugspray supports the following assertion macros:

- `REQUIRE(condition)`: fails and aborts the test if `condition`
  is `false`.
- `CHECK(condition)`: fails and continues the test if `condition`
  is `false`.
- `FAIL()`: fails and aborts the test unconditionally.
- `FAIL_CHECK()`: fails and continues the test unconditionally.
- `SUCCEED()`: logs a successful assertion and continues.
- `REQUIRE_NOTHROW(expression)`: fails and aborts the test if
  expression throws.
- `CHECK_NOTHROW(expression)`: fails and continues the test if
  expression throws.
- `REQUIRE_THROWS(expression)`: fails and aborts the test if
  expression doesn't throw.
- `CHECK_THROWS(expression)`: fails and continues the test if
  expression doesn't throw.
- `REQUIRE_THROWS_AS(type, expression)`: fails and aborts the
  test if expression doesn't throw an expression of the given
  type.
- `CHECK_THROWS_AS(type, expression)`: fails and continues the test
  if expression doesn't throw an expression of the given type.

### Additional Diagnostic Output

Bugspray offers the `CAPTURE` macro to capture the value
of variables. If any assertion within the scope of the
capture fails, the value of the variable at the time of
capture will be reported.

## Error Reporting

### At Compilation Time

If a test fails during constant evaluation, Bugspray does
its best to provide you with as much information as possible.
In particular, the compilation output will contain:

- The failed test case.
- The failed assertion.
- The expansion of the variables used in the assertion.
- The values of other explicitly captured variables.

For example, given this test case:

```c++
TEST_CASE("This will fail")
{
    int other_var = 6;
    int variable  = 42;
    CAPTURE(other_var);
    REQUIRE(variable == 0);
}
EVAL_TEST_CASE("This will fail");
```

The compilation output is going to look something
like this (here from gcc-12):

```
In file included from bugspray/macro_interface/test_case_macros.hpp:28,
                 from bugspray/bugspray.hpp:31,
                 from test/compile-failure-tests/00-test_case_failure.cpp:24:
bugspray/test_evaluation/evaluate_test_case.hpp: In instantiation of ‘constexpr void bs::compile_time_error() [with structural_string<...auto...> ...Messages = {bs::structural_string<163>{"bugspray/test/compile-failure-tests/00-test_case_failure.cpp:31: REQUIRE(variable == 0) ### WITH EXPANSION: 42 == 0 ### WITH: other_var: 6"}}]’:
test/compile-failure-tests/00-test_case_failure.cpp:26:1:   required from here
test/compile-failure-tests/00-test_case_failure.cpp:33:1:   in ‘constexpr’ expansion of ‘bs::evaluate_compiletime_test<bs::structural_string<15>{"This will fail"}, 0>{}.bs::evaluate_compiletime_test<bs::structural_string<15>{"This will fail"}, 0>::operator()()’
bugspray/test_evaluation/evaluate_test_case.hpp:84:39: error: static assertion failed: Test evaluation failed.
   84 |     static_assert((sizeof...(Messages), false), "Test evaluation failed.");
      |                   ~~~~~~~~~~~~~~~~~~~~^~~~~~~~
bugspray/test_evaluation/evaluate_test_case.hpp:84:39: note: ‘((void)0, false)’ evaluates to false
In file included from bugspray/bugspray.hpp:31,
                 from test/compile-failure-tests/00-test_case_failure.cpp:24:
test/compile-failure-tests/00-test_case_failure.cpp:33:1: error: static assertion failed
   33 | EVAL_TEST_CASE("This will fail");
      | ^~~~~~~~~~~~~~
```

In a big compilation log, search for `bs::compile_time_error()`
to quickly find the test output.

### At Run Time

By default, all output will go to the local terminal.

## Limitations

TBD.