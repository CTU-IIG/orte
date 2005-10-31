noinst_PROGRAMS = m_subscriber

m_subscriber_SOURCES = m_subscriber.c
m_subscriber_LDADD   = -L$(top_builddir)/orte/liborte/ -lorte 

EXTRA_DIST = Makefile Makefile.omk \
msvc

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
