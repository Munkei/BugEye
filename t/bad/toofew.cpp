// Copyright Theo Willows 2013-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye3.h>

static auto t = bugeye::test("Too Few").plan(100) = [] {
  ok(true, "asdf");
};
