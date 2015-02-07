#
# Project: FullereneViewer
# Version: 1.0
# Copyright: (C) 2011-15 Dr.Sc.KAWAMOTO,Takuji (Ext)
#

SOURCES= $(wildcard [A-Z]*.cc)
OBJECTS= $(subst .cc,.o,$(SOURCES))
MAINS= $(wildcard [a-z]*-*.cc)
MBJECTS= $(subst .cc,.o,$(MAINS))
TARGETS= $(subst .cc,,$(MAINS))

QMAKE= qmake

INCLUDES= -I/usr/local/include -I/usr/local/include/drm
CXXFLAGS= -g -Wall -Wno-unused-parameter -D_THREAD_SAFE $(INCLUDES) -pthread
LDFLAGS= -L/usr/local/lib -L/usr/X11/lib
LOADLIBES= -lGLU -lGL
INSTALL= install
INSTALLDIR= ~/bin/$(shell uname -s).$(shell uname -m)

SYSTEM= $(shell uname)
CLANGTEST= $(shell which clang)
ifeq ($(CLANGTEST),)
  CC= gcc
  CXX= g++
else
  CC= clang
  CXX= clang++
endif

.PHONY:	all Qtmakefile Qt install depend tags dist clean Qtclean distclean

all: depend $(TARGETS)

$(TARGETS): %: %.o $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

Qtmakefile:
	cd Qt; $(QMAKE) FullereneViewer.pro

Qt: Qtmakefile
ifeq ($(SYSTEM),Darwin)
	echo "Click folder FullereneViewer.xcodeproj by finder."
else
	cd Qt; $(MAKE)
endif

install: all
	$(INSTALL) -cp $(TARGETS) $(INSTALLDIR)

depend:
	$(CXX) -M $(INCLUDES) $(MAINS) $(SOURCES) > Depend.mk

tags:
	gtags -v

clean: Qtclean
	rm -f $(OBJECTS) $(MBJECTS) $(TARGETS) *.ini *.pov *.png
	rm -f *.core *~ *.bak */*~ */*.bak GPATH GRTAGS GTAGS ID
	rm -f *.shape *.bmp
	rm -rf **/axes **/char **/dmat

Qtclean: Qtmakefile
	cd Qt; $(MAKE) clean

distclean: clean
	cd Qt; $(MAKE) distclean

include Depend.mk
