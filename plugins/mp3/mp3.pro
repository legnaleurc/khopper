# Path setting
ROOT_PATH = ../..
CORE_PATH = $${ROOT_PATH}/core

INC_DIRS =	\
	$${CORE_PATH}/include/codec	\
	$${CORE_PATH}/include/common	\
	$${CORE_PATH}/include/widget	\
	./include
SRC_DIRS = ./src

INCLUDEPATH += $${INC_DIRS} $${SRC_DIRS}
DEPANDPATH  += $${INC_DIRS}

HEADERS = include/mp3panel.hpp
SOURCES = src/mp3panel.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
TEMPLATE = lib
CONFIG  += plugin debug_and_release
TARGET   = $$qtLibraryTarget(kp_mp3panel)

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/bin/debug/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/lib/debug -lkhopper
} else {
	DESTDIR     = $${ROOT_PATH}/bin/release/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/lib/release -lkhopper
}

unix {
	QMAKE_CXXFLAGS += -std=c++0x
}
