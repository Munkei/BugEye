cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_COLOR_MAKEFILE ON )

include( lib/cmake/VersionFromGit/VersionFromGit.cmake )

version_from_git(
  LOG       ON
  TIMESTAMP "%Y%m%d%H%M%S"
)

project( BugEye
  VERSION   ${VERSION}
  LANGUAGES CXX
)

set( CMAKE_VERBOSE_MAKEFILE ON )

enable_testing()

find_program ( got_ccache ccache )
if( got_ccache )
  set_property( GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache )
  set_property( GLOBAL PROPERTY RULE_LAUNCH_LINK    ccache )
endif( got_ccache)

add_custom_target( TAGS
  COMMAND           ctags --langmap=c++:+.h *.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target( statistics
  COMMAND           cloc *.h
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_subdirectory( t )

################################################################################

find_program ( got_washer washer )
if ( got_washer )
  add_test(
    NAME              washer
    COMMAND           washer -v
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endif ()

################################################################################

install(
  FILES
    BugEye.h
    CHANGES.md
    LICENSE_1_0.txt
    README.md
  DESTINATION .
)

set( CPACK_GENERATOR                   TGZ;ZIP                                       )
set( CPACK_INCLUDE_TOPLEVEL_DIRECTORY  1                                             )
set( CPACK_MONOLITHIC_INSTALL          1                                             )
set( CPACK_PACKAGE_DESCRIPTION_FILE    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"       )
set( CPACK_PACKAGE_VERSION_MAJOR       ${PROJECT_VERSION_MAJOR}                      )
set( CPACK_PACKAGE_VERSION_MINOR       ${PROJECT_VERSION_MINOR}                      )
set( CPACK_PACKAGE_VERSION_PATCH       ${PROJECT_VERSION_PATCH}                      )
set( CPACK_PACKAGE_FILE_NAME           BugEye-v${SEMVER}                             )
INCLUDE( CPack )
