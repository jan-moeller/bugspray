# Limitations

## Assertions in regular functions

Since Bugspray allows `constexpr` evaluation of most test cases, it needs
to implement all its functionality with the C++ subset available in
`constexpr` functions. In particular this means that it cannot make use of
mutable global state. Therefore, all information about test results has to
be passed in and out using arguments and return values. This, unfortunately,
means that the [assertion](./assertions.md) macros cannot be used in any odd
function, since they assume to be placed in a function that has certain
parameters. This is the reason why
[asserting functions](./asserting-functions.md) were introduced.

## Stringification of built-in types

In C++20, none of the standard library stringification functions are
`constexpr` enabled. While implementing this for integers is straight
forward (and Bugspray does that), implementing it for floating point types
is a much bigger headache. Therefore, Bugspray currently can't display
floating point types during constant evaluation.

## Compiletime failure reporting

Bugspray currently places its compiletime test results in the stack trace
of a function that will always fail to compile. The error report is in
the string-like template argument of the `bs::compile_time_error` function.
This comes with certain limitations:

- It is not possible to have line breaks. Instead, Bugspray currently uses
  the semicolon as a separator.
- There is a maximum message length. At the time of writing, Bugspray
  defines this limit to be 2048 characters. Anything beyond this will be
  truncated.

On the other hand, gcc formats the message in a very usable way (scroll right):

```
/proj/bugspray/include/bugspray/test_evaluation/evaluate_test_case.hpp: In instantiation of ‘constexpr void bs::compile_time_error() [with structural_string<...auto...> ...Messages = {bs::structural_string<277>{"FAILURE: /home/jan/CLionProjects/bugspray/test/failing-examples/00-simple/test.cpp:31: REQUIRE(variable == 0); WITH EXPANSION: 42 == 0; WITH: other_var: 6; BEFORE REACHING TARGET; IN: /home/jan/CLionProjects/bugspray/test/failing-examples/00-simple/test.cpp:26: This will fail"}}]’:
/proj/bugspray/test/failing-examples/00-simple/test.cpp:26:1:   required from here
/proj/bugspray/test/failing-examples/00-simple/test.cpp:33:1:   in ‘constexpr’ expansion of ‘bs::evaluate_compiletime_test<bs::structural_string<15>{"This will fail"}, 0>{}.bs::evaluate_compiletime_test<bs::structural_string<15>{"This will fail"}, 0>::operator()()’
/proj/bugspray/include/bugspray/test_evaluation/evaluate_test_case.hpp:92:39: error: static assertion failed: Test evaluation failed.
   92 |     static_assert((sizeof...(Messages), false), "Test evaluation failed.");
      |                   ~~~~~~~~~~~~~~~~~~~~^~~~~~~~
```

## Compiler support

Bugspray is tested against gcc-11 and gcc-12. Unfortunately, neither clang
nor msvc currently implement enough of C++20 (or the implementation is too
bug-ridden to be useful). I am re-evaluating this as soon as new versions
are published.