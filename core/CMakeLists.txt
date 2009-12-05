find_package(LibAv REQUIRED)
find_package(LibLoki REQUIRED)

file(GLOB_RECURSE KHOPPER_CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
if(NOT WIN32)
    list(REMOVE_ITEM KHOPPER_CORE_SOURCES src/codec/wfile.cpp)
endif()
include_directories(include ${LIBAV_INCLUDE_DIR} ${LIBLOKI_INCLUDE_DIR})

add_library(k_core SHARED ${KHOPPER_CORE_SOURCES})
target_link_libraries(k_core ${LIBAV_LIBRARIES} ${LIBLOKI_LIBRARIES} ${QT_LIBRARIES})
