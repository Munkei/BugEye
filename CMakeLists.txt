cmake_minimum_required( VERSION 3.0.0 )

include( lib/cmake/munkei-cmake/Munkei.cmake )

include( MunkeiCcache         )
include( MunkeiCoverage       )
include( MunkeiCppcheck       )
include( MunkeiStatistics     )
include( MunkeiTags           )
include( MunkeiVersionFromGit )
include( MunkeiWasher         )

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

if( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )
  enable_coverage()
endif( CMAKE_CXX_COMPILER_ID MATCHES "GNU" )

ccache()
cppcheck(
  FORCE
  IGNORE
    lib/
  INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}
  INCONCLUSIVE
  SUPPRESS
    missingIncludeSystem
)
statistics(
  EXCLUDE_DIRS
    lib
  OPTIONS
    --by-file-by-lang
)
tags()
washer()

add_subdirectory( t )

################################################################################

install(
  FILES
    BugEye3.h
    CHANGELOG.md
    LICENSE.md
    MANUAL.md
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
