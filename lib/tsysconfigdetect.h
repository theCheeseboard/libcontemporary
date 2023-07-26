#ifndef TSYSCONFIGDETECT_H
#define TSYSCONFIGDETECT_H

#include <QObject>

// Qt Version
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0) && QT_VERSION < QT_VERSION_CHECK(5, 99, 99)
    #define T_QT_5
#elif QT_VERSION > QT_VERSION_CHECK(6, 0, 0)
    #define T_QT_6
#endif

// Operating System
#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC) && !defined(Q_OS_ANDROID)
    #define T_OS_UNIX_NOT_MAC
#endif

// System Architecture
// MSVC
#if defined(_M_AMD64)
    #define T_ARCH_X86
    #define T_ARCH_X86_64
#endif

#if defined(_M_ARM64)
    #define T_ARCH_ARM
    #define T_ARCH_AARCH64
#endif

#endif // TSYSCONFIGDETECT_H
