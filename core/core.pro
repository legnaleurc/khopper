include(../configure.pri)

# Path settings
INC_DIRS = . ./include
SRC_DIRS = .	\
	./src/codec	\
	./src/plugin	\
	./src/util

DEPENDPATH  += $${INC_DIRS} $${SRC_DIRS}
INCLUDEPATH += $${INC_DIRS}
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
	./include/plugin/plugincontext.hpp	\
	./include/util/error.hpp	\
	./include/util/os.hpp	\
	./include/util/text.hpp	\
	./include/util/tr1.hpp
SOURCES +=	\
	./src/codec/abstractreader.cpp	\
	./src/codec/abstractwriter.cpp	\
	./src/codec/defaultreader.cpp	\
	./src/codec/defaultwriter.cpp	\
	./src/plugin/abstractpanel.cpp	\
	./src/plugin/abstractreadercreator.cpp	\
	./src/plugin/abstractwritercreator.cpp	\
	./src/plugin/plugincontext.cpp	\
	./src/util/error.cpp	\
	./src/util/text.cpp

# Config
TARGET   = k_core
TEMPLATE = lib
CONFIG  += debug_and_release
VERSION  = $${KHOPPER_VERSION}

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
	CONFIG         += link_pkgconfig
	PKGCONFIG      += libavcodec libavformat libavutil
	LIBS           += -lloki
	SOURCES        += ./src/util/linux.cpp
}

win32 {
	SOURCES += ./src/util/windows.cpp ./src/codec/wfile.cpp
	LIBS    += -lavcodec -lavformat -lavutil
}
