find_package(Flac REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_READER_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${CMAKE_SOURCE_DIR}/core/include" ${FLAC_INCLUDE_DIR})
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)

add_library(khopper_reader_flac SHARED ${KHOPPER_PLUGIN_READER_FLAC_SOURCES})
add_dependencies(khopper_reader_flac khopper_core)
target_link_libraries(khopper_reader_flac khopper_core ${FLAC_LIBRARIES})
set_target_properties(khopper_reader_flac PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
