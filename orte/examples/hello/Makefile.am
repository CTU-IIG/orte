if CONFIG_ORTE_RT
  module_PROGRAMS = HelloWorldSubscriber_rt.ko HelloWorldPublisher_rt.ko
  HelloWorldSubscriber_rt_ko_SOURCES = HelloWorldSubscriber.c  
  HelloWorldSubscriber_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
  HelloWorldPublisher_rt_ko_SOURCES = HelloWorldPublisher.c  
  HelloWorldPublisher_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
else
  noinst_PROGRAMS = HelloWorldPublisher HelloWorldSubscriber
if CONFIG_ORTE_MINGW
  win_libs= $(top_srcdir)/orte/contrib/win_pthread/libpthreadGC.a
endif
  HelloWorldPublisher_SOURCES = HelloWorldPublisher.c
  HelloWorldPublisher_LDADD   = $(top_builddir)/orte/liborte/liborte.la $(win_libs)
  HelloWorldPublisher_LDFLAGS = -static
  HelloWorldSubscriber_SOURCES = HelloWorldSubscriber.c
  HelloWorldSubscriber_LDADD   = $(top_builddir)/orte/liborte/liborte.la $(win_libs)
  HelloWorldSubscriber_LDFLAGS = -static
endif