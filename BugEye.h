/**
 * This is BugEye, a minimal C++ unit testing framework.
 *
 * See LICENSE.md for licensing information.
 *
 * See README.md for more information.
 *
 * @auhtor Theo 'Biffen' Willows
 */

#pragma once

#ifdef _TEST

#include <vector>

/**
 * Keep /most of/ the BugEye specific code in its own namespace.
 */
namespace BugEye {

  /**
   * This class is a superclass for all classes that have unit tests.  Its
   * function `run_test()` is meant to contain the actual test code.
   */
  class test {

  public:

    /**
     * This function is called once for each class when running tests.  It is
     * not supposed to return anything, if something goes wrong that should halt
     * execution, something should be thrown, preferably a `std::exception`.
     */
    virtual void run_test() {};

  };

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
    std::vector<test*>::iterator it;
    for (it = all_tests.begin(); it != all_tests.end(); it++) {
      (*it)->run_test();
    }

    return 0;
  }

} // namespace BugEye

/**
 * Call `BugEye::main` with a macro, so that it doesn't run if not compiled with
 * `_TEST`.
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
     * The constructor adds `this` to the global list of `txt::test`    \
     * instances.                                                       \
     */                                                                 \
    SUBJECT##_TEST() {                                                  \
      BugEye::all_tests.push_back(this);                                \
    };                                                                  \
                                                                        \
    ~SUBJECT##_TEST(){};                                                \
                                                                        \
    void run_test();                                                    \
                                                                        \
  };                                                                    \
                                                                        \
/**                                                                     \
 * Create an instance, so that the constructor runs and the instance    \
 * is added to the global list of `BugEye::test` instances.             \
 */                                                                     \
  SUBJECT##_TEST SUBJECT##_TEST_INSTANCE;                               \
                                                                        \
/**                                                                     \
 * Implement the `run_test` method with the code used in the macro.     \
 */                                                                     \
  void SUBJECT##_TEST::run_test() {                                     \
    CODE                                                                \
      }

#else // #ifdef _TEST

/*
 * TEST should do nothing when not in test mode
 */
#define TEST(SUBJECT, CODE)

/*
 * BUGEYE_RUN should do nothing when not in test mode
 */
#define BUGEYE_RUN

#endif // #ifdef _TEST
