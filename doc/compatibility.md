# Compatibility

Bugspray's API is very similar to
[Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/command-line.md)'s.
However, they are not the same, and never will. The reason for this is, that
Bugspray needs to account for the constraints of `constexpr` evaluation,
which Catch does not.

However, Bugspray is intentionally compatible with Catch2 where this is
reasonable. This should

1) simplify moving existing codebases between Catch2 and Bugspray and
2) allow using existing tooling written for Catch2, with Bugspray.

As an example for 2), Jetbrains CLion IDE comes with a GUI interface for
Catch2 tests. Bugspray can be used with that UI.