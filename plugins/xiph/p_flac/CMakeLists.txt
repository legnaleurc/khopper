file(GLOB_RECURSE KHOPPER_PLUGIN_PANEL_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${KHOPPER_XIPH_DIR}/l_flac" "${CMAKE_SOURCE_DIR}/core/include")
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)

add_library(khopper_panel_flac SHARED ${KHOPPER_PLUGIN_PANEL_FLAC_SOURCES})
add_dependencies(khopper_panel_flac khopper_core)
target_link_libraries(khopper_panel_flac khopper_core)
set_target_properties(khopper_panel_flac PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
