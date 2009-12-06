cmake_minimum_required(VERSION 2.8)

project(Khopper)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x)
elseif(MSVC)
    add_definitions(/Zc:wchar_t-)
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(plugins)

add_dependencies(khopper khopper_core)
