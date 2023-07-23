#include "didependentobject.h"

#include "tdimanager.h"
#include <QMetaMethod>
#include <QSharedPointer>
#include <tlogger.h>

struct DIDependentObjectPrivate {
        QMetaObject constructible;
        QStringList dependencies;

        tDIManager* diManager;
};

DIDependentObject::DIDependentObject(QMetaObject constructible, tDIManager* diManager) :
    QObject{nullptr} {
    d = new DIDependentObjectPrivate();
    d->constructible = constructible;
    d->diManager = diManager;
}

QSharedPointer<tDIBaseInterface> DIDependentObject::construct() {
    tBaseDIManager::pushCurrentDIManager(d->diManager);
    auto returnValue = d->constructible.newInstance();
    tBaseDIManager::popCurrentDIManager();
    return QSharedPointer<tDIBaseInterface>(dynamic_cast<tDIBaseInterface*>(returnValue));
}
