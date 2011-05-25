# - Find flac
# This module can be used to find flac
# Once done this will define
#
# FLAC_FOUND       If false, don't try to use flac
#
# FLAC_INCLUDE_DIR The Flac include directory
# FLAC_LIBRARIES   Link these to use Flac

if(NOT FLAC_FOUND)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_FLAC flac)
        if(PC_FLAC_FOUND)
            set(FLAC_INCLUDE_DIR ${PC_FLAC_INCLUDE_DIRS} CACHE INTERNAL "")
            set(FLAC_LIBRARIES ${PC_FLAC_LIBRARIES} CACHE INTERNAL "")
            set(FLAC_FOUND TRUE CACHE INTERNAL "")
        endif()
    else()
        find_path(FLAC_INCLUDE_DIR
            NAMES FLAC/stream_encoder.h FLAC/stream_decoder.h)
        find_library(FLAC_LIBRARIES_RELEASE FLAC)
        find_library(FLAC_LIBRARIES_DEBUG FLACd)

        if(FLAC_LIBRARIES_RELEASE AND FLAC_LIBRARIES_DEBUG)
            set(FLAC_LIBRARIES optimized ${FLAC_LIBRARIES_RELEASE} debug ${FLAC_LIBRARIES_DEBUG})
        elseif(FLAC_LIBRARIES_RELEASE)
            set(FLAC_LIBRARIES ${FLAC_LIBRARIES_RELEASE})
        endif()

        if(FLAC_INCLUDE_DIR AND FLAC_LIBRARIES)
            set(FLAC_FOUND TRUE)
        endif()
    endif()

    if(FLAC_FOUND)
        set(FLAC_LIBRARIES ${FLAC_LIBRARIES} CACHE STRING "libFLAC libraries" FORCE)
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
