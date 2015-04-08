LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE    := orte
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS += -Wall
LOCAL_SRC_FILES := \
liborte/conv.c \
liborte/debug.c \
liborte/event.c \
liborte/fnmatch.c \
liborte/htimerNtp.c \
liborte/objectEntry.c \
liborte/objectEntryTimer.c \
liborte/objectUtils.c \
liborte/ORTEAppRecvThread.c \
liborte/ORTEAppSendThread.c \
liborte/ORTEDomainApp.c \
liborte/ORTEPublication.c \
liborte/ORTEPublicationTimer.c \
liborte/ORTESubscription.c \
liborte/ORTEDomain.c \
liborte/ORTEDomainMgr.c \
liborte/ORTEInit.c \
liborte/ORTEThreadUtils.c \
liborte/ORTETypeRegister.c \
liborte/ORTEVerbosity.c \
liborte/ORTEPattern.c \
liborte/ORTEMisc.c \
liborte/parameter.c \
liborte/RTPSAck.c \
liborte/RTPSCSTReader.c \
liborte/RTPSCSTReaderTimer.c \
liborte/RTPSCSTReaderProc.c \
liborte/RTPSCSTWriter.c \
liborte/RTPSCSTWriterTimer.c \
liborte/RTPSGap.c \
liborte/RTPSHeader.c \
liborte/RTPSHeardBeat.c \
liborte/RTPSInfoDST.c \
liborte/RTPSInfoREPLY.c \
liborte/RTPSInfoSRC.c \
liborte/RTPSInfoTS.c \
liborte/RTPSPad.c \
liborte/RTPSUtils.c \
liborte/RTPSVar.c \
liborte/RTPSIssue.c \
liborte/sock.c \
liborte/ul_gavl.c \
liborte/ul_gavlprim.c \
liborte/cdr.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := jorte
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS += -Wall 
LOCAL_SRC_FILES := \
libjorte/onLoad.c \
libjorte/getNtpTime.c \
libjorte/setType.c \
libjorte/setTopic.c \
libjorte/createAppInfo.c \
libjorte/createPubInfo.c \
libjorte/createSubInfo.c \
libjorte/JGetORTEConstant.c \
libjorte/JIPAddressToString.c \
libjorte/JNtpTimeToStringMs.c \
libjorte/JNtpTimeToStringUs.c \
libjorte/JORTEDomainAppCreate.c \
libjorte/JORTEDomainAppDestroy.c \
libjorte/JORTEDomainEvents.c \
libjorte/JORTEDomainEventsDestroy.c \
libjorte/JORTEDomainInitEvents.c \
libjorte/JORTEDomainMgrCreate.c \
libjorte/JORTEDomainMgrDestroy.c \
libjorte/JORTEDomainPropDefaultGet.c \
libjorte/JORTEDomainPropSet.c \
libjorte/JORTEDomainPropDestroy.c \
libjorte/JORTEDomainStart.c \
libjorte/JORTEInit.c \
libjorte/JORTEPublicationCreate.c \
libjorte/JORTEPublicationDestroy.c \
libjorte/JORTEPublicationSend.c \
libjorte/JORTEPublicationPropertiesSet.c \
libjorte/JORTEPublicationPropertiesGet.c \
libjorte/JORTEPublicationGetStatus.c \
libjorte/JORTEPublicationWaitForSubscriptions.c \
libjorte/JORTERecvInfoHelpers.c \
libjorte/JORTESleepMs.c \
libjorte/JORTESubscriptionCreate.c \
libjorte/JORTESubscriptionDestroy.c \
libjorte/JORTESubscriptionPropertiesGet.c \
libjorte/JORTESubscriptionPropertiesSet.c \
libjorte/JORTESubscriptionPull.c \
libjorte/JORTESubscriptionGetStatus.c \
libjorte/JORTESubscriptionWaitForPublications.c \
libjorte/JORTETypeRegisterAdd.c \
libjorte/JORTETypeRegisterDestroyAll.c \
libjorte/JORTEVerbositySetLogFile.c \
libjorte/JORTEVerbositySetOptions.c \
libjorte/JStringToIPAddress.c

LOCAL_STATIC_LIBRARIES := orte

include $(BUILD_SHARED_LIBRARY)

