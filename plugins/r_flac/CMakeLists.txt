# flac reader

find_package(Flac REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_READER_FLAC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE KHOPPER_PLUGIN_READER_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories(${FLAC_INCLUDE_DIR})

add_library(khopper_reader_flac SHARED ${KHOPPER_PLUGIN_READER_FLAC_HEADERS} ${KHOPPER_PLUGIN_READER_FLAC_SOURCES})
khopper_add_plugin(khopper_reader_flac)
target_link_libraries(khopper_reader_flac ${FLAC_LIBRARIES})
