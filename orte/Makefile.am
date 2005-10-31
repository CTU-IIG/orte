#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04 
#
#  Uncomment and customize the following to ORTE your needs:
#

SUBDIRS	= liborte manager examples 

if CONFIG_JORTE
SUBDIRS += libjorte
endif

if CONFIG_ORTE_IDL
SUBDIRS += idl-compiler
endif

EXTRA_DIST = Makefile Makefile.omk \
contrib include java templates

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
