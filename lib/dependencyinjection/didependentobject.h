#ifndef DIDEPENDENTOBJECT_H
#define DIDEPENDENTOBJECT_H

#include "tdibaseinterface.h"
#include <QObject>

struct DIDependentObjectPrivate;
class tDIManager;
class DIDependentObject : public QObject {
        Q_OBJECT
    public:
        explicit DIDependentObject(QMetaObject constructible, tDIManager* diManager);

        virtual QSharedPointer<tDIBaseInterface> getInstance() = 0;

    protected:
        QSharedPointer<tDIBaseInterface> construct();

    signals:

    private:
        DIDependentObjectPrivate* d;
};

#endif // DIDEPENDENTOBJECT_H
