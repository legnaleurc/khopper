find_package(Lame REQUIRED)
find_package(TagLib REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_PANEL_MP3_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${CMAKE_SOURCE_DIR}/core/include" ${LAME_INCLUDE_DIR} ${TAGLIB_INCLUDE_DIR})
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
add_definitions(-DKHOPPER_PLUGIN_ID=khopper_panel_mp3)

qt4_wrap_cpp(KHOPPER_PLUGIN_PANEL_MP3_MOC_SOURCES mp3panel.hpp)

add_library(khopper_panel_mp3 SHARED ${KHOPPER_PLUGIN_PANEL_MP3_SOURCES} ${KHOPPER_PLUGIN_PANEL_MP3_MOC_SOURCES})
add_dependencies(khopper_panel_mp3 khopper_core)
target_link_libraries(khopper_panel_mp3 khopper_core ${LAME_LIBRARIES} ${TAGLIB_LIBRARIES})
set_target_properties(khopper_panel_mp3 PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
