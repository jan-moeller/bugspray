# Captures

## CAPTURE(*expr* [, *expr*, ...])

Captures the value of the passed expressions and reports them to the current
reporter in case of a failed expression.

### Arguments

*expr*: Any number of comma separated expressions.

### Notes

* The reported value is that of the time of capture - not the time the
  assertion failed.
* Bugspray knows how to stringify the following categories of types:
    - `bool`, `std::bool_constant`
    - `char`, `wchar_t`, `char8_t`, `char16_t`, `char32_t`
    - Any type satisfying `std::ranges::forward_range`
    - Any type satisfying `std::floating_point` (runtime only)
    - Any type satisfying `std::integral`, `std::integral_constant`
    - Any type satisfying `std::is_pointer` (runtime only), `std::nullptr_t`
    - Strings (forward ranges with character types as value_type)

  Anything else will be printed as `{}`, unless the user provides the
  stringification capability themselves. For more info on this, see
  [Stringification](./stringification.md).