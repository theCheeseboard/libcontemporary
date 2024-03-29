#-------------------------------------------------
#
# Project created by QtCreator 2016-12-30T16:59:03
#
#-------------------------------------------------

QT       += widgets multimedia svg printsupport

TEMPLATE = lib
TARGET = contemporary

unix:!android {
    CONFIG += c++14

    system("pkg-config --version") {
        CONFIG += link_pkgconfig
        packagesExist(libunwind) {
            message("Building with libunwind support");
            PKGCONFIG += libunwind
            DEFINES += HAVE_LIBUNWIND
        } else {
            message("libunwind not found on this system.");
        }

        !macx:packagesExist(x11) {
            message("Building with X11 support");
            PKGCONFIG += x11
            DEFINES += HAVE_X11

            equals(QT_MAJOR_VERSION, 5) {
                QT += x11extras
            }
        } else {
            message("X11 not found on this system.");
        }

        packagesExist(gsettings-qt) {
            message("Building with GSettings support");
            PKGCONFIG += gsettings-qt
            DEFINES += HAVE_GSETTINGS
        } else {
            message("GSettings not found on this system.");
        }
    }
}

macx {
    CONFIG += c++14
    LIBS += -framework CoreFoundation -framework Cocoa

    SOURCES += tcsdtools/csdbuttonbox-objc.mm \
        tcsdtools/tcsdtools-objc.mm
}

win32 {
    CONFIG += c++17
    LIBS += -lUser32 -lKernel32 -lDbgHelp -lwindowsapp
    DEFINES += _WIN32_WINNT=0x0601 # Windows 7 or up
}

DEFINES += CNTP_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += tvariantanimation.cpp \
    jobs/jobbutton.cpp \
    jobs/jobspopover.cpp \
    private/debuglogpopover.cpp \
    private/debuglogwindow.cpp \
    private/translatorproxy.cpp \
    taboutdialog.cpp \
    tconditionalwidget.cpp \
    tcsdtools.cpp \
    tcsdtools/csdbuttonbox.cpp \
    tcsdtools/csdsizegrip.cpp \
    private/nativeeventfilter.cpp \
    tdatetimepicker.cpp \
    tdatetimepicker/datetimepart.cpp \
    tdatetimepicker/datetimepartbutton.cpp \
    tdocumenttabber.cpp \
    terrorflash.cpp \
    thelpmenu.cpp \
    ticon.cpp \
    tinputdialog.cpp \
    tjob.cpp \
    tjobmanager.cpp \
    tlocale.cpp \
    tlogger.cpp \
    tpaintcalculator.cpp \
    tprintpopover.cpp \
    tpromise.cpp \
    tpropertyanimation.cpp \
    thelibsglobal.cpp \
    tscrim.cpp \
    tsettings.cpp \
    tstatusframe.cpp \
    tstylemanager.cpp \
    ttitlelabel.cpp \
    ttoast.cpp \
    tvirtualkeyboard.cpp \
    tcircularspinner.cpp \
    tnotification/tnotification-common.cpp \
    tapplication.cpp \
    tshortcuthud.cpp \
    tstackedwidget.cpp \
    tpopover.cpp \
    tmessagebox.cpp \
    tswitch.cpp \
    tsystemsound.cpp \
    tx11info.cpp

HEADERS += tvariantanimation.h\
    jobs/jobbutton.h \
    jobs/jobspopover.h \
    private/debuglogpopover.h \
    private/debuglogwindow.h \
    private/translatorproxy.h \
    taboutdialog.h \
    tconditionalwidget.h \
    tcsdtools.h \
    tcsdtools/csdbuttonbox.h \
    tcsdtools/csdbuttonbox_p.h \
    tcsdtools/csdsizegrip.h \
    private/nativeeventfilter.h \
    tdatetimepicker.h \
    tdatetimepicker/datetimepart.h \
    tdatetimepicker/datetimepartbutton.h \
    tdocumenttabber.h \
    terrorflash.h \
    the-libs_global.h \
    thelpmenu.h \
    ticon.h \
    tinputdialog.h \
    tjob.h \
    tjobmanager.h \
    tlocale.h \
    tlogger.h \
    tpaintcalculator.h \
    tprintpopover.h \
    tpropertyanimation.h \
    tscrim.h \
    tsettings.h \
    tstatusframe.h \
    tstylemanager.h \
    ttitlelabel.h \
    ttoast.h \
    tnotification.h \
    tvirtualkeyboard.h \
    tcircularspinner.h \
    tapplication.h \
    tpromise.h \
    tshortcuthud.h \
    tstackedwidget.h \
    tpopover.h \
    tmessagebox.h \
    tswitch.h \
    tsystemsound.h \
    tx11info.h

DBUS_ADAPTORS += jobs/com.vicr123.thelibs.tjob.Manager.xml jobs/com.vicr123.thelibs.tjob.Job.xml

# Include required build tools
include($$PWD/prifiles/varset.pri)
include($$PWD/prifiles/gentranslations.pri)

# Translations are compiled in so installation is not required

# Install rules
header.files = *.h
module.path = $$CNTP_INSTALL_MODULES
prifiles.files = prifiles/*

rootcontemporary.files = libcontemporary.pri
rootcontemporary.path = $$CNTP_INSTALL_PREFIX/

unix {
    module.files = qt_libcontemporary.pri
}

unix:!macx:!android {
    QT += dbus

    target.path = $$CNTP_INSTALL_LIB
    header.path = $$CNTP_INSTALL_HEADERS/the-libs
    module.files = qt_thelib.pri
    prifiles.path = $$CNTP_INSTALL_PREFIX/share/the-libs/pri

    HEADERS += tnotification/tnotification-linux.h \
        jobs/jobdbus.h \
        jobs/jobdbusmanager.h

    SOURCES += tnotification/tnotification-linux.cpp \
        jobs/jobdbus.cpp \
        jobs/jobdbusmanager.cpp
}

macx {
    CONFIG(debug, debug|release): TARGET = contemporary_debug
    header.path = /usr/local/include/libcontemporary
    prifiles.path = /usr/local/share/libcontemporary/pri

    target.path = /usr/local/lib
    module.files = qt_thelib_mac.pri

    SOURCES += tnotification/tnotification-mac.mm
}

win32 {
    module.files = qt_thelib.pri

    CONFIG(debug, debug|release): TARGET = libcontemporaryd
    header.path = "C:/Program Files/libcontemporary/include"
    target.path = "C:/Program Files/libcontemporary/lib"
    prifiles.path = "C:/Program Files/libcontemporary/pri"

    SOURCES += tnotification/tnotification-win.cpp
    HEADERS += tnotification/tnotification-win.h
}

android {
    target.path = /libs/armeabi-v7a
    module.files = qt_thelib.pri
    module.path = /mkspecs/modules

    header.path = /include/libcontemporary
    prifiles.path = /share/libcontemporary/pri

    SOURCES += tnotification/tnotification-android.cpp
}

INSTALLS += target module header prifiles

DISTFILES += \
    libcontemporary.pri \
    prifiles/buildmaster.pri \
    prifiles/gentranslations.pri \
    prifiles/installtranslations.pri \
    prifiles/checkblueprint.pri \
    prifiles/varset.pri \
    qt_libcontemporary.pri

FORMS += \
    jobs/jobspopover.ui \
    private/debuglogpopover.ui \
    private/debuglogwindow.ui \
    taboutdialog.ui \
    tcsdtools/csdbuttonbox.ui \
    tdocumenttabber.ui \
    tinputdialog.ui \
    tprintpopover.ui \
    tshortcuthud.ui \
    tstatusframe.ui

RESOURCES += \
    thelibs_icons.qrc \
    thelibs_translations.qrc
