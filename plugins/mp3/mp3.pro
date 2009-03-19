# Path setting
ROOT_PATH = ../..
CORE_PATH = $${ROOT_PATH}/core

INC_DIRS =	\
	$${CORE_PATH}/include/codec	\
	$${CORE_PATH}/include/common	\
	$${CORE_PATH}/include/widget	\
	./include
SRC_DIRS =	\
	$${CORE_PATH}/src/codec	\
	$${CORE_PATH}/src/common	\
	$${CORE_PATH}/src/widget	\
	./src

INCLUDEPATH += $$INC_DIRS $$SRC_DIRS
DEPANDPATH  += $$INC_DIRS

HEADERS = include/mp3panel.hpp
SOURCES = src/mp3panel.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
TEMPLATE = lib
CONFIG += plugin debug_and_release
TARGET = $$qtLibraryTarget(kp_mp3panel)

CONFIG( debug, debug|release ) {
	DESTDIR = ../../bin/debug/plugins
	OBJECTS_DIR = ../../tmp/obj/debug

	unix:LIBS += -L$${ROOT_PATH}/lib/debug -lkhopper
} else {
	DESTDIR = ../../bin/release/plugins
	OBJECTS_DIR = ../../tmp/obj/release
	DEFINES += QT_NO_DEBUG_OUTPUT

	unix:LIBS += -L$${ROOT_PATH}/lib/release -lkhopper
}

unix {
	QMAKE_CXXFLAGS += -std=c++0x
#	CONFIG += link_pkgconfig
#	PKGCONFIG += libavcodec libavformat libavutil
#	INCLUDEPATH += /usr/include/ffmpeg/libavcodec /usr/include/ffmpeg/libavformat
}
