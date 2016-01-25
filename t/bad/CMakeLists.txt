add_executable( bad
  ../../BugEye3.h
  ../main.cpp
  failures.cpp
  toofew.cpp
  toomany.cpp
)

add_test(
  NAME    bad
  COMMAND bad
)

set_tests_properties( bad
  PROPERTIES WILL_FAIL YES
)

coverage_target( bad )
