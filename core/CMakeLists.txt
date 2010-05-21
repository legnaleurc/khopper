# core

find_package(FFmpeg REQUIRED)
find_package(Loki REQUIRED)

# hack MSCV path
if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DKHOPPER_LIBRARY)

file(GLOB_RECURSE KHOPPER_CORE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE KHOPPER_CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(KHOPPER_CORE_MOC_HEADERS include/application.hpp)

qt4_wrap_cpp(KHOPPER_CORE_MOC_SOURCES ${KHOPPER_CORE_MOC_HEADERS})

if(NOT WIN32)
    list(REMOVE_ITEM KHOPPER_CORE_SOURCES src/codec/wfile.cpp)
endif()
include_directories(include src ${FFMPEG_INCLUDE_DIR} ${LOKI_INCLUDE_DIR})

source_group("Generated Files" FILES ${KHOPPER_CORE_MOC_SOURCES})

add_library(khopper_core SHARED ${KHOPPER_CORE_HEADERS} ${KHOPPER_CORE_SOURCES} ${KHOPPER_CORE_MOC_SOURCES})
target_link_libraries(khopper_core ${FFMPEG_LIBRARIES} ${LOKI_LIBRARIES} ${QT_LIBRARIES})
set_target_properties(khopper_core PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})