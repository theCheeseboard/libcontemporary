#ifndef DIDEPENDENTOBJECT_H
#define DIDEPENDENTOBJECT_H

#include <QObject>

struct DIDependentObjectPrivate;
class tDIManager;
class DIDependentObject : public QObject {
        Q_OBJECT
    public:
        explicit DIDependentObject(QMetaObject constructible, tDIManager* diManager);

        virtual QSharedPointer<QObject> getInstance() = 0;

    protected:
        QSharedPointer<QObject> construct();

    signals:

    private:
        DIDependentObjectPrivate* d;
};

#endif // DIDEPENDENTOBJECT_H
