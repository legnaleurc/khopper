# - Find taglib
# This module can be used to find taglib
# Once done this will define
#
# TAGLIB_FOUND        If false, don't try to use taglib
#
# TAGLIB_INCLUDE_DIRS The taglib include directory
# TAGLIB_LIBRARIES    Link these to use taglib

include(KhopperUtility)

if(NOT TAGLIB_FOUND)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_TAGLIB taglib)
        if(PC_TAGLIB_FOUND)
            set(TAGLIB_INCLUDE_DIRS ${PC_TAGLIB_INCLUDE_DIRS})
            set(TAGLIB_LIBRARIES ${PC_TAGLIB_LIBRARIES})
        endif()
    else()
        path_from_env(TAGLIB_ROOT)

        find_path(TAGLIB_INCLUDE_DIRS id3v2tag.h
            HINTS ${TAGLIB_ROOT}
            PATH_SUFFIXES "include/taglib")
        find_library(TAGLIB_LIBRARIES_RELEASE tag
            HINTS ${TAGLIB_ROOT}
            PATH_SUFFIXES "lib")
        find_library(TAGLIB_LIBRARIES_DEBUG tagd
            HINTS ${TAGLIB_ROOT}
            PATH_SUFFIXES "lib")

        if(TAGLIB_LIBRARIES_RELEASE AND TAGLIB_LIBRARIES_DEBUG)
            set(TAGLIB_LIBRARIES optimized ${TAGLIB_LIBRARIES_RELEASE} debug ${TAGLIB_LIBRARIES_DEBUG})
        elseif(TAGLIB_LIBRARIES_RELEASE)
            set(TAGLIB_LIBRARIES ${TAGLIB_LIBRARIES_RELEASE})
        endif()
    endif()

    include(FindPackageHandleStandardArgs)

    find_package_handle_standard_args(TAGLIB DEFAULT_MSG TAGLIB_INCLUDE_DIRS TAGLIB_LIBRARIES)

    mark_as_advanced(TAGLIB_INCLUDE_DIRS TAGLIB_LIBRARIES)
    message(STATUS ${TAGLIB_LIBRARIES})
endif()
