cmake_minimum_required( VERSION 3.0.0 )

include( MunkeiAddCompileFlags )
include( MunkeiValgrind        )

set( CMAKE_VERBOSE_MAKEFILE ON )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" )
  add_compile_flags(
    REQUIRED
    # LOG YES
    -g
    -pedantic
    -pedantic-errors
    -Wall
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Werror
    -Wextra
    # -Wfatal-errors
    -Wfloat-equal
    -Wformat=2
    -Winit-self
    -Wmissing-declarations
    -Wmissing-format-attribute
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

  add_compile_flags(
    # LOG YES
    -fdiagnostics-color=always
    -Qunused-arguments
    -Wcovered-switch-default
    -Wdisabled-macro-expansion
    -Wdouble-promotion
    -Weffc++
    -Wformat-signedness
    -Wgcc-compat
    -Wlogical-op
    -Wmissing-noreturn
    -Wno-c++98-compat-pedantic
    -Wno-format-nonliteral
    -Wnoexcept
    -Wreserved-id-macro
    -Wstrict-null-sentinel
    -Wsuggest-attribute=const
    -Wsuggest-attribute=format
    -Wsuggest-attribute=noreturn
    -Wsuggest-attribute=pure
    -Wsuggest-final-methods
    -Wsuggest-final-types
    -Wsuggest-override
    -Wswitch-enum
    -Wuseless-cast
    -Wzero-as-null-pointer-constant
  )
elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
  add_compile_flags(
    # Displays the full path of source code files
    /FC
    # Enables run-time type information (RTTI).
    /GR
    /MDd
    # Builds multiple source files concurrently.
    /MP
    /Od
    /W4
    /WX
    # Generates complete debugging information.
    /Zi
  )
  add_definitions(
    /D_CRT_SECURE_NO_WARNINGS
    /D_UNICODE
    /D_USE_ATTRIBUTES_FOR_SAL=1
    /DUNICODE
  )
else()
  message( WARNING
    "Using an unsupported compiler (${CMAKE_CXX_COMPILER_ID}). Good luck!"
  )
endif()

add_definitions( -D_TEST )

include_directories(
  ..
)

add_subdirectory( notests   )

add_subdirectory( bad       )
add_subdirectory( bailout   )
add_subdirectory( exception )
add_subdirectory( good      )

valgrind(
  TARGETS
    # bad
    # bailout
    # exception
    good
    notests
)
