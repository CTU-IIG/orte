SOURCES	+= main.cpp \
	Publisher.cpp \
	ViewFrm.cpp \
	Subscriber.cpp \
	ortedemo_types.c
HEADERS	+= Publisher.h \
	ViewFrm.h \
	ortedemo_types.h \
	Subscriber.h
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= MulticlipForm.ui \
	MainFrm.ui 
TEMPLATE	=app
CONFIG	+= qt warn_on release
INCLUDEPATH	+= ../../include
LIBS	+= ../../../debug/orte/liborte/liborte.a -lpthread
LANGUAGE	= C++
