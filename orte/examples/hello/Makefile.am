if CONFIG_ORTE_RT
  module_PROGRAMS = h_subscriber_rt.ko h_publisher_rt.ko
  h_subscriber_rt_ko_SOURCES = h_subscriber.c  
  h_subscriber_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
  h_publisher_rt_ko_SOURCES = h_publisher.c  
  h_publisher_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
else
  noinst_PROGRAMS = h_publisher h_subscriber
  h_publisher_SOURCES = h_publisher.c
  h_publisher_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
  h_publisher_LDFLAGS = -static
  h_subscriber_SOURCES = h_subscriber.c
  h_subscriber_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
  h_subscriber_LDFLAGS = -static
endif