SOURCES	+= main.cpp Publisher.cpp ViewFrm.cpp Subscriber.cpp 
HEADERS	+= Publisher.h ViewFrm.h define.h Subscriber.h 
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
FORMS	= MulticlipForm.ui MainFrm.ui 
TEMPLATE	=app
CONFIG	+= qt warn_on release
INCLUDEPATH	+= ../../include 
LIBS	+= ../../liborte.a
DBFILE	= Orte_demo.db
LANGUAGE	= C++
