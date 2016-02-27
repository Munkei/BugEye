# BugEye Manual #

**Copyright Theo Willows 2016**

This document describes how to *use* the BugEye C++ unit testing framework. If
you're looking for information on how to get it, how it is licensed, etc. please
see [`README.md`] or [BugEye's GitHub page].

## Introduction ##

The general concept is that testing code is written within [Tests], and that an
[Entry Point] is called at runtime to run all (or a selection) of those tests.

The code can the be compiled (see ‘[Compiling]’) with or without the tests, thus
allowing the same source code to be used for production binaries and unit tests.

Everything you need is in the `BugEye3.h` file. It's a good idea to include it
as late as possible, e.g. just above a test, since it contains some preprocessor
macros.

Below is the documentation for the BugEye API. Any part of BugEye not documented
here is not part of the API, and can change without prior notice in future
versions. Don't use it, or do so at your own risk.

## Tests, Sub-Tests and Assertions ##

All test code resides within [Tests], which can contain [Sub-Tests], to better
organise tests and their results.

### Tests ###

A test is created using the `bugeye::test` type and assigning a lambda to it,
like so:

```cpp
#include <bugeye/BugEye3.h>

static auto test = bugeye::test("Test name") = [] {
  // Test code
}
```

The one parameter to `bugeye::test` is the name of the test. It will be used for
printing the result and for identifying the test when [Running].

The name of the variable does not affect the test, so it could be anything you
like. `static` is optional but recommended, as it allows the same variable name
to be used in different compilation units.

Optionally, you can specify a plan for a test. A plan is simply the number of
[Assertions] and [Sub-Tests] that you plan to run in a test. Should the actual
number not be the same as the plan, the test will fail. It's entirely optional,
but can be a good way of keeping track of your test code. To specify a plan, use
`.plan(N)`:

```cpp
static auto test = bugeye::test("Test with a plan").plan(42) = [] {
  // Test code with 42 assertions and sub-tests
}
```

### Sub-Tests ###

To organise your test code, you can divide [Tests] into sub-tests. Sub-tests
have their own names, and will show up indented in the [Output], and are great
for things that ‘go together’.

To make a sub-test, use `bugeye::subtest`, much like you use `bugeye::test`,
within a [Test] (or sub-test(!)):

```cpp
static auto test = bugeye::test("Test name") = [] {
  // Test code

  bugeye::subtest("subtest") = [] {
    // Sub-test code
  }

  // More test code
}
```

You can nestle sub-tests to your heart's content. The only limit is the maximum
value of `size_t` and your available memory.

Sub-tests can have their own plans. Simply add `.plan(N)`, just like [Tests].

Note that a sub-test counts as *one* item in it's parent's plan, regardless of
how many assertions and sub-tests *it* contains.

### Assertions ###

Finally, we're getting to the actual testing!

There are a number of assertions available, all described below. Note that they
are preprocessor macros (so that their location can be printed later).

All assertions have an optional format string parameter with a list of
arguments. This is the message that shows up in the [Output] together with the
result of the test. The format and it's arguments work like the `printf()`
family of functions. See the [`std::printf` documentation] for details.

#### `ok(expression[, format, args...])` ####

Checks that `expression` is true.

#### `is(got, expected[, format, args...])` ####

Checks that `got` and `expected` are equal (using `==`). If they are not, their
values will show up in the [Output] for you to inspect (using magic
stringification techniques).

#### `isnt(got, unexpected[, format, args...])` ####

The opposite of `is()`.

#### `pass([format, args...])` ####

Simply create a good assertion.

#### `fail([format, args...])` ####

Simply create a bad assertion. Great for use in a `catch` block.

### Other Commands ###

These are commands that control test results and [Output], but aren't really
assertions.

#### `diag(format, args...)` ####

Print (log) a diagnostic message. Doesn't affect test results.

#### `skip(condition, count, format, args...) { ... }` ####

Sometimes, you might want to skip some assertions. Perhaps under certain
circumstances, such as a missing resource.

If `condition` is true, then the whole block will be skipped, i.e. not executed
at all. `count` is the number of [Assertions] (and [Sub-tests]) that are being
skipped. The [Output] will still contain the skipped tests, but they will all be
considered successful, and be marked with `skip`.

`format` and its `args...` are for describing the reason of skipping.

```cpp
auto file = get_test_data();

skip(!file.exists(), 42, "No test data (%s)", file.name.c_str() ) {
  // Test code that may not run
}
```

#### `todo(format, args...) { ... }` ####

A bit like `skip(){}`, but the contents will *always* be executed. Failed
[Assertions] will still appear as failures in the [Output], but won't be counted
in the total number of failures, and won't make their parent fail.

Great for when you are testing things that are not yet complete, but you still
want to have the tests in place early.

```cpp
todo("The thingamajig is still under development") {
  // Tests for the thingamajig
}
```

`format` and its `args...` are for describing the reason the contained block is
not yet done.

#### `bail_out(format, args...)` ####

Sometimes all we can do is give up. `bail_out()` will stop all testing
immediately (no `return` necessary) and fail the whole process.

```cpp
auto our_library = dlopen("our_library.so", RTLD_LAZY);

if (our_library == nullptr) {
  bail_out("Failed to load our library! Can't test anything now!");
}
```

## Entry Point ##

Once you've written [Tests] you'll want a way to run them. It all starts with
`bugeye::run()`. Put it at the top of your `main()`:

```cpp
#include <BugEye3.h>

int main(int         argc,
         char const* argv[]) {
  bugeye::run(argc, argv);

  // Code that runs when not testing
}
```

Passing `argc` and `argv` is optional, but necessary if you want to use [Command
Line Arguments].

When [Compiling] for testing, `bugeye::run()` will exit (`std::exit()`), so
anything after it will *not* be executed.

When *not* [Compiling] for testing, `bugeye::run()` is a no-op, and the rest of
your code will run as normal.

## Compiling ##

You can compile your code with or without [Tests]: If `_TEST` is defined, then
`bugeye::run()` will run the tests. If it is not defined, then `bugeye::run()`
won't do anything, and all the test code goes unused (and your compiler should
optimise it away).

There are two things worth noting: Firstly, there is no need to wrap test code
in `#ifdef _TEST`. The BugEye API will take care of all of that for you. You
*can* still wrap things, but beware of making your test code diverge too much
from your production code.

Secondly, even if test code goes unused without `_TEST`, it still needs to be
valid code. It does get compiled, just not used anywhere. Like it or not, at
least it has the benefit of catching errors in test code early.

You will probably set up your build system to produce different binaries for
tests and ‘ordinary’ code. How to do this won't be described here, since it
depends on the build system used.

## Running ##

In its simplest form, running is as easy as executing the test-compiled binary
in a terminal and watching the output. However, you can control what gets run
with arguments.

### Command Line Arguments ###

BugEye understands a number of options, listed below. Short options begin with
`-` and long ones with `--`. Short options can be grouped.

Arguments that aren't options, or their values, are names of tests to be run.
E.g:

```sh
test-binary foo bar
```

Will run the tests named ‘foo’ and ‘bar’ only.

#### `--list`, `-l` ####

Instead of *running* any tests, just print the names of all tests, one per line,
to STDOUT.

#### `--regex PATTERN`, `-r PATTERN` ####

Run tests that match `PATTERN`. Can be repeated with different patterns, and
combined with naming tests.

Note that the pattern is tested against *the whole* name.

Note also that the regex implementation is the one supplied by the C++ standard
library you use. Some (older) implementations are known to have poor regex
support.

#### `--case-insensitive`, `-c` ####

When naming tests, or using `regex`, don't care about case.

#### `--ignore-missing`, `-I` ####

Normally when a test is named explicitly and no test with that name exists, or
when `--regex` is used but doesn't match any tests, it is considered an error
and the whole process is aborted (without running *any* tests).

Using this option makes BugEye continue and run whatever tests *did* match, or
even none at all, without considering it an error.

### Output ###

The output of the tests is printed to STDOUT. This includes test failures, etc.
Errors on the other hand, are printed to STDERR.

The test results are printed in [TAP] format. [Tests] and [Sub-Tests] are
indented. This is not part of the official TAP specification, but a common
practice.

Test results are given colour (using ANSI escape notation); green for good and
red for bad.

### Exit Code ###

The exit code of the execution reflects the result of the testing:

*   **0** means everything went well: All tests passed and there were no errors.

*   **3** means one or more tests failed.

*   **2** means something went wrong, e.g. invalid command line arguments were
    supplied, or something internally to BugEye failed. STDERR output should
    provide more information.

Any other exit code means something beyond the control of BugEye failed.

---

  [Assertions]: #assertions

  [BugEye's GitHub page]: https://github.com/Munkei/BugEye

  [Command Line Arguments]: #command-line-arguments

  [Compiling]: #compiling

  [Entry Point]: #entry-point

  [Output]: #output

  [`std::printf` documentation]: http://en.cppreference.com/w/cpp/io/c/fprintf

  [`README.md`]: ./README.md

  [Running]: #running

  [Sub-Tests]: #sub-tests

  [TAP]: http://testanything.org

  [Test]: #tests

  [Tests]: #tests
