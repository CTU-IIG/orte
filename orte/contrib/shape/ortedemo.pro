SOURCES	+= main.cpp \
	MyQFrame.cpp \
	richtext.cpp \
	BoxType.c
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
INCLUDEPATH	+= .
DEFINES += HAVE_STDINT_H QT_THREAD_SUPPORT
LIBS	+= -lorte -lpthread 
LANGUAGE	= C++
