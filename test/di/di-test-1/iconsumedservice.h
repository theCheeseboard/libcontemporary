#ifndef ICONSUMEDSERVICE_H
#define ICONSUMEDSERVICE_H

#include <QObject>

class IConsumedService {
        Q_GADGET
    public:
        virtual QString helloWorld() = 0;
};

#endif // ICONSUMEDSERVICE_H
