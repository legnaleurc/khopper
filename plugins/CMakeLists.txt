# plugins

include_directories("${CMAKE_SOURCE_DIR}/core/include")

function(KHOPPER_ADD_PLUGIN target)
    find_package(Qt4 4.7 QUIET)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
    add_definitions(-DKHOPPER_PLUGIN_ID=${target})
    add_dependencies(${target} khopper)
    target_link_libraries(${target} khopper ${QT_LIBRARIES})
    set_target_properties(${target} PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
    install(TARGETS ${target}
        RUNTIME DESTINATION "plugins"
        LIBRARY DESTINATION "lib/khopper/plugins")
endfunction()

set(KHOPPER_USE_FFMPEG_OR_LIBAV "ffmpeg" CACHE STRING "Use FFmpeg or Libav")
set_property(CACHE KHOPPER_USE_FFMPEG_OR_LIBAV PROPERTY STRINGS "ffmpeg" "libav" "libav_0_8")

add_subdirectory(cuesheet)
if("${KHOPPER_USE_FFMPEG_OR_LIBAV}" STREQUAL "ffmpeg")
    add_subdirectory(ffmpeg)
elseif("${KHOPPER_USE_FFMPEG_OR_LIBAV}" STREQUAL "libav")
    add_subdirectory(libav)
elseif("${KHOPPER_USE_FFMPEG_OR_LIBAV}" STREQUAL "libav_0_8")
    add_subdirectory(libav_0_8)
endif()
add_subdirectory(mp3lame)
add_subdirectory(redbook)
add_subdirectory(single)
add_subdirectory(xiph)
