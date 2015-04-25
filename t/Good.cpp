// Copyright Theo Willows 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "Good.h"

Good::Good() {}

Good::~Good() {}

void Good::public_function(const char* x) {
  (void)x;
}

void Good::protected_function() {}

void Good::private_function() {}

#include <BugEye.h>

CLASS_TEST(Good, 11) {
  this->public_function("secret");
  this->protected_function();
  // this->private_function(); // Won't work

  ok(true, "ok (true)");

  is( (1 + 1), 2, "%d + %d == %d", 1, 1, 2);

  is("foo", "foo", "\"foo\" == \"foo\"");

  bool        b = ( ( (void*)"secret") > (void*)0);
  ok(b, "The secret stuff");
  const char* x = "I hope I don't find this in the wo_test exe.";
  diag(x);

  pass("pass");

  diag("Diagnostic message with numbers: %d (8), %d (10), %d (16)",
       010,
       10,
       0x10);

  todo("These aren't done yet") {
    ok(false, "false");

    fail("fail");
  }

  skip(true, 2, "skip these") {
    ok(false, "false");

    fail("fail");
  }

  skip(false, 2, "do *not* skip these") {
    ok(true, "false");

    pass("pass");
  }
}
