set(QT_USE_PHONON TRUE)

file(GLOB_RECURSE KHOPPER_MAIN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
include_directories(
    src/album
    src/widget
    "${CMAKE_SOURCE_DIR}/core/include"
    ${QT_PHONON_INCLUDE_DIR})
link_directories("${CMAKE_SOURCE_DIR}/core")

add_executable(khopper ${KHOPPER_MAIN_SOURCES})
target_link_libraries(khopper k_core ${QT_LIBRARIES})
