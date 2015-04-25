// Copyright Theo Willows 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Failures.h"

Failures::Failures() {}

Failures::~Failures() {}

void Failures::public_function() {}

void Failures::protected_function() {}

void Failures::private_function() {}

#include <BugEye.h>

CLASS_TEST(Failures, 4) {
  this->public_function();
  this->protected_function();
  // this->private_function(); // Won't work

  ok(1 > 2, "1 > 2");

  is( (1 + 1), 3, "1 + 1 is 3");

  is("foo", "bar", "\"foo\" is \"bar\"");

  fail("fail");
}
