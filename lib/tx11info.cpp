/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2022 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "tx11info.h"

#include <X11/Xlib.h>

bool tX11Info::isPlatformX11() {
    return true;
}

Display* tX11Info::display() {
    return qApp->nativeInterface<QNativeInterface::QX11Application>()->display();
}

Window tX11Info::appRootWindow() {
    return DefaultRootWindow(qApp->nativeInterface<QNativeInterface::QX11Application>()->display());
}

unsigned long tX11Info::appTime() {
    return CurrentTime;
}

tX11Info::tX11Info(QObject* parent) :
    QObject{parent} {
}
