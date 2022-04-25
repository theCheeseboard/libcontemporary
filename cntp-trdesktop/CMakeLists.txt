cmake_minimum_required(VERSION 3.21.0)

project(tltrdesktop VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Svg Network Multimedia)

add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(SOURCES
        desktopfile.cpp
        jsonfile.cpp
        main.cpp)

set(HEADERS
        "desktopfile.h"
        "jsonfile.h")

add_executable(cntp-trdesktop ${SOURCES} ${HEADERS})
set_target_properties(cntp-trdesktop PROPERTIES
        MACOSX_BUNDLE FALSE)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(cntp-trdesktop PROPERTIES
            INSTALL_RPATH ${CMAKE_INSTALL_LIBDIR})
ENDIF()

target_link_libraries(cntp-trdesktop Qt6::Widgets Qt6::DBus Qt6::Svg Qt6::Network Qt6::Multimedia libcontemporary)

install(TARGETS cntp-trdesktop
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
