# BugEye ⦿ #

[![Build Status](https://travis-ci.org/Munkei/BugEye.svg?branch=master)](https://travis-ci.org/Munkei/BugEye)
[![Build status](https://ci.appveyor.com/api/projects/status/yv4glyv9s6evgxgr?svg=true)](https://ci.appveyor.com/project/biffen/bugeye)

---

**Copyright Theo Willows 2013–2014**

**Distributed under the Boost Software License, Version 1.0.  (See accompanying
file `LICENSE_1_0.txt` or copy at
[`http://www.boost.org/LICENSE_1_0.txt`](http://www.boost.org/LICENSE_1_0.txt))**

---

Documentation for BugEye v2.0.0.

## Description ##

This is BugEye, a minimal, non-intrusive, C++ unit testing framework.  It only
provides three things:

1. A place to put test code, that does not interfere with the rest of your
code.

2. A way to make all the test code run and produce output.

3. Functions to determine whether tests pass or fail.

Test code is put in blocks.  Each block will either belong to a class, or be
distinguished by its name.  The former have access to protected members of their
respective classes.

For more information see
[BugEye's GitHub page](https://github.com/Munkei/BugEye).

## Usage ##

### Acquiring BugEye ###

1.  Either download the tarball from
    [BugEye's GitHub page](https://github.com/Munkei/BugEye/releases), or clone
    the repository
    ([`https://github.com/Munkei/BugEye.git`](https://github.com/Munkei/BugEye.git)).

2. Put the directory (the cloned repository or the unpacked tarball) in a good
place and add it to your compiler's list of include directories.

3. Follow the documentation and examples below.

### Functions and Macros ###

The idea is that all macros are defined to do absolutely nothing when not
compiling for testing.

#### Where the Test Code Goes ####

```cpp
CLASS_TEST(class, plan) { ... }
CLASS_TEST_NO_PLAN(class) { ... }
NAMED_TEST(name, plan) { ... }
NAMED_TEST_NO_PLAN(name) { ... }
```

> **!** Only effective in test mode, otherwise they do nothing.

The `CLASS_TEST`:s are run in a subclass member function environment, providing
easy access (with `this`) to member functions and variables, including protected
ones.

* **`subject`**

  The class to be tested.

  The macro can only be used *once for each subject class*, but that should
  not be a problem.

* **`name`**

  Tests that do not belong to a certain class can be put into a "named test",
  and `name` will identify such a test.  Note that the name can not contain
  spaces or other "special" characters (in short, it has the same restrictions
  as class names do in C++), and that names have to be unique.

The code within the curly brackets is the test code.

Within a class test block, the `this` keyword is a pointer to an object of a
class that is a subclass of the subject class.  The nature of that class, such
as its name, is of little interest to the test author.  What is interesting is
that protected members (and of course public ones) are available for testing.

Private members of the subject class are *not* accessible.  See the FAQ below
for more information.

These macros could appear anywhere in your code, but keeping the class tests
together with their respective classes (at the bottom of each `.cpp` file) is a
good idea.

#### How To Test Things ####

To register single events as either passed or failed tests, use the functions
below in your test code.

Whenever a function takes a `format` parameter, it will be used together with
the remaining parameters as the message for the test.  See the documentation for
the standard `printf()` function for details on the formatting.

* **`ok(code expression, const char* format, ...)`**

  Tests whether `expression` resolves to `true`.  If it does, that's a passed
  test, otherwise it's a failed one.

* **`is(actual, expected, const char* format, ...)` and `isnt(actual,
  not_expected, const char* format, ...)`**

  Tests whether two values (`actual` and `expected`) are equal, or not equal,
  respectively.  Any types of values can be used, as long as they can be
  compared with `==` or `!=`, respectively.

* **`pass(const char* format, ...)`**

  Passes a single test.

* **`fail(const char* format, ...)`**

  Fails a single test.

* **`todo(const char* format, ...) { code }`**

  Marks all tests within the curly brackets as "todo tests".  When such tests
  fail they will not add to the total number of failed test.

  The reason tests are expected not to pass is usually that they test parts of
  your code that are not yet implemented.  The message will show up in the
  output for each test.

  Example:

  ```cpp
  CLASS_TEST(Foo, 2) {
    ok(thing(), "thing");

    todo("Not done yet") {
      ok(new_thing(), "new thing");
    }
  }
  ```

  The above will still run the two `ok`s (so the plan is indeed 2).  The
  latter one will fail and show up as `not ok`, but will not count as a
  failure in the total:

  ```
  Test for class: Foo ..
  1..2
  ok 1 - thing
  not ok 2 - new thing # TODO Not done yet
  ok
  Summary
       Groups run:      1
    Groups failed:      0
        Tests run:      2
     Tests failed:      0
  Result: PASS
  ```

* **`skip(bool condition, unsigned int howmany, const char* format, ...) { code }`**

  If `condition` evaluates to `true` the tests within the curly brackets are
  skipped (in fact, the whole block is skipped, not just tests).

  `howmany` should be the number of tests in the block, so that the plan will
  stay intact.  In the output the defined number of tests will show up as
  `ok`, with the supplied message.

  Useful for tests that that should only be run under certain conditions,   e.g.
  something platform specific or something that requires a certain
  environment.

  Example:

  ```cpp
  CLASS_TEST(Foo, 3) {
    ok(local(), "local");

    skip(!test_server_is_up(), 2, "No test server") {
      ok(remote("foo"), "remote with foo");
      ok(remote("bar"), "remote with bar");
    }
  }
  ```

  The above will produce three tests in the output, but if the test server is
  not up, the last two will always be just `ok`:

  ```tap
  Test for class: Foo ..
  1..3
  ok 1 - local
  ok 2 # SKIP No test server
  ok 3 # SKIP No test server
  ok
  ```

* **`bail_out(const char* format, ...)`**

  Stop testing by ‘bailing out’.

#### Running the Tests ####

```cpp
BUGEYE_RUN;
```

> **!** Only effective in test mode, otherwise does nothing.

Runs all the tests, and makes the *current function* return the number of failed
tests.

In your program's `main()` function, use this macro like so:

```cpp
#include <BugEye.h>

// ...

int main() {
  BUGEYE_RUN;

  // Non-test code goes here
  // ...
}
```

#### Configuration ####

```cpp
BUGEYE_SET(parameter, value);
```

> **!** Only effective in test mode, otherwise does nothing.

To set a configuration parameter, use the `BUGEYE_SET` macro somewhere before
the `BUGEYE_RUN` macro, e.g:

```cpp
int main() {
  BUGEYE_SET(verbosity, 2);
  BUGEYE_RUN;

  ...
}
```

Note that setting an unknown parameter, or a known parameter with the wrong type
of value, will *not* cause any error or feedback.  If you think a parameter
doesn't get set properly, make sure the spelling and the value type are correct.

##### Configuration Parameters #####

* **`out` (`FILE*`)**

  Whereto BugEye will print the output.  The default target is standard out
  (`stdout`), which should be fine in most cases.

  If you want the output to go into a file in, say, your continuous build
  environment, you should *consider* redirecting it externally, e.g. in your
  build script.  This way it is still possible to build and run the test
  executable and see the result in a terminal.

  Default:  `stdout`

* **`verbosity` (`int`)**

  How much BugEye should print.  A higher value means more output.

  Default:  `0`

* **`class_prefix` (`const char*`)**

  The string pre-pended to class names to make the names of their tests.

  Default:  `"Test for class: "`

### Compiling ###

To compile your program for testing, define `_TEST`.  For instance; using `g++`
this would be done with a `-D_TEST` parameter.  Other compilers will have
similar ways to do it.

When `_TEST` is defined, the `BUGEYE_RUN` macro will run all the tests and
*return* from `main()`.  This way the rest of your program will not run when it
has been compiled for testing.

BugEye will print out TAP for all your tests.  At the end of the TAP stream,
there will be the plan, and before that a diagnostic message containing a
summary of the test results.

When `_TEST` is *not* defined, the `BUGEYE_RUN` and `..._TEST` macros do
nothing, letting your program run as normal.

> **♥** Remember to compile both testing *and* normal versions in your
continuous or nightly builds to catch compilation errors in the test code.

BugEye requires some C++11 features, so the compiler will have to support it.
In the case of GCC that means version 4.7 or later, and the `-std=c++11` option.

BugEye is tested to compile with GCC's `-Wall -Wextra -Werror` options, i.e. you
shouldn't be seeing any warnings from the BugEye code.  If you do, please file a
bug (see "Bugs" below).

### Running ###

Simply execute your binary and see the results of all the tests.

The output is meant to look like the Test Anything Protocol (TAP)
(pseudo-)standard, but grouped by the tests.  It will not *strictly* be TAP,
since TAP has not (yet) defined a standard for test groups.

If you're not familiar with the TAP protocol, see the links at the end of this
document.

#### Exit Code ####

* **0**

  All test were run and they all passed.

* **1–124**

  Tests were run, but some failed.  The number of failed tests is the same as
  the exit code.

* **125**

  Tests were run, but 125 *or more* of them failed.

* **126**

  An internal error occurred.  More details should have been printed to
  STDERR.

* **Other values**

  Something out of the control of BugEye.

### Example ###

#### In `Foo.h` ####

```cpp
class Foo {

  ...

  protected:

    int add(int a, int b);

};
```

As you can see, no test-specific code is needed in the header file.  We just
declare a function that we want to test.

#### In `Foo.cpp` ####

```cpp
#include <Foo.h>

...

int Foo::add(int a, int b) {
  return a + b;
}

#include <BugEye.h>

CLASS_TEST(Foo, 1) {
  is(this->add(1, 1), 2, "Adding 1 and 1");
}
```

In short, at the end of the file (or wherever you prefer) include `BugEye.h` and
use one of the `..._TEST` macros.  Within its block, write your test code, using
the testing functions (`ok`, `is`, etc.).

You probably usually include files at the top of each file, but since the BugEye
specifics are only needed in the test code, it is enough to include it like in
the example above.  This goes well with the philosophy of keeping test code out
of the way of "real" code.  It also avoids clashes with BugEye macros, although
they *should* be rare (see the FAQ).

## FAQ ##

### Won't all the macros clash with my code? ###

Hopefully not.  As much as possible of the BugEye code is put in the `BugEye`
namespace.  Preprocessor macros will use variables and such prefixed with
`_bugeye_`, and the same goes for any public and protected functions that the
test classes inherit.  What's left are the macros themselves:  `ok`, `is`,
`pass`, etc.  These *you* will have to avoid.  Sorry.

In short:  As long as your project isn't also called "BugEye", you should be
safe.

### Where does the test code go when not compiling with `_TEST`? ###

The code is compiled (as you may have noticed), which saves you from making
errors in the test code that go unnoticed until you attempt to test your code.
(You can decide for yourself whether this is a feature or a flaw.)

However, the code is put into a function of an unused class (its name will begin
with `_BUGEYE_UNUSED_`, so it shouldn't clash with the rest of your code), and
that class and the function are templated, but not used or specialised, so your
compiler *should* optimise them all away (this has been confirmed with GCC 4.8,
even *without* any `-O` option).

### How do I test private members? ###

Many developers (and other know-it-alls) say that if you find yourself about to
unit test private members (be they functions or variables), then the very
structure of your code is wrong.  They may be right, but sometimes the choice to
refactor is not up to you.

But if you do *have* to test private members, you have a few different options:

* Use [friend classes](http://en.wikipedia.org/wiki/Friend_class).  You can wrap
  the friend class declaration and definition in `#ifdef _TEST`:s as described
  above.

* Create a protected member function in the subject class, that can do all the
  testing, and then call that function from the test code.  Again, you can wrap
  the function definition and declaration in `#ifdef _TEST`:s as described
  above.

* "Hack" BugEye itself.  Keep in mind, though, that whenever a new version of
  BugEye is available, and you wish to use it, you'll have to merge your
  modifications, which can be a bit of a hassle.

  > **♥** You'll probably want to extend the `BugEye::TestGroup` class and
  somehow add an instance to the list of groups (using
  `BugEye::TestHarness::get().add_group()`).

### Are there any examples available? ###

Apart from the snippets of code in this document, there are the files used for
the internal testing of BugEye.  They can be found in the repository, under
`t/src/`.

## Restrictions ##

* The names of named tests have some limitations.  This *may* be changed in
  later versions.

* Class tests for classes without a default constructor (or an inaccessible one)
  will *probably* not compile, and most likely the test code will not work.

## Future Features ##

Some things that *may* be implemented in future versions:

* [ ] The ability to specify which test groups to run (or not run), possibly by
  handing the `main()` parameters to `BUGEYE_RUN` and use good old argument
  parsing.

  Also, a way to list available test groups.

* More test helpers:

  * [ ] `isa(object, class)`

  * [ ] `no_throw(code)`

  * [ ] `throws(code, typename)`

* [ ] Put test results in one file per test group (a.k.a. archiving).

* [ ] Grouped/categorised test groups, so that one can run one or more groups of
  test groups.

* [ ] Coloured output.

If you have an idea for a new feature, please register a feature request at
[the GitHub page](https://github.com/Munkei/BugEye/issues).  Or, if you're up
for it, implement the feature and send a pull request.

## Bugs ##

If you find a bug, please look for it at
[the GitHub page](https://github.com/Munkei/BugEye/issues), and if it isn't
already reported, please report it.

## Extended Usage ##

If there is code outside of the `..._TEST` macros that should only be compiled
when testing (e.g. for setting up test result logging), it can use the `_TEST`
definition like so:

```cpp
#ifdef _TEST
// Code for testing only
#endif
```

It is *not* necessary to wrap the `BUGEYE_*` and `..._TEST` macros, or the
`#include <BugEye.h>` statement, this way.

Conversely, if you have code that should *not* be compiled when testing simply
substitute `#ifndef` for `#ifdef`.

Be careful not to make test-compiled code differ too much from the
non-test-compiled code, so as not to have your unit tests test something other
than your program.

## Links ##

* [TAP's homepage](http://testanything.org)

* [The Wikipedia page on TAP](https://en.wikipedia.org/wiki/Test_Anything_Protocol)

* [prove](http://perldoc.perl.org/prove.html),
[`Test::Simple`](https://metacpan.org/module/Test::Simple) and
[`Test::More`](https://metacpan.org/module/Test::More) (from the Perl world)
have served as inspiration for BugEye.
