#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04
#

LIBJORTE_FILES = \
getNtpTime.c \
JGetORTEConstant.c \
JIPAddressToString.c \
JNtpTimeToStringMs.c \
JNtpTimeToStringUs.c \
JORTEDomainAppCreate.c \
JORTEDomainAppDestroy.c \
JORTEDomainInitEvents.c \
JORTEDomainMgrCreate.c \
JORTEDomainMgrDestroy.c \
JORTEDomainPropDefaultGet.c \
JORTEDomainStart.c \
JORTEInit.c \
JORTEPublicationCreate.c \
JORTEPublicationDestroy.c \
JORTEPublicationSend.c \
JORTESleepMs.c \
JORTESubscriptionCreate.c \
JORTESubscriptionDestroy.c \
JORTETypeRegisterAdd.c \
JStringToIPAddress.c

lib_LTLIBRARIES = libjorte.la
libjorte_la_SOURCES = $(LIBJORTE_FILES)
libjorte_la_LDFLAGS = \
    -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
    -release $(LT_RELEASE) \
    -export-dynamic 
libjorte_la_CFLAGS = $(AM_CFLAGS)
libjorte_la_LIBADD = $(top_builddir)/orte/liborte/liborte.la 




