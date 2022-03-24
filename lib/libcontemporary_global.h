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

#ifndef THELIBS_GLOBAL_H
#define THELIBS_GLOBAL_H

#include <QObject>
#include <QSettings>
#include <QtCore/qglobal.h>

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(5, 99, 99)
    #define T_QT_5
#elif QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    #define T_QT_6
#endif

#ifdef QT_WIDGETS_LIB
    #include <QApplication>
    #include <QStyle>
    #include <QStyleFactory>
#endif

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID)
    #define T_OS_UNIX_NOT_MAC
#endif

#ifdef T_OS_UNIX_NOT_MAC
    #include <QDBusConnection>
    #include <QDBusMessage>
    #include <QDBusReply>
#endif

#if defined(THELIBS_LIBRARY)
    #define THELIBSSHARED_EXPORT Q_DECL_EXPORT
#else
    #define THELIBSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define THE_LIBS_API_VERSION 9
#define THE_LIBS_VERSION "6.0"

#ifdef QT_WIDGETS_LIB
    #define SC_DPI(pixels) static_cast<int>(pixels * theLibsGlobal::getDPIScaling())
    #define SC_DPI_T(value, type) static_cast<type>(value * theLibsGlobal::getDPIScaling())
#endif

struct theLibsGlobalPrivate;
class THELIBSSHARED_EXPORT theLibsGlobal : public QObject {
        Q_OBJECT

    public:
        static theLibsGlobal* instance();

#ifdef QT_WIDGETS_LIB
        static double getDPIScaling();
        static void tintImage(QImage& image, QColor tint);
#endif
        static QStringList searchInPath(QString executable);

        static QColor lineColor(QColor textColor);

    public Q_SLOTS:
        bool powerStretchEnabled();
        bool allowSystemAnimations();

    private Q_SLOTS:
        void powerStretchChangedPrivate(bool isOn);
        void dbusPropertyChangedPrivate(QString interfaceName, QMap<QString, QVariant> changedProperties, QStringList invalidatedProperties);

    Q_SIGNALS:
        void powerStretchChanged(bool isOn);

    private:
        theLibsGlobal();

        theLibsGlobalPrivate* d;
};

#endif // THELIBS_GLOBAL_H
