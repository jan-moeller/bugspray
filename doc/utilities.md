# Utilities

Bugspray comes with several useful utilities. All of these are located
in `namespace bs`.

## static_for

```c++
template<std::integral auto Start, std::integral auto End, typename Fn>
constexpr void static_for(Fn&& fn);
```

A compile-time for loop. Semantically more or less equivalent to
`for (auto i = Start; i != End; i = next(i)) { fn(i); }`, where `next(i)`
either increments or decrements by 1, depending on whether `End < Start` or
not. Unlike the above loop, `static_for`'s loop variable is usable in a
`constexpr` context.

### Arguments

+ *Start*: Initializer of the loop variable.
+ *End*: The end condition.
+ *fn*: A callable taking any std::integral_constant as a first argument.
  It must either return `void` or `bs::static_for_continuation`.
  If *fn* returns `bs::static_for_continuation`, then the loop will stop
  iterating when `bs::static_for_continuation::break` is returned.

### Example

```c++
bs::static_for_each<0, 16>([&](auto I) {
    std::array<int, I> arr;
});
```

## static_for_each_type

```c++
template<typename... Types, typename Fn>
constexpr void static_for_each_type(Fn&& fn);
```

A compile-time for loop which will be called once for every type in the
template argument list.

### Arguments

+ *Types*: Types to loop over
+ *fn*: A callable with a templated `operator()`, either taking a type or
  a type `T` and a integer `I` as template arguments. It will be called once for
  each type, and `I` is an increasing index into the type list.

### Example

```c++
bs::static_for_each_type<int, double, std::string>([&]<typename T, auto I>() {
    std::array<T, I> arr;
});
```