# BugEye #

**Copyright Theo Willows 2012**

**Distributed under the Boost Software License, Version 1.0.  (See accompanying
file `LICENSE_1_0.txt` or copy at http://www.boost.org/LICENSE_1_0.txt)**

## Description ##

This is BugEye, a minimal C++ unit testing framework.  It only provides three
things:

1.  A place to put test code, that does not interfere with the rest of your
    code.

2.  A way to make all the test code run and produce output.  The output follows
    the Test Anything Protocol (TAP) (pseudo-)standard.  If you're not familiar
    with the protocol, see the links at the end of this document.

2.  Functions to determine whether tests pass or fail.

Each block of test code is meant to be written for a specific class.  The tests
are run in a subclass member function environment, providing easy access (with
`this`) to member functions and variables, including protected ones.

For more information see
[BugEye's GitHub page](https://github.com/Munkei/BugEye).

## Usage ##

The idea is that all macros are defined to do absolutely nothing when not
compiling for testing.

### Functions and Macros ###

*   `TEST(class subject, block code)`

    > **!** Only effective in test mode, otherwise does nothing.

    *   `subject`

        The class to be tested.

        The macro can only be used *once for each subject class*, but that
        should not be a problem.

    *   `code`

        The test code.

        It is wise to keep the test code within curly braces.  This results in
        the funny-looking `})` syntax, but without the braces your code probably
        won't compile.

        Within the block, the `this` keyword is a pointer to an object of a
        class that is a subclass of the subject class.  The nature of that
        class, such as its name, is of little interest to the test author.  What
        is interesting is that protected members (and of course public ones) are
        available for testing.

        Private members of the subject class are *not* accessible.  See below
        for more information.

    This macro could appear anywhere in your code, but keeping the test code
    together with the class (at the bottom of the `.cpp` file) is a good idea.

*   `ok(bool expression [, const char* message])`

    *   `expression`

        The expression to be evaluated.  If the expression resolves to `true`
        the test passes, if it resolves to `false` the test fails.

    *   `message` (optional)

*    `pass([const char* message])`

    *   `message` (optional)

    Passes a single test.

*   `fail([const char* message])`

    *   `message` (optional)

    Fails a single test.

*   `todo_begin([const char* message])`

    *   `message` (optional)

        A message describing why the following tests are ‘todo tests’.

    Mark the following tests (until an appearance of `todo_end()`) as ‘todo
    tests’, i.e. tests that are not yet expected to pass.

    The reason tests are not expected to pass is usually that they test parts of
    your code that are not yet implemented.

*   `todo_end()`

    End a section of ‘todo tests’.

*   `bail_out([const char* message])`

    Stop testing by ‘bailing out’.

*   `BUGEYE_RUN`

    Runs all the tests, and makes the *current function* return the number of
    failed tests.

    In your program's `main()` function, use this macro like so:

    ```c++
    #include <BugEye.h>

    ...

    int main() {
      BUGEYE_RUN;

      // Non-test code goes here
      ...
    }
    ```

    > **♡** Note the semi-colon.

*   `BUGEYE_SET_OUT(FILE* stream)`

    > **!** Only effective in test mode, otherwise does nothing.

    *   `stream`

        Whereto BugEye will print the TAP output.  The default target is
        standard out (`stdout`), which should be fine in most cases.

        If you want the output to go into a file in, say, your continuous build
        environment, you should *consider* redirecting it externally, e.g. in
        your build script.  This way it is still possible to build and run the
        test executable and see the result in a terminal.

*   `BUGEYE_SET_VERBOSE(bool verbose)`

    > **!** Only effective in test mode, otherwise does nothing.

    *   `verbose`

        Whether BugEye should produce verbose output.

### Compiling and Running ###

To compile your program for testing, define `_TEST`.  For instance; using `g++`
this would be done with a `-D_TEST` parameter.  Other compilers will have
similar ways to do it.

When `_TEST` is defined, the `BUGEYE_RUN` macro will run all the tests and
*return* from `main()`.  This way the rest of your program will not run when it
has been compiled for testing.

BugEye will print out TAP for all your tests.  At the end of the TAP stream,
there will be the plan, and before that a diagnostic message containing a
summary of the test results.

When `_TEST` is *not* defined, the `BUGEYE_RUN` and `TEST` macros do nothing,
letting your program run as normal.  The test code is not even compiled, so that
your binaries will not be full of unused code.

> **♡** Remember to compile both testing *and* normal versions in your
> continuous or nightly builds to catch compilation errors in the test code.

### Example ###

#### In `Foo.h` ####

```c++
class Foo {

...

protected:

  int add(int a, int b);

};
```

As you can see, no test-specific code is needed in the header file.  We just
declare a function that we want to test.

#### In `Foo.cpp` ####

```c++
#include <BugEye.h>

...

int Foo::add(int a, int b) {
  return a + b;
}

TEST(Foo, {
  ok((this->add(1, 1) == 2), "Adding 1 and 1");
})
```

## Extended Usage ##

If there is code outside of the `TEST` macros that should only be compiled when
testing (e.g. for setting up test result logging), it can use the `_TEST`
definition like so:

```c++
#ifdef _TEST
 // Code for testing only
 ...
#endif
```

It is *not* necessary to wrap the `BUGEYE_*` and `TEST` macros, or the
`#include <BugEye.h>` statement, this way.

Conversely, if you have code that should *not* be compiled when testing simply
substitute `#ifndef` for `#ifdef`.

Be careful not to make test-compiled code differ too much from the
non-test-compiled code, so as not to have your unit tests test something other
than your program.

### Testing Private Members ###

Many developers (and other know-it-alls) say that if you find yourself about to
unit test private members (be they functions or variables), then the very
structure of your code is wrong.  They may be right, but sometimes the choice to
refactor is not up to you.

But if you do *have* to test private members, you have a few different options:

*   Use [friend classes](http://en.wikipedia.org/wiki/Friend_class).  You can
    wrap the friend class declaration and definition in `#ifdef _TEST`:s as
    described above.

*   Create a protected member function in the subject class, that can do all the
    testing, and then call that function from the test code.  Again, you can
    wrap the function definition and declaration in `#ifdef _TEST`:s as
    described above.

*   Hack BugEye itself.  Keep in mind, though, that whenever a new version of
    BugEye is available, and you wish to use it, you'll have to merge your
    modifications, which can be a bit of a hassle.

    > **♡** You'll probably want to extend the `BugEye::test` class and somehow
    > add an instance to `BugEye::all_tests`.

## Links ##

*   [http://testanything.org](TAP's homepage)

*   [https://en.wikipedia.org/wiki/Test_Anything_Protocol](The Wikipedia page on
    TAP)

*   [https://metacpan.org/module/Test::Simple](`Test::Simple`) and
    [https://metacpan.org/module/Test::More](`Test::More`) Perl packages (on
    [https://metacpan.org](MetaCPAN)).  Both packages have served as inspiration
    for, and helped in the development of, BugEye.
