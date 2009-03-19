# Path setting
INC_DIRS =	\
	./include/album	\
	../core/include/codec	\
	../core/include/common	\
	../core/include/widget	\
	./include/widget
SRC_DIRS =	\
	./src/album	\
	./src/widget

DEPENDPATH  += $$INC_DIRS $$SRC_DIRS
INCLUDEPATH += $$INC_DIRS
MOC_DIR = ../tmp/moc
RCC_DIR = ../tmp/rcc

# Input files
HEADERS +=	\
	./include/album/cuesheet.hpp	\
	./include/album/index.hpp	\
	./include/album/track.hpp	\
	./include/widget/converter.hpp	\
	./include/widget/mainwindow.hpp	\
	./include/widget/player.hpp	\
	./include/widget/preference.hpp	\
	./include/widget/progress.hpp	\
	./include/widget/songlist.hpp	\
	./include/widget/textcodec.hpp	\
	./include/widget/threads.hpp
SOURCES +=	\
	./src/album/cuesheet.cpp	\
	./src/album/index.cpp	\
	./src/album/track.cpp	\
	./src/main.cpp	\
	./src/widget/converter.cpp	\
	./src/widget/mainwindow.cpp	\
	./src/widget/player.cpp	\
	./src/widget/preference.cpp	\
	./src/widget/progress.cpp	\
	./src/widget/songlist.cpp	\
	./src/widget/textcodec.cpp	\
	./src/widget/threads.cpp

RESOURCES += main.qrc

# config
TARGET  = khopper
CONFIG += debug_and_release
QT     += phonon

CONFIG( debug, debug|release ) {
	DESTDIR = ../bin/debug
	OBJECTS_DIR = ../tmp/obj/debug

	unix:LIBS += -L../lib/debug -lkhopper
} else {
	DESTDIR = ../bin/release
	OBJECTS_DIR = ../tmp/obj/release
	DEFINES += QT_NO_DEBUG_OUTPUT

	unix:LIBS += -L../lib/release -lkhopper
}

unix {
	TEMPLATE = app
	QMAKE_CXXFLAGS += -std=c++0x
	LIBS += -lboost_regex
}

win32 {
	TEMPLATE = vcapp
}
