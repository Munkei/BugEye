// Copyright Theo Willows 2013–2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <BugEye3.h>

#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wunreachable-code"
#endif

int main(int         argc,
         char const* argv[]) {
  bugeye::run(argc, argv);

  // cppcheck-suppress unreachableCode
  std::cout << "This is the code that runs when we're not testing" << std::endl;

  return 0;
}
