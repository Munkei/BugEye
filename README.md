# BugEye ⦿ #

[![GitHub license](https://img.shields.io/github/license/Munkei/BugEye.svg?style=flat-square)](https://github.com/Munkei/BugEye/blob/master/LICENSE.md)
[![Travis](https://img.shields.io/travis/Munkei/BugEye.svg?style=flat-square)](https://travis-ci.org/Munkei/BugEye)
[![AppVeyor](https://img.shields.io/appveyor/ci/biffen/bugeye.svg?style=flat-square)](https://ci.appveyor.com/project/biffen/bugeye)

---

Copyright © Theo Willows 2013-2017

Distributed under the Boost Software License, Version 1.0. (See accompanying
file `LICENSE.md` or copy at
[`http://www.boost.org/LICENSE_1_0.txt`](http://www.boost.org/LICENSE_1_0.txt))

---

## Description ##

This is BugEye, a minimal, non-intrusive, C++ unit testing framework.  It only
provides three things:

1.  A place to put test code, that does not interfere with the rest of your
    code.

2.  A way to make all the test code run and produce output.

3.  Functions to determine whether tests pass or fail.

For more information see [BugEye's GitHub page].

## Usage ##

### Acquiring BugEye ###

1.  Either download the tarball from
    <https://github.com/Munkei/BugEye/releases>, or clone the repository
    (`https://github.com/Munkei/BugEye.git`).

2.  Put the directory (the cloned repository or the unpacked tarball) in a good
    place and add it to your compiler's list of include directories.

3.  Follow the documentation and examples in the [Manual].

### Requirements ###

BugEye is supposed to strictly follow the C++11 standard. So a compiler that
supports C++11 is all you should need.

It is continuously tested (big thanks to [Travis CI] and [AppVeyor]) with various
versions of the following compilers:

*   [Clang]
*   [GCC]
*   [Microsoft Visual C++]

If you use some other compiler and find that BugEye doesn't work with it, please
report it as a bug (see below).

## Bugs ##

If you find a bug, please look for it among the [issues], and if it isn't
already reported, please report it.

## Links ##

*   [TAP's homepage]

*   [The Wikipedia page on TAP]

*   [prove], [`Test::Simple`] and [`Test::More`] from the Perl world have served
    as inspiration for BugEye.

---

  [AppVeyor]: https://ci.appveyor.com/project/biffen/bugeye

  [Clang]: https://clang.llvm.org

  [GCC]: https://gcc.gnu.org/

  [BugEye's GitHub page]: https://github.com/Munkei/BugEye

  [issues]: https://github.com/Munkei/BugEye/issues

  [manual]: MANUAL.md

  [Microsoft Visual C++]: https://docs.microsoft.com/en-gb/cpp

  [prove]: http://perldoc.perl.org/prove.html

  [TAP's homepage]: https://testanything.org

  [`Test::More`]: https://metacpan.org/module/Test::More

  [`Test::Simple`]: https://metacpan.org/module/Test::Simple

  [The Wikipedia page on TAP]: https://en.wikipedia.org/wiki/Test_Anything_Protocol

  [Travis CI]: https://travis-ci.org/Munkei/BugEye

<!--
LocalWords:  AppVeyor BugEye BugEye's TAP's md
-->
