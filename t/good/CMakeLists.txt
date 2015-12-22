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
