CXX           = g++
MARKDOWN      = markdown
SHELL         = /bin/sh
bugeye_header = BugEye.h
changes       = CHANGES.md
dist          = BugEye-$(version).tar.gz
doc_html      = $(doc_src:%.md=%.html)
doc_src       = $(changes) $(readme)
license       = LICENSE_1_0.txt
readme        = README.md
version       = $(strip $(shell echo 'BUGEYE_VERSION' | $(CXX) -x c++ -include "$(bugeye_header)" -E -P -) )

# Default is to check
all : check

check : check_doc
	$(MAKE) -C t/ $@

# Makes sure the documentation looks good
check_doc :
	grep -q "Documentation\s\+for\s\+version\s\+$(version)\." "$(readme)"
	@echo "$(readme): Is for this version (\`$(version)')"
	grep -q "^## $(version) ##$$" "$(changes)"
	@echo "$(changes): Contains this version (\`$(version)')"

# Removes all generated files
clean :
	rm -fv "$(dist)" "$(doc_html)"
	$(MAKE) -C t/ $@

# Creates a tarball for distribution, containing the header file, the license
# and the documentation
dist : check version $(dist)
	@echo "Package: \`$(dist)'"

# Makes sure documentation is good
doc : check_doc html

html : $(doc_html)

# Prints the version ("parsed" from the header file)
version :
	@echo "Version: \`$(version)'"

.PHONY : all check check_doc clean dist doc version

%.html : %.md
	echo '<html>\n<title>BugEye $(version)</title>\n<body>' > "$@"
	$(MARKDOWN) "$<" >> "$@"
	echo '</body></html>' >> "$@"

# TODO With newer version of Pandoc, use:
# pandoc \
#  --from markdown_github \
#  --to html5+ignore_line_breaks \
#  --output "$@" \
#  --standalone \
#  --toc \
#  --highlight-style tango \
#  --self-contained \
#  --section-divs \
#  "$<"

$(dist) : $(bugeye_header) $(doc_src) $(doc_html) $(license)
	@rm -f "$(dist)"
	tar \
  --create \
  --auto-compress \
  --file "$(dist)" \
  --verbose \
  $^
