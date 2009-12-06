find_package(OggVorbis REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_PANEL_OGG_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${KHOPPER_XIPH_DIR}/l_flac" "${CMAKE_SOURCE_DIR}/core/include" ${OGGVORBIS_INCLUDE_DIR})
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)

add_library(khopper_panel_ogg SHARED ${KHOPPER_PLUGIN_PANEL_OGG_SOURCES})
add_dependencies(khopper_panel_ogg khopper_core)
target_link_libraries(khopper_panel_ogg khopper_core ${OGGVORBIS_LIBRARIES})
