#ifndef THELIBS_GLOBAL_H
#define THELIBS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QObject>
#include <QSettings>

#ifdef QT_WIDGETS_LIB
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#endif

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
#define T_OS_UNIX_NOT_MAC
#endif

#ifdef T_OS_UNIX_NOT_MAC
#include <QDBusMessage>
#include <QDBusReply>
#include <QDBusConnection>
#endif

#if defined(THELIBS_LIBRARY)
#  define THELIBSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define THELIBSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define THE_LIBS_API_VERSION 3

#ifdef QT_WIDGETS_LIB
    #define SC_DPI(pixels) static_cast<int>(pixels * theLibsGlobal::getDPIScaling())
    #define SC_DPI_T(value, type) static_cast<type>(value * theLibsGloba::getDPIScaling())
#endif

class THELIBSSHARED_EXPORT theLibsGlobal : public QObject {
    Q_OBJECT

    public:
    static theLibsGlobal* instance();

#ifdef QT_WIDGETS_LIB
    static double getDPIScaling();
#endif
    static QStringList searchInPath(QString executable);

    public Q_SLOTS:
        bool powerStretchEnabled();
        bool allowSystemAnimations();

    private Q_SLOTS:
        void powerStretchChangedPrivate(bool isOn);

    Q_SIGNALS:
        void powerStretchChanged(bool isOn);

    private:
        theLibsGlobal();

        bool powerStretch = false;
        #ifdef T_OS_UNIX_NOT_MAC
            QSettings* themeSettings = new QSettings("theSuite", "ts-qtplatform");
        #endif

};

#endif // THELIBS_GLOBAL_H
