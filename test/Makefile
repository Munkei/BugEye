
compiler   = g++
flags      = -I ./../
src        = Foo.cpp
test-exe   = Foo_TEST.out
normal-exe = Foo.out

all : normal-run test-run

normal-compile :
	${compiler} ${flags} ${src} -o ${normal-exe}

normal-run : normal-compile
	./${normal-exe}

test-compile :
	${compiler} -D_TEST ${flags} ${src} -o ${test-exe}

test-run : test-compile
	./${test-exe}
