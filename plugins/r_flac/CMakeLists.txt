find_package(Flac REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_READER_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${CMAKE_SOURCE_DIR}/core/include" ${FLAC_INCLUDE_DIR})
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)

add_library(r_flac SHARED ${KHOPPER_PLUGIN_READER_FLAC_SOURCES})
target_link_libraries(r_flac ${FLAC_LIBRARIES})
