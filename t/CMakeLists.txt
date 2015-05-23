cmake_minimum_required( VERSION 3.0.0 )

include( CheckCXXCompilerFlag )

project( BugEye-tests
  LANGUAGES CXX
)

set( CMAKE_VERBOSE_MAKEFILE ON )

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" )
  add_compile_options(
    -g
    -pedantic
    -Wall
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Werror
    -Wextra
    -Wfatal-errors
    -Wformat=2
    -Winit-self
    -Wmissing-declarations
    -Wmissing-include-dirs
    -Wno-error=deprecated-declarations
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wshadow
    -Wsign-compare
    -Wsign-conversion
    -Wsign-promo
    -Wstrict-overflow=5
    -Wswitch-default
    -Wundef
  )

  list( APPEND CMAKE_CXX_FLAGS --std=c++11 )

  if ( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    add_compile_options(
      -Wlogical-op
      -Wnoexcept
      -Wstrict-null-sentinel
    )
  endif ()

  if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    add_compile_options(
      -Qunused-arguments
      -Wno-format-nonliteral
    )
  endif ()

elseif ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
  add_compile_options(
    /FC     # Displays the full path of source code files passed to cl.exe in diagnostic text.
    /GR     # Enables run-time type information (RTTI).
    /MP     # Builds multiple source files concurrently.
    /W4
    /WX
    /Za     # Disables language extensions.
    /Zi     # Generates complete debugging information.
  )
  add_definitions(
    /D_CRT_SECURE_NO_WARNINGS
    /D_UNICODE
    /D_USE_ATTRIBUTES_FOR_SAL=1
    /DUNICODE
  )
else ()
  message( WARNING
    "Using an unsupported compiler (${CMAKE_CXX_COMPILER_ID}). Good luck!"
  )
endif ()

check_cxx_compiler_flag( -fdiagnostics-color=always compiler_colour )
if ( compiler_colour )
  add_compile_options(
    -fdiagnostics-color=always
  )
endif ()

include_directories(
  ..
)

################################################################################

add_executable( good
  Good.cpp
  GoodNoPlan.cpp
  main.cpp
  Named.cpp
)

target_compile_definitions( good PUBLIC _TEST )

add_test(
  NAME    good
  COMMAND good
)

################################################################################

add_executable( bad
  Failures.cpp
  main.cpp
  TooFew.cpp
  TooMany.cpp
)

target_compile_definitions( bad PUBLIC _TEST )

add_test(
  NAME    bad
  COMMAND bad
)

set_tests_properties( bad
  PROPERTIES WILL_FAIL YES
)

################################################################################

add_executable( bailout
  BailOut.cpp
  main.cpp
)

target_compile_definitions( bailout PUBLIC _TEST )

add_test(
  NAME    bailout
  COMMAND bailout
)

set_tests_properties( bailout
  PROPERTIES WILL_FAIL YES
)

################################################################################

add_executable( exception
  Exception.cpp
  main.cpp
)

target_compile_definitions( exception PUBLIC _TEST )

add_test(
  NAME    exception
  COMMAND exception
)

set_tests_properties( exception
  PROPERTIES WILL_FAIL YES
)

################################################################################

add_executable( notests
  main.cpp
)

add_test(
  NAME    notests
  COMMAND notests
)

set_tests_properties( notests
  PROPERTIES PASS_REGULAR_EXPRESSION "This is the code that runs when we're not testing"
)
