#ifndef NATIVEEVENTFILTER_H
#define NATIVEEVENTFILTER_H

#include "libcontemporary_global.h"
#include <QAbstractNativeEventFilter>
#include <QObject>

namespace theLibsPrivate {
    class NativeEventFilter : public QObject,
                              public QAbstractNativeEventFilter {
            Q_OBJECT
        public:
            explicit NativeEventFilter(QObject* parent = nullptr);
        signals:
            void powerStretchChanged(bool powerStretchOn);

        public slots:

        private:
            bool filter(const QByteArray& eventType, void* message);

            // QAbstractNativeEventFilter interface
        public:
#if defined(T_QT_5)
            bool nativeEventFilter(const QByteArray& eventType, void* message, long* result);
#elif defined(T_QT_6)
            bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result);
#endif
    };
} // namespace theLibsPrivate

#endif // NATIVEEVENTFILTER_H
