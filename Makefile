#
# Project: FullereneViewer
# Version: 1.0
# Copyright: (C) 2011-14 Dr.Sc.KAWAMOTO,Takuji (Ext)
# Create: 2014/05/25 17:00:00 JST
#

PROJECT= FullereneViewer
VERSION= DATE-TIME

DATE= $(shell date +20%y%m%d-%H%M%S)
ARCH= $(PROJECT)-$(DATE).tar.gz

.PHONY: all project version
.PHONY:	Qt install depend tags dist clean Qtclean distclean spike jerry

all:
	touch src/Depend.mk
	cd src; $(MAKE) depend
	cd src; $(MAKE) all

project:
	@echo $(PROJECT)

version:
	@echo $(VERSION)

Qt:
	touch src/Depend.mk
	cd src; $(MAKE) depend
	cd src; $(MAKE) Qt

install:
	touch src/Depend.mk
	cd src; $(MAKE) depend
	cd src; $(MAKE) install

depend:
	touch src/Depend.mk
	cd src; $(MAKE) depend

tags:
	touch src/Depend.mk
	cd src; $(MAKE) tags

dist:
	rm -rf ../dist/$(PROJECT)
	mkdir -p ../dist/$(PROJECT)
	echo $(ARCH) > archive.name
	cp -prf * ../dist/$(PROJECT)
	(cd ../dist/$(PROJECT); $(MAKE) distclean)
	touch archive.name ../dist/$(PROJECT)/archive.name
	(cd ../dist; tar cvfz ../`cat $(PROJECT)/archive.name` $(PROJECT))
	rm -rf ../dist/$(PROJECT)

clean:
	rm -f *.core *~ *.bak *~ */*~ */*.bak
	touch src/Depend.mk
	cd src; $(MAKE) clean

Qtclean:
	touch src/Depend.mk
	cd src; $(MAKE) Qtclean

distclean: clean
	touch src/Depend.mk
	cd src; $(MAKE) distclean
