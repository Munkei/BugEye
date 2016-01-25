add_executable( bailout
  ../../BugEye3.h
  ../main.cpp
  bailout.cpp
)

add_test(
  NAME    bailout
  COMMAND bailout
)

set_tests_properties( bailout
  PROPERTIES WILL_FAIL YES
)

coverage_target( bailout )
