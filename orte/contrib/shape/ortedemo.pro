SOURCES	+= main.cpp \
        MainForm.cpp \
	MyQFrame.cpp \
	richtext.cpp \
	FPublisher.cpp \
	FSubscriber.cpp \
	BoxType.c 

HEADERS	+= richtext.h \
	MyQFrame.h \
	MainForm.h \
	FPublisher.h \
	FSubscriber.h

TARGET	= ortedemo

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

FORMS	= MainForm.ui \
	FPublisher.ui \
	FSubscriber.ui

RESOURCES += shapes.qrc

TEMPLATE	=app
CONFIG	+= qt-mt 
INCLUDEPATH	+= . ../../../_compiled/include
DEFINES += HAVE_STDINT_H QT_THREAD_SUPPORT
LIBS	+= -L../../../_compiled/lib -lorte
!blackberry:LIBS += -lpthread
blackberry:LIBS += -lsocket
win32:LIBS	+= -lws2_32 
LANGUAGE	= C++
