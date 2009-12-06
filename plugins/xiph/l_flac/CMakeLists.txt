find_package(Flac REQUIRED)

file(GLOB_RECURSE KHOPPER_LIBRARY_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
include_directories("${CMAKE_SOURCE_DIR}/core/include" ${FLAC_INCLUDE_DIR})

add_library(l_flac SHARED ${KHOPPER_LIBRARY_FLAC_SOURCES})
target_link_libraries(l_flac ${FLAC_LIBRARIES})
