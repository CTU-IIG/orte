#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04
#

LIBORTE_FILES = \
cdr.c \
conv.c \
debug.c \
event.c \
fnmatch.c \
htimerNtp.c \
objectEntry.c \
objectEntryTimer.c \
objectUtils.c \
ORTEAppRecvThread.c \
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
ORTEMisc.c \
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

LIBORTE_HDRS1 = \
../include/orte.h 

LIBORTE_HDRS2 = \
$(top_builddir)/orte/include/orte/orte_config.h \
../include/orte/cdr.h \
../include/orte/basic_types.h \
../include/orte/typedefs_defines_rtps.h \
../include/orte/defines_api.h \
../include/orte/typedefs_api.h \
../include/orte/protos_api.h 

if CONFIG_ORTE_RT
# --------------- KERNEL SPACE ---------------
module_PROGRAMS = orte_rt.ko
orte_rt_ko_SOURCES = $(LIBORTE_FILES)
orte_rt_ko_LINK = $(top_builddir)/modtool --link -o $@
if USE_RTLINUX
SUBDIRS = rtl
endif
else
# --------------- USER  SPACE ---------------
lib_LTLIBRARIES = liborte.la
liborte_la_SOURCES = $(LIBORTE_FILES)
liborte_la_LDFLAGS = \
    -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
    -release $(LT_RELEASE) \
    -export-dynamic 
#    $(LDADDS)
liborte_la_CFLAGS = $(AM_CFLAGS)
endif

# headers
liborte1dir=$(includedir)
liborte1_HEADERS= $(LIBORTE_HDRS1)
liborte2dir=$(includedir)/orte
liborte2_HEADERS= $(LIBORTE_HDRS2)

EXTRA_DIST = usedSections.txt Makefile Makefile.omk \
msvc pharlap

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
