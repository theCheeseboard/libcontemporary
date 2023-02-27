cmake_minimum_required(VERSION 3.21.0)

set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS Core)

set(SOURCES
    main.cpp
    diskimage.cpp
)

set(HEADERS
    diskimage.h
)

add_executable(cntp-macpack ${SOURCES} ${HEADERS})
set_target_properties(cntp-macpack PROPERTIES
        MACOSX_BUNDLE FALSE)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(cntp-macpack PROPERTIES
            INSTALL_RPATH_USE_LINK_PATH True)
ENDIF()

target_link_libraries(cntp-macpack libcontemporary Qt6::Core)

install(TARGETS cntp-macpack
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
