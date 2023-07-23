#ifndef IINNERSERVICE_H
#define IINNERSERVICE_H

#include <QMetaObject>
#include <QString>
#include <dependencyinjection/tdibaseinterface.h>

class IInnerService : public tDIBaseInterface {
        Q_GADGET
    public:
        virtual QString helloWorld() = 0;
};

#endif // IINNERSERVICE_H
