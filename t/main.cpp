// Copyright Theo Willows 2013-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye.h>

#include <cstdio>

int main() {
  BUGEYE_SET(verbosity, 2);
  BUGEYE_RUN;

  printf("This is the code that runs when we're not testing\n");
}
