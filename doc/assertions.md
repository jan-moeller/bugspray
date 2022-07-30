# Assertions

Assertions are used to validate the unit under test behaves as expected.
They can be used inside [test cases](./test-cases.md),
[sections](./sections.md), and
[asserting functions](./asserting-functions.md).

## REQUIRE(*expr*)

Evaluates a unary or binary boolean expression. If the expression evaluates
to false, marks the test run as failed and aborts the run. Reports the
values of the expression to the current reporter.

### Arguments

*expr*: A side effect free unary or binary boolean expression.

### Notes

* Higher-order expressions can be reduced to binary by parenthesising them.
* If the expression contains parenthesized subexpressions, only the values
  of the first level of expressions will be reported.

### Examples

```c++
REQUIRE(b);
REQUIRE(i == 42);
REQUIRE(a && b);
REQUIRE((a && b) || c);
```

## REQUIRE_NOTHROW(*expr*)

Evaluates a potentially throwing expression. If the expression throws,
marks the test run as failed and aborts the run.

### Arguments

*expr*: A potentially throwing expression

### Notes

* During constexpr evaluation, evaluation of a throw expression always
  results in a compile error. Therefore, this macro only makes sense
  in a runtime context.

### Examples

```c++
REQUIRE_NOTHROW(foo());
```

## REQUIRE_THROWS(*expr*)

Evaluates a potentially throwing expression. If the expression doesn't
throw, marks the test run as failed and aborts the run.

### Arguments

*expr*: A potentially throwing expression

### Notes

* During constexpr evaluation, evaluation of a throw expression always
  results in a compile error. Therefore, this macro only makes sense
  in a runtime context.

### Examples

```c++
REQUIRE_THROWS(foo());
```

## REQUIRE_THROWS_AS(*type*, *expr*)

Evaluates a potentially throwing expression. If the expression doesn't
throw an object of the given type, marks the test run as failed and aborts
the run.

### Arguments

*expr*: A potentially throwing expression

### Notes

* During constexpr evaluation, evaluation of a throw expression always
  results in a compile error. Therefore, this macro only makes sense
  in a runtime context.

### Examples

```c++
REQUIRE_THROWS_AS(std::runtime_error, foo());
```

## CHECK(*expr*)

Evaluates a unary or binary boolean expression. If the expression evaluates
to false, marks the test run as failed and continues the run. Reports the
values of the expression to the current reporter.

### Arguments

*expr*: A side effect free unary or binary boolean expression.

### Notes

* Higher-order expressions can be reduced to binary by parenthesising them.
* If the expression contains parenthesized subexpressions, only the values
  of the first level of expressions will be reported.

### Examples

```c++
CHECK(b);
CHECK(i == 42);
CHECK(a && b);
CHECK((a && b) || c);
```

## CHECK_NOTHROW(*expr*)

Evaluates a potentially throwing expression. If the expression throws,
marks the test run as failed and continues the run.

### Arguments

*expr*: A potentially throwing expression

### Notes

* During constexpr evaluation, evaluation of a throw expression always
  results in a compile error. Therefore, this macro only makes sense
  in a runtime context.

### Examples

```c++
CHECK_NOTHROW(foo());
```

## CHECK_THROWS(*expr*)

Evaluates a potentially throwing expression. If the expression doesn't
throw, marks the test run as failed and aborts the run.

### Arguments

*expr*: A potentially throwing expression

### Notes

* During constexpr evaluation, evaluation of a throw expression always
  results in a compile error. Therefore, this macro only makes sense
  in a runtime context.

### Examples

```c++
CHECK_THROWS(foo());
```

## CHECK_THROWS_AS(*type*, *expr*)

Evaluates a potentially throwing expression. If the expression doesn't
throw an object of the given type, marks the test run as failed and
continues the run.

### Arguments

*expr*: A potentially throwing expression

### Notes

* During constexpr evaluation, evaluation of a throw expression always
  results in a compile error. Therefore, this macro only makes sense
  in a runtime context.

### Examples

```c++
CHECK_THROWS_AS(std::runtime_error, foo());
```

## FAIL()

Marks the test run as failed and aborts the run.

### Examples

```c++
FAIL();
```

## FAIL_CHECK()

Marks the test run as failed and continues the run.

### Examples

```c++
FAIL_CHECK();
```

## SUCCEED()

Logs a successful assertion and continues the run.

### Examples

```c++
SUCCEED();
```
