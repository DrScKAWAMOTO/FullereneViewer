#
# Project: FullereneViewer
# Version: 1.0
# Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
# Create: 2014/05/25 17:00:00 JST
#

MDTOP= ../../../README.md
HTMLTOP= $(subst .md,.html,$(subst ../../../,,$(MDTOP)))
MDDOC= $(wildcard ../../../doc/*.md)
HTMLDOC= $(subst .md,.html,$(subst ../../../,,$(MDDOC)))

.PHONY: all clean distclean

all:  $(HTMLTOP) $(HTMLDOC)

$(HTMLTOP): %.html: ../../../%.md
	pandoc -s -f markdown_github -t html $< | sed -e s/ja\.md/ja.html/g > $@

$(HTMLDOC): doc/%.html: ../../../doc/%.md
	pandoc -s -f markdown_github -t html $< | sed -e s/README\\.md/README.html/g | sed -e s/ja\\.md/ja.html/g > $@

clean:
	rm -f $(HTMLTOP) $(HTMLDOC)
