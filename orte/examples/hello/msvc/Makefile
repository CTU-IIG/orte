# ---------------------------------------------------------------------
# Makefile for the Ocera Real-Time Ethernet
# ---------------------------------------------------------------------

LIBS=       kernel32.lib \
            ws2_32.lib \
            ..\..\..\contrib\win_pthread\pthreadVC.lib \
            ..\..\..\liborte\msvc\libortesd.lib

CFLAGS = -c -Od -ZI -MDd -I..\..\..\include -I..\..\..\include\win32

CC =        cl
LINK =      link

TARGETS = h_publisher.exe h_subscriber.exe

all :   default

default : $(TARGETS)

h_publisher.exe : h_publisher.obj

h_subscriber.exe : h_subscriber.obj

# ---------------------------------------------------------------------
# Generic Compilation Rules
#
# Do NOT change anything below this point.
# ---------------------------------------------------------------------
.SUFFIXES: .c .obj .exe

{..\}.c{}.obj:
    $(CC) $(CFLAGS) $< -Fo$*.obj

.obj.exe: 
    $(LINK) -DEBUG -out:$@ $< $(LIBS)

clean :
    if exist *.obj del *.obj
    if exist *.exe del *.exe
    if exist *.pdb del *.pdb
    if exist *.ilk del *.ilk
    if exist *.idb del *.idb
