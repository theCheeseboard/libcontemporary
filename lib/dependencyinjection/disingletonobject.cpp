#include "disingletonobject.h"

#include <QSharedPointer>

struct DISingletonObjectPrivate {
        QSharedPointer<QObject> object;
};

DISingletonObject::DISingletonObject(QMetaObject constructible, tDIManager* diManager) :
    DIDependentObject{constructible, diManager} {
    d = new DISingletonObjectPrivate();
}

QSharedPointer<QObject> DISingletonObject::getInstance() {
    if (!d->object) {
        d->object = this->construct();
    }

    return d->object;
}
