/* -*- C++ -*- */

// Copyright Theo Willows 2015-2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BUGEYE3_H
#define BUGEYE3_H

#ifdef _MSC_VER
#  pragma warning(push, 0)
#endif

#include <string>

#ifdef _MSC_VER
#  pragma warning(pop)
#endif

#if defined(TEST) || defined(_TEST)

#  ifdef _MSC_VER
#    pragma warning(push, 0)
#  endif

#  include <algorithm>
#  include <climits>
#  include <cstdarg>
#  include <cstdlib>
#  include <functional>
#  include <iomanip>
#  include <iostream>
#  include <iterator>
#  include <locale>
#  include <map>
#  include <memory>
#  include <regex>
#  include <set>
#  include <sstream>
#  include <stack>
#  include <type_traits>
#  include <unordered_set>
#  include <utility>
#  include <vector>

#  ifdef _MSC_VER
#    pragma warning(pop)
#    define BUGEYE_FORMAT _In_opt_z_ _Printf_format_string_
#  else
#    define BUGEYE_FORMAT
#  endif

namespace bugeye {

  enum class directive_t : unsigned char {
    none,
    skip,
    todo,
  };

  struct location_t final {

    const std::string file;

    const size_t      line;

    location_t(const std::string& file_,
               const size_t       line_);

    location_t(const location_t& other)                     = default;

    location_t&          operator=(const location_t& other) = delete;

    std::ostream&        stream_to(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream&     os,
                                    const location_t& location);

  }; // struct location_t

  class assertion final {

    public:

      const std::string description;

      directive_t directive;

      std::string directive_reason;

      const std::shared_ptr<location_t> location;

      size_t number;

      assertion(
        const bool                                 ok_,
        const std::string&                         description_ = "",
        const std::shared_ptr<bugeye::location_t>& location_ = nullptr,
        const directive_t                          directive_ = directive_t::none
      );

      assertion(const assertion& other) = default;

      bool                 ok() const;

      assertion&           operator=(const assertion& other) = delete;

      std::ostream&        stream_to(std::ostream& os) const;

      friend std::ostream& operator<<(std::ostream&    os,
                                      const assertion& assertion_);

    protected:

    private:

      const bool _ok;

  }; // struct assertion

  struct bail_out_exception final
    : public std::runtime_error {

    const location_t location;

    explicit bail_out_exception(const location_t&  location_,
                                const std::string& what_arg);

  }; // class bail_out_exception

  struct config_error final
    : public std::logic_error {

    explicit config_error(const std::string& what_arg);

  }; // struct config_error

  enum class exit_code : int {
    ok            = 0,
    configuration = 2,
    tests_failed  = 3,
  }; // enum class exit_code

  class test {

    public:

      enum class end_result_t : unsigned char {
        none,
        ok,
        not_ok,
        exception,
        too_few,
        too_many,
      };

      class execution;
      class impl;

      const std::string name;

      explicit test(const std::string& name_);

      test(const test& other) = default;

      virtual ~test()         = default;

      template<typename T>
      test& operator=(T body);

      test& operator=(const test& other) = delete;

      test& plan(size_t p);

    protected:

      static std::set<impl>& tests();

      std::shared_ptr<size_t> _plan;

    private:

  }; // class test

  class test::impl
    : public test {

    public:

      template<typename body_t> impl(const test&   test_,
                                     const body_t& body);

      impl(const impl& other) = delete;

      bool         operator<(const impl& other) const;

      virtual void run() const;

    protected:

      explicit impl(const std::string& name_);

      std::function<void()> _body;

    private:

  }; // class test::impl

  class test::execution final {

    public:

      static void bail_out_(const location_t&         location,
                            BUGEYE_FORMAT const char* format = nullptr,
                            ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
      __attribute__ ( (noreturn) )
#  endif
      ;

      static std::stack<execution>& current();

      static void                   diag_(
        BUGEYE_FORMAT const char* format = nullptr,
        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 1, 2) ) )
#  endif
      ;

      static void fail_(const location_t&         location,
                        BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
#  endif
      ;

      static bool ok_(const location_t&         location,
                      bool                      expr,
                      BUGEYE_FORMAT const char* format = nullptr,
                      ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 3, 4) ) )
#  endif
      ;

      static void pass_(const location_t&         location,
                        BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 2, 3) ) )
#  endif
      ;

      static void push(assertion ar);

      static bool run(const test::impl& t);

      static bool run_all(int          argc,
                          char const** argv);

      template<typename got_t,
               typename expected_t>
      static bool is_(const location_t&         location,
                      const got_t&              got,
                      const expected_t&         expected,
                      BUGEYE_FORMAT const char* format = nullptr,
                      ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 4, 5) ) )
#  endif
      ;

      template<typename got_t,
               typename unexpected_t>
      static bool isnt_(const location_t&         location,
                        const got_t&              got,
                        const unexpected_t&       unexpected,
                        BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 4, 5) ) )
#  endif
      ;

      static void skip_(const location_t&         location,
                        const size_t              how_many,
                        BUGEYE_FORMAT const char* format = nullptr,
                        ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 3, 4) ) )
#  endif
      ;

      static bool todo_start(BUGEYE_FORMAT const char* format = nullptr,
                             ...)
#  ifdef __GNUC__
      __attribute__( (format(printf, 1, 2) ) )
#  endif
      ;

      static bool todo_stop();

      std::vector<assertion> assertions;

      std::stack<std::pair<const directive_t,
                           const std::string> > directives;

      end_result_t end_result;

      explicit execution(const test::impl& test);

      execution(const execution& other)            = delete;

      execution(execution&& other)                 = delete;

      execution& operator=(const execution& other) = delete;

      bool       run();

    protected:

    private:

      const test::impl& _test;

  }; // class test::execution

  class subtest final
    : public test::impl {

    public:

      explicit subtest(const std::string& name_);

      template<typename T>
      const subtest& operator=(const T& body);

      void           run() const override;

      subtest&       plan(size_t p);

    protected:

    private:

  }; // class subtest

  namespace util {

    template<typename T>
    class is_iterable final {

      private:

        template<typename U>
        static auto test(int)->decltype(
          (void)(std::declval<
                   decltype(std::begin(std::declval<U>() ) )
                 >()
                 !=
                 std::declval<
                   decltype(std::end(std::declval<U>() ) )
                 >()
                 // Older versions of Clang complain about ‘inequality
                 // comparison result unused’ witout the following line 😒
                 ? 0 : 1),
          std::true_type()
        );

        template<typename>
        static auto test(...)->std::false_type;

        is_iterable() = default;

      public:

        static const bool value = decltype(test<T>(0) )::value;

        ~is_iterable() = default;

    }; // class is_iterable

    template<typename T>
    class is_streamable final {

      private:

        template<typename U>
        static auto test(int)->decltype(
          std::declval<std::ostream&>() << std::declval<U>(),
            std::true_type()
        );

        template<typename>
        static auto test(...)->std::false_type;

        is_streamable() = default;

      public:

        static const bool value = decltype(test<T>(0) )::value;

        ~is_streamable() = default;

    }; // class is_streamable

    template<typename T>
    class is_tuple final {

      private:

        template<typename U,
                 typename = typename std::enable_if<
                   !is_iterable<U>::value
                            >::type>
        static auto test(int)->decltype(
          std::tuple_size<U>::value,
          std::true_type()
        );

        template<typename>
        static auto test(...)->std::false_type;

        is_tuple() = default;

      public:

        static const bool value = decltype(test<T>(0) )::value;

        ~is_tuple() = default;

    }; // class is_tuple

    class stringifier final {

      public:

        stringifier() = default;

        template<typename value_t>
        std::string operator()(const value_t& value) const;

      private:

        template<typename value_t>
        struct convert final {

          std::string operator()(const value_t& value) const;

        }; // struct convert

        template<typename value_t>
        struct iterate final {

          std::string operator()(const value_t& value) const;

        }; // struct iterate

        template<typename value_t>
        struct pointer final {

          std::string operator()(const value_t& value) const;

        }; // struct pointer

        template<typename value_t>
        struct rtti final {

          std::string operator()(const value_t&) const;

        }; // struct rtti

        template<typename value_t>
        struct stream final {

          std::string operator()(const value_t& value) const;

        }; // struct stream

        template<typename value_t>
        struct tuple final {

          std::string operator()(const value_t& value) const;

          private:

            template<std::size_t I,
                     typename... Tp>
            static typename std::enable_if<I == sizeof ... (Tp),
                                           void>::type each(
              std::ostringstream&      /*oss*/,
              const std::tuple<Tp...>& /*t*/
            );

            template<std::size_t I,
                     typename... Tp>
            static void each(std::ostringstream&     oss,
                             const std::pair<Tp...>& value);

            template<std::size_t I,
                     typename... Tp>
            static typename std::enable_if < (I < sizeof ... (Tp) ),
            void > ::type each(std::ostringstream&      oss,
                               const std::tuple<Tp...>& value);

        }; // struct tuple

        template<typename value_t>
        using implementation_t
                = typename std::conditional<
                std::is_pointer<value_t>::value
                || std::is_same<value_t, std::nullptr_t>::value,
                typename bugeye::util::stringifier::pointer<value_t>,
                typename std::conditional<
                  std::is_arithmetic<value_t>::value,
                  typename bugeye::util::stringifier::stream<value_t>,
                  typename std::conditional<
                    std::is_assignable<std::string, value_t>::value,
                    typename bugeye::util::stringifier::convert<value_t>,
                    typename std::conditional<
                      bugeye::util::is_tuple<value_t>::value,
                      typename bugeye::util::stringifier::tuple<value_t>,
                      typename std::conditional<
                        bugeye::util::is_iterable<value_t>::value,
                        typename
                        bugeye::util::stringifier::iterate<value_t>,
                        typename std::conditional<
                          bugeye::util::is_streamable<value_t>::value,
                          typename bugeye::util::stringifier::stream<value_t>,
                          typename bugeye::util::stringifier::rtti<value_t>
                        >::type
                      >::type
                    >::type
                  >::type
                >::type
                  >::type;

    };

    std::string ansi(const std::string& spec,
                     const std::string& string);

    std::string ok_str(const bool ok);

    template<typename value_t>
    std::string        stringify(const value_t& value);

    static std::string vstringf(BUGEYE_FORMAT const char* format,
                                va_list                   args)
#  ifdef __GNUC__
    __attribute__( (format(printf, 1, 0) ) )
#  endif
    ;

  } // namespace util

  std::ostream& operator<<(std::ostream&            os,
                           const bugeye::assertion& assertion_);

  std::ostream& operator<<(std::ostream&             os,
                           const bugeye::location_t& location);

  void run(int          argc = 0,
           char const** argv = nullptr)
#  ifdef __GNUC__
  __attribute__ ( (noreturn) )
#  endif
  ;

} // namespace BugEye

// *****************************************************************************

// assertion

inline bugeye::assertion::assertion(
  const bool                                 ok_,
  const std::string&                         description_,
  const std::shared_ptr<bugeye::location_t>& location_,
  const directive_t                          directive_
)
  : description(description_),
  directive(directive_),
  directive_reason(),
  location(location_),
  number(0),
  _ok(ok_) {}

inline std::ostream& bugeye::assertion::stream_to(std::ostream& os) const {
  const std::string indentation(
    bugeye::test::execution::current().size() * 4,
    ' '
  );

  os << indentation << bugeye::util::ok_str(_ok);
  if (number > 0) {
    os << " " << number;
  }
  if (!description.empty() ) {
    os << " " << description;
  }
  if (directive != directive_t::none) {
    os << " # ";
    switch (directive) {
      case directive_t::skip:
        os << "skip";
        break;

      case directive_t::todo:
        os << "TODO";
        break;

      case directive_t::none:
#  ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wcovered-switch-default"
#  endif
      default:
#  ifdef __clang__
#    pragma clang diagnostic pop
#  endif
        break;
    } // switch
    if (!directive_reason.empty() ) {
      os << " " << directive_reason;
    }
  }
  os << std::endl;

  if (!ok() ) {
    os << indentation
       << "# Assertion failed";
    if (location) {
      os << " at " << *location;
    }
    os << std::endl;
  }

  return os;
} // bugeye::assertion::stream_to

inline bool bugeye::assertion::ok() const {
  return directive == directive_t::none
         ? _ok
         : true;
}

// bail_out_exception

inline bugeye::bail_out_exception::bail_out_exception(
  const location_t&  location_,
  const std::string& what_arg
)
  : std::runtime_error(what_arg),
  location(location_) {}

// config_error

inline bugeye::config_error::config_error(const std::string& what_arg)
  : std::logic_error(what_arg) {}

// location_t

inline bugeye::location_t::location_t(const std::string& file_,
                                      const size_t       line_)
  : file(file_),
  line(line_) {}

inline std::ostream& bugeye::location_t::stream_to(std::ostream& os) const {
  os << file << ":" << line;
  return os;
}

// execution

inline void bugeye::test::execution::bail_out_(
  const bugeye::location_t& location,
  BUGEYE_FORMAT const char* format,
  ...
) {
  va_list args;

  va_start(args, format);
  std::string message = bugeye::util::vstringf(format, args);
  va_end(args);

  throw bail_out_exception(location, message);
} // bugeye::test::execution::bail_out

inline std::stack<bugeye::test::execution>& bugeye::test::execution::current() {
  static std::stack<bugeye::test::execution> current;

  return current;
}

inline void bugeye::test::execution::diag_(BUGEYE_FORMAT const char* format,
                                           ...) {
  va_list           args;
  const std::string indentation(current().size() * 4, ' ');

  va_start(args, format);
  std::stringstream ss(bugeye::util::vstringf(format, args) );
  va_end(args);

  std::string line;
  while (std::getline(ss, line) ) {
    std::cout << indentation << "# " << line << std::endl;
  }
} // bugeye::test::execution::diag

inline void bugeye::test::execution::fail_(
  const bugeye::location_t& location,
  BUGEYE_FORMAT const char* format,
  ...) {
  va_list args;

  va_start(args, format);
  push(assertion(
         false,
         bugeye::util::vstringf(format, args),
         std::make_shared<location_t>(location)
       ) );
  va_end(args);
} // bugeye::test::execution::fail

inline bool bugeye::test::execution::ok_(
  const bugeye::location_t& location,
  bool                      expr,
  BUGEYE_FORMAT const char* format,
  ...) {
  va_list args;

  va_start(args, format);
  push(assertion(
         expr,
         bugeye::util::vstringf(format, args),
         std::make_shared<location_t>(location)
       ) );
  va_end(args);

  return expr;
} // bugeye::test::execution::ok

inline void bugeye::test::execution::pass_(
  const bugeye::location_t& location,
  BUGEYE_FORMAT const char* format,
  ...) {
  va_list args;

  va_start(args, format);
  push(assertion(
         true,
         bugeye::util::vstringf(format, args),
         std::make_shared<location_t>(location)
       ) );
  va_end(args);
} // bugeye::test::execution::pass

inline void bugeye::test::execution::push(assertion ar) {
  if (current().empty() ) {
    throw config_error(
            "No current execution to which to push assertion"
    );
  }

  execution& e = current().top();

  if (!e.directives.empty() ) {
    auto&& cd = e.directives.top();
    ar.directive        = cd.first;
    ar.directive_reason = cd.second;
  }
  ar.number = e.assertions.size() + 1;
  e.assertions.push_back(ar);

  if (!ar.ok() ) {
    e.end_result = end_result_t::not_ok;
  }

  std::cout << ar;
} // bugeye::test::execution::push

inline bool bugeye::test::execution::run(const test::impl& t) {
  current().emplace(t);
  execution& e  = current().top();
  bool       ok = e.run();
  current().pop();
  return ok;
} // bugeye::test::execution::run

inline bool bugeye::test::execution::run_all(int          argc,
                                             char const** argv) {
  typedef
    std::map<
      std::set<std::string>,
      std::function<void (std::function<std::string()> )>
    >
    options_t;

  std::unordered_set<std::string> explicit_tests;
  bool                            case_sensitive = true;
  bool                            ignore_missing = false;
  std::unordered_set<std::string> regexen;

  const options_t                 options = {
    {
      { "i", "case-insensitive" },
      [&](std::function<std::string()> )          {
        case_sensitive = false;
      }
    },
    {
      { "I", "ignore-missing"   },
      [&](std::function<std::string()> )          {
        ignore_missing = true;
      }
    },
    {
      { "l", "list"             },
      [&](std::function<std::string()> )          {
        for (const auto& t : tests() )            {
          std::cout << t.name << std::endl;
        }
        std::exit(int(exit_code::ok) );
      }
    },
    {
      { "r", "regex"            },
      [&](std::function<std::string()> get_value) {
        regexen.insert(get_value() );
      }
    },
  };

  const auto                      find_option
    = [&options](const std::string& option_name) {
        return std::find_if(
          options.begin(),
          options.end(),
          [&option_name](const options_t::value_type& v) {
      return v.first.find(option_name) != v.first.end();
    }
        );
      };

  for (int i = 1; i < argc; ++i) {
    const std::string arg(argv[i]);

    if (arg == "--") {
      for (++i; i < argc; ++i) {
        explicit_tests.emplace(argv[i]);
      }
    } else if ( (arg.size() >= 2) && (arg[0] == '-') && (arg[1] != '-') ) {
      for (size_t j = 1; j < arg.size(); j++) {
        const std::string option = arg.substr(j, 1);
        auto              it     = find_option(option);
        if (it == options.end() ) {
          throw bugeye::config_error("Unkown option ‘-" + option + "’");
        }
        it->second(
          [&]() -> std::string {
          if (j >= arg.size() - 1) {
            if (++i >= argc) {
              throw bugeye::config_error("‘-" + option + "’ requires a value");
            }
            return std::string(argv[i]);
          } else {
            std::string value = arg.substr(j + 1);
            j = arg.size();
            return value;
          }
        });
      }
    } else if ( (arg.size() >= 3) && (arg.substr(0, 2) == "--") ) {
      const std::string option = arg.substr(2);
      auto              it     = find_option(option);
      if (it == options.end() ) {
        throw bugeye::config_error("Unkown option ‘--" + option + "’");
      }
      it->second(
        [&]() -> std::string {
        if (++i >= argc) {
          throw bugeye::config_error("‘--" + option + "’ requires a value");
        }
        return std::string(argv[i]);
      });
    } else {
      explicit_tests.insert(arg);
    }
  }

  std::set<std::string> actual_tests;
  if (explicit_tests.empty()
      && regexen.empty() ) {
    std::transform(tests().begin(),
                   tests().end(),
                   std::inserter(actual_tests, actual_tests.begin() ),
                   [](const bugeye::test::impl& t) {
      return t.name;
    });
  } else {
    auto to_lower
      = [](std::string& s) {
          std::transform(s.begin(),
                         s.end(),
                         s.begin(),
                         [](char c) {
        return std::tolower(c, std::locale {});
      });
        };
    auto it = explicit_tests.begin();
    while (it != explicit_tests.end() ) {
      std::string explicit_test = *it;
      if (!case_sensitive) {
        to_lower(explicit_test);
      }
      bool matched = false;
      for (const auto& test : tests() ) {
        std::string name = test.name;
        if (!case_sensitive) {
          to_lower(name);
        }
        if (name == explicit_test) {
          matched = true;
          actual_tests.insert(test.name);
        }
      }
      if (matched) {
        it = explicit_tests.erase(it);
      } else {
        ++it;
      }
    }

    if (!ignore_missing
        && !explicit_tests.empty() ) {
      throw bugeye::config_error(
              "No test named ‘" + *(explicit_tests.begin() ) + "’"
      );
    }

    for (const auto& regex : regexen) {
      const std::regex_constants::syntax_option_type regex_options
        = case_sensitive
          ? std::regex::ECMAScript
          : std::regex::ECMAScript | std::regex::icase;
      bool matched = false;
      for (const auto& test : tests() ) {
        try {
          if (std::regex_match(test.name, std::regex(regex, regex_options) ) ) {
            matched = true;
            actual_tests.insert(test.name);
          }
        } catch (const std::regex_error& e) {
          throw bugeye::config_error(
                  "Invalid regex ‘" + regex + "’: " + e.what()
          );
        }
      }
      if (!matched
          && !ignore_missing) {
        throw bugeye::config_error(
                "No test matches /" + regex + "/" + (case_sensitive ? "" : "i")
        );
      }
    }
  }

  if (actual_tests.empty() ) {
    if (ignore_missing) {
      return true;
    }
    throw config_error("No tests to run");
  }

  size_t failed = 0;
  size_t done   = 0;

  std::cout << "1.." << actual_tests.size() << std::endl;

  // This is where it happens!
  for (auto& t : tests() ) {
    if (actual_tests.find(t.name) == actual_tests.end() ) {
      continue;
    }

    auto ok = run(t);
    ++done;
    std::cout
      << bugeye::util::ok_str(ok)
      << " "
      << done
      << " "
      << t.name
      << std::endl;

    if (!ok) {
      ++failed;
    }
  }

  if (failed > 0) {
    const auto total = actual_tests.size();
    std::cout << "# "
              << failed
              << " of "
              << total
              << " "
              << (total == 1 ? "test" : "tests")
              << " "
              << bugeye::util::ansi("1;31", "failed")
              << "!"
              << std::endl;
  } else {
    std::cout << "# All tests "
              << bugeye::util::ansi("1;32", "passed")
              << std::endl;
  }

  return failed == 0;
} // bugeye::test::execution::run_all

template<typename got_t,
         typename expected_t>
inline bool bugeye::test::execution::is_(
  const bugeye::location_t& location,
  const got_t&              got,
  const expected_t&         expected,
  BUGEYE_FORMAT const char* format,
  ...
) {
  va_list args;
  // TODO impl. numeric comparison
  bool    ok = (got == expected);

  va_start(args, format);
  push(assertion(
         ok,
         bugeye::util::vstringf(format, args),
         std::make_shared<location_t>(location)
       ) );
  va_end(args);

  if (!ok) {
    diag_("Got:      %s", bugeye::util::stringify(got).c_str() );
    diag_("Expected: %s", bugeye::util::stringify(expected).c_str() );
  }

  return ok;
} // bugeye::test::execution::is

template<typename got_t,
         typename unexpected_t>
inline bool bugeye::test::execution::isnt_(
  const bugeye::location_t& location,
  const got_t&              got,
  const unexpected_t&       unexpected,
  BUGEYE_FORMAT const char* format,
  ...
) {
  va_list args;
  bool    ok = (got != unexpected);

  va_start(args, format);
  push(assertion(
         ok,
         bugeye::util::vstringf(format, args),
         std::make_shared<location_t>(location)
       ) );
  va_end(args);

  if (!ok) {
    diag_("Got:        %s", bugeye::util::stringify(got).c_str() );
    diag_("Unexpected: %s", bugeye::util::stringify(unexpected).c_str() );
  }

  return ok;
} // bugeye::test::execution::isnt

inline void bugeye::test::execution::skip_(
  const bugeye::location_t& location,
  const size_t              how_many,
  BUGEYE_FORMAT const char* format,
  ...
) {
  va_list args;

  if (current().empty() ) {
    throw bugeye::config_error("No current execution");
  }

  va_start(args, format);
  const auto description = bugeye::util::vstringf(format, args);
  va_end(args);

  execution& e = current().top();
  e.directives.push({ directive_t::skip,
                      description });

  for (size_t i = 0; i < how_many; ++i) {
    e.push(assertion(
             true,
             description,
             std::make_shared<location_t>(location),
             directive_t::skip
           ) );
  }
  e.directives.pop();
} // bugeye::test::execution::skip

inline bool bugeye::test::execution::todo_start(
  BUGEYE_FORMAT const char* format,
  ...
) {
  va_list args;

  if (current().empty() ) {
    throw bugeye::config_error("No current execution");
  }

  execution& e = current().top();

  va_start(args, format);
  e.directives.push({ directive_t::todo,
                      bugeye::util::vstringf(format, args) });
  va_end(args);

  return true;
} // bugeye::test::execution::todo_start

inline bool bugeye::test::execution::todo_stop() {
  if (current().empty() ) {
    throw bugeye::config_error("No current execution");
  }

  execution& e = current().top();

  e.directives.pop();

  return false;
} // bugeye::test::execution::todo_stop

inline bugeye::test::execution::execution(const test::impl& test)
  : assertions(),
  directives(),
  end_result(end_result_t::none),
  _test(test) {}

inline bool bugeye::test::execution::run() {
  end_result = end_result_t::ok;

  const std::string indentation(current().size() * 4, ' ');

  if (_test._plan) {
    std::cout << indentation << "1.." << *_test._plan << std::endl;
  }
  try {
    _test.run();
  } catch (const bail_out_exception& e) {
    std::cout << indentation << bugeye::util::ansi("31", "Bail out!");
    std::string message(e.what() );

    if (!message.empty() ) {
      std::cout << " " << message;
    }
    std::cout << " At " << e.location << std::endl;
    std::exit(int(exit_code::tests_failed) );
  } catch (const config_error& e) {
    std::cerr << "Something went wrong: " << e.what()
              << std::endl;
    std::exit(int(exit_code::configuration) );
  } catch (const std::exception& e) {
    diag_("%s: %s",
          bugeye::util::ansi("31", "Caught exception").c_str(),
          e.what() );
    end_result = end_result_t::exception;
  } catch (...) {
    diag_("%s", bugeye::util::ansi("31", "Caught exception").c_str() );
    end_result = end_result_t::exception;
  }

  if (_test._plan) {
    auto actual  = assertions.size();
    auto planned = *_test._plan;
    if (actual < planned) {
      end_result = end_result_t::too_few;
      std::cout << indentation
                << "# Looks like you planned "
                << planned
                << " "
                << ( (planned == 1) ? "test" : "tests")
                << " but only "
                << actual
                << " "
                << (actual == 1 ? "was" : "were")
                << " run"
                << std::endl;
    } else if (actual > planned) {
      end_result = end_result_t::too_many;
      std::cout << indentation
                << "# Looks like you only planned "
                << planned
                << " "
                << ( (planned == 1) ? "test" : "tests")
                << " but "
                << actual
                << " "
                << (actual == 1 ? "was" : "were")
                << " run"
                << std::endl;
    }
  } else if (end_result != end_result_t::exception) {
    std::cout << indentation
              << "1.."
              << assertions.size()
              << std::endl;
  }

  return end_result == end_result_t::ok;
} // bugeye::test::execution::run

// test

inline bugeye::test::test(const std::string& name_)
  : name(name_),
  _plan(nullptr) {}

template<typename T>
inline bugeye::test& bugeye::test::operator=(T body) {
  static_assert(std::is_assignable<std::function<void()>, T>::value,
                "Test can only be assigned functions");

  if (std::find_if(tests().begin(),
                   tests().end(),
                   [this](const test::impl& i) {
    return i.name == this->name;
  }) != tests().end() ) {
    throw config_error("Duplicate test");
  }

  tests().emplace(*this, body);

  // GCC's `-Weffc++` doesn't seem to understand the `return *this` below. :(
#  ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Weffc++"
#  endif
  return *this;

#  ifdef __GNUC__
#    pragma GCC diagnostic pop
#  endif
} // =

inline bugeye::test& bugeye::test::plan(size_t p) {
  _plan.reset(new size_t(p) );
  return *this;
}

inline std::set<bugeye::test::impl>& bugeye::test::tests() {
  static std::set<bugeye::test::impl> set;

  return set;
}

// test::impl

template<typename body_t>
inline bugeye::test::impl::impl(const test&   test_,
                                const body_t& body)
  : test(test_),
  _body(body) {}

inline bool bugeye::test::impl::operator<(const impl& other) const {
  return name < other.name;
}

inline void bugeye::test::impl::run() const {
  _body();
}

inline bugeye::test::impl::impl(const std::string& name_)
  : test(name_),
  _body() {}

// subtest

inline bugeye::subtest::subtest(const std::string& name_)
  : test::impl(name_) {}

template<typename T>
inline const bugeye::subtest& bugeye::subtest::operator=(const T& body) {
  _body = body;
  bool ok = test::execution::run(*this);
  test::execution::push(assertion(ok, name) );

  // GCC's `-Weffc++` doesn't seem to understand the `return *this` below. :(
#  ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Weffc++"
#  endif
  return *this;

#  ifdef __GNUC__
#    pragma GCC diagnostic pop
#  endif
} // =

inline bugeye::subtest& bugeye::subtest::plan(size_t p) {
  _plan.reset(new size_t(p) );
  return *this;
}

inline void bugeye::subtest::run() const {
  _body();
}

// util

template<typename value_t>
inline std::string bugeye::util::stringifier::operator()(
  const value_t& value
) const {
  static const implementation_t<value_t> implementation {};

  return implementation(value);
}

template<typename value_t>
inline std::string bugeye::util::stringifier::convert<value_t >::operator()(
  const value_t& value
) const {
  return value;
}

template<typename value_t>
inline std::string bugeye::util::stringifier::iterate<value_t >::operator()(
  const value_t& value
) const {
  const auto         end = std::end(value);
  auto               it  = std::begin(value);

  std::ostringstream oss;

  oss << "[ ";
  while (it != end) {
    oss << bugeye::util::stringify(*it);
    if (++it != end) {
      oss << ", ";
    }
  }
  oss << " ]";
  return oss.str();
} // bugeye::util::stringifier::iterate<value_t>::operator()

template<typename value_t>
inline std::string bugeye::util::stringifier::pointer<value_t >::operator()(
  const value_t& value
) const {
  std::ostringstream oss;

  oss << "0x"
      << std::hex
      << std::setfill('0')
      << std::setw(sizeof(uintptr_t) * (CHAR_BIT / 4) )
      << reinterpret_cast<const uintptr_t>(value);
  return oss.str();
}

template<typename value_t>
inline std::string bugeye::util::stringifier::rtti<value_t >::operator()(
  const value_t& /*value*/
) const {
  return typeid(value_t).name();
}

template<typename value_t>
inline std::string bugeye::util::stringifier::stream<value_t >::operator()(
  const value_t& value
) const {
  typedef
    typename std::conditional <
    ( (!std::is_same<value_t, bool>::value)
    && std::is_integral<value_t>::value
    && (sizeof(value_t) < sizeof(long) ) ),
  long,
  const value_t&
  > ::type
  value_t2;

  std::ostringstream oss;
  value_t2           value2 = value;

  oss << std::boolalpha << value2;
  return oss.str();
} // bugeye::util::stringifier::stream<value_t>::operator()

template<typename value_t>
inline std::string bugeye::util::stringifier::tuple<value_t >::operator()(
  const value_t& value
) const {
  std::ostringstream oss;

  oss << "{ ";
  each<0>(oss, value);
  oss << " }";
  return oss.str();
}

template<typename value_t>
template<std::size_t I,
         typename... Tp>
inline typename std::enable_if<
  I == sizeof ... (Tp),
  void
>::type bugeye::util::stringifier::tuple<value_t >::each(
  std::ostringstream&      /*oss*/,
  const std::tuple<Tp...>& /*t*/
) {}

template<typename value_t>
template<std::size_t I,
         typename... Tp>
inline void bugeye::util::stringifier::tuple<value_t >::each(
  std::ostringstream&     oss,
  const std::pair<Tp...>& value
) {
  oss << bugeye::util::stringify(value.first)
      << ", "
      << bugeye::util::stringify(value.second);
}

template<typename value_t>
template<std::size_t I,
         typename... Tp>
inline typename std::enable_if
< (I < sizeof ... (Tp) ),
void
> ::type bugeye::util::stringifier::tuple<value_t >::each(
  std::ostringstream&      oss,
  const std::tuple<Tp...>& value
) {
  oss << bugeye::util::stringify(std::get<I>(value) );

#  ifdef _MSC_VER
  // MSVC bug: https://msdn.microsoft.com/en-us/library/2c8f766e.aspx
#    pragma warning(push)
#    pragma warning(disable: 4127)
#  endif

  if (I + 1 < sizeof ... (Tp) ) {
    oss << ", ";
  }

#  ifdef _MSC_VER
#    pragma warning(pop)
#  endif

  each<I + 1, Tp...>(oss, value);
} // bugeye::util::stringifier::tuple<value_t>::each

inline std::string bugeye::util::ansi(const std::string& spec,
                                      const std::string& string) {
  return std::string("\033[") + spec + "m" + string + "\033[0m";
}

inline std::string bugeye::util::ok_str(const bool ok) {
  std::string str    = ok ? "ok" : "not ok";
  // TODO configurable
  const char  colour = ok ? '2' : '1';

  return ansi(std::string("1;3") + colour, str);
}

template<typename value_t>
inline std::string bugeye::util::stringify(const value_t& value) {
  static const stringifier s {};

  return std::move(s(value) );
} // stringify

inline std::string bugeye::util::vstringf(BUGEYE_FORMAT const char* format,
                                          va_list                   args) {
  if (format == nullptr) {
    return "";
  }

  va_list args2;

  va_copy(args2, args);
  auto size = std::vsnprintf(nullptr, 0, format, args2) + 1;

  if (size < 0) {
    // cppcheck-suppress va_end_missing
    throw std::logic_error("std::vsnprintf() failed");
  }

  std::vector<char> buffer(size_t(size), 0x00);

  std::vsnprintf(buffer.data(), size_t(size), format, args);

  return std::string(buffer.data() );
} // bugeye::util::vstringf

// bugeye

inline std::ostream& bugeye::operator<<(std::ostream&            os,
                                        const bugeye::assertion& assertion_) {
  return assertion_.stream_to(os);
}

inline std::ostream& bugeye::operator<<(std::ostream&             os,
                                        const bugeye::location_t& location) {
  return location.stream_to(os);
}

inline void bugeye::run(int          argc,
                        char const** argv) {
  try {
    std::exit(int(bugeye::test::execution::run_all(argc, argv)
                  ? exit_code::ok
                  : exit_code::tests_failed) );
  } catch (const bugeye::config_error& e) {
    std::cerr << bugeye::util::ansi("31", e.what() ) << std::endl;
    std::exit(int(exit_code::configuration) );
  }
}

// *****************************************************************************

#  define BUGEYE_CONCAT_(a, b) a##b
#  define BUGEYE_CONCAT(a, b)  BUGEYE_CONCAT_(a, b)

#  define OK(...)                             \
  ::bugeye::test::execution::ok_(             \
    ::bugeye::location_t(__FILE__, __LINE__), \
    __VA_ARGS__                               \
  )

#  define IS(...)                             \
  ::bugeye::test::execution::is_(             \
    ::bugeye::location_t(__FILE__, __LINE__), \
    __VA_ARGS__                               \
  )

#  define ISNT(...)                           \
  ::bugeye::test::execution::isnt_(           \
    ::bugeye::location_t(__FILE__, __LINE__), \
    __VA_ARGS__                               \
  )

#  define PASS(...)                           \
  ::bugeye::test::execution::pass_(           \
    ::bugeye::location_t(__FILE__, __LINE__), \
    __VA_ARGS__                               \
  )

#  define FAIL(...)                           \
  ::bugeye::test::execution::fail_(           \
    ::bugeye::location_t(__FILE__, __LINE__), \
    __VA_ARGS__                               \
  )

#  define DIAG(...) \
  ::bugeye::test::execution::diag_(__VA_ARGS__)

#  define TODO(...)                                                            \
  for (bool _bugeye_todo = ::bugeye::test::execution::todo_start(__VA_ARGS__); \
       _bugeye_todo;                                                           \
       _bugeye_todo = ::bugeye::test::execution::todo_stop() )

#  define SKIP(CONDITION, ...)                  \
  if (CONDITION) {                              \
    ::bugeye::test::execution::skip_(           \
      ::bugeye::location_t(__FILE__, __LINE__), \
      __VA_ARGS__                               \
    );                                          \
  }                                             \
  else

#  define BAIL_OUT(...)                       \
  ::bugeye::test::execution::bail_out_(       \
    ::bugeye::location_t(__FILE__, __LINE__), \
    __VA_ARGS__                               \
  )

#else // if defined(TEST) || defined(_TEST)

namespace bugeye {

  struct test final {

    explicit test(const std::string&);

    test(const test& other) = default;

    ~test()                 = default;

    test& plan(size_t);

    template<typename T>
    test& operator=(T);

    test& operator=(const test& other) = delete;

  };

  struct subtest final {

    explicit subtest(const std::string&);

    subtest& plan(size_t);

    template<typename T>
    subtest& operator=(T);

    subtest& operator=(const test& other) = delete;

  };

  void run(int = 0,
           char const** = nullptr);

  template<typename... args_t>
  void unused(args_t...);

} // namespace bugeye

inline bugeye::test::test(const std::string&) {}

inline bugeye::test& bugeye::test::plan(size_t) {
  return *this;
}

// GCC's `-Weffc++` doesn't seem to understand the `return *this`s below. :(
#  ifdef __GNUC__
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Weffc++"
#  endif

template<typename T>
inline bugeye::test& bugeye::test::operator=(T) {
  return *this;
}

inline bugeye::subtest::subtest(const std::string&) {}

inline bugeye::subtest& bugeye::subtest::plan(size_t) {
  return *this;
}

template<typename T>
inline bugeye::subtest& bugeye::subtest::operator=(T) {
  return *this;
}

#  ifdef __GNUC__
#    pragma GCC diagnostic pop
#  endif

inline void bugeye::run(int, char const**) {}

template<typename... args_t>
inline void bugeye::unused(args_t...) {}

#  define OK(...)       { ::bugeye::unused(__VA_ARGS__); }
#  define IS(...)       { ::bugeye::unused(__VA_ARGS__); }
#  define ISNT(...)     { ::bugeye::unused(__VA_ARGS__); }
#  define DIAG(...)     { ::bugeye::unused(__VA_ARGS__); }
#  define PASS(...)     { ::bugeye::unused(__VA_ARGS__); }
#  define FAIL(...)     { ::bugeye::unused(__VA_ARGS__); }
#  define SKIP(...)     { ::bugeye::unused(__VA_ARGS__); }
#  define TODO(...)     { ::bugeye::unused(__VA_ARGS__); }
#  define BAIL_OUT(...) { ::bugeye::unused(__VA_ARGS__); }

#endif // if defined(TEST) || defined(_TEST)

#if defined(BUGEYE_LOWERCASE) && BUGEYE_LOWERCASE
#  define bail_out BAIL_OUT
#  define diag     DIAG
#  define fail     FAIL
#  define is       IS
#  define isnt     ISNT
#  define ok       OK
#  define pass     PASS
#  define skip     SKIP
#  define todo     TODO
#endif

#endif // ifndef BUGEYE3_H
