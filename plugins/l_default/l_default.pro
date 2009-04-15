# Path setting
ROOT_PATH = ../..

INCLUDEPATH += .
DEPANDPATH  += .

SOURCES = wfile.cpp

MOC_DIR = $${ROOT_PATH}/tmp/moc

# Config
TEMPLATE = vclib
CONFIG  += staticlib debug_and_release
TARGET   = klp_default
LIBS += -lavcodec-52 -lavformat-52 -lavutil-49

CONFIG( debug, debug|release ) {
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/debug
} else {
	OBJECTS_DIR = $${ROOT_PATH}/tmp/obj/release
	DEFINES    += QT_NO_DEBUG_OUTPUT
}
DESTDIR     = $${OBJECTS_DIR}
