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
TARGET   = $$qtLibraryTarget(kpp_mp3)

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/build/debug -lkhopper
	win32:LIBS += -L$${ROOT_PATH}/build/debug -lkhopper -lloki_D
} else {
	DESTDIR     = $${ROOT_PATH}/build/release/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/build/release -lkhopper
	win32:LIBS += -L$${ROOT_PATH}/build/release -lkhopper -lloki

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	QMAKE_CXXFLAGS += -std=c++0x
}
