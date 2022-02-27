# Bugspray

Bugspray might become a modern, constexpr-friendly unit test framework
for C++20. At this stage, it is nothing more than a proof-of-concept.
Consider everything as "in flux".

## The Problem

With more code becoming constexpr-enabled, an interesting issue arises:
How do you make sure that your code works both at runtime and at
compiletime? The common strategies are:

* Ignore the issue and only test at runtime.
* Write all tests twice, once for runtime execution and once for
  compiletime execution.
* Wrap all your tests in constexpr functions and then call those
  twice: once at runtime and once at compiletime.

Unfortunately, current testing frameworks don't really help. For example,
imagine you're trying to test a fixed_vector with Catch2:

```c++
TEST_CASE("fixed_vector") {
    fixed_vector<int, 3> vector;
    REQUIRE(vector.empty());
    STATIC_REQUIRE(vector.empty()); // FAILS: vector is not allowed in a constexpr context.
}
```

Catch2's `STATIC_REQUIRE` doesn't really help you all that much, since it
only works with compiletime constants. What we really want, is to execute
the entire `TEST_CASE` at compiletime. Bugspray allows this.

## The Syntax

The syntax is inspired by Catch2, however only a small subset of its
functionality is supported at this point.

In Bugspray, the above problem is solved as follows:

```c++
TEST_CASE("fixed_vector") { // Constexpr-enabled by default
    fixed_vector<int, 3> vector;
    REQUIRE(vector.empty());
}
EVAL_TEST_CASE("fixed_vector"); // Forces compiletime evaluation
```

The test case "fixed_vector" is registered as a runtime test case, but
the `EVAL_TEST_CASE` macro also forces its evaluation in a constexpr
context. Therefore the test doesn't have to be duplicated.

Catch2-Style sections are also supported:

```c++
TEST_CASE("fixed_vector", "[TAG1]") {
    fixed_vector<int, 3> vector;
    SECTION("default constructor leaves it empty") {
        CHECK(vector.empty());
        CHECK(vector.size() == 0);
        CHECK(vector.begin() == vector.end());
    }
}
EVAL_TEST_CASE("fixed_vector");
```

This example is also using `CHECK` instead of `REQUIRE`, which continues
execution even on failure. The tags don't currently do anything, but
if you know Catch2 they might look familiar.

### Runtime-Only Test Cases

If a test shouldn't be run at compile time or can't be called from a
constexpr function, then the `TEST_CASE` macro can be called with an
additional argument:

```c++
TEST_CASE("Hello, World!", "[TAG1]", runtime) {
    // This isn't constexpr and will only be run at runtime
}
```

The same is true for `SECTION`s:

```c++
TEST_CASE("Hello, World!", "[TAG1]") {
    SECTION("both compiletime and runtime") {
    }
    SECTION("runtime-only", runtime) {
    }
}
EVAL_TEST_CASE("Hello, World!");
```

In this example, the `TEST_CASE` is run both at runtime and compiletime,
however the `"runtime-only"` section is run at runtime exclusively.
This is nice for sharing setup code, even if part of the API isn't
constexpr-enabled.

# Can This Be Integrated Into Catch2?
Unfortunately I don't see how. Even though the syntax looks the same,
Catch2's usage of global variables prevents execution in a constexpr
context. And changing this would be a breaking change. For example,
this is possible in Catch2, but it isn't (and can't be) in Bugspray:

```c++
void foo(int i) {
    REQUIRE(i == 0);
}
TEST_CASE("Hello, World!") {
    foo(0);
}
```