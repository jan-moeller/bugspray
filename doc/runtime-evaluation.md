# Runtime Evaluation

## Test Executable CLI

Test executables have a (currently limited) interface:

```
usage: ./executable [-h] [--version] [-r] [-o] [-d] [--order] [--rng-seed] test-spec

positional arguments:
 test-spec              specify which tests to run

options:
 -h, --help             show this help message and exit
 --version              show the bugspray version and exit
 -r, --reporter         select reporter from [console, xml]
 -o, --out              send all output to a file
 -d, --durations        specify whether durations are reported
 --order                specify order of test case execution from [decl, lex, rand]
```

This interface is compatible with
[Catch2](https://github.com/catchorg/Catch2/blob/devel/docs/command-line.md).
For more information about compatibility between Catch2 and Bugspray, see
[Compatibility](./compatibility.md).

### Specifying which tests to run

The *test-spec* is a string containing tags and/or test case names.
Wildcards `*` are supported at the beginning and end of a string. A
prefixed `exclude:` or `~` invert the term, meaning that anything *but*
matches are run. A series of tags means that all of them need to match.
A comma-separated list of tags means that any of the tags suffice.

Examples:

| *test-spec* | Description                                        |
|-------------|----------------------------------------------------|
| name        | Matches exactly the test case called "name"        |
| *foo*       | Matches anything with "foo" in the middle          |
| exclude:foo | Matches all tests except "foo"                     |
| ~foo*       | Matches all tests except those starting with "foo" |
| [foo][bar]  | Maches all tests tagged "foo" and "bar"            |
| [foo],[bar] | Matches all tests tagged "foo" or "bar"            |

## Reporters

By default, the console reporter is used. However, the `-r` parameter can
be used to select the xml reporter instead. Currently, neither of them take
additional arguments.

### Console

The console reporter prints test case failures to the command line.

For example (taken from the example-03):

```
-------------------------------------------------------------------------------
categorize_utf8_codeunit
-------------------------------------------------------------------------------
/projects/bugspray/test/failing-examples/03-utf8-codeunit-categorization/test.cpp:5d
...............................................................................

Before reaching target section
/projects/bugspray/test/failing-examples/03-utf8-codeunit-categorization/test.cpp:65: FAILED:
  CHECK(categorize_utf8_codeunit(0xF8u) == invalid)
WITH EXPANSION: 00001110 == invalid



===============================================================================
test cases: 1	| 0 passed	| 1 failed
assertions: 6	| 5 passed	| 1 failed

```

### XML

For compatibility reasons, the xml reporter currently identifies test runs
as `Catch2TestRun`.

```
<?xml version="1.0" encoding="UTF-8"?>
<Catch2TestRun name="my-test" rng-seed="1131124103">
  <TestCase name="categorize_utf8_codeunit" filename="/projects/bugspray/test/failing-examples/03-utf8-codeunit-categorization/test.cpp" line="93">encountered weird code unit: f8
    <OverallResult success="false"/>
  </TestCase>
  <OverallResults successes="5" failures="1" expectedFailures="0"/>
  <OverallResultsCases successes="0" failures="1" expectedFailures="0"/>
</Catch2TestRun>
```