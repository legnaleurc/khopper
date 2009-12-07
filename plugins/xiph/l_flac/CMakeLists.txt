find_package(Flac REQUIRED)
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")

file(GLOB_RECURSE KHOPPER_LIBRARY_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
include_directories("${CMAKE_SOURCE_DIR}/core/include" ${FLAC_INCLUDE_DIR})

add_library(khopper_writer_flac SHARED ${KHOPPER_LIBRARY_FLAC_SOURCES})
add_dependencies(khopper_writer_flac khopper_core)
target_link_libraries(khopper_writer_flac khopper_core ${FLAC_LIBRARIES})
set_target_properties(khopper_writer_flac PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
