# Microsoft Developer Studio Project File - Name="liborte" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=liborte - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "liborte.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "liborte.mak" CFG="liborte - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "liborte - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "liborte - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "liborte - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "_ORTE_WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../include" /I "../../include/win32" /YX /FD /c
# ADD BASE RSC /l 0x405 /d "NDEBUG"
# ADD RSC /l 0x405 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "liborte - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_ORTE_WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /I "../../include" /I "../../include/win32" /YX /FD /GZ /c
# SUBTRACT CPP /WX /Fr
# ADD BASE RSC /l 0x405 /d "_DEBUG"
# ADD RSC /l 0x405 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "liborte - Win32 Release"
# Name "liborte - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\conv.c
# End Source File
# Begin Source File

SOURCE=..\debug.c
# End Source File
# Begin Source File

SOURCE=..\event.c
# End Source File
# Begin Source File

SOURCE=..\fnmatch.c
# End Source File
# Begin Source File

SOURCE=..\globals.c
# End Source File
# Begin Source File

SOURCE=..\htimerNtp.c
# End Source File
# Begin Source File

SOURCE=..\objectEntry.c
# End Source File
# Begin Source File

SOURCE=..\objectEntryTimer.c
# End Source File
# Begin Source File

SOURCE=..\objectUtils.c
# End Source File
# Begin Source File

SOURCE=..\ORTEAppRecvMetatrafficThread.c
# End Source File
# Begin Source File

SOURCE=..\ORTEAppRecvUserdataThread.c
# End Source File
# Begin Source File

SOURCE=..\ORTEAppSendThread.c
# End Source File
# Begin Source File

SOURCE=..\ORTEDomain.c
# End Source File
# Begin Source File

SOURCE=..\ORTEDomainApp.c
# End Source File
# Begin Source File

SOURCE=..\ORTEDomainMgr.c
# End Source File
# Begin Source File

SOURCE=..\ORTEInit.c
# End Source File
# Begin Source File

SOURCE=..\ORTEPattern.c
# End Source File
# Begin Source File

SOURCE=..\ORTEPublication.c
# End Source File
# Begin Source File

SOURCE=..\ORTEPublicationTimer.c
# End Source File
# Begin Source File

SOURCE=..\ORTESubscription.c
# End Source File
# Begin Source File

SOURCE=..\ORTEThreadUtils.c
# End Source File
# Begin Source File

SOURCE=..\ORTETypeRegister.c
# End Source File
# Begin Source File

SOURCE=..\ORTEVerbosity.c
# End Source File
# Begin Source File

SOURCE=..\parameter.c
# End Source File
# Begin Source File

SOURCE=..\RTPSAck.c
# End Source File
# Begin Source File

SOURCE=..\RTPSCSTReader.c
# End Source File
# Begin Source File

SOURCE=..\RTPSCSTReaderProc.c
# End Source File
# Begin Source File

SOURCE=..\RTPSCSTReaderTimer.c
# End Source File
# Begin Source File

SOURCE=..\RTPSCSTWriter.c
# End Source File
# Begin Source File

SOURCE=..\RTPSCSTWriterTimer.c
# End Source File
# Begin Source File

SOURCE=..\RTPSGap.c
# End Source File
# Begin Source File

SOURCE=..\RTPSHeader.c
# End Source File
# Begin Source File

SOURCE=..\RTPSHeardBeat.c
# End Source File
# Begin Source File

SOURCE=..\RTPSInfoDST.c
# End Source File
# Begin Source File

SOURCE=..\RTPSInfoREPLY.c
# End Source File
# Begin Source File

SOURCE=..\RTPSInfoSRC.c
# End Source File
# Begin Source File

SOURCE=..\RTPSInfoTS.c
# End Source File
# Begin Source File

SOURCE=..\RTPSIssue.c
# End Source File
# Begin Source File

SOURCE=..\RTPSPad.c
# End Source File
# Begin Source File

SOURCE=..\RTPSUtils.c
# End Source File
# Begin Source File

SOURCE=..\RTPSVar.c
# End Source File
# Begin Source File

SOURCE=..\sock.c
# End Source File
# Begin Source File

SOURCE=..\ul_gavl.c
# End Source File
# Begin Source File

SOURCE=..\ul_gavlprim.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
