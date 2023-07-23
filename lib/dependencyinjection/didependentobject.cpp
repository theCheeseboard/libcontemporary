#include "didependentobject.h"

#include "tdimanager.h"
#include <QMetaMethod>
#include <QSharedPointer>
#include <tlogger.h>

struct DIDependentObjectPrivate {
        QMetaObject constructible;
        QMetaMethod constructor;
        QStringList dependencies;

        tDIManager* diManager;
};

DIDependentObject::DIDependentObject(QMetaObject constructible, tDIManager* diManager) :
    QObject{nullptr} {
    d = new DIDependentObjectPrivate();
    d->constructible = constructible;
    d->diManager = diManager;

    auto name = QString::fromUtf8(d->constructible.className());
    auto constructors = d->constructible.constructorCount();

    for (auto i = 0; i < d->constructible.constructorCount(); i++) {
        auto constructor = d->constructible.constructor(i);
        if (constructor.parameterCount() == 0) {
            d->constructor = constructor;
            break;
        }
    }

    if (!d->constructor.isValid()) {
        // The constructible has no valid constructor
        tCritical("DIDependentObject") << "Class " << constructible.className() << " registered for dependency injection has no default constructor";
    }
}

QSharedPointer<tDIBaseInterface> DIDependentObject::construct() {
    tBaseDIManager::pushCurrentDIManager(d->diManager);
    auto returnValue = d->constructible.newInstance();
    tBaseDIManager::popCurrentDIManager();
    return QSharedPointer<tDIBaseInterface>(dynamic_cast<tDIBaseInterface*>(returnValue));
}
