#ifndef ICONSUMEDSERVICE_H
#define ICONSUMEDSERVICE_H

#include <QObject>
#include <dependencyinjection/tdibaseinterface.h>

class IConsumedService : public tDIBaseInterface {
        Q_GADGET
    public:
        virtual QString helloWorld() = 0;
};

#endif // ICONSUMEDSERVICE_H
