noinst_PROGRAMS = publisher subscriberreliable subscriberbesteffort

if CONFIG_ORTE_MINGW
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a
endif

publisher_SOURCES = publisher.c
publisher_LDADD   = ../../liborte/liborte.a $(win_libs)

subscriberreliable_SOURCES = subscriberreliable.c
subscriberreliable_LDADD   = ../../liborte/liborte.a $(win_libs)

subscriberbesteffort_SOURCES = subscriberbesteffort.c
subscriberbesteffort_LDADD   = ../../liborte/liborte.a $(win_libs)
