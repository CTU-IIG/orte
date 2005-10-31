if CONFIG_ORTE_RT
  module_PROGRAMS = h_subscriber_rt.ko h_publisher_rt.ko
  h_subscriber_rt_ko_SOURCES = h_subscriber.c  
  h_subscriber_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
  h_publisher_rt_ko_SOURCES = h_publisher.c  
  h_publisher_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
else
  noinst_PROGRAMS = h_publisher h_subscriber
  h_publisher_SOURCES = h_publisher.c
  h_publisher_LDADD   = -L$(top_builddir)/orte/liborte/ -lorte 
  h_subscriber_SOURCES = h_subscriber.c
  h_subscriber_LDADD   = -L$(top_builddir)/orte/liborte/ -lorte 
endif

EXTRA_DIST = Makefile Makefile.omk \
msvc

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
