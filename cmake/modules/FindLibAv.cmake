# - Find libav*
# This module can be used to find libav*
#
# Below is a detailed list of variables that FindLibAv.cmake sets.
# LIBAV_FOUND         If false, don't try to use libav*
#
# LIBAV_INCLUDE_DIR   Path to "include" of libav*
# LIBAV_LIBRARIES_DIR Path to "lib" of libav*
# LIBAV_LIBRARIES     The libav* libraries

find_path(LIBAV_INCLUDE_DIR NAMES libavformat/avformat.h libavcodec/avcodec.h)
find_library(LIBAV_FORMAT_LIBRARY avformat HINTS ${LIBAV_LIBRARIES_DIR})
find_library(LIBAV_CODEC_LIBRARY avcodec HINTS ${LIBAV_LIBRARIES_DIR})
find_library(LIBAV_UTIL_LIBRARY avutil HINTS ${LIBAV_LIBRARIES_DIR})
find_path(LIBAV_LIBRARIES_DIR NAMES ${LIBAV_FORMAT_LIBRARY} ${LIBAV_CODEC_LIBRARY} ${LIBAV_UTIL_LIBRARY})

mark_as_advanced(LIBAV_FORMAT_LIBRARY LIBAV_CODEC_LIBRARY LIBAV_UTIL_LIBRARY)

if(LIBAV_INCLUDE_DIR AND LIBAV_FORMAT_LIBRARY AND LIBAV_CODEC_LIBRARY AND LIBAV_UTIL_LIBRARY)
    message(STATUS "Found libav*")
    set(LIBAV_FOUND TRUE)
    set(LIBAV_LIBRARIES ${LIBAV_FORMAT_LIBRARY} ${LIBAV_CODEC_LIBRARY} ${LIBAV_UTIL_LIBRARY})
else()
    message(STATUS "Find no libav*")
endif()
