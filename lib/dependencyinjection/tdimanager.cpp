#include "tdimanager.h"

#include "disingletonobject.h"
#include <QMap>
#include <tlogger.h>

struct tDIManagerPrivate {
        QMap<QString, DIDependentObject*> dependencies;
};

tDIManager::tDIManager(QObject* parent) :
    QObject{parent} {
    d = new tDIManagerPrivate();
}

void tDIManager::addSingleton(QMetaObject interface, QMetaObject implementation) {
    auto interfaceName = QString::fromUtf8(interface.className());
    if (d->dependencies.contains(interfaceName)) {
        tCritical("tDIManager") << "Attempting to register two instances of " << interfaceName;
        return;
    }

    auto dependentObject = new DISingletonObject(implementation, this);
    d->dependencies.insert(interfaceName, dependentObject);
}

tInjectedPointer<QObject> tDIManager::requiredService(QMetaObject interface) {
    auto interfaceName = QString::fromUtf8(interface.className());
    return tInjectedPointer<QObject>(internalRequiredService(interfaceName));
}

QSharedPointer<QObject> tDIManager::internalRequiredService(QString interface) {
    if (!d->dependencies.contains(interface)) {
        return {};
    }

    auto dependentObject = d->dependencies.value(interface);
    return dependentObject->getInstance();
}
