cmake_minimum_required(VERSION 3.4.0)

project(lib VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include(GNUInstallDirs)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets DBus Svg Network Multimedia)
find_package(PkgConfig REQUIRED)
find_library(CF_FRAMEWORK CoreFoundation)
find_library(COCOA_FRAMEWORK Cocoa)

pkg_check_modules(Unwind IMPORTED_TARGET libunwind)
pkg_check_modules(X11 IMPORTED_TARGET x11)
pkg_check_modules(GsettingsQt IMPORTED_TARGET gsettings-qt)

add_compile_definitions(SYSTEM_LIBRARY_DIRECTORY="/usr/lib")

set(SOURCES
        taboutdialog.cpp
        tapplication.cpp
        tcircularspinner.cpp
        tconditionalwidget.cpp
        tcsdtools.cpp
        tdatetimepicker.cpp
        tdocumenttabber.cpp
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
        tpropertyanimation.cpp
        tscrim.cpp
        tsettings.cpp
        tshortcuthud.cpp
        tstackedwidget.cpp
        tstatusframe.cpp
        tstylemanager.cpp
        tswitch.cpp
        tsystemsound.cpp
        ttitlelabel.cpp
        ttoast.cpp
        tvariantanimation.cpp
        tvirtualkeyboard.cpp
        private/debuglogpopover.cpp
        private/debuglogwindow.cpp
        private/nativeeventfilter.cpp
        private/translatorproxy.cpp
        tcsdtools/csdbuttonbox.cpp
        tcsdtools/csdsizegrip.cpp
        tdatetimepicker/datetimepart.cpp
        tdatetimepicker/datetimepartbutton.cpp
        jobs/jobbutton.cpp
        jobs/jobspopover.cpp

        taboutdialog.ui
        tdocumenttabber.ui
        tinputdialog.ui
        tshortcuthud.ui
        tstatusframe.ui
        private/debuglogpopover.ui
        private/debuglogwindow.ui

        thelibs_icons.qrc
        thelibs_translations.qrc)

set(HEADERS
        "taboutdialog.h"
        "tapplication.h"
        "tcircularspinner.h"
        "tconditionalwidget.h"
        "tcsdtools.h"
        "tdatetimepicker.h"
        "tdocumenttabber.h"
        "terrorflash.h"
        "libcontemporary_global.h"
        "thelpmenu.h"
        "ticon.h"
        "tinputdialog.h"
        "tjob.h"
        "tjobmanager.h"
        "tlocale.h"
        "tlogger.h"
        "tmessagebox.h"
        "tpaintcalculator.h"
        "tpopover.h"
        "tpromise.h"
        "tpropertyanimation.h"
        "tscrim.h"
        "tsettings.h"
        "tshortcuthud.h"
        "tstackedwidget.h"
        "tstatusframe.h"
        "tstylemanager.h"
        "tswitch.h"
        "tsystemsound.h"
        "ttitlelabel.h"
        "ttoast.h"
        "tvariantanimation.h"
        "tvirtualkeyboard.h")

qt_add_dbus_adaptor(SOURCES jobs/com.vicr123.thelibs.tjob.Manager.xml manageradaptor.h)
qt_add_dbus_adaptor(SOURCES jobs/com.vicr123.thelibs.tjob.Job.xml jobadaptor.h)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SOURCES ${SOURCES}
            tcsdtools/csdbuttonbox-objc.mm
            tcsdtools/tcsdtools-objc.mm
            tnotification/tnotification-mac.mm)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(SOURCES ${SOURCES}
            tnotification/tnotification-linux.cpp
            jobs/jobdbus.cpp
            jobs/jobdbusmanager.cpp)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(SOURCES ${SOURCES}
            tnotification/tnotification-win.cpp)
ENDIF()

add_library(contemporary SHARED ${SOURCES} ${HEADERS})
set_target_properties(contemporary PROPERTIES
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER com.vicr123.libcontemporary
        VERSION 6.0.0
#        SOVERSION 6.0.0
        PUBLIC_HEADER "${HEADERS}")

target_link_libraries(contemporary Qt6::Widgets Qt6::DBus Qt6::Svg Qt6::Network Qt6::Multimedia)
IF(${Unwind_FOUND})
    target_link_libraries(contemporary PkgConfig::Unwind)
    add_compile_definitions(HAVE_LIBUNWIND)
ENDIF()

IF(${X11_FOUND} AND ${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(contemporary PkgConfig::X11)
    add_compile_definitions(HAVE_X11)
ENDIF()

IF(${GsettingsQt_FOUND})
    target_link_libraries(contemporary PkgConfig::GsettingsQt)
    add_compile_definitions(HAVE_GSETTINGS)
ENDIF()

IF(CF_FRAMEWORK_FOUND)
    target_link_libraries(contemporary CF_FRAMEWORK COCOA_FRAMEWORK)
ENDIF()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(contemporary User32 Kernel32 DbgHelp windowsapp)
ENDIF()

install(TARGETS contemporary
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/libcontemporary
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FRAMEWORK DESTINATION ${CMAKE_INSTALL_LIBDIR})