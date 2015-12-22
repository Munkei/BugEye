// Copyright Theo Willows 2013-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <BugEye3.h>

static auto t = bugeye::test("Good").plan(11) = [] {
  ok(true);

  is( (1 + 1), 2);

  is(std::string("foo"), "foo");

  pass("pass");

  diag("Diagnostic message with numbers: %d (8), %d (10), %d (16)",
       010,
       10,
       0x10);

  bugeye::subtest("subtest").plan(2) = [] {
    ok(true);
    diag("Diagnostic message in a sub-test");
    ok(true);
  };

  todo("These aren't done yet") {
    ok(false, "false");

    fail("fail");
  }

  skip(true, 2, "skip these") {
    ok(false, "false");

    fail("fail");
  }

  skip(false, 2, "do *not* skip these") {
    ok(true, "false");

    pass("pass");
  }
};

#ifdef _TEST

struct A {};

static auto t2 = bugeye::test("Self").plan(1) = [] {
  bugeye::subtest("stringify") = [] {
    const auto nullptr_str = bugeye::util::stringify(nullptr);
    diag("stringify(nullptr) → %s", nullptr_str.c_str() );
    is(nullptr_str.substr(0, 2), "0x", "nullptr begins with ‘0x’");
    is(nullptr_str.back(), '0', "nullptr ends with ‘0’");

    const auto void_ptr_str
      = bugeye::util::stringify(reinterpret_cast<void*>(0x1234) );
    diag("stringify(void*) → %s", void_ptr_str.c_str() );
    is(void_ptr_str.substr(0, 2), "0x", "void* begins with ‘0x’");
    is(void_ptr_str.substr(void_ptr_str.size() - 4),
       "1234",
       "void* ends with ‘1234’");

    const int  x           = 42;
    const int* int_ptr     = &x;
    const auto int_ptr_str = bugeye::util::stringify(int_ptr);
    diag("stringify(int*) → %s", int_ptr_str.c_str() );
    is(int_ptr_str.substr(0, 2), "0x", "int* begins with ‘0x’");

    is(bugeye::util::stringify(1.2f), "1.2", "float");
    is(bugeye::util::stringify(double(1.2) ), "1.2", "double");
    is(bugeye::util::stringify<long double>(1.2), "1.2", "long double");

    is(bugeye::util::stringify(true), "true", "bool");

    is(bugeye::util::stringify<char>('A'), "65", "char");
    is(bugeye::util::stringify<signed char>('A'), "65", "signed char");
    is(bugeye::util::stringify<unsigned char>('A'), "65", "unsigned char");

    is(bugeye::util::stringify<char16_t>('A'), "65", "char16_t");
    is(bugeye::util::stringify<char32_t>('A'), "65", "char32_t");
    is(bugeye::util::stringify<wchar_t>('A'), "65", "wchar_t");

    is(bugeye::util::stringify<short>(42), "42", "short");
    is(bugeye::util::stringify<int>(42), "42", "int");
    is(bugeye::util::stringify<int>(-42), "-42", "int (negative)");
    is(bugeye::util::stringify<long>(42), "42", "long");
    is(bugeye::util::stringify<long long>(42), "42", "long long");

    is(bugeye::util::stringify<unsigned short>(42), "42", "unsigned short");
    is(bugeye::util::stringify<unsigned int>(42), "42", "unsigned int");
    is(bugeye::util::stringify<unsigned long>(42), "42", "unsigned long");
    is(bugeye::util::stringify<unsigned long long>(42),
       "42",
       "unsigned long long");

    const A a;
    ok(!bugeye::util::is_streamable<A>::value);
    ok(!bugeye::util::stringify(a).empty(), "stringify(A) does *something*");

    is(bugeye::util::stringify(std::vector<int> { 1, 2, 3 }),
       "[ 1, 2, 3 ]",
       "vector<int>");

    is(bugeye::util::stringify(std::pair<unsigned short,
                                         std::string> { 42, "foo" }),
       "{ 42, foo }",
       "pair<unsigned short, string>");
    is(bugeye::util::stringify(std::tuple<bool,
                                          unsigned short,
                                          std::string> { false, 42, "bar" }),
       "{ false, 42, bar }",
       "tuple<bool, unsigned short, string>");

    is(bugeye::util::stringify(
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

#endif // ifdef _TEST
