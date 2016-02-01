add_executable( good
  ../../BugEye3.h
  ../main.cpp
  good.cpp
  goodnoplan.cpp
)

add_test(
  NAME    good
  COMMAND good
)

add_test(
  NAME    good_cla_1
  COMMAND good Good
)

add_test(
  NAME    good_cla_2
  COMMAND good Good Self
)

add_test(
  NAME    good_cla_3
  COMMAND good -ir "[gs].*"
)

add_test(
  NAME    good_cla_4
  COMMAND good -I asdf -- -x
)

add_test(
  NAME    good_cla_5
  COMMAND good --list
)

coverage_target( good )
