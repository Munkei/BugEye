cmake_minimum_required( VERSION 3.0.0 )

include( lib/cmake/VersionFromGit/VersionFromGit.cmake )

version_from_git(
  LOG       ON
  TIMESTAMP "%Y%m%d%H%M%S"
)

project( BugEye
  VERSION   ${VERSION}
  LANGUAGES CXX
)

set( CMAKE_COLOR_MAKEFILE        ON )
set( CMAKE_CXX_STANDARD          11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_VERBOSE_MAKEFILE      ON )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

enable_testing()

find_program( ccache ccache )
if( ccache )
  set_property( GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${ccache} )
  set_property( GLOBAL PROPERTY RULE_LAUNCH_LINK    ${ccache} )
endif( ccache)

add_custom_target( TAGS
  COMMAND           ctags --langmap=c++:+.h *.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target( statistics
  COMMAND           cloc
                      --by-file-by-lang
                      --exclude-dir lib/
                      .
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_subdirectory( t )

################################################################################

find_program( washer washer )
if( washer )
  add_test(
    NAME              washer
    COMMAND           ${washer} -v
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif( washer )

################################################################################

find_program( cppcheck cppcheck )
if( cppcheck )
  message( STATUS "Using Cppcheck: ${cppcheck}" )
  set( cppcheck_template "ERROR: [{file}:{line}] ({severity}:{id}) {message}" )
  add_test(
    NAME              cppcheck
    COMMAND           ${cppcheck}
                        --enable=all
                        --error-exitcode=2
                        --force
                        -I ${PROJECT_SOURCE_DIR}
                        -i lib/
                        --inconclusive
                        --inline-suppr
                        --std=c++11
                        --suppress=missingIncludeSystem
                        --template=${cppcheck_template}
                        .
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif( cppcheck )

################################################################################

install(
  FILES
    BugEye3.h
    CHANGES.md
    LICENSE_1_0.txt
    README.md
  DESTINATION .
)

set( CPACK_GENERATOR                   TGZ;ZIP                                 )
set( CPACK_INCLUDE_TOPLEVEL_DIRECTORY  1                                       )
set( CPACK_MONOLITHIC_INSTALL          1                                       )
set( CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/README.md" )
set( CPACK_PACKAGE_VERSION_MAJOR       ${PROJECT_VERSION_MAJOR}                )
set( CPACK_PACKAGE_VERSION_MINOR       ${PROJECT_VERSION_MINOR}                )
set( CPACK_PACKAGE_VERSION_PATCH       ${PROJECT_VERSION_PATCH}                )
set( CPACK_PACKAGE_FILE_NAME           BugEye-v${SEMVER}                       )

include( CPack )
