noinst_PROGRAMS = publisher subscriberreliable subscriberbesteffort

if CONFIG_ORTE_MINGW
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a
endif

publisher_SOURCES = publisher.c
publisher_LDADD   = $(top_builddir)/orte/liborte/liborte.la $(win_libs)
publisher_LDFLAGS = -static

subscriberreliable_SOURCES = subscriberreliable.c
subscriberreliable_LDADD   = $(top_builddir)/orte/liborte/liborte.la $(win_libs)
subscriberreliable_LDFLAGS = -static

subscriberbesteffort_SOURCES = subscriberbesteffort.c
subscriberbesteffort_LDADD   = $(top_builddir)/orte/liborte/liborte.la $(win_libs)
subscriberbesteffort_LDFLAGS = -static
