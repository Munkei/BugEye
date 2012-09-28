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
  BUGEYE_RUN;

  printf("This is the code that runs when we're not testing\n");
}

TEST(Foo,
     {
       printf("This is the test function for Foo\n");

       this->public_function();
       this->protected_function();
       //this->private_function(); // Won't work

       assert(true);
     }
  )// TEST

