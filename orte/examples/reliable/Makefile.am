noinst_PROGRAMS = r_publisher r_subscriber_reliable r_subscriber_besteffort

r_publisher_SOURCES = r_publisher.c
r_publisher_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
r_publisher_LDFLAGS = -static

r_subscriber_reliable_SOURCES = r_subscriber_reliable.c
r_subscriber_reliable_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
r_subscriber_reliable_LDFLAGS = -static

r_subscriber_besteffort_SOURCES = r_subscriber_besteffort.c
r_subscriber_besteffort_LDADD   = $(top_builddir)/orte/liborte/liborte.la 
r_subscriber_besteffort_LDFLAGS = -static
