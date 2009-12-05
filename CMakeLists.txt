cmake_minimum_required(VERSION 2.8)

project(Khopper)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x)
endif()

add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(plugins)
