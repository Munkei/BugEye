// Copyright Theo Willows 2013-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>

#include <BugEye3.h>

int main(int         argc,
         char const* argv[]) {
  bugeye::run(argc, argv);

  std::cout << "This is the code that runs when we're not testing" << std::endl;

  return 0;
}
