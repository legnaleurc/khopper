# - Find FFmpeg
# This module can be used to find FFmpeg
# Once done this will define
#
# FFMPEG_FOUND         If false, don't try to use FFmpeg
#
# FFMPEG_INCLUDE_DIR   Include directory necessary for using the FFmpeg headers
# FFMPEG_LIBRARIES     Link this to use FFmpeg

if(FFMPEG_INCLUDE_DIR AND FFMPEG_LIBRARIES)
    set(FFMPEG_FOUND TRUE)
else()
    if(NOT WIN32)
        find_package(PkgConfig)
        pkg_check_modules(PC_FFMPEG libavformat libavcodec libavutil)
        set(FFMPEG_DEFINITIONS
            ${PC_FFMPEG_libavformat_CFLAGS_OTHER}
            ${PC_FFMPEG_libavcodec_CFLAGS_OTHER}
            ${PC_FFMPEG_libavutil_CFLAGS_OTHER})
    endif()

    find_path(FFMPEG_INCLUDE_DIR
        NAMES libavformat/avformat.h libavcodec/avcodec.h
        HINTS
            ${PC_FFMPEG_libavformat_INCLUDEDIR}
            ${PC_FFMPEG_libavformat_INCLUDE_DIRS}
            ${PC_FFMPEG_libavcodec_INCLUDEDIR}
            ${PC_FFMPEG_libavcodec_INCLUDE_DIRS})
    find_library(FFMPEG_FORMAT_LIBRARY avformat
        HINTS
            ${PC_FFMPEG_libavformat_LIBDIR}
            ${PC_FFMPEG_libavformat_LIBRARY_DIRS})
    find_library(FFMPEG_CODEC_LIBRARY avcodec
        HINTS
            ${PC_FFMPEG_libavcodec_LIBDIR}
            ${PC_FFMPEG_libavcodec_LIBRARY_DIRS})
    find_library(FFMPEG_UTIL_LIBRARY avutil
        HINTS
            ${PC_FFMPEG_libavutil_LIBDIR}
            ${PC_FFMPEG_libavutil_LIBRARY_DIRS})

    set(FFMPEG_LIBRARIES)
    if(FFMPEG_FORMAT_LIBRARY)
        set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_FORMAT_LIBRARY})
    endif()
    if(FFMPEG_CODEC_LIBRARY)
        set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_CODEC_LIBRARY})
    endif()
    if(FFMPEG_UTIL_LIBRARY)
        set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} ${FFMPEG_UTIL_LIBRARY})
    endif()

    if(FFMPEG_INCLUDE_DIR AND FFMPEG_LIBRARIES)
        set(FFMPEG_FOUND TRUE)
    endif()

    if(FFMPEG_FOUND)
        if(NOT FFmpeg_FIND_QUIETLY)
            message(STATUS "Found FFmpeg: ${FFMPEG_LIBRARIES} ${FFMPEG_INCLUDE_DIR}")
        endif()
    else()
        if(FFmpeg_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find FFmpeg")
        else()
            if(NOT FFmpeg_FIND_QUIETLY)
                message(STATUS "Could NOT find FFmpeg")
            endif()
        endif()
    endif()

    mark_as_advanced(FFMPEG_FORMAT_LIBRARY FFMPEG_CODEC_LIBRARY FFMPEG_UTIL_LIBRARY)
endif()
