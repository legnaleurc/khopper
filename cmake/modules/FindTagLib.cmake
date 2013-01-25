# - Find taglib
# This module can be used to find taglib
# Once done this will define
#
# TAGLIB_FOUND        If false, don't try to use taglib
#
# TAGLIB_INCLUDE_DIRS The taglib include directory
# TAGLIB_LIBRARIES    Link these to use taglib

include(KhopperUtility)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_TAGLIB QUIET taglib)
    if(PC_TAGLIB_FOUND)
        set(TAGLIB_INCLUDE_DIRS ${PC_TAGLIB_INCLUDE_DIRS})
        set(TAGLIB_LIBRARIES ${PC_TAGLIB_LIBRARIES})
    endif()
endif()

path_from_env(TAGLIB_ROOT)

if(NOT TAGLIB_INCLUDE_DIRS)
    find_path(TAGLIB_INCLUDE_DIRS id3v2tag.h
        HINTS ${TAGLIB_ROOT}
        PATH_SUFFIXES "include/taglib")
endif()

if(NOT TAGLIB_LIBRARIES)
    find_library(TAGLIB_LIBRARY_RELEASE tag
        HINTS ${TAGLIB_ROOT}
        PATH_SUFFIXES "lib")
    find_library(TAGLIB_LIBRARY_DEBUG tagd
        HINTS ${TAGLIB_ROOT}
        PATH_SUFFIXES "lib")

    include(SelectLibraryConfigurations)
    select_library_configurations(TAGLIB)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TAGLIB DEFAULT_MSG TAGLIB_INCLUDE_DIRS TAGLIB_LIBRARIES)

mark_as_advanced(TAGLIB_INCLUDE_DIRS TAGLIB_LIBRARIES)
