# Path setting
ROOT_PATH = ../..
CORE_PATH = $${ROOT_PATH}/core

INC_DIRS = .	\
	$${CORE_PATH}/include
SRC_DIRS = .

INCLUDEPATH += $${INC_DIRS} $${SRC_DIRS}
DEPANDPATH  += $${INC_DIRS}

HEADERS = flacpanel.hpp
SOURCES = flacpanel.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
CONFIG  += plugin debug_and_release
TARGET   = $$qtLibraryTarget(kpp_flac)

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	unix:LIBS  += -L$${ROOT_PATH}/build/debug -lk_core
	win32:LIBS += -L$${ROOT_PATH}/build/debug -lk_core0 -lloki_D
} else {
	DESTDIR     = $${ROOT_PATH}/build/release/plugins
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:LIBS  += -L$${ROOT_PATH}/build/release -lk_core
	win32:LIBS += -L$${ROOT_PATH}/build/release -lk_core0 -lloki

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	TEMPLATE = lib
	QMAKE_CXXFLAGS += -std=c++0x
}

win32 {
	TEMPLATE = vclib
}
