# Path setting
ROOT_PATH = ../..
CORE_PATH = $${ROOT_PATH}/core

INC_DIRS =	\
	$${CORE_PATH}/include/codec	\
	$${CORE_PATH}/include/common	\
	./include
SRC_DIRS = ./src

INCLUDEPATH += $${INC_DIRS} $${SRC_DIRS}
DEPANDPATH  += $${INC_DIRS}

HEADERS = include/defaultwriter.hpp include/defaultwc.hpp
SOURCES = src/defaultwriter.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
TEMPLATE = lib
CONFIG  += plugin debug_and_release
TARGET   = $$qtLibraryTarget(kwp_default)

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/build/debug -lkhopper
} else {
	DESTDIR     = $${ROOT_PATH}/build/release/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/build/release -lkhopper

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	QMAKE_CXXFLAGS += -std=c++0x
	CONFIG         += link_pkgconfig
	PKGCONFIG      += libavcodec libavformat libavutil
}

win32 {
	LIBS += -lavcodec-52 -lavformat-52 -lavutil-49
}
