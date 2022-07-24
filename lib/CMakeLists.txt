project(lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

file(GLOB EXTRA_FUNCTIONS "${CMAKE_CURRENT_LIST_DIR}/cmake-scripts/*.cmake")
foreach(f ${EXTRA_FUNCTIONS})
    include(${f})
endforeach()

find_package(Qt6 REQUIRED COMPONENTS Widgets Svg Network Multimedia PrintSupport OPTIONAL_COMPONENTS DBus)
find_package(QCoro6 COMPONENTS Core Network OPTIONAL_COMPONENTS DBus)
find_library(CF_FRAMEWORK CoreFoundation)
find_library(COCOA_FRAMEWORK Cocoa)


if(NOT ${QCoro6Core_FOUND})
    message(FATAL_ERROR "QCoro6 library not found")
ENDIF()

cntp_find_pkgconfig(Unwind libunwind FAIL_ON_ANDROID)
cntp_find_pkgconfig(X11 x11)
#cntp_find_pkgconfig(GsettingsQt gsettings-qt)

add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

set(SOURCES
        taboutdialog.cpp
        tapplication.cpp
        tcircularspinner.cpp
        tconditionalwidget.cpp
        tcsdtools.cpp
        tdatetimepicker.cpp
        terrorflash.cpp
        libcontemporary_global.cpp
        thelpmenu.cpp
        ticon.cpp
        tinputdialog.cpp
        tjob.cpp
        tjobmanager.cpp
        tlocale.cpp
        tlogger.cpp
        tmessagebox.cpp
        tpaintcalculator.cpp
        tpopover.cpp
        tpromise.cpp
        tprintpopover.cpp
        tpropertyanimation.cpp
        tscrim.cpp
        tsettings.cpp
        tshortcuthud.cpp
        tstackedwidget.cpp
        tstatusframe.cpp
        tstylemanager.cpp
        tswitch.cpp
        tsystemsound.cpp
        tstandardlabel.cpp
        tsubtitlelabel.cpp
        ttitlelabel.cpp
        ttoast.cpp
        tvariantanimation.cpp
        tvirtualkeyboard.cpp
        twindowtabber.cpp
        twindowtabberbutton.cpp
        tx11info.cpp
        tcontentsizer.cpp
        private/debuglogpopover.cpp
        private/debuglogwindow.cpp
        private/nativeeventfilter.cpp
        private/translatorproxy.cpp
        tcsdtools/csdbuttonbox.cpp
        tcsdtools/csdsizegrip.cpp
        tdatetimepicker/datetimepart.cpp
        tdatetimepicker/datetimepartbutton.cpp
        tnotification/tnotification-common.cpp
        jobs/jobbutton.cpp
        jobs/jobspopover.cpp
        private/errorflashwidget.cpp private/errorflashwidget.ui

        tsettingswindow/tsettingswindow.cpp tsettingswindow/tsettingswindow.ui
        tsettingswindow/tsettingspane.cpp
        tsettingswindow/tsettingswindowdelegate.cpp

        taboutdialog.ui
        tinputdialog.ui
        tshortcuthud.ui
        tstatusframe.ui
        tprintpopover.ui
        private/debuglogpopover.ui
        private/debuglogwindow.ui

        thelibs_icons.qrc
        thelibs_translations.qrc

        "private/tmessageboxbackend.h")

set(HEADERS
        taboutdialog.h
        tapplication.h
        tcircularspinner.h
        tconditionalwidget.h
        tcsdtools.h
        tdatetimepicker.h
        terrorflash.h
        libcontemporary_global.h
        thelpmenu.h
        ticon.h
        tinputdialog.h
        tjob.h
        tjobmanager.h
        tlocale.h
        tlogger.h
        tmessagebox.h
        tnotification.h
        torderedmap.h
        tpaintcalculator.h
        tpopover.h
        tpromise.h
        tprintpopover.h
        tpropertyanimation.h
        tscrim.h
        tsettings.h
        tshortcuthud.h
        tstackedwidget.h
        tstatusframe.h
        tstylemanager.h
        tswitch.h
        tsystemsound.h
        ttitlelabel.h
        ttoast.h
        tvariantanimation.h
        tvirtualkeyboard.h
        twindowtabber.h
        twindowtabberbutton.h
        tx11info.h
        tstandardlabel.h
        tsubtitlelabel.h
        tcontentsizer.h
        plugins/plugininterface.h
        plugins/tpluginmanager.h

        tsettingswindow/tsettingswindow.h
        tsettingswindow/tsettingspane.h
    )


set(PRIVATE_HEADERS
    private/errorflashwidget.h
    tsettingswindow/tsettingswindowdelegate.h
)


if(${Qt6DBus_FOUND})
    qt_add_dbus_adaptor(SOURCES jobs/com.vicr123.thelibs.tjob.Manager.xml manageradaptor.h)
    qt_add_dbus_adaptor(SOURCES jobs/com.vicr123.thelibs.tjob.Job.xml jobadaptor.h)
endif()


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SOURCES ${SOURCES}
            tcsdtools/csdbuttonbox-objc.mm
            tcsdtools/tcsdtools-objc.mm
            tnotification/tnotification-mac.mm
            private/nsalertmac.mm
            private/tmessageboxbackend-mac.mm
            private/tapplicationmacprivate.mm)

    set(HEADERS ${HEADERS}
        private/tapplicationmacprivate.h)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(SOURCES ${SOURCES}
            tnotification/tnotification-linux.cpp
            jobs/jobdbus.cpp
            jobs/jobdbusmanager.cpp
            private/tmessageboxbackend-qt.cpp)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SOURCES ${SOURCES}
            tnotification/tnotification-win.cpp
            private/tmessageboxbackend-win.cpp
            private/tmessageboxbackend-win.h
    )
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    set(SOURCES ${SOURCES}
            tnotification/tnotification-android.cpp
            private/tmessageboxbackend-qt.cpp
    )
ENDIF()

add_library(libcontemporary SHARED ${SOURCES} ${HEADERS} ${PRIVATE_HEADERS})
cntp_target_public_headers(libcontemporary
        DESTINATION_DIRECTORY libcontemporary
        HEADERS ${HEADERS})
set_target_properties(libcontemporary PROPERTIES
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.vicr123.libcontemporary
        VERSION 6.0.0)
cntp_enable_coroutines(libcontemporary)

target_link_libraries(libcontemporary Qt6::Widgets Qt6::Svg Qt6::Network Qt6::Multimedia Qt6::PrintSupport QCoro::Coro QCoro::Core QCoro::Network)
target_compile_definitions(libcontemporary PRIVATE LIBCONTEMPORARY_LIBRARY)

if(${Qt6DBus_FOUND})
    target_link_libraries(libcontemporary Qt6::DBus)
    target_compile_definitions(libcontemporary PRIVATE HAVE_QT_DBUS)
ENDIF()

if(${QCoro6DBus_FOUND})
    target_link_libraries(libcontemporary QCoro::DBus)
ENDIF()

IF(${Unwind_FOUND})
    target_link_libraries(libcontemporary PkgConfig::Unwind)
    target_compile_definitions(libcontemporary PRIVATE HAVE_LIBUNWIND)
ENDIF()

IF(${X11_FOUND})
    IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        target_link_libraries(libcontemporary PkgConfig::X11)
        target_compile_definitions(libcontemporary PRIVATE HAVE_X11)
    ENDIF()
ENDIF()

#IF(${GsettingsQt_FOUND})
#    target_link_libraries(libcontemporary PkgConfig::GsettingsQt)
#    target_compile_definitions(libcontemporary PRIVATE HAVE_GSETTINGS)
#ENDIF()

IF(CF_FRAMEWORK_FOUND)
    target_link_libraries(libcontemporary CF_FRAMEWORK COCOA_FRAMEWORK)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(libcontemporary User32 Kernel32 Comctl32 DbgHelp windowsapp)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    get_target_property(LIBCONTEMPORARY_VERSION libcontemporary VERSION)

    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/libcontemporary.framework/Resources/CMake)
ELSE()
    set(CMAKECONFIG_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/libcontemporary)
ENDIF()

install(TARGETS libcontemporary
        EXPORT libcontemporaryTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${libcontemporary_INCLUDE_DIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})

configure_package_config_file(libcontemporaryConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/libcontemporaryConfig.cmake
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcontemporaryConfig.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(DIRECTORY cmake-scripts
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(EXPORT libcontemporaryTargets
        FILE libcontemporaryTargets.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR})
