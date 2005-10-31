noinst_PROGRAMS = schneider_subscriber

schneider_subscriber_SOURCES = schneider_subscriber.c
schneider_subscriber_LDADD   = -L$(top_builddir)/orte/liborte/ -lorte 

EXTRA_DIST = Makefile Makefile.omk 

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
