// Copyright Theo Willows 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye.h>

NAMED_TEST_NO_PLAN(Exception) {
  throw std::runtime_error("Testing throwing an exception");
}
