find_package(FFmpeg REQUIRED)
find_package(Loki REQUIRED)

file(GLOB_RECURSE KHOPPER_CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
if(NOT WIN32)
    list(REMOVE_ITEM KHOPPER_CORE_SOURCES src/codec/wfile.cpp)
endif()
include_directories(include ${FFMPEG_INCLUDE_DIR} ${LOKI_INCLUDE_DIR})

add_library(k_core SHARED ${KHOPPER_CORE_SOURCES})
target_link_libraries(k_core ${FFMPEG_LIBRARIES} ${LOKI_LIBRARIES} ${QT_LIBRARIES})
