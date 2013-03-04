# - Find FFMPEG
# This module can be used to find FFMPEG
# Once done this will define
#
# FFMPEG_FOUND         If false, don't try to use FFMPEG
#
# FFMPEG_INCLUDE_DIRS  Include directory necessary for using the FFMPEG headers
# FFMPEG_LIBRARIES     Link this to use FFMPEG

include(KhopperUtility)

if(NOT FFMPEG_FOUND)
    find_package(PkgConfig QUIET)
    if(PKG_CONFIG_FOUND)
		pkg_check_modules(PC_FFMPEG QUIET libavformat libavcodec libavutil libswresample)
        if(PC_FFMPEG_FOUND)
            set(FFMPEG_DEFINITIONS ${PC_FFMPEG_CFLAGS_OTHER})
            set(FFMPEG_INCLUDE_DIRS ${PC_FFMPEG_INCLUDE_DIRS})
            set(FFMPEG_LIBRARIES ${PC_FFMPEG_LIBRARIES})
        endif()
    endif()

    path_from_env(FFMPEG_ROOT)

    if(NOT FFMPEG_INCLUDE_DIRS)
        find_path(FFMPEG_INCLUDE_DIRS
            NAMES libavformat/avformat.h libavcodec/avcodec.h
            HINTS ${FFMPEG_ROOT}
            PATH_SUFFIXES "include")
    endif()

    if(NOT FFMPEG_LIBRARIES)
        find_library(FFMPEG_FORMAT_LIBRARY avformat
            HINTS ${FFMPEG_ROOT}
            PATH_SUFFIXES "lib")
        find_library(FFMPEG_CODEC_LIBRARY avcodec
            HINTS ${FFMPEG_ROOT}
            PATH_SUFFIXES "lib")
        find_library(FFMPEG_UTIL_LIBRARY avutil
            HINTS ${FFMPEG_ROOT}
            PATH_SUFFIXES "lib")

        set(FFMPEG_LIBRARIES)
        if(FFMPEG_FORMAT_LIBRARY)
            list(APPEND FFMPEG_LIBRARIES ${FFMPEG_FORMAT_LIBRARY})
        endif()
        if(FFMPEG_CODEC_LIBRARY)
            list(APPEND FFMPEG_LIBRARIES ${FFMPEG_CODEC_LIBRARY})
        endif()
        if(FFMPEG_UTIL_LIBRARY)
            list(APPEND FFMPEG_LIBRARIES ${FFMPEG_UTIL_LIBRARY})
        endif()
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(FFMPEG DEFAULT_MSG FFMPEG_INCLUDE_DIRS FFMPEG_LIBRARIES)

    mark_as_advanced(FFMPEG_INCLUDE_DIRS FFMPEG_LIBRARIES)
endif()
