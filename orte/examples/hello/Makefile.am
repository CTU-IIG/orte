if CONFIG_ORTE_RT
  module_PROGRAMS = HelloWorldSubscriber_rt.ko HelloWorldPublisher_rt.ko
  HelloWorldSubscriber_rt_ko_SOURCES = HelloWorldSubscriber.c  
  HelloWorldSubscriber_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
  HelloWorldPublisher_rt_ko_SOURCES = HelloWorldPublisher.c  
  HelloWorldPublisher_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
else
  noinst_PROGRAMS = HelloWorldPublisher HelloWorldSubscriber
  HelloWorldPublisher_SOURCES = HelloWorldPublisher.c
  HelloWorldPublisher_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
  HelloWorldPublisher_LDFLAGS = -static
  HelloWorldSubscriber_SOURCES = HelloWorldSubscriber.c
  HelloWorldSubscriber_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
  HelloWorldSubscriber_LDFLAGS = -static
endif