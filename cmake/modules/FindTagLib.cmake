# - Find taglib
# This module can be used to find taglib
# Once done this will define
#
# TAGLIB_FOUND       If false, don't try to use taglib
#
# TAGLIB_INCLUDE_DIR The taglib include directory
# TAGLIB_LIBRARIES   Link these to use taglib

if(NOT TAGLIB_FOUND)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_TAGLIB taglib)
        if(PC_TAGLIB_FOUND)
            set(TAGLIB_INCLUDE_DIR ${PC_TAGLIB_INCLUDE_DIRS} CACHE INTERNAL "")
            set(TAGLIB_LIBRARIES ${PC_TAGLIB_LIBRARIES} CACHE INTERNAL "")
            set(TAGLIB_FOUND TRUE CACHE INTERNAL "")
        endif()
    else()
        find_path(TAGLIB_INCLUDE_DIR id3v2tag.h)
        find_library(TAGLIB_LIBRARIES_RELEASE tag)
        find_library(TAGLIB_LIBRARIES_DEBUG tagd)

        if(TAGLIB_LIBRARIES_RELEASE AND TAGLIB_LIBRARIES_DEBUG)
            set(TAGLIB_LIBRARIES optimized ${TAGLIB_LIBRARIES_RELEASE} debug ${TAGLIB_LIBRARIES_DEBUG})
        elseif(TAGLIB_LIBRARIES_RELEASE)
            set(TAGLIB_LIBRARIES ${TAGLIB_LIBRARIES_RELEASE})
        endif()

        if(TAGLIB_INCLUDE_DIR AND TAGLIB_LIBRARIES)
            set(TAGLIB_FOUND TRUE)
        endif()
    endif()

    if(TAGLIB_FOUND)
        set(TAGLIB_LIBRARIES ${TAGLIB_LIBRARIES} CACHE STRING "TabLib libraries" FORCE)
        if(NOT TagLib_FIND_QUIETLY)
            message(STATUS "Found TagLib: ${TAGLIB_LIBRARIES} ${TAGLIB_INCLUDE_DIR}")
        endif()
    else()
        if(TagLib_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find TagLib")
        else()
            if(NOT TagLib_FIND_QUIETLY)
                message(STATUS "Could NOT find TagLib")
            endif()
        endif()
    endif()
endif()
