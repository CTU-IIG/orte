#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04 
#
AUTOMAKE_OPTIONS = foreign

SUBDIRS	= orte 

ACLOCAL_AMFLAGS = -I admin

EXTRA_DIST = authors copying ChangeLog install readme \
readme.makerules Makefile Makefile.omk Makefile.cvs Makefile.rules OCERA_TOP_DIR-ocera \
kernelcfg2mk switch2standalone todo \
doc

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
	rm -rf `find $(distdir) -name CVS`
