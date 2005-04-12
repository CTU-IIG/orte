# ---------------------------------------------------------------------
# Ocera Real-Time Ethernet
# Makefile for the ORTE manager
# ---------------------------------------------------------------------

LIBS=       kernel32.lib \
            ws2_32.lib \
            advapi32.lib \
            ..\..\contrib\win_pthread\pthreadVC.lib \
            ..\..\liborte\msvc\libortes.lib

CFLAGS = -c -O2 -MD -I..\..\include -I..\..\include\win32

CC=         cl 
LINK=       link

TARGETS = ortemanager.exe

all :	default

default : $(TARGETS)

ortemanager.exe : service.obj ortemanager.obj getopt.obj getopt_long.obj

# ---------------------------------------------------------------------
# Generic Compilation Rules
#
# Do NOT change anything below this point.
# ---------------------------------------------------------------------
#.SUFFIXES: .c .obj .exe

.c.obj:
	$(CC) $(CFLAGS) $< -Fo$*.obj

{..\}.c{}.obj:
	$(CC) $(CFLAGS) $< -Fo$*.obj	

.obj.exe:
    $(LINK) -out:$@ $(LIBS) $** 


clean :
    if exist *.obj del *.obj
    if exist *.exe del *.exe
    if exist *.pdb del *.pdb
    if exist *.ilk del *.ilk
    if exist *.idb del *.idb
