cmake_minimum_required( VERSION 3.0.0 )

include( MunkeiAddCompileFlags )
include( MunkeiValgrind        )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" )

  add_compile_flags(
    REQUIRED
    # LOG YES
    -Wall
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Werror
    -Wextra
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
    -g
    -pedantic
    -pedantic-errors
    )

  add_compile_flags(
    # LOG YES
    -Qunused-arguments
    -Wconversion
    -Wcovered-switch-default
    -Wdisabled-macro-expansion
    -Wdouble-promotion
    -Wduplicated-branches
    -Wduplicated-cond
    -Weffc++
    -Weverything
    -Wformat-signedness
    -Wgcc-compat
    -Wjump-misses-init
    -Wlogical-op
    -Wmissing-noreturn
    -Wno-c++98-compat-pedantic
    -Wno-exit-time-destructors
    -Wno-format-nonliteral
    -Wno-global-constructors
    -Wno-padded
    -Wno-weak-vtables
    -Wnoexcept
    -Wnull-dereference
    -Wold-style-cast
    -Wreserved-id-macro
    -Wrestrict
    -Wshadow
    -Wstrict-null-sentinel
    -Wsuggest-attribute=const
    -Wsuggest-attribute=format
    -Wsuggest-attribute=noreturn
    -Wsuggest-attribute=pure
    -Wsuggest-final-methods
    -Wsuggest-final-types
    -Wsuggest-override
    -Wswitch
    -Wswitch-enum
    -Wuseless-cast
    -Wzero-as-null-pointer-constant
    -fdiagnostics-color=always
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
    /w44061
    /w44062
    /w44263
    /w44264
    /w44266
    /w44296
    /w44545
    /w44546
    /w44547
    /w44548
    /w44549
    /w44574
    /w44668
    )

  add_definitions(
    /D_CRT_SECURE_NO_WARNINGS
    /D_UNICODE
    /DUNICODE
    )

else()

  message( WARNING
    "Using an unsupported compiler (${CMAKE_CXX_COMPILER_ID}). Good luck!"
    )

endif()

add_definitions(
  -DTEST
  )

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
