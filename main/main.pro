# Path setting
ROOT_PATH = ..
CORE_PATH = $${ROOT_PATH}/core
INC_DIRS =	\
	$${CORE_PATH}/include	\
	./include/album	\
	./include/widget
SRC_DIRS =	\
	./src/album	\
	./src/widget

DEPENDPATH  += $${INC_DIRS} $${SRC_DIRS}
INCLUDEPATH += $${INC_DIRS}
MOC_DIR      = $${ROOT_PATH}/tmp/moc
RCC_DIR      = $${ROOT_PATH}/tmp/rcc

# Input files
HEADERS +=	\
	./include/album/cuesheet.hpp	\
	./include/album/index.hpp	\
	./include/album/track.hpp	\
	./include/widget/converter.hpp	\
	./include/widget/mainwindow.hpp	\
	./include/widget/outputoption.hpp	\
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
	./src/widget/outputoption.cpp	\
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
	DESTDIR     = $${ROOT_PATH}/build/debug
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/build/debug -lk_core
	win32:LIBS += -L$${ROOT_PATH}/build/debug -lk_core0
} else {
	DESTDIR     = $${ROOT_PATH}/build/release
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/build/release -lk_core
	win32:LIBS += -L$${ROOT_PATH}/build/release -lk_core0

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/$${TARGET}
}

unix {
	TEMPLATE = app
	QMAKE_CXXFLAGS += -std=c++0x
}

win32 {
	TEMPLATE = vcapp
}