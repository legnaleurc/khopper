# - Find flac
# This module can be used to find flac
# Once done this will define
#
# FLAC_FOUND       If false, don't try to use flac
#
# FLAC_INCLUDE_DIR The Flac include directory
# FLAC_LIBRARIES   Link these to use Flac

if(FLAC_INCLUDE_DIR AND FLAC_LIBRARIES)
    set(FLAC_FOUND TRUE)
else()
    if(NOT WIN32)
        find_package(PkgConfig)
        pkg_check_modules(PC_FLAC flac)
    endif()

    find_path(FLAC_INCLUDE_DIR
        NAMES FLAC/stream_encoder.h FLAC/stream_decoder.h
        HINTS
            ${PC_FLAC_INCLUDEDIR}
            ${PC_FLAC_INCLUDE_DIRS})
    find_library(FLAC_LIBRARIES_RELEASE FLAC
        HINTS
            ${PC_FLAC_LIBDIR}
            ${PC_FLAC_LIBRARY_DIRS})
    find_library(FLAC_LIBRARIES_DEBUG FLAC_D
        HINTS
            ${PC_FLAC_LIBDIR}
            ${PC_FLAC_LIBRARY_DIRS})
    if(FLAC_LIBRARIES_RELEASE AND FLAC_LIBRARIES_DEBUG)
        set(FLAC_LIBRARIES optimized ${FLAC_LIBRARIES_RELEASE} debug ${FLAC_LIBRARIES_DEBUG})
    elseif(FLAC_LIBRARIES_RELEASE)
        set(FLAC_LIBRARIES ${FLAC_LIBRARIES_RELEASE})
    endif()

    if(FLAC_INCLUDE_DIR AND FLAC_LIBRARIES)
        set(FLAC_FOUND TRUE)
    endif()

    if(FLAC_FOUND)
        if(NOT Flac_FIND_QUIETLY)
            message(STATUS "Found Flac: ${FLAC_LIBRARIES}")
        endif()
    else()
        if(Flac_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find Flac")
        else()
            if(NOT Flac_FIND_QUIETLY)
                message(STATUS "Could NOT find Flac")
            endif()
        endif()
    endif()
endif()
