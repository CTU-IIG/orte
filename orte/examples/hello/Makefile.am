if CONFIG_ORTE_RT
  module_PROGRAMS = helloSubscriber.ko helloPublisher.ko
  helloSubscriber_ko_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
  helloSubscriber_ko_SOURCES = HelloWorldSubscriber.c  
  helloSubscriber_ko_LINK = $(top_builddir)/modtool --link -o $@
  helloPublisher_ko_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
  helloPublisher_ko_SOURCES = HelloWorldPublisher.c  
  helloPublisher_ko_LINK = $(top_builddir)/modtool --link -o $@
else
  noinst_PROGRAMS = HelloWorldPublisher HelloWorldSubscriber
if CONFIG_ORTE_WIN
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a
endif
  HelloWorldPublisher_SOURCES = HelloWorldPublisher.c
  HelloWorldPublisher_LDADD   = ../../liborte/liborte.a $(win_libs)
  HelloWorldSubscriber_SOURCES = HelloWorldSubscriber.c
  HelloWorldSubscriber_LDADD   = ../../liborte/liborte.a $(win_libs)
endif