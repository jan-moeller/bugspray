# Build Configuration

## BUGSPRAY_DONT_USE_STD_VECTOR

Can be set to `ON` to instruct Bugspray not to use std::vector. Instead,
Bugspray is going to use an internal `constexpr` vector implementation.
This is provided as a workaround for some standard library implementations
of `std::vector` not being fully `constexpr` yet.

## BUGSPRAY_DONT_USE_STD_STRING

Can be set to `ON` to instruct Bugspray not to use std::string. Instead,
Bugspray is going to use an internal `constexpr` string implementation.
This is provided as a workaround for some standard library implementations
of `std::string` not being fully `constexpr` yet.
