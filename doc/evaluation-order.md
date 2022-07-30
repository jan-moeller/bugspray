# Evaluation Order

Test cases are generally executed exactly once for every leaf section. For
example:

```c++
TEST_CASE("3 times") { 
    SECTION("a") {
        SECTION("b") {}
        SECTION("c") {}
    }
    SECTION("d") {}
}
```

The above test case will be executed 3 times. The first run will execute
the path `a` -> `b` (skipping `c` and `d`), the second run `a` -> `c`
(skipping `b` and `d`), and the third just `d` (skipping all of `a`).

## Checking pre- and post-conditions

A natural way to check pre- and post-conditions is before and after the
sections they apply to. For example:

```c++
TEST_CASE("foo") {
    foo f;
    REQUIRE(precondition_is_met(f));
    SECTION("a") { REQUIRE(f.blarz()); }
    SECTION("b") { REQUIRE(f.blurp()); }
    REQUIRE(postcondition_is_met(f));
}
```

## Loops

Bugspray generally supports loops in its test cases, as long as they do not
break out of a code path early. This limitation is important, because
Bugspray needs to discover the sections contained in a test case at
runtime. Early breaks mess with this discovery.

For example:

```c++
TEST_CASE("bar") {
    for (int i = 0; i < 128; ++i) {
        SECTION("a") {
            REQUIRE(bar(i));
        }
    }
    SECTION("b") {}
}
```

In this example, the test case is called twice, once for `a` and once for
`b`. However, the *section* `a` is entered 128 times due to the loop.

## Creating Sections In a Loop

Bugspray contains a facility to parameterize (parts of) tests:
`bs::static_for_each_type`. It can be used to loop sections with multiple
types. For example:

```c++
TEST_CASE("vector") {
    bs::static_for_each_type<int, float>(
            [&]<typename T>() {
                DYNAMIC_SECTION("with " + bs::stringify_typename<T>()) {
                    vector<T> v;
                    REQUIRE(v.empty());
                }
            });
}
```

This is, essentially, a loop creating several sections - in this case two.
This can be used for type-parameterized tests. For more information, see
[Utilities](./utilities.md).