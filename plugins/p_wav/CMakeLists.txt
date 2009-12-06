file(GLOB_RECURSE KHOPPER_PLUGIN_PANEL_WAV_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${CMAKE_SOURCE_DIR}/core/include")
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)

add_library(khopper_panel_wav SHARED ${KHOPPER_PLUGIN_PANEL_WAV_SOURCES})
add_dependencies(khopper_panel_wav khopper_core)
target_link_libraries(khopper_panel_wav khopper_core)
set_target_properties(khopper_panel_wav PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
