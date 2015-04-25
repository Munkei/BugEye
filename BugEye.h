/* -*- C++ -*- */

// Copyright Theo Willows 2012-2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/**
 * This is BugEye, a minimal C++ unit testing framework.
 *
 * See README.md for more information.
 *
 * @author Theo 'Biffen' Willows
 */

#pragma once

/* *INDENT-OFF* */
#define BUGEYE_VERSION 2.0.0
/* *INDENT-ON* */

#ifdef _TEST

#  include <algorithm>
#  include <cstdarg>
#  include <cstdio>
#  include <cstring>
#  include <functional>
#  include <map>
#  include <memory>
#  include <sstream>
#  include <stdexcept>
#  include <string>
#  include <vector>

/**
 * Keep *most of* the BugEye specific code in its own namespace.
 */
namespace BugEye {

  /**
   * An object of this class is thrown when `TestGroup::bail_out()` is called,
   * and caught by the harness to mark a group as having bailed out.
   */
  class BailOut final {

    public:

      BailOut(const char*       file,
              const int         line,
              const std::string message);

      ~BailOut();

      std::string get_file() const;

      int         get_line() const;

      std::string get_message() const;

    private:

      std::string file;

      int         line;

      std::string message;

  }; // class BailOut

  /**
   * This class is a superclass for all unit test groups.  Its function `_run()`
   * is meant to contain the actual test code.
   */
  class TestGroup {

    public:

      TestGroup(int         plan,
                const char* file,
                const int   line);

      virtual ~TestGroup();

      bool _bugeye_is_ok();

      int  _bugeye_get_failed() const;

      int  _bugeye_get_planned() const;

      int  _bugeye_get_run() const;

      /**
       * This function is called once for each class when running tests.
       */
      void                _bugeye_run();

      virtual std::string _bugeye_get_name() const = 0;

    protected:

      virtual void _bugeye_inner_run() = 0;

      bool         _bugeye_ok(const std::function<bool()>& expr,
                              const char*                  expr_str,
                              const char*                  file,
                              const int                    line,
                              const char*                  format,
                              ...);

      bool _bugeye_is(std::function<bool(std::string&,
                                         std::string&)> expr,
                      const char*                       file,
                      const int                         line,
                      const char*                       format,
                      ...);

      bool _bugeye_isnt(std::function<bool(std::string&,
                                           std::string&)> expr,
                        const char*                       file,
                        const int                         line,
                        const char*                       format,
                        ...);

      void _bugeye_pass(const char* format,
                        ...);

      void _bugeye_fail(const char* file,
                        const int   line,
                        const char* format,
                        ...);

      void _bugeye_diag(const char* format,
                        ...) const;

      void _bugeye_bail_out(const char* file,
                            const int   line,
                            const char* format,
                            ...);

      void _bugeye_skip_begin(const char* format,
                              ...);

      void _bugeye_skip_end();

      bool _bugeye_is_skip() const;

      bool _bugeye_is_todo() const;

      void _bugeye_todo_begin(const char* format,
                              ...);

      void _bugeye_todo_end();

    private:

      bool has_plan() const;

      void tally(bool        result,
                 const char* format,
                 va_list     args);

      bool        failed;

      std::string location_file;

      int         location_line;

      bool        skip;

      std::string skip_message;

      int         tests_failed;

      int         tests_planned;

      int         tests_run;

      bool        todo;

      std::string todo_message;

  }; // class TestGroup

  class ClassTest
    : public TestGroup {

    public:

      ClassTest(int         plan,
                const char* clazz,
                const char* file,
                const int   line);

      virtual ~ClassTest();

      std::string _bugeye_get_name() const override final;

    protected:

      std::string clazz;

  }; // class ClassTest

  class NamedTest
    : public TestGroup {

    public:

      NamedTest(int         plan,
                const char* name,
                const char* file,
                const int   line);

      virtual ~NamedTest();

      std::string _bugeye_get_name() const override final;

    protected:

      std::string name;
  }; // class NamedTest

  class TestHarness final {
    public:

      static TestHarness& get();

      static std::string  stringf(const char* format,
                                  ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 1, 2) ) )
#  endif
      ;

      static std::string vstringf(const char* format,
                                  va_list     args);

      ~TestHarness();

      bool printf(const int   level,
                  const char* format,
                  ...);

      bool vprintf(const int   level,
                   const char* format,
                   va_list     args);

      void add_group(TestGroup* group);

      template<typename T>
      T config_get(const char* key);

      template<typename T>
      void config_set(const char* key,
                      T           value);

      int run();

    private:

      static const unsigned int bugeye_error = 126;

      TestHarness();

      TestHarness(const TestHarness&);

      void operator=(const TestHarness&);

      template<typename T>
      std::map<std::string, T>& config_for();

      std::vector<TestGroup*> all_groups;

  }; // class TestHarness

  inline BailOut::BailOut(const char*       file,
                          const int         line,
                          const std::string message)
    : file(file),
    line(line),
    message(message) {}

  inline BailOut::~BailOut() {}

  inline std::string BailOut::get_file() const {
    return file;
  }

  inline int BailOut::get_line() const {
    return line;
  }

  inline std::string BailOut::get_message() const {
    return message;
  }

  inline TestGroup::TestGroup(int         plan,
                              const char* file,
                              const int   line)
    : failed(false),
    location_file(file),
    location_line(line),
    skip(false),
    skip_message(),
    tests_failed(0),
    tests_planned(plan),
    tests_run(0),
    todo(false),
    todo_message() {
    TestHarness::get().add_group(this);
  }

  inline TestGroup::~TestGroup() {}

  inline bool TestGroup::_bugeye_is_ok() {
    if ( (tests_failed > 0)
         || (has_plan()
             && (tests_planned != tests_run) ) ) {
      failed = true;
    }

    return !failed;
  }

  inline void TestGroup::tally(bool        result,
                               const char* format,
                               va_list     args) {
    tests_run++;
    std::string message = TestHarness::vstringf(format, args);

    TestHarness::get().printf(
      ( (result || todo) ? 1 : 0 ),
      "%s %d%s%s%s\n",
      // OK or not?
      ( (result) ? "ok" : "not ok" ),
      // The number of this test
      tests_run,
      // A comment, if any
      ( (message.empty() ) ? "" : std::string(" - ").append(message).c_str() ),
      // A TODO mark, if applicable
      ( (todo) ? std::string(" # TODO ").append(todo_message).c_str() : "" ),
      // A SKIP mark, if applicable
      ( (skip) ? std::string(" # SKIP ").append(skip_message).c_str() : "" )
    );

    // Report this as a fail, unless it is a todo
    if (!result
        && !todo
        && !skip) {
      tests_failed++;
    }
  } // tally

  inline int TestGroup::_bugeye_get_failed() const {
    return tests_failed;
  }

  inline int TestGroup::_bugeye_get_planned() const {
    return tests_planned;
  }

  inline int TestGroup::_bugeye_get_run() const {
    return tests_run;
  }

  inline void TestGroup::_bugeye_run() {
    tests_failed = 0;
    tests_run    = 0;

    if (has_plan() ) {
      TestHarness::get().printf(1, "1..%d\n", tests_planned);
    }

    try {
      _bugeye_inner_run();
    } catch (const BailOut&) {
      tests_failed++;
      throw;
    } catch (const std::exception& e) {
      TestHarness::get().printf(-1, "Caught exception: %s\n", e.what() );
      tests_failed++;
    } catch (...) {
      TestHarness::get().printf(-1, "Caught exception\n");
      tests_failed++;
    }

    if (has_plan() ) {
      if (tests_run < tests_planned) {
        TestHarness::get().printf(
          -1,
          "\
# %s:%d:\n\
#   Looks like you planned %d %s, but only %d %s run\n",
          location_file.c_str(),
          location_line,
          tests_planned,
          ( (tests_planned == 1) ? "test" : "tests"),
          tests_run,
          ( (tests_run == 1) ? "was" : "were" )
        );
      } else if (tests_run > tests_planned) {
        TestHarness::get().printf(
          -1,
          "\
# %s:%d:\n\
#   Looks like you only planned %d %s, but %d %s run\n",
          location_file.c_str(),
          location_line,
          tests_planned,
          ( (tests_planned == 1) ? "test" : "tests"),
          tests_run,
          ( (tests_run == 1) ? "was" : "were" )
        );
      }
    } else {
      // No plan, so print what was run
      TestHarness::get().printf(1, "1..%d\n", tests_run);
    }
  } // TestGroup::run

  inline bool TestGroup::_bugeye_ok(const std::function<bool()>& expr,
                                    const char*                  expr_str,
                                    const char*                  file,
                                    const int                    line,
                                    const char*                  format,
                                    ...) {
    va_list args;
    bool    result = expr();

    va_start(args, format);
    tally(result, format, args);
    va_end(args);

    if (!result
        && !todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%d:\n\
#   Expression is false:\n\
#     %s\n",
        file,
        line,
        expr_str
      );
    }

    return result;
  } // TestGroup::_bugeye_ok

  inline bool TestGroup::_bugeye_is(std::function<bool(std::string&,
                                                       std::string&)> expr,
                                    const char*                       file,
                                    const int                         line,
                                    const char*                       format,
                                    ...) {
    std::string actual_str;
    va_list     args;
    std::string expected_str;

    bool        result = expr(actual_str, expected_str);

    va_start(args, format);
    tally(result, format, args);
    va_end(args);

    if (!result
        && !todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%d:\n\
#   Got:\n\
#     %s\n\
#   Expected:\n\
#     %s\n",
        file,
        line,
        actual_str.c_str(),
        expected_str.c_str()
      );
    }

    return result;
  } // _bugeye_is

  inline bool TestGroup::_bugeye_isnt(std::function<bool(std::string&,
                                                         std::string&)> expr,
                                      const char*                       file,
                                      const int                         line,
                                      const char*                       format,
                                      ...) {
    std::string actual_str;
    va_list     args;
    std::string not_expected_str;
    bool        result = expr(actual_str, not_expected_str);

    va_start(args, format);
    tally(result, format, args);
    va_end(args);

    if (!result
        && !todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%d:\n\
#   Got:\n\
#     %s\n\
#   The same as:\n\
#     %s\n",
        file,
        line,
        actual_str.c_str(),
        not_expected_str.c_str()
      );
    }

    return result;
  } // _bugeye_isnt

  inline void TestGroup::_bugeye_pass(const char* format,
                                      ...) {
    va_list args;

    va_start(args, format);
    tally(true, format, args);
    va_end(args);
  }

  inline void TestGroup::_bugeye_fail(const char* file,
                                      const int   line,
                                      const char* format,
                                      ...) {
    va_list args;

    va_start(args, format);
    tally(false, format, args);
    va_end(args);

    if (!todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%d:\n\
#   Fail!\n",
        file,
        line
      );
    }
  } // _bugeye_fail

  inline void TestGroup::_bugeye_diag(const char* format,
                                      ...) const {
    va_list args;

    // TODO Find newlines and prepend "# " to each
    va_start(args, format);
    TestHarness::get().vprintf(
      1,
      std::string("# ").append(format).append("\n").c_str(),
      args
    );
    va_end(args);
  } // _bugeye_diag

  inline void TestGroup::_bugeye_bail_out(const char* file,
                                          const int   line,
                                          const char* format,
                                          ...) {
    va_list     args;
    std::string message;

    failed = true;

    va_start(args, format);
    message = TestHarness::vstringf(format, args);
    va_end(args);

    throw BailOut(file, line, message);
  } // _bugeye_bail_out

  inline void TestGroup::_bugeye_skip_begin(const char* format,
                                            ...) {
    va_list args;

    skip = true;

    va_start(args, format);
    skip_message = TestHarness::vstringf(format, args);
    va_end(args);
  }

  inline void TestGroup::_bugeye_skip_end() {
    skip         = false;
    skip_message = std::string();
  }

  inline bool TestGroup::_bugeye_is_skip() const {
    return skip;
  }

  inline bool TestGroup::_bugeye_is_todo() const {
    return todo;
  }

  inline void TestGroup::_bugeye_todo_begin(const char* format,
                                            ...) {
    va_list args;

    todo = true;

    va_start(args, format);
    todo_message = TestHarness::vstringf(format, args);
    va_end(args);
  }

  inline void TestGroup::_bugeye_todo_end() {
    todo         = false;
    todo_message = std::string();
  }

  inline bool TestGroup::has_plan() const {
    return tests_planned >= 0;
  }

  inline ClassTest::ClassTest(int         plan,
                              const char* clazz,
                              const char* file,
                              const int   line)
    : TestGroup(plan, file, line),
    clazz(clazz) {}

  inline ClassTest::~ClassTest() {}

  inline std::string ClassTest::_bugeye_get_name() const {
    return TestHarness::get().config_get<std::string>("class_prefix") + clazz;
  }

  inline NamedTest::NamedTest(int         plan,
                              const char* name,
                              const char* file,
                              const int   line)
    : TestGroup(plan, file, line),
    name(name) {}

  inline NamedTest::~NamedTest() {}

  inline std::string NamedTest::_bugeye_get_name() const {
    return name;
  }

  inline TestHarness& TestHarness::get() {
    static TestHarness instance;

    return instance;
  }

  inline std::string TestHarness::stringf(const char* format,
                                          ...) {
    va_list     args;
    std::string string;

    va_start(args, format);
    string = vstringf(format, args);
    va_end(args);

    return string;
  }

  inline std::string TestHarness::vstringf(const char* format,
                                           va_list     args) {
#  ifndef va_copy
#    define va_copy(a, b) ( (a) = (b) )
#  endif

    va_list     args2;
    char*       c_string;
    int         size;
    std::string string;

    va_copy(args2, args);
    size     = (vsnprintf(nullptr, 0, format, args2) + 1);

    c_string = (char*)malloc(size);
    memset(c_string, 0, size);

    vsnprintf(c_string, size, format, args);
    string.assign(c_string, (size - 1) );

    delete c_string;

    return string;
  } // vstringf

  inline bool TestHarness::printf(const int   level,
                                  const char* format,
                                  ...) {
    va_list args;
    bool    res;

    va_start(args, format);
    res = vprintf(level, format, args);
    va_end(args);

    return res;
  }

  inline bool TestHarness::vprintf(const int   level,
                                   const char* format,
                                   va_list     args) {
    int verbosity = config_get<int>("verbosity");

    if (level <= verbosity) {
      vfprintf(config_get<FILE*>("out"), format, args);

      return true;
    }

    return false;
  } // vprintf

  inline void TestHarness::add_group(TestGroup* group) {
    all_groups.push_back(group);
  }

  template<typename T>
  inline T TestHarness::config_get(const char* key) {
    try {
      return config_for<T>().at(key);
    } catch (...) {
      fprintf(stderr, "BugEye: No config value for key \"%s\"\n", key);
      exit(bugeye_error);
    }
  }

  template<typename T>
  inline void TestHarness::config_set(const char* key,
                                      T           value) {
    config_for<T>()[key] = value;
  }

  /**
   * This function is responsible for running all test.  It should not be called
   * directly; use the `BUGEYE_RUN` macro instead.
   */
  inline int TestHarness::run() {
    unsigned int                      groups_failed = 0;
    unsigned int                      groups_run    = 0;

    std::vector<TestGroup*>::iterator it;
    size_t                            longest_name = 0;
    unsigned int                      tests_failed = 0;
    unsigned int                      tests_run    = 0;

    for (it = all_groups.begin(); it != all_groups.end(); it++) {
      longest_name = std::max( (*it)->_bugeye_get_name().size(), longest_name);
    }

    for (it = all_groups.begin(); it != all_groups.end(); it++) {
      TestGroup* group = *it;
      printf(0,
             "%s %s",
             group->_bugeye_get_name().c_str(),
             std::string(
               ( (longest_name - group->_bugeye_get_name().size() ) + 2),
               '.').c_str()
      );
      if (!printf(1, "\n") ) {
        printf(0, " ");
      }

      try {
        group->_bugeye_run();
      } catch (const BailOut& b) {
        printf(
          1,
          "\
Bail out!  %s\n\
# - at %s:%d\n",
          b.get_message().c_str(),
          b.get_file().c_str(),
          b.get_line()
        );
      }
      printf(0,
             "%s\n",
             ( group->_bugeye_is_ok() ? "ok" : "not ok") );

      tests_failed += group->_bugeye_get_failed();
      tests_run    += group->_bugeye_get_run();
      groups_run++;
      if (!group->_bugeye_is_ok() ) {
        groups_failed++;
      }
    }

    // Print the summary
    if (groups_failed == 0) {
      printf(0, "All tests successful.\n");
    }

    printf(
      1,
      "\
Summary\n\
     Groups run: %6d\n\
  Groups failed: %6d\n\
      Tests run: %6d\n\
   Tests failed: %6d\n",
      groups_run,
      groups_failed,
      tests_run,
      tests_failed
      // TODO todos and skipped
    );
    // TODO Time, e.g: "3 wallclock secs ( 0.04 usr  0.02 sys +  1.27 cusr  0.10
    // csys =  1.43 CPU)"
    printf(
      0,
      "Result: %s\n",
      ( (groups_failed <= 0) ? "PASS" : "FAIL" )
    );

    return std::min(tests_failed, (bugeye_error - 1) );
  } // run

  inline TestHarness::TestHarness()
    : all_groups() {
    config_set<int>("verbosity", 0);
    config_set<std::string>("class_prefix", "Test for class: ");
    config_set<FILE*>("out", stdout);
  }

  inline TestHarness::~TestHarness() {}

  template<typename T>
  inline std::map<std::string, T>& TestHarness::config_for() {
    static std::map<std::string, T> map;

    return map;
  }

} // namespace BugEye

#  define BUGEYE_SET(KEY, VALUE) \
  BugEye::TestHarness::get().config_set(#KEY, VALUE)

#  define BUGEYE_RUN return BugEye::TestHarness::get().run()

#  define CLASS_TEST(CLAZZ, PLAN)                                   \
  class _BUGEYE_CLASS_TEST_##CLAZZ                                  \
    : public CLAZZ,                                                 \
      public virtual BugEye::ClassTest {                            \
                                                                    \
    public:                                                         \
                                                                    \
      _BUGEYE_CLASS_TEST_##CLAZZ()                                  \
        : BugEye::ClassTest(PLAN, #CLAZZ, __FILE__, __LINE__) {};   \
                                                                    \
      ~_BUGEYE_CLASS_TEST_##CLAZZ() {};                             \
                                                                    \
      void _bugeye_inner_run() override;                            \
                                                                    \
  };                                                                \
                                                                    \
  _BUGEYE_CLASS_TEST_##CLAZZ _BUGEYE_CLASS_TEST_##CLAZZ##_INSTANCE; \
                                                                    \
  void _BUGEYE_CLASS_TEST_##CLAZZ::_bugeye_inner_run()

#  define NAMED_TEST(NAME, PLAN)                                 \
  class _BUGEYE_NAMED_TEST_##NAME                                \
    : public virtual BugEye::NamedTest {                         \
                                                                 \
    public:                                                      \
                                                                 \
      _BUGEYE_NAMED_TEST_##NAME()                                \
        : NamedTest(PLAN, #NAME, __FILE__, __LINE__) {};         \
                                                                 \
      ~_BUGEYE_NAMED_TEST_##NAME() {}                            \
                                                                 \
      void _bugeye_inner_run() override;                         \
                                                                 \
  };                                                             \
                                                                 \
  _BUGEYE_NAMED_TEST_##NAME _BUGEYE_NAMED_TEST_##NAME##INSTANCE; \
                                                                 \
  void _BUGEYE_NAMED_TEST_##NAME::_bugeye_inner_run()

#  define ok(EXPR, ...)      \
  _bugeye_ok([&]() -> bool { \
    return (EXPR);           \
  },                         \
             #EXPR,          \
             __FILE__,       \
             __LINE__,       \
             __VA_ARGS__)

#  define is(ACTUAL, EXPECTED, ...)                   \
  _bugeye_is([&](std::string& actual_str,             \
                 std::string& expected_str) -> bool { \
    std::stringstream ss;                             \
    auto actual = (ACTUAL);                           \
    auto expected = (EXPECTED);                       \
                                                      \
    try {                                             \
      ss << actual;                                   \
      actual_str.assign(ss.str() );                   \
    }                                                 \
    catch (...) {                                     \
      actual_str.assign(#ACTUAL);                     \
    }                                                 \
                                                      \
    ss.str(std::string() );                           \
                                                      \
    try {                                             \
      ss << expected;                                 \
      expected_str.assign(ss.str() );                 \
    }                                                 \
    catch (...) {                                     \
      expected_str.assign(#EXPECTED);                 \
    }                                                 \
                                                      \
    return ( (actual) == (expected) );                \
  },                                                  \
             __FILE__,                                \
             __LINE__,                                \
             __VA_ARGS__)

#  define isnt(ACTUAL, NOT_EXPECTED, ...)                   \
  _bugeye_isnt([&](std::string& actual_str,                 \
                   std::string& not_expected_str) -> bool { \
    std::stringstream ss;                                   \
    auto actual = (ACTUAL);                                 \
    auto not_expected = (NOT_EXPECTED);                     \
                                                            \
    try {                                                   \
      ss << actual;                                         \
      actual_str.assign(ss.str() );                         \
    }                                                       \
    catch (...) {                                           \
      actual_str.assign(#ACTUAL);                           \
    }                                                       \
                                                            \
    ss.str(std::string() );                                 \
                                                            \
    try {                                                   \
      ss << not_expected;                                   \
      not_expected_str.assign(ss.str() );                   \
    }                                                       \
    catch (...) {                                           \
      not_expected_str.assign(#NOT_EXPECTED);               \
    }                                                       \
                                                            \
    return ( (actual) != (not_expected) );                  \
  },                                                        \
               __FILE__,                                    \
               __LINE__,                                    \
               __VA_ARGS__)

#  define diag(...) \
  _bugeye_diag(__VA_ARGS__)

#  define pass(...) \
  _bugeye_pass(__VA_ARGS__)

#  define fail(...)      \
  _bugeye_fail(__FILE__, \
               __LINE__, \
               __VA_ARGS__)

#  define todo(...)                     \
  for (_bugeye_todo_begin(__VA_ARGS__); \
       _bugeye_is_todo();               \
       _bugeye_todo_end() )

#  define skip(CONDITION, HOWMANY, ...) \
  if (CONDITION) {                      \
    _bugeye_skip_begin(__VA_ARGS__);    \
    for (unsigned int _bugeye_i = 0;    \
         _bugeye_i < (HOWMANY);         \
         _bugeye_i++) {                 \
      pass("");                         \
    }                                   \
    _bugeye_skip_end();                 \
  }                                     \
  else                                  \

#  define bail_out(...)      \
  _bugeye_bail_out(__FILE__, \
                   __LINE__, \
                   __VA_ARGS__)

#else // #ifdef _TEST

/*
 * All these macros should do nothing when not in test mode.
 */

#  define BUGEYE_RUN
#  define BUGEYE_SET(...)
#  define BUGEYE_VOID(...)

#  define _BUGEYE_UNUSED(...) \
  []() {}                     \
  (__VA_ARGS__);

#  define CLASS_TEST(CLAZZ, ...)         \
  template<typename _bugeye_unused>      \
  class _BUGEYE_UNUSED_CLASS_TEST##CLAZZ \
    : private CLAZZ {                    \
    private:                             \
      void _bugeye_();                   \
  };                                     \
                                         \
  template<typename _bugeye_unused>      \
  void _BUGEYE_UNUSED_CLASS_TEST##CLAZZ<_bugeye_unused>::_bugeye_()

#  define NAMED_TEST(NAME, PLAN)           \
  template<typename _bugeye_unused>        \
  class _BUGEYE_UNUSED_NAMED_TEST_##NAME { \
    private:                               \
      void _bugeye_();                     \
  };                                       \
                                           \
  template<typename _bugeye_unused>        \
  void _BUGEYE_UNUSED_NAMED_TEST_##NAME<_bugeye_unused>::_bugeye_()

#  define ok(...)       _BUGEYE_UNUSED(__VA_ARGS__)
#  define is(...)       _BUGEYE_UNUSED(__VA_ARGS__)
#  define isnt(...)     _BUGEYE_UNUSED(__VA_ARGS__)
#  define diag(...)     _BUGEYE_UNUSED(__VA_ARGS__)
#  define pass(...)     _BUGEYE_UNUSED(__VA_ARGS__)
#  define fail(...)     _BUGEYE_UNUSED(__VA_ARGS__)
#  define skip(...)     _BUGEYE_UNUSED(__VA_ARGS__)
#  define todo(...)     _BUGEYE_UNUSED(__VA_ARGS__)
#  define bail_out(...) _BUGEYE_UNUSED(__VA_ARGS__)

#endif // ifdef _TEST

#define CLASS_TEST_NO_PLAN(CLAZZ) \
  CLASS_TEST(CLAZZ, -1)

#define NAMED_TEST_NO_PLAN(NAME) \
  NAMED_TEST(NAME, -1)
