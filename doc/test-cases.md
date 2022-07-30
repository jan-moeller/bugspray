# Test Cases

## TEST_CASE(*name* [, *tags*, *qualifier*])

Introduces a test case and registers it for runtime evaluation. Depending on
the qualifiers, the test case might also be executable at compile time.

Test cases must not contain `return` statements. They can contain loops,
however up to the constraints described in
[Evaluation Order](./evaluation-order.md).

### Arguments

- *name*: A string literal describing the test case. Must be globally
  unique.
- *tags*: A string literal containing a list of square-bracket-enclosed
  tags. An invalid tag string will result in a compilation error. If no
  tags are specified, an empty list of tags is assumed.
- *qualifier*: Specifies whether the test case is meant for runtime,
  compile-time or both. Must be exactly one of [*runtime*, *compiletime*,
  *both*]. If no qualifier is specified, *both* is assumed.

### Examples

```c++
TEST_CASE("foo must be barking"){}
TEST_CASE("vector", "[container][template]"){}
TEST_CASE("this is runtime only", "", runtime){}
TEST_CASE("this is compiletime only", "", compiletime){}
```

## BUGSPRAY_EVAL_TEST_CASE(*name*)

Evaluates a previously defined *compiletime* or *both*-qualified test case
in a `constexpr` context.

### Arguments

*name*: String literal of previously defined test case.

### Examples

```c++
TEST_CASE("foo must be barking"){}
BUGSPRAY_EVAL_TEST_CASE("foo must be barking");
```