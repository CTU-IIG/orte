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
	shapes.png \
	marble.png \
	FMainIcon.png \
	FPublisherIcon.png \
	FSubscriberIcon.png
TEMPLATE	=app
CONFIG	+= qt-mt 
INCLUDEPATH	+= .
DEFINES += HAVE_STDINT_H QT_THREAD_SUPPORT
LIBS	+= -lorte -lpthread 
win32:LIBS	+= -lws2_32 
LANGUAGE	= C++

#The following line was inserted by qt3to4
QT +=  qt3support 
#The following line was inserted by qt3to4
CONFIG += uic3
