add_executable( exception
  ../../BugEye3.h
  ../main.cpp
  exception.cpp
  )

add_test(
  NAME    exception
  COMMAND exception
  )

set_tests_properties( exception
  PROPERTIES WILL_FAIL YES
  )

coverage_target( exception )
