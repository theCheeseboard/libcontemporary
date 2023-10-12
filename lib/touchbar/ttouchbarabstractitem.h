#ifndef TTOUCHBARABSTRACTITEM_H
#define TTOUCHBARABSTRACTITEM_H

#include "ttouchbar_forwarddeclares.h"
#include <QObject>

struct tTouchBarAbstractItemPrivate;
class tTouchBarAbstractItem : public QObject {
        Q_OBJECT
    public:
        explicit tTouchBarAbstractItem(QString identifier, QString customizationLabel, QObject* parent = nullptr);
        ~tTouchBarAbstractItem();

        QString identifier();
        QString customizationLabel();

        virtual NSTouchBarItem* makeItem() = 0;

    signals:
        void invalidateTouchBar();

    private:
        tTouchBarAbstractItemPrivate* d;
};

#endif // TTOUCHBARABSTRACTITEM_H
