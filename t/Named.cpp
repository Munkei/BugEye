// Copyright Theo Willows 2013-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye.h>

NAMED_TEST(Named_Test, 1) {
  ok(true, "ok");
}

NAMED_TEST_NO_PLAN(Named_Test_2) {
  ok(true, "ok");
}
