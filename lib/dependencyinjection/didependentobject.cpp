#include "didependentobject.h"

#include "tdimanager.h"
#include <QMetaMethod>
#include <QSharedPointer>
#include <tlogger.h>

struct DIDependentObjectPrivate {
        QMetaObject constructible;
        QMetaMethod constructor;
        QMetaMethod injectConstructor;
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

    d->injectConstructor = d->constructible.method(d->constructible.indexOfMethod("cntp_inject_construct(QList<tInjectedPointer<QObject>>)"));
    if (d->constructible.constructorCount() > 1) {
        for (auto i = 0; i < d->constructible.constructorCount(); i++) {
            //            if (d->constructible.constructor(i).tag())
        }
    } else if (d->constructible.constructorCount() == 1) {
        d->constructor = d->constructible.constructor(0);
    }

    if (!d->injectConstructor.isValid()) {
        // The constructible has no valid constructor
        tCritical("DIDependentObject") << "Class " << constructible.className() << " registered for dependency injection was not declared with T_INJECTABLE macro";
    }
    if (!d->constructor.isValid()) {
        // The constructible has no valid constructor
        tCritical("DIDependentObject") << "Class " << constructible.className() << " registered for dependency injection has no constructors";
    }
}

QSharedPointer<QObject> DIDependentObject::construct() {
    QList<tInjectedPointer<QObject>> args;

    for (auto i = 0; i < d->constructor.parameterCount(); i++) {
        auto metatype = d->constructor.parameterMetaType(i);
        auto paramName = d->constructor.parameterNames().at(i);
        if (paramName == "parent" && metatype == QMetaType::fromType<QObject>()) {
        } else {
            auto typeName = QString::fromUtf8(metatype.name());
            auto pointer = d->diManager->internalRequiredService("IInnerService");
            args.append(pointer);

            tDebug("DIDependentObject")
                << typeName;
        }
    }

    QObject* returnValue = nullptr;
    d->injectConstructor.invoke(nullptr, qReturnArg(returnValue), Q_ARG(QList<tInjectedPointer<QObject>>, args));
    return QSharedPointer<QObject>(returnValue);
}
