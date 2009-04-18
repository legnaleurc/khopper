# Path settings
ROOT_PATH = ..
INC_DIRS = ./include
SRC_DIRS =	\
	./src/codec	\
	./src/plugin	\
	./src/util

DEPENDPATH  += $${INC_DIRS} $${SRC_DIRS}
INCLUDEPATH += $${INC_DIRS} ./src/plugin
MOC_DIR      = $${ROOT_PATH}/tmp/moc

# Input files
HEADERS +=	\
	./include/codec/abstractreader.hpp	\
	./include/codec/abstractwriter.hpp	\
	./include/codec/codec_base.hpp	\
	./include/codec/defaultreader.hpp	\
	./include/codec/defaultwriter.hpp	\
	./include/plugin/abstractpanel.hpp	\
	./include/plugin/abstractcreator.hpp	\
	./include/plugin/abstractreadercreator.hpp	\
	./include/plugin/abstractwritercreator.hpp	\
	./include/util/error.hpp	\
	./include/util/os.hpp	\
	./include/util/text.hpp	\
	./include/util/tr1.hpp	\
	./src/plugin/plugin_impl.hpp
SOURCES +=	\
	./src/codec/abstractreader.cpp	\
	./src/codec/abstractwriter.cpp	\
	./src/codec/defaultreader.cpp	\
	./src/codec/defaultwriter.cpp	\
	./src/plugin/abstractpanel.cpp	\
	./src/plugin/abstractreadercreator.cpp	\
	./src/plugin/abstractwritercreator.cpp	\
	./src/util/error.cpp	\
	./src/util/text.cpp

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
	TEMPLATE        = lib
	QMAKE_CXXFLAGS += -std=c++0x
	CONFIG         += link_pkgconfig
	PKGCONFIG      += libavcodec libavformat libavutil
	LIBS           += -lloki
	SOURCES        += ./src/util/linux.cpp
}

win32 {
	TEMPLATE = vclib
	SOURCES += ./src/util/windows.cpp ./src/codec/wfile.cpp
	LIBS    += -lavcodec-52 -lavformat-52 -lavutil-49
}
