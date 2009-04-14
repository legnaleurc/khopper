# Path settings
ROOT_PATH = ..
INC_DIRS = ./include
SRC_DIRS =	\
	./src/common	\
	./src/plugin

DEPENDPATH  += $${INC_DIRS} $${SRC_DIRS}
INCLUDEPATH += $${INC_DIRS} ./src/plugin
MOC_DIR      = $${ROOT_PATH}/tmp/moc

# Input files
HEADERS +=	\
	./include/plugin/abstractpanel.hpp	\
	./include/plugin/abstractreader.hpp	\
	./include/plugin/abstractwriter.hpp	\
	./include/plugin/codec_base.hpp	\
	./include/plugin/creator.hpp	\
	./src/plugin/plugin_impl.hpp	\
	./include/plugin/readerplugin.hpp	\
	./include/plugin/writerplugin.hpp	\
	./include/common/error.hpp	\
	./include/common/os.hpp	\
	./include/common/text.hpp	\
	./include/common/tr1.hpp
SOURCES +=	\
	./src/plugin/abstractpanel.cpp	\
	./src/plugin/abstractreader.cpp	\
	./src/plugin/abstractwriter.cpp	\
	./src/plugin/readerplugin.cpp	\
	./src/plugin/writerplugin.cpp	\
	./src/common/error.cpp	\
	./src/common/text.cpp

# Config
TARGET   = k_core
CONFIG  += debug_and_release
VERSION  = 0.1.70

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug

	win32:LIBS += -lloki_D
} else {
	DESTDIR     = $${ROOT_PATH}/build/release
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	win32:LIBS += -lloki

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	TEMPLATE = lib
	QMAKE_CXXFLAGS += -std=c++0x
	LIBS        += -lloki
	SOURCES += ./src/common/linux.cpp
}

win32 {
	TEMPLATE = vclib
	SOURCES += ./src/common/windows.cpp
}
