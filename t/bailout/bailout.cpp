// Copyright Theo Willows 2013–2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye3.h>

static auto t = bugeye::test("BailOut") = [] {
  bail_out("Testing bailing out");
};
