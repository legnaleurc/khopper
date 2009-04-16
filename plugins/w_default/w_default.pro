# Path setting
ROOT_PATH = ../..
CORE_PATH = $${ROOT_PATH}/core
LIB_PATH = ../l_default

INC_DIRS =	\
	$${CORE_PATH}/include	\
	$${LIB_PATH}/include	\
	./include
SRC_DIRS = ./src

INCLUDEPATH += $${INC_DIRS} $${SRC_DIRS}
DEPANDPATH  += $${INC_DIRS}

HEADERS = include/defaultwritercreator.hpp
SOURCES = src/defaultwritercreator.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
CONFIG  += plugin debug_and_release
TARGET   = $$qtLibraryTarget(kwp_default)

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/build/debug -lk_core -L$${ROOT_PATH}/build/debug -lk_default
	win32:LIBS += -L$${ROOT_PATH}/build/debug -lk_core0 -L$${ROOT_PATH}/build/debug -lk_default0 -lloki_D
} else {
	DESTDIR     = $${ROOT_PATH}/build/release/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/build/release -lk_core -L$${ROOT_PATH}/build/release -lk_default
	win32:LIBS += -L$${ROOT_PATH}/build/release -lk_core0 -L$${ROOT_PATH}/build/release -lk_default0 -lloki

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	TEMPLATE = lib
	QMAKE_CXXFLAGS += -std=c++0x
}

win32 {
	TEMPLATE = vclib
}
