# Path settings
ROOT_PATH = ..
INC_DIRS =	\
	./include/codec	\
	./include/common	\
	./include/widget
SRC_DIRS =	\
	./src/codec	\
	./src/common	\
	./src/widget

DEPENDPATH  += $${INC_DIRS} $${SRC_DIRS}
INCLUDEPATH += $${INC_DIRS}
MOC_DIR      = $${ROOT_PATH}/tmp/moc

# Input files
HEADERS +=	\
	./include/codec/abstractaudioreader.hpp	\
	./include/codec/abstractaudiowriter.hpp	\
	./include/codec/codec_base.hpp	\
	./include/codec/defaultaudioreader.hpp	\
	./include/codec/defaultaudiowriter.hpp	\
	./include/common/error.hpp	\
	./include/common/os.hpp	\
	./include/common/text.hpp	\
	./include/common/tr1.hpp	\
	./include/widget/abstractpanel.hpp
SOURCES +=	\
	./src/codec/abstractaudioreader.cpp	\
	./src/codec/abstractaudiowriter.cpp	\
	./src/codec/defaultaudioreader.cpp	\
	./src/codec/defaultaudiowriter.cpp	\
	./src/common/error.cpp	\
	./src/common/text.cpp	\
	./src/widget/abstractpanel.cpp

# Config
TEMPLATE = lib
TARGET   = khopper
CONFIG  += debug_and_release
VERSION  = 0.1.60

CONFIG( debug, debug|release ) {
	DESTDIR     = $${ROOT_PATH}/build/debug
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug
} else {
	DESTDIR     = $${ROOT_PATH}/build/release
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT

	unix:QMAKE_POST_LINK = strip $${DESTDIR}/lib$${TARGET}.so
}

unix {
	QMAKE_CXXFLAGS += -std=c++0x

	CONFIG      += link_pkgconfig
	PKGCONFIG   += libavcodec libavformat libavutil
	INCLUDEPATH += /usr/include/ffmpeg/libavcodec /usr/include/ffmpeg/libavformat
	LIBS        += -lboost_regex

	SOURCES += ./src/common/linux.cpp
}

win32 {
	LIBS    += -lavcodec-52 -lavformat-52 -lavutil-49
	SOURCES += ./src/common/windows.cpp
}
