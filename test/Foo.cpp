
//          Copyright Theo Willows 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include "Foo.h"

#include <BugEye.h>
#include <assert.h>

#include <stdio.h>
#include <vector>

Foo::Foo() {
}

Foo::~Foo() {
}

void Foo::public_function() {
}

void Foo::protected_function() {
}

void Foo::private_function() {
}

int main() {
  BUGEYE_SET_OUT(stdout);
  BUGEYE_VERBOSE(true);
  BUGEYE_RUN;

  printf("This is the code that runs when we're not testing\n");
}

TEST(Foo,
     {
       this->public_function();
       this->protected_function();
       //this->private_function(); // Won't work

       ok(true,  "ok (true)");

       ok(false, "ok (false)");

       pass("pass");

       fail("fail");

       diag("diag");

       note("note");

       todo_begin("todo");

       ok(true,  "ok (true) (todo)");

       ok(false, "ok (false) (todo)");

       pass("pass (todo)");

       fail("fail (todo)");

       todo_end();

       //bail_out("XXX");
     }
  )// TEST

