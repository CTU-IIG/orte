#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04 
#
#  Uncomment and customize the following to ORTE your needs:
#
if CONFIG_ORTE_RT
SUBDIRS = hello 
else
SUBDIRS	= hello multicast reliable ping spy schneider mpo
endif

EXTRA_DIST = Makefile Makefile.omk \
type

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
