#include "Foo.h"

#include <BugEye.h>
#include <assert.h>

#include <stdio.h>
#include <vector>

Foo::Foo() {
  printf("%s\n", __FUNCTION__);
}

Foo::~Foo() {
  printf("%s\n", __FUNCTION__);
}

void Foo::public_function() {
  printf("%s\n", __FUNCTION__);
}

void Foo::protected_function() {
  printf("%s\n", __FUNCTION__);
}

void Foo::private_function() {
  printf("%s\n", __FUNCTION__);
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
