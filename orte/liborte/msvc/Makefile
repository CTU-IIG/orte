# ---------------------------------------------------------------------
# Ocera Real-Time Ethernet
# Makefile for the liborte library files
# ---------------------------------------------------------------------

LOCAL_INCLUDE = -I..\..\include -I..\..\include\win32

LIBS =  kernel32.lib \
        ws2_32.lib \
        ..\..\contrib\win_pthread\pthreadVC.lib
         
# Setting for RELEASE build
RELEASE_FLAGS = -c -O2 -MD $(LOCAL_INCLUDE)  
      
# Setting for DEBUG build
DEBUG_FLAGS = -c -Od -ZI -MDd $(LOCAL_INCLUDE)

ORTELIB_OBJS= \
	$(OBJ_PATH)\cdr.obj \
	$(OBJ_PATH)\conv.obj \
	$(OBJ_PATH)\debug.obj \
	$(OBJ_PATH)\event.obj \
	$(OBJ_PATH)\fnmatch.obj \
	$(OBJ_PATH)\htimerNtp.obj \
	$(OBJ_PATH)\objectEntry.obj \
	$(OBJ_PATH)\objectEntryTimer.obj \
	$(OBJ_PATH)\objectUtils.obj \
	$(OBJ_PATH)\ORTEAppRecvThread.obj \
	$(OBJ_PATH)\ORTEAppSendThread.obj \
	$(OBJ_PATH)\ORTEDomainApp.obj \
	$(OBJ_PATH)\ORTEPublication.obj \
	$(OBJ_PATH)\ORTEPublicationTimer.obj \
	$(OBJ_PATH)\ORTESubscription.obj \
	$(OBJ_PATH)\ORTEDomain.obj \
	$(OBJ_PATH)\ORTEDomainMgr.obj \
	$(OBJ_PATH)\ORTEInit.obj \
	$(OBJ_PATH)\ORTEThreadUtils.obj \
	$(OBJ_PATH)\ORTETypeRegister.obj \
	$(OBJ_PATH)\ORTEVerbosity.obj \
	$(OBJ_PATH)\ORTEPattern.obj \
	$(OBJ_PATH)\ORTEMisc.obj \
	$(OBJ_PATH)\parameter.obj \
	$(OBJ_PATH)\RTPSAck.obj \
	$(OBJ_PATH)\RTPSCSTReader.obj \
	$(OBJ_PATH)\RTPSCSTReaderTimer.obj \
	$(OBJ_PATH)\RTPSCSTReaderProc.obj \
	$(OBJ_PATH)\RTPSCSTWriter.obj \
	$(OBJ_PATH)\RTPSCSTWriterTimer.obj \
	$(OBJ_PATH)\RTPSGap.obj \
	$(OBJ_PATH)\RTPSHeader.obj \
	$(OBJ_PATH)\RTPSHeardBeat.obj \
	$(OBJ_PATH)\RTPSInfoDST.obj \
	$(OBJ_PATH)\RTPSInfoREPLY.obj \
	$(OBJ_PATH)\RTPSInfoSRC.obj \
	$(OBJ_PATH)\RTPSInfoTS.obj \
	$(OBJ_PATH)\RTPSPad.obj \
	$(OBJ_PATH)\RTPSUtils.obj \
	$(OBJ_PATH)\RTPSVar.obj \
	$(OBJ_PATH)\RTPSIssue.obj \
	$(OBJ_PATH)\sock.obj \
	$(OBJ_PATH)\ul_gavl.obj \
	$(OBJ_PATH)\ul_gavlprim.obj

CC=         cl
LINK=		link
LIBRARIAN=	lib


help:
	@ echo Run one of the following command lines:
	@ echo nmake static        (to build the MSVC static lib)
	@ echo nmake static-debug  (to build the MSVC static lib with debug info)
	@ echo nmake dynamic       (to build the MSVC dll)
	@ echo nmake dynamic-debug (to build the MSVC dll with debug info)
	@ echo nmake clean  (complete clean)
	@ echo nmake static-clean (clean MSVC static lib), etc.
	 
	
default : help

all : static dynamic

static :
	if not exist Release mkdir Release
	@ nmake OBJ_PATH=".\Release" CFLAGS="$(RELEASE_FLAGS)" LDFLAGS="" libORTEs.lib
	 
static-debug :
	if not exist Debug mkdir Debug
	@ nmake OBJ_PATH=".\Debug" CFLAGS="$(DEBUG_FLAGS) -Fd.\Debug\vc70.pdb" LDFLAGS=""  libORTEsd.lib

dynamic :
	if not exist Release_DLL mkdir Release_DLL
	@ nmake OBJ_PATH=".\Release_DLL" CFLAGS="$(RELEASE_FLAGS)" LDFLAGS=""  libORTEd.dll

dynamic-debug :
	if not exist Debug_DLL mkdir Debug_DLL
	@ nmake OBJ_PATH=".\Debug_DLL" CFLAGS="$(DEBUG_FLAGS) -Fd.\Debug_DLL\vc70.pdb" \
	LDFLAGS="-DEBUG -PDB:.\Debug_DLL\liborte.pdb" libORTEdd.dll


# ---------------------------------------------------------------------
# Compilation rules
# ---------------------------------------------------------------------

libORTEs.lib libORTEsd.lib : $(ORTELIB_OBJS)
    $(LIBRARIAN) -OUT:$@ $(ORTELIB_OBJS)


libORTEd.dll libORTEdd.dll : $(ORTELIB_OBJS)
    $(LINK) -OUT:$@ -DLL -DEF:"liborte.def"  -IMPLIB:"$*.lib" \
            $(LDFLAGS) $(LIBS) $(ORTELIB_OBJS)

{..\}.c{$(OBJ_PATH)}.obj:
		$(CC) $(CFLAGS) $< -Fo$*.obj	

# ---------------------------------------------------------------------
# CLEAN 
# ---------------------------------------------------------------------

clean : clean-static clean-static-debug clean-dynamic clean-dynamic-debug

clean-static :
     @ nmake OBJ_PATH=".\Release" LIBNAME="libORTEs" clean-path 

clean-static-debug :
     @ nmake OBJ_PATH=".\Debug" LIBNAME="libORTEsd" clean-path 
    
clean-dynamic : 
     @ nmake OBJ_PATH=".\Release_DLL" LIBNAME="libORTEd" clean-path 

clean-dynamic-debug :
     @ nmake OBJ_PATH=".\Debug_DLL" LIBNAME="libORTEdd" clean-path 

clean-path :
     if exist $(LIBNAME).lib del $(LIBNAME).lib
     if exist $(LIBNAME).dll del $(LIBNAME).dll
     if exist $(LIBNAME).exp del $(LIBNAME).exp
     if exist $(LIBNAME).ilk del $(LIBNAME).ilk
     if exist $(OBJ_PATH)\*.obj del $(OBJ_PATH)\*.obj
     if exist $(OBJ_PATH)\*.pdb del $(OBJ_PATH)\*.pdb
     if exist $(OBJ_PATH)\*.idb del $(OBJ_PATH)\*.idb
    
