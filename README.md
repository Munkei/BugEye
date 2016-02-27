# BugEye ⦿ #

[![GitHub license](https://img.shields.io/github/license/Munkei/BugEye.svg?style=flat-square)](https://github.com/Munkei/BugEye/blob/master/LICENSE.md)
[![Travis](https://img.shields.io/travis/Munkei/BugEye.svg?style=flat-square)](https://travis-ci.org/Munkei/BugEye)
[![AppVeyor](https://img.shields.io/appveyor/ci/biffen/bugeye.svg?style=flat-square)](https://ci.appveyor.com/project/biffen/bugeye)

---

**Copyright Theo Willows 2013–2016**

**Distributed under the Boost Software License, Version 1.0.  (See accompanying
file `LICENSE.md` or copy at
[`http://www.boost.org/LICENSE_1_0.txt`](http://www.boost.org/LICENSE_1_0.txt))**

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

## Bugs ##

If you find a bug, please look for it among the [issues], and if it isn't
already reported, please report it.

## Links ##

*   [TAP's homepage]

*   [The Wikipedia page on TAP]

*   [prove], [`Test::Simple`] and [`Test::More`] (from the Perl world) have
    served as inspiration for BugEye.

---

  [`Test::More`]: https://metacpan.org/module/Test::More

  [`Test::Simple`]: https://metacpan.org/module/Test::Simple

  [BugEye's GitHub page]: https://github.com/Munkei/BugEye

  [issues]: https://github.com/Munkei/BugEye/issues

  [manual]: MANUAL.md

  [prove]: http://perldoc.perl.org/prove.html

  [TAP's homepage]: http://testanything.org

  [The Wikipedia page on TAP]: https://en.wikipedia.org/wiki/Test_Anything_Protocol
