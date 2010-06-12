# core

find_package(Loki REQUIRED)
# use QtMultimedia
set(QT_USE_QTMULTIMEDIA TRUE)

# hack MSCV path
if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DKHOPPER_LIBRARY)

file(GLOB_RECURSE KHOPPER_CORE_PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.hpp")
file(GLOB_RECURSE KHOPPER_CORE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE KHOPPER_CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
set(KHOPPER_CORE_MOC_HEADERS
    "include/khopper/application.hpp"
    "src/plugin/pluginmanager.hpp")

qt4_wrap_cpp(KHOPPER_CORE_MOC_SOURCES ${KHOPPER_CORE_MOC_HEADERS})

include_directories("include/khopper" src ${LOKI_INCLUDE_DIR})

source_group("Generated Files" FILES ${KHOPPER_CORE_MOC_SOURCES})

add_library(khopper_core SHARED ${KHOPPER_CORE_HEADERS} ${KHOPPER_CORE_SOURCES} ${KHOPPER_CORE_MOC_SOURCES})
target_link_libraries(khopper_core ${LOKI_LIBRARIES} ${QT_LIBRARIES} ${QT_QTMULTIMEDIA_LIBRARY})
set_target_properties(khopper_core
    PROPERTIES
        VERSION ${KHOPPER_VERSION}
        SOVERSION ${KHOPPER_VERSION}
        FRAMEWORK TRUE
        PUBLIC_HEADER "${KHOPPER_CORE_PUBLIC_HEADERS}")

install(TARGETS khopper_core
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    FRAMEWORK DESTINATION "/Library/Frameworks"
    PUBLIC_HEADER DESTINATION "include/khopper")
