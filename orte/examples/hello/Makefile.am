if CONFIG_ORTE_RT
  module_PROGRAMS = rt_HelloWorldSubscriber.ko rt_HelloWorldPublisher.ko
  rt_HelloWorldSubscriber_ko_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
  rt_HelloWorldSubscriber_ko_SOURCES = HelloWorldSubscriber.c  
  rt_HelloWorldSubscriber_ko_LINK = $(top_builddir)/modtool --link -o $@
  rt_HelloWorldPublisher_ko_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
  rt_HelloWorldPublisher_ko_SOURCES = HelloWorldPublisher.c  
  rt_HelloWorldPublisher_ko_LINK = $(top_builddir)/modtool --link -o $@
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