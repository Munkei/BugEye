// Copyright Theo Willows 2013-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye3.h>

static auto t = bugeye::test("Failures").plan(5) = [] {
  ok(1 > 2, "is 1 > 2?");

  is( (1 + 1), 3, "is (1 + 1) 3?");

  is(std::string("foo"), "bar", "\"foo\" is \"bar\"");

  fail("fail");

  bugeye::subtest("subtest") = [] {
    ok(false);
    is(1, 2);
  };
};
