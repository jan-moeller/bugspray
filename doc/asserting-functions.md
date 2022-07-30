# Asserting Functions

Asserting functions are functions that are not test cases themselves, but
can be called from them and they can contain [assertions](./assertions.md)
and [sections](./sections.md). They can take any number of arguments, but
they can't return anything. For more info on why this is neccessary, see
[limitations](./limitations.md).

## ASSERTING_FUNCTION(*identifier* [, *parameters*, *qualifier*])

Defines an asserting function.

### Arguments

- *identifier*: Name of the function. Must be a valid c++ identifier.
- *parameters*: A parenthesized list of parameters. If none is specified,
  no parameters are assumed.
- *qualifier*: One of `runtime`, `compiletime`, `both`. If none is
  specified, `both` is assumed.

### Notes

This macro generates a regular function. Therefore, it is possible to
prefix it with a `template` introducer to generate a template. It is also
possible to use `requires` clauses. Marking an ASSERTING_FUNCTION
`noexcept` is considered undefined behavior.

### Examples

```c++
ASSERTING_FUNCTION(check_smth) {}
ASSERTING_FUNCTION(check_smth_else, (int i, std::string s)) {}
ASSERTING_FUNCTION(check_smth_at_runtime, (), runtime) {}
template<typename T> ASSERTING_FUNCTION(check_templated, (T foo), runtime) {}
```

## CALL(*identifier* [, *args*, ...])

Calls an asserting function with the given arguments.

### Examples

```c++
CALL(check_smth);
CALL(check_smth_else, 42, "foobar");
CALL(check_templated<int>, 42);
```