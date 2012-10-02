
//          Copyright Theo Willows 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
 * This is BugEye, a minimal C++ unit testing framework.
 *
 * See README.md for more information.
 *
 * @author Theo 'Biffen' Willows
 */

#pragma once

#ifdef _TEST

#include <stdio.h>
#include <string>
#include <vector>

/**
 * Keep /most of/ the BugEye specific code in its own namespace.
 */
namespace BugEye {

  int         failed;
  FILE*       out;
  int         total;
  bool        verbose = false;
  std::string version = "0.1.0";

  class bailout {

  public:

    bailout(const char* msg);
    std::string getMessage() const;

  private:

    std::string msg;

  };

  bailout::bailout(const char* msg) : msg(msg) {}

  std::string bailout::getMessage() const {
    return msg;
  }

  /**
   * This class is a superclass for all classes that have unit tests.  Its
   * function `run_test()` is meant to contain the actual test code.
   */
  class test {

  public:

    /**
     * This function is called once for each class when running tests.
     */
    void run_test();

  protected:

    std::string  name;
    bool         todo;
    std::string  todo_msg;

    virtual void inner_test() {};
    bool ok(bool expr, const char* msg);
    void pass(const char* msg);
    void fail(const char* msg);
    void diag(const char* msg);
    void note(const char* msg);
    void bail_out(const char* msg);
    void todo_begin(const char* msg);
    void todo_end();

  };

  void test::run_test() {
    try {
      inner_test();
    } catch (bailout const& b) {
      throw b;
    } catch (...) {
    }
  }

  bool test::ok(bool expr, const char* msg = 0) {
    total++;

    fprintf(out,
            "%s %d - %s%s%s\n",
            ( (expr) ? "ok" : "not ok" ),
            total,
            name.c_str(),
            ( (msg == 0) ? "" : std::string(" : ").append(msg).c_str() ),
            ( (todo) ? std::string(" # TODO ").append(todo_msg).c_str() : "" )
      );

    // Report this as a fail, unless it is a todo
    if (!todo) {
      if (!expr) {
        failed++;
      }
    }

    return expr;
  }

  void test::pass(const char* msg = 0) {
    ok(true, msg);
  }

  void test::fail(const char* msg = 0) {
    ok(false, msg);
  }

  void test::diag(const char* msg = 0) {
    fprintf(out,
            "# %s\n",
            ( (msg == 0) ? "" : std::string(msg).c_str() )
      );
  }

  void test::note(const char* msg = 0) {
    if (verbose) {
      diag(msg);
    }
  }

  void test::bail_out(const char* msg = 0) {
    throw bailout((msg == 0) ? "" : msg);
  }

  void test::todo_begin(const char* msg) {
    todo = true;
    todo_msg = (msg == 0) ? std::string() : std::string(msg);
  }

  void test::todo_end() {
    todo = false;
    todo_msg = std::string();
  }

  /**
   * This `std::vector` is meant to hold an instance of each class that extends
   * `BugEye::test`.
   */
  std::vector<test*> all_tests;

  /**
   * This function is responsible for running all test.  It should not be called
   * directly; use the `BUGEYE_RUN` macro instead.
   */
  int main() {
    failed = 0;
    out    = (out == 0) ? stdout : out;
    total  = 0;

    if (verbose) {
      fprintf(out, "# BugEye v%s\n", version.c_str());
    }

    std::vector<test*>::iterator it;
    for (it = all_tests.begin(); it != all_tests.end(); it++) {
      try {
        (*it)->run_test();
      } catch (bailout const& b) {
        fprintf(out, "Bail out!  %s\n", b.getMessage().c_str());
      }
    }

    // Print the summary
    fprintf(out, "\
# Totals\n\
#    Subjects %6d\n\
#   Tests run %6d\n\
#      Passed %6d\n\
#      Failed %6d\n",
            all_tests.size(),
            total,
            ( total - failed ),
            failed);

    // Print the plan
    fprintf(stdout, "1..%d\n", total);

    return failed;
  }


} // namespace BugEye

#define BUGEYE_SET_OUT(OUT) BugEye::out = OUT

#define BUGEYE_VERBOSE(B) BugEye::verbose = B

/**
 * Call `BugEye::main()` with a macro, so that it doesn't run if not compiled
 * with `_TEST`.
 */
#define BUGEYE_RUN return BugEye::main()

/**
 * This macro is the one that should be used the most.  It wraps a piece of test
 * code for a class.
 *
 * Within the code, `this` refers to an instance of a subclass of the class that
 * is being tested.
 *
 * Through subclassing it is possible to call protected functions, and use
 * protected member variables, but not private ones.
 *
 * @param SUBJECT The class to be tested
 * @param CODE The code to run to test the class
 */
#define TEST(SUBJECT, CODE)                                             \
  /**                                                                   \
   * This class is a subclass of `SUBJECT` and of `BugEye::test`, thus  \
   * allowing it to run  the test and use protected members of          \
   * `SUBJECT`.                                                         \
   */                                                                   \
  class SUBJECT##_TEST : public BugEye::test, SUBJECT {                 \
                                                                        \
  public:                                                               \
                                                                        \
    /**                                                                 \
     * The constructor adds `this` to the global list of `BugEye::test` \
     * instances.                                                       \
     */                                                                 \
    SUBJECT##_TEST() {                                                  \
      name = #SUBJECT;                                                  \
      BugEye::all_tests.push_back(this);                                \
    };                                                                  \
                                                                        \
    ~SUBJECT##_TEST(){};                                                \
                                                                        \
    void inner_test();                                                  \
                                                                        \
  };                                                                    \
                                                                        \
  /**                                                                   \
   * Create an instance, so that the constructor runs and the instance  \
   * is added to the global list of `BugEye::test` instances.           \
   */                                                                   \
  SUBJECT##_TEST SUBJECT##_TEST_INSTANCE;                               \
                                                                        \
  /**                                                                   \
   * Implement the `run_test` method with the code used in the macro.   \
   */                                                                   \
  void SUBJECT##_TEST::inner_test() {                                   \
    CODE                                                                \
      }

#else // #ifdef _TEST

/*
 * All these macros should do nothing when not in test mode.
 */
#define BUGEYE_SET_OUT(OUT)
#define BUGEYE_VERBOSE(B)
#define BUGEYE_RUN
#define TEST(SUBJECT, CODE)

#endif // #ifdef _TEST
