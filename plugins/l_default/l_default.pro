# Path setting
ROOT_PATH = ../..
CORE_PATH = $${ROOT_PATH}/core

INC_DIRS =	\
	$${CORE_PATH}/include	\
	./include
SRC_DIRS = ./src

INCLUDEPATH += $${INC_DIRS} $${SRC_DIRS}
DEPANDPATH  += $${INC_DIRS}

HEADERS = include/defaultreader.hpp include/defaultwriter.hpp
SOURCES = src/defaultreader.cpp src/defaultwriter.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
CONFIG += debug_and_release
TARGET  = klp_default
VERSION = 0.1.70

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug/lib
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/build/debug/lib -lk_core
	win32:LIBS += -L$${ROOT_PATH}/build/debug/lib -lk_core0
} else {
	DESTDIR     = $${ROOT_PATH}/build/release/lib
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/build/release/lib -lk_core
	win32:LIBS += -L$${ROOT_PATH}/build/release/lib -lk_core0

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	TEMPLATE = lib
	QMAKE_CXXFLAGS += -std=c++0x
	CONFIG         += link_pkgconfig
	PKGCONFIG      += libavcodec libavformat libavutil
}

win32 {
	TEMPLATE = vclib
	LIBS += -lavcodec-52 -lavformat-52 -lavutil-49
	SOURCES += src/wfile.cpp
}
