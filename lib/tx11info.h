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
#ifndef TX11INFO_H
#define TX11INFO_H

#include "the-libs_global.h"

#if defined(T_QT_5)
    #include <QX11Info>
#elif defined(T_QT_6)
    #include "tapplication.h"
#endif

typedef unsigned long Window;
class THELIBSSHARED_EXPORT tX11Info : public QObject {
        Q_OBJECT
    public:
        static bool isPlatformX11();
        static Display* display();
        static Window appRootWindow();
        static unsigned long appTime();

    signals:

    private:
        explicit tX11Info(QObject* parent = nullptr);
};

#endif // TX11INFO_H