CCACHE            = ccache
CCACHE_VERSION   := $(shell $(CCACHE) --version 2>/dev/null)
CXXFLAGS         += \
	-Wall \
	-Wextra \
	-Werror \
	-Wfatal-errors \
	-pedantic-errors \
	$(call compiler_supports_option,-fdiagnostics-color=always) \
	--std=c++11 \
	$(addprefix -I,$(includedirs))
FIND              = find
SHELL             = /bin/sh
deps_test         = $(objects_test:%.o=%.d)
deps_wo_test      = $(objects_wo_test:%.o=%.d)
expected_failures = 4
includedirs       = ..
objects_test      = $(addprefix $(outdir_test)/,$(srcs:$(srcdir)/%=%.o))
objects_wo_test   = $(addprefix $(outdir_wo_test)/,$(srcs:$(srcdir)/%=%.o))
outdir            = out
outdir_test       = $(outdir)/test
outdir_wo_test    = $(outdir)/wo_test
srcdir            = src
srcs             := $(shell $(FIND) $(srcdir)/ -type f -iname '*.cpp')
test_exe          = $(outdir)/test.exe
wo_test_exe       = $(outdir)/wo_test.exe

compiler_supports_option = $(strip $(shell echo | gcc -fsyntax-only "$(1)" -xc - 2>/dev/null && echo "$(1)" || echo ))

ifdef CCACHE_VERSION
	CXX := $(CCACHE) $(CXX)
endif

ifneq "$(findstring clang,$(CXX))" ""
	CXXFLAGS += -Qunused-arguments
endif

all : check

check : run_test run_wo_test

clean :
	rm -frv "$(outdir)"

compile : $(test_exe) $(wo_test_exe)

run_test : $(test_exe)
	$(test_exe) ; \
		[ $$? -eq $(expected_failures) ]

run_wo_test : $(wo_test_exe)
	$(wo_test_exe)

.PHONY : all check clean compile run_test run_wo_test

$(outdir) $(outdir_test) $(outdir_wo_test) :
	mkdir -pv "$@"

$(outdir_test)/%.o : $(srcdir)/% | $(outdir_test)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MD -MF "$(@:%.o=%.d)" -D_TEST -c "$<" -o "$@"

$(outdir_wo_test)/%.o : $(srcdir)/% | $(outdir_wo_test)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MD -MF "$(@:%.o=%.d)" -c "$<" -o "$@"

$(test_exe) : $(objects_test)
	$(CXX) $(LDFLAGS) $(objects_test) $(LOADLIBES) $(LDLIBS) -o $(test_exe)

$(wo_test_exe) : $(objects_wo_test)
	$(CXX) $(LDFLAGS) $(objects_wo_test) $(LOADLIBES) $(LDLIBS) -o $(wo_test_exe)

-include $(deps_test)
-include $(deps_wo_test)
