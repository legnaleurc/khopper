# - Find LIBAV
# This module can be used to find LIBAV
# Once done this will define
#
# LIBAV_FOUND         If false, don't try to use LIBAV
#
# LIBAV_INCLUDE_DIRS  Include directory necessary for using the LIBAV headers
# LIBAV_LIBRARIES     Link this to use LIBAV

include(KhopperUtility)

if(LIBAV_FOUND)
    return()
endif()

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_LIBAV QUIET libavformat libavcodec libavutil libavresample)
    if(PC_LIBAV_FOUND)
        set(LIBAV_DEFINITIONS ${PC_LIBAV_CFLAGS_OTHER})
        set(LIBAV_INCLUDE_DIRS ${PC_LIBAV_INCLUDE_DIRS})
        set(LIBAV_LIBRARIES ${PC_LIBAV_LIBRARIES})
    endif()
endif()

path_from_env(LIBAV_ROOT)

if(NOT LIBAV_INCLUDE_DIRS)
    find_path(LIBAV_INCLUDE_DIRS
        NAMES libavformat/avformat.h libavcodec/avcodec.h
        HINTS ${LIBAV_ROOT}
        PATH_SUFFIXES "include")
endif()

if(NOT LIBAV_LIBRARIES)
    find_library(LIBAV_FORMAT_LIBRARY avformat
        HINTS ${LIBAV_ROOT}
        PATH_SUFFIXES "lib")
    find_library(LIBAV_CODEC_LIBRARY avcodec
        HINTS ${LIBAV_ROOT}
        PATH_SUFFIXES "lib")
    find_library(LIBAV_UTIL_LIBRARY avutil
        HINTS ${LIBAV_ROOT}
        PATH_SUFFIXES "lib")
    find_library(LIBAV_RESAMPLE_LIBRARY avresample
        HINTS ${LIBAV_ROOT}
        PATH_SUFFIXES "lib")

    set(LIBAV_LIBRARIES)
    if(LIBAV_FORMAT_LIBRARY)
        list(APPEND LIBAV_LIBRARIES ${LIBAV_FORMAT_LIBRARY})
    endif()
    if(LIBAV_CODEC_LIBRARY)
        list(APPEND LIBAV_LIBRARIES ${LIBAV_CODEC_LIBRARY})
    endif()
    if(LIBAV_UTIL_LIBRARY)
        list(APPEND LIBAV_LIBRARIES ${LIBAV_UTIL_LIBRARY})
    endif()
    if(LIBAV_RESAMPLE_LIBRARY)
        list(APPEND LIBAV_LIBRARIES ${LIBAV_RESAMPLE_LIBRARY})
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBAV DEFAULT_MSG LIBAV_INCLUDE_DIRS LIBAV_LIBRARIES)

mark_as_advanced(LIBAV_INCLUDE_DIRS LIBAV_LIBRARIES)
