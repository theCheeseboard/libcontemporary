#ifndef DISINGLETONOBJECT_H
#define DISINGLETONOBJECT_H

#include "didependentobject.h"

struct DISingletonObjectPrivate;
class DISingletonObject : public DIDependentObject {
        Q_OBJECT
    public:
        explicit DISingletonObject(QMetaObject constructible, tDIManager* diManager);

        QSharedPointer<tDIBaseInterface> getInstance();

    signals:

    private:
        DISingletonObjectPrivate* d;
};

#endif // DISINGLETONOBJECT_H
