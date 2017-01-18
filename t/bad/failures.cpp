// Copyright Theo Willows 2013–2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye3.h>

static auto t = bugeye::test("Failures").plan(5) = [] {
  OK(1 > 2, "is 1 > 2?");

  IS( (1 + 1), 3, "is (1 + 1) 3?");

  IS(std::string("foo"), "bar", "\"foo\" is \"bar\"");

  FAIL("fail");

  bugeye::subtest("subtest") = [] {
    OK(false);
    IS(1, 2);
  };
};
