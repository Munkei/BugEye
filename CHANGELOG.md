# BugeEye Change Log #

All notable changes to this project will be documented in this file. This
project adheres to [Semantic Versioning], and this file to [Keep a CHANGELOG].

## Unreleased ##

### Changed ###

*   FIXME

## [2.0.1] - 2015-05-23 ##

### Added ###

*   Messages for all tests are now optional (but a missing message *may* produce
    a compiler warning).

### Fixed ###

*   Bailouts and uncaught exceptions sometimes weren't counted as failures,
    which could lead to false positives. They will now *always* lead to a
    non-zero exit.

*   ‘Consumed’ objects during stringification has been fixed.  Stringification
    now works somewhat differently. It's dumber, but less dangerous. Expect it
    to be better in a minor release in the near future.

*   A lot of compiler warnings have been fixed.  (BugEye now builds with even
    stricter settings.)

## [2.0.0] - 2013-12-14 ##

### Added ###

*   Named tests, for test code not belonging to any particular class.
*   Mark tests as "TODO" or "SKIP" (à la TAP).
*   Better diagnostics for failing tests.
*   Formatted test messages (à la `printf()`).
*   Increased compatibility with different subject classes.

## [0.1.0] - 2012-10-02 ##

Initial version.

---

  [2.0.1]: https://github.com/Munkei/BugEye/compare/v2.0.0...v2.0.1

  [2.0.0]: https://github.com/Munkei/BugEye/compare/v0.1.0...v2.0.0

  [0.1.0]: https://github.com/Munkei/BugEye/compare/4f418d2...v0.1.0

  [Keep a CHANGELOG]: http://keepachangelog.com

  [Semantic Versioning]: http://semver.org