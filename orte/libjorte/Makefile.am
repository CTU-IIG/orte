#
#  Makefile for the Ocera Real-Time Ethernet
#
#  $Id: Makefile.am,v 1.0 2003/08/04
#

LIBJORTE_FILES = \
onLoad.c \
getNtpTime.c \
JGetORTEConstant.c \
JIPAddressToString.c \
JNtpTimeToStringMs.c \
JNtpTimeToStringUs.c \
JORTEDomainAppCreate.c \
JORTEDomainAppDestroy.c \
JORTEDomainEvents.c \
JORTEDomainEventsDestroy.c \
JORTEDomainInitEvents.c \
JORTEDomainMgrCreate.c \
JORTEDomainMgrDestroy.c \
JORTEDomainPropDefaultGet.c \
JORTEDomainPropSet.c \
JORTEDomainPropDestroy.c \
JORTEDomainStart.c \
JORTEInit.c \
JORTEPublicationCreate.c \
JORTEPublicationDestroy.c \
JORTEPublicationSend.c \
JORTERecvInfoHelpers.c \
JORTESleepMs.c \
JORTESubscriptionCreate.c \
JORTESubscriptionDestroy.c \
JORTESubscriptionPull.c \
JORTETypeRegisterAdd.c \
JStringToIPAddress.c

lib_LTLIBRARIES = libjorte.la
libjorte_la_SOURCES = $(LIBJORTE_FILES)
libjorte_la_LDFLAGS = \
    -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) \
    -export-dynamic 
libjorte_la_CFLAGS = $(AM_CFLAGS)
libjorte_la_LIBADD = $(top_builddir)/orte/liborte/liborte.la 

EXTRA_DIST = Makefile Makefile.omk \
createAppInfo.c createPubInfo.c createSubInfo.c \
JORTEPublicationGetStatus.c \
JORTEPublicationPropertiesGet.c \
JORTEPublicationPropertiesSet.c \
JORTEPublicationWaitForSubscriptions.c \
JORTESubscriptionGetStatus.c \
JORTESubscriptionPropertiesGet.c \
JORTESubscriptionPropertiesSet.c \
JORTESubscriptionWaitForPublications.c \
JORTETypeRegisterDestroyAll.c \
JORTEVerbositySetLogFile.c \
JORTEVerbositySetOptions.c \
setTopic.c setType.c

dist-hook:
	cp -f  $(srcdir)/Makefile $(distdir)/Makefile
