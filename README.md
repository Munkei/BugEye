# `twt` #

> **WARNING!  This software comes with no warranty, no guarantees, and no
> promises.  It may cause unexpected damage to your system, including, but not
> limited to, loss and/or corruption of data.**

> All warnings aside, it is a simple bit of code, and if you have some knowledge
> of C++ programming and meta-programming you should have no problem
> understanding the source code, and thus rid yourself of any concerns.

## Description ##

This is `twt`, a minimal C++ unit testing framework.  It only provides a place
to put test code, and a way to make that code run.

Each test (i.e. block of test code) is meant to be written for a specific class.
The test is run in a subclass member function environment, providing easy access
(with `this`) to member functions and variables, including protected ones.

For more information see [`twt`'s GitHub page](https://github.com/Munkei/twt).

## License ##

See LICENSE.md

## Example Usage ##

### In `Foo.h` ###

```c++
class Foo {

...

protected:

  int add(int a, int b);

};
```

As you can see, no test-specific code is needed in the header file.  We just
declare a function that we want to test.

### In `Foo.cpp` ###

```c++
#include <twt.h>
#include <assert.h>

...

int Foo::add(int a, int b) {
  return a + b;
}

TEST(Foo, {
  assert(this->add(1, 1) == 2);
})
```

#### The `TEST` Macro ####

The test code all resides within the `TEST` macro.  This macro could appear
anywhere in your code, but keeping the test code together with the class (at the
bottom of the `.cpp` file) is a good idea.

The first parameter to the macro is the class to be tested (henceforth the
"subject class"), the second parameter is the test code.

A few things to note about the `TEST` macro:

* The macro can only be used once for each class, but that should not be a
  problem.

* It is wise to keep the test code within curly braces.  This results in the
  funny-looking `})` syntax.

* `twt` does *not* provide any assertion functions or the like.  You can use
  `assert()`, but you might want to use something that produces some kind of
  output, even for successful tests.

* Within the block, the `this` keyword is a pointer to an object of a class that
  is a subclass of the subject class.  The nature of that class, sush as its
  name, is of little interest to the test author.  What is interesting is that
  protected members (and of course public ones) are available for testing.

* Private members of the subject class are not accessible, but many say that if
  you need to unit test private members, then you should consider restructuring
  your code.

### Compiling and Running ###

In your program's `main()` function, use the macro `TWT_RUN` like so:

```c++
#include <twt.h>

...

int main() {
  TWT_RUN;

  // Non-test code goes here
  ...
}
```

Note the semi-colon.

To compile your program for testing, define `_TEST`.  For instance; using `g++`
this would be done with a `-D_TEST` parameter.  Other compilers will have
similar ways to do it.

When `_TEST` is defined, the `TWT_RUN` macro will run all the tests and *return*
from `main()`.  This way the rest of your program will not run when it has been
compiled for testing.

When `_TEST` is *not* defined, the `TWT_RUN` and `TEST` macros do nothing,
letting your program run as normal.  The test code is not even compiled, so that
your binaries will not be full of unused code.  (Remember to compile testing and
normal versions in your continuous or nightly builds to catch compilation errors
in the test code.)

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

It is *not* necessary to wrap the `TWT_RUN` and `TEST` macros, or the
`#include <twt.h>` statement, this way.

Conversely, if you have code that should *not* be compiled when testing simply
substitute `#ifndef` for `#ifdef`.

Be careful not to make test-compiled code differ too much from the
non-test-compiled code, so as not to have your unit tests test something other
than your program.

### Testing Private Members ###

If you do *have* to test private members, read up on `friend` classes.  You can
wrap such code in `#ifdef`:s as described above.
