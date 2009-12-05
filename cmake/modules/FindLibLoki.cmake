# - Find libloki
# This module can be used to find libloki
#
# Below is a detailed list of variables that FindLibLoki.cmake sets.
# LIBLOKI_FOUND       If false, don't try to use libloki
# LIBLOKI_INCLUDE_DIR Path to "include" of libloki
# LIBLOKI_LIBRARIES   The libloki libraries

find_path(LIBLOKI_INCLUDE_DIR NAMES loki/Factory.h loki/Singleton.h)
find_library(LIBLOKI_LIBRARIES NAMES loki)

if(LIBLOKI_INCLUDE_DIR AND LIBLOKI_LIBRARIES)
    message(STATUS "Found libloki")
    set(LIBLOKI_FOUND TRUE)
else()
    message(STATUS "Find no libloki")
endif()
