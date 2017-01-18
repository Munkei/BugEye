remove_definitions( -DTEST )

add_executable( notests
  ../bad/failures.cpp
  ../bad/toofew.cpp
  ../bad/toomany.cpp
  ../bailout/bailout.cpp
  ../../BugEye3.h
  ../exception/exception.cpp
  ../good/good.cpp
  ../good/goodnoplan.cpp
  ../main.cpp
  )

add_test(
  NAME    notests
  COMMAND notests
  )

set_tests_properties( notests
  PROPERTIES PASS_REGULAR_EXPRESSION "This is the code that runs when we're not testing"
  )

coverage_target( notests )
