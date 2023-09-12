#include "ttouchbarabstractitem.h"

struct tTouchBarAbstractItemPrivate {
        QString identifier;
};

tTouchBarAbstractItem::tTouchBarAbstractItem(QString identifier, QObject* parent) :
    QObject{parent} {
    d = new tTouchBarAbstractItemPrivate();
    d->identifier = identifier;
}

tTouchBarAbstractItem::~tTouchBarAbstractItem() {
    delete d;
}

QString tTouchBarAbstractItem::identifier() {
    return d->identifier;
}
