# Change Log for BugeEye #

---

**Copyright Theo Willows 2013–2015**

**Distributed under the Boost Software License, Version 1.0.  (See accompanying
file `LICENSE_1_0.txt` or copy at
[`http://www.boost.org/LICENSE_1_0.txt`](http://www.boost.org/LICENSE_1_0.txt))**

---

This is BugEye, a minimal, non-intrusive, C++ unit testing framework.

For the full documentation, see the README file or
[BugEye's GitHub page](https://github.com/Munkei/BugEye).

## v2.0.1 ##

* 🐛 Bailouts and uncaught exceptions sometimes weren't counted as failures,
  which could lead to false positives.  They will now *always* lead to   a
  non-zero exit.

* 🐛 ‘Consumed’ objects during stringification has been fixed.  Stringification
  now works somewhat differently.  It's dumber, but less dangerous.  Expect it
  to be better in a minor release in the near future.

* Messages for all tests are now optional (but a missing message *may* produce a
  compiler warning).

* 🏗 A lot of compiler warnings have been fixed.  (BugEye now builds with even
  stricter settings.)

## v2.0.0 ##

A lot of improvements, and some new features:

* Named tests, for test code not belonging to any particular class.

* Mark tests as "TODO" or "SKIP" (à la TAP).

* Better diagnostics for failing tests.

* Formatted test messages (à la `printf()`).

* Increased compatibility with different subject classes.

## v0.1.0 ##

Initial version.
