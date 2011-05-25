# - Find FFmpeg
# This module can be used to find FFmpeg
# Once done this will define
#
# FFMPEG_FOUND         If false, don't try to use FFmpeg
#
# FFMPEG_INCLUDE_DIR   Include directory necessary for using the FFmpeg headers
# FFMPEG_LIBRARIES     Link this to use FFmpeg

if(NOT FFMPEG_FOUND)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_FFMPEG libavformat libavcodec libavutil)
        if(PC_FFMPEG_FOUND)
            set(FFMPEG_DEFINITIONS ${PC_FFMPEG_CFLAGS_OTHER})
            set(FFMPEG_INCLUDE_DIR ${PC_FFMPEG_INCLUDE_DIRS} CACHE INTERNAL "")
            set(FFMPEG_LIBRARIES ${PC_FFMPEG_LIBRARIES} CACHE INTERNAL "")
            set(FFMPEG_FOUND TRUE CACHE INTERNAL "")
        endif()
    else()
        find_path(FFMPEG_INCLUDE_DIR
            NAMES libavformat/avformat.h libavcodec/avcodec.h)
        find_library(FFMPEG_FORMAT_LIBRARY avformat)
        find_library(FFMPEG_CODEC_LIBRARY avcodec)
        find_library(FFMPEG_UTIL_LIBRARY avutil)

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

        mark_as_advanced(FFMPEG_FORMAT_LIBRARY FFMPEG_CODEC_LIBRARY FFMPEG_UTIL_LIBRARY)

        if(FFMPEG_INCLUDE_DIR AND FFMPEG_LIBRARIES)
            set(FFMPEG_FOUND TRUE)
        endif()
    endif()

    if(FFMPEG_FOUND)
        set(FFMPEG_LIBRARIES ${FFMPEG_LIBRARIES} CACHE STRING "FFmpeg libraries" FORCE)
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
endif()
