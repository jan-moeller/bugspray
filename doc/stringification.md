# Stringification

When it comes to stringifying values for the reporting of failed
[assertions](./assertions.md) or the [capture](./captures.md) macro,
Bugspray has a cascade of stringification functions it attempts to call.
Users can provide specialized stringification functions for their types,
and even override the stringification functions Bugspray already ships with.

The cascade for a given type `T` is as follows:

1. `to_string(bs::to_string_override_tag, T)`
2. `to_string(T)`
3. `to_string(bs::to_string_tag, T)`
4. `operator<<(std::stringstream&, T)`

Here, the 2. entry will generally find the Bugspray implementation, if there
is any for this type. However, other implementations might be found via ADL.

The recommended way to provide stringifcation for custom types is by
defining a (`constexpr`) `to_string()` function in the same namespace as
the type.