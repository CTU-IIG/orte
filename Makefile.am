#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04 
#
AUTOMAKE_OPTIONS = foreign

if USE_RTLINUX
SUBDIRS	= orte RTL_UDP
else
SUBDIRS	= orte 
endif

EXTRA_DIST = AUTHORS COPYING ChangeLog INSTALL README TODO orte.kdevelop

ACLOCAL_AMFLAGS = -I admin
