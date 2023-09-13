#include "ttouchbarabstractitem.h"

struct tTouchBarAbstractItemPrivate {
        QString identifier;
        QString customizationLabel;
};

tTouchBarAbstractItem::tTouchBarAbstractItem(QString identifier, QString customizationLabel, QObject* parent) :
    QObject{parent} {
    d = new tTouchBarAbstractItemPrivate();
    d->identifier = identifier;
    d->customizationLabel = customizationLabel;
}

tTouchBarAbstractItem::~tTouchBarAbstractItem() {
    delete d;
}

QString tTouchBarAbstractItem::identifier() {
    return d->identifier;
}

QString tTouchBarAbstractItem::customizationLabel() {
    return d->customizationLabel;
}
