TARGET		= ortedemo

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt debug

LIBS	+= /orte/debug/orte/liborte/liborte.a -lpthread
INCLUDEPATH	+= /orte/orte/include .

HEADERS	+= richtext.h \
	MyQFrame.h
SOURCES	+= main.cpp \
	MyQFrame.cpp \
	richtext.cpp \
	ortedemo_types.c
FORMS	= MainForm.ui \
	FPublisher.ui \
	FSubscriber.ui
IMAGES	= ocera_logo.png

