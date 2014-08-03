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
SMEDIAS= $(wildcard ../../../media/*.png)
TMEDIAS= $(subst ../../../,,$(SMEDIAS))

.PHONY: all clean distclean

all:  $(HTMLTOP) $(HTMLDOC) $(TMEDIAS)

$(HTMLTOP): %.html: ../../../%.md
	pandoc -s -f markdown_github -t html $< | sed -e s/ja\.md/ja.html/g > $@

$(HTMLDOC): doc/%.html: ../../../doc/%.md
	pandoc -s -f markdown_github -t html $< | sed -e s/README\\.md/README.html/g | sed -e s/ja\\.md/ja.html/g > $@

$(TMEDIAS): media/%.png: ../../../media/%.png
	cp -f $< $@

clean:
	rm -f $(HTMLTOP) $(HTMLDOC) $(TMEDIAS)
