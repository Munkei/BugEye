// Copyright Theo Willows 2013–2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye3.h>

#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wunreachable-code"
#endif

#ifdef _MSC_VER
#  pragma warning(disable: 4127)
#endif

static auto t = bugeye::test("Good").plan(11) = [] {
  OK(true);

  IS( (1 + 1), 2);

  IS(std::string("foo"), "foo");

  PASS("pass");

  DIAG("Diagnostic message with numbers: %d (8), %d (10), %d (16)",
       010,
       10,
       0x10);

  bugeye::subtest("subtest").plan(2) = [] {
    OK(true);
    DIAG("Diagnostic message in a sub-test");
    OK(true);
  };

  TODO("These aren't done yet") {
    OK(false, "false");

    FAIL("fail");
  }

  SKIP(true, 2, "skip these") {
    OK(false, "false");

    FAIL("fail");
  }

  SKIP(false, 2, "do *not* skip these") {
    OK(true, "false");

    PASS("pass");
  }
};

#ifdef TEST

struct A {};

static auto t2 = bugeye::test("Self").plan(1) = [] {
  bugeye::subtest("stringify") = [] {
    const auto nullptr_str = bugeye::util::stringify(nullptr);
    DIAG("stringify(nullptr) → %s", nullptr_str.c_str() );
    IS(nullptr_str.substr(0, 2), "0x", "nullptr begins with ‘0x’");
    IS(nullptr_str.back(), '0', "nullptr ends with ‘0’");

    const auto void_ptr_str
      = bugeye::util::stringify(reinterpret_cast<void*>(0x1234) );
    DIAG("stringify(void*) → %s", void_ptr_str.c_str() );
    IS(void_ptr_str.substr(0, 2), "0x", "void* begins with ‘0x’");
    IS(void_ptr_str.substr(void_ptr_str.size() - 4),
       "1234",
       "void* ends with ‘1234’");

    const int  x           = 42;
    const int* int_ptr     = &x;
    const auto int_ptr_str = bugeye::util::stringify(int_ptr);
    DIAG("stringify(int*) → %s", int_ptr_str.c_str() );
    IS(int_ptr_str.substr(0, 2), "0x", "int* begins with ‘0x’");

    IS(bugeye::util::stringify(1.2f), "1.2", "float");
    IS(bugeye::util::stringify(1.2), "1.2", "double");
    IS(bugeye::util::stringify(1.2l), "1.2", "long double");

    IS(bugeye::util::stringify(true), "true", "bool");

    IS(bugeye::util::stringify<char>('A'), "65", "char");
    IS(bugeye::util::stringify<signed char>('A'), "65", "signed char");
    IS(bugeye::util::stringify<unsigned char>('A'), "65", "unsigned char");

    IS(bugeye::util::stringify<char16_t>('A'), "65", "char16_t");
    IS(bugeye::util::stringify<char32_t>('A'), "65", "char32_t");
    IS(bugeye::util::stringify<wchar_t>('A'), "65", "wchar_t");

    IS(bugeye::util::stringify<short>(42), "42", "short");
    IS(bugeye::util::stringify<int>(42), "42", "int");
    IS(bugeye::util::stringify<int>(-42), "-42", "int (negative)");
    IS(bugeye::util::stringify<long>(42), "42", "long");
    IS(bugeye::util::stringify<long long>(42), "42", "long long");

    IS(bugeye::util::stringify<unsigned short>(42), "42", "unsigned short");
    IS(bugeye::util::stringify<unsigned int>(42), "42", "unsigned int");
    IS(bugeye::util::stringify<unsigned long>(42), "42", "unsigned long");
    IS(bugeye::util::stringify<unsigned long long>(42),
       "42",
       "unsigned long long");

    const A a {};
    OK(!bugeye::util::is_streamable<A>::value);
    OK(!bugeye::util::stringify(a).empty(), "stringify(A) does *something*");

    IS(bugeye::util::stringify(std::vector<int> { 1, 2, 3 }),
       "[ 1, 2, 3 ]",
       "vector<int>");

    IS(bugeye::util::stringify(std::pair<unsigned short,
                                         std::string> { 42, "foo" }),
       "{ 42, foo }",
       "pair<unsigned short, string>");
    IS(bugeye::util::stringify(std::tuple<bool,
                                          unsigned short,
                                          std::string> { false, 42, "bar" }),
       "{ false, 42, bar }",
       "tuple<bool, unsigned short, string>");

    IS(bugeye::util::stringify(
         std::vector<
           std::tuple<bool,
                      unsigned short,
                      std::string>
         >({
      std::tuple<bool,
                 unsigned short,
                 std::string> { true, 3, "foo" },
      std::tuple<bool,
                 unsigned short,
                 std::string> { false, 42, "bar" },
    })
       ),
       "[ { true, 3, foo }, { false, 42, bar } ]",
       "vector<tuple<bool, unsigned short, string> >");
  };
};

#endif // ifdef TEST
