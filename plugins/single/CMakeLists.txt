# single plugin

include(KhopperUtility)

#find TagLib
find_package(TagLib REQUIRED)
set_package_properties(TagLib
    PROPERTIES
    TYPE REQUIRED
    PURPOSE "Necessary for metadata reading")

file(GLOB_RECURSE KHOPPER_PLUGIN_SINGLE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE KHOPPER_PLUGIN_SINGLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
group_sources("src")

include_directories(${TAGLIB_INCLUDE_DIRS})

add_library(khopper_plugin_single SHARED ${KHOPPER_PLUGIN_SINGLE_HEADERS} ${KHOPPER_PLUGIN_SINGLE_SOURCES})
target_link_libraries(khopper_plugin_single ${TAGLIB_LIBRARIES})
khopper_add_plugin(khopper_plugin_single)
