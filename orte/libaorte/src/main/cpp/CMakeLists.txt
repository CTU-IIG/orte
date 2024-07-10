# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("orte")

include_directories("include")
# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
add_library("orte" STATIC
        liborte/conv.c
        liborte/debug.c
        liborte/event.c
        liborte/fnmatch.c
        liborte/htimerNtp.c
        liborte/objectEntry.c
        liborte/objectEntryTimer.c
        liborte/objectUtils.c
        liborte/ORTEAppRecvThread.c
        liborte/ORTEAppSendThread.c
        liborte/ORTEDomainApp.c
        liborte/ORTEPublication.c
        liborte/ORTEPublicationTimer.c
        liborte/ORTESubscription.c
        liborte/ORTEDomain.c
        liborte/ORTEDomainMgr.c
        liborte/ORTEInit.c
        liborte/ORTEThreadUtils.c
        liborte/ORTETypeRegister.c
        liborte/ORTEVerbosity.c
        liborte/ORTEPattern.c
        liborte/ORTEMisc.c
        liborte/parameter.c
        liborte/RTPSAck.c
        liborte/RTPSCSTReader.c
        liborte/RTPSCSTReaderTimer.c
        liborte/RTPSCSTReaderProc.c
        liborte/RTPSCSTWriter.c
        liborte/RTPSCSTWriterTimer.c
        liborte/RTPSGap.c
        liborte/RTPSHeader.c
        liborte/RTPSHeardBeat.c
        liborte/RTPSInfoDST.c
        liborte/RTPSInfoREPLY.c
        liborte/RTPSInfoSRC.c
        liborte/RTPSInfoTS.c
        liborte/RTPSPad.c
        liborte/RTPSUtils.c
        liborte/RTPSVar.c
        liborte/RTPSIssue.c
        liborte/sock.c
        liborte/ul_gavl.c
        liborte/ul_gavlprim.c
        liborte/cdr.c)

add_library("jorte" SHARED
        libjorte/onLoad.c
        libjorte/getNtpTime.c
        libjorte/setType.c
        libjorte/setTopic.c
        libjorte/createAppInfo.c
        libjorte/createPubInfo.c
        libjorte/createSubInfo.c
        libjorte/JGetORTEConstant.c
        libjorte/JIPAddressToString.c
        libjorte/JNtpTimeToStringMs.c
        libjorte/JNtpTimeToStringUs.c
        libjorte/JORTEDomainAppCreate.c
        libjorte/JORTEDomainAppDestroy.c
        libjorte/JORTEDomainEvents.c
        libjorte/JORTEDomainEventsDestroy.c
        libjorte/JORTEDomainInitEvents.c
        libjorte/JORTEDomainMgrCreate.c
        libjorte/JORTEDomainMgrDestroy.c
        libjorte/JORTEDomainPropDefaultGet.c
        libjorte/JORTEDomainPropSet.c
        libjorte/JORTEDomainPropDestroy.c
        libjorte/JORTEDomainStart.c
        libjorte/JORTEInit.c
        libjorte/JORTEPublicationCreate.c
        libjorte/JORTEPublicationDestroy.c
        libjorte/JORTEPublicationSend.c
        libjorte/JORTEPublicationPropertiesSet.c
        libjorte/JORTEPublicationPropertiesGet.c
        libjorte/JORTEPublicationGetStatus.c
        libjorte/JORTEPublicationWaitForSubscriptions.c
        libjorte/JORTERecvInfoHelpers.c
        libjorte/JORTESleepMs.c
        libjorte/JORTESubscriptionCreate.c
        libjorte/JORTESubscriptionDestroy.c
        libjorte/JORTESubscriptionPropertiesGet.c
        libjorte/JORTESubscriptionPropertiesSet.c
        libjorte/JORTESubscriptionPull.c
        libjorte/JORTESubscriptionGetStatus.c
        libjorte/JORTESubscriptionWaitForPublications.c
        libjorte/JORTETypeRegisterAdd.c
        libjorte/JORTETypeRegisterDestroyAll.c
        libjorte/JORTEVerbositySetLogFile.c
        libjorte/JORTEVerbositySetOptions.c
        libjorte/JStringToIPAddress.c)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries("jorte"
        # List libraries link to the target library
        android
        log
        orte)