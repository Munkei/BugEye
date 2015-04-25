// Copyright Theo Willows 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "GoodNoPlan.h"

GoodNoPlan::GoodNoPlan() {}

GoodNoPlan::~GoodNoPlan() {}

void GoodNoPlan::public_function(const char* x) {
  (void)x;
}

void GoodNoPlan::protected_function() {}

void GoodNoPlan::private_function() {}

#include <BugEye.h>

CLASS_TEST_NO_PLAN(GoodNoPlan) {
  this->public_function("secret");
  this->protected_function();
  // this->private_function(); // Won't work

  ok(true, "ok (true)");
}
