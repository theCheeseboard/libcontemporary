#include "tdimanager.h"

#include "disingletonobject.h"
#include "tinjectedpointer.h"
#include <QMap>
#include <tlogger.h>

struct tDIManagerPrivate {
        QMap<QString, DIDependentObject*> dependencies;
};

tDIManager::tDIManager(QObject* parent) :
    tBaseDIManager{parent} {
    d = new tDIManagerPrivate();
    tBaseDIManager::pushCurrentDIManager(this);
}

tDIManager::~tDIManager() {
    tBaseDIManager::popCurrentDIManager();
}

void tDIManager::addSingleton(QMetaObject interface, QMetaObject implementation) {
    pushDependency(interface, new DISingletonObject(implementation, this));
}

tInjectedPointer<tDIBaseInterface> tDIManager::requiredService(QMetaObject interface) {
    auto interfaceName = QString::fromUtf8(interface.className());
    return tInjectedPointer<tDIBaseInterface>(internalRequiredService(interfaceName));
}

QSharedPointer<tDIBaseInterface> tDIManager::internalRequiredService(QString interface) {
    if (!d->dependencies.contains(interface)) {
        return {};
    }

    auto dependentObject = d->dependencies.value(interface);
    return dependentObject->getInstance();
}

void tDIManager::pushDependency(QMetaObject interface, DIDependentObject* dependentObject) {
    auto interfaceName = QString::fromUtf8(interface.className());
    if (d->dependencies.contains(interfaceName)) {
        tCritical("tDIManager") << "Attempting to register two instances of " << interfaceName;
        return;
    }

    d->dependencies.insert(interfaceName, dependentObject);
}
