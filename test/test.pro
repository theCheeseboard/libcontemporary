/*******************************************************************************
 *
 * libcontemporary - Common libraries
 * Copyright (C) 2022 Victor Tran
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T20:40:49
#
#-------------------------------------------------

QT       += core widgets testlib dbus

TARGET = tst_unittesting
CONFIG   += console testcase c++14
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        tst_unittesting.cpp

win32 {
    CONFIG(release, debug|release) {
        LIBS += -L$$OUT_PWD/../lib/release/ -lthe-libs
    } else {
        LIBS += -L$$OUT_PWD/../lib/debug/ -lthe-libs
    }
} else {
    mac {
        LIBS += $$OUT_PWD/../lib/libthe-libs.dylib
    } else {
        unix {
            LIBS += -L$$OUT_PWD/../lib/ -lthe-libs
        }
    }
}

INCLUDEPATH += $$PWD/../lib
DEPENDPATH += $$PWD/../lib
