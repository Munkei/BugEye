cmake_minimum_required( VERSION 3.0.0 )

include( CheckCXXCompilerFlag )

set( CMAKE_VERBOSE_MAKEFILE ON )

if( CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU" )
  add_compile_options(
    -g
    -pedantic
    -Wall
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Werror
    -Wextra
    # -Wfatal-errors
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

  if( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
    add_compile_options(
      -Wlogical-op
      -Wnoexcept
      -Wstrict-null-sentinel
      -Wsuggest-attribute=const
      -Wsuggest-attribute=format
      -Wsuggest-attribute=noreturn
      -Wsuggest-attribute=pure
    )
  endif()

  if( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    add_compile_options(
      -Qunused-arguments
      -Wno-format-nonliteral
    )
  endif()

elseif( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
  add_compile_options(
    /FC # Displays the full path of source code files passed to cl.exe in diagnostic text.
    /GR # Enables run-time type information (RTTI).
    /MP # Builds multiple source files concurrently.
    /MTd
    /Od
    /W4
    /WX
    /Za # Disables language extensions.
    /Zi # Generates complete debugging information.
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

check_cxx_compiler_flag( -fdiagnostics-color=always compiler_colour )
if( compiler_colour )
  add_compile_options(
    -fdiagnostics-color=always
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
