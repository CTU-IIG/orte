SOURCES	+= main.cpp \
	MyQFrame.cpp \
	richtext.cpp \
	ortedemo_types.c
HEADERS	+= richtext.h \
	MyQFrame.h
TARGET		= ortedemo

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

FORMS	= MainForm.ui \
	FPublisher.ui \
	FSubscriber.ui
IMAGES	= ocera_logo.png \
	shapes.png
TEMPLATE	=app
CONFIG	+= qt-mt 
INCLUDEPATH	+= /orte/orte/include .
DEFINES += HAVE_STDINT_H QT_THREAD_SUPPORT
LIBS	+= ../../../debug/orte/liborte/liborte.a -lpthread
LANGUAGE	= C++
