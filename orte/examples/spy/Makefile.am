bin_PROGRAMS = ortespy

ortespy_SOURCES = ortespy.c
ortespy_LDADD   = -L$(top_builddir)/orte/liborte/ -lorte

EXTRA_DIST = Makefile Makefile.omk \
msvc

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
