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
CONFIG	+= qt debug
INCLUDEPATH	+= /orte/orte/include .
LIBS	+= -lorte -lpthread
LANGUAGE	= C++
