bin_PROGRAMS = orteping

orteping_SOURCES = orteping.c
orteping_LDADD   = -L$(top_builddir)/orte/liborte/ -lorte

EXTRA_DIST = Makefile Makefile.omk \
msvc

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
