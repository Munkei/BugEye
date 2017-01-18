set( html   "${CMAKE_CURRENT_BINARY_DIR}/MANUAL.html" )
set( manual "${CMAKE_SOURCE_DIR}/MANUAL.md"                  )

add_custom_command(
  OUTPUT "${html}"
  COMMAND pandoc
  --from markdown
  --to html5
  --output "${html}"
  "${manual}"
  MAIN_DEPENDENCY "${manual}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
  VERBATIM
  )

add_custom_target( doc
  DEPENDS "${html}"
  SOURCES "${manual}"
  )

install(
  FILES
  "${html}"
  "${manual}"
  DESTINATION .
  COMPONENT   doc
  # EXCLUDE_FROM_ALL
  )
