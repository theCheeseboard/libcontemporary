cmake_minimum_required(VERSION 3.4.0)

project(lib VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(libcontemporary REQUIRED)

set(SOURCES
)

set(HEADERS
    lib$($LCPNAME)_global.h
)

set(PRIVATE_HEADERS

)

add_library(lib$($LCPNAME) SHARED ${SOURCES} ${HEADERS} ${PRIVATE_HEADERS})
cntp_init(lib$($LCPNAME) 20)
set_target_properties(lib$($LCPNAME) PROPERTIES
        OUTPUT_NAME $($LCPNAME)
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER $($desktop-id)
        VERSION 1.0.0
        PUBLIC_HEADER "${HEADERS}")

target_link_libraries(lib$($LCPNAME) Qt::Widgets)
target_compile_definitions(lib$($LCPNAME) PRIVATE LIB$($UCPNAME)_LIBRARY)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/lib$($LCPNAME).framework/Resources/CMake)
    set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
    set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/lib$($LCPNAME).framework/Headers)
    set(LIBRARY_INSTALL_DIR ../)
ELSE()
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/lib$($LCPNAME))
    set(HEADER_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/lib$($LCPNAME))
    set(INCLUDE_INSTALL_DIR ${HEADER_INSTALL_DIR})
    set(LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
ENDIF()

configure_package_config_file(lib$($LCPNAME)Config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/lib$($LCPNAME)Config.cmake
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
        PATH_VARS HEADER_INSTALL_DIR LIBRARY_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib$($LCPNAME)Config.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(EXPORT lib$($LCPNAME)Targets
        FILE lib$($LCPNAME)Targets.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(TARGETS lib$($LCPNAME)
        EXPORT lib$($LCPNAME)Targets
        PUBLIC_HEADER DESTINATION ${HEADER_INSTALL_DIR}
        INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})

cntp_mark_support_library_directory()
