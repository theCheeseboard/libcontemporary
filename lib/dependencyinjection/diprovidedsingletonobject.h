#ifndef DIPROVIDEDSINGLETONOBJECT_H
#define DIPROVIDEDSINGLETONOBJECT_H

#include "didependentobject.h"
#include <QObject>
#include <QSharedPointer>

struct DIProvidedSingletonObjectPrivate;
template<typename T> class DIProvidedSingletonObject : public DIDependentObject {
    public:
        explicit DIProvidedSingletonObject(QMetaObject constructible, tDIManager* diManager, T& object) :
            DIDependentObject(constructible, diManager),
            object(object) {
            pointer = QSharedPointer<tDIBaseInterface>(dynamic_cast<tDIBaseInterface*>(&object));
        };

        ~DIProvidedSingletonObject() = default;

        // DIDependentObject interface
    public:
        QSharedPointer<tDIBaseInterface> getInstance() {
            return pointer;
        };

        T& object;
        QSharedPointer<tDIBaseInterface> pointer;
};

#endif // DIPROVIDEDSINGLETONOBJECT_H
