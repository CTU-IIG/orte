noinst_PROGRAMS = ReliablePublisher ReliableSubscriber BestEffortSubscriber

if CONFIG_ORTE_WIN
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a
endif

ReliablePublisher_SOURCES = ReliablePublisher.c
ReliablePublisher_LDADD   = ../../liborte/liborte.a $(win_libs)

ReliableSubscriber_SOURCES = ReliableSubscriber.c
ReliableSubscriber_LDADD   = ../../liborte/liborte.a $(win_libs)

BestEffortSubscriber_SOURCES = BestEffortSubscriber.c
BestEffortSubscriber_LDADD   = ../../liborte/liborte.a $(win_libs)
