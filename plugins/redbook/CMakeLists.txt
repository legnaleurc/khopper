# redbook plugin

include(KhopperUtility)

file(GLOB_RECURSE KHOPPER_PLUGIN_REDBOOK_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE KHOPPER_PLUGIN_REDBOOK_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
group_sources("src")

add_library(khopper_plugin_redbook SHARED ${KHOPPER_PLUGIN_REDBOOK_HEADERS} ${KHOPPER_PLUGIN_REDBOOK_SOURCES})
khopper_add_plugin(khopper_plugin_redbook)
