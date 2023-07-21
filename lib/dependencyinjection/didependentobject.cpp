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

    if (d->constructible.constructorCount() > 1) {
        for (auto i = 0; i < d->constructible.constructorCount(); i++) {
            //            if (d->constructible.constructor(i).tag())
        }
    } else if (d->constructible.constructorCount() == 1) {
        d->constructor = d->constructible.constructor(0);
    }
    if (!d->constructor.isValid()) {
        // The constructible has no valid constructor
        tCritical("DIDependentObject") << "Class " << constructible.className() << " registered for dependency injection has no constructors";
    }
}

QSharedPointer<QObject> DIDependentObject::construct() {
    QList<QMetaMethodArgument> args;
    QList<QSharedPointer<QObject>> createdPointers;
    auto arg = Q_ARG(tInjectedPointer<QObject>, tInjectedPointer<QObject>(nullptr));

    for (auto i = 0; i < d->constructor.parameterCount(); i++) {
        auto metatype = d->constructor.parameterMetaType(i);
        auto paramName = d->constructor.parameterNames().at(i);
        if (paramName == "parent" && metatype == QMetaType::fromType<QObject>()) {
        } else {
            auto typeName = QString::fromUtf8(metatype.name());
            auto created = metatype.create(nullptr);
            auto pointer = d->diManager->internalRequiredService("IInnerService");
            tInjectedPointer<QObject>(pointer).propagateTo(created);
            //            createdPointers.append(pointer);
            args.append(QMetaMethodArgument{nullptr, metatype.name(), &created});

            tDebug("DIDependentObject")
                << typeName;
        }
    }

    QObject* returnValue = nullptr;
    if (args.length() == 0) {
        //        d->constructor.invoke(nullptr, qReturnArg(returnValue));
        //        d->constructor.invoke(nullptr, QtPrivate::Invoke::returnArgument(QString::fromUtf8(d->constructible.className()).append("*").toUtf8().data(), returnValue));
        returnValue = d->constructible.newInstance();
    } else if (args.length() == 1) {
        //        d->constructor.invoke(nullptr, qReturnArg(returnValue), args.constFirst());
        returnValue = d->constructible.newInstance(args.first());
    } else if (args.length() == 2) {
        returnValue = d->constructible.newInstance(args.at(0), args.at(1));
    }
    return QSharedPointer<QObject>(returnValue);
}
