#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04
#

LIBORTE_SOURCES = \
conv.c \
debug.c \
event.c \
fnmatch.c \
htimerNtp.c \
objectEntry.c \
objectEntryTimer.c \
objectUtils.c \
ORTEAppRecvMetatrafficThread.c \
ORTEAppRecvUserdataThread.c \
ORTEAppSendThread.c \
ORTEDomainApp.c \
ORTEPublication.c \
ORTEPublicationTimer.c \
ORTESubscription.c \
ORTEDomain.c \
ORTEDomainMgr.c \
ORTEInit.c \
ORTEThreadUtils.c \
ORTETypeRegister.c \
ORTEVerbosity.c \
ORTEPattern.c \
parameter.c \
RTPSAck.c \
RTPSCSTReader.c \
RTPSCSTReaderTimer.c \
RTPSCSTReaderProc.c \
RTPSCSTWriter.c \
RTPSCSTWriterTimer.c \
RTPSGap.c \
RTPSHeader.c \
RTPSHeardBeat.c \
RTPSInfoDST.c \
RTPSInfoREPLY.c \
RTPSInfoSRC.c \
RTPSInfoTS.c \
RTPSPad.c \
RTPSUtils.c \
RTPSVar.c \
RTPSIssue.c \
sock.c \
ul_gavl.c \
ul_gavlprim.c

if CONFIG_ORTE_RT
module_PROGRAMS = orte.ko
orte_ko_CFLAGS = $(ORTE_CFLAGS) $(LINUX_CFLAGS) $(RTAI_CFLAGS) $(RTLINUX_CFLAGS)
orte_ko_SOURCES = $(LIBORTE_SOURCES)
orte_ko_LINK = $(top_builddir)/modtool --link -o $@

SUBDIRS = rtl
else
lib_LIBRARIES = liborte.a
liborte_a_SOURCES = $(LIBORTE_SOURCES)
endif

EXTRA_DIST = usedSections.txt
