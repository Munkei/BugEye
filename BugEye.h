/* -*- C++ -*- */

// Copyright Theo Willows 2012-2015.
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

#  ifdef _MSC_VER
#    define _BUGEYE_FORMAT _In_opt_z_ _Printf_format_string_
#    define _BUGEYE_zu     "Iu"
#  else
#    define _BUGEYE_FORMAT
#    define _BUGEYE_zu "zu"
#  endif

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

      BailOut(const char* file,
              const size_t line,
              const std::string message);

      ~BailOut();

      std::string get_file() const;

      size_t      get_line() const;

      std::string get_message() const;

    private:

      std::string _file;

      size_t      _line;

      std::string _message;

  }; // class BailOut

  /**
   * This class is a superclass for all unit test groups.  Its function `_run()`
   * is meant to contain the actual test code.
   */
  class TestGroup {

    public:

      TestGroup(const long long plan,
                const char*     file,
                const size_t    line);

      virtual ~TestGroup();

      bool                    _bugeye_is_ok();

      size_t                  _bugeye_get_failed() const;

      std::unique_ptr<size_t> _bugeye_get_planned() const;

      size_t                  _bugeye_get_run() const;

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
                              const size_t                 line,
                              _BUGEYE_FORMAT const char*   format = nullptr,
                              ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 6, 7) ) )
#  endif
      ;

      bool _bugeye_is(std::function<bool()>      expr,
                      const char*                file,
                      const size_t               line,
                      _BUGEYE_FORMAT const char* format = nullptr,
                      ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 5, 6) ) )
#  endif
      ;

      bool _bugeye_isnt(std::function<bool()>      expr,
                        const char*                file,
                        const size_t               line,
                        _BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 5, 6) ) )
#  endif
      ;

      void _bugeye_pass(_BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
#  endif
      ;

      void _bugeye_fail(const char*                file,
                        const size_t               line,
                        _BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 4, 5) ) )
#  endif
      ;

      void _bugeye_diag(_BUGEYE_FORMAT const char* format = nullptr,
                        ...) const
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
#  endif
      ;

      void _bugeye_bail_out(const char*                file,
                            const size_t               line,
                            _BUGEYE_FORMAT const char* format = nullptr,
                            ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 4, 5) ) )
#  endif
      ;

      void _bugeye_skip_begin(_BUGEYE_FORMAT const char* format = nullptr,
                              ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
#  endif
      ;

      void _bugeye_skip_end();

      bool _bugeye_is_skip() const;

      bool _bugeye_is_todo() const;

      void _bugeye_todo_begin(_BUGEYE_FORMAT const char* format = nullptr,
                              ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
#  endif
      ;

      void _bugeye_todo_end();

    private:

      bool has_plan() const;

      void tally(bool                       result,
                 _BUGEYE_FORMAT const char* format,
                 va_list                    args);

      bool                    _failed;

      std::string             _location_file;

      size_t                  _location_line;

      bool                    _skip;

      std::string             _skip_message;

      size_t                  _tests_failed;

      std::unique_ptr<size_t> _tests_planned;

      size_t                  _tests_run;

      bool                    _todo;

      std::string             _todo_message;

  }; // class TestGroup

  class ClassTest
    : public TestGroup {

    public:

      ClassTest(const long long plan,
                const char*     clazz,
                const char*     file,
                const size_t    line);

      virtual ~ClassTest();

      std::string _bugeye_get_name() const override final;

    protected:

      std::string _clazz;

  }; // class ClassTest

  class NamedTest
    : public TestGroup {

    public:

      NamedTest(const long long plan,
                const char*     name,
                const char*     file,
                const size_t    line);

      virtual ~NamedTest();

      std::string _bugeye_get_name() const override final;

    protected:

      std::string _name;

  }; // class NamedTest

  class TestHarness final {

    public:

      static TestHarness& get();

      static std::string  stringf(
        _BUGEYE_FORMAT const char* format = nullptr,
        ...
      )
#  ifdef __GNUC__
      __attribute__( (format(printf, 1, 2) ) )
#  endif
      ;

      static std::string vstringf(_BUGEYE_FORMAT const char* format,
                                  va_list                    args);

      ~TestHarness();

      bool printf(const int   level,
                  const char* format = nullptr,
                  ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 3, 4) ) )
#  endif
      ;

      bool vprintf(const int                  level,
                   _BUGEYE_FORMAT const char* format,
                   va_list                    args);

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

      TestHarness(const TestHarness &);

      void operator=(const TestHarness&);

      template<typename T>
      std::map<std::string, T>& config_for();

      std::vector<TestGroup*> _all_groups;

  }; // class TestHarness

  inline BailOut::BailOut(const char*       file,
                          const size_t      line,
                          const std::string message)
    : _file(file),
      _line(line),
      _message(message) {}

  inline BailOut::~BailOut() {}

  inline std::string BailOut::get_file() const {
    return _file;
  }

  inline size_t BailOut::get_line() const {
    return _line;
  }

  inline std::string BailOut::get_message() const {
    return _message;
  }

  inline TestGroup::TestGroup(const long long plan,
                              const char*     file,
                              const size_t    line)
    : _failed(false),
      _location_file(file),
      _location_line(line),
      _skip(false),
      _skip_message(),
      _tests_failed(0),
      _tests_planned( (plan < 0)
                      ? nullptr
                      : new size_t(static_cast<unsigned>(plan) ) ),
      _tests_run(0),
      _todo(false),
      _todo_message() {
    TestHarness::get().add_group(this);
  }

  inline TestGroup::~TestGroup() {}

  inline bool TestGroup::_bugeye_is_ok() {
    if ( (_tests_failed > 0)
         || (_tests_planned
             && (*_tests_planned != _tests_run) ) ) {
      _failed = true;
    }

    return !_failed;
  }

  inline void TestGroup::tally(bool                       result,
                               _BUGEYE_FORMAT const char* format,
                               va_list                    args) {
    _tests_run++;
    std::string message = TestHarness::vstringf(format, args);

    TestHarness::get().printf(
      ( (result || _todo) ? 1 : 0 ),
      "%s %" _BUGEYE_zu "%s%s%s\n",
      // OK or not?
      ( (result) ? "ok" : "not ok" ),
      // The number of this test
      _tests_run,
      // A comment, if any
      ( (message.empty() ) ? "" : std::string(" - ").append(message).c_str() ),
      // A TODO mark, if applicable
      ( (_todo) ? std::string(" # TODO ").append(_todo_message).c_str() : "" ),
      // A SKIP mark, if applicable
      ( (_skip) ? std::string(" # SKIP ").append(_skip_message).c_str() : "" )
    );

    // Report this as a fail, unless it is a todo
    if (!result
        && !_todo
        && !_skip) {
      _tests_failed++;
    }
  } // tally

  inline size_t TestGroup::_bugeye_get_failed() const {
    return _tests_failed;
  }

  inline std::unique_ptr<size_t> TestGroup::_bugeye_get_planned() const {
    return std::move(std::unique_ptr<size_t>(_tests_planned
                                             ? nullptr
                                             : new size_t(*_tests_planned) ) );
  }

  inline size_t TestGroup::_bugeye_get_run() const {
    return _tests_run;
  }

  inline void TestGroup::_bugeye_run() {
    _tests_failed = 0;
    _tests_run    = 0;

    if (has_plan() ) {
      TestHarness::get().printf(1, "1..%" _BUGEYE_zu "\n", *_tests_planned);
    }

    try {
      _bugeye_inner_run();
    } catch (const BailOut&) {
      _tests_failed++;
      throw;
    } catch (const std::exception& e) {
      TestHarness::get().printf(-1, "Caught exception: %s\n", e.what() );
      _tests_failed++;
    } catch (...) {
      TestHarness::get().printf(-1, "Caught exception\n");
      _tests_failed++;
    }

    if (_tests_planned) {
      if (_tests_run < *_tests_planned) {
        TestHarness::get().printf(
          -1,
          "\
# %s:%" _BUGEYE_zu ":\n\
#   Looks like you planned %" _BUGEYE_zu " %s, but only %" _BUGEYE_zu " %s run\n",
          _location_file.c_str(),
          _location_line,
          *_tests_planned,
          ( (*_tests_planned == 1) ? "test" : "tests"),
          _tests_run,
          ( (_tests_run == 1) ? "was" : "were" )
        );
      } else if (_tests_run > *_tests_planned) {
        TestHarness::get().printf(
          -1,
          "\
# %s:%" _BUGEYE_zu ":\n\
#   Looks like you only planned %" _BUGEYE_zu " %s, but %" _BUGEYE_zu " %s run\n",
          _location_file.c_str(),
          _location_line,
          *_tests_planned,
          ( (*_tests_planned == 1) ? "test" : "tests"),
          _tests_run,
          ( (_tests_run == 1) ? "was" : "were" )
        );
      }
    } else {
      // No plan, so print what *was* run
      TestHarness::get().printf(1, "1..%" _BUGEYE_zu "\n", _tests_run);
    }
  } // TestGroup::run

  inline bool TestGroup::_bugeye_ok(const std::function<bool()>& expr,
                                    const char*                  expr_str,
                                    const char*                  file,
                                    const size_t                 line,
                                    _BUGEYE_FORMAT const char*   format,
                                    ...) {
    va_list args;
    bool    result = expr();

    va_start(args, format);
    tally(result, format, args);
    va_end(args);

    if (!result
        && !_todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%" _BUGEYE_zu ":\n\
#   Expression is false:\n\
#     %s\n",
        file,
        line,
        expr_str
      );
    }

    return result;
  } // TestGroup::_bugeye_ok

  inline bool TestGroup::_bugeye_is(std::function<bool()>      expr,
                                    const char*                file,
                                    const size_t               line,
                                    _BUGEYE_FORMAT const char* format,
                                    ...) {
    va_list args;

    // TODO unless SKIP
    bool result = expr();

    va_start(args, format);
    tally(result, format, args);
    va_end(args);

    if (!result
        && !_todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%" _BUGEYE_zu "\n",
        file,
        line
      );
    }

    return result;
  } // _bugeye_is

  inline bool TestGroup::_bugeye_isnt(std::function<bool()>      expr,
                                      const char*                file,
                                      const size_t               line,
                                      _BUGEYE_FORMAT const char* format,
                                      ...) {
    va_list args;
    bool    result = expr();

    va_start(args, format);
    tally(result, format, args);
    va_end(args);

    if (!result
        && !_todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%" _BUGEYE_zu "\n",
        file,
        line
      );
    }

    return result;
  } // _bugeye_isnt

  inline void TestGroup::_bugeye_pass(_BUGEYE_FORMAT const char* format,
                                      ...) {
    va_list args;

    va_start(args, format);
    tally(true, format, args);
    va_end(args);
  }

  inline void TestGroup::_bugeye_fail(const char*                file,
                                      const size_t               line,
                                      _BUGEYE_FORMAT const char* format,
                                      ...) {
    va_list args;

    va_start(args, format);
    tally(false, format, args);
    va_end(args);

    if (!_todo) {
      TestHarness::get().printf(
        2,
        "\
# %s:%" _BUGEYE_zu ":\n\
#   Fail!\n",
        file,
        line
      );
    }
  } // _bugeye_fail

  inline void TestGroup::_bugeye_diag(_BUGEYE_FORMAT const char* format,
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

  inline void TestGroup::_bugeye_bail_out(const char*                file,
                                          const size_t               line,
                                          _BUGEYE_FORMAT const char* format,
                                          ...) {
    va_list     args;
    std::string message;

    _failed = true;

    va_start(args, format);
    message = TestHarness::vstringf(format, args);
    va_end(args);

    throw BailOut(file, line, message);
  } // _bugeye_bail_out

  inline void TestGroup::_bugeye_skip_begin(_BUGEYE_FORMAT const char* format,
                                            ...) {
    va_list args;

    _skip = true;

    va_start(args, format);
    _skip_message = TestHarness::vstringf(format, args);
    va_end(args);
  }

  inline void TestGroup::_bugeye_skip_end() {
    _skip         = false;
    _skip_message = std::string();
  }

  inline bool TestGroup::_bugeye_is_skip() const {
    return _skip;
  }

  inline bool TestGroup::_bugeye_is_todo() const {
    return _todo;
  }

  inline void TestGroup::_bugeye_todo_begin(_BUGEYE_FORMAT const char* format,
                                            ...) {
    va_list args;

    _todo = true;

    va_start(args, format);
    _todo_message = TestHarness::vstringf(format, args);
    va_end(args);
  }

  inline void TestGroup::_bugeye_todo_end() {
    _todo         = false;
    _todo_message = std::string();
  }

  inline bool TestGroup::has_plan() const {
    return !!_tests_planned;
  }

  inline ClassTest::ClassTest(const long long plan,
                              const char*     clazz,
                              const char*     file,
                              const size_t    line)
    : TestGroup(plan, file, line),
      _clazz(clazz) {}

  inline ClassTest::~ClassTest() {}

  inline std::string ClassTest::_bugeye_get_name() const {
    return TestHarness::get().config_get<std::string>("class_prefix") + _clazz;
  }

  inline NamedTest::NamedTest(const long long plan,
                              const char*     name,
                              const char*     file,
                              const size_t    line)
    : TestGroup(plan, file, line),
      _name(name) {}

  inline NamedTest::~NamedTest() {}

  inline std::string NamedTest::_bugeye_get_name() const {
    return _name;
  }

  inline TestHarness& TestHarness::get() {
    static TestHarness instance;

    return instance;
  }

  inline std::string TestHarness::stringf(_BUGEYE_FORMAT const char* format,
                                          ...) {
    va_list     args;
    std::string string;

    va_start(args, format);
    string = vstringf(format, args);
    va_end(args);

    return string;
  }

  inline std::string TestHarness::vstringf(_BUGEYE_FORMAT const char* format,
                                           va_list                    args) {
#  ifndef va_copy
#    define va_copy(a, b) ( (a) = (b) )
#  endif

    if (format == nullptr) {
      return "";
    }

    va_list args2;

    va_copy(args2, args);
    size_t size
      = static_cast<unsigned>(std::vsnprintf(nullptr, 0, format, args2) ) + 1;

    std::vector<char> buffer(size, 0x00);

    std::vsnprintf(buffer.data(), size, format, args);

    return std::move(std::string(buffer.begin(),
                                 buffer.end() ) );
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

  inline bool TestHarness::vprintf(const int                  level,
                                   _BUGEYE_FORMAT const char* format,
                                   va_list                    args) {
    int verbosity = config_get<int>("verbosity");

    if (level <= verbosity) {
      if (format != nullptr) {
        std::vfprintf(config_get<FILE*>("out"), format, args);
      }

      return true;
    }

    return false;
  } // vprintf

  inline void TestHarness::add_group(TestGroup* group) {
    _all_groups.push_back(group);
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
    size_t                            groups_failed = 0;
    size_t                            groups_run    = 0;
    std::vector<TestGroup*>::iterator it;
    size_t                            longest_name  = 0;
    size_t                            tests_failed  = 0;
    size_t                            tests_run     = 0;

    for (it = _all_groups.begin(); it != _all_groups.end(); it++) {
      longest_name = std::max( (*it)->_bugeye_get_name().size(), longest_name);
    }

    for (it = _all_groups.begin(); it != _all_groups.end(); it++) {
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
# - at %s:%" _BUGEYE_zu "\n",
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
     Groups run: %6" _BUGEYE_zu "\n\
  Groups failed: %6" _BUGEYE_zu "\n\
      Tests run: %6" _BUGEYE_zu "\n\
   Tests failed: %6" _BUGEYE_zu "\n",
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

    return std::min(tests_failed, size_t(bugeye_error - 1) );
  } // run

  inline TestHarness::TestHarness()
    : _all_groups() {
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

#  define ok(EXPR, ...)       \
  _bugeye_ok([&]() -> bool {  \
               return (EXPR); \
             },               \
             #EXPR,           \
             __FILE__,        \
             __LINE__,        \
             __VA_ARGS__)

#  define is(ACTUAL, EXPECTED, ...)               \
  _bugeye_is([&]() -> bool {                      \
               return ( (ACTUAL) == (EXPECTED) ); \
             },                                   \
             __FILE__,                            \
             __LINE__,                            \
             __VA_ARGS__)

#  define isnt(ACTUAL, NOT_EXPECTED, ...)               \
  _bugeye_isnt([&]() -> bool {                          \
                 return ( (ACTUAL) != (NOT_EXPECTED) ); \
               },                                       \
               __FILE__,                                \
               __LINE__,                                \
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
      pass(nullptr);                    \
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
